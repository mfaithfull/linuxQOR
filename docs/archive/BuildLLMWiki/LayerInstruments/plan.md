---
Type: Plan
Title: "Layer instruments"
Parent: "../plan.md"
Level: 1
Items:
  - Title: Census instrument
    Status: Refined
    Content: ./CensusInstrument/plan.md
  - Title: Ref and index instruments
    Status: Refined
    Content: ./RefIndexInstruments/plan.md
  - Title: Reconcile and lint instruments
    Status: Refined
    Content: ./ReconcileLintInstruments/plan.md
  - Title: Retire instrument
    Status: Refined
    Content: ./RetireInstrument/plan.md
  - Title: Register the six tools
    Status: Atomic
    Content:
---

# Layer instruments

## Goal

Write the six scripts that make wiki maintenance mechanical, so that the first page ever written
is judged by something that already runs and has already printed a verdict line.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: closed` - 5 of 5 items done 15 Nov 2026, so this plan is **terminal** (4 `Refined` whose
  subtrees are closed, 1 `Atomic`). The six instruments are on disk and each ran its own self-test
  against the real tree plus a `tmp/` fixture, in the item order that is the build order:
  `wiki-census` 27/27, `wiki-ref-check` 75/75, `wiki-index` 96/96, `wiki-lint` 105/105,
  `wiki-reconcile` 110/110, `wiki-retire` 109/109 - **522 checks, 0 failed**, re-run in sequence by
  `tmp/_tmp_llmwiki_all_verify.ps1` (sequentially on purpose: each self-test asserts it created no
  new file, so two at once could make a clean tool look dirty). Registration re-reads by
  `tmp/_tmp_llmwiki_register_verify.ps1` -> **137 of 137, failed = 0**, and its own red power was
  checked by `tmp/_tmp_llmwiki_register_red.ps1` (3/3: a missing shape entry, a blanked `Usage:` and
  a `LastRun:` field each turn it red) - a verifier that cannot fail proves nothing.
  The shared shape is `tools/wiki-census/wiki-shape.ps1`, now **`wiki-shape-5`**, dot-sourced by all
  six: one notion of a page, an anchor, a queue entry, a judge and a stale stamp. Two facts the next
  root item needs: `docs/wiki/schema.md` **still does not exist**, and `wiki-lint` says so on every
  run (`source = builtin`, `schema_absent = 1`) rather than assuming a vocabulary - root item 3
  (`Layer governance`) writing that file is what makes `illegal_fields` mean governance and not
  `wiki-shape`'s builtin guess. And the fixtures `Layer gate proof` will reuse are already there:
  `tmp/_tmp_{refcheck,index,lint,reconcile,retire}_fixture/`, each **reset at the start** of its
  self-test rather than deleted at the end, so the trees survive for the gate proof to point at. They
  are under `tmp/`, which `.gitignore:41` covers and 0 of its 41 patterns match `docs/` - verified
  again after the full sweep: `docs/wiki/` still holds **only** the scaffolding - the 4 `purpose.md`
  files (`.`/`archive`/`invariants`/`modules`) plus the generated `index.md` with **0** rows - so no
  fixture page and nothing archived leaked into the tracked tree; and `pending.md` still holds
  **0** entries after `refresh`'s self-test, which appends to a fixture queue and never the real one.

## Context

Inherited at creation - re-read rather than trusted, the tree has moved since:

- **Parent goal**: the three layers of `docs/WikiDesign.md`, so that maintenance is instruments.
- **Parent locked decisions**: the unit of wiki work is a **build target**; every layer is tracked;
  `docs/wiki/schema.md` is one file; `docs/raw/` is a queue consumed to empty.
- **Sibling plan titles**: `Layer scaffolding and folders` (runs first - it creates the folders and
  the queue file these instruments read), `Layer governance`, `Layer pages`, `Layer ops`,
  `Layer gate proof`.
- **Already installed**: `tools/doc-fetch/doc-fetch.ps1` is the only tool in the tracked `tools/`
  tree today; `docs/agent/TOOL-FORMAT.md` defines the tool shape and the contract rule (required
  args, resolved paths the run writes, no default encoding a task).
- **Measured**: `docs/agent/verification.md` is **529** lines with **129** `- ` bullets and
  **27** headings, of which **4** rule sections (`## Build commands (incremental)` L6, `## Lint`
  L10, `## Test` L13, the green rule L15) sit above the `## Results` log at L17 - those four are
  the verification ladder `wiki-reconcile` reads as the vocabulary of judge commands; the Results
  log is evidence, not a rule.

## Decisions

- **Six instruments, not seven** - `docs/WikiDesign.md` §11. `wiki-invariant-check` is not built:
  an invariant page's `Judges:` are already a build, a test binary or a `dumpbin` call.
- **Every instrument reads one shape** - §5. The parse of front matter and fields is shared, and no
  instrument opens prose. That is what keeps them short and what makes them independent of the
  wiki's content.
- **Verdict lines, not exit codes** - §9. Every instrument prints its own verdict line; the pair
  `pending=0` and `reconcile fail=0` is the verdict, and neither implies the other.

## Census instrument

`wiki-census` is the first instrument because the other five read its shape. See
[`./CensusInstrument/plan.md`](./CensusInstrument/plan.md) - 2 `Atomic`: write the script
(4 subcommands, enumerated by id) and run its self-test against an empty wiki.

## Ref and index instruments

`wiki-ref-check` and `wiki-index` share the page read with the census, so they are one plan. See
[`./RefIndexInstruments/plan.md`](./RefIndexInstruments/plan.md) - 4 `Atomic`: write each script
(subcommands enumerated by id), self-test each. The self-tests are the honest part: `0 of 0`
proves only that the instrument parsed, and each must print that distinction explicitly.

## Reconcile and lint instruments

`wiki-reconcile` and `wiki-lint` are the pair that make freshness mechanical and governance
enforceable. See [`./ReconcileLintInstruments/plan.md`](./ReconcileLintInstruments/plan.md) -
4 `Atomic`: write each script (subcommands enumerated by id), self-test each. `wiki-reconcile` is
the largest of the six and is the one whose input is external: it reads `docs/agent/verification.md`
(its **4** rule sections) for the judge vocabulary, so its self-test must print the vocabulary
count it read.

## Retire instrument

`wiki-retire` is the one op that moves content, which is why it is separate: the archive discipline
(pages only, never references) is its own decision rather than a line inside another script. See
[`./RetireInstrument/plan.md`](./RetireInstrument/plan.md) - 2 `Atomic`.

## Register the six tools

Blocking precondition: all six scripts have run their self-test and printed their verdict line.

One edit to `memory-bank/tools.md`: six `index:` entries, one per instrument, each with `UseWhen`
phrased **generally** (the retrieval key - never per task), `Scope`, and `File:` relative to
`memory-bank/`; plus one `memory-bank/tools/<slug>/tool.md` per instrument with the required
`What it does` / `Contract` / `Provenance` sections. Units enumerated by id: `wiki-census`,
`wiki-ref-check`, `wiki-index`, `wiki-reconcile`, `wiki-lint`, `wiki-retire` - 6 index entries,
6 tool docs.

Input verified: the six scripts exist and their self-tests printed, from the four `Refined` items
above.

Stop condition: `Select-String -Path memory-bank/tools.md -Pattern 'File: "tools/wiki-'` re-reads
**6 of 6**, and each tool doc re-reads with `Type: Tool` plus a `Scripts:` block whose `File:`,
`Usage:` and `Output:` are all non-empty. The `LastRun` field must **not** appear - it is mutable
state in a tracked file (§11's warning), and its presence is a lint failure, not a style choice.
