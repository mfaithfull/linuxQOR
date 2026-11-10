# Implementation Plan

## Overview
Turn the procedure in `docs/Deep planning.txt` into a reusable, model-invoked skill named `deep-planning` that converts one loose request into a persistent hierarchical plan tree under `docs/future/`, refining one node per skill invocation by calling itself, and resumable after a context reset.

Scope: three new skill files under `C:\Users\mfaithfull\.agents\skills\deep-planning\` plus one repo convention file. No product code, no CMake, no build changes.

Approach: keep the format the doc proposes, because it already matches a convention this repo uses (`docs/agent/DECISIONS.md` front matter + `File:` pointers + prose in the body). Repair the algorithm bugs, then add the state the doc omits: a terminal status, context inheritance, a resume protocol, a breadth cap, a slug rule, and tree verification.

## Decisions locked

- Invocation: **model-invoked** (user chose). Omit `disable-model-invocation`; write a model-facing `description` carrying the trigger branches so the agent can re-fire the skill for each child node. `agents/openai.yaml` sets `allow_implicit_invocation: true`.
- Root of a tree: `docs/future/<RequestSlug>/plan.md` (repo-relative). Children nest inside the parent folder: `docs/future/<RequestSlug>/<ItemSlug>/plan.md`.
- Format: YAML front matter = machine state only; the markdown body carries the prose. Pointers are relative to the file that holds them.
- `Content:` is always a path pointer (empty until refined). It never inlines a plan.
- Status enum replaces the two-state vocab: `Planning`, `Drafted`, `Refined`, `Atomic`, `Deferred`.
- Breadth cap: at most 7 items per plan (repo rule of 7, `docs/skills/QORconventions.md`). Depth cap: never refine an item in a plan whose `Level: 5` or higher.
- The doc line `Return to step 1` is a bug: step 1 creates a root plan from the request and would erase the tree. Return to step 2.
- The doc sub-step labels (`a,b,c,d,c,e`) are mislabelled; the corrected order lives in the Functions section.
- The gate `makes a single edit / single file / single tool call` is undecidable at plan time; reframed as `names one concrete action on one named artifact`.
- Planning only. Implementation is a hand-off, not a step of this skill.
- The root plan registers in `docs/agent/TODO.md`; its locked choices link to `docs/agent/DECISIONS.md` D-entries; no duplication.

## Types

Front matter of every `plan.md`:

```yaml
---
Type: Plan
Title: "Write the game engine"
Parent: "../plan.md"        # path relative to this file; empty at the root
Level: 1                    # root = 0; child = parent + 1
Items:
  - Title: Write the engine core
    Status: Planning        # Planning | Drafted | Refined | Atomic | Deferred
    Content:                # path to the child plan; empty until Drafted or Refined
  - Title: Rename one file
    Status: Atomic
    Content:
---
```

Body sections, in order: `## Goal` (one line: what this plan is finding its way to), `## Context` (inherited: parent Goal line, parent locked decisions, sibling titles), `## Decisions` (choices locked by this plan), then `## <Item Title>` one per item.

Status meaning: `Planning` no child plan, needs refinement / `Drafted` child plan exists, subtree still has work / `Refined` child plan exists and its whole subtree is terminal / `Atomic` needs no refinement and owns no child plan / `Deferred` out of scope, never descends.

Slug rule for folder names: drop `the`/`a`/`an`/`of`, PascalCase each remaining word, ASCII only, strip everything outside `[A-Za-z0-9]`, cap at 24 characters, append a 2-digit counter on collision. Record the chosen slug in the item `Content:` path so uniqueness is stored, not recomputed.

Refinement gate, first match wins: `Refined`/`Atomic`/`Deferred` -> no / plan `Level` >= 5 -> no / item names one concrete action on one named artifact (one file, one edit, one build-test-gate run) -> no / item is one question to the user -> no / item is itself a planning instruction -> no / otherwise -> yes.

## Files

**New**
- `C:\Users\mfaithfull\.agents\skills\deep-planning\SKILL.md` - the driver: front matter, corrected procedure, gate, resume protocol.
- `C:\Users\mfaithfull\.agents\skills\deep-planning\agents\openai.yaml` - `interface` (display_name "Deep Planning", short_description) + `policy: allow_implicit_invocation: true`; mirrors every existing skill in that folder.
- `C:\Users\mfaithfull\.agents\skills\deep-planning\PLAN-FORMAT.md` - disclosed reference: the front-matter schema, status table, slug rule, gate table, and the worked example tree from the doc. Reached from `SKILL.md` by one pointer (progressive disclosure).
- `c:\RAID0\Develop\linuxQOR\docs\future\purpose.md` - repo convention: every folder carries a `purpose.md`; explains the tree and points at `docs/agent/TODO.md`.

**Modified**
- `docs/agent/TODO.md` - one queue entry per tree started, `[~]` marker, linking the root `plan.md`.

**Move (optional, after the skill passes)**
- `docs/Deep planning.txt` -> `docs/archive/deep-planning-spec.md` with the RETIRED banner used in `docs/archive/qor_mime_plan.md`, once the skill supersedes it.

## Functions

Procedures named in `SKILL.md`, each with its trigger:

