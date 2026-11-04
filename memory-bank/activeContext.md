# Active Context

Last updated: 4 Nov 2026 (session 32 `text.cpp` CLOSED - **33 of 38 TUs reviewed**.
class fixed**: the folder's only two `WINAPI`-qualified `User32::` definition heads (`ShutdownBlockReasonDestroy`/`Query`)
disagreed with the unmarked header declarations, which makes the TU **ill-formed for x86** (`C2373` x2) and a **no-op for
x64** - proved by a caller probe with one identical `__cdecl` decoration on both sides, `dumpbin /disasm` byte-identical
and a `/EP` diff whose only code change is those two heads. The folder-wide measure that came out of it is in
`registers/u6.md`: **x86 is unbuildable - 93 of the 106 api_layer TUs fail at `kernel32.h:92` (`PUNWIND_HISTORY_TABLE`)**,
the 13 survivors being exactly the `winqor_api_*.cpp` glue TUs; that same blind spot is why 30 sessions of x64 gates never
saw the `C2373` pair. HEAD carries sessions 1-30; **session 31 is committed at this close**. **Next: session 32
`text.cpp`** (44 lines, 4 `*T` text wrappers, all `qor_pp_useswinapiAW` over A/W-only stems) - pre-flags are the last block
of `status.md`, full hand-off at the end of `archive/session-log.md`. Record state after this close: `status.md` 214 lines
/ 21.5 KB, `archive/session-log.md` **41.1 KB - now over the 40 KB ceiling and the top slice candidate** (rows 27-31),
`gotchas.md` 34.5 KB, `registers/u16.md` 279 lines. `method.md`'s "Session start-up cost" rules and the three close
audits (compile gate dated by `.obj` freshness, `tmp/_tmp_s31_cite_audit_s31.ps1`,
`tmp/_tmp_u15_locator_verify.ps1`) apply every session; `tooling.md` needs the session-31 instruments added at its next
slice. `method.md` was **split on 2 Nov 2026** into `method.md` (the numbered steps), `tooling.md` (the instrument index)
and `gotchas.md` (the caveats it points at). Completed-session records stay in `memory-bank/archive/`, never here.

## Current focus: winqor_api_user source review (one TU per session)

- Reviewing `src/platform/os/windows/api_layer/user` (class `qor::win::api::User32` over `user32.dll`) for
  correctness, consistency, safety, UB and convention conformance. **Protocol: exactly one translation unit
  per session**; small unambiguous defects fixed in place, everything behavioural recorded as an open item.
- **Session 31 (`systemshutdown.cpp`) closed 4 Nov 2026 - 1 defect class fixed; 32 of 38 TUs reviewed.** The TU's two
  `WINAPI`-qualified definition heads were a real (x86-only) ill-formedness, not style. Durable record additions:
  `registers/u6.md`'s addendum (**x86 buildability - 93 of 106 api_layer TUs fail on `kernel32.h:92`
  `PUNWIND_HISTORY_TABLE`**) and the refuted 0x0601 U16 lead in `registers/u16.md`. Sessions 28-30 (`rectangle.cpp`,
  `resource.cpp`'s **U11** fix, `scrollbar.cpp`'s module-token normalization) and 23-27 (`message.cpp` **P2 CLOSED**,
  `monitor.cpp`, `mouse.cpp` **10 P1 binding fixes**, `painting.cpp`, `processthread.cpp`) are closed; their detail is
  `archive/tu/<tu>.md` and the session log, not here. The load-bearing result of session 25 stays in force: `AW` over a
  stem with **no A/W family** throws at run time on every call, so bare `qor_pp_useswinapi` is **D13's other half**, not a
  style call.

## Decisions in force (pointer only - not restated here)
 - Project wide decisions are recorded in `docs/agent/DECISIONS.md`. The memory bank deliberately does not duplicate their text.
- **Rule adopted 2 Nov 2026 (user):** this file carries the current focus, and pointers **never a per-session narrative**. At each session close, the outgoing "recent changes" entry is either reduced to the two or three
lines a new session needs to start, or moved to `memory-bank/archive/`. Session detail belongs in the project record tree
(item 3), not here.

## Where the record lives (boot order)

1. `src/platform/os/windows/api_layer/user/status.md` - the review boot file: live state, TU queue, register locator,
   verification gate and the current TU's pre-flags.
2. `src/platform/os/windows/api_layer/user/registers/` - one file per open item; open at the locator line, never the
   whole tree. `../method.md` holds the procedure, `../tooling.md` the instrument index and `../gotchas.md` the caveats
   that index points at.
3. `src/platform/os/windows/api_layer/user/archive/` - per-TU narratives (`tu/<name>.md`) and the row index
   (`session-log.md`). This is the authoritative session history; the memory bank keeps no per-session narrative.
4. `memory-bank/archive/` - session records moved out of this file (index: `memory-bank/archive/README.md`; **not boot
   material** - do not read it at session start).

**If previous activities need to be researched they are in the git history**
