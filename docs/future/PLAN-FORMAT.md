# Plan format

The record format for every `plan.md` in a deep-planning tree. [`SKILL.md`](SKILL.md) points here; this file is the single source of truth for the schema, the statuses, the slug rule, and the gate.

## Front matter

YAML front matter carries machine state only. The markdown body carries the prose. Pointers are relative to the file that holds them.

```yaml
---
Type: Plan
Title: "Write the game engine"
Parent: "../plan.md"      # path relative to this file; empty at the root
Level: 1                  # root = 0; a child is its parent Level + 1
Items:
  - Title: Write the engine core
    Status: Planning
    Content: ./WriteTheEngineCore/plan.md
  - Title: Add one file
    Status: Atomic
    Content:
---
```

`Content:` is always a path pointer. It is empty while an item is unrefined and never inlines a plan.

## Statuses

| Status | Meaning | Refine it? |
|---|---|---|
| `Planning` | no child plan yet | yes, if the gate says so |
| `Drafted` | child plan written, its subtree still has work | resume inside it |
| `Refined` | child plan written and its whole subtree terminal | no |
| `Atomic` | one concrete action; owns no child plan | no |
| `Deferred` | out of scope for this effort | no |

A plan is **terminal** when every item is `Refined`, `Atomic`, or `Deferred`. A tree is done when its root plan is terminal.

## Body sections, in order

- `## Goal` - one line: what this plan is finding its way to.
- `## Resume` - cursor pointing to the first incomplete step while plan is in progress
- `## Context` - inherited at creation: the parent `## Goal` line, the parent locked decisions, the sibling titles. A child planned in isolation loses the parent decisions, so this section is what keeps it attached.
- `## Decisions` - choices this plan locks, linked to `docs/agent/DECISIONS.md` D-entries rather than restated.
- `## <Item Title>` - one per item, in item order: the narrative for that item.

## Slug rule

Folder name from a title: drop `the`, `a`, `an`, `of`; PascalCase the remaining words; ASCII only; strip everything outside `[A-Za-z0-9]`; cap at 24 characters; on collision append a 2-digit counter. Windows paths are case-insensitive, so the counter is load-bearing. Record the chosen slug in the item `Content:` path; uniqueness is stored, never recomputed.

## Gate

Ask of each item, first match wins. The size tests run **before** the artifact test, because first-match-wins otherwise lets "one action on one named artifact" swallow a 124-row step and call it `Atomic`.

| Test | Refine? |
|---|---|
| Status is `Refined`, `Atomic`, or `Deferred` | no |
| This plan `Level` >= 5 | no - but see the depth-cap rule below: an oversize item here is `Deferred` with a written reason, never `Atomic` |
| The item's unit count over a named collection exceeds the leaf budget. The count is read from an instrument line `ITEM <title> units=N` printing `claimed = N of N` over the artifact, never from prose | **yes** - cut the collection, not the verb |
| The item's action repeats over a named collection but no measured unit count is stated for it | **yes** - measure first, then apply the row above |
| The item must discover or validate its own inputs at execution time (an input whose existence/resolution no prior item has checked) | **yes** - add a preceding verification item; a leaf may not pay for its own inputs |
| The item's body does not enumerate its units by id (row id / stem / `file:line` / URL slug), one per unit | **yes** - an unenumerated leaf is not a step |
| The item mixes unit kinds and a secondary kind exceeds the dominant kind 1:1 | **yes** - one leaf does one pass of one kind over the same units |
| The item names one concrete action over its enumerated units, its unit count is within the leaf budget, every input is already verified, and it states its stop condition as an instrument or a re-read that prints a number | no |
| Item is a single question to the user | no |
| Item is itself a planning instruction | no |
| otherwise | yes |

Leaf budget: **10 units of the dominant kind per leaf**, and every secondary kind **at most 1:1** with it plus exactly one stop check. So a fetch leaf of 10 URLs may also write those same 10 rows and run 1 census - 10 fetch + 10 write + 1 census is inside the budget; 10 fetch + 25 edits is not. The bound is per leaf, not per plan: a 55-row artifact becomes 6 slices of 10, and the plan that owns them is a `Refined` item, never a step. A leaf may not name a second unit of the same kind after its stop condition.

A **unit** is work an agent performs by hand - a fetch, an edit, a lookup, a written row. A scripted pass over the whole artifact (one instrument that reads every row and prints a count) is **one** unit, and its stop condition is the line it prints. That is what keeps the budget honest on both sides: a 597-row probe is one `Atomic` item, a 597-row fill is 92.

Stop condition: every `Atomic` item states, in its section, the one check that closes it - an instrument line or a re-read that prints a count. An item without a stated stop condition is not a step - refine it.

Input rule: an `Atomic` leaf consumes inputs a prior item has already verified and marked in the artifact (a verified column, a probe log, a build that passed). An unverified input makes the leaf non-atomic, because the leaf would otherwise spend its budget on discovering its own work - the fix is a preceding verification item, never a bigger leaf.

Enumerated units rule: the leaf's body lists the exact ids it owns, so a session can check its work against the list. A body that says "same procedure" with no ids is not a step.

Depth-cap rule: at `Level: 5` an item that fails the size or input tests is `Deferred` with a written reason naming the sub-plan that owns it - the depth cap stops recursion, it does not license an oversize step.

Cut rule: when a collection is cut, the cut is fixed by an instrument that prints `claimed = N of N` and "every unit owned by exactly one item" over the artifact, so no unit is owned twice and none is left unfetched. Prose counts are not the authority.

Breadth cap: at most 7 items per plan, per the repo rule of 7. Depth cap: a plan at `Level: 5` holds terminal items only, so no `plan.md` sits at `Level: 6`.

Budget note: "one action on one named artifact" is not enough - a step that fills 124 rows of one CSV passes the artifact test and still cannot be finished in one pass. The unit-count test is what makes `Atomic` mean "one session can close it", and it is measured, not asserted. The input rule is the second half of the same lesson: the 124-lookup step also carried URLs nothing had checked, so its budget was spent on 404s rather than on rows.

## Handoff

will be added by handoff skill when the plan is fully refined.

## Worked tree

Request: "Plan how to write a 3D game"

```
docs/future/WriteA3DGame/plan.md                                      Level 0
docs/future/WriteA3DGame/WriteTheGameEngine/plan.md                  Level 1
docs/future/WriteA3DGame/WriteTheGameEngine/WriteTheRenderer/plan.md  Level 2
docs/future/WriteA3DGame/WriteTheGameLogic/plan.md                   Level 1
docs/future/WriteA3DGame/ExtensiveTesting/plan.md                    Level 1
```

Root items: `Write the game engine` (`Drafted`, `Content: ./WriteTheGameEngine/plan.md`), `Write the game logic` (`Planning`), `Extensive testing` (`Planning`). The root `Content:` and the child `Parent: ../plan.md` resolve both ways, so any node is reachable from the root with no index other than the folders.
