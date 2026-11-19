---
Type: Plan
Title: "Index drift gate and ingest catalog step"
Parent: ""
Level: 0
Items:
  - Title: Add the check subcommand
    Status: Atomic
    Content:
  - Title: Add the catalog step to the skill
    Status: Atomic
    Content:
  - Title: Record the drain order in schema
    Status: Atomic
    Content:
  - Title: Land the two commits clean
    Status: Atomic
    Content:
  - Title: Restate the op in WikiDesign
    Status: Deferred
    Content:
---

# IndexDriftGateAndIngest

Slug: `IndexDriftGateAndIngest` - from "Index drift gate and ingest catalog step" after dropping
`the`, `and`, PascalCasing, ASCII, 23 chars; no collision in `docs/future`.

## Goal

Make a stale `docs/wiki/index.md` impossible between commits by regenerating it inside the ingest
drain and printing its drift so a later forgetful drain goes red instead of quietly lying.

## Resume

Cursor: `Add the check subcommand`. Nothing from this plan is executed yet. Terminal when the four
`Atomic` items and the one `Deferred` item are all closed/deferred and the final gate block leaves
only the standing `refs broken = 1`.

## Decisions

1. Regenerate `index.md` as part of ingest; duplication is accepted because regeneration is one
   step. User wording: drift matters more than duplication.
2. Do not delete `Summary` from page front matter and do not thin the index row format.
3. Keep `index.md` generated and never hand-edited.
4. Add drift verification as a gate/check command, not prose only.
5. Do not fix the standing `refs broken = 1` in this plan.

## Context

Agreed 19 Nov 2026 after the `hook-fd3b46ba9f-qor_stringsource` drain (commit `44ba8861`).

Current measured state:
- `docs/raw/pending.md` is empty: `pending = 0 of 0`.
- `docs/wiki/index.md:156` is stale for `qor_stringsource`.
- `tools/wiki-index/wiki-index.ps1` has `Get-WikiIndexRow` 34-52, `Invoke-IndexBuild` 55-95,
  subcommand registration 191-197, dispatcher 206-217.
- Current `build` fails only on page-row count mismatch, not changed summary text.
- `memory-bank/tools/wiki-index/tool.md` still documents only `build | lookup | diff`.
- `docs/skills/wiki-ingest.md` currently has 8 numbered steps and no `wiki-index` gate.
- `docs/wiki/schema.md` ingest Ops row currently says `carry -> print -> delete`.
- Standing accepted red: `refs broken = 1` for `modules/qor_netserver.md -> maybe_unused`.

## Add the check subcommand

Units:
1. Read `tools/wiki-index/wiki-index.ps1:34-95,191-217` to verify the facts above.
2. Edit `tools/wiki-index/wiki-index.ps1:55-95`: extract catalog/header assembly into a reusable
   function used by `build`.
3. Edit `tools/wiki-index/wiki-index.ps1:55-95`: add `Invoke-IndexCheck` that compares generated
   catalog lines to `docs/wiki/index.md` and prints `drift = <D> of <N>`.
4. Edit `tools/wiki-index/wiki-index.ps1:191-217`: register `check` in usage, param handling,
   `$subcommands`, and dispatcher.
5. Edit `memory-bank/tools/wiki-index/tool.md`: add `check` to `What it does` and `Contract`.
6. Run check before regeneration: expect stale row drift (`drift = 1 of 156`, `VERDICT fail=1`).
7. Run build.
8. Run check again: expect `drift = 0 of 156`, `VERDICT fail=0`.

Stop condition: `check` prints `drift = 1 of 156` before `build` and `drift = 0 of 156` after
`build`.

## Add the catalog step to the skill

Units:
1. Read `docs/skills/wiki-ingest.md` to verify 8 steps and no `wiki-index` step.
2. Edit `docs/skills/wiki-ingest.md`: insert a `catalog` step after `delete`, before `gates`.
3. Edit `docs/skills/wiki-ingest.md`: renumber steps so execution order is 1-9.
4. Edit `docs/skills/wiki-ingest.md`: in the commit step, name `index.md` as committable generated
   output.

Stop condition: re-read prints 9 numbered steps and a `catalog` step before `gates`.

## Record the drain order in schema

Units:
1. Read `tools/wiki-lint/wiki-lint.ps1` Ops-table parsing before editing.
2. Edit `docs/wiki/schema.md` ingest row: change `carry -> print -> delete` to
   `carry -> print -> delete -> catalog`.
3. Edit `docs/wiki/schema.md` under Ops: state that `catalog` regenerates and checks `index.md`.
4. Run `pwsh -NoProfile -File tools/wiki-lint/wiki-lint.ps1 -Command schema -WikiRoot docs/wiki`.

Stop condition: schema prints `schema_ops = 5 of 5`, `schema_ops_no_verdict = 0`,
`governance_drift = 0`, `VERDICT fail=0`.

## Land the two commits clean

Units:
1. Run `git status --short` and stage only `tools/wiki-index/wiki-index.ps1`,
   `memory-bank/tools/wiki-index/tool.md`, and `docs/wiki/index.md` for commit A.
2. Commit A: `wiki: add wiki-index check and regenerate index.md`.
3. Run `wiki-census -Command pending -WikiRoot docs/wiki`; expect `pending = 0 of 0`.
4. Run `git status --short` and stage only `docs/skills/wiki-ingest.md` and
   `docs/wiki/schema.md` for commit B.
5. Commit B: `wiki: record catalog in wiki ingest schema`.
6. Re-run `wiki-census -Command pending -WikiRoot docs/wiki`; expect `pending = 0 of 0`.
7. Run the wiki gate block and accept only the standing `refs broken = 1`.

Stop condition: `git log --oneline -2` shows both commits and the census runs print
`pending = 0 of 0`.

## Restate the op in WikiDesign

Deferred. `docs/WikiDesign.md` is design prose, not the source the gates read. Reopen only if a
reader mistakes `index.md` for a maintained page or asks why the drain gained a step.
