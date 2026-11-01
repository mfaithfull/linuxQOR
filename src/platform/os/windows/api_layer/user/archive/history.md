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
