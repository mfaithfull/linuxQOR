# winqor_api_user - review method, tooling and instrument caveats

Verbatim from `status.md` (1 Nov 2026 split). Read the caveats section before acting on any instrument flag.

## Review method (validated on accelerator.cpp)

1. Read the TU; pair every definition against its declaration in `user32.h` (return type, parameter types, `T` suffix).
2. Compare each wrapper signature with the SDK prototype in `winuser.h` and with the export table in
   `tmp/_tmp_user32_exports.txt` (`dumpbin /exports` of `C:\Windows\System32\user32.dll`, 1018 names).
3. Resolve the binding name the wrapper *actually* asks for - preprocess the TU with `cl /EP` and grep the
   `qor_pp_useswinapi` expansion. Never trust the source spelling; see ground rule G1.
 3b. Check **binding-vs-member agreement** with `tmp/_tmp_wrong_stem_scan.ps1` (added after session 8, which found
    three wrappers calling `GetDCEx`). Step 3 proves the bound name resolves; it does not prove the wrapper calls
    the entry point it wraps. Run the scan and expect this TU's sites to be clean. A flag is a defect until disproved -
    the only benign patterns so far are a deliberate alias to a different export (`cursor.cpp` `CopyCursorT` ->
    `CopyIcon`, pre-scan P3) and a definition not qualified with `User32::` (`resource.cpp`, open item U11).
4. Check the call-site idiom against `api_layer/kernel` + `api_layer/shell`: `qor_pp_fcontext;`,
   `Win32ErrorHandler _;`, `qor_pp_useswinapi*`, `Library::Call<RET, Args...>` with explicit template
   arguments, `CheckReturn<...>::TType` where zero/false means failure.
5. Look for an in-tree caller of every wrapper (`src`/`examples`/`test`). Name the consumer chain one level deep -
   which `ui/gui` controller or handler calls it - then stop: whether that controller should itself be called is a
   `winqor_gui` question. **Since 2 Nov 2026 an unreachable wrapper is not recorded at all** - open item U15, which used
   to collect these, is CLOSED by user decision (policy **D14**, `docs/agent/DECISIONS.md`); there is no U15 tally to
   append to. Do the check anyway when another item's severity depends on reachability - whether a P1/P5 mis-binding is
   latent or live, U18's divergent parameter at `TrackPopupMenu`, U19's LAYER-vs-BYPASS split - and record the fact in
   that item's entry. `registers/u15.md` keeps the instances measured before the closure as frozen history.
