---
Type: Context
Description: "Caveats measured in sessions 28-30 (rectangle, resource, scrollbar)"
---

Moved verbatim out of `gotchas.md` on 4 Nov 2026 by `tmp/_tmp_gotchas_split.ps1` - snapshot `tmp/_tmp_gotchas_presplit.md`, verbatim rejoin proven by `tmp/_tmp_gotchas_verify.ps1`.

## Session 28 (`rectangle.cpp`, 3 Nov 2026)

- **A normalisation pass that names its target lines is unsafe in a file that also gets a contract block** (session 28,
  caught before it wrote). `_tmp_rect_edit_s28.ps1` started as `$targets = @(20, 27, 34, ...)`; the block inserted above
  the definitions moved those 11 call sites to `:101`-`:170`, so the script would have rewritten 11 lines of **prose** and
  left the real sites untouched - and its own guard (`if($old -notmatch 'Library::Call') { throw }`) only fires if the
  prose line happens not to mention `Library::Call`, which a contract block paragraph does. Fix: **self-locate** (start at
  the first `^\s*BOOL User32::` line, take every `Library::Call` line after it) and assert the expected count of changed
  lines. The related trap: two definitions with **identical** call lines (`InflateRect` and `OffsetRect` both
  `Call< BOOL, ::LPRECT, int, int >(pFunc, lprc, dx, dy);`) defeat the `_tmp_pt_edit_s27.ps1` "substring must match
  exactly once" guard, so a pass over such a TU must be line-scoped, not substring-scoped.
- **Order-dependent steps must never go in one parallel tool batch** (session 28, hit twice). A batch holding
  `git checkout -- rectangle.cpp`, the edit script and the block splice ran them concurrently, so the splice read the file
  the checkout had just restored; a batch holding the register index generator, the `status.md` locator splice and the
  locator verifier made the splice read the **previous** session's `_tmp_status_index_s22.md` (59 lines instead of 63).
  Both were silent - the wrong input simply produced plausible output. Sequence any chain where step B consumes step A's
  output, and re-run the consumer when in doubt (`_tmp_u15_locator_verify.ps1` after every `status.md` locator change is
  what caught the second one).
- **A coverage scan that matches the export name verbatim reports `*A`/`*W` SDK names as unwrapped** (session 28).
  `_tmp_rect_region_s28.ps1` flagged `GetWindowLongA`/`GetWindowLongW` as having no `user32.h` member; the layer wraps
  the family as `GetWindowLongT` (`user32.h:625`), so the rows are artifacts. Resolve `stem -> stemT` (and
  `stemA/W -> stem -> stemT`) before claiming a gap - `_tmp_rect_family_s28.ps1` does. Cross-check any "no member" claim
  with a plain `Select-String user32.h -Pattern <Stem>` before it enters a register.
- **A same-named member *definition* reads as an indirect call in the U19 scan** (session 28). The scan reported 2
  `INDIRECT SetRect via EditController / via Region` rows; both are definitions of methods that merely *share* the SDK
  name - `EditController::SetRect` (`ui/gui/controllers/a_h/editcontroller.cpp:220`) sends `EM_SETRECT` through
  `SendMessageT`, `Region::SetRect` (`ui/gui/gdiobjects/region.cpp:48`) calls `GDI32::SetRectRgn`. Neither touches
  `SetRect` in `user32.dll`. An INDIRECT row is only a reachability fact if the line is a **call** into the layer or the
  SDK, so always read the row's source line before counting it.
- **In a file that is 57% comment, every grep hit needs a comment-state check before it is recorded as a call site**
  (session 28, this time the filter worked). `ui/gl/windowimpl.cpp:1470` has a live-looking
  `OffsetRect(&rect, ...);` that sits inside the `/* */` block opening at `:1303` and closing at `:1507`.
  `tmp/_tmp_comment_state.ps1` (character-level, handles `/*` inside string literals) is the instrument; the 3 real
  BYPASS sites in that file are `:516`, `:521` and `:1756`.
- **A TU line scanner must count constructs over the code region, not the whole file, once the TU has a contract
  block** (session 28, caught on the final capture). `_tmp_rect_lines_s28.ps1` reported `qor_pp_fcontext sites: 12`
  and 10/10/3 `CONST` lines because the block quotes all three spellings and the macro in prose; the narrative said
  11 and 7/7/2 and was right. Fix: hygiene counts (TAB, trailing space, non-ASCII, longest line) stay whole-file - a
  trailing space in a comment is still a defect - but definition, binding, `Call<>` spacing and type-spelling counts
  start at the first `^\s*BOOL User32::` line. `tmp/_tmp_rect_lines_final2_s28.txt` is the trustworthy capture;
  `_tmp_rect_lines_final_s28.txt` (11:27) is the polluted one and must not be cited. Same class as the session-27 note
  above: **when a scanner total disagrees with the record, suspect the scanner's scope before the reading.**