1. **Chart a tree** - trigger: a request too big for one session, no matching tree on disk. Create `docs/future/<RequestSlug>/plan.md` at `Level: 0`, empty `Parent:`, 3-7 items `Status: Planning`, body `## Goal`/`## Context`/`## Decisions`; register in `docs/agent/TODO.md`; then Refine the first item.
2. **Refine one node** - trigger: the current plan has an item the gate sends to refinement. Slug the title; create the child folder if absent; write `<child>/plan.md` at `Level: parent + 1`, `Parent: ../plan.md`, seeding `## Goal` from the item title and copying `## Context` down from the parent; set the parent item `Content:` to that path and `Status: Drafted`; call the Skill tool with `deep-planning` for that item; on return re-read the parent, keep it coherent, set the item `Status: Refined`.
3. **Climb back** - trigger: the current plan has no item needing refinement. Mark remaining items `Atomic` or `Deferred`, return to the caller, set that parent item `Refined`, and let the parent take its next item needing refinement. Climb until some ancestor has work left.
4. **Resume** - trigger: a tree on disk, no new request. Read the roots under `docs/future/*/plan.md`, walk pre-order (root first, items in document order, descend into `Drafted`), take the first `Planning` or `Drafted` item. Never recreate a folder that exists.
5. **Verify after every node** - every `Content:` path exists; every `Parent:` resolves; `Level` is parent + 1; at most 7 items; at most one `Drafted` item per plan. Fix a break in the plan file that holds it.
6. **Hand off** - trigger: every item in the root tree terminal. Append `## Handoff` to the root naming the deepest complete plan for the item the user picks, plus `/implement` or `/to-tickets` for the work itself.

## Classes

Record types (no code classes change):
- **`Plan` record** - one `plan.md` file; fields `Type, Title, Parent, Level, Items`; owns exactly one folder and 0 or N child `Plan` records.
- **`Item` record** - a list entry; fields `Title, Status, Content`; links to one child `Plan` through `Content:`.
- **Tree root** - exactly one `Plan` with empty `Parent:`, at `docs/future/<RequestSlug>/plan.md`.
- **Invariants** - parent and child `plan.md` sit in adjacent folders; the `Parent:`/`Content:` pair resolves both ways; the file name is always `plan.md`.

## Dependencies

None new. Uses the Skill tool for self-invocation, the file tools for reading and writing, and `run_commands` for the verification walk. Authoring reference: `writing-for-agents` (model-invoked, already installed) for front matter and disclosure rules. Optional siblings referenced by name only, not wired in: `grilling`, `domain-modeling`, `to-tickets`, `implement`.

## Testing

No unit tests apply to a markdown skill; verify by running it:
1. Front matter parses: `name` and `description` present, no `disable-model-invocation`; `agents/openai.yaml` mirrors an existing skill.
2. Trigger test: a fresh session asked "plan how to write a 3D game" fires `deep-planning` with no human naming it.
3. Chart test: `docs/future/WriteA3DGame/plan.md` exists, `Level: 0`, 3-7 items, all `Planning`, `Parent:` empty.
4. Refine test: one child folder plus child `plan.md`, `Parent: ../plan.md`, `Level: 1`, non-empty `## Context`, parent item `Status: Drafted`, correct `Content:` path.
5. Depth cap: no `plan.md` at `Level: 6`. Breadth cap: no plan with 8 items.
6. Resume test: on a half-refined tree a new session picks the first `Planning`/`Drafted` item pre-order and does not recreate the root.
7. Collision test: two titles that slug alike get distinct folders.
8. Verification walk (read-only PowerShell): every `Content:` resolves, every `Parent:` resolves, `Level` equals parent + 1.

## Implementation Order

1. Create `C:\Users\mfaithfull\.agents\skills\deep-planning\` and its `agents\openai.yaml`.
2. Write `PLAN-FORMAT.md` (schema block, status table, slug rule, gate table, worked example tree).
3. Write `SKILL.md` front matter (name, description with trigger branches, `argument-hint`).
4. Write the `SKILL.md` body: overview, the six procedures in order, the gate as a decision list, the resume protocol, the verification walk, one pointer to `PLAN-FORMAT.md`.
5. Apply `writing-for-agents` rules: one source of truth per meaning, leading words, positive targets, progressive disclosure.
6. Create `c:\RAID0\Develop\linuxQOR\docs\future\purpose.md`.
7. Add the `TODO.md` registration line to the Chart-a-tree procedure.
8. Dry-run: invoke `deep-planning` on a throwaway request; inspect the tree against the 8 checks.
9. Fix any invariant break in `SKILL.md`, not in the tree.
10. Resume test on the half-built tree from the dry run.
11. Retire `docs/Deep planning.txt` to `docs/archive/` once the skill passes.
12. Delete the dry-run tree from `docs/future/` (keep `purpose.md`) unless the user wants it kept.

## Open items for the user

- `docs/future/` is not in `.gitignore` (re-checked after writing), so charted plan trees become committable. Add `docs/future` to `.gitignore` if plan trees should stay out of git.
- `docs/Deep planning.txt` is left in place until the skill passes its dry run.
- `docs/agent/TODO.md` registration is a step of the skill, not a pre-written entry: no queue line exists until a tree is charted.
