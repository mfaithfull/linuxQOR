# winqor_api_user - tooling index (`tmp/` scratch instruments)

Open before running an instrument or the gate. Procedure: `method.md`. **This file says what each instrument prints;
`gotchas.md` says what it does not see - read that entry before acting on a flag.**

All instruments live in `tmp/` (git-ignored scratch, never committed). Per-TU naming settled in session 21:
`<family>_{prescan,bypass,guards,pp,probe}_s<N>`, with the shared censuses re-run to `<census>_s<N>.txt` and the gate to
`_tmp_cl_all_s<N>.log`. Everything below moved here verbatim from `method.md` on 2 Nov 2026, in the order it was written,
so a session's instruments can be read as one block.

## Standing tooling - what the review method's steps run
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

What these instruments do **not** see - the `pwsh -File` array trap, `_tmp_sigcmp.ps1`'s string comparisons, counts taken
from unstripped source, the export-table parse, the gate wrapper's exit code and whose log is whose - is in `gotchas.md`.

## The compile gate - how it is run, and what has been re-run
Full-library compile after the session-1 edit: 38/38 TUs, `/W4`, exit 0, no diagnostics. Re-run after the session-3
edit: same, 38/38, exit 0, no diagnostics (`tmp/_tmp_cl_all.log`). Re-run after the session-8 binding fix: same, 38/38,
exit 0, no diagnostics - the log's one `warning|error` grep hit is the filename `errorhandling.cpp`, not a diagnostic.
Re-run after the session-9 comment-only edit: same, 38/38, exit 0, no diagnostics (`tmp/_tmp_cl_all_s9.log`, `exit=0
loglines=43`, same single grep hit). Re-run after the session-11 edit (one space + a comment block): same, 38/38, exit 0,
no diagnostics (`tmp/_tmp_cl_all_s11.log`, `exit=0 loglines=43`, same single grep hit). Final session-11 re-run after the
last citation fixes: `exit=0 loglines=43`, and the `cl` log `tmp/_tmp_cl_all.log` shows **38 TU echo lines with 38 fresh
`.obj` files** in `tmp/obj` - the belt-and-braces 38/38 confirmation described above.

Two traps in reading the gate's own output (the wrapper's exit code, and which file is actually the `cl` log) are entries
in `gotchas.md`.

## Sessions 13-20 + the U6 off-protocol fix - sliced out

Those three sections (the sessions 13-16 tooling index, the sessions 17-20 additions and the U6 off-protocol item
fix with the gate rule it established) moved **verbatim** to `archive/tooling-13-20.md` on 3 Nov 2026 when this file
crossed 40 KB; they are the history of how the current standing instruments were built, and no live session needs
them unless it re-derives one. `tmp/_tmp_tooling_preslice_s29.md` is the pre-slice file if the rejoin ever has to be
checked. Everything from session 21 onward stays here.

## Tooling added in session 21 (`mdi.cpp`)
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
  a separate invocation** and keep the batch for the builds. Same lesson as the U6 disassembly probe
  (`archive/tooling-13-20.md`): x64-only evidence is structurally blind to this defect class.
- Per-TU instrument naming settled this session: `<family>_{prescan,bypass,guards,pp,probe}_s<N>` plus the shared
  censuses re-run with an `_s<N>` output suffix (`_tmp_p1census_s21.txt`, `_tmp_tsuffix_s21_full.txt`,
  `_tmp_cl_all_s21.log`). The `_tmp_mdi_guards_s21.ps1` wrapper exists only because `tmp/_tmp_sdk_guard_scan.ps1` takes a
  `-Names` **array** and `pwsh -File script.ps1 -Names A,B` delivers one string - the same trap as the U19 stem wrapper
  recorded in `archive/tooling-13-20.md`. A probe that resolves names must also record that it had to `LoadLibraryW` first:
  `GetModuleHandleW(L"user32")` fails with gle=126 in a bare console process (G5), and a probe that reports every name
  MISSING is measuring the loader, not the export table.

