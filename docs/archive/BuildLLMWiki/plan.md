---
Type: Plan
Title: "Build the LLM wiki for this repo (Karpathy pattern for code)"
Parent: ""
Level: 0
Items:
  - Title: Layer scaffolding and folders
    Status: Refined
    Content: ./LayerScaffoldingAndFolders/plan.md
  - Title: Layer instruments
    Status: Refined
    Content: ./LayerInstruments/plan.md
  - Title: Layer governance
    Status: Refined
    Content: ./LayerGovernance/plan.md
  - Title: Layer pages
    Status: Refined
    Content: ./LayerPages/plan.md
  - Title: Layer ops
    Status: Refined
    Content: ./LayerOps/plan.md
  - Title: Layer gate proof
    Status: Refined
    Content: ./LayerGateProof/plan.md
  - Title: Records and commit
    Status: Atomic
    Content:
---

# Build the LLM wiki for this repo (Karpathy pattern for code)

Slug: `BuildLLMWiki` - from the request title "Build the LLM wiki for this repo (Karpathy pattern
for code)" (drop-words `the`/`a`/`an`/`of` removed, PascalCased, parentheses and other
non-`[A-Za-z0-9]` stripped, 12 chars, inside the 24-char cap). No collision with the sibling trees
`SourceCommentShadowTree`, `User32ErrorHandlerPass`, `User32ReturnCheckPass`. Uniqueness is stored
here, not recomputed.

## Goal

Land the three layers of `docs/WikiDesign.md` - a queue at `docs/raw/` that ingest consumes to
empty, prose at `docs/wiki/` whose every claim carries a pointer into the versioned tree plus the
command that would falsify it, and governance at `docs/wiki/schema.md` - so that maintaining the
wiki is a scheduled set of instruments rather than an LLM deciding what is fresh.

## Resume (state slot - the only mutable section of this plan)

Cache, not record: the front matter owns structure, the disk owns truth, this slot only makes the
resume cheap. Read the front matter and this slot, re-derive from disk, rewrite this slot if they
disagree. Rotate in place - never append below this block.

- `Cursor: plan.md -> re-read 18 Nov 2026, tree TERMINAL` - all 7 items terminal (6 `Refined` with
  child plans on disk, 1 `Atomic`); measured by `tmp/_tmp_llmwiki_tree_verify.ps1` -> `.txt`
  (recreate by slug): `plans=42 levels=0,1,2,3,4,5 atomic=176 refined=41 planning=0 VERDICT fail=0`
  (re-measured 18 Nov 2026 after `ConceptPages` was cut).
  The counts moved on 16 Nov 2026 when root item 4's two lib-page plans were re-cut at one page per
  leaf, and on 18 Nov 2026 when `GotchaPages` (6 leaves) and `InvariantPages` (6 leaves) were charted
  as children of `ClaimCarryingPages`, and again the same day when `DecisionPages` was re-cut: 55
  one-page leaves cannot fit the 7-item breadth cap at one nesting level, so it now holds probe,
  purpose and close plus four `Refined` groups (L4) over eight leaf-plans (L5) - levels reach 5.
  See that plan's slot; no other item changed shape.
