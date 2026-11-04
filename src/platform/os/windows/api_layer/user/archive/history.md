# consolidation + split history of this record

## Consolidated session notes (sessions 1-16) - the narratives moved out

**Where each kind of fact lives now.** The session-log table at the top of this file is the per-TU verdict-and-action
record. The ground rules (G1-G6), open items (U1-U26), pre-scan items (P1-P5) and deferred tasks (T1-T4) above are the
authority for every cross-cutting finding, count and decision, and they were left intact by this consolidation. The
**full per-TU narrative write-ups of sessions 2-15 are archived verbatim in `session-log-archive.md`** (1,214 lines /
122 KB, moved out of this file on 31 Oct 2026 when it passed 237 KB) together with the `help.cpp` pre-flags of sessions 15
and 16 and the `accelerator.cpp` write-up; nothing was deleted, and every load-bearing fact from those write-ups is also
carried by the registers above. Session 16 (`help.cpp`) has no narrative of its own - its record is the table row, U26, T4
and the section "Session 16 residue" below.


### Session 16 residue (facts not carried by the registers)
- `dumpbin /linkermember:1` on the built `winqor_api_user.lib` proves the help section publishes exactly three members, so
  the `user32.h:642-643` menu pair is absent from the binary as well as from the sources (T4).
- Help coverage: the SDK's help-id neighbourhood is 4 entry points and this header declares all 4 (2 wrapped, 2 not); the
  help command vocabulary (`HELP_CONTEXT` .. `HELP_TCARD_OTHER_CALLER`, `winuser.h:12333-12355`) and
  `HELPWININFO`/`HELPWININFOW` have **no QOR-side spelling anywhere**, which is why the only caller passes a bare
  `unsigned int` for `uCommand`; and the successor API Microsoft points to, `HtmlHelp`, is declared in **no** api_layer
  header at all, even though `comctl32.cpp:324` wraps `MenuHelp` - recorded under U21/U12 placement, not a TU fix.
- U16 measured **0 of 3** for this TU although the version guard is real (`winuser.h:9105` `WINVER >= 0x0400` + Desktop
  for the four help-id prototypes; `:12270` `#ifndef NOHELP` + Desktop for `WinHelpA/W`): the difference from
  `FLASHWINFO` (session 14) and `HDEVNOTIFY` (session 12) is that **no type these wrappers name is version-gated**, so a
  lower `WINVER` leaves the header well-formed. U16 counts *type* dependencies, not macro adjacency.
- Liveness: the three wrappers' only sites are `ui/gui/controllers/windowcontroller.cpp:785`, `:790`, `:795`
  (`SetContextHelpId`, `GetContextHelpId`, `Help` - declared `windowcontroller.h:284-286`), and those three methods have
  no call site in `src`/`examples`/`test`; the gui's own `WM_HELP` path (`ui/gui/view/handlers/interactive/help.cpp:46`)
  reads the id out of the `HELPINFO` payload, so it never goes through the wrapper. No U24 instance here - all three SDK
  returns are genuine success flags or plain values, unlike `FlashWindow`'s previous-state `BOOL`.
- **Two numbers a later session should re-run**: session 16's guard-scan count and its `git diff --stat` line were taken
  while the agent-side shell validator was rejecting most `run_commands` inputs, so treat those two as unconfirmed; every
  other number here was measured with a working shell.
- Bookkeeping at the time of writing: `help.cpp` (session 16) and this file were **uncommitted**; HEAD is `9ecbf581`
  (session 15). Session 12's `devicemanagement.cpp` was likewise uncommitted when its row was written, so a working-tree
  diff of this folder can show more than one file.


