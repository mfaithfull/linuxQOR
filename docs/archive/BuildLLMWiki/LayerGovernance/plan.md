---
Type: Plan
Title: "Layer governance"
Parent: "../plan.md"
Level: 1
Items:
  - Title: Write the schema body
    Status: Atomic
    Content:
  - Title: Write the op table
    Status: Atomic
    Content:
---

# Layer governance

## Goal

One file at `docs/wiki/schema.md` that every op re-reads, so that the rules exist in exactly one
place and are enforced by an instrument rather than remembered by an LLM.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: CLOSED 15 Nov 2026` - both `Atomic` items landed, in item order. `docs/wiki/schema.md`
  exists (159 lines) and `wiki-lint schema` judges it:
  `schema = docs/wiki/schema.md  source = schema.md`, `fields = 13 of 13`,
  `schema_sections = 7 of 7 [fields pages links provenance archive budget ops]`,
  `schema_ops = 5 of 5 [ingest refresh wiki-reconcile wiki-file wiki-retire]`,
  `schema_ops_no_verdict = 0`, `illegal_fields = 0`, `VERDICT fail=0`
  (`tmp/_tmp_ls.md`, run from `tmp/_tmp_llmwiki_lint_verify.ps1` -> **126 checks, 0 failed**).
- `The governance file is now the authority` - `wiki-lint` prints `source = schema.md` and reads the
  13 fields and the 200-line budget out of the file; the builtin vocabulary of `wiki-shape` survives
  only as the fallback a run names with `source = builtin`, so a deleted schema is visible, not quiet.
- `Next` - root item 4 `Layer pages`: no page may be written before its target list is reconciled.

## Context

Inherited at creation - re-read rather than trusted, the tree has moved since:

- **Parent goal**: land the three layers so maintenance is instruments, not an LLM deciding
  freshness.
- **Parent locked decisions**: `docs/wiki/schema.md` is **one file**, not a folder; the unit of
  wiki work is a build target; every layer is tracked.
- **Sibling plan titles**: `Layer scaffolding and folders`, `Layer instruments` (both run before
  this one - the schema is validated by `wiki-lint schema`, which must exist first), `Layer pages`,
  `Layer ops`, `Layer gate proof`.
- **The design**: `docs/WikiDesign.md` §5 - the schema is the single source of page rules.

## Decisions

- **Schema is a file, not a folder** - §13.1. The repo rule "every new folder needs a `purpose.md`"
  would otherwise demand a folder with one member.
- **The schema states the budget** - §11. Page size limits live here so that `wiki-lint size` has a
  number to enforce rather than a prose instruction to interpret.

## Write the schema body

Input verified: `docs/WikiDesign.md` §2.3 states this file's content requirement; the instruments'
print lines exist from the instruments layer.

One write: `docs/wiki/schema.md`. Sections, enumerated by id: `fields` (every field and its type);
`pages` (what may live in which folder); `links` (the wikilink rule); `provenance` (no secrets, no
URLs, no absolute paths); `archive` (the retirement discipline - pages only, never references);
`budget` (page size limits, and the standing note that the qmd rung is not built because the page
count is small enough for `index.md` alone); `ops` (pointer to the op table below).

Stop condition: `wiki-lint schema` runs against the file and prints `fields = N of N` with
`illegal_fields = 0`, and the section re-scan prints **7 of 7** section headings.

## Write the op table

Input verified: the schema body exists from the item above; §12 of the design lists F1-F12.

One edit to the same file: append the `## Ops` table, one row per op, enumerated by id:
`ingest` (drain the queue, with its drain order `carry` then `print` then `delete` and its
verdict line `pending=0 consumed=N carried=N uncarried=0 VERDICT fail=0`); `refresh`;
`wiki-reconcile` (its verdict line `fail=0`); `wiki-file`; `wiki-retire`. Each row states its drain
order, its verdict line, and its failure modes by id (F9, F12 for ingest; F5 for retire).

Blocking precondition: the schema body exists, from the item above.

Stop condition: the table re-reads **5 rows of 5** ops, every row's verdict line non-empty, and
`wiki-lint schema` still prints `illegal_fields = 0` against the amended file.
