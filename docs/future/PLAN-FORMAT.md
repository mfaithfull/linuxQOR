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
- `## Resume` - cursor pointing to the first incomplete while plan is in progress
- `## Context` - inherited at creation: the parent `## Goal` line, the parent locked decisions, the sibling titles. A child planned in isolation loses the parent decisions, so this section is what keeps it attached.
- `## Decisions` - choices this plan locks, linked to `docs/agent/DECISIONS.md` D-entries rather than restated.
- `## <Item Title>` - one per item, in item order: the narrative for that item.

## Slug rule

Folder name from a title: drop `the`, `a`, `an`, `of`; PascalCase the remaining words; ASCII only; strip everything outside `[A-Za-z0-9]`; cap at 24 characters; on collision append a 2-digit counter. Windows paths are case-insensitive, so the counter is load-bearing. Record the chosen slug in the item `Content:` path; uniqueness is stored, never recomputed.

## Gate

Ask of each item, first match wins:

| Test | Refine? |
|---|---|
| Status is `Refined`, `Atomic`, or `Deferred` | no |
| This plan `Level` >= 5 | no |
| Item names one concrete action on one named artifact (one file, one edit, one build/test/gate run) | no |
| Item is a single question to the user | no |
| Item is itself a planning instruction | no |
| otherwise | yes |

Breadth cap: at most 7 items per plan, per the repo rule of 7. Depth cap: a plan at `Level: 5` holds terminal items only, so no `plan.md` sits at `Level: 6`.

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