## Tooling added in session 22 (`menu.cpp`)
- **The SDK return type is on its own line, so a declaration-line instrument cannot see it.** In `winuser.h` a
  prototype is `WINUSERAPI` / `BOOL` / `WINAPI` / `Stem(` on four separate lines, so `tmp/_tmp_menu_proto_s22.ps1`
  printed the stem line plus the parameters and reported nothing about the return type - method step 2 looks satisfied
  and is not. `tmp/_tmp_menu_ret_s22.ps1` walks back from the stem up to 4 lines, skips `WINAPI`/`WINUSERAPI`/`CALLBACK`
  and blank lines, and takes the first remaining token as the return type; that is what produced the 39/39 agreement
  here. Reuse the pattern for every future TU, and treat "return types match" as unmeasured unless such an instrument
  ran.
- **A Win32 family is defined by a macro region, not by a line range, and a header can open the region twice.**
  `tmp/_tmp_menu_nomenus_s22.ps1` walks `#ifndef NOMENUS` ... `#endif` spans with a depth counter and counts the
  declaration heads inside each; it found **two** such regions (`winuser.h:7432`-`:8101` and `:10426`-`:10535`) and the
  second holds exactly one stem (`CheckMenuRadioItem`), which is why that one wrapper pairs with a `WINVER` guard
  instead of the region's own condition. Same shape for `NOMDI`/`NOSYSTEM` etc - always enumerate the markers, never
  assume one span.

## Record-editing tooling from the U15 closure pass (2 Nov 2026, no TU reviewed)
- **A closed register entry must be removed from the generator's own instructions**, not just from the register file:
  `tmp/_tmp_status_index_gen_s22.ps1` writes the `registers/README.md` preamble that lists the accumulating entries, so
  leaving `u15` in that list would re-tell the next session to append to a frozen entry. It now names
  `(u16, u17, u19, u20)` and states that u15 is closed (policy D14).
- **`tmp/_tmp_u15_locator_verify.ps1`** (read-only): parses every `status.md` locator bullet and asserts the
  `registers/<file>:<line>` it names really starts that id's entry. Every bullet must resolve except the one deliberate
  exception - U27 points at the shared `registers/p6-u27.md` P6 extract bullet. Run it after any register-entry insert or
  deletion, not just after a split.

## Tooling added in session 23 (`message.cpp`)
- **`tmp/_tmp_msg_class_s23.ps1`** - per-stem bare-vs-AW classifier (for each stem: is `<stem>` exported, do `<stem>A`/`<stem>W`
  exist, does `winuser.h` define a `Stem` macro?). This is the instrument that answers the D13 question per TU and it is the
  one to clone for the remaining bare-binding TUs (`mouse.cpp`, `window.cpp`); session 22's `_tmp_menu_class_s22.ps1` was its
  first form. Output `tmp/_tmp_msg_class_pre_s23.txt` / `_post_s23.txt` - **compare them**, they are byte-identical when a
  bare -> `AW` conversion is behaviour-neutral.
- **`tmp/_tmp_msg_ret_s23.ps1`** - session 22's return-type instrument pointed at `winuser.h`; monitor/message stems live in
  `winuser.h` too, where the return type is on the line above the stem and the qualifier lines (`WINUSERAPI`, `_Check_return_`)
  above that. It reported **0 of 24** divergences; the two `CALL TYPE MISMATCH` rows it emits for `InSendMessageEx` and
  `PostQuitMessage` are artifacts - the "sdk ''" side is this library's own prototype, and the reported "types" are the *next*
  function's `Call<>` list. Always read a `CALL TYPE MISMATCH` against the source lines before acting.
- **`tmp/_tmp_msg_regions_s23.ps1`** (U21 region census: every `#ifdef`/`#ifndef` name that opens a region containing a
  section), **`_tmp_msg_guards_s23.ps1`** (U16 `WINVER` guards per SDK declaration), **`_tmp_msg_bypass_s23.ps1`** +
  **`_tmp_msg_u19_s23.ps1`** (U19 LAYER/INDIRECT/BYPASS/MEMBER), **`_tmp_msg_pos_s23.ps1`** (def positions vs header order),
  **`_tmp_msg_pp_s23.bat`** (own `/EP` output files, per the shared-`_tmp_pp_out.txt` caveat).
