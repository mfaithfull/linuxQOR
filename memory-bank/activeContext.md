# Active Context

Last updated: 4 Nov 2026 (session 33 `window.cpp` CLOSED - **34 of 38 TUs reviewed, 4 left**; the review is in its last
mile). **The headline: P1 is CLOSED folder-wide - 0 unresolvable bindings of 428.** `window.cpp`'s `CreateWindowT` bound
`qor_pp_useswinapiAW(user32, CreateWindow)` -> `CreateWindowW`, which **is not an export and never can be**: the SDK's
`CreateWindow`/`CreateWindowW` are function-like macros forwarding to `CreateWindowExA/W` (`winuser.h:4464`, `:4468`) and
the binding macro stringizes the pasted name before any macro can expand (G1) - so every spelling of that stem is a P1,
which is why the fix is to bind the real export with an explicit `0` `dwExStyle`. Same TU: **U18's founding pair**
(`LogicalToPhysicalPoint`/`PhysicalToLogicalPoint`, `void` + `voidCall` -> `BOOL`, header included - a published-signature
change, consumer probe clean), **two dropped `const`s** restored, and **U14(b)'s last three argument-deducing `Call<>`
sites** made explicit -> the folder is now **428 explicit / 0 deducing** while U14(a) stays at its D15 freeze (163 spaced /
265 tight). U16's largest guarded population yet: **34 of 85** prototypes version-gated, highest floor `0x0605`, three
guard names new to the review. **HEAD is still at session 32 + the gotchas refactor (`0a24b655`) - session 33's 20 modified
+ 3 new record/source files are uncommitted at this close**; commit them before starting session 34.
Record state after this close: `status.md` **198 lines / 20.5 KB**, `archive/session-log.md` 233 lines / **29.0 KB** (its
31/33 close-out notes sliced into `archive/session-log-notes-31-33.md`, 135 lines / 12.9 KB, hash-verified rejoin),
`gotchas/sessions-31-33.md` (renamed from `sessions-31-32.md`), `tooling.md` **~36.5 KB - next over the ceiling**.
Close audits all green: gate `tmp/_tmp_cl_all_final3_s33.log` (38 echoes, 0 diagnostics, 0 stale `.obj`, run 22:25:10 after
the last source touch at 20:11:31 - the earlier `final2` run was invalidated by a late comment-only edit), locator
46/46 pointing right, cite audit 41 cited artefacts / 0 unexpected missing, `registers/README.md` + `archive/README.md`
line counts refreshed. The record tree is `src/platform/os/windows/api_layer/user/{status,method,tooling,gotchas}.md` +
`registers/` + `archive/`. Completed-session records stay in `memory-bank/archive/`, never here.

## Current focus: winqor_api_user source review (one TU per session)

- Reviewing `src/platform/os/windows/api_layer/user` (class `qor::win::api::User32` over `user32.dll`) for
  correctness, consistency, safety, UB and convention conformance. **Protocol: exactly one translation unit
  per session**; small unambiguous defects fixed in place, everything behavioural recorded as an open item.
- **Session 33 (`window.cpp`) closed 4 Nov 2026 - 85 wrappers, 6 code fixes; the last TU of the review with defects.**
  Its record additions are `registers/prescan.md` (**P1 CLOSED**, P5 re-measured to 2 benign rows), `u18.md` (founding
  pair fixed, item kept open as the published-type list), `u16.md` (34/85 gated), `u19.md` (71 LAYER / 49 INDIRECT upper
  bound / 49 BYPASS with 2 real groups; the `TrackPopupMenu` lead discharged to `menu.cpp`), `u14.md` (b) closed sites),
  `u25.md` and `census.md`. Sessions 28-32 and 23-27 stay in `archive/tu/<tu>.md` + the session log, not here. The
  load-bearing rule from session 25 still governs the four remaining TUs: `AW` over a stem with **no A/W family** throws
  on every call, so bare `qor_pp_useswinapi` is **D13's other half**, not a style call - and session 33 adds its mirror:
  a stem whose bare name is a **macro** can never be bound at all. Census correction booked with this close: **428 bindings
  is the closed folder total over all 38 sources, not a floor** - the scans always walked all 38 files, so the session-32
  "add `window.cpp`'s 85 later" framing was wrong (`registers/census.md`, session-33 entry).

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