5b. For every stem whose callers are established, classify the call sites with `tmp/_tmp_bypass_scan.ps1`
   (**LAYER** through `User32::` vs **BYPASS** through the SDK import) and record the split against open item U19.
   **Never trust a raw grep for liveness**: comments must be stripped first (`tmp/_tmp_comment_state.ps1` for one file,
   the bypass scan's own stripper for a sweep) - `ui/gl/windowimpl.cpp` is 57% commented-out code and produced eight false
   bypass hits (see U19's method-hazard note).
   **Feed the scan every suffix variant of the stem** (`stem`, `stemA`, `stemW`, `stemT`): it matches `stem\s*\(`, so a
   run with bare stems alone misses every QOR layer call (which uses the `T`-suffixed member) and every bypass that
   calls the `W` export directly. Measured 30 Oct 2026 on the display family: bare stems gave 8 LAYER / 4 BYPASS, the
   full variant list with the pre-fix scan gave 16 LAYER / 10 BYPASS, and with both instrument fixes in place the same
   family gives **16 LAYER / 12 BYPASS** - the bare run hid all four `ui/gdi/displaydevices.cpp` and four
   `i_m/monitor.cpp` layer sites and six of the `*W` bypass sites. Also record, for each bypass file, whether it is
   in a CMake target at all: a bypass site in a file no target compiles is source debt, not run-time behaviour.
   **Second instrument fix this session (30 Oct 2026):** `::stem(` must be matched anywhere in the line. The previous
   pattern required either `^\s*::stem(` or a non-word, non-`:` character immediately before the stem, so a mid-line
   `if (::ChangeDisplaySettingsExW(...)` or `const HDC dc = ::GetDC(NULL);` fell through every branch - two sites in
   `ui/gl/monitorimpl.cpp` were invisible (`:138`, `:263`). Earlier sessions' per-family BYPASS counts were taken with the
   unfixed pattern and are therefore **lower bounds**; re-run the scan with the full variant list before quoting any of
   them (the cursor-family and coordinate-family entries under U19 both need that re-run).
6. Compile the TU standalone (`tmp/_tmp_compile_one.ps1 <name>.cpp`, `/W4`, must be exit 0).
7. Note which SDK prototypes the wrappers pair with are emitted only behind a version guard (`#if WINVER >= ...`,
   `#if NTDDI_VERSION >= ...`, `_WIN32_WINDOWS`, `_WIN32_IE`) and which SDK-visible entry points relevant to the
   section are absent from the installed SDK at that guard level. Record them against open item U16 as a per-TU list;
   do **not** change any build-wide version macro - the pin is deferred to the end of the review (user decision,
   session 5).

Tooling in `tmp/` (git-ignored scratch): `_tmp_compile_one.ps1 <name>.cpp` (single-TU `/W4` compile),
`_tmp_compile_all.ps1` (all 38 TUs; baseline 29 Oct 2026 = exit 0, zero warnings),
`_tmp_pp_one.bat <name.cpp>` then grep `DefProc pFunc` in `tmp/_tmp_pp_out.txt` (the resolved binding names),
`_tmp_name_resolve_probe.cpp` + `_tmp_run_probe.bat` (live `GetModuleHandleW`/`GetProcAddress` probe against
the real `user32.dll`), `_tmp_cmake_file_diff.ps1` (disk vs `add_library` file list),
`_tmp_user32_exports.txt` (the export table), `_tmp_static_census.ps1` (declaration-vs-definition census: how many
`user32.h` members are `static`, how many are defined through `User32::`, and which defined members are not `static`
- added in session 3), `_tmp_undefined_static.ps1` (the reverse direction: `static` declarations with no definition
in the folder), `_tmp_u14_census.ps1 [-Dir <api_layer folder>]` (bracket-matching census of every `Call`/`voidCall`
site - explicit vs argument-deducing template arguments, plus the spaced/tight `Call<` split; added in session 4, it
replaced the faulty grep-based U14 count), `_tmp_tsuffix_census.ps1 [-Header <header>] [-Exports <exports.txt>] [-ShowAll]` (added session 6, the U17 instrument: parses the 8-space-indented member declarations of a `user32.h`-style header, strips the folder's trailing `T`, looks `<stem>A`/`<stem>W` up in an export table and reports A/W-family-without-suffix vs suffix-without-A/W; current `user32.h` census 525 parsed = 108 norm + 404 single-export + 11 deviations, plus 2 mirror-case flags), `_tmp_sdk_guard_scan.ps1 [-File <sdk header>] [-Names <name>...]` (added session 6, the U16 instrument: replays the `#if`/`#else`/`#endif` nesting of an SDK header and prints the guard stack open at each named declaration; defaults to `winuser.h` 10.0.26100), `_tmp_wrong_stem_scan.ps1 [-Dir <folder>]` (added session 8, the P5 instrument: pairs every `qor_pp_useswinapi`/`qor_pp_useswinapiAW` binding with the `User32::` member that encloses it and flags any stem that is neither that member nor its `T`-stripped base nor that base plus `A`/`W`; also mechanically reproduces U11 by reporting bindings outside any `User32::` definition), `_tmp_bypass_scan.ps1 -Stems <stems...> [-Roots <dirs...>]` (added session 8, the U19 instrument: comment-stripping scan that classifies every occurrence of a stem in `src`/`examples`/`test` as LAYER (`User32::`), INDIRECT (qualified on an intermediate class), MEMBER (`x.stem(...)` / `Class::stem` - a same-named
member, not an SDK call; added session 9 after the cursor sweep showed 9 of 13 raw hits were members) or BYPASS
(unqualified / `::`, i.e. going straight to the SDK import)), `_tmp_comment_state.ps1 -Path <file> [-Lines <n...>]` (added session 8: character-level block/line-comment and string-literal state machine; says whether given lines are CODE or COMMENT for the compiler and prints the commented-line fraction of the file - the guard against treating grep hits in commented-out regions as live). Session 11 added `_tmp_pp_dc.bat` (the same `/EP`
run written for one TU, with its own `_tmp_dc_pp.txt` / `_tmp_dc_pp_err.txt` outputs - the shared `_tmp_pp_one.bat` writes
`_tmp_pp_out.txt`, which is easy to read stale: a copy issued before `cl` finished produced a whole session's worth of
desktop.cpp lines under a devicecontext name), `_tmp_sigcmp.ps1` (generalised from the session-10 one-off: any section,
`-Tu <file> -First <line> -Last <line>`; checks return type, parameter types, parameter names and `Call<>` arity/types),
`_tmp_dc_bypass.txt` / `_tmp_dc_bypass2.txt` / `_tmp_dc_bypass3.txt` (the first, bare-stem run is
kept as the worked example of the suffix blind spot in step 5b; the third is the post-both-fixes run, 16 LAYER / 12 BYPASS),
`_tmp_aw_spans.ps1` (prints the exact `winuser.h` line span - `WINUSERAPI` through `#endif // !UNICODE` - of each A/W
family for a stem list; used to re-check the `devicecontext.cpp` comment's family ranges) and
`_tmp_cl_all_s11.log` / `_tmp_cl_all_s11b.log`.
**Session 12 added** `_tmp_name_divergence_scan.ps1 [-Tu <file>]` (the parameter-name consistency instrument: pairs every
`User32::` definition in the folder with its `user32.h` declaration and compares the parameter names **case-sensitively**;
baseline at writing = 2 divergent names in 426 definitions, both in `devicemanagement.cpp`, and 0 after that TU's fix - it
now expects 0 for every remaining TU), `_tmp_pp_devman.bat` (the per-TU `/EP` run for this TU, outputs
`_tmp_devman_pp.txt`/`_tmp_devman_pp_err.txt`) and six new probe rows in `_tmp_name_resolve_probe.cpp` for the
device-notification and power-setting spellings. **Instrument fix it also made:** `_tmp_sigcmp.ps1` compared parameter
names with PowerShell `-ne`, which is **case-insensitive**, so case-only divergence (`NotificationFilter` vs
`notificationFilter`, `Handle` vs `handle`) compared equal and the script printed `decl==def ok` - the comparison is now
`-cne`. Re-running the corrected script over the previously reviewed TUs changed no earlier verdict (`devicecontext.cpp`
8/8 still clean, `desktop.cpp`'s fixed pair still clean). Two PowerShell gotchas the new scan hit first, both recorded here
so future instruments avoid them: testing for the closing `)` on the captured *inside-parens* text can never succeed
(the capture excludes it), so the signature-continuation loop swallowed whole function bodies; and
`$x = if($c){ @(..) } else { @(..) }` **unrolls a one-element array to a scalar**, so `$x[0]` then indexed a character of
the name - assign inside the branch instead.
**Compile-log gotcha (measured 30 Oct 2026):** `_tmp_compile_all.ps1` writes the `cl` output to the fixed path
`tmp/_tmp_cl_all.log` regardless of the caller, so redirecting the script's own stdout to a per-session file captures only
its summary line (`exit=N loglines=M`) plus its internal diagnostic grep - it is not the `cl` log. The summary's `exit=0` is
already sufficient for the 38/38 claim (one `cl` invocation over all 38 sources fails as a whole if any TU fails), and the
belt-and-braces check is in `tmp/_tmp_cl_all.log`: 38 `\.cpp$` echo lines and 38 `.obj` files in `tmp/obj` newer than the
run - both confirmed for the session-11 run.

Full-library compile after the session-1 edit: 38/38 TUs, `/W4`, exit 0, no diagnostics. Re-run after the session-3
edit: same, 38/38, exit 0, no diagnostics (`tmp/_tmp_cl_all.log`). Re-run after the session-8 binding fix: same, 38/38,
exit 0, no diagnostics - the log's one `warning|error` grep hit is the filename `errorhandling.cpp`, not a diagnostic.
Re-run after the session-9 comment-only edit: same, 38/38, exit 0, no diagnostics (`tmp/_tmp_cl_all_s9.log`, `exit=0
loglines=43`, same single grep hit). Re-run after the session-11 edit (one space + a comment block): same, 38/38, exit 0,
no diagnostics (`tmp/_tmp_cl_all_s11.log`, `exit=0 loglines=43`, same single grep hit). Final session-11 re-run after the
last citation fixes: `exit=0 loglines=43`, and the `cl` log `tmp/_tmp_cl_all.log` shows **38 TU echo lines with 38 fresh
`.obj` files** in `tmp/obj` - the belt-and-braces 38/38 confirmation described above.


### Tooling added after the "Review method" index (sessions 13-16)
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
`_tmp_status_HEAD_backup.md` (this file as of the session-15 commit, kept beside the archive).


### Instrument caveats - read a flag before acting on it
- `pwsh -NoProfile -File script.ps1 -Stems A,B -Roots x,y` **does not deliver arrays** - each comma list arrives as one
  literal string. `_tmp_bypass_scan.ps1` then reports `stems: 1` and can die at `:12` with a misleading
  `Get-ChildItem: A parameter cannot be found ...` **while still printing a totals line of 0 BYPASS from an empty file
  list**; `_tmp_sdk_guard_scan.ps1` printed `hits: 0` for four real names (a silent false negative). Correct forms: use
  the script's default roots and pass one stem, or `pwsh -Command "& script.ps1 -Stems @('A','B')"`, or a wrapper script.
  A totals line printed next to an error line is not a clean scan.
- `_tmp_sigcmp.ps1` compares **spellings as strings**, so these rows are false positives: `voidCall` rows print
  `(no Library::Call - voidCall or inlineCall?)` by design; `sdk '::DLGPROC' vs call 'DLGPROC'` (cured by spelling the
  call sites `::DLGPROC`); `CONST::X*` vs `CONST ::X*` (3/3 sites in `filledshapes.cpp`, and it will do the same in
  `rectangle.cpp`, `painting.cpp`, `text.cpp` - U25; normalize the instrument, not the source).
- `_tmp_returncheck_usage.ps1` counts `CheckReturn<` in **comments**, so its folder count over-reads (3 shown, 2 real);
  same over-count class as the `Win32ErrorHandler` matches (5, all comments).
- `_tmp_compile_all.ps1`'s `pwsh -File` wrapper returned exit 1 in session 15 while the `.bat` it writes reported
  `cl_exit=0` with a clean log - run `cmd /c tmp/_tmp_cl_all.bat` and read the log, don't trust that wrapper's exit code.
- `cl /EP` output must go to a **per-TU file**: the shared `_tmp_pp_one.bat` writes `_tmp_pp_out.txt`, and reading it
  before `cl` finished once produced a whole session's worth of another TU's text.
- Counting en/em dashes by piping `git show HEAD:<path>` through PowerShell reports **0** (cp1252 re-decode) - measure with
  `[System.IO.File]::ReadAllText` on the working file and compare code-point groups instead; do not "normalize" encoding.
  The original note with the memory-bank dash counts is in the archive's `filledshapes.cpp` section. Measured 31 Oct: this
  file has **3** non-ASCII characters (all U+2026), the archive **4**; `.cpp` sources in this folder stay pure ASCII.



### Tooling added in sessions 17-20, and the caveats they carry
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
- **Export-table parsing caveat:** `tmp/_tmp_user32_exports.txt` lists 1,018 names but an ordinal+RVA regex parses
  **1,014**; the four misses are the `(forwarded to NTDLL...)` rows (P4), which carry no RVA. Quote 1,014 as the parsed
  count and do not read the gap as missing exports.
- `tmp/_tmp_kb_bypass_s20.ps1` - wrapper that passes the 55 A/W/`T` suffix variants of a TU's stems to
  `_tmp_bypass_scan.ps1`, because `pwsh -File script.ps1 -Stems A,B` delivers **one string**, not an array: the scan then
  silently sees one 100-character stem and reports 0 sites everywhere. Never record a 0-site U19 result without it.
- `tmp/_tmp_kb_u18_s20.ps1` compares each member's return type with the SDK prototype and prints the `winuser.h` line -
  the instrument behind U18's per-TU counts. It finds **return** divergences only; parameter divergences (e.g.
  `keybd_event`'s `void*` vs `ULONG_PTR`) still need a manual pass over the prototype block.
- `tmp/_tmp_comment_state.ps1 -Path <file> -Lines a,b,c` answers "is this bypass live code or a comment?" - the U19 tally
  is meaningless without it (`ui/gl/windowimpl.cpp` is 57% comment lines).

### Off-protocol item fix (U6, 1 Nov 2026) - the two instruments it needed, and the gate rule it established

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

### Tooling added in session 21 (`mdi.cpp`), and the caveats they carry

- `tmp/_tmp_int_sites_s21.ps1` - the **`Call<>`/`voidCall<>` template-argument correctness** scan. It extracts every
  `(Call|voidCall)\s*<[^>]*>` on a line and tests it case-**sensitively** for a standalone `INT` token, which is how a
  template list can contradict the signature of the function it sits in. Caveats: case-sensitivity is the whole point
  (`int` is the correct spelling for many SDK parameters, and a case-insensitive scan flags well over a hundred
  legitimate sites across the folder); the regex stops at the first `>`, so it only reads single-level type lists; and it
  can only see sites where the arguments were spelled out - the 4 deducing sites of U14(b) are invisible to it. Its
  denominator is raw source text, so comments inflate it: `tmp/_tmp_callcount_s21.ps1` counts 435 `Call<>`/
  `voidCall<>` template lists against 429 `qor_pp_useswinapi`/`...AW` occurrences and the P1 census' 428 code-line
  bindings (`mdi.cpp` alone reads 7 because its TU comment names `Call<>` and the macros). Quote 435 for this scan,
  428 for the census; the one site it found (`DefFrameProcT`) is now **0**. Pair it with `tmp/_tmp_sigcmp.ps1`, the
  only instrument that compares a call site against the declaration at all.
- `tmp/_tmp_msg_mirror_s21.cpp` + `tmp/_tmp_msg_mirror_s21.bat` - the **mirror-struct layout probe** (U26's method). The
  caller-side field lists are copied verbatim into a probe TU next to the SDK type and `sizeof`/`offsetof` printed for
  every field. The rule it establishes: **build it twice**, x64 (`vcvars64.bat`) and x86 (`vcvarsamd64_x86.bat`) - on
  x64 alone this probe reported "identical" and U26's third instance would have been missed. Two caveats measured:
  `offsetof` on a member that the SDK guards with `#ifdef _MAC` (`MSG::lPrivate`) is a hard C2039/C2618 error, so print
  only fields both sides have; and launching a freshly-built exe from inside the batch failed
  (`'..._x64.exe' is not recognized` while cmd had already created the `.txt` redirect file), so **run the two exes from
  a separate invocation** and keep the batch for the builds. Same lesson as the U6 disassembly probe above: x64-only
  evidence is structurally blind to this defect class.
- Per-TU instrument naming settled this session: `<family>_{prescan,bypass,guards,pp,probe}_s<N>` plus the shared
  censuses re-run with an `_s<N>` output suffix (`_tmp_p1census_s21.txt`, `_tmp_tsuffix_s21_full.txt`,
  `_tmp_cl_all_s21.log`). The `_tmp_mdi_guards_s21.ps1` wrapper exists only because `tmp/_tmp_sdk_guard_scan.ps1` takes a
  `-Names` **array** and `pwsh -File script.ps1 -Names A,B` delivers one string - the same trap as the U19 stem wrapper
  recorded above. A probe that resolves names must also record that it had to `LoadLibraryW` first:
  `GetModuleHandleW(L"user32")` fails with gle=126 in a bare console process (G5), and a probe that reports every name
  MISSING is measuring the loader, not the export table.

### Record-editing caveats (measured in session 21 while trimming the brief)

- **PowerShell `-like` treats the backtick as its escape character**, so a pattern containing a markdown code span never
  matches: the line-35 anchor of the brief returned False under -like while the same text matched under .Contains. For any
  anchor over this record's markdown use .Contains(...) or -match [regex]::Escape(...), and verify the anchor before
  splicing - a silent False there is how a slice loses lines.
- **The `editor` tool matches whole lines**, so on a markdown table whose rows are one very long line it either reports
  "text not found" or refuses an over-large payload. For those rows use a PowerShell script that replaces exact substrings
  with [IO.File]::WriteAllText and re-measures afterwards (tmp/_tmp_slog_fix_s21.ps1, tmp/_tmp_progress_fix_s21.ps1,
  tmp/_tmp_s21_close_a.ps1, _tmp_s21_close_b.ps1, _tmp_s21_close_c.ps1).
- **Splices must run bottom-up** (highest line range first) and print the before/after line and CRLF counts: status.md
  went 200 -> 179 lines / 17.4 KB in tmp/_tmp_s21_close_b.ps1, whose anchors are what caught the -like trap above.
- **Commands sent in one batch may run concurrently**, so never verify in the same batch a file another command writes:
  twice the reader ran first and reported the just-written compile log "does not exist" although the compile had exit 0.
### Tooling added in session 22 (`menu.cpp`), and the caveats they carry

- **The SDK return type is on its own line, so a declaration-line instrument cannot see it.** In `winuser.h` a
  prototype is `WINUSERAPI` / `BOOL` / `WINAPI` / `Stem(` on four separate lines, so `tmp/_tmp_menu_proto_s22.ps1`
  printed the stem line plus the parameters and reported nothing about the return type - method step 2 looks satisfied
  and is not. `tmp/_tmp_menu_ret_s22.ps1` walks back from the stem up to 4 lines, skips `WINAPI`/`WINUSERAPI`/`CALLBACK`
  and blank lines, and takes the first remaining token as the return type; that is what produced the 39/39 agreement
  here. Reuse the pattern for every future TU, and treat "return types match" as unmeasured unless such an instrument
  ran.
- **Inserting a TU comment block invalidates the block's own line references.** `menu.cpp` went 289 -> 355 lines, so
  every `:NNN` the new block cites for a definition in the same file shifts by the block length (+66). Write those
  references from the **post-insert** numbering, or re-point them immediately with an exact-anchor script
  (`tmp/_tmp_s22_lineref_fix3.ps1`), and grep for the old numbers afterwards - `':159|:194|:275|:184|:230|:280'` found
  the leftovers here. The register rows written before the fix carry the same risk.
- **PowerShell flattens nested arrays, which turns a pair table into a character index.** `@( @('a','b') )` is **two
  strings**, so `foreach($p in $pairs){ $p[0] }` yields the character `a` - the session-22 line-reference script reported
  "MISSING/AMBIG x120" for a one-pair file while multi-pair files in the same script worked, because only the single-pair
  case flattened. Use a flat alternating array stepped by 2, or hashtables, and always print the match count per anchor.
- **`[IO.File]::*Text` resolves relative paths against the .NET current directory, not PowerShell's.** In the same
  command, `cd <dir>` then `[IO.File]::ReadAllText('session-log.md')` threw "Could not find file
  `C:\RAID0\Develop\linuxQOR\session-log.md`" while `Get-Item 'session-log.md')` in the **same** statement list resolved
  correctly. Every record-editing script must use absolute paths.
- **A Win32 family is defined by a macro region, not by a line range, and a header can open the region twice.**
  `tmp/_tmp_menu_nomenus_s22.ps1` walks `#ifndef NOMENUS` ... `#endif` spans with a depth counter and counts the
  declaration heads inside each; it found **two** such regions (`winuser.h:7432`-`:8101` and `:10426`-`:10535`) and the
  second holds exactly one stem (`CheckMenuRadioItem`), which is why that one wrapper pairs with a `WINVER` guard
  instead of the region's own condition. Same shape for `NOMDI`/`NOSYSTEM` etc - always enumerate the markers, never
  assume one span.
- **Record hygiene arithmetic:** the ceiling must be measured **after** the row you are about to add. `session-log.md`
  was 45,673 bytes; slicing rows 1-8 (verified `MATCH`) left 40,464 - still over - so the session-22 row would have
  breached it again and rows 9-16 had to go too (`tmp/_tmp_s22_logsplit.ps1`, `_tmp_s22_logsplit2.ps1`, both
  rejoin-and-hash first). Keep the two slice files verbatim, repeat the table header in each, and re-point
  `archive/README.md`.
- **Bypass classification false positives are now known, not guessed.** The U19 classifier reads an unqualified member
  **declaration** in another library (`ui/gui/controllers/.../menu.h:95 Menu* GetSubMenu(int)`) as a BYPASS call site:
  3 of the 18 menu rows are that. Quote row counts and real-call counts separately, and check the file/line kind before
  calling a site a bypass.

### Record-editing tooling and caveats from the U15 closure pass (2 Nov 2026, no TU reviewed)

- **A closed register entry must be removed from the generator's own instructions**, not just from the register file:
  `tmp/_tmp_status_index_gen_s22.ps1` writes the `registers/README.md` preamble that lists the accumulating entries, so
  leaving `u15` in that list would re-tell the next session to append to a frozen entry. It now names
  `(u16, u17, u19, u20)` and states that u15 is closed (policy D14).
- **Regenerate the locator, then splice it, in two separate commands.** One batch ran generator +
  `tmp/_tmp_s22_locsplice.ps1` concurrently, so the splice compared against the previous generated index, printed
  "differing entries: 0" and wrote stale `tasks.md:27/50/83` pointers after the T2 edit had shifted those entries to
  `29/52/85`. Re-running the splice alone fixed it; the batch-race caveat above is the mechanism.
- **New check `tmp/_tmp_u15_locator_verify.ps1`** (read-only): parses every `status.md` locator bullet and asserts the
  `registers/<file>:<line>` it names really starts that id's entry. It reports 44 correct of 45 plus the one deliberate
  exception - U27 points at the shared `registers/p6-u27.md` P6 extract bullet. Run it after any register-entry insert or
  deletion, not just after a split.

