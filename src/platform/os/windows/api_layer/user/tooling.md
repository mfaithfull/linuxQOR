# winqor_api_user - tooling index (`tmp/` scratch instruments)

> **FROZEN at session 34 (5 Nov 2026).** Provenance only: not boot material, never appended to, never read unless a named closed session is being researched. Per-TU instances now live in the review banner of the source file itself (`status.md`, protocol v2).

Open before running an instrument or the gate. Procedure: `method.md`. **This file says what each instrument prints;
`gotchas.md` (the index; the caveats are in `gotchas/`) says what it does not see - read that entry before acting on a
flag.**

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

## Sessions 21-26 (`mdi`, `menu`, the U15 closure pass, `message`, `monitor`, `painting`) - sliced out

Those sections (sessions 21-24 under their own headings, sessions 25 and 26 as paragraphs at the tail of the
session-24 section) moved **verbatim** to `archive/tooling-21-26.md` on 3 Nov 2026 when this file approached the
40 KB ceiling again after session 29 slice had run; they are the history of how the standing per-TU instruments
(per-stem classifier, return-type dump, region walker, mirror probe, close-out audits) were built, and no live
session needs them unless it re-derives one. `tmp/_tmp_tooling_preslice_s30.md` is the pre-slice file if the
rejoin ever has to be checked. Everything from session 27 onward stays here.

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

## Tooling added in session 30 (`scrollbar.cpp`, 3 Nov 2026)

Session-scoped names in `tmp/`. **`_tmp_scroll_pp_s30.bat`** (own `/EP` output pair, per the shared
`_tmp_pp_out.txt` caveat); **`_tmp_scroll_ret_s30.ps1`** (the winuser.h return+params dump for the 12 stems, a clone of
`_tmp_pt_ret_s27.ps1`); **`_tmp_scroll_names_s30.ps1`** (header / definition / SDK three-way parameter-name compare for
one section - **and the fix: the compare is `-cne`, not `-ne`**. `_tmp_paint_pre_s26.ps1` and `_tmp_pt_pre_s27.ps1` both
compared with `-ne`, which is case-INSENSITIVE, so a case-only name divergence in that family compares equal; the
folder-wide `_tmp_name_divergence_scan.ps1` is separately case-sensitive by construction, which is why the two
instruments disagree in appearance and only this one is wrong); **`_tmp_scroll_family_s30.ps1`** (export-table family
walk: every export whose name contains a substring, which of them the SDK declares, which the layer wraps - the
instrument that finds SDK-undeclared exports like `ScrollChildren`); **`_tmp_scroll_children_s30.ps1`** (recursive
`um`/`shared`/`ucrt` header grep for a stem, plus per-stem SDK declaration-site counts);
**`_tmp_scroll_regions_s30.ps1`** (region opens/closes + typedef containment + bare `#define <Stem>` alias count);
**`_tmp_scroll_guards_s30.ps1`** and **`_tmp_scroll_stack_s30.ps1`** (innermost guard, and the full depth-counted guard
stack per prototype); **`_tmp_scroll_mirror_s30.cpp` + `.bat`** (caller-mirror `sizeof`/`offsetof` probe, **x64 and x86
in one batch** - and note the C++ gotcha it hit: `wprintf(L"%s", char*)` is a vararg type mismatch under `/W4`, so print
`char*` with `printf` and keep the wide names in a parallel `const char*` table);
**`_tmp_scroll_g4probe_s30.cpp` + `.bat`** (the G4 re-measurement: same module for every token case, `NULL` for a bogus
name, and per-entry-point `GetProcAddress` address identity across spellings, both arches);
**`_tmp_scroll_neutral_s30.ps1`** (the `/EP` neutrality diff that classifies each differing line - "only the module
literal" vs "any other kind" - and compares binding-literal multisets; the instrument that turns a token change from an
assertion into a proof); **`_tmp_s30_ep_commentproof.ps1`** (strips comments and compares **code-only** `/EP` hashes, so
a comment-block insertion is proved neutral by `154,346 == 154,346` and one SHA-256 instead of an 87-line raw diff);
**`_tmp_scroll_callers_s30.ps1`** (names the consumer chain one level deep and whether it is in a CMake target - the
measurement D14 needs before a "no caller" statement is worth writing); `_tmp_s30_hygiene.txt`,
`_tmp_s30_measure2.txt`, `_tmp_s30_final_measures.txt`, `_tmp_s30_gate_final.txt`, `_tmp_s30_gatecheck.txt`.

