# Decision log

## D1: Unit of work = one CMake library
Rationale: qor_ lib is shared, incremental-rebuildable, and matches ctags granularity.
Evidence: 60 leaf libs found by set(target qor_*) scan.

## D2: ctags per lib, not global
Rationale: ~250k LOC; global index too big for context. Per-lib keeps index <12k tokens.

## D3: Verification = compiler as judge
Rationale: user directive. Use cmake --build --target qor_X (incremental). Record in verification.md.

## D4: STATE.MD / TODO.md re-read every turn
Rationale: user directive; prevents context overflow on 250k LOC codebase.

## D5: Duplicate qor_http (http_old) — RESOLVED (user)
Rule: protocols/http_old is NOT built. It is redundant reference code; the target name qor_http collides but only protocols/http is compiled.
Action: ignore src/components/protocols/http_old entirely in repo map, ctags, and build. Only src/components/protocols/http (qor_http) is live.

## D6: header-only core modules — RESOLVED
Rule (confirmed by scan): a source dir with NO CMakeLists.txt = header-only.
All 8 named in docs/structure.md are HDR-ONLY, at these actual locations:
  assert          src/qor/tdd/assert            (+proposition, subject, util)
  datastructures  src/qor/essentials/datastructures (+frozen, bits, detail)
  factory         src/qor/memory/factory
  instance        src/qor/memory/instance
  mock            src/qor/tdd/mock              (+detail, exceptions, parameters, printing, repository, returnvalue)
  profiling       src/qor/tdd/profiling
  reference       src/qor/memory/reference
  test            src/qor/tdd/test              (+detail)
Other HDR-ONLY (not in structure.md): essentials/{iterators,mpl+sub,objectcontext,traits}, text/{buffers,codepage,iterators,strings}, memory/{allocators,exceptions,sources}, reflection(+detail).
Only src/qor LIBS: module, property, function(=qor_tef), memory/{injection,thread}, essentials/{current,reporting,text,textshaping,unicode}, flyers/{error,interception,issue,log}.
Note: sync is src/framework/parallel/sync, NOT src/qor — structure.md mislabels its location.
## D7: ctags availability — RESOLVED (supersedes earlier hand-index assumption)
Re-check finding: Universal Ctags 6.1.0 IS on box. Installed via WinGet; the executable is named ctags.exe, so the earlier `where.exe ctags` probe missed it, but Get-Command 'ctags.exe' -> C:\Users\mfaithfull\AppData\Local\Microsoft\WinGet\Links\ctags.exe. Verified: --version -> "Universal Ctags 6.1.0(v6.1.0) ... +win32,+regex,+pcre2,...".
Corrected rule: use ctags to index EVERY lib (any size) — no hand-indexing needed.
Canonical per-lib cmd:
  & "C:\Users\mfaithfull\AppData\Local\Microsoft\WinGet\Links\ctags.exe" --languages=C,C++ --recurse --tag-relative -f docs/agent/ctags/<lib>.tags <src-lib-dir>
Universal Ctags enables C++ kinds by default (file, class, struct, union, function, functionPrototype, macro, namespace, member, typedef, enum, ...).
qor_module.tags (hand-emitted, 26 lines / 23 symbols) is already valid ctags format; regenerate with ctags when convenient.

## D8: no ctest — RESOLVED (user, 20 Oct 2026)
Rule: the QOR project does not use ctest. Do not reference ctest in plans, commands, docs, or
test CMakeLists. Test judge = run the test exe from its build folder and read its `[OK](n/n)`
output plus the shell exit code (0 = all passed). Therefore a QOR test CMakeLists carries no
`enable_testing()` and no `add_test()` — that is also the dominant convention (17 of 21 test
CMakeLists already omit them; only yaml/json-generator/jsondata-era targets added them, and
`test/framework/data/mime` now omits them too).

Contract preserved: grep docs/agent/ctags/<lib>.tags -> tag<TAB>file<TAB>/regex/;<TAB>linenum -> jump to file+offset; NEVER re-read a source file once its lib is indexed.

## D9: performance verification needs a per-parse timer + a noise floor — RESOLVED (22 Oct 2026)
Rule (measured; FINAL entry = `docs/agent/verification.md` "qor_jsondata parse-perf FINALIZATION (round 18)"):
1. Judge parse performance ONLY with a timer wrapped around the parse call itself. In the committed harness that
   is `test/components/formats/json/main.cpp` lines 284-305, which prints `[perf] 2MB JSON parse (cold): N ms`
   and `[perf] 2MB JSON parse (warm): N ms` for fixture `test/data/data_2mb.json` (2,219,028 bytes). There is NO
   `PARSE_TIME_MS` env mode and no `[JSON parse time]` line in the committed harness (grep of main.cpp finds
   neither), and the handler is `DefaultLogHandler logHandler(log::Level::Informative)` (main.cpp line 82), so
   run logs are 7 non-blank lines / 1332-1336 bytes in Debug AND RelWithDebInfo — nothing is dominated by a `log::debug` flood.
   The framework `Profile:` line is whole-run, not the parse metric.
2. Quote a delta only if it exceeds the noise floor: <=0.6% between runs of ONE binary (cold vs warm sample),
   but **13.5% across independent builds of IDENTICAL sources** (the current tree measured 1875.4 / 1881.9 /
   2128.5 ms means in three separate builds; git-HEAD spread 11.8%; MSVC code layout moves the hot loop across
   cache lines and MSBuild recompiles different object sets - build logs 132 vs 154 non-blank lines). Anything
   under ~14% measured across different builds is not evidence.
