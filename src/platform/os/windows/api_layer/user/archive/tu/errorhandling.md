# narrative: errorhandling.cpp, session 14

## errorhandling.cpp - session 14 (31 Oct 2026)

- **Scope:** the "Error Handling functions" section, `user32.h:613-617` - **4 declarations** (`:614-617`), 4 definitions in
  the header's order, all `static`, all defined `User32::`-qualified. No U4 member (the whole section lives in this TU),
  no U11, no U13. The TU was 46 lines before this session and 92 after (the 46-line section comment now at `:16-61`);
  `user32.h` untouched. In `add_library` at `user/CMakeLists.txt:6`.
- **0 binding / correctness / UB defects, and the bare spelling is provably right for all four.** `cl /EP`
  (`tmp/_tmp_pp_errh.bat` -> `tmp/_tmp_errh_pp.txt`) yields the four literals `"FlashWindow"`, `"FlashWindowEx"`,
  `"MessageBeep"`, `"SetLastErrorEx"` (`:453288`, `:453295`, `:453302`, `:453310` of the 31 Oct run, in definition order);
  the export table gives 115, 116, 283, 342; and the extended live probe (`tmp/_tmp_name_resolve_probe.cpp`, 16 spellings
  added this session) reports **RESOLVED** for all four stems and **MISSING** for every `*A`/`*W` candidate -
  `FlashWindowA/W`, `FlashWindowExA/W`, `MessageBeepA/W`, `SetLastErrorExA/W` all absent. So this is the D13 case in its
  purest form: `qor_pp_useswinapi` is correct and `qor_pp_useswinapiAW` would break all four bindings. None of the four
  stems is among the nine `user32.h:9-17` `#undef`s, so G1 has nothing to expand and the source spelling is literally the
  `GetProcAddress` argument. `tmp/_tmp_wrong_stem_scan.ps1` scanned 428 bindings across 38 files, flagged 4
  (`cursor.cpp:63`, `mouse.cpp:82`, `resource.cpp:19`, `resource.cpp:26` - all known) and **nothing in this file**.
- **The signature's one non-trivial type was checked rather than assumed:** `FlashWindowEx(::PFLASHWINFO)` at
  `user32.h:615` names the SDK's own typedef (`winuser.h:4694`), and the wrapper is a pure pass-through, so the caller
  hands `user32` a real `FLASHWINFO` and `cbSize` must be `sizeof(FLASHWINFO)`. Unlike U23's `MsgBoxParams` there is no
  QOR-side mirror struct in play in this TU - and none exists at all: this library publishes neither the `FLASHW_*` flags
  (`winuser.h:4702-4707`) nor the `SLE_*` severities (`:13551-13553`).
- **2 nits fixed (both trailing spaces at call sites):** the two-line `Library::Call< BOOL, UINT >(` + `pFunc, uType );`
  pair at `MessageBeep` became one line ending `(pFunc, uType);`, and
  `Library::voidCall< DWORD, DWORD >(pFunc, dwErrCode, dwType )` became `(pFunc, dwErrCode, dwType)`.
  Inner template-argument spacing is left alone: this file is 2 tight / 2 spaced against the folder's 250 tight / 179
  spaced, which is not a norm either way (U14(a)).