- **`tmp/_tmp_u14c_s24.ps1`** closes the U14(c) count and is named `_s24` although session 23 ran it - a naming slip; keep
  instrument names tied to the session that ran them, later searches for `_s23` miss it.
- **`tmp/_tmp_s23_closeout.ps1`** - the **record close-out audit**, six checks: gate log (dated by its own redirect, not by
  a copy), `session-log` slice rejoin hashes + row-id uniqueness across the four slices, record ceilings, `status.md`
  locator pointer resolution, `archive/` + `registers/` README coverage, and edited-TU hygiene/binding counts. Run it
  before writing a session's row and again before committing; output `tmp/_tmp_s23_closeout.txt`. Two counting rules it
  encodes, both of which bit this session: a range placeholder row (`| 24..38 | not started |`) is **not** a session id, and
  a TU's contract block names the binding macros in prose (`message.cpp:20`), so binding counts must exclude comment lines -
  the real `message.cpp` split is **12 `AW` / 12 bare over 24 definitions**, which is what its contract block claims.

## Tooling added in session 24 (`monitor.cpp`)
Instruments (all session-scoped names, `tmp/`): **`_tmp_mon_pp_s24.bat`** (own `/EP` output pair, per the
shared-`_tmp_pp_out.txt` caveat); **`_tmp_mon_class_s24.ps1`** - the per-stem bare/AW/P6 classifier, a clone of
`_tmp_msg_class_s23.ps1` with the stems swapped, parameterised by `-PpFile` so the pre and post `/EP` outputs run through
the same code path; **`_tmp_mon_mirror_s24.cpp` + `_tmp_mon_mirror_s24.bat`** - the third struct-mirror probe
(`MONITORINFO`/`MonitorInfo`, `MONITORINFOEXW/A`/`MonitorInfoExW/A`, `POINT`/`Point`, `RECT`/`Rect`,
`MONITORENUMPROC` argument widths), built for **x64 and x86 in one batch run**; **`_tmp_mon_probe_s24.cpp` +
`_tmp_run_mon_probe_s24.bat`** - the live `GetProcAddress` name probe (the `*T` and `Ex` spellings are included so the
probe also disproves the spellings the TU does *not* ask for); `_tmp_mon_pre_s24.ps1`, **`_tmp_mon_region_s24.ps1`** - the
SDK region walker (walks the `#if(WINVER >= 0x0500)` block opened at `winuser.h:13594` with a `#if/#endif` depth counter
and prints every declaration head **plus which TU defines it**, which is how U21's 45-head region got its 6/17/22
attribution; the earlier `_tmp_mon_pre2_s24.txt` hand-count of the same region guessed "34 in `window.cpp`" and was
wrong - measure, do not bucket), `_tmp_mon_guards_s24.ps1`, `_tmp_mon_bypass_s24.ps1`, and
**`_tmp_s24_closeout.ps1`** - the record close-out audit, seven checks (gate log dated by its own redirect and compared
against the last source edit; record ceilings; every locator pointer resolving to the named line; the generated locator
matching the spliced block; `archive/` + `registers/` index coverage; TU hygiene; `/EP` multiset neutrality).