3. FINAL baseline, fixture `test/data/data_2mb.json` (2,219,028 bytes = 2.12 MiB), `QORTestJSON`
   RelWithDebInfo in `cmakebuild_rel` (21 runs, 7 builds; logs `tmp/json_final_rel_{A,A2,E,F,D,D2,E2}_run{1,2,3}.log`,
   deleted from `tmp/` 22 Oct 2026 - the values below are the recorded readings):
   **git-HEAD sources 2110 ms mean (18 samples, per-build means 1959.6-2188.6); current tree with the three
   micro-edits 1879 ms mean (12 samples, builds 1875.4/1881.9; a third build of the same sources,
   `json_perf_rel_now.log` (since deleted), measured 2128.5 ms mean); mask-only 2259 ms; mask-less 2255 ms.**
   Debug with the current tree: 54.6-54.8 s (`json_perf_debug_now.log`, since deleted). The earlier "2MB ~56 s → ~18 s"
   figure does not reproduce and is void - no config ever measured ~18 s. The round-16 figures (1.93 s / 2.14 s
   on a 2,102,067-byte `big_json.txt`, 53.2 s / 55.1 s Debug) are unreproducible: that fixture, the `tmp/pristine_qorjson`
   tree and the `PARSE_TIME_MS` mode are absent from this tree.
4. Consequence: the three micro-edits (RTTI removal, `#ifndef NDEBUG` logging guards, podbuffer power-of-2 masking)
   are **performance-neutral within the noise floor**. The apparent A → D gain (-11.0%) is SMALLER than the
   same-source build spread (13.5% for D, 11.8% for A), and single-edit attribution is self-contradictory (mask-only and mask-less builds both measure ~2.26 s,
   +7% vs HEAD, while the combination measures 1.88 s), so the earlier "mask alone is an 11% regression (2.137 s →
   2.377 s)" claim does NOT reproduce. The `% m_size` → `& m_wrapMask` decision is therefore not a performance call:
   keep it (it bounds every index read, and the `assert` removal stays) or revert it purely on review grounds -
   recommend keeping, with no performance credit claimed either way.
   **Recorded decision 22 Oct 2026: all three micro-edits are KEPT** in the tree; the structural phase (TODO 15) is
   DEFERRED until greater resources are available; `tmp/` pruned to the round-18 evidence (45 files / 300,687 B
   deleted - see the cleanup bullet in the FINAL verification entry).
5. RTTI removal is INCOMPLETE while model nodes are built through `AnyObject::Cast<Fastflow>`
   (`typedany.h:158` `CastToBase_` does a live `dynamic_cast` per node); it only disappears in
   NDEBUG builds. The structural phase must remove the `AnyObject` hop to make the win real.

## D10: Threadpool callback-environment API is not bindable in the kernel api_layer — RESOLVED (26 Oct 2026)
Rule: InitializeThreadpoolEnvironment, DestroyThreadpoolEnvironment and the four SetThreadpoolCallback functions
(InitializeThreadpoolEnvironment, DestroyThreadpoolEnvironment, SetThreadpoolCallbackCleanupGroup / Library / Pool / RunsLong)
stay DECLARED in kernel32.h for interface completeness but get NO wrapper in winqor_api_kernel. They are not a
gap to fill - there is nothing to bind, so a wrapper would be dead code that cannot link.
Evidence (dumpbin /exports, 26 Oct 2026, System32 AND SysWOW64 copies of both DLLs, files tmp/_tmp_k32_exports_now.txt,
tmp/_tmp_k32wow_exports_now.txt, tmp/_tmp_ntdllwow_exports_now.txt): kernel32.dll exports exactly 30 threadpool names
and none of them is one of these 6; ntdll.dll exports none of them under the public names and none under a Tp* name
either - it exposes only the private threadpool core (TpAllocPool, TpReleasePool, TpSetTimer, TpPostWork, TpIsTimerSet,
TpCallbackMayRunLong, TpStartAsyncIoOperation, TpWaitForWork, ...). The pre-existing note in kthreadpool.cpp claimed
"they are ntdll.dll exports"; that is wrong and was corrected in place, along with the cross-reference in kprocess.cpp.
Audit: tmp/_tmp_audit4.cpp extended with a documented non-export list and rebuilt as tmp/_tmp_audit6.exe.
tmp/_tmp_audit_v6.txt now reads SUMMARY implemented=362 documented_nonexport=6 missing=0 total=368 (v5 read missing=6).
Declared count and impl token count are unchanged by the added header comments (368 / 362), so the audit parse is stable.

### D10-CORRECTION (same day, 26 Oct 2026) - the rationale recorded above was WRONG; the six are header-inline, not unbindable
The Windows SDK headers explain the missing exports, and they contradict two claims above ("no wrapper possible"
and "ntdll exposes the threadpool core only through undocumented Tp* internals").
C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\um\WinBase.h lines 7913-7983 DEFINE all six as
FORCEINLINE VOID shims, e.g. InitializeThreadpoolEnvironment calls TpInitializeCallbackEnviron, SetThreadpoolCallbackPool
calls TpSetCallbackThreadpool, DestroyThreadpoolEnvironment calls TpDestroyCallbackEnviron. winnt.h lines 25607 ff.
then define those Tp* helpers as FORCEINLINE too - they are plain field assignments on TP_CALLBACK_ENVIRON_V1
(winnt.h 25583-25601). Nothing in the chain is a DLL function, so the absence of a kernel32/ntdll export is expected
behaviour, not an anomaly, and "undocumented internals" is wrong - the whole family is documented and in the headers.
Consequence: a wrapper IS possible, but it must bind at compile time. The runtime path used by the other 362 wrappers
cannot reach it, and it fails SILENTLY at run time, not at link time: qor_pp_useswinapi resolves through
Kernel32::GetProcAddress (kernel32.h:50) which returns null for these names, and Library::Call / Library::voidCall
(library.h:58, library.h:80) then throw "Missing library function." - a latent run-time trap, not a compile error.
See D11 for the variant that closes it.

