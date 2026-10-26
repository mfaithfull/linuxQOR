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