### How this file was consolidated (31 Oct 2026, before session 17)
Before: 2,151 lines / 237 KB (session-15 commit) plus session 16's uncommitted additions. After: **1,201 lines / 141 KB**
(`[System.IO.File]::ReadAllLines`, trailing blank lines included; this section is the last thing in the file).
The register sections (convention, pre-scan, session log, deferred tasks, method, ground rules, open items) were kept
verbatim; session 16's additions were folded back in (table row 16, task **T4**, item **U26**, the queue and the
next-session block, all of which a `git checkout` had momentarily reverted); and the per-TU narrative was moved verbatim
to `session-log-archive.md` (1,214 lines) and replaced by the census above.
**Completeness check, run mechanically:** every non-blank line of the session-15 commit (2,049 lines) was tested for
presence in `status.md` + `session-log-archive.md`. First pass: 23 not found - the superseded `help.cpp` pre-flags (with
session 16's confirmation/correction pass over them) and the superseded queue/"not started" bookkeeping rows; all 23 were
then archived verbatim in that file's last sections. Re-run: **0 lines lost**. A fact now appears once: either in a TU's
table row, in the U/G/P/T register entry it feeds, or in this consolidated section.




## How the record was split (1 Nov 2026) - the history, moved out of `status.md` at session-18 closure

`status.md` was 1,222 lines / 145 KB (open items 709 lines = 52%, session-log table 25.6 KB in 92 lines) and every
session had to hold it all, which is what caused the context failures and compaction cycles. Review depth is unchanged:
the record was sliced by section - `tmp/_tmp_status_split.ps1` then `tmp/_tmp_status_index_gen.ps1`, pre-split file kept
at `tmp/_tmp_status_presplit_backup.md`. 1,209 of 1,222 lines moved verbatim (the 13-line gap is this header); all 41
standalone register entries are indexed above; P6 and U27 had no standalone entry (they lived only inside the session-log
row-17 cell), so the split promoted their text verbatim into the "P6 and U27" section of `registers.md`.


## How `method.md` was split (2 Nov 2026) - the procedure stays, tooling and caveats move out

`method.md` held the review method, the growing `tmp/` tooling index and every instrument caveat, and it was the first
record file to pass the ~40 KB ceiling. The split is size hygiene only - the method itself is unchanged.

- **`method.md`** keeps the numbered review steps and a pointer to the other two files.
- **`tooling.md`** (new) is the instrument index: the standing tooling the steps run, the compile-gate history, then the
  instruments as each session added them (13-16, 17-20, the U6 gate rule and its two probes, 21, 22, the U15 closure pass,
  23, 24).
- **`gotchas.md`** (new, referenced from `tooling.md`) carries the caveats, grouped by kind instead of by session:
  instrument blind spots, evidence integrity (gate logs, `/EP` captures, verifier output) and the PowerShell traps hit while
  editing these records.

Mechanics, same as the 1 Nov splits: the pre-split file is kept at `tmp/_tmp_s26_presplit_method.md`, the sections moved by
exact line range under `tmp/_tmp_s26_method_split.ps1`, and `tmp/_tmp_s26_method_verify.ps1` asserts that **every non-blank
line of the pre-split file appears verbatim in the union** of the three files. What it reports as restated is deliberate:
two caveats measured twice (the `-like` backtick trap, the concurrent-batch race) are one entry each now, and exact record
file sizes and line numbers were dropped where they were illustration rather than measurement.

Re-pointed: the `status.md` index table and its routing rule, the U6-instruments pointer in
`archive/session-log.md`, and the two memory-bank pointers. Rows that record a caveat as "written into `method.md`" (the
session-21/22/24/25 close-out notes, `registers/u26.md`) are left as written - that content now lives in `gotchas.md`.
Any close-out audit that enumerates record files for the ceiling check must now include `tooling.md` and `gotchas.md`;
`tmp/_tmp_s25_verify.ps1` still names only `method.md`.


## How `gotchas.md` was split (4 Nov 2026) - the index stays, the caveats move to `gotchas/`

`gotchas.md` had reached 393 lines / 39,677 B - the largest live record file in the folder - and sessions 29-30 had
recorded slicing it as the next size-hygiene step (`tooling.md`, session-30 note). It is now an **index** in the shape of
`memory-bank/gotchas.md`: YAML frontmatter (`Type: Index`) carrying `Trigger:` / `File:` pairs, then the routing notes.
The caveats live in `gotchas/`, one file per kind:

- `gotchas/instrument-flags.md` - the standing instrument blind spots (was the "Instrument caveats" section).
- `gotchas/evidence-integrity.md` - gate logs, `/EP` captures, verifier output (was "Evidence integrity").
- `gotchas/record-editing.md` - the traps in editing these records from PowerShell (was "Editing these records (and any
  long text file) from PowerShell": sessions 21-27).
- `gotchas/sessions-28-30.md` and `gotchas/sessions-31-32.md` - the session blocks of sessions 28-32, **kept as they were
  measured** under their own headings rather than re-sorted by kind, so every entry keeps the pass that measured it.

Mechanics, same as the splits above: the pre-split file is kept at `tmp/_tmp_gotchas_presplit.md`, the sections moved by
exact line range (`12-31`, `33-67`, `70-188`, `190-321`, `322-393`) under `tmp/_tmp_gotchas_split.ps1`, and
`tmp/_tmp_gotchas_verify.ps1` asserts that **every non-blank line of the pre-split file appears verbatim in the union** of
the index and the five files - 368 of 368, restated/dropped 0, snapshot SHA256
`73BDB1E78D3C2B5E45BA2159964C12FEB3B37BB1370A110145C4CDC9ADF1D739`. Nothing was merged, reworded or dropped: the only new
text is the index's trigger block and file table, and one provenance line at the head of each moved section. Each new file
was re-checked for the folder format (ASCII, CRLF, final newline, no trailing whitespace, no tabs).

Re-pointed: the `status.md` index row and its routing rule, the `method.md` folder list, the `tooling.md` lead-in and the
session-30 "should slice `gotchas.md`" note. Pointers that say "see `gotchas.md`" - `tooling.md`, the registers, the TU
narratives, the session-log slices - are left as written: that path is now the index, and the trigger block inside it
names the file. Any close-out audit that enumerates record files for the ceiling check must now include `gotchas/*.md`;
`tmp/_tmp_s29_sizes.ps1` and `tmp/_tmp_s25_verify.ps1` still name only `gotchas.md`.