Session 26 (`painting.cpp`, 3 Nov 2026) instruments: `_tmp_paint_pp_s26.bat` (own `/EP` output files), `_tmp_paint_ret_s26.ps1`
(multi-line winuser.h return+params dump for the 25 stems), `_tmp_paint_class_s26.ps1` (per-stem bare/AW/P6 classifier),
`_tmp_paint_pre_s26.ps1` (header/def/SDK three-way name compare for the section), `_tmp_paint_guards_s26.ps1` (innermost-guard
walk), `_tmp_paint_probe_s26.cpp` + `_tmp_paint_probe_s26.bat` (x64 + x86 GetProcAddress probe AND a runtime A/W mirror: it
measures what `DrawStateA` vs `DrawStateW` / `GrayStringA` vs `GrayStringW` actually draw with identical opaque arguments),
`_tmp_s26_pre.ps1` (one wrapper that re-runs the four folder-wide scans; remember `pwsh -File x.ps1 -Stems A,B` delivers ONE
string, not an array - call the scans from a wrapper script instead). **New PowerShell gotcha, same family as the `-ne` case
trap above:** `-replace 'T$'` is **case-insensitive**, so stripping the folder's `T` suffix from a name ending in a lowercase
`t` eats the wrong character - `BeginPaint` became `BeginPain` and six stems silently fell out of the SDK-side compare until
switched to `-creplace`. `_tmp_sdk_param_name_scan.ps1:238-244` had already documented this trap and avoided it by trying the
exact spelling first; any new instrument that strips `T` MUST use `-creplace`. Two more for the close-out:
`_tmp_paint_lines_s26.ps1` (prints the current def/binding/call-site line numbers of the TU - the only safe way to
re-point a contract block's self-citations after an insertion) and `_tmp_s26_closecheck.ps1` (block self-citations +
spot-checks + record sizes). It improves on `_tmp_s25_verify.ps1`: external pointers are classified by an explicit
`-ExternalZones` line-range list rather than flagged as stale, so the run ends at **0 out of range** instead of 4.

## Tooling added in session 27 (`processthread.cpp`, 3 Nov 2026)

Session-scoped names in `tmp/`. **`_tmp_pt_ret_s27.ps1`** - the multi-line `winuser.h` return-type + parameter dump for
the 5 stems (the SDK puts the return type on its own line above the stem, so a declaration-line scan cannot see it).
**`_tmp_pt_class_s27.ps1`** - the per-stem bare/AW/P6 classifier over a `/EP` capture (`-PpFile`, so pre and post run
through the same code). **`_tmp_pt_pre_s27.ps1`** - header / TU-definition / SDK three-way parameter-name compare for one
section (hdr-vs-def is exact, def-vs-sdk advisory because of the SAL prefixes and the multi-line prototypes).
**`_tmp_pt_guards_s27.ps1`** - the innermost-guard walk with an `#if`/`#endif` depth counter (session 25's crossed
`#endif` comment caveat, now also the guard against **empty** guard pairs - see `gotchas.md`).
**`_tmp_pt_pp_s27.bat`** - own `/EP` output pair (the shared `_tmp_pp_one.bat` writes `_tmp_pp_out.txt`, easy to read
stale). **`_tmp_pt_lines_s27.ps1`** - the TU's def/binding/call-site line numbers plus hygiene counts (tabs, trailing
space, non-ASCII, longest line) and the per-site spaced/tight `Call<` split: the only safe way to re-point a contract
block's self-citations after an insertion. **`_tmp_pt_edit_s27.ps1`** - the whitespace pass itself: substring-scoped
replacements, each asserted to match **exactly once**, refusing to write otherwise.
**`_tmp_pt_coverage_s27.ps1`** - the coverage instrument the review had never run: which exported names have **no**
member declaration in `user32.h`, splitting them into declared-in-`winuser.h` (SDK-visible gap) vs not declared there at
all (unreachable, not a gap). First run: 1,018 exports -> 370 unwrapped, 114 SDK-visible, 256 undeclared.
**`_tmp_pt_cite_s27.ps1`** - the contract-block citation verifier: resolves **every** `:NNN` in the block against the
file it belongs to (a bare citation inherits the last file marker in that paragraph, and "(in this TU: ...)" marks a
self-citation), asserts each def/binding self-citation lands on the right construct, and reports unresolved targets. It
caught a wrong path (`gui/CMakeLists.txt` -> `ui/gui/CMakeLists.txt`) and three citations that had drifted 6 lines when a
paragraph grew. **`_tmp_pt_neutral_s27.ps1`** - the `/EP` pre-vs-final neutrality diff (line counts, non-blank multiset,
per-literal byte counts). **`_tmp_s27_pre.ps1`** - one wrapper re-running the four folder-wide scans (P1 census, wrong-stem
scan, U14 census, U19 bypass scan over every suffix variant). **`_tmp_s27_logslice.ps1`** - the session-log slice with
per-row sha256 verification against the pre-slice snapshot.