- **U18 - no new instance, settled from the SDK.** `SetLastErrorEx` is `VOID` in the SDK
  (`winuser.h:13558-13563`, `WINUSERAPI VOID WINAPI SetLastErrorEx(_In_ DWORD dwErrCode, _In_ DWORD dwType)`), so
  `Library::voidCall< DWORD, DWORD >` matches it exactly and the `void` return in the header is correct. U18 keeps its one
  pair (`window.cpp`'s `void`-returning point converters) and gains nothing.
- **Return-check policy instance (U1/U2, settled):** 4 call sites, 1 `CheckReturn` (`MessageBeep`, `:77-83`), 0
  `Win32ErrorHandler`. That site is **1 of only 4 `CheckReturn` uses in `winqor_api_user`** - the other 3 are
  `processthread.cpp:20`, `:29`, `:45` (measured with a `Get-ChildItem | Select-String` pass over `api_layer`, where
  `kernel`/`shell`/`winsock2` use the pattern heavily). G6's "always live" was re-verified on this TU's `/EP` dump: only the
  checked branch of `returncheck.h:19-23` survives (`typedef CheckReturn< TReturn, TCheck > TType;` at pp line 452867;
  `typedef TReturn TType;` appears nowhere in the 453k-line dump), so every `FALSE` from `MessageBeep` raises a continuable
  issue at run time. `FlashWindow` deliberately has no check - U24 explains why adding one would be a behaviour bug - and
  `FlashWindowEx` (FALSE on failure) is unchecked, which is a U2 policy gap rather than a TU defect.
- **Citation correction applied to G6 in place:** G6 cited the `#if` as `returncheck.h:40`; measured now it is `:19`
  (checked typedef `:20`, `#else` `:21`, bare typedef `:22`, `#endif` `:23`). The finding itself stands unchanged.
- **Instrument results after the edits:** `_tmp_sigcmp.ps1 -Tu errorhandling.cpp -First 614 -Last 617` parses 4
  declarations and prints `decl==def ok` + `ok` for `FlashWindow`, `FlashWindowEx` and `MessageBeep`; the `SetLastErrorEx`
  row prints `(no Library::Call - voidCall or inlineCall?)` by design, so its call site was read out of the `/EP` dump
  instead: `Library::voidCall< DWORD, DWORD >(pFunc, dwErrCode, dwType)` - two `DWORD` template arguments for two `DWORD`
  arguments, matching the SDK. `_tmp_name_divergence_scan.ps1` -> 0 of 4 definitions; the folder-wide parameter-name
  divergence count stays **0 of 426**, a third consecutive TU confirming it.
- **Convention counts:** U13 0 of 4; U14(b) 0 deducing (4/4 explicit); U17 0 (no A/W family in the section, so no `T`
  suffix is due on any of the four); U4 0; U11 0; U15 **3 of 4**; U19 **1 LAYER / 0 INDIRECT / 0 BYPASS / 0 MEMBER**
  (`tmp/_tmp_errh_bypass.txt`); U16 **1 version-guarded of 4**, and it is the type-level sub-case again - `PFLASHWINFO` is
  typedef'd inside the same `#if(WINVER >= 0x0500)` block as the prototype, so `WINVER < 0x0500` makes `user32.h:615`
  ill-formed rather than merely prototype-less (full guard stack in the U16 entry).
- **Liveness:** only `FlashWindow` has a call site at all - `WindowController::Flash`
  (`ui/gui/controllers/windowcontroller.cpp:717`, built at `gui/CMakeLists.txt:27`) - and its only in-tree caller,
  `WindowImpl::RequestAttention()` (`ui/gl/windowimpl.cpp:1833-1836`), belongs to `winqor_gl`, which `ui/CMakeLists.txt:4`
  comments out of the build. That one call site is also **U24**, opened this session: the method turns a
  previous-active-state `BOOL` into a `bool` "did it work" result.
- **Section coverage:** `user32.dll` exports two more error-adjacent entry points that `user32.h` declares nowhere -
  `RegisterErrorReportingDialog` (2E6) and `SetDebugErrorLevel` (32F), both **RESOLVED** in the probe - while
  `GetLastError`/`SetLastError` are **MISSING from `user32.dll`** (probe) and correctly belong to `kernel32`.
  `DdeGetLastError` (8E) is part of the undeclared DDE section (U4).
- **Verified at exit:** `errorhandling.cpp` compiles standalone at `/W4` **exit 0** (re-run after every edit,
  `_tmp_compile_one.ps1`); the whole library compiled **38/38 exit 0** with the edits in place
  (`tmp/_tmp_compile_all.ps1` -> `exit=0 loglines=43`, `errorhandling.cpp` echoed, no diagnostics); the post-edit `/EP`
  re-run still yields the four bare literals in definition order; `user32.h` untouched.
- **Instrument note for every later session (cost one wasted scan): array parameters and `pwsh -File`.** Passing
  `-Stems A,B -Roots 'x','y'` to a script through `pwsh -NoProfile -File script.ps1` does **not** deliver arrays - each
  comma list arrives as one literal string, quotes and all. `_tmp_bypass_scan.ps1` then reports `stems: 1`, and the bogus
  `-Roots` string makes it die at `:12` with the misleading `Get-ChildItem: A parameter cannot be found that matches
  parameter name 'File'` (a path that resolves to a provider with no `-File` dynamic parameter) **while still printing its
  totals line with 0 BYPASS from an empty file list**. Correct forms: rely on the script's default `-Roots`
  (`src`, `examples`, `test`) and pass `-Stems` as a single element, or invoke
  `pwsh -NoProfile -Command "& script.ps1 -Stems @('A','B')"`. A totals line printed alongside a `:12` error is not a clean
  scan - the session's real numbers came from the `-Command` form.
- **Instruments this session leaves behind:** `tmp/_tmp_pp_errh.bat` (+ `tmp/_tmp_errh_pp.txt`,
  `tmp/_tmp_errh_pp_err.txt`), `tmp/_tmp_errh_bypass.txt`, and `tmp/_tmp_name_resolve_probe.cpp` extended with 16
  error-family spellings (4 stems x 3 spellings plus `RegisterErrorReportingDialog`, `SetDebugErrorLevel`, `GetLastError`,
  `SetLastError`).