## D11: Variant API wrapper for header-inline Windows API entry points - RULE (26 Oct 2026)
Classification rule: every declared api_layer entry point is exactly one of
  (E) export-bound     -> qor_pp_useswinapi + Library::Call / voidCall, the existing 362 wrappers
  (I) header-inline    -> the SDK header supplies the definition (FORCEINLINE in WinBase.h / winnt.h); the wrapper
                          MUST call it directly at compile time and MUST NOT use Library::DefProc / GetProcAddress.
Decide (E) vs (I) from the SDK header text - grep the Windows Kits include tree for a FORCEINLINE definition of the
name - NOT from dumpbin. Absence of an export is only meaningful once the header is known to provide the definition.

library.h variant, mirroring Call / voidCall but with no import and no null test:
    template< typename ret, class ...MethodArgs >
    static ret inlineCall(ret (*pFn)(MethodArgs...), MethodArgs... args)
    { __try { return (pFn)(std::forward< MethodArgs >(args)...); }
      __except(ExceptionFilterFunction(GetExceptionInformation())) { } ret _{}; return _; }
    template< class ...MethodArgs >
    static void voidInlineCall(void (*pFn)(MethodArgs...), MethodArgs... args)
    { __try { (pFn)(std::forward< MethodArgs >(args)...); } __except(ExceptionFilterFunction(GetExceptionInformation())) { } }
plus a marker macro beside qor_pp_useswinapi (kernel32.h:50):
    #define qor_pp_usesinlineapi( _NAME )   // compile-time binding marker - expands to nothing
used as
    VOID Kernel32::InitializeThreadpoolEnvironment( ::PTP_CALLBACK_ENVIRON pcbe )
    { qor_pp_usesinlineapi( InitializeThreadpoolEnvironment ); Library::voidInlineCall( ::InitializeThreadpoolEnvironment, pcbe ); }
Rationale for keeping the macro: the audit tool and grep-based tooling keep one recognisable binding marker per
wrapper, and the exception-guard semantics stay identical to the export path. The :: prefix is required - the SDK
definitions are at global scope, the QOR declarations live in qor::win::api.

Feasibility is proven, not assumed (probe tmp/_tmp_inline_probe.cpp -> tmp/_tmp_inline_probe.log, 26 Oct 2026):
compiled with cl /nologo /EHsc /std:c++17 /c using ONLY the inherited INCLUDE env, which lists
C:\Program Files (x86)\Windows Kits\10\include\10.0.26100.0\um (that is why the project resolves Windows.h at all -
the MSVC 14.50 include dir has no Windows.h or winbase.h). Result exit 0, 0 errors, and dumpbin /linksymbols on the
object lists ZERO undefined externals for the six names - MSVC emits a static local copy for the address-of on a
FORCEINLINE, so there is still no import and no DLL dependency.

Guard conditions the inline variant must respect (WinBase.h 7905-7991): the definitions sit inside
WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP) and _WIN32_WINNT >= 0x0600; SetThreadpoolCallbackPriority additionally
needs _WIN32_WINNT >= _WIN32_WINNT_WIN7; SetThreadpoolCallbackPersistent sits in the DESKTOP partition only (WinBase.h
7996-8007). Any inline wrapper needs the same guards, or the project must pin _WIN32_WINNT explicitly.

Surface gap found while confirming this: SetThreadpoolCallbackPriority and SetThreadpoolCallbackPersistent exist in the
SDK but are NOT declared in kernel32.h at all - the callback-environ surface in the api_layer is incomplete. That is a
declaration gap, not a missing wrapper.

## D11-IMPLEMENTED: the variant wired in for the six; the two gap declarations added unwrapped (26 Oct 2026)
Implemented, not just designed. library.h gains inlineCall / voidInlineCall beside Call / voidCall (same
__try / __except(ExceptionFilterFunction(GetExceptionInformation())) guard, no DefProc, no null test). kernel32.h gains
#define qor_pp_usesinlineapi( _NAME ) beside qor_pp_useswinapi, expanding to nothing. kthreadpool.cpp gains the six wrappers
- InitializeThreadpoolEnvironment, DestroyThreadpoolEnvironment, SetThreadpoolCallbackPool, SetThreadpoolCallbackLibrary,
SetThreadpoolCallbackCleanupGroup, SetThreadpoolCallbackRunsLong - each one qor_pp_fcontext + Win32ErrorHandler + the marker
+ Library::voidInlineCall with EXPLICIT template arguments, matching how the export wrappers already call Library::voidCall.
kernel32.h also declares the two callback-environ functions that were absent - SetThreadpoolCallbackPersistent (the SDK
defines it only inside WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)) and SetThreadpoolCallbackPriority (needs
_WIN32_WINNT >= _WIN32_WINNT_WIN7). Both are declared, marked header-inline, and left UNWRAPPED on purpose: wiring a
DESKTOP-only entry point into a library that also builds for other WINAPI_FAMILY configurations is a portability call, and
it is the one open decision left for review - the variant makes each of them a one line change if wanted.
Evidence, all 26 Oct 2026: probe tmp/_tmp_inline_probe_v2.cpp -> tmp/_tmp_probe_v2.log compiles all eight call forms with
cl /nologo /EHsc /std:c++20 /c, exit 0, 0 errors, 0 warnings, no undefined Threadpool or Tp* externals. Audit rebuilt as
tmp/_tmp_audit8.exe with the two new names added to doc_nonexport -> tmp/_tmp_audit_v8.txt SUMMARY implemented=368
header_inline_nonexport=2 missing=0 total=370. Target winqor_api_kernel rebuilt exit 0 with 0 error lines in
tmp/_tmp_kernel_build_d11b.log; dumpbin on the fresh kthreadpool.obj shows the six SDK names as DEFINED local symbols
(MSVC static copies of the address-of on a FORCEINLINE) and ZERO undefined Threadpool externals, while the six
qor::win::api::Kernel32 wrappers are defined public symbols. No import, no DLL dependency, as D11 predicted.
Audit run contract, a trap worth keeping: audit8 <kernel.h> <k32dump> <ntdump> [implFile ...] and the impl file paths must
be relative with forward slashes - absolute backslash paths from Get-ChildItem load as zero impl tokens and silently turn
the whole run into MISSING for every name.

