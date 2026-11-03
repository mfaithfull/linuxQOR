# winqor_api_user - instrument and record-editing caveats

Referenced from `tooling.md`: every instrument in that index has a measured blind spot, and editing these records from
PowerShell has its own set. **Read the entry that qualifies a flag before acting on the flag.** Procedure: `method.md`;
instrument index: `tooling.md`.

Entries are grouped by kind, not by session; each names the session or pass that measured it, and the instrument slug is
the handle to search for in `tooling.md`. Text moved here verbatim from `method.md` on 2 Nov 2026, with two exceptions:
caveats measured twice are merged into one entry, and exact record file sizes and line numbers were dropped where they
were illustration rather than measurement - those citations go stale faster than they inform.

## Instrument caveats - read a flag before acting on it
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
- **Export-table parsing caveat:** `tmp/_tmp_user32_exports.txt` lists 1,018 names but an ordinal+RVA regex parses
  **1,014**; the four misses are the `(forwarded to NTDLL...)` rows (P4), which carry no RVA. Quote 1,014 as the parsed
  count and do not read the gap as missing exports.
- **Bypass classification false positives are now known, not guessed.** The U19 classifier reads an unqualified member
  **declaration** in another library (`ui/gui/controllers/.../menu.h:95 Menu* GetSubMenu(int)`) as a BYPASS call site:
  3 of the 18 menu rows are that. Quote row counts and real-call counts separately, and check the file/line kind before
  calling a site a bypass.

## Evidence integrity - gate logs, `/EP` captures, verifier output
**Compile-log gotcha (measured 30 Oct 2026):** `_tmp_compile_all.ps1` writes the `cl` output to the fixed path
`tmp/_tmp_cl_all.log` regardless of the caller, so redirecting the script's own stdout to a per-session file captures only
its summary line (`exit=N loglines=M`) plus its internal diagnostic grep - it is not the `cl` log. The summary's `exit=0` is
already sufficient for the 38/38 claim (one `cl` invocation over all 38 sources fails as a whole if any TU fails), and the
belt-and-braces check is in `tmp/_tmp_cl_all.log`: 38 `\.cpp$` echo lines and 38 `.obj` files in `tmp/obj` newer than the
run - both confirmed for the session-11 run.
- `_tmp_compile_all.ps1`'s `pwsh -File` wrapper returned exit 1 in session 15 while the `.bat` it writes reported
  `cl_exit=0` with a clean log - run `cmd /c tmp/_tmp_cl_all.bat` and read the log, don't trust that wrapper's exit code.
- `cl /EP` output must go to a **per-TU file**: the shared `_tmp_pp_one.bat` writes `_tmp_pp_out.txt`, and reading it
  before `cl` finished once produced a whole session's worth of another TU's text.
