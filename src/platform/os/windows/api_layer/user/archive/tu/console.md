# narrative: console.cpp, session 7

## console.cpp - session 7 (30 Oct 2026)

The "Console functions" section (`user32.h:689-690`): one wrapper, one binding - and the only TU in the review whose
wrapper has **no SDK declaration at all** to pair against, so method step 2 (SDK prototype comparison) had no target and
the export table became the sole authority for the name.

| Wrapper | Declaration / definition | Resolved binding (`cl /EP`) | Export |
|---|---|---|---|
| `ConsoleControl(CONSOLECONTROL, PVOID, DWORD) -> NTSTATUS` | `user32.h:690` (was non-static - **U13**, now `static`) / `console.cpp:18+` | `"ConsoleControl"` against `L"user32"` | yes - ordinal 1594, hint 55, RVA 00028FD0, real code (not one of the four table forwarders) |

- **Binding correct as written; no P1/P2 conversion needed.** `user32.dll` exports exactly one `*Console*` name (fresh
  `dumpbin /exports`: the `Console` grep returns that single line), so there is no A/W family and no `stem -> stemW`
  macro - live probe: `ConsoleControl` RESOLVED, `ConsoleControlA` MISSING, `ConsoleControlW` MISSING. Under D13 the bare
  `qor_pp_useswinapi` is therefore the right spelling and `/EP` proves the literal name reaches `GetProcAddress`. No `T`
  suffix on the wrapper - correct. The stem is not one of the nine `#undef`s at `user32.h:9-17`, so G2 does not apply,
  and the U17 census classifies it with the 404 correct single-export members.
- **The signature cannot be verified against Microsoft - the headline risk of this TU, not a defect in it.**
  `ConsoleControl`, `CONSOLECONTROL` and the five structures at `user32.h:19-62` appear in **no** header of
  SDK 10.0.26100.0 or 10.0.10240.0 (whole `Include\` tree grepped; `wincon.h` only pulls in
  `consoleapi.h`/`consoleapi2.h`/`consoleapi3.h`, whose sole hit for the pattern set is `SetConsoleMode` at
  `consoleapi.h:138`). Consequences recorded: no collision risk (U10's SDK-side check is now closed), the enum numbering
  (0-7 with `Reserved1/2/3` holes) and the structure layouts rest on the de-facto internal definition rather than on
  anything installed here, and the parameter names (`Command`, `ConsoleInformation`, `ConsoleInformationLength`) are the
  project's own with no authority to check them against. `NTSTATUS` is the one SDK-supplied type in the signature
  (`typedef LONG NTSTATUS`, `/EP` line 320947), so the return really is a 32-bit signed status. A newer console may
  expose control codes past `ConsoleEndTask` (7); nothing installed here can tell us, so the enum is recorded as
  *possibly incomplete* - a documentation gap, not a defect.
- **Fixed (U13, the promised one-token fix).** `static` added at `user32.h:690`. Measured both ways with
  `tmp/_tmp_console_static_probe.cpp` (compile-only caller probe, `ConsoleSetWindowOwner` + `CONSOLEWINDOWOWNER`): before
  = **1 x C2352** "a call of a non-static member function requires an object", exit 2; after = `/W4` exit 0. Census
  re-run after the fix: 525 member-declaration-shaped lines, 428 `static` (was 427), 426 distinct defined names, zero
  defined-but-non-static. `desktop.cpp` no longer needs the U10 collision check - the grep was whole-tree.
- **Fixed (nits).** Missing space `,DWORD` -> `, DWORD` at the declaration; section comment `//Console` ->
  `//Console functions` (every other section comment ends in "functions"); dead `#undef WinHelp` at `console.cpp:14`
  removed - the TU references `WinHelp` nowhere and `user32.h:10` already undefs the stem, so the line was inert in
  every state of this file (`help.cpp:14` carries the same line and keeps it for that TU's session).
- **Fixed (documentation).** New TU comment inside the namespace (the `brush.cpp`/`combobox.cpp` pattern): the
  no-SDK-authority finding, the section-completeness claim, the bare-stem/D13 rationale with the live-probe numbers, the
  caller-owned in/out buffer contract (`ConsoleInformation` must be the structure belonging to the chosen control code,
  `ConsoleInformationLength` its size, nothing allocated or freed by the call), the NTSTATUS success semantics, and the
  console-ownership / access-denied behaviour of the sensitive control codes.
- **Verified correct as written:** return and parameter types against the export's real contract; `Library::Call<NTSTATUS,
  CONSOLECONTROL, PVOID, DWORD>` explicit 1/1 with tight spacing (no U14 churn); `qor_pp_fcontext` present; the include
  block identical to the folder norm (4 QOR includes, then `user32.h`, `../returncheck.h`, `../library.h` last);
  4-space indent; `}}}//qor::win::api`.
- **Left alone deliberately:** no `Win32ErrorHandler _;` and no `CheckReturn` at the single call site (settled policy,
  U1/U2 - this TU's tally is 1 call site, 0 handlers, 0 checks); `../returncheck.h` included and unused; the binding is
  cached in a function-local `static const` for process lifetime (U5); `Call`'s default-`__cdecl` `fPtr` against a
  `WINAPI` export (U6, 1 more site) - and here the callee really is user32's own stub, since `win32u.dll` exports
  `NtUserConsoleControl` and not `ConsoleControl` (probe: MISSING for the latter), so no forwarding is involved.
- **Return-contract note for any future return-check decision (the worst U7 instance so far).** NTSTATUS success is
  `>= 0` (0 = `STATUS_SUCCESS`, positive = informational), so `== 0` is not a valid failure test and `returncheck.h` has
  no NTSTATUS-aware check - only `SuccessCheck`/`TSuccessCheck`, which test `== 0`. Worse, `Library::Call` turns a
  swallowed SEH fault into `NTSTATUS _{}` = **0 = STATUS_SUCCESS**: a fault in this wrapper is not merely silent, it is
  reported as success. If U2 is ever reopened, this is the wrapper that needs a real check class (test `< 0`) and a
  non-zero default.
- **Surface-scope note (feeds U9's missing scope note).** The console surface a caller would actually want is in
  `kernel32.dll` - 135 `*Console*` exports (`AllocConsole`, `GetConsoleWindow`, `ReadConsoleW`, `WriteConsoleW`, ...) -
  and `api_layer/kernel/kconsole.cpp` already wraps it, with the kernel error-handler idiom intact; `user32.dll` has
  exactly one, which is why this section is one wrapper long. Nothing in `src`/`examples`/`test` calls this wrapper
  (U15 instance), and there is no console controller in `ui/gui` that could.
- Verified: `/EP` after the edits still shows `qor_pp_useswinapi` -> `GetModuleHandleW(L"user32")` + `"ConsoleControl"`;
  `console.cpp` compiles standalone at `/W4` exit 0; the caller probe compiles `/W4` exit 0; the whole library compiles
  **38/38 at `/W4`, exit 0, no diagnostics** (`tmp/_tmp_cl_all.log`) - the `static` addition broke no other TU.

---

