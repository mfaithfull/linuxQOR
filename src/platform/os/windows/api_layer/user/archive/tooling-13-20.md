# archive/tooling-13-20.md - tooling sections for sessions 13-20 and the U6 fix (verbatim slice)
Sliced out of `tooling.md` on 3 Nov 2026 (session 29). Was `tooling.md:65-126`. Read-only history: append new
tooling to `tooling.md`, never here.

## Tooling added after the review-method index (sessions 13-16)
`_tmp_pp_dialog.bat`, `_tmp_pp_errh.bat`, `_tmp_pp_fs.bat`, `_tmp_pp_help.bat` (per-TU `cl /EP`, each writing its own
`_tmp_*_pp.txt`); `_tmp_binding_names.ps1` (reads binding literals out of a `/EP` dump in definition order - the only
practical way to audit a 453k-line preprocessed file); `_tmp_decl_no_def_scan.ps1` / `_tmp_decl_no_def_repo_scan.ps1`
(declared-but-never-defined census, 64 names in `_tmp_decl_no_def_s13.txt`, reconciled with U4's 97);
`_tmp_guard_stack_scan.ps1` (generalised U16 guard-stack scan, `-Pattern <regex>` over `winuser.h`, replacing the
hard-coded `_tmp_sdk_guard_scan.ps1`); `_tmp_fs_bypass.txt`, `_tmp_errh_bypass.txt`, `_tmp_help_bypass.txt`;
`_tmp_dump_help.bat` -> `_tmp_help_linkermember.txt` (first `dumpbin /linkermember:1` use - the tool for confirming a U4
"declared but undefined" claim at the binary level); `_tmp_tchar_probe.cpp` + `_tmp_run_tchar_probe.bat`;
`_tmp_ascii_check.ps1` (non-ASCII byte counter); `_tmp_returncheck_usage.ps1` (feeds U2/U26);
`_tmp_hook_pre.bat`, `_tmp_hook_guards.ps1`/`.bat` (staged for session 17); `_tmp_name_resolve_probe.cpp` extended by
+24 dialog spellings (+7 exported-but-undeclared), +16 error-family, +12 filled-shape and +5 help spellings;
`_tmp_status_HEAD_backup.md` (a copy of `status.md` as of the session-15 commit, kept beside the archive).

## Tooling added in sessions 17-20
- `tmp/_tmp_kb_p1census_s20.ps1` (session 20) - **the binding census to run in every session.** For each of the 428
  `qor_pp_useswinapi`/`...AW` bindings it asks whether the literal the macro produces is in the export table and prints
  `FAIL <tu>:<line> <bare|AW>(user32, Stem) bare=? A=? W=?`. Post-fix run: **17 FAILs of 428**, decoding as 11 genuinely
  broken (`mouse.cpp` x10, `window.cpp:124`) + 6 P2 (`menu.cpp:159,194`, `message.cpp:124,145,152,159`). Companion
  `tmp/_tmp_kb_awchecklist_post_s20.txt` prints the same per TU as `bare= AW= single-ok=`; `single-ok=0` means the TU has
  no correctly-spelled bare-single-export binding at all (`mouse.cpp` reads `bare=0 AW=10 single-ok=0`).
- **New defect class (session 20): `qor_pp_useswinapiAW` over a stem with no A/W pair.** The macro pastes `W`
  unconditionally (`kernel32.h:52`), so the call asks for a name that does not exist and `pFunc` caches NULL (G5). P5
  cannot see it (stem == member) and neither can the compile (`Library::Call` has no prototype); only the export-table
  comparison does. `tmp/_tmp_kb_awpair_s20.ps1` is the per-stem `bare / <stem>A / <stem>W` check to run before choosing
  bare vs AW.
- The export table's parsed count differs from the names it lists, and the gap is not a missing export - see `gotchas.md`.
- `tmp/_tmp_kb_bypass_s20.ps1` - wrapper that passes the 55 A/W/`T` suffix variants of a TU's stems to
  `_tmp_bypass_scan.ps1`, because `pwsh -File script.ps1 -Stems A,B` delivers **one string**, not an array: the scan then
  silently sees one 100-character stem and reports 0 sites everywhere. Never record a 0-site U19 result without it.
- `tmp/_tmp_kb_u18_s20.ps1` compares each member's return type with the SDK prototype and prints the `winuser.h` line -
  the instrument behind U18's per-TU counts. It finds **return** divergences only; parameter divergences (e.g.
  `keybd_event`'s `void*` vs `ULONG_PTR`) still need a manual pass over the prototype block.
- `tmp/_tmp_comment_state.ps1 -Path <file> -Lines a,b,c` answers "is this bypass live code or a comment?" - the U19 tally
  is meaningless without it (`ui/gl/windowimpl.cpp` is 57% comment lines).

## Off-protocol item fix (U6, 1 Nov 2026) - the gate rule it established and the two instruments it needed
The U6 closure was not a TU session, so it is logged here rather than as a session row. It produced one gate rule and two
instruments that no `/W4` compile can replace:

- **Rule: a change to a header shared across api_layer libraries must be gated across all of them, not just the reviewed
  folder.** `library.h` is included by all 12 libraries, so the standing 38/38 user-folder gate proves nothing about the
  other 68 sources. New gate: `tmp/_tmp_cl_all_apiall_u6.bat`, which compiles all **106** api_layer `.cpp` files
  standalone at `/W4` from the response file `tmp/_tmp_u6_apiall_sources.rsp` (a `cl @file.rsp` source list is the only
  way to pass that many paths under cmd's 8 KB command-line limit). Its pre-fix and post-fix logs
  (`tmp/_tmp_cl_all_apiall_u6_pre.log`, `..._post.log`) are byte-identical, which is the form of evidence to quote for
  "no regression outside the reviewed folder". **Baseline it carries:** 165 `/W4` warnings (`gdi` C4995 on the deprecated
  `UpdateICMRegKeyW`, `uxtheme` C4100) plus one pre-existing `fatal error C1083` at `opengl/wgl.cpp:31` (cannot open
  `gdi32.h` with this cl line) - so the extended gate exits non-zero on HEAD too, and its exit code alone is meaningless;
  always diff it against a pre-fix run.
- **Calling-convention evidence needs an x86 build plus the disassembly, never a compile.** On x64 there is one calling
  convention, so every existing check is structurally blind to this defect class. `tmp/_tmp_u6_cc_probe.cpp` calls a
  `WINAPI` callee through `Library::Call`/`voidCall`; `tmp/_tmp_u6_cc_x86.bat` compiles it with
  `vcvarsamd64_x86.bat` (host x64, target x86) and dumps the object with `dumpbin /disasm` into
  `tmp/_tmp_u6_cc_x86_<pre|post>.asm`. The fact to read is the instruction after the indirect `call`: caller-side cleanup
  (`add esp,0Ch`) is `__cdecl`, no adjustment is `__stdcall`. The object dump must be written from inside the batch file -
  `cmd /c 'foo.bat > log 2>&1'` intermittently produced no file through this shell.
- **Census worth reusing:** `Select-String -Pattern 'qor_pp_useswinapi(AW)?\(\s*([A-Za-z0-9_]+)'` over
  `api_layer\*\*.cpp` grouped by module gives the blast radius of anything in the binding/call path - 1,638 bindings
  across 12 modules (user32 429, kernel32 330, gdi32 324, ole32 201, ws2_32 135, shell32 60, comctl32 53, uxtheme 48,
  opengl32 27, dwmapi 23, Xinput1_4 7, shcore 1).

