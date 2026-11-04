# `systemshutdown.cpp` - session 31 narrative (4 Nov 2026)

51 -> 102 lines. 5 wrappers, the whole `user32.h:601`-`:606` section, whose label the header itself misspells
`//System Shutdwown functions`. Members in header order: `ExitWindowsEx` (`:67`), `LockWorkStation` (`:74`),
`ShutdownBlockReasonCreate` (`:81`), `ShutdownBlockReasonDestroy` (`:88`), `ShutdownBlockReasonQuery` (`:95`) - all
`static`, all returning `BOOL`, all bound with bare `qor_pp_useswinapi(user32, <stem>)` and called through
`Library::Call<...>` with tight parens.

## What the session fixed

`:37` and `:44` (pre-edit numbering) carried `WINAPI` on the definition heads of `ShutdownBlockReasonDestroy` and
`ShutdownBlockReasonQuery` - the only two `WINAPI`-qualified `User32::` definition heads in the folder. Removed, because:

- **The disagreement is diagnosed, not ignored.** Compiling the TU for x86 (`vcvarsamd64_x86`) produced
  `error C2373: 'qor::win::api::User32::ShutdownBlockReasonDestroy': redefinition; different type modifiers` at `:37` and
  the same at `:44`, each with a `note: see declaration of ...` pointing at `user32.h:605`/`:606`. The header declares all
  five members with no convention token; the only `WINAPI` substring in `user32.h` is inside the
  `QOR_WINAPIUSER` export macro at `:66`.
- **On the arch the tree builds (x64) it changed nothing.** `tmp/_tmp_s31_winapi_probe_s31.cpp` calls the members through
  the header declaration only; `dumpbin /symbols` shows the definition side and the reference side emitting/consuming the
  **same** decorated name (`?ShutdownBlockReasonDestroy@User32@api@win@qor@@SAHPEAUHWND__@@@Z`, `__cdecl`), so the token
  never reached the ABI. `dumpbin /disasm` of the object before and after removal is **42,310 lines on both sides with 2
  differing lines, both the dump's own file-name header** - instruction-identical.
- **The `/EP` diff shows the same thing from the source side**: 55 differing lines, 51 of them blank (comment-block
  insertion), the remaining 4 being exactly the two definition heads `BOOL __stdcall User32::...` -> `BOOL User32::...`;
  **non-blank preprocessor output 154,302 lines on both sides**, and all 5 binding literals unchanged.

Rule taken from it: an entry point's `__stdcall` lives in `Library::Call`'s `fPtr` / `DefProc` (U6, closed 1 Nov 2026); a
wrapper's own convention is whatever its in-class declaration states, and MSVC enforces that agreement. The SDK's
`WINUSERAPI BOOL WINAPI` heads (`winuser.h:3639`, `:14916`, `:15924`, `:15931`, `:15939`) are free functions, where the
token is correct and expected.

## Why the x86 attempt was made (and what else it measured)

The folder has been gated x64-only for 30 sessions, and `CMakeSettings.json:4` configures `x64-Debug` only. Probing x86
for the convention question exposed a second, larger fact: `kernel32.h:92` declares
`RtlLookupFunctionEntry(ULONGLONG, PULONGLONG, PUNWIND_HISTORY_TABLE)`, and the SDK typedefs `PUNWIND_HISTORY_TABLE` only
inside `#if defined(_AMD64_)||defined(_ARM_)||defined(_ARM64_)` (`um/winnt.h:21939`-`:21963`). Compiling all 106 api_layer
sources for x86 gives **93 failures with `C2061 identifier 'PUNWIND_HISTORY_TABLE'`** (digest
`tmp/_tmp_s31_x86_digest_s31.ps1`); the 13 that survive are exactly the `winqor_api_*.cpp` glue TUs. `kerrorhandling.cpp`
adds a `C2065 'TargetGp': undeclared identifier` from the same line. No change made - it is a kernel-header decision
outside this review's protocol - but it is the same blind spot that made U6's stack corruption invisible until the
dedicated x86 probe of 1 Nov 2026, and it kept this TU's C2373 pair undetected for 30 sessions.

## Clean measurements (no action)

- **Binding:** 5/5 stems are bare single exports with no A/W family and no `#define <Stem>` alias in the `um` headers -
  `ExitWindowsEx` 10F, `LockWorkStation` 26C, `ShutdownBlockReasonCreate` 39C, `ShutdownBlockReasonDestroy` 39D,
  `ShutdownBlockReasonQuery` 39E - so bare `qor_pp_useswinapi` is D13's required spelling and P1/P2/P6 cannot arise.
- **Signatures:** 5/5 `decl==def` on return type, parameter types, parameter names and `Call<>` arity; no name divergence
  anywhere - the first reviewed TU with no U18 name traffic at all.
- **U16:** all 5 prototypes `WINAPI_PARTITION_DESKTOP`; only `LockWorkStation` is version-gated
  (`_WIN32_WINNT >= 0x0500`); the `ShutdownBlockReason` trio has no version floor (guard depth 2), which refutes the
  session-30 lead that expected the highest floor yet.
- **U19/U15:** 0 call sites of any class in `src`, `examples`, `test` for all five stems - published surface (D14), so no
  row recorded.
- **U1/U2:** 0 checks, 0 `_Check_return_` on the prototypes; `ShutdownBlockReasonDestroy`'s single `BOOL` carries the
  `ERROR_SHUTDOWN_IN_PROGRESS` / `ERROR_NOT_FOUND` distinction that the layer discards. `:11`'s unused `../returncheck.h`
  is T7's 37th dead include.
- **Hygiene:** 0 TAB, 0 trailing space, 0 non-ASCII; longest line 124 (`:9`, the include-order comment); the 3 original
  comment lines plus the new 50-line contract block.

## Gate

`tmp/_tmp_compile_one.ps1 systemshutdown.cpp` -> `cl_exit=0` after the edit; library gate `tmp/_tmp_cl_all_s31c.log`
**38 TU echoes / 0 diagnostics**, all 38 objects newer than the edited source (the log hash cannot date a clean log -
`gotchas.md`); x86 scan `tmp/_tmp_s31_x86_apiall_s31.txt` (206 lines, digest only).