- **A copied gate log cannot date its own run** (session 23's interrupted close-out pass). `Copy-Item` preserves the
  source's `LastWriteTime`, and every passing gate run writes output of the same shape, so a copy of an older log is
  indistinguishable from a new run by timestamp alone. That pass produced `tmp/_tmp_cl_all_s23final.log` and `_s23pre.log`
  that are **byte-identical to `_tmp_cl_all_s22final.log`** (SHA256 `7EB28546A9062CBCC...`) and therefore predate both
  session-23 source edits. Either run `tmp/_tmp_cl_all.bat` with its redirect **outside** the batch and snapshot the file
  afterwards, or verify: `tmp/obj/<tu>.obj` `LastWriteTime` must be later than the edited source's, and the new log's hash
  must differ from the previous session's.
- **Re-run `/EP` after the last edit.** `tmp/_tmp_msg_pp_post_s23.txt` (11:17) predates the `user32.h` `(void)` edit (11:36),
  so the neutral-conversion proof had to be redone as `_tmp_msg_pp_final_s23.txt`; diff the captures whose timestamps bracket
  the final edit, never the first "post" file you find.
- **Locator pointer verification:** use `tmp/_tmp_u15_locator_verify.ps1`, not an ad-hoc matcher - it knows the one
  deliberate exception (U27's pointer targets the shared `p6-u27.md` extract bullet, which does not start with
  `- **U27**`). Re-read the totals it prints after every register edit; the exception count is the only number there that
  may not move.
- **Comment-only edits have a free neutrality proof - use it instead of arguing** (session 24). `cl /EP` before and after
  a large comment block differ by exactly the block's own line count, **all of them blank, 0 removed, 0 non-blank added**
  (`/EP` drops the comment text but keeps one output line per comment line), i.e. the emitted code is identical. Three
  counts from `Compare-Object` prove it: total, `<=`-side count, non-blank `=>` count.
- **`GetModuleHandleW(L"user32")` fails with `gle=126` in a bare console process**, so every live name probe must
  `LoadLibraryW` first and print that it did (G5); `monitor`, `mdi` and `keyboard` probes all take that path.
- **Verify a cited artefact before you cite it.** Session 25's records cited a verifier report while its script was still
  being edited: the file was a 22:22 mid-session snapshot (`archive/tu/mouse.md: ABSENT`, 0 citations counted), and
  re-running `tmp/_tmp_s25_verify.ps1` - the point of that script - changed several of its lines. Close-out verifiers run
  last, after every record edit, and their output is quoted only from that final run.


## Editing these records (and any long text file) from PowerShell
- Counting en/em dashes by piping `git show HEAD:<path>` through PowerShell reports **0** (cp1252 re-decode) - measure with
  `[System.IO.File]::ReadAllText` on the working file and compare code-point groups instead; do not "normalize" encoding.
  The original note with the memory-bank dash counts is in the archive's `filledshapes.cpp` section. Measured 31 Oct: `method.md` had
  **3** non-ASCII characters (all U+2026), the archive **4**; `.cpp` sources in this folder stay pure ASCII.
- **PowerShell `-like` treats the backtick as its escape character**, so a pattern containing a markdown code span never
  matches. Measured twice: in session 21 an anchor line of the session brief returned False under `-like` while the same
  text matched under `.Contains`; in session 23 a locator splice reported `start=-1 end=-1` for a pattern of the form
  `'- **P1** `registers/*'`. For any anchor over these records use `.Contains(...)` or `-match` with an escaped regex
  (`tmp/_tmp_s23_locsplice.ps1` now uses `-match '^- \*\*P1\*\* .registers/'`), and verify the anchor before splicing - a
  silent False there is how a slice loses lines.
- **The `editor` tool matches whole lines**, so on a markdown table whose rows are one very long line it either reports
  "text not found" or refuses an over-large payload. For those rows use a PowerShell script that replaces exact substrings
  with [IO.File]::WriteAllText and re-measures afterwards (tmp/_tmp_slog_fix_s21.ps1, tmp/_tmp_progress_fix_s21.ps1,
  tmp/_tmp_s21_close_a.ps1, _tmp_s21_close_b.ps1, _tmp_s21_close_c.ps1).
- **Splices must run bottom-up** (highest line range first) and print the before/after line and CRLF counts -
  `tmp/_tmp_s21_close_b.ps1` does exactly that, and its anchors are what caught the `-like` trap above.
- **Inserting a TU comment block invalidates the block's own line references** (session 22, `menu.cpp`). Every `:NNN` the
  new block cites for a definition below it in the same file shifts by the block's own length. Write those references from
  the **post-insert** numbering, or re-point them immediately with an exact-anchor script
  (`tmp/_tmp_s22_lineref_fix3.ps1`), then grep for the pre-insert numbers to find the leftovers. The register rows written
  before the fix carry the same risk.
- **PowerShell flattens nested arrays, which turns a pair table into a character index.** `@( @('a','b') )` is **two
  strings**, so `foreach($p in $pairs){ $p[0] }` yields the character `a` - the session-22 line-reference script reported
  "MISSING/AMBIG x120" for a one-pair file while multi-pair files in the same script worked, because only the single-pair
  case flattened. Use a flat alternating array stepped by 2, or hashtables, and always print the match count per anchor.
- **`[IO.File]::*Text` resolves relative paths against the .NET current directory, not PowerShell's.** In the same
  command, `cd <dir>` then `[IO.File]::ReadAllText('session-log.md')` threw "Could not find file
  `C:\RAID0\Develop\linuxQOR\session-log.md`" while `Get-Item 'session-log.md')` in the **same** statement list resolved
  correctly. Every record-editing script must use absolute paths.
- **Record hygiene arithmetic:** the ceiling must be measured **after** the row you are about to add. In session 22 the
  first slice of the oldest `session-log.md` rows left that file over its ceiling, so the next block of rows had to go too
  (`tmp/_tmp_s22_logsplit.ps1`, `_tmp_s22_logsplit2.ps1`, both rejoin-and-hash first). Keep the slice files verbatim,
  repeat the table header in each, and re-point `archive/README.md`.
- **Regenerate the locator, then splice it, in two separate commands** (the U15 closure pass). One batch ran the generator
  and `tmp/_tmp_s22_locsplice.ps1` concurrently, so the splice compared against the previous generated index, printed
  "differing entries: 0" and wrote stale `tasks.md` pointers - an earlier edit had already shifted the entries that index
  named. Re-running the splice on its own fixed it; the batch-race entry below is the mechanism.
- **`run_commands` runs its command array concurrently, so never chain a producer and its consumer in one call.** Measured
  three times: in session 21 the reader ran first and reported a just-written compile log "does not exist" although the
  compile had exit 0 (twice); in the U15 closure pass a locator generator and the splice that reads its output raced, and
  the splice wrote stale pointers (see the regenerate-then-splice entry); and in session 24 the `/EP` batch and
  `_tmp_mon_class_s24.ps1 -PpFile <that output>` were issued together and the classifier died with
  `Could not find file ..._pp_post_s24.txt` although the batch echoed `pp_exit=0` - the file existed a second later. Any
  step that reads another step's artefact must go in a **later** call, or both steps must live inside one shell command.
- **A TU comment block must be inserted *inside* the namespace, never by re-emitting the namespace line.** The first
  insert reproduced `namespace qor { namespace win { namespace api {` as the first line of the block, which left the
  whole file inside a second `api` namespace and produced 11 errors - `C2888 '...' symbol cannot be defined within
  namespace 'api'` on every wrapper plus `C1075` on the closing `}}}`. Cheap to avoid: anchor the insert on existing
  text, and compile the single TU (`tmp/_tmp_compile_one.ps1 monitor.cpp`) immediately after the insert, before writing
  any record.
- **Insert-at-line-number is positional, not semantic.** Three chunks inserted at successive line numbers landed one
  *inside* another paragraph (the U26 block split the U21 paragraph mid-sentence and left two orphan lines). For
  mid-file record text, use the `old_text` replacement form with a unique anchor; use `insert_line` only at EOF or into
  a known-empty gap.
- **Any insert that adds lines invalidates the TU's own line citations** (session 24, `monitor.cpp`). A block inserted
  above the definitions pushes every definition below it down, so the block's self-citations - its definition range, the
  `AW` binding line, the by-value `POINT` in the `Call<>` list - have to be re-pointed after the insert: re-measure with
  `Select-String -Pattern '^    (BOOL|HMONITOR) User32::','qor_pp_useswinapi','Library::Call'` and fix before the gate.
- **An empty `Compare-Object` result assigns `$null`, not an empty array**, so a check written as
  `if ($null -eq $dd) { 'BLOCK NOT FOUND' } elseif (@($dd).Count -eq 0) { 'IDENTICAL' }` reports the **identical** case as
  a missing block. `_tmp_s24_closeout.ps1` printed exactly that false negative until the "block found" condition was
  tracked in its own variable and the diff wrapped in `@()`. Rule: decide "not found" from your own sentinel, never from
  the diff being empty - the same trap makes `@($x).Count -eq 0` ambiguous in the other direction.
- **A one-letter PowerShell function name is a trap: `H` is the built-in alias for `Get-History`** (session 25). The log
  slice defined `function H([string]$s)`; the call `H($old[$i])` bound the row text to `Get-History -Id` and died with
  `Cannot convert value "| 20 | ... "` - a whole markdown row reported inside a type error. It aborted *before* rewriting
  the source, which is the only reason the slice was safe: verify, then rewrite. Use verb-form names (`Get-ShaHex`).
- **Hand-built name matrices lie; grep the artefact.** `_tmp_paint_prep_s26.ps1`'s bare/A/W matrix for the `painting`
  stems emitted spurious bare `A`/`W` rows and dropped `DrawStateA`/`DrawStateW` (which the export table has, `D9`/`DA`);
  `Select-String` over `tmp/_tmp_user32_exports.txt` gave the truth. Cross-check any generated name list against a direct
  grep before it feeds a conclusion.
- **PowerShell `-replace` is case-INsensitive: `-replace 'T$'` eats a trailing `t`, not only the folder's `T` suffix**
  (session 26). `_tmp_paint_pre_s26.ps1` stripped the "SDK stem" with `$name -replace 'T$',''`, which silently produced
  `BeginPain`, `EndPain`, `GetUpdateRec`, `DrawFocusRec`, `InvalidateRec`, `ValidateRec` - six prototypes that exist
  nowhere - and the TU looked like it had 6 SDK names unmatched (`sdk=0` rows) plus 6 phantom parameter-name
  divergences. Use `-creplace 'T$',''` (or an explicit `-cmatch 'T$'` guard) whenever a suffix strip is case-significant,
  and treat an unexpected `0`/`not found` count as a bug in the instrument until a direct grep proves otherwise. The same
  pattern in `_tmp_sdk_param_name_scan.ps1:242` is harmless only because that script tries the unstripped stem first and
  keeps the stripped arm last (`:238`-`:241`), so a false strip just adds candidates.

- **A coverage superlative must be measured before it is written down** (session 27). The `processthread.cpp` contract
  block originally asserted "the only two `user32` exports with no wrapper anywhere", invented from two remembered
  ordinals. `_tmp_pt_coverage_s27.ps1` then measured the real figure: of 1,018 export names, **370** have no member
  declaration in `user32.h` at all (114 of them declared in the installed `winuser.h`, 256 not declared there), and one of
  the two names the sentence cited - `DdeSetUserHandle` - is in fact declared, at `user32.h:528`, inside U4's 34-member
  DDE block. Rule: "the only", "none left", "all wrapped" and every similar phrase needs an instrument behind it **before**
  it enters the block, and if the measurement disagrees, rewrite the paragraph before the gate rather than after.
- **Bare `:NNN` citations inherit the last file named in the paragraph, and they drift when the block grows** (session 27).
  A contract block cites its own lines and other files' lines in the same breath, so `:12417` means `winuser.h` only
  because that file was named earlier in the paragraph - a verifier has to carry that marker to resolve it, and any edit
  that changes the block's line count invalidates the block's **own** self-citations. `_tmp_pt_cite_s27.ps1` does the
  resolution mechanically (28 citations here, plus an assertion that each def/binding self-citation lands on the right
  construct) and found both a wrong path (`gui/CMakeLists.txt` vs `ui/gui/CMakeLists.txt`) and three self-citations that
  had drifted 6 lines when a paragraph grew. Run it **after** the block settles, never mid-edit.
- **`_tmp_sigcmp.ps1`'s call-site check reads one source line, so a wrapped call is a silent no-op** (session 27, fixed).
  Its pattern needed `Library::Call<...>(pFunc, ...);` complete on a single line; every multi-line call printed
  `CALL NOT PARSED` and the arity/type comparison never ran. `CALL NOT PARSED` therefore means *"the instrument could not
  read this site"*, not *"this site is wrong"* - read it as unverified, and check the template list against the
  definition by hand or fix the scanner (it now joins lines to the terminating `;`). Records from sessions 12-26 that
  quote `CALL NOT PARSED` never had that half of the check.
- **Empty preprocessor guard pairs fool a backward guard scan** (session 27). `winuser.h:4121`-`:4122` is
  `#if (_WIN32_WINNT >= 0x602)` immediately followed by its `#endif` - it guards nothing - and it sits between
  `AttachThreadInput` and `WaitForInputIdle`, so a scan that walks backwards to the first `#if` reports **0x0602** as
  `WaitForInputIdle`'s version gate. A U16 guard answer is only trustworthy if the scanner keeps an `#if`/`#endif` depth
  counter (`_tmp_pt_guards_s27.ps1`, `_tmp_sdk_guard_scan.ps1`, `_tmp_mon_region_s24.ps1` all do).
- **`[string]::Replace` overload resolution can hand `WriteAllText` a `$null` and truncate a record to 0 bytes**
  (session 27, measured). `($t -replace ...)` style tidying was written as `$t.Replace($a, $b)` where one argument was a
  **one-character** string, so PowerShell bound the `Replace(char, char)` overload, threw, and left `$t` `$null`; the very
  next statement was `[IO.File]::WriteAllText($path, $t)`, which happily wrote an empty file. `archive/session-log.md` was
  rebuilt the same session from the pre-slice snapshot by replaying the slice transformation -
  `tmp/_tmp_s27_logrestore.ps1` prints the proof (both sliced rows sha256-matched, every other snapshot line reproduced
  byte-identically). Rules: never write a record file from a variable produced by a statement that can throw - assert
  `if($null -eq $t) { throw }` and `if($out.Length -lt 1) { throw }` immediately before the write; snapshot the file first;
  and prefer the `editor` tool for record prose, since it cannot truncate.
- **Parse instrument tables, do not eyeball their columns.** Summing the per-file `Spaced` column of
  `tmp/_tmp_u14_post_s27.txt` by hand gave 186 against the script's `TOTAL spaced=174`; parsing the same rows with a regex
  reproduces the TOTAL exactly (425 / 3 / 174 / 254). The script strips comments before counting, so a quoted `Call<`
  inside a contract block is not a site - when a total disagrees with a table, suspect the reading before the scanner.

