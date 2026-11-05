# Active Context

Last updated: 5 Nov 2026 - **session 35 (`windowproperties.cpp`) CLOSED: 36 of 38 TUs reviewed, 2 left**
(`windowprocedure.cpp` 30 lines, `winqor_api_user.cpp` 14 - 44 source lines). Session 35 fixed nothing: the TU measured
clean (5/5 declaration-definition pairs in header order, 0 return-type divergence, 5/5 bindings resolving to `<stem>W` on
both arches, 0 U4/U11/U13/U17), its one probe settled that `PROPENUMPROC`/`PROPENUMPROCEX` are the **W** callback shapes
under `STRICT` (so the callback parameters are type-checked, not `FARPROC`), and its two blemishes (the U14 spacing nit,
the dead `returncheck.h` include) were left as-is by user decision. It also corrected a stale hand-off:
`SetLayeredWindowAttributes` is not declared-without-definition - it is defined at `window.cpp:565`.

**The record protocol changed at this close - protocol v2, defined in `status.md`.** The record had reached 88 files
/ 762 KB and the read-set a session was told to open had reached 269 KB, which forced compaction and then re-checking.
Under v2: the review banner written into each reviewed `.cpp` is the session narrative; record files are write-once
and are never edited afterwards; the generated locator, the README line-count columns, the queue audit and the `tmp/`
cite audit are retired; and `status.md` + `method.md` + the current TU (~30 KB) are the whole boot read-set.
Per-session narrative does not belong in this file - it is in `status.md`, the session log and the source banners.

## Current focus: winqor_api_user source review (one TU per session)

- Reviewing `src/platform/os/windows/api_layer/user` (class `qor::win::api::User32` over `user32.dll`) for
  correctness, consistency, safety, UB and convention conformance. **Protocol: exactly one translation unit
  per session**; small unambiguous defects fixed in place, everything behavioural recorded as an open item.
- **Session 35 (`windowproperties.cpp`) CLOSED 5 Nov 2026** - 5 wrappers, 0 binding / signature / convention defects
  requiring a change. Narrative: the review banner at `windowproperties.cpp:16-77`.
- **Next: session 36, `windowprocedure.cpp`** (30 lines, 2 wrappers - `CallWindowProcT`, `DefWindowProcT`), then the glue
  `winqor_api_user.cpp` closes the library. Fresh pre-flags for it are in `status.md`; instruments already prepared:
  `tmp/_tmp_s36_pre.ps1`, `tmp/_tmp_s36_ppclass.ps1`, `tmp/_tmp_win_pp_s36.bat` (export table + `/EP` capture already run).

## Decisions in force (pointer only - not restated here)
 - Project wide decisions are recorded in `docs/agent/DECISIONS.md`. The memory bank deliberately does not duplicate their text.
- **Rule adopted 2 Nov 2026 (user):** this file carries the current focus, and pointers **never a per-session narrative**. At each session close, the outgoing "recent changes" entry is either reduced to the two or three
lines a new session needs to start, or moved to `memory-bank/archive/`. Session detail belongs in the project record tree
(item 3), not here.

## Where the record lives (boot order)

1. `src/platform/os/windows/api_layer/user/status.md` - the review boot file: live state, TU queue, the id-to-file map,
   the verification gate and the current TU's pre-flags. With `method.md` and the TU itself this is the **whole** boot
   read-set (protocol v2): about 30 KB.
2. `src/platform/os/windows/api_layer/user/registers/` - one file per open item; open only the file for an id you are
   acting on, and read its entry head. Never the whole tree. `../method.md` holds the procedure, `../tooling.md` the
   instrument index and `../gotchas.md` the caveats that index points at.
3. `src/platform/os/windows/api_layer/user/archive/` - per-TU narratives (`tu/<name>.md`, sessions 1-15 and 21-33) and the
   session-log rows (the only narrative for sessions 16-20). Frozen, authoritative history; **not boot material**. Since
   session 34 the session narrative is the review banner at the top of each reviewed `.cpp`.
4. `memory-bank/archive/` - session records moved out of this file (index: `memory-bank/archive/README.md`; **not boot
   material** - do not read it at session start).

**If previous activities need to be researched they are in the git history**

