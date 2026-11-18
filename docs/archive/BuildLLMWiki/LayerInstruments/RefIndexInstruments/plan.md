---
Type: Plan
Title: "Ref and index instruments"
Parent: "../plan.md"
Level: 2
Items:
  - Title: Write wiki-ref-check
    Status: Atomic
    Content:
  - Title: Self-test wiki-ref-check
    Status: Atomic
    Content:
  - Title: Write wiki-index
    Status: Atomic
    Content:
  - Title: Self-test wiki-index
    Status: Atomic
    Content:
---

# Ref and index instruments

## Goal

Two scripts that own the pointer layer: every pointer resolves, and the index is generated rather
than maintained.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: closed` - 4 of 4 `Atomic` done 14 Nov 2026. `tools/wiki-ref-check/wiki-ref-check.ps1`
  (4 subcommands, `-Help` prints 4 of 4) self-tested by `tmp/_tmp_llmwiki_refcheck_verify.ps1` ->
  **75 of 75, failed = 0**; `tools/wiki-index/wiki-index.ps1` (3 subcommands, `-Help` prints 3 of 3)
  self-tested by `tmp/_tmp_llmwiki_index_verify.ps1` -> **96 of 96, failed = 0**, including the
  agreement this plan asks for: `index_rows` equals the page count `wiki-census` reads from the same
  disk state. The `Layer gate proof` fixture leg is **recorded as skipped** (`tmp/wiki_fixture` does
  not exist yet) and replaced by each self-test's own `tmp/` fixture, so no subcommand is registered
  on a green-only test. Both scripts dot-source `tools/wiki-census/wiki-shape.ps1` - one shape, and
  the ref-check self-test found and fixed the false green that shape used to carry (a
  `[Parameter(Mandatory)]` arg let a run continue past a bind error to `VERDICT fail=0`).

## Context

Inherited at creation - re-read rather than trusted, the tree has moved since:

- **Parent goal**: the six scripts that make wiki maintenance mechanical.
- **Parent locked decisions**: one shared shape, no instrument opens prose; verdict lines not exit
  codes; `0 of 0` proves only that a parse happened.
- **Sibling plan titles**: `Census instrument` (first - its parse this plan reuses),
  `Reconcile and lint instruments`, `Retire instrument`, `Register the six tools`.
- **Inputs on disk**: the queue file and the five folders; `wiki-census` already written and
  self-tested, so its parse is a reusable function rather than a re-invention.

## Decisions

- **The index is generated, never edited** - §6. An LLM editing `index.md` directly is an error
  condition, so there is no hand-written index to keep in step.
- **`wiki-ref-check` is not `wiki-reconcile`** - §5. One answers "every pointer resolves", the other
  "what is compiled still matches the tree". They share the parse and never share a verdict.

## Write wiki-ref-check

Input verified: `wiki-census` exists and printed the page and target counts it feeds.

One write: `tools/wiki-ref-check/wiki-ref-check.ps1`. Subcommands, enumerated by id:
`anchors` - every `path:`/`pin:`/`verified:` in every page resolves against the tree, printing
`anchors = N of N` and `dead = N`;
`refs` - every `[[target]]` and `See also` wikilink resolves to an existing page, printing
`links = N of N` and `broken = N`;
`orphans` - every anchor in `docs/raw/` referenced by no page, printing `orphans = N of N`
(the leak that makes ingest's `carry` step verifiable);
`drift` - anchors whose `pin:` is older than the tree's current pin for that path.

Contract: required args are the wiki root, the raw root and the tree root; reports to `tmp/`.

Stop condition: `-Help` prints **4 of 4** subcommands and touches no file.

## Self-test wiki-ref-check

One run over an empty wiki, enumerated by id: `anchors`, `refs`, `orphans`, `drift`. The expected
print is `0 of 0` on all four, and the script must additionally print `vacuity: pages=0` - §9's
guard, because `anchors = 0 of 0` looks identical to a clean sweep over a full wiki.

Input verified: the script exists and printed 4 of 4 from the item above.

Stop condition: the run prints `vacuity: pages=0` and `VERDICT fail=0`; the same run pointed at the
fixture page written by `Layer gate proof`'s first item prints `dead >= 1` and `VERDICT fail=1`.
If the fixture does not exist yet, this item's second leg is skipped and recorded as such - it is
not silently reported as passing.

## Write wiki-index

One write: `tools/wiki-index/wiki-index.ps1`. Subcommands, enumerated by id:
`build` - regenerate `docs/wiki/index.md` from page front matter alone (topic, id, one line),
printing `pages = N` and `index_rows = N of N`;
`lookup` - return the page for a target or a topic, printing the hit count;
`diff` - print pages whose content changed since a ref passed as an argument, printing `moved = N`.

Contract: required args are the wiki root and, for `diff`, the ref; `build` writes
`docs/wiki/index.md` and prints the path it resolved.

Input verified: the parse and page listing come from `wiki-census`, already written and self-tested.

Stop condition: `-Help` prints **3 of 3** subcommands and touches no file.

## Self-test wiki-index

One run over an empty wiki, enumerated by id: `build`, `lookup`, `diff`. Expected: `build` writes
an index whose body is empty and prints `pages = 0` with `index_rows = 0 of 0`; `lookup` prints
`hits = 0`; `diff` against the repo's current head prints `moved = 0`. Each zero must be printed
with its `of N` denominator so the empty read stays visible.

Input verified: the script exists and printed 3 of 3 from the item above.

Stop condition: the run prints `VERDICT fail=0` and `index_rows` equals the page count the census
reports from the same disk state - the two instruments must agree, and a disagreement is a bug in
one of them, not a wiki state.
