---
Type: Plan
Title: "Layer pages"
Parent: "../plan.md"
Level: 1
Items:
  - Title: Reconcile the target list
    Status: Atomic
    Content:
  - Title: Seed the core lib pages
    Status: Refined
    Content: ./SeedPages/plan.md
  - Title: Framework lib pages
    Status: Refined
    Content: ./FrameworkLibPages/plan.md
  - Title: Components lib pages
    Status: Refined
    Content: ./ComponentsLibPages/plan.md
  - Title: Claim-carrying pages
    Status: Refined
    Content: ./ClaimCarryingPages/plan.md
---

# Layer pages

## Goal

Write the pages that populate the wiki, bounded by a measured number of units - 90 distinct
build targets - so that "the wiki is behind" becomes a named set of targets rather than a guess.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `PASS CLOSED 18 Nov 2026 - all five items closed. Item 5 `Claim-carrying pages` closed last:
  GotchaPages (3 pages) + InvariantPages (4) + DecisionPages (**56 pages** landed 18 Nov 2026,
  close census `  decisions = 56`, D56 page carries NO anchor - NO-COMMIT, fill after the root
  Records and commit) + ConceptPages (purpose + **5 concept pages**, `  concepts = 5`); the
  parent's close line: census `files = 165` `pages = 156` `strays = 0` `purpose_files = 7`,
  `wiki-index build` `index_rows = 156 of 156 / unindexed = 0 / groups = 5`, anchors
  `516 of 516 / dead = 0`, schema `illegal_fields = 0`, freshness `stale = 0 / undated = 148`,
  each `VERDICT fail=0` (reports `tmp/_tmp_cp_*.txt`). Carried findings unchanged: refs
  `broken = 1` (`modules/qor_netserver.md:73`), provenance `violations = 2`, `coverage = 88 of
  90` (`qor_mime` / `qor_yaml` uncatalogued).` Item 4 `Components lib pages` **CLOSED 18 Nov 2026** - all
  45 component pages landed across 7 groups; the region close line printed `coverage = 88 of 90`,
  `uncovered_targets = 2` (`qor_mime`, `qor_yaml` - the unindexed F10 pair from item 1, the queue's to
  resolve), `illegal_fields = 0`, `dead = 0` (`anchors = 454 of 454`), each `VERDICT fail=0`. With
  items 1-4 done the whole build-target page pass is complete (88 of 90 targets catalogued); only
  `ClaimCarryingPages` remains in this plan, and it opens on `coverage = 88 of 90`. Inside item 4,
  `ComponentsPagesGroup3` **CLOSED 18 Nov 2026**:
  all 7 pages landed (`qor_generatorsource`, `qor_randomsource`, `qor_stringsource`,
  `qor_serialize`, `qor_generator`, `qor_netclient`, `qor_httpclient`); close-line stop
  pair printed `coverage = 64 of 90`, `uncovered_targets = 26`, `illegal_fields = 0`,
  `dead = 0` (`anchors = 310 of 310`), each `VERDICT fail=0`; `wiki-lint provenance`
  unchanged at `violations = 2`. `qor_httpclient` is the first page whose `Judges`
  command fails - the target is indexed but never configured (`MSB1009`), recorded
  on the page. `ComponentsPagesGroup2`
  **CLOSED 18 Nov 2026**:
  all 7 pages landed (`qor_unicodetextfilter`, `qor_datasink`, `qor_deserializersink`,
  `qor_parsersink`, `qor_stringsink`, `qor_aggregatesource`, `qor_datasource`); close-line stop
  pair printed `coverage = 57 of 90`, `uncovered_targets = 33`, `illegal_fields = 0`
  (`pages = 57 of 63`, `strays = 0`), `dead = 0` (`anchors = 268 of 268`), each `VERDICT fail=0`;
  `wiki-lint provenance` unchanged at `violations = 2` (the same pre-existing pair).
  `ComponentsPagesGroup1` **CLOSED 18 Nov 2026**:
  all 7 pages landed (`qor_jsondata`, `qor_ttf`, `qor_parser`, `qor_base64decodefilter`,
  `qor_base64encodefilter`, `qor_jpegdecodefilter`, `qor_linereader`); close-line stop pair
  printed `coverage = 50 of 90`, `uncovered_targets = 40`, `illegal_fields = 0`, `dead = 0`
  (`anchors = 229 of 229`), each `VERDICT fail=0`.
  Item 3 `Framework lib pages` **CLOSED 17 Nov 2026**:
  all 26 framework pages landed in 4 groups, and leaf 26 printed the region close line exactly -
  `coverage = 43 of 90`, `uncovered_targets = 47`, `dangling_module_pages = 0`,
  `illegal_fields = 0` (`pages = 43 of 49`, `strays = 0`), `dead = 0` (`anchors = 190 of 190`),
  each `VERDICT fail=0`; `wiki-lint provenance` sits at `violations = 2`, both pre-existing
  (`qor_task.md:45` github URL, `qor_signals.md:52` `token:` false positive). See
  [`./FrameworkLibPages/plan.md`](./FrameworkLibPages/plan.md) for the page contract and the leaf
  lessons; they bind `ComponentsLibPages` too.
  Item 1 `Reconcile the target list` **CLOSED 15 Nov 2026**
  by `tmp/_tmp_llmwiki_target_reconcile.ps1` -> `tmp/_tmp_lp_targets.txt`:
  `targets = 90 of 90`, `index_rows = 88 of 88`, `indexed = 88 of 88`, `orphan_rows = 0`,
  `unindexed = 2 [qor_mime qor_yaml]`, `VERDICT fail=0` (census `targets` and `coverage` both
  `fail=0`; `declared_twice = [qor_http]` is one declaration repeated in two CMakeLists files).
  **The two names above are the F10 pair the plan's decision promised to name**: 90 targets against
  88 index rows, so `qor_mime` and `qor_yaml` are wiki units with no `repo_map.md` row. A leaf that
  pages them has no index row to cite, so it cites `src/.../CMakeLists.txt` and says so in the page.