## D12: the shell32.dll wrapper layer (winqor_api_shell) - 60 stateless wrappers (26 Oct 2026)

Implemented, not designed. `src/platform/os/windows/api_layer/shell` now carries the whole modern Shell API of
shell32.dll: `shell.h` declares `class qor_pp_module_interface(QOR_WINAPISHELL) Shell` inside `qor::win::api`
with 60 static prototypes grouped by category, and seven implementation files (`shell.cpp` 14, `drag.cpp` 5,
`fileops.cpp` 11, `icon.cpp` 12, `notify.cpp` 7, `recycle.cpp` 6, `shellprops.cpp` 5) define exactly those 60
bodies in the kernel pattern - `qor_pp_fcontext; Win32ErrorHandler _; qor_pp_useswinapi(shell32, Name);
return Library::Call<RET, Args...>(pFunc, args...)`, with `CheckReturn<HRESULT>::TType` for every SDK HRESULT,
`CheckReturn<BOOL>::TType` for every SDK BOOL, `Library::voidCall` for the two void entry points
(`DragFinish`, `SHFreeNameMappings`) and raw `Library::Call` for the value-only families
(`CommandLineToArgvW`, `DragQueryFile*`, `SHAppBarMessage`, `SHGetFileInfo*`, `ExtractIconEx*`, icon handles,
`ShellExecute*`, `FindExecutable*`, `ShellAbout*`, `DuplicateIcon`, `ExtractAssociatedIcon*`).

Why the split is the way it is, in one line each: HRESULT and BOOL are failure-status returns where a failed
result means "the shell did not do it", so the kernel layer's checked-return idiom raises Win32Error
immediately at the call site and the caller never has to re-test; the pointer/handle/UINT families use zero or
a handle value as a legitimate answer (icon counts, path lengths, `SHGetFileInfo` returning the input
`pszPath`), so raising on them would be a lie. A/W variants are wrapped as separate functions because shell32.dll
exports both spellings as distinct symbols - aliasing them would drop half the surface.

Three findings that only a live DLL teaches: (1) `SHTestTokenMembership` is declared `STDAPI_(BOOL)` in shellapi.h,
not `SHSTDAPI_`, and is nevertheless a shell32.dll export - it is in scope and wrapped; (2) the SDK declares seven
more entry points than the locked 60 (`SHEnumerateUnreadMailAccountsA/W`, `SHGetUnreadMailCountA/W`,
`SHSetUnreadMailCountA/W`, `SHGetDriveMedia`) and only the four wide variants exist in the live export table, so
the ANSI trio is dead in this build of shell32.dll - the four live ones are the one open surface decision, four
wrappers of work, deliberately left out to keep the agreed 60-name scope; (3) `REFIID` resolves to the QOR
`UUIDGMP` type when unqualified (QOR re-include ordering) while `::REFIID` is the SDK GUID typedef - the
unqualified spelling is what compiles in this module and is what the three REFIID wrappers take, so a caller
passes a QOR UUID, not a `GUID`. `::HWND`/`::HICON`/`::HINSTANCE`/`::HDROP` stay SDK-qualified because QOR
redefines some of those names inside its own namespace.

Evidence, all 26 Oct 2026: `cmake --build cmakebuild --target winqor_api_shell --parallel 8` exit 0 with zero
`error C`/`fatal error` lines in `tmp/build_shell2.log` (16/16 objects, DLL relinked 26 Oct 2026 09:21).
`dumpbin /imports` on the fresh DLL (`tmp/_tmp_shell_dll_imports.txt`) lists
`winqor_api_kernel.dll qor_error.dll qor_interception.dll qor_issue.dll qor_current.dll qor_host.dll
MSVCP140D.dll VCRUNTIME140_1D.dll VCRUNTIME140D.dll ucrtbased.dll KERNEL32.dll` and no SHELL32.DLL -
GetProcAddress binding, no import library, as designed. Audit `tmp/_tmp_shell_full_audit.py` parses the SDK
header with prototypes joined before matching (line-based matching drops return types - the hazard that bit
the earlier extraction passes) and reports sdk 60 / header 60 / impl 60 with `header ^ impl` empty, no
duplicate definitions, and `impl not exported` empty against `dumpbin /exports` of the live
`C:\Windows\System32\shell32.dll`. `tmp/_tmp_shell_protos3_out.txt` (the earlier 59-name extraction) plus
`SHTestTokenMembership` is exactly the implemented set.


## D13: A/W binding spelling for export-bound api_layer wrappers - RULE (29 Oct 2026)
Rule (user decision, applies to every `user/` translation unit as it is reviewed; `accelerator.cpp` already
converted): a wrapper binds with `qor_pp_useswinapiAW(<mod>, Stem)` whenever the SDK has an A/W family for
`Stem`, and with the bare `qor_pp_useswinapi(<mod>, Stem)` ONLY where the entry point is a single export with no
A/W variants (`DestroyAcceleratorTable`, `ShowWindow`, `GetComboBoxInfo`, `CallNextHookEx`, ...).
`qor_pp_useswinapiAW` (`kernel32.h:51-53`) already exists and maps to `Stem##W` under `qor_pp_unicode`; it is
the spelling that names what `GetProcAddress` is really given, so the source states the binding instead of
depending on an SDK macro surviving into the binding expression.