**Standing-instrument fix (session 27): `_tmp_sigcmp.ps1` could not see wrapped call sites.** Its call-site pattern
matched `Library::Call<...>(pFunc, ...);` on **one** line, so every multi-line call printed `CALL NOT PARSED` and the
arity/type half of the check silently never ran - 3 of `processthread.cpp`'s 5 sites are wrapped, and `painting.cpp`,
`monitor.cpp` and `mouse.cpp` have the same shape. It now joins lines to the terminating `;` before matching, which is why
the TU record can claim 5/5 call sites "ok". Any earlier session record that quotes `CALL NOT PARSED` for a multi-line
site did **not** have that check.


**Two more instruments from the same close-out pass (3 Nov 2026).** **`_tmp_rcinclude_scan.ps1`** - the
`returncheck.h` include census written for **T7**: `-Dir` any api_layer folder, it lists the sources that include
`"../returncheck.h"`, the ones with a code hit for any of that header's 14 declared names (with the line numbers) and the
**include-without-use** set. It skips `//`-leading lines on purpose - `devicecontext.cpp:29` quotes `CheckReturn<>` in a
comment and would otherwise read as a user. user 37/2/35, kernel 22/20/2. **`_tmp_rect_pre_s28.ps1`** - the pre-flag
sweep for the next TU (`rectangle.cpp`): the SDK guard stack for the 11 `//Rectangle functions` prototypes (all in the
Desktop sub-block from `winuser.h:9717`, **no** version guard on any of them), the SDK parameter spellings, the
bare/A/W + macro table for the 11 stems (all bare, no family, no macro) and the include census for the folder. It reads
only; its output is `tmp/_tmp_rect_pre_s28.txt`.

**Self-fix to a session instrument (same pass).** `_tmp_pt_neutral_s27.ps1` compared the non-blank multiset over
`_tmp_pt_pp_pre_s27.txt` vs `_tmp_pt_pp_final_s27.txt` but counted binding literals over `..._post_s27.txt` - a different
capture. Both are post-edit here so the result stands, but the pair mismatch is exactly how a neutrality claim can go
wrong quietly, so the script now uses `final` for both halves.

**Close-out instruments (session 27).** **`_tmp_s27_verify.ps1`** - the one-run verifier, output
`tmp/_tmp_s27_verify.txt`: gate log newer than the newest `user/*.cpp` edit plus 38/38 echoes and 0 diagnostics, the `/EP`
neutrality pair (both halves over ONE capture), TU state (defs, bindings, tabs, trailing space, spaced/tight `Call<>`)
counted over **code lines only** - the contract block quotes `qor_pp_useswinapi` and `Call<` in prose, which is how a
first version reported 6 bindings for a 5-wrapper TU, and that reading error is exactly the kind this check exists to
catch; the three folder measures, the citation-verifier tail, every record file against its size/line ceiling,
session-log integrity (row 27 present verbatim, rows 23-24 present only in the slice, placeholder advanced, no stray
`|` lines, file not 0 bytes), the `archive/README.md` index rows and `user32.h` still 695 lines.
**`_tmp_s27_logrestore.ps1`** - the recovery script written after the 0-byte incident (`gotchas.md`): rebuilds
`archive/session-log.md` from `tmp/_tmp_s27_preslice_sessionlog.md` by replaying `_tmp_s27_logslice.ps1`'s exact
transformation, then re-inserts row 27 and the close-out note; it verifies the sliced rows' sha256 and that every snapshot
line except the sliced pair is reproduced **before** it writes, and refuses to write an empty document.

## Tooling added in session 28 (`rectangle.cpp`, 3 Nov 2026)