## Session 29 (`resource.cpp`, 3 Nov 2026)

- **A link log quotes the tested symbol twice, so a plain grep of it over-counts.** `_tmp_res_ref_s29.cpp`'s link emitted
  11 `LNK2019` rows in which `?CopyImage@User32@api@win@qor@@SA...` appears **as the *referencing* function** of the
  cross-module noise; a grep for the member name therefore claimed 10 "unresolved members" when the truth is **0**. The
  correct predicate matches the name inside the quoted unresolved part only:
  `unresolved external symbol "..." \((\?)(CopyImage|LoadImageT)@User32@` (`tmp/_tmp_res_link_final_s29.txt`, 0 rows).
- **A clean 38-TU gate log is byte-identical across runs, so for a clean run the "hash must differ" dating test is void.**
  `_tmp_cl_all_s28b.log` (3 Nov 11:28) and `_tmp_cl_all_s29b.log` (3 Nov 15:00) have the same SHA256
  (`7EB28546A9062CBCC...`) because a passing log contains nothing but the 38 TU-name echoes. Date such a run by the
  objects instead: `tmp/obj/resource.obj` 15:00:28 vs `resource.cpp` 14:58:40, and all 38 TU objects newer than the
  source (`tmp/_tmp_res_finalchk_s29.txt`). The session-23 rule still holds against *copies* - it just cannot prove a
  fresh clean run on its own.
- **Any multiset diff over a `/EP` capture must be `Group-Object`/hashtable based.** Two instances in one session: the
  first `_tmp_res_neutral_s29.ps1` (`Where-Object { $arr -notcontains $_ }` over 154,284 non-blank lines) did not finish
  inside the 300 s tool timeout - it completed **about 2 h later**, and its output agrees with the rewrite line for line,
  so the reading is corroborated but the instrument must not be waited on. The first
  `_tmp_res_neutral_final_s29.ps1` reintroduced the same shape and had to be rewritten. At 453k lines also use
  `[IO.File]::ReadAllLines` plus index loops rather than `Get-Content` pipelines. The fixed `_tmp_res_neutral_s29b.ps1`
  ran in about a minute and located all 4 differing lines by position.
- **`dumpbin | findstr` is the wrong tool for a symbol question.** The member-vs-free distinction lives in the mangled
  middle letters (`SA` static member vs `YA` free) and in the demangled text, and `findstr`'s pattern list splits on
  spaces - `_tmp_res_syms_s29.bat`'s `"| ?CopyImage ?LoadImageT"` filter matched noise. Dump to a file, then match in
  PowerShell (`-match '\?CopyImage@User32@'`, filter on the `External` column): that is how `_tmp_res_evid_s29.ps1`
  reads both the `/symbols` and the link logs.
- **"What the library publishes today" needs the build artefacts dated first.** `cmakebuild`'s Debug DLL is 29 Oct and
  `cmakebuild_rel` is 22 Oct, both older than session 29; the export/import-library counts (439 names, 854 member
  symbols) are only usable as the U11 "before" evidence because `resource.cpp` was untouched between 29 Oct and this
  session. Say that in the record - `registers/u11.md` now does.
- **Do not verify a generated index in the same parallel batch that regenerates it.** A re-run of
  `_tmp_status_index_gen_s22.ps1` and the `Select-String` that checked `archive/README.md` were issued in one batch, so the
  grep read the pre-write file and reported `session-log.md` at 305 lines when the regenerated row says **307**. The
  numbers were never wrong - the check was. Re-read generated files in a later call.
- **The installed SDK has no `// Shell Support Functions` label.** `help.cpp:27`-`:32` groups `CopyImage`/`LoadImage`
  under the *classic* SDK's "Shell Support Functions"; measured, that string occurs **0** times in any `um` header of
  10.0.26100.0, where grouping is by `#pragma region` (`Desktop or Games Family` `winuser.h:10865`, `Desktop Family`
  `:10900` - the latter contains `LoadImageA` `:10906`, `LoadImageW` `:10916`, `CopyImage` `:10932`, all under
  `#if(WINVER >= 0x0400)` at `:10883`). Cite the pragma region + guard, not the label.
- **A TU contract block must not cite its own file's line numbers** (the session-22 rule, tripped again immediately):
  the first draft of `resource.cpp`'s block said the wrong-stem scan "used to flag `:26` and `:33`", which the block
  itself shifts. It now says "both of its bindings"; the numbers live in the records, which are written after the final
  line layout is fixed.
