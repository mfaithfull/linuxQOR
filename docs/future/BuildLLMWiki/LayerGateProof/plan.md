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

- `Cursor: not started` - 2 `Atomic`, in item order.

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
`fx-orphan` through `wiki-ref-check` mode `orphans` must print `orphans >= 1` (F6);
`fx-unstamped` through `wiki-retire` mode `stamp` must print `unstamped >= 1` and refuse (F11);
`fx-illegal-field` through `wiki-lint` mode `schema` must print `illegal_fields >= 1` (F10);
`fx-secret` through `wiki-lint` mode `provenance` must print `violations >= 1` (F7);
`fx-over-budget` through `wiki-lint` mode `budget` must print `over_budget >= 1` (F9/F8).

Input verified: the six fixtures exist on disk and the item above printed `fixtures = 6 of 6`.

Blocking precondition: the six fixtures exist and printed `6 of 6` from the item above.

Stop condition: **6 of 6** expected reds printed, each gate's own line re-reading `VERDICT fail=1`
over the fixture, and the same gates re-run over the real wiki root printing `VERDICT fail=0`.
If any gate prints `fail=0` over the fixture, report it as a **design failure** naming the gate and
the fixture, and do not retry until red - an untested gate is the F1-shaped bug this whole design
exists to prevent.