Session-scoped names in `tmp/`. **`_tmp_rect_ret_s28.ps1`** - the multi-line `winuser.h` return-type + parameter dump
for the 11 stems (session 22's walk-back-off-the-stem pattern, cloned from `_tmp_pt_ret_s27.ps1`); it is what lets the
record say "every return is `BOOL`" rather than "unmeasured". **`_tmp_rect_class_s28.ps1`** - the per-stem
bare/AW/P6 classifier over a `/EP` capture; for this TU it also proved the *negative*: 0 macros and 0 A/W families,
so P2/P6 are unrepresentable and **no live probe is needed** (a probe only matters when some spelling is being asked
for that the DLL might lack). **`_tmp_rect_pair_s28.ps1`** - header/TU/SDK three-way parameter-name compare.
**`_tmp_rect_lines_s28.ps1`** - the def/binding/call-site line map plus hygiene counts, extended with two censuses
this TU needed: the space-before-comma sites (`int , int` at old `:69`, a shape no earlier TU had) and the three
`CONST` spellings counted **per line** (`CONST::` on the definition lines, `CONST ::` on the call lines, bare
`CONST X*` inside two template lists) so a U25 claim can be attributed to a construct. **It counts constructs over the
code region only** (from the first `^\s*BOOL User32::` line) while hygiene stays whole-file - the final capture of the
first version read `fcontext: 12` and `CONST` on three block lines, see `gotchas.md`. **`_tmp_rect_edit_s28.ps1`** -
the whitespace/`Call<>`-spacing pass, **self-locating**: it takes every `Library::Call` line from the first
`BOOL User32::` definition on and asserts exactly 11 changed (see `gotchas.md` for why the first, line-number-based
version was wrong). **`_tmp_rect_neutral_s28.ps1`** - `/EP` pre-vs-final neutrality (both halves over ONE capture
pair, the session-27 self-fix applied from the start). **`_tmp_rect_cite_s28.ps1`** - session 27's citation verifier
cloned for this TU. **`_tmp_s28_pre.ps1` / `_tmp_s28_post.ps1`** - the four folder-wide scans in one wrapper (P1
census, wrong-stem, U14 census, U19 bypass over all 44 suffix variants). **`_tmp_rect_pp_s28.bat`** and
**`_tmp_rect_cl_s28.bat`** - own `/EP` and compile captures (never the shared `_tmp_pp_out.txt`).

**Two new coverage instruments, worth cloning for the remaining TUs.** `_tmp_rect_region_s28.ps1` lists every
prototype head in a `winuser.h` line range and says whether `user32.h` has a member for it - which is how
`GetWindowWord`/`SetWindowWord` (no member at all) and `SetSysColors` (declared at `user32.h:651`, defined nowhere)
surfaced from a TU that wraps none of them. Its blind spot is recorded in `gotchas.md`: `GetWindowLongA`/`W` read as
"no member" because the layer declares `GetWindowLongT`. `_tmp_rect_family_s28.ps1` fixes that by resolving
`stem -> stemT` (and `stemA/W -> stem -> stemT`) before deciding, and works from the export table rather than a line
range: 51 export names containing `Rect`, 41 SDK-declared, 39 wrapped. Use the **family** form for claims and the
**region** form to spot neighbours.

**`_tmp_s28_logslice.ps1`** - the session-log slice for rows 25-26 (clone of `_tmp_s27_logslice.ps1`): snapshot, hash
the two rows, write the slice, re-read and hash-verify **before** rewriting the source, and refuse to write an empty
document. Its final line ("non-slice lines that changed, expect 1") reported 3 - the two blank lines the inserted
paragraph contributes are not in its exclusion filter, so that count is a soft signal, not a proof; the hard proof is
the `Compare-Object` against the snapshot, which shows exactly 2 rows removed and 6 lines added.




## Session 29 (`resource.cpp`, 3 Nov 2026) - all `_tmp_res_*`

- **`_tmp_res_pre_s29.ps1` -> `_tmp_res_pre_s29.txt`** - the pre-flag capture (SDK halves of the two members, the guard
  stack, header/definition shape, TU hygiene). Its findings are the pre-flag block that was in `status.md`.
- **`_tmp_res_scan_s29.ps1` -> `_tmp_res_scan_final_s29.txt`** - the per-TU step set in one script: `sigcmp` over
  `user32.h:443-444` vs the TU, the U4 reverse census (`static` declarations with no definition), the wrong-stem scan
  over all 38 files, the T-suffix census, the U19 bypass over the 8 spellings of the TU's stems, and the `Call<>`
  bracket/paren census. Re-runnable per session; the `4b` section is what U4's folder count reads from.
- **`_tmp_res_syms_s29.bat` -> `_tmp_res_exports_s29.txt` / `_tmp_res_lnkimp_s29.txt`** - the *published-symbol* census:
  `dumpbin /exports` of the built DLL and `dumpbin /linkermember:1` of the built import library, filtered for the TU's
  names. The instrument for "can anyone link this wrapper at all", and the one that proved U11's two free functions were
  never published (439 exports / 854 member symbols, neither name present). Needs the artefact dates checked first
  (`gotchas.md`).
- **`_tmp_res_ver_s29.cpp`** - a 6-line TU that references both spellings, compiled standalone so `dumpbin /symbols`
  shows exactly what the definitions emit (free `YA` vs member `SA`).
- **`_tmp_res_post_s29.bat`, `_tmp_res_final_s29.bat` -> `_tmp_res_final_s29.log`** - the gate wrapper: single-TU `/W4`,
  `/EP` capture, `dumpbin /symbols` of the fresh object, then the consumer probe compile **and link**.
- **`_tmp_res_ref_s29.cpp`** - the **consumer-side link probe**: takes the address of both members and links against the
  TU's own object. This is the clean way to prove a declaration/definition pair really closes, and it is worth cloning
  for every U4 / T1-T6 session: expect **0 unresolved externals for the tested members**, with the cross-module noise
  (`ThisModule`, `qor::FunctionContext`, `qor::When/Where/What/Serious`, `Kernel32::GetModuleHandleW/GetProcAddress`)
  allowed. Count with the gotchas.md predicate, not a bare grep.
- **`_tmp_res_evid_s29.ps1`** - extracts a clean evidence summary (exit markers, emitted `External` member symbols,
  unresolved-name list) out of the mixed dumpbin/link log.
- **`_tmp_res_neutral_s29b.ps1`** (pre-fix vs post-fix `/EP`, positional + hash-multiset, finds the 4 differing lines by
  number) and **`_tmp_res_neutral_final_s29.ps1`** (adds the contract-block leg: +11 lines, all blank, non-blank
  identical) - both hash-based; the `-notcontains` originals are kept as the failed first attempts.
- **`_tmp_res_lines_s29.ps1` / `_tmp_res_finalchk_s29.txt`** - TU shape and hygiene with the session-28 scope rule
  applied, plus the gate dating proof (source vs object mtimes) and the blank/non-blank census of the three `/EP` captures.
- **`_tmp_res_blockchk_s29.ps1`** - which sources carry a TU contract block (the census's "every reviewed TU" claim);
  prints lines / comment lines / longest comment run per file, and is what showed `resource.cpp` was the only reviewed TU
  without one.
- Logs: `_tmp_res_cl_s29.log` (single TU), `_tmp_cl_all_s29b.log` (**this session's authoritative 38-TU gate**),
  `_tmp_res_link_final_s29.txt`, `_tmp_res_pp_final_s29.txt`, `_tmp_res_syms_final_s29.txt`.
- **`_tmp_status_locator_s29.ps1`** - splices the generated locator (`tmp/_tmp_status_index_s22.md`) into `status.md`
  mechanically: it finds the contiguous run of `- **<id>**` bullets, replaces exactly that run, and prints the before/after
  line counts. Use it instead of hand-editing the locator, because the generator re-points every pointer when a register
  file grows (this session: `P1 :107 -> :113`, `P5 :117 -> :123`, plus the new `P5 :98` row). Run
  `_tmp_status_index_gen_s22.ps1` **after** the register edits and **before** the splice.
- **`_tmp_s29_cite_audit.ps1` -> `_tmp_s29_cite_audit.txt`** - the **record-integrity** instrument: regex-harvests every
  `tmp/_tmp_*.ps1|bat|cpp|txt|log|csv|md|obj` citation out of the live record files (`status.md`, `tooling.md`,
  `gotchas.md`, `archive/session-log.md`, the current TU narrative, the register files touched this session,
  `memory-bank/activeContext.md`) and reports MISSING against the filesystem. Run it **at session close**, after the
  records are written: it is how a mis-cited instrument gets caught while it is still cheap to fix. Session 29's run:
  194 cited artefacts, **1 missing** - `census.md:132` named the census instrument `_tmp_p1census_s20.ps1` under a `tmp/`
  path, a file that never existed; the real instrument is `tmp/_tmp_kb_p1census_s20.ps1` (`tooling.md`'s "binding census
  to run in every session"), and the census line is now corrected. Note the self-reference trap: writing the bad path in
  the fix's own record makes the next run report it again, because the audit scans this file too - name the bad path
  **without** its `tmp/` prefix. Output is sorted, so the MISSING lines are one grep away.
- **Record size after this session:** `tooling.md` **was sliced in-session** - it reached 41,067 B, so the three oldest
  post-index sections (was `:65-126`, sessions 13-16 + 17-20 + the U6 off-protocol fix) moved verbatim to
  `archive/tooling-13-20.md` by `tmp/_tmp_tooling_slice_s29.ps1` (hash-verified rejoin against
  `tmp/_tmp_tooling_preslice_s29.md`); this file sits just inside the 40 KB cap at close (`tmp/_tmp_s29_sizes.txt` has
  every live record file's size). Two internal "as ... above" references that the slice orphaned were re-pointed at the
  archive file (`:92`, `:98`). **This file is the one to slice at the start of session 30**, before adding that session's
  instruments; `status.md` also needs its session bullets trimmed against its 200-line / 20 KB cap. Do not write this
  file's own size here - the sentence changes it.
- **`_tmp_scroll_pre_s30.ps1` -> `_tmp_scroll_pre_s30.txt`** (185 lines) - the session-30 pre-flag capture for
  `scrollbar.cpp`: TU shape/hygiene (102 lines, 4,079 B, 12 definitions at `:16,23,30,37,44,51,59,66,74,81,88,95`, longest
  line 154 at `:66`), their `user32.h:574-585` declarations under the `//ScrollBar functions` label at `:573`, the bare/A/W
  export shape per stem (all 12 bare single exports - no A/W family, so the bare `qor_pp_useswinapi` at all 12 sites is the
  correct D13 spelling and P1/P2/P6 cannot arise), the SDK guard stack per name (**four** groups: `ScrollWindow` `:8766`,
  `ScrollDC` `:8776`, `ScrollWindowEx` `:8788` under the Desktop partition only (`:8751`); `SetScrollPos` `:8816`,
  `GetScrollPos` `:8825`, `SetScrollRange`, `GetScrollRange` `:8842`, `ShowScrollBar` `:8851`, `EnableScrollBar` `:8859`
  under `#ifndef NOSCROLL` (`:8811`); `SetScrollInfo` `:12052`, `GetScrollInfo` `:12061` under `#if(WINVER >= 0x0400)`
  (`:12026`) + `NOCTLMGR`/`NOWINMESSAGES`; `GetScrollBarInfo` `:14763` under `#if(WINVER >= 0x0500)` (`:13594`) +
  `NOWINABLE` - a **U16** candidate), and the U19 tally (8 LAYER / 8 INDIRECT in
  `ui/gui/controllers/s_s/scrollbarcontroller.cpp`, **0** bypass). Clone its structure for the remaining TUs; note it must
  call the helper scripts with `& $script -Names $array`, not `powershell -File ... -Names ($array -join ',')`.