- **A record file must not state its own size or line count** - the sentence that states it changes it. `tooling.md` and
  the session-29 close-out note both said "the file is now 355 lines / 37,053 B", which was stale the moment they were
  written. They now point at `tmp/_tmp_s29_sizes.txt` (from `tmp/_tmp_s29_sizes.ps1`, a snapshot of every live record file
  taken once at close). Same shape as the session-22 "no line numbers inside a TU contract block" rule, one level up.
- **The citation audit is self-scanning.** `_tmp_s29_cite_audit.ps1` checks `tooling.md` too, so naming a missing artefact
  inside the fix's own description makes the next run report it again. Name the bad path without its `tmp/` prefix (the
  regex needs the prefix). Run 1: 194 cited / 1 missing (a real five-session-old typo in `registers/census.md:132`);
  run 2: 195 / 1 (the fix's own text); after the wording fix: **194 / 0**.
- **This session's shared terminal is not a reliable evidence channel.** Three failures in one stretch: a
  `pwsh -NoProfile -Command "[string[]]@('a','b')"` argument arrived at the shell as a literal string (printed in quotes,
  not executed), a `cmd /c '"<path with spaces>" ... '` invocation collapsed its quoting, and after each failure the
  terminal replayed **stale scrollback** that looked like the new command's output - including a 40 k-character
  `dumpbin /symbols` dump that had nothing to do with the query asked. Rule: for anything that matters, write the
  instrument's output to a `tmp/` file and read that file (`read_files`), never trust the echoed terminal; keep
  multi-step work in a `.bat`/`.ps1` **file** rather than in a `-Command` string, and re-run rather than re-read.

## Session 30 (`scrollbar.cpp`, 3 Nov 2026)

- **`_tmp_sigcmp.ps1`'s `CALL TYPE MISMATCH` has a second false-positive source: `*` adjacency, not just `CONST::`.** For
  `ScrollDC` it printed `sdk 'const ::RECT *lprcScroll' vs call 'const ::RECT*'` twice (args 3 and 4) - the declaration
  puts a space before `*`, the `Library::Call<>` list does not. Same type, same generated symbols, and this TU's **only**
  two flags were these. Rule: read every `CALL TYPE MISMATCH` against the source lines before acting (the session-23
  rule), and normalize whitespace/`*`-adjacency **in the instrument**, never in the source. U25's normalization list now
  carries five shapes for the same type.
- **The three-way name-compare family compared names case-INsensibly until this session.** `_tmp_paint_pre_s26.ps1` and
  `_tmp_pt_pre_s27.ps1` use PowerShell `-ne`, so a case-only divergence (`wSBFlags` vs `wSBflags`) compares equal and the
  row prints `def-vs-sdk[]` - the same defect the session-12 instrument had and fixed. `_tmp_scroll_names_s30.ps1` uses
  `-cne`; on this TU it found 4 divergences the older family's shape would have printed as 0. The folder-wide
  `_tmp_name_divergence_scan.ps1` is separately case-sensitive by construction (it reported 0 for this TU - it only
  compares header-vs-definition, and those genuinely agree). Any "0 name divergences" from a `_pre_s2x.ps1` clone means
  "0 case-insensitive ones".
- **A citation-range check flags legitimate external citations.** The block-citation scan reported 6 "out-of-range" cites
  in `scrollbar.cpp`'s contract block; all 6 are real and correct - `user32.h:573`-`:585`, and
  `tmp/_tmp_scroll_pre_s30.txt:167-:185` - they are simply not in the TU's 156-line space. Use the session-26 approach
  (`_tmp_s26_closecheck.ps1`'s explicit `-ExternalZones` line ranges) or the run will end at 6 flags instead of 0.
- **A U19 "INDIRECT" row is usually a *definition*, and here all 8 were.** `bool ScrollBarController::EnableScrollBar(...)`
  and its 7 siblings matched the stem-and-paren pattern. Confirmed a second time (session 28 found 2 of 2 in
  `rectangle.cpp`); the shared `tmp/_tmp_bypass_scan.ps1` still counts them, so INDIRECT stays an upper bound until
  opened by hand.
- **`wprintf(L"%s", char*)` is a `/W4` diagnostic waiting to happen.** In the G4 probe, printing an ASCII name table
  through `wprintf` is a vararg type mismatch; and `%p` on a wide string prints garbage. Keep parallel `const char*`
  tables for the names and print with `printf`, using `wprintf` only for the `wchar_t` literals - the mirror probe
  (`_tmp_scroll_mirror_s30.cpp`) builds clean at `/W4` with that split.
- **A digest that indexes `tmp/` indexes itself mid-write.** `_tmp_s30_digest.ps1` lists every `_tmp*s30*` file, so its
  first entry was `_tmp_s30_digest.txt lines=2 bytes=0` - the half-written file being written into while it is being
  read. Same family as "a record file must not state its own size": exclude the output file from any scan that lists its
  own directory.