Why the bare spelling is unsafe even where it works today - measured on `winqor_api_user`, 29 Oct 2026:
`_ATXT(_NAME)` inside `qor_pp_useswinapi` (`kernel32.h:50`) is a nested macro call, so `_NAME` is macro-expanded
before it is stringized. `cl /EP` on the real TU proves it - `qor_pp_useswinapi(user32, CreateAcceleratorTable)`
preprocesses to `Kernel32::GetProcAddress(..., "CreateAcceleratorTableW")`. The SDK's `Stem -> StemW` macro is
what selects the entry point, not the source text. `UNICODE`/`_UNICODE` is set from `qor_pp_unicode` at
`kernel32.h:7-10`, so the QOR switch and the SDK switch are coupled only through include order (`user32.h:7`
pulls `kernel32.h` in before `<windows.h>`).

Two failure classes the bare spelling hides, both confirmed against the live `C:\Windows\System32\user32.dll`
export table (`tmp/_tmp_user32_exports.txt`, 1018 names) plus a `GetModuleHandleW`/`GetProcAddress` probe
(`tmp/_tmp_name_resolve_probe.cpp` run by `tmp/_tmp_run_probe.bat`):
  (a) `user32.h:9-17` `#undef`s nine stems (`RegisterDeviceNotification`, `WinHelp`, `SystemParametersInfo`,
      `GetRawInputDeviceInfo`, `DlgDirList`, `DlgDirSelectEx`, `DlgDirListComboBox`, `DlgDirSelectComboBoxEx`,
      `LoadBitmap`). After the `#undef` the literal is what resolves, and the literal is NOT exported while the
      `W` is (probe: `WinHelp` MISSING / `WinHelpW` RESOLVED, and the same shape for all nine). A bare binding of
      any of them throws `"Missing library function."` at run time - `combobox.cpp:20,27` was exactly this until its
      own session (session 6, 29 Oct 2026) converted both to `qor_pp_useswinapiAW`.
  (b) a binding that carries the QOR wrapper's own `T` suffix stringizes literally: `GetDlgItemTextT`,
      `VkKeyScanT`, `VkKeyScanExT`, `GetKeyboardLayoutNameT` - none exported, so those throw too
      (`dialog.cpp:89`, `keyboard.cpp:82,243,250`).
Audit corollary: grepping the source spelling is not evidence about run-time binding. A list of "bindings to
names absent from the export table" has to come from `cl /EP` output of the TU, not from the text. The earlier
`tmp/_tmp_audit_report.txt` section A is wrong for every name whose SDK A/W macro is still in scope
(`CreateAcceleratorTable`, `LoadAccelerators`, `MessageBox`, `TranslateAccelerator`, `SendMessage`, ... all bind).

Module-token case is cosmetic, not a defect: `_TXT(User32)` -> `L"User32"` and the module lookup is
case-insensitive - `GetComboBoxInfo` resolved against `User32`, `USER32` and `user32` alike. `combobox.cpp` /
`scrollbar.cpp` spelling the token `User32` is a style break to fix in those TUs' own sessions. `combobox.cpp`'s three
sites were fixed in session 6; `scrollbar.cpp`'s twelve remain.

Addendum - wrapper naming alongside the binding spelling (29 Oct 2026, session 6, user decision): a wrapper that binds
an A/W family also carries the folder's `T` suffix (SDK stem + `T`), so the name and the binding agree about what is
switchable. The first two offenders were renamed inside their own TU's session - `User32::DlgDirListComboBox` ->
`DlgDirListComboBoxT` and `DlgDirSelectComboBoxEx` -> `DlgDirSelectComboBoxExT` (`user32.h:551-552` plus the
definitions); neither had a call site anywhere in `src`/`examples`/`test`, so the rename was behaviour-neutral.
Measured with `tmp/_tmp_tsuffix_census.ps1`, 11 further A/W members are still declared without the suffix (only 4 of
them implemented) and 2 carry the suffix with no A/W family (`CreateWindowT` `user32.h:144`, `CopyCursorT` `:383`).
Those wait for one folder-wide call rather than per-TU churn; tracked as open item U17 in
`src/platform/os/windows/api_layer/user/status.md`.

## D14: an exported wrapper with no reachable in-tree caller is not a review finding - POLICY (2 Nov 2026)

Rule (user decision; takes effect at the `api_layer/user` review's session 23 and in any later library that inherits its
register): **open item U15 is CLOSED without a fix, and its instances are no longer recorded.** A wrapper that is
exported through `qor_pp_module_interface(QOR_WINAPIUSER)`, fully defined, linked and callable, but has no caller
anywhere in `src`/`examples`/`test`, is published surface rather than a defect - whether the tree exercises it is a
consumer-side (`winqor_gui`) question, and this review's scope is the correctness of the layer itself. The 23 reviewed
TUs measured the shape repeatedly (`caret.cpp` 8/8, `clipboard.cpp` 20/20, `keyboard.cpp` 29/34, `icon.cpp` 12/14,
`mdi.cpp` 4/4 ...), so the tally had stopped telling the audit anything it did not already know.

What changes in the protocol:
- method step 5 still finds the in-tree caller and names the consumer one level deep, but an unreachable wrapper
  produces no record: no per-TU tally, no session-log column, nothing appended to `registers/u15.md`, whose
  sessions-1-22 instances stay in place as frozen history.
- reachability is still measured wherever another item's severity turns on it - whether a P1/P5 mis-binding is latent
  or live, U18's divergent `TrackPopupMenu` parameter, U19's LAYER-vs-BYPASS split - and the fact is recorded in that
  item, not under U15. This is what session 8 proved: the `coordtransform.cpp` wrappers are live, so its `GetDCEx`
  mis-binding was a shipped defect rather than a dead-code curiosity.