- Item 2 `Seed the core lib pages` **CLOSED 15 Nov 2026** - its two slices are both closed; the
  17 core pages exist and print `pages = 17`, `illegal_fields = 0`, `anchors = 40 of 40` with
  `dead = 0`, `coverage = 17 of 90`, `uncovered_targets = 73`. See its own plan's slot for the shape
  facts (no `VerifiedAgainst:` field; judges run against `cmakebuild`, not `build`).
- `Remaining` - none; items 1-5 closed. The 90-target build-target page pass is complete at 88
   pages (`qor_mime` / `qor_yaml` unindexed pair uncatalogued), and the claim-carrying layer holds
   3 + 4 + 56 + 5 pages with purpose pages in all six vocabulary folders.
- Item 5's `Gotcha pages` was refined 18 Nov 2026 into 6 `Atomic` leaves (the gate rows it failed:
  self-discovered inputs, mixed kinds past 1:1, a stop line `wiki-census pages` never prints) and
  **CLOSED the same day**: purpose + 3 pages + index rebuild, `docs/wiki/index.md` regenerated at
  103 lines over 91 pages. Two findings it carried out, both **pre-existing and recorded, not
  fixed**: `wiki-ref-check refs` `broken = 1` (`modules/qor_netserver.md:73`, `[[maybe_unused]]` -
  a C++ attribute in wikilink form; 497/498 before the pass, 501/502 after), and `wiki-lint
  freshness` prints all 91 pages `undated` (`evidenced = 0`, informational by the tool's own
  wording; the rule-as-enforced now sits in `docs/wiki/gotchas/purpose.md`). Provenance unchanged
  at `violations = 2`.

## Context

Inherited at creation - re-read rather than trusted, the tree has moved since:

- **Parent goal**: land the three layers so that maintenance is instruments, not an LLM deciding
  freshness.
- **Parent locked decisions**: the unit of wiki work is a **build target**, never a file; every
  layer is tracked; the qmd retrieval rung is not built - `index.md` alone is the index.