**Record-editing method change agreed this session (context budget).** Sessions 21-29 re-read the whole record set
(~200 KB of markdown) while writing their close-out, which is what kept forcing compaction mid-close-out. Session 30's
records were instead generated by one script from the session's own `tmp/_tmp_*_s30*.txt` outputs: **one digest read, one
write pass** (`_tmp_s30_digest.ps1` -> `_tmp_s30_digest.txt`, then `_s30_*.txt` fragments + an assembler). The next
sessions should keep that shape, and should slice `gotchas.md` (30 KB - **done 4 Nov 2026**, the caveats are now in
`gotchas/`, see "How `gotchas.md` was split" in `archive/history.md`) and freeze the closed per-TU rows of
`registers/u16.md` (27 KB) and `registers/u19.md` (26 KB) the way `tooling.md` was sliced in sessions 29-30.
`archive/tooling-21-26.md` is this session's slice of `tooling.md:73-184`.


## Added 4 Nov 2026 (session 32, `text.cpp`)

| Instrument | What it prints | Notes |
|---|---|---|
| `tmp/_tmp_s32_preflags.ps1` | per-TU const-site spellings, binding macros, binding literals, guard/version lines for a TU | generic; take `-Tu`-style paths inline |
| `tmp/_tmp_text_pp_s32.bat <src> <out.txt> <out.err>` | `cl /EP /P` capture of one TU (the 453k-line stream) | never print it; feed it to the neutrality diff |
| `tmp/_tmp_s32_neutral.ps1` (+`_neutral2.ps1` for the post2 capture) | line-count delta, **non-blank difference count**, and the binding-literal check between two `/EP` captures | the comment-edit gate; see `gotchas.md` on naming captures |
| `tmp/_tmp_s32_pplits.ps1` | `User32` token split (code vs comment) for one file set at a time | the file-split regex cannot take two overlapping `-InFile` sets |
| `tmp/_tmp_s32_guards.ps1` | `#if`/`#endif` ranges and guard names around a TU's members (`tmp/_tmp_s32_guards.txt`) | source of the `NODRAWTEXT` and `WINVER >= 0x0400` findings |
| `tmp/_tmp_s32_tcallers.ps1` | unqualified definition heads + caller census across `src` for a stem list | found `windowproperties.cpp:173` (U12/U13) |
| `tmp/_tmp_s32_family.ps1` | per-file member counts (T-suffixed vs not), bound stems, macro-vs-direct binding | used for the queue/family cross-check |
| `tmp/_tmp_s32_names.ps1` | **parameter-name and type comparison, definition vs SDK declaration**, with the corrected const matcher | supersedes `_tmp_sigcmp.ps1` for const typedefs (`gotchas.md`) |
| `tmp/_tmp_s32_dbg.ps1` | small helper that prints the intermediate match sets the naming scan used | debugging only |
| `tmp/_tmp_s32_u23_probe.cpp` / `_neg.cpp` + `tmp/_tmp_s32_u23_probe.bat` | positive/negative probe that proves `CONST LPINT` == `INT * const` | the U23 evidence; run the negative too |
| `tmp/_tmp_s32_gatecheck.ps1` | gate-log TU echoes, error/warning lines, **stale or missing `.obj`**, `text.cpp` vs `text.obj` freshness | the only sound way to date the gate (`gotchas.md`) |
| `tmp/_tmp_s32_queue_audit.ps1` | for each of the 38 `.cpp` files, the number of session-log rows naming it | **re-derives the Queue table**; it exposed the duplicated `text.cpp` row and the missing `windowprocedure.cpp` row |
| `tmp/_tmp_s33_preflags.ps1` | first-pass counts for `window.cpp` | **broken head regex - reported 0 heads; do not trust it**, use the direct `Select-String` counts in the session-33 pre-flags |
| `tmp/_tmp_s33_prebind.ps1` | every `window.cpp` binding classified `bare-export` / `AW-macro-needed` / `NEITHER` against the export table | run it before reading its `.txt`; the first output was the pre-fix all-`NEITHER` run |
| `tmp/_tmp_s32_slice_log.ps1` | **the reference record splice**: measures a line range, verifies `head + block + tail == original` by SHA256 **before** writing, writes the slice with the block hash in its header, leaves a pointer, prints both sizes | use this whenever a record file passes ~40 KB; run it, then re-point `archive/README.md` and the `status.md` locator |