- Not changed by this decision: **U4** and tasks **T1/T3/T4/T5** are declared-without-definition, so a caller gets
  `LNK2019` - a link-time landmine, not a reachability question - and all stay open; **T2** ("the whole Window Station /
  Desktop section is dead exported API") stays open because its point is the conventions of a section no caller can
  exercise, not the deadness; **U19** (entry points reached through the SDK import thunk instead of this layer) is a
  different question and keeps accumulating.

Record of the closure: the entry head in `src/platform/os/windows/api_layer/user/registers/u15.md`, the "Convention
decision" bullet and locator bullet in that folder's `status.md`, `method.md` step 5, `registers/README.md` (u15 dropped
from the accumulating entries in `tmp/_tmp_status_index_gen_s22.ps1`) and `registers/census.md`. **No source file, header
or CMake file was touched** - the policy stops recording and changes no behaviour, so the review's gate (38/38 at `/W4`)
is unaffected.


## D15: the `Library::Call<>` template-argument spacing split (U14) is closed as a review item - POLICY (3 Nov 2026)

Rule (user decision at the `api_layer/user` review's session 28, `rectangle.cpp`): **open item U14 is CLOSED without a
folder-wide fix and its tally is frozen; nothing is appended to `registers/u14.md` any more.** The item counted two
things - (a) `Library::Call< T, ... >` written with angle-bracket padding versus the folder's tight
`Library::Call<T, ...>`, and (b) sites that leave the argument types to deduction instead of spelling them - and after 28
reviewed TUs both halves are measured to exhaustion: **163 spaced / 265 tight, 425 explicit / 3 deducing**
(`tmp/_tmp_u14_post_s28.txt`). 133 of the 163 spaced sites are in TUs already reviewed under the standing rule "normalise
only where the line is being edited anyway", so the item can no longer be settled TU by TU: finishing it needs a
folder-wide mechanical pass, which is a task, not a per-TU finding.

Why it is safe to freeze: the spacing never changes what the compiler produces. Every session that touched a call site
proved it with `cl /EP` neutrality - session 28's pass (`tmp/_tmp_rect_neutral_s28.txt`) left the non-blank pre/post
multiset identical at 154,338 lines with only the 11 intended call lines differing, and 11/11 binding literals
byte-identical; the library compiled 38/38 at `/W4` with no diagnostics after it. Two sessions normalised their own TU
deliberately under user decisions (session 27 `processthread.cpp` 5 sites, session 28 `rectangle.cpp` 11 sites); no other
session touched spacing.

What changes in the protocol:
- a session no longer reports a spaced/tight tally for its TU and appends nothing to `registers/u14.md`; the frozen
  folder number above is quoted from `registers/census.md` when it matters.
- the 3 argument-deducing sites (`window.cpp:118`, `:254`, `:317`) do not get a new item - they are named as a hand-off
  in the review's queue note and are recorded and fixed inside `window.cpp`'s own session, where U18's `void` converters
  and P3's `CreateWindowT` are already waiting.
- **not** closed by this decision: **U25** (`const`/`CONST::`/`CONST ::`/bare `CONST X` spelling, which the same session
  proved is four populations, not three, and which keeps accumulating), and the per-TU `qor_pp_fcontext` presence check,
  which stays in every TU narrative.
- if the mechanical spacing pass is ever wanted, it becomes a new numbered entry in `registers/tasks.md` - not a reopened
  U14.

Record of the closure: the closing paragraph in `src/platform/os/windows/api_layer/user/registers/u14.md`, the "Convention
decision" bullet and locator bullets in that folder's `status.md`, `registers/README.md` and
`tmp/_tmp_status_index_gen_s22.ps1` (u14 dropped from the accumulating-entry list), `registers/census.md`. The policy
itself touches no source, header or CMake file; the only code edits are the 11 normalised call sites of session 28, which
passed the review's gate.

## D16: the T7 dead-include sweep is executed - ACTION (5 Nov 2026)

Decision (user, 5 Nov 2026): **execute T7** - delete `#include "../returncheck.h"` from every `api_layer/user` source
that uses nothing from it. T7 was registered on 3 Nov 2026 as a deferred task precisely because the fix is mechanical
and only the go-ahead was missing; this is that go-ahead, taken once for the folder instead of 38 times TU by TU.

Measured with the task's own instrument (`tmp/_tmp_rcinclude_scan.ps1`). Before: `sources scanned: 38`,
`includes ../returncheck.h: 37`, `TUs with a real code hit: 2`, `include-without-use: 35`. After: **`includes: 2`,
`include-without-use: 0`** - the two survivors are the only consumers, `processthread.cpp` (`CheckReturn` x3 at `:85`,
`:94`, `:110`) and `errorhandling.cpp` (`CheckReturn` x1 at `:81`); the glue `winqor_api_user.cpp` never included it.
`git diff --numstat` over the folder: **35 files, each exactly one line down** - 27 pure `0 1`, and 8 that also carry a
one-line comment fix at `1 2` or `2 3` - 0 net change beyond that, and no source line above 120 columns.

**Instrument defect found running the gate:** the scan's include test was not line-anchored, so it also counted the
three TUs whose review banner *quotes* `#include "../returncheck.h"` inside a `//` comment (`windowclass.cpp`,
`windowprocedure.cpp`, `windowproperties.cpp`) - an un-anchored post-fix run printed `includes: 5 / dead: 3`. The
anchored test (`^\s*#include\s+"\.\./returncheck\.h"\s*$`) prints `2 / 0`; quote only the anchored form, and note that
the register's expected post-fix numbers were always the anchored ones.

Gate (unchanged from the register): the whole folder in one `cl` invocation at `/W4` - **exit 0, 38 `.cpp` echoes in the
log, 0 diagnostics, and every source's `.obj` in `tmp/obj` newer than the source**. Include-and-comment only: no ABI, no
published-surface change, `user32.h` untouched, so the 106-source all-libraries gate is not in play.

Why it is safe to take: `../returncheck.h` declares only the 14 check templates in `qor::win::api`, and `../library.h` -
which every wrapper TU includes - already pulls `src/qor/flyers/error/error.h`, so nothing a hit-free TU was using
disappears; the header's include guard means a TU that later wants a check adds one line back. The settled U1/U2 policy
(no `CheckReturn` in this library, sessions record only) is untouched by this: G6's "the header is what would make a
future check compile" argues for re-adding it where a check lands, not for keeping 35 unused copies.

**Consequence recorded, deliberately not repaired in this pass:** deleting line 11 moves every line below it up by one,
which invalidates the bare `:NNN` self-citations inside the in-source comment blocks of those 35 files. Measured with
`tmp/_tmp_t7_cites.ps1`: **703 bare citation tokens with `N >= 12` across the 35 files - 388 resolve past the end of
their own file** (they name another file's lines and do not move), **242 resolve to a code line** and **73 to a comment
line** of their own file. Only the last two groups are candidates, and separating genuine in-file citations from ones
that inherit a header named earlier in the paragraph is the paragraph-level rule in
`src/platform/os/windows/api_layer/user/gotchas/evidence-integrity.md` - a semantic pass over 30 files, i.e. a task in
its own right, not folded into T7. Until that pass runs, a bare citation below line 11 in one of these 35 files reads as
**one lower** than written. The five TUs with in-source review banners (`text`, `window`, `windowclass`,
`windowprocedure`, `windowproperties`) are where it matters most, since protocol v3 makes the banner the record.

Record of the action: the T7 entry head in
`src/platform/os/windows/api_layer/user/registers/tasks.md`, the closed/open id lines and the queue `lines` column in
that folder's `status.md`, and its `preflags.md`. No header, CMake file or file outside that folder's 35 sources changed.

## D17: Windows support is 64-bit (amd64) only - no 32-bit x86 target - POLICY (5 Nov 2026)

Decision (user, 5 Nov 2026): **there will be no 32-bit x86 (Win32) target.** Recorded in-tree at
`src/platform/os/windows/purpose.md:3` ("Windows support is 64-bit (amd_x64) only. No 32-bit (x86) support"). It
formalises what the build already does: `CMakeSettings.json:4` carries the single `x64-Debug` profile with
`inheritEnvironments: [ "msvc_x64_x64" ]` at `:7`. It is a **scope** decision, not a build change - nothing in the
reviewed `api_layer/user` folder is architecture-conditional at all. Measured 5 Nov 2026 over that folder's 38 `.cpp` and
over `user32.h`: **0 `#ifdef _WIN64` / `_M_IX86` / `_X86_` arms**; the only `_WIN64` tokens in the folder are sentences
inside `windowclass.cpp`'s review banner (`:42`, `:53`) describing the SDK's own guards.

What it settles in the `api_layer/user` review record (`src/platform/os/windows/api_layer/user/`):

## D18: winqor_api_user's purpose is a full wrapper set over user32.dll's exports - a wrapper's signature mirrors the export - POLICY (5 Nov 2026)

Decision (user, 5 Nov 2026): the folder now carries a purpose statement,
`src/platform/os/windows/api_layer/user/purpose.md` - "winqor_api_user intends to provide a full set of wrapper functions
for the exports of the User32.dll Windows system library". Recorded as a decision because it changes the review's
acceptance criterion for that folder: a wrapper is judged against **the export it binds**, and an unwrapped export is a
**gap**, not a scope choice. It also fills the folder's last `purpose.md` hole - the four sibling folders `api_layer/`,
`api_layer/gdi`, `api_layer/kernel` and `api_layer/shell` already had one, per the rule at
`memory-bank/systemPatterns.md:16` ("every new folder needs `purpose.md`").

What it settles in the `api_layer/user` record:

- **U18 stops being a deferred surface decision and becomes conformance work** (`registers/u18.md`). Its divergences were
  left unfixed on two grounds: "published signature, therefore a user decision", and "same width on x86 and x64, so no
  ABI effect" - the second already removed by D17. Under the purpose statement the direction is fixed: **a wrapper's
  published signature equals the SDK prototype for the export it binds** (return type, parameter types, const-ness,
  parameter names), changed in `user32.h`, in the definition and in the `Library::Call<>` list together. The executable
  plan is **T8** in `registers/tasks.md`.
- **Coverage becomes measurable, and is measured** (5 Nov 2026, instrument `tmp/_tmp_u18_coverage4.ps1` over the captured
  export table and `winuser.h` 10.0.26100): **1,018 export names** (1,014 ordinal+RVA rows + 4 forwarded rows - the split
  already recorded in `gotchas/instrument-flags.md`), **525 members** declared in `user32.h`, **428 bindings** across the
  38 TUs, **648 exports matched** by a member name after A/W and `T` normalization, **117 SDK-declared exports with no
  wrapper at all**, and **253 unwrapped exports that have no `winuser.h` prototype** (undocumented / internal).
- **U4's direction is completion, not deletion**: where a declared-but-undefined member names a real export (T1's
  `Get`/`SetUserObjectSecurity`, T3's power pair), the stated intent points at adding the definition, not removing the
  declaration.

What it does **not** decide, and which T8 step 0 asks before anything executes: whether the 253 undocumented exports are
inside the "full set"; whether both `A` and `W` variants must be wrapped or the `W` form plus a `T` alias suffices (that
is U17's suffix policy); and how superseded exports (`keybd_event`, `mouse_event`, the DDE family) are dispositioned.

Record: the D18 status block in `registers/u18.md`, the new **T8** in `registers/tasks.md`, the `purpose.md` pointer and
id lines in that folder's `status.md` and `preflags.md`, and one line in the api_layer section of
`memory-bank/systemPatterns.md`. No source, header or CMake file is touched by this decision.


- **U28 is CLOSED by this decision** (`registers/u28.md`). The four `*Ptr` members (`windowclass.cpp:77`, `:119`, `:140`,
  `:147`, binding `Get/SetWindowLongPtrW` and `Get/SetClassLongPtrW`) name entry points the 32-bit `user32.dll` image does
  not export. That is now simply the correct spelling for the only image the project targets, so the wrappers stay exactly
  as they are and the arch-conditional binding that entry contemplated as the fix is out of scope. The x64 leg of the
  session-34 probe (16/16 names RESOLVED) is the leg that describes the supported target.
- **U6's x86 blocker is withdrawn as a blocker** (`registers/u6.md`, session-31 addendum (b)). `kernel32.h:92` names
  `PUNWIND_HISTORY_TABLE`, which the SDK typedefs with no `_X86_` arm, so 93 of 106 `api_layer` TUs fail an x86 compile with
  `C2061`. That line stays as it is, and the suggested kernel-layer follow-up recorded in that entry ("give
  `RtlLookupFunctionEntry`'s third parameter a type the SDK defines on every arch, or guard the declaration") is **not to
  be actioned**. The measurement keeps its value: it is the reason x64-only gates cannot see arch-dependent defects, which
  is why U6's stack corruption needed a dedicated probe and why the `WINAPI`-on-a-member `C2373` pair survived 30 sessions.
- **x86 compile legs stop being a step anywhere.** The guidance at `gotchas/sessions-31-33.md:13-16` to add an x86 compile
  when touching a `qor_pp_uses*` / definition-head shape is retired, together with the x86 legs of the review's two-arch
  instruments indexed in `tooling.md` (the `Message`/`MSG`, `MONITORINFOEX`, `SCROLLINFO` and `WINDOW*` mirror probes, the
  `DrawState`/`GrayString` live probe, U6's calling-convention probe). No folder's gate includes an x86 compile. Frozen
  history (`census.md`, `archive/`, `gotchas/sessions-*.md`) keeps the x86 numbers as measured and is not edited.
- **x86-only exposure is no longer exposure.** U26's `Message`/`MSG` caller mirror is field-identical at 48 bytes on x64 and
  divergent only on x86 (40 vs 28 bytes from `lParam` on), so on the supported arch the cast is sound: the live defects U26
  carries are the published `winqor_gui` headers' uninitialized fields (`ScrollInfo::nPos`/`nTrackPos`,
  `WindowPlacement::length`), not any layout. U18's "same width on x86 and x64, so no ABI effect" notes and U16's
  `_WIN64`-guarded-prototype notes need no follow-up under this decision.

What it does **not** buy is an x86 build, or a clear failure when someone attempts one. An x86 compile still dies at
`kernel32.h:92` with a misleading `C2061 'PUNWIND_HISTORY_TABLE'` in 93 of 106 `api_layer` TUs, the 13 survivors being
exactly the library glue TUs. An arch guard (an `#error` in the Windows platform headers, or a CMake arch check) would turn
that into a one-line message, but it is a `cmake/`/kernel-header change of the same class as the version-string hazard
session 37 left unregistered at `src/platform/os/windows/api_layer/user/preflags.md`, so this pass deliberately registers
neither.

Record: the closure line in `registers/u28.md`, the status line on the session-31 addendum in `registers/u6.md`, the
architecture bullet and id lines in that folder's `preflags.md` and `status.md`, and a Windows-target bullet in
`memory-bank/techContext.md`. No source, header or CMake file is touched by this decision.
- **x86 compile legs stop being a step anywhere.** The guidance at `gotchas/sessions-31-33.md:13-16` to add an x86 compile
  when touching a `qor_pp_uses*` / definition-head shape is retired, together with the x86 legs of the review's two-arch
  instruments indexed in `tooling.md` (the `Message`/`MSG`, `MONITORINFOEX`, `SCROLLINFO` and `WINDOW*` mirror probes, the
  `DrawState`/`GrayString` live probe, U6's calling-convention probe). No folder's gate includes an x86 compile. Frozen
  history (`census.md`, `archive/`, `gotchas/sessions-*.md`) keeps the x86 numbers as measured and is not edited.
- **x86-only exposure is no longer exposure.** U26's `Message`/`MSG` caller mirror is field-identical at 48 bytes on x64 and
  divergent only on x86 (40 vs 28 bytes from `lParam` on), so on the supported arch the cast is sound: the live defects U26
  carries are the published `winqor_gui` headers' uninitialized fields (`ScrollInfo::nPos`/`nTrackPos`,
  `WindowPlacement::length`), not any layout. U18's "same width on x86 and x64, so no ABI effect" notes and U16's
  `_WIN64`-guarded-prototype notes need no follow-up under this decision.

What it does **not** buy is an x86 build, or a clear failure when someone attempts one. An x86 compile still dies at
`kernel32.h:92` with a misleading `C2061 'PUNWIND_HISTORY_TABLE'` in 93 of 106 `api_layer` TUs, the 13 survivors being
exactly the library glue TUs. An arch guard (an `#error` in the Windows platform headers, or a CMake arch check) would turn
that into a one-line message, but it is a `cmake/`/kernel-header change of the same class as the version-string hazard
session 37 left unregistered at `src/platform/os/windows/api_layer/user/preflags.md`, so this pass deliberately registers
neither.

Record: the closure line in `registers/u28.md`, the status line on the session-31 addendum in `registers/u6.md`, the
architecture bullet and id lines in that folder's `preflags.md` and `status.md`, and a Windows-target bullet in
`memory-bank/techContext.md`. No source, header or CMake file is touched by this decision.