- **Sibling plan titles**: `Layer scaffolding and folders`, `Layer instruments`,
  `Layer governance` (all run before this one - pages are checked against the schema by
  instruments that must already run), `Layer ops`, `Layer gate proof`.
- **Measured at 14 Nov 2026**: 91 `set(target qor_` declarations / **90 distinct targets**;
  `docs/agent/repo_map.md` indexes **88** `- qor_*` rows; 14 core `src/qor` + 3 `src/platform`
  + 26 `framework` + 45 `components` = 88; 3 files in `memory-bank/gotchas/`; 55 files in
  `docs/agent/decisions/`; 5 `- [` rows in `docs/learning/RCU/RESOURCES.md`; 11 `- ` bullets in
  `docs/agent/verification.md`.

## Decisions

- **The target list is verified before any page is written** - the gate's input rule. A leaf that
  discovers its own targets spends its budget on discovery rather than on pages, so the list is
  produced and printed by the `Atomic` item above the slices.
- **Two targets have no `repo_map.md` row** - 90 distinct against 88 indexed. The difference is
  named by the reconciling item, not absorbed silently, because a page for a target nobody has
  indexed is the F10 shape (a field nothing checks).
- **Decision pages are one page per D-entry** - §14 item 10 read literally. If the wiki should carry
  decision **clusters** instead, that is a design change: 55 pages becomes a smaller set, and the
  `DecisionPages` plan is re-cut. Flagged rather than assumed.

## Reconcile the target list

Input verified: `wiki-census targets` printed the 91-line list on disk, and
`docs/agent/repo_map.md` holds 88 `- qor_*` rows.

Blocking precondition: `wiki-census` exists and its `targets` subcommand passed its self-test.

One run: `wiki-census targets` and `wiki-census coverage`, printing the distinct target count, the
indexed row count, and the **named** targets present in one and absent from the other. The output
is the authority every leaf below consumes.

Stop condition: the run prints `targets = 90 of 90`, `indexed = 88 of 88`, and `unindexed = 2` with
the two names listed - a count without names is a list no leaf can act on.

## Seed the core lib pages

See [`./SeedPages/plan.md`](./SeedPages/plan.md) - 2 `Atomic`, slices of at most 10 targets,
enumerated by id. The core libs (14 `src/qor` + 3 `src/platform`) are seeded first because §13 Q2's
measured page count says the first pass sits far under Karpathy's ~100-source threshold, which is
what keeps the qmd rung unbuilt.

## Framework lib pages

See [`./FrameworkLibPages/plan.md`](./FrameworkLibPages/plan.md) - 4 `Refined` groups holding 26
`Atomic` leaves of **one page each**. The unit of a leaf is a lookup-plus-write-plus-judge, not a
page: a leaf that promises ten pages is ten leaves wearing one title, and 16 Nov 2026 measured what
that costs (2 of 10 pages, 35 probe files, one compaction cycle).

## Components lib pages

See [`./ComponentsLibPages/plan.md`](./ComponentsLibPages/plan.md) - 7 `Refined` groups holding 45
`Atomic` leaves of one page each. `components` is the largest region and is cut densest-last, per
the sibling tree's lesson that a leaf's cost is set by its densest unit, not its TU count - which is
now enforced structurally: the densest lib in the tree is a leaf of one page, not one tenth of a
leaf of ten.

## Claim-carrying pages

See [`./ClaimCarryingPages/plan.md`](./ClaimCarryingPages/plan.md) - 2 `Atomic` + 2 `Refined`
(`./ClaimCarryingPages/GotchaPages/plan.md` holds 6 `Atomic`,
`./ClaimCarryingPages/DecisionPages/plan.md` holds 6). These pages are the ones that carry a
falsification handle rather than a description: gotchas judge against the command that reproduces
them, invariants against the verification ladder, decisions against the commit that made them,
concepts against nothing - and a page that judges nothing is deliberately marked so, which is what
keeps `docs/learning/` from being mistaken for claims.
