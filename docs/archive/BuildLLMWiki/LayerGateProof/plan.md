---
Type: Plan
Title: "Layer gate proof"
Parent: "../plan.md"
Level: 1
Items:
  - Title: Build the fixtures
    Status: Atomic
    Content:
  - Title: Run the gates and read the reds
    Status: Atomic
    Content:
---

# Layer gate proof

## Goal

Prove that the gates fire, by constructing the failure modes deliberately - so that a green gate
over a broken wiki is caught here rather than believed later.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: CLOSED 18 Nov 2026` - both `Atomic` leaves closed.
  **Build the fixtures**: six units under `tmp/wiki_fixture/` plus `tmp/wiki_fixture_raw/pending.md`
  - `fx-dead-anchor`, `fx-illegal-field`, `fx-secret`, `fx-unstamped`, `fx-over-budget` (210 lines)
  and the queued pair `fx-orphan-entry` / `fx-carried-leak` (carried via `Sources:` front matter on
  `fx-dead-anchor.md`); the leak scan found **0** fixture files outside `tmp/`.
  **Run the gates and read the reds**: 6 of 6 reds over the fixture, each its own `VERDICT fail = 1`
  (`tmp/_tmp_gp_*.txt`) - anchors `dead = 1`, orphans `carried_not_deleted = 1` (the plain queued
  entry prints `fail = 0` as designed), retire stamp `unstamped = 1` (the `stamp` action also
  REFUSES a fixture page: `not one of [archive]`), schema `illegal_fields = 1`, provenance
  `violations = 1`, size `over_budget = 1`. The same six gates over the real wiki root print
  `VERDICT fail = 0` (`tmp/_tmp_gp_real2_*.txt`): 156 pages, anchors `516 of 516 / dead = 0`,
  schema `illegal_fields = 0` + `governance_drift = 0`, provenance `violations = 0`, freshness
  `stale = 0 / unstamped = 0 / undated = 151`, size `over_budget = 0 / max_lines = 195`,
  orphans `0 of 0 / carried_not_deleted = 0`.
- Corrections the run forced, because `provenance` counts each hit (`fail = violations`) so the
  real-rerun `fail = 0` admits no exception: five real-wiki provenance reds were **fixed in the
  run**, not recorded - three introduced by this tree's own decision pages (the
  `dumpbin /exports C:\Windows\System32\user32.dll` judge lines at `decisions/D39.md`, `D48.md`,
  `D49.md` now read `$env:SystemRoot\System32\user32.dll`, the same command, portable spelling),
  and two pre-existing module-page findings (`modules/qor_task.md` dropped the cppcoro issue URL
  per the schema's "No URL" rule - the reference stays reconstructible by name; `modules/qor_signals.md`
  reworded `wrong token: ...` to break the secret pattern's `token\s*[:=]`). Side effect: freshness
  `evidenced` fell 8 -> 5 (three judges no longer name a statically-resolving path); `undated` is
  informational and no verdict moved. `wiki-ref-check refs` keeps its one known pre-existing finding
  (`modules/qor_netserver.md -> maybe_unused`) - not a gate-proof condition; reported at item 7.

## Context

Inherited at creation - re-read rather than trusted, the tree has moved since:

- **Parent goal**: land the three layers so that maintenance is instruments, not an LLM deciding
  freshness.
- **Parent locked decisions**: an instrument that has never seen a red case is untested; the red
  case is constructed in `tmp/` fixtures and never by damaging a tracked page; the pair
  `pending=0` and `reconcile fail=0` is the verdict, and `0 of 0` proves only that a parse happened.
- **Sibling plan titles**: `Layer scaffolding and folders`, `Layer instruments`,
  `Layer governance`, `Layer pages`, `Layer ops`. This plan runs last - it is the only place a red
  case is legal, and only inside a fixture.
- **Failure modes**: `docs/WikiDesign.md` §10, F1-F12.

## Decisions

- **Fixtures live in `tmp/`** - §11. `tmp/` is gitignored and cleaned per task, so a fixture cannot
  become a tracked lie. The gate instruments take the wiki root as an argument for exactly this
  reason (§11's tool-contract rule: no default may encode a task).
- **A green gate over a fixture is a design failure** - reported as such, not retried until it turns
  red. Retrying until green is the thing that makes an untested gate look tested.

## Build the fixtures

Input verified: `wiki-census` and `wiki-ref-check` exist and passed their self-tests, so the
fixture's shape is known rather than assumed.

One fixture wiki under `tmp/wiki_fixture/`, six units enumerated by id: `fx-dead-anchor` (a page
whose `path:` names a file that does not exist); `fx-orphan` (a `docs/raw/pending.md` entry no
page references); `fx-unstamped` (a page retired with no reason leg); `fx-illegal-field` (a field
absent from `schema.md`); `fx-secret` (a page carrying an absolute path); `fx-over-budget` (a page
past the budget the schema states).

Stop condition: a listing prints `fixtures = 6 of 6`, and a scan confirms **0** fixture files exist
outside `tmp/` - a fixture that leaks into `docs/` is a tracked lie and a run failure.

## Run the gates and read the reds

Six units, enumerated by the fixture id each one exercises, each with its expected red:
`fx-dead-anchor` through `wiki-ref-check` mode `anchors` must print `dead >= 1` (F1);
`fx-carried-leak` through `wiki-ref-check` mode `orphans` must print `carried_not_deleted >= 1` (F6 -
a page's `Sources:` cites an id whose queue entry still exists); the plain queued `fx-orphan-entry`
must print `fail = 0`, because an unconsumed entry is the queue's legal state and `orphans >= 1` is
open work, not a red; `fx-unstamped` through `wiki-retire` mode `stamp` must print `unstamped >= 1`
(F11 - and the `stamp` action itself refuses any page outside an `archive/` folder);
`fx-illegal-field` through `wiki-lint` mode `schema` must print `illegal_fields >= 1` (F10);
`fx-secret` through `wiki-lint` mode `provenance` must print `violations >= 1` (F7);
`fx-over-budget` through `wiki-lint` mode `size` (the subcommand's real name - there is no `budget`
subcommand) must print `over_budget >= 1` (F9/F8).

Input verified: the six fixtures exist on disk and the item above printed `fixtures = 6 of 6`.

Blocking precondition: the six fixtures exist and printed `6 of 6` from the item above.

Stop condition: **6 of 6** expected reds printed, each gate's own line re-reading `VERDICT fail=1`
over the fixture, and the same gates re-run over the real wiki root printing `VERDICT fail=0`.
If any gate prints `fail=0` over the fixture, report it as a **design failure** naming the gate and
the fixture, and do not retry until red - an untested gate is the F1-shaped bug this whole design
exists to prevent.