## Added 4 Nov 2026 (session 33, `window.cpp`)

Two session-33 instruments are already indexed at the end of the session-32 table (`_tmp_s33_preflags.ps1` - **broken
head regex, do not run**; `_tmp_s33_prebind.ps1` - per-TU binding classifier, re-run before reading its `.txt`).

| Instrument | What it prints | Notes |
|---|---|---|
| `tmp/_tmp_s33_pre.ps1` | per-member table for a TU: bind spelling, export class, SDK declaration line + A/W suffix, bare-`#define <Stem>` macro site, **version floor**, other guard stack (`tmp/_tmp_s33_final.txt`) | the U16 source for this TU: 85 `bind:` rows, 34 with `floor[if ...]`; `floor[PARSER MISS]` = no SDK declaration, not a walk failure |
| `tmp/_tmp_win_pp_s33.bat` + `tmp/_tmp_win_ppclass_s33.ps1` | `/EP`-authority binding audit: binding-line count, module literals, distinct name literals **not in the export table**, `…WW` count, `…T` count | keeps only lines matching `DefProc pFunc`; pairs quotes with a plain `"` scan; uses `-cnotmatch` (case-sensitive) against `tmp/_tmp_user32_exports.txt` - the instrument that proved the `CreateWindowW` P1 |
| `tmp/_tmp_win_bypass_s33.txt` + `tmp/_tmp_s33_u19sum.ps1` | U19 counts for the TU's stem family (defs, LAYER, INDIRECT, BYPASS list, MEMBER false positives, members with no caller, api_layer-internal vs elsewhere) | INDIRECT stays an upper bound (same-named member definitions); the sum script is the cheap way to read a 340-variant scan |
| `tmp/_tmp_win_mirror_s33.cpp` + `.bat` | `sizeof` + every `offsetof` of the caller-side mirrors vs the SDK structs, **x64 and x86 in one batch** | 8 structs matched on both arches; extend it when a new mirror reaches a TU |
| `tmp/_tmp_win_consumer_s33.bat` | compiles `ui/gui/controllers/windowcontroller.cpp` against the edited layer (captures `_tmp_win_consumer_s33.log` / `_s33b.log` / `_s33c.log`) | the probe that makes a published-signature change safe; `CL_EXIT=0`, 11 warnings (C4100 x9, C4189 x2 pre-existing in `ui`), and the three captures are line-identical pre/post - there is no `..._s33b.bat`, citing one is a fabricated name |
| `tmp/_tmp_s33_verify.ps1` | file line count, definition-head line map, spaced/tight call parens, `const` sites, comment-stripped code counts | its `\bCONST\b` test is **case-insensitive** (reports 8 where the truth is 1) and its P5 regex flags a space before `>` - not the U14 class |
| `tmp/_tmp_s33_gatecheck.ps1` | gate-log TU echoes + diagnostics, and every `tmp/obj/*.obj` older than its `.cpp` | the only sound way to date a gate; re-point it at the current TU |
| `tmp/_tmp_s33_slice_log.ps1` | the record splice for `archive/session-log.md` (hash-verified rejoin before writing) | left `session-log.md` 233 lines / 29.0 KB, slice `session-log-notes-31-33.md` 135 lines / 12.9 KB |
| `tmp/_tmp_s33_rec1.ps1` | digest of the saved s33 captures: floor histogram, guard-name populations, partition rows, U14 pre/post totals | the one-digest pattern from session 30 - read this, not the raw captures |
| `tmp/_tmp_s33_preflag_retire.ps1`, `tmp/_tmp_s33_status_compress.ps1` | retire a consumed pre-flag block / compress `status.md` to its ceiling, saving what was removed under `tmp/` | both anchor on line prefixes, not fixed line numbers |
| `tmp/_tmp_rcinclude_scan.ps1` (re-run 4 Nov) | `returncheck.h` include/use census: 37 includes, 2 TUs with a hit, **35 dead** | T7's number is unchanged by session 33 |
| `tmp/_tmp_s33_closeaudit.ps1` | the three close checks at once: record-file ceilings (>36/>40 KB), every `tmp/_tmp_*s33*` artefact cited in the session's records vs the filesystem, and the presence of the files the pointers name | run it before calling a close-out done - it caught two citations with no matching file (`_tmp_s33_gatecheck.ps1` cited before it existed, `_tmp_win_consumer_s33b.bat` never existed); its one remaining MISSING row is that fabricated name quoted inside `gotchas/sessions-31-33.md` as the negative example |
| `tmp/_tmp_status_index_gen_s33.ps1` | regenerate the register locator with **one bullet per id** (entry head + "(+N per-TU instance bullets later)"), 46 entries | replaces the s19-s22 behaviour of one bullet per register bullet (83 lines, over `status.md`'s ceiling); splice its output with the s33 splice script, then re-run `tmp/_tmp_u15_locator_verify.ps1` |
| `tmp/_tmp_s33_readme_refresh.ps1` | refresh the `Lines` column of `registers/README.md` and `archive/README.md` in place, adding rows for files that have none | the two indexes go stale after any append; run it at close-out (it is idempotent) |


## Added 6 Nov 2026 (T8 step 6a, `pointer.cpp`)

| Instrument | What it prints | Notes |
|---|---|---|
| `tmp/_tmp_u18_coverage4.ps1` | folder coverage: members, export names, covered, **declared-and-unwrapped**, no-prototype, plus the member-names-matching-no-export list | walks the **export** table, so a declared-but-not-exported name never shows as a gap - that is how `EnableMouseInPointerForThread` nearly got wrapped (`registers/u18-coverage.md`) |
| `tmp/_tmp_u18_buckets.ps1` | the unwrapped list split by family, plus A/W pairs both unwrapped and whether the layer declares each stem | the family tranche plan in `u18-coverage.md` comes from this; the raw 83 is not the net backlog (75) |
| `tmp/_tmp_sdk_guard_scan.ps1` `-Names ...` | for each name: SDK line + the **stack of open `#if` guards** at that line | takes `-Names` on the command line; a guard stack with no `WINVER` row means the declaration really is unguarded (the 4 dialog DPI members) |
| `tmp/_tmp_t8_6a_ppclass.ps1` | digest of an `/EP` capture: binding-line count, module literals, distinct name literals, **how many are not byte-equal to an export name**, duplicates | digest only - the capture itself was 453,294 lines and must never be opened (rule 9) |
| `tmp/_tmp_t8_6a_apialldiff.ps1` | pre/post diff of the 107-source all-libraries log: exits, `.cpp` echoes, warning/error line counts, **set-difference of diagnostic lines** | its last row (`pointer_echo_in_post=0`) is a **false zero** - the log echoes the bare file name, not a path, so a `user\pointer\.cpp$` pattern matches nothing. Count with `-SimpleMatch 'pointer.cpp'` instead |
| `tmp/_tmp_t8_6a_gate.bat` / `_tmp_t8_6a_single.bat` | the two gate legs: whole folder in one `cl`, and one TU standalone | recreate them from the `status.md` command line; `/Fo` must stay attached to the switch |
| `tmp/_tmp_t8_6a_dig1.ps1` | one digest file per gate log: exit, echoes, **real** diagnostics, the `loose_diag_lines` that are only file-name echoes, `.obj` vs source freshness | `diag=1` on a clean log was `errorhandling.cpp` matching an `error` pattern - the loose rows are why the two counts differ |


## Added 6 Nov 2026 (T8 step 6b, `dpi.cpp`)

| Instrument | What it prints | Notes |
|---|---|---|
| `tmp/_tmp_t8_6b_pre.ps1` | one block per name of the tranche: bucket re-derivation from the gap list, export-table row, `winuser.h` decl line, full prototype text, the **guard stack open at that line**, macro count, A/W siblings, whether the stem is already declared in `user32.h` | the guard stack is a **forward pass** with an explicit stack - the backward `pop` form is the documented PowerShell trap (`0..-1` is a descending range); its `bucket=` line is the tranche's name list, and it filters `tmp/_tmp_u18_gap_sdk.txt`, so **re-run `_tmp_u18_coverage4.ps1` first** or the previous tranche's names are still in the input |
| `tmp/_tmp_t8_6b_guards.ps1` | per name: SDK line + only the innermost `WINVER` / `NTDDI_VERSION` test | feeds the banner's Guarding paragraph - the group counts there (13 / 3 / 4 / 1) come from this digest, not from reading headers |
| `tmp/_tmp_t8_6b_protos.cpp` + `tmp/_tmp_t8_6b_probe.bat` | compiles 21 function-pointer typedefs, each initialised with `&<SDK name>` | **compile the signatures instead of reading them.** Exit 0 at `/W4` proves two things at once: every declaration is live under the build's defaults (a guarded-out name would be undeclared) and every published signature equals the prototype (a mismatched function-pointer initialiser is `C2440`). Run with **no QOR define and no version macro** so it stands on `sdkddkver.h` defaults; `void*` casts defeat it, keep the typedefs |
| `tmp/_tmp_t8_6b_consumers2.ps1` | one `Select-String` pass over the tree for the tranche's names, each hit classified LAYER / COMMENT / BYPASS-OTHER, plus a loose "family-shaped" call-site pass | the **single pass matters** - the per-name x per-file loop over 3,228 files does not finish. `Select-String` is case-insensitive by default, so the loose pass matched `CallNamedPipe` and `SetBackgroundPixmap` on a `Dpi`-shaped pattern: quote loose counts only with `-CaseSensitive`, and always read the enclosing comment state before calling a hit live (`ui/gl/windowimpl.cpp:550` looked like a bypass and sits inside a `/* */` that closes 751 lines later) |
| `tmp/_tmp_t8_6b_apialldiff3.ps1` | pre/post diff of the 108-source all-libraries log: echoes (bare-name aware), warning/error line counts, **diagnostic set-difference**, and the log lines unique to each side | fixes both 6a caveats: matches `dpi.cpp` as a bare echo, and treats the pre run's `.exit` as untrusted (its 0 is the parse-time artifact) |
| `tmp/_tmp_t8_6b_final.bat`, `_tmp_t8_6b_post4.bat`, `_tmp_t8_6b_single.bat`, `_tmp_t8_6b_pp.bat` | the four legs: standalone TU, whole folder, `/EP` capture, all-libraries with a trustworthy exit | `_tmp_t8_6b_final.bat` writes an exit file with the parse-time artifact, `_tmp_t8_6b_post4.bat` is the version that does not; date the run by `.obj` > source instead |
| `tmp/_tmp_t8_6b_ppclass.ps1` (copy of `_tmp_t8_6a_ppclass.ps1`, takes `-PpFile`) | digest of an `/EP` capture: binding lines, module literals, distinct name literals, not-byte-equal-to-an-export, duplicates | the capture is 453,298 lines and must never be opened (rule 9) |
| `tmp/_tmp_t8_6b_pair2.ps1` | for the new TU: section header line, the declarations between the section header and the **next** `//X functions` line, the TU's `User32::` definitions and bindings, then order-equality of all three, plus folder totals | **use this, not `_tmp_t8_6b_pair.ps1`.** The 6b first version ended the section at its first `//` line, so a section carrying an in-section comment (`user32.h:609-611`, the `NTDDI_VERSION` note) truncated it: it saw 5 of 21 declarations and printed a false `defined-but-not-declared` list of the other 16. Any section with a comment inside it reads wrong. `_tmp_t8_6b_counts.ps1` is the folder-total companion: **483 bindings comment-stripped against 536 raw** over 40 sources, because TU banners spell `qor_pp_useswinapi` out in prose - the same inflation `_tmp_wrong_stem_scan.ps1` has always skipped comments for |