- `Implementation: root items 1-3 CLOSED` - item 1 `Layer scaffolding and folders` on 14 Nov 2026
  (4 of 4 `Atomic`, `tmp/_tmp_llmwiki_scaffold_verify.ps1` -> `VERDICT fail=0`), item 2
  `Layer instruments` on 15 Nov 2026 (its 5 items -> 12 `Atomic` leaves, 6 instruments, **522 checks
  0 failed** across the six self-tests + registration `137 of 137`; details in that plan's Resume),
  item 3 `Layer governance` on 15 Nov 2026 (2 of 2 `Atomic`: `docs/wiki/schema.md`, 159 lines, judged
  by the instrument it governs - `source = schema.md`, `fields = 13 of 13`,
  `schema_sections = 7 of 7`, `schema_ops = 5 of 5`, `schema_ops_no_verdict = 0`,
  `illegal_fields = 0`).
   **Root item 4 `Layer pages` CLOSED 18 Nov 2026** (see its slot): item 1
   (`Reconcile the target list`) via
   `tmp/_tmp_llmwiki_target_reconcile.ps1` prints `targets = 90 of 90`, `indexed = 88 of 88`,
   `unindexed = 2 [qor_mime qor_yaml]`, `orphan_rows = 0`, so the two targets with no `repo_map.md`
   row are named; items 2-4 landed 88 module pages, `coverage = 88 of 90`; item 5 landed
   `GotchaPages` 3 + `InvariantPages` 4 + `DecisionPages` **56** (judge-schema question settled by
   **D56** written into `docs/agent/decisions/` 18 Nov 2026 - sentinel `Judges: none - <reason>`) +
   `ConceptPages` purpose + 5, on the same day. Item-4 close line: census `files = 165`
   `pages = 156` `purpose_files = 7`, `index_rows = 156 of 156 / groups = 5`, anchors
   `516 of 516 / dead = 0`, schema `illegal_fields = 0`, freshness `stale = 0 / undated = 148`,
   all `VERDICT fail=0` (reports `tmp/_tmp_{dp,cp}_*.txt`). **Root item 5 `Layer ops` CLOSED
   18 Nov 2026** (`docs/skills/wiki-ingest.md` 8 steps under 30 lines, carry/print/delete in
    order; `docs/skills/wiki-file.md` 6 steps, queue-not-invent step names `docs/raw/pending.md`;
    corrections: no `VerifiedAgainst:` field, and `wiki-file` checks freshness via
    `wiki-ref-check anchors` per the plan's own lock). **Root item 6 `Layer gate proof` CLOSED
    18 Nov 2026** (see its slot): 6 fixture reds over `tmp/wiki_fixture/`, each `VERDICT fail = 1`
    (`tmp/_tmp_gp_*.txt`), and the same six gates over the real wiki root all `VERDICT fail = 0`
    (`tmp/_tmp_gp_real2_*.txt`, provenance `violations = 0` after fixing 3 `dumpbin` abspath judge
    lines on D39/D48/D49 to `$env:SystemRoot\...` spelling, the cppcoro URL in `modules/qor_task.md`,
    and the `token:` phrase in `modules/qor_signals.md` - all five in-run, none excepted).
    **Cursor is now root item 7, `Records and commit`** - the tree's last `Atomic` item: drain stays
    `pending = 0`, six tool-doc entries in `memory-bank/tools.md`, the memory-bank rotation, retire
    this tree to `docs/archive/`, one `wiki:` commit that also carries
    `docs/agent/decisions/D56.md`; only afterwards fill the D56 page's NO-COMMIT anchor with the
    committed sha and re-run its stop triple. The known `wiki-ref-check refs broken = 1`
    (`modules/qor_netserver.md -> maybe_unused`) is reported, not gate-blocking.
  The six self-tests re-run after the governance file landed: **544 checks, 0 failed**
  (`tmp/_tmp_llmwiki_all_verify.ps1`). The `index` verifier needed two snapshot counts replaced by a
  disk read - `docs/wiki/` legitimately gained `schema.md`, and a hard-coded `nav_files = 1` went
  stale while the instrument stayed right.
  Item 1 also corrected one of this tree's own measurements: `.gitignore` holds 41 pattern lines, not
  2 - and 0 of them match `docs/`, which is the half the argument needed.
- `Blocking precondition: no wiki page is written before the instruments that judge it exist.`
  **Satisfied 15 Nov 2026** by root item 2 - the six instruments run and print verdicts over the real
  tree today, and over 0 pages they say so (`pages = 0 of 5`, `vacuity:` lines) rather than passing.
  The order still binds the rest of the tree: governance (3) before pages (4) before ops (5), and item
  6 (`Layer gate proof`) is the first place a red case is legal, only ever inside `tmp/` fixtures.
- `Measured seeding, 14 Nov 2026`: 91 `set(target qor_` declarations / 90 distinct targets across
  208 `CMakeLists.txt` files; 88 `- qor_*` rows in `docs/agent/repo_map.md`'s Library index while
  its header claims 33; 3 files in `memory-bank/gotchas/`; 55 files in `docs/agent/decisions/`;
  5 `- [` rows in `docs/learning/RCU/RESOURCES.md`; `docs/agent/verification.md` is **529** lines
  / **129** `- ` bullets / **27** headings, of which **4** rule sections (`## Build commands
  (incremental)` L6, `## Lint` L10, `## Test` L13, the green rule L15) sit above the `## Results`
  log that starts at L17.

## Context

Inherited at creation - re-read rather than trusted, the tree has moved since:

- **Parent**: this is the root.
- **The design**: `docs/WikiDesign.md` §1-16. Settled decisions at §13 (five questions answered by
  the user); consequences rolled at §13.1. The design is the record of why, not a plan of work.
- **The five settled decisions** this plan must honour, in one line each: (1) all three layers sit
  under `docs/`; (2) the unit of wiki work is a **build target**, never a file; (3) every layer is
  **tracked**; (4) `docs/learning/` and `docs/wiki/` are separate catalogs that never sync, joined
  only by a shared anchor id, and the learning track supplies anchors to the wiki on request;
  (5) `ingest-commit` is **on**, so the queue drain is a standing expectation, not a hope.
- **The consequences** §13.1 rolls out: `docs/wiki/schema.md` is **one file** (the `purpose.md` rule
  would otherwise demand a folder holding one member); `git diff -> targets -> pages` is a
  dependency; the qmd retrieval rung **retires before it is built**; wiki maintenance **makes
  commits**, so repo commit rules apply to wiki work; the hook that enqueues **must exclude
  `docs/wiki/` and `docs/raw/`** or the ingest's own commit re-enqueues forever; two catalogs that
  never sync; `pending=0` and `reconcile fail=0` are independent and only the pair is a verdict.
- **Repo-wide** (`docs/agent/skill.md`, `docs/agent/verification.md`): the **Rule of 7** - anything
  decomposed into more than 7 parts is untrustworthy; one build+test per increment; commit only at
  logical milestones; `docs/agent/` is the machine layer, `docs/` is the human layer.
- **Sibling titles** in `docs/future/`: `SourceCommentShadowTree`, `User32ErrorHandlerPass`,
  `User32ReturnCheckPass`. Their trees are the worked examples of the format and gate this plan
  follows; `User32ReturnCheckPass` in particular is the cautionary lesson - "one action on one
  named artifact" is not enough to make an item `Atomic`.
- **Instruments already installed**: `tools/doc-fetch/doc-fetch.ps1` (one tool exists in the tracked
  `tools/` tree today); `.ua/` knowledge bank for structural questions; Universal Ctags per lib.

## Decisions

Choices this plan locks; the why lives in `docs/WikiDesign.md`, linked not restated:

- **Three layers, one direction** - §2, §4.4. `docs/raw/` is a queue, not a stock: entries are
  **consumed and deleted** at ingest, `docs/raw/` empty is the invariant, and references are never
  archived because the page that ingested them already holds their anchors.
- **Target granularity** - §13 Q2. The unit of wiki work is a build target; a page's `Judges:` are
  that target's build/test commands.
- **Tracked, and wiki maintenance commits** - §13 Q3 plus §8 protocol 1. One commit per drain,
  `wiki:`-prefixed, carrying its verdict line.
- **Two catalogs** - §13 Q4. `docs/learning/` keeps its own index and vocabulary; the wiki never
  syncs it.
- **Instruments before pages** - §11's "nothing about freshness is ever an LLM's judgement". An
  instrument that has never seen a red case is untested, so the red case is constructed in `tmp/`
  fixtures - never by damaging a tracked page.
- **`wiki-invariant-check` is not built** - §11. An invariant page's `Judges:` are already a build,
  a test binary or a `dumpbin` call; a wrapper would re-implement the compiler.
- **First page pass is the 17 core libs** (14 `src/qor` + 3 `src/platform`), then `framework`
  (**26**) and `components` (**45**) - `docs/WikiDesign.md` §13.1's measured page count. The qmd
  rung stays unbuilt throughout.
- **No D-entry is written by this plan** - §13.1. D-entries are written when a decision lands in
  code. Here: the queue, the schema, and the six tools.


## Layer scaffolding and folders

Why: the three folders and their `purpose.md` files are what make the layers addressable, and the
`.gitignore` line is what makes them tracked. Both must be right before any instrument is written,
because every instrument path resolves through them.

Four `Atomic` items: `docs/raw/pending.md` with its header; `purpose.md` for `docs/raw/`; the three
`docs/wiki/` folders with their `purpose.md` files; and the `.gitignore` line that keeps only
`/.ua` and `/tmp` ignored. Measured input: the current `.gitignore` holds exactly two patterns, and
neither folder exists.

## Layer instruments

Why: these six scripts are the whole of this design's labour, and the thing that makes them cheap is
that they read one shape and know nothing about prose. They are built before pages so that the
first page written is judged by something that already runs.

Three `Refined` items - census, cross-reference and index as one plan (they share the parse);
reconcile and lint as one plan (they share the page read); retire on its own - plus one `Atomic`
item that registers all six in `memory-bank/tools.md` once they pass their own self-test.

## Layer governance

Why: `docs/wiki/schema.md` is a file every op re-reads, so it is the only place the rules exist.
Writing it before the pages is what lets the pages be checked against it rather than against
prose in a design document.

Two `Atomic` items: the schema body (one file, its sections enumerated by id) and the op table
(the five ops, each with its drain order and its verdict line).

## Layer pages

Why: this is the bulk of the tree, and the bulk is bounded by a measured number - 90 distinct
targets, not "every file". The gate cut the collections rather than the verbs, which is why the
leaf count is larger than the design's 15-item table.

One `Atomic` item (the measured target list every leaf consumes, per the gate's input rule) plus
four `Refined` items: the seed pages for the 17 core libs, the `framework` lib pages, the
`components` lib pages, and the claim-carrying pages (gotchas, invariants, decisions, concepts).
**Each page-bearing leaf from the 16 Nov 2026 re-cut writes exactly one page** and names the target
it owns; the leaf budget in `docs/future/PLAN-FORMAT.md` counts units of work, and a module page is
several lookups, one write and one judge build - so a leaf of ten pages was ten leaves under one
title, which is what cost 16 Nov 2026 a compaction cycle at 2 of 10 pages. Groups are capped at 7
items because 7 is the breadth cap a plan may hold, not because 7 pages fit a session. (`SeedPages`
closed before the rule was measured, at 2 leaves of 10 cheap core libs; it is left as it closed.)

## Layer ops

Why: the two skills are the only prose an LLM ever runs, and §11 says they are short because the
instruments do the work. They are written after the pages exist so that they are written against
artifacts that are on disk rather than imagined.

Two `Atomic` items: `wiki-ingest` and `wiki-file`. Each is one file, body under 30 lines,
enumerating its steps by id.

## Layer gate proof

Why: the instruments must be proven to fire, not assumed to. An instrument that never sees a red
case may be a `pass=0` vacuity. Nothing in this item may damage a tracked page - the red cases are
constructed in `tmp/` fixtures.

Two `Atomic` items: build the fixtures, then run the gates and read the expected reds. The stop
conditions are the printed counts, and a green gate over a fixture is a failure of the design, not
of the run.

## Records and commit

Input verified: every earlier root item closed, the gate reds were read over the fixture, and the
seeded pages carry anchors.

Why: wiki maintenance makes commits (§13.1), so the repo's own rules apply - commit only at a
logical milestone, never per increment. This is the milestone.

One `Atomic` item: the records and the one commit. Its units are enumerated by id: `docs/raw/pending.md`
empty after the drain, the six tool-doc entries in `memory-bank/tools.md`, the memory-bank
`activeContext.md` rotation, the `docs/future/BuildLLMWiki` retirement to `docs/archive/`, then
`git commit` carrying the drain's verdict line. Stop condition: `git log -1` re-reads the recorded
hash and `wiki-census` re-reads `pending = 0 of 0`.


## Handoff

Measured at this write: 27 plans on disk; `tmp/_tmp_llmwiki_tree_verify.ps1` -> `.txt` reads
`plans=27 levels=0,1,2,3 atomic=107 refined=26 planning=0 VERDICT fail=0` - no `Planning` or
`Drafted` item anywhere, so the tree is terminal, and breadth, depth, `Content:` and `Parent:` all
re-read clean. `tmp/_tmp_llmwiki_page_cut.ps1` -> `.txt` holds the page-bearing plans to their
targets: `framework claimed = 26 of 26  owned_once = 26  landed = 2`,
`components claimed = 45 of 45  owned_once = 45  landed = 0`, `VERDICT fail=0`.

The order the executor must respect is **item order** - instruments before governance before pages
before ops before the gate proof - with two exceptions noted inside the plans themselves: the
`Register the six tools` item inside `Layer instruments` runs after its self-tests, and the
`Records and commit` item runs last.

| Pick | Deepest complete plan | What it lands |
|---|---|---|
| folders and tracking | `./LayerScaffoldingAndFolders/plan.md` (Level 1, 4 `Atomic`) | the three folders, their `purpose.md` files, the queue file's header, the `.gitignore` line |
| the six instruments | `./LayerInstruments/plan.md` (Level 1, 4 `Refined` + 1 `Atomic` → 12 `Atomic` leaves inside) | `wiki-census`, `wiki-ref-check`, `wiki-index`, `wiki-reconcile`, `wiki-lint`, `wiki-retire` - each self-tested before it is registered |
| governance | `./LayerGovernance/plan.md` (Level 1, 2 `Atomic`) | `docs/wiki/schema.md` - the one file every op re-reads |
| the pages | `./LayerPages/plan.md` (Level 1, 1 `Atomic` + 4 `Refined` → 83 `Atomic` leaves over 17 plans) | 17 core lib pages, 26 `framework`, 45 `components`, plus the claim-carrying pages - **one page per leaf**, proven by `tmp/_tmp_llmwiki_page_cut.ps1` |
| the two skills | `./LayerOps/plan.md` (Level 1, 2 `Atomic`) | `wiki-ingest`, `wiki-file` |
| proving the gates fire | `./LayerGateProof/plan.md` (Level 1, 2 `Atomic`) | fixtures in `tmp/`, then the reds each gate must print |
| the records | root item `Records and commit` (`Atomic`) | the tool-doc entries, the memory-bank rotation, the tree's retirement, one `wiki:` commit |

Start at `./LayerScaffoldingAndFolders/plan.md` and walk the item order; `/implement-spec` on §14's
numbered table if the design is to stay the spec, `/to-tickets` to spread the page passes across
sessions. **The wiki is not "up to date" until `pending = 0 of 0` and `reconcile fail=0` both print**
- neither implies the other, and an instrument that prints `0 of 0` proves only that it parsed.

