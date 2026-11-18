---
Type: Plan
Title: "Reconcile and lint instruments"
Parent: "../plan.md"
Level: 2
Items:
  - Title: Write wiki-reconcile
    Status: Atomic
    Content:
  - Title: Self-test wiki-reconcile
    Status: Atomic
    Content:
  - Title: Write wiki-lint
    Status: Atomic
    Content:
  - Title: Self-test wiki-lint
    Status: Atomic
    Content:
---

# Reconcile and lint instruments

## Goal

The two scripts that make freshness mechanical and governance enforceable - the pair that turns
"the wiki is behind" into a named set of units rather than a guess.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: closed` - 4 of 4 `Atomic` done 15 Nov 2026. `tools/wiki-reconcile/wiki-reconcile.ps1`
  (5 subcommands `anchors|verify|refresh|audit|history`, `-Help` prints 5 of 5) self-tested by
  `tmp/_tmp_llmwiki_reconcile_verify.ps1` -> **110 of 110, failed = 0**; `tools/wiki-lint/wiki-lint.ps1`
  (5 subcommands `schema|provenance|freshness|purpose|size`, `-Help` prints 5 of 5) self-tested by
  `tmp/_tmp_llmwiki_lint_verify.ps1` -> **105 of 105, failed = 0**. The two inputs this plan named
  both printed what they must: the judge vocabulary is **read**, not copied - every run prints
  `rule_sections = 4 of 4` from `docs/agent/verification.md` (the 135 `## Results` bullets are counted
  separately as evidence, and `dumpbin` is printed as the builtin allowance rather than hidden) - and
  `docs/wiki/schema.md` still does not exist, so `wiki-lint` prints `source = builtin` plus
  `schema_absent = 1` and measures every count against `wiki-shape`'s vocabulary. Root item 3 owns that
  file; when it lands, `-Schema` picks it up and `source` stops saying `builtin`.
  Two bugs the self-tests found and the scripts fixed, both of the false-green kind: a
  `[Parameter(Mandatory)][string]` given `''` throws a *non-terminating* bind error, so the script
  printed the error and ran on to `VERDICT fail=0` - requiredness is now tested before the call; and
  `Set-Content -NoNewline` on a queue file that already holds rows writes them **all on one line**, so
  the append "succeeded" and the next run parsed nothing - the shape exposes `Add-WikiQueueEntry`.

## Context

Inherited at creation - re-read rather than trusted, the tree has moved since:

- **Parent goal**: the six scripts that make wiki maintenance mechanical.
- **Parent locked decisions**: one shared shape, no instrument opens prose; the unit of wiki work is
  a build target and a page's `Judges:` are that target's build/test commands; verdict lines not
  exit codes.
- **Sibling plan titles**: `Census instrument`, `Ref and index instruments`, `Retire instrument`,
  `Register the six tools`.
- **Input measured**: `docs/agent/verification.md` is **529** lines with **129** `- ` bullets and
  **27** headings, of which **4** rule sections (`## Build commands (incremental)` L6, `## Lint`
  L10, `## Test` L13, the green rule at L15) sit above the `## Results` log that starts at L17.
  `wiki-reconcile` reads those four for its judge vocabulary so the wiki does not invent a second,
  divergent notion of green - and reads none of the 129 log bullets, because a log line is evidence
  about a past run, not a rule about the current tree.

## Decisions

- **The judge vocabulary is read, not copied** - §13 Q2 and §11. A page's `Judges:` are drawn from
  `docs/agent/verification.md`'s four rule sections; a judge command that appears in none of them
  is a lint failure, because two vocabularies would let a page be green while its library is red.
- **`refresh` and `audit` enqueue, they never enqueue silently** - §9. They emit queue entries into
  `docs/raw/pending.md`; they never edit a page. The LLM acts on the entry, not the script.

## Write wiki-reconcile

Input verified: `wiki-census targets` printed the 91-line list, and `wiki-ref-check` exists from the
sibling plan.

One write: `tools/wiki-reconcile/wiki-reconcile.ps1`. Subcommands, enumerated by id:
`anchors` - every anchor resolves, printing `anchors = N of N` and `dead = N`;
`verify` - for each page, run its `Judges:` commands and compare the newest judge timestamp against
`LastVerified:`, printing `stale = N` and a per-page line;
`refresh` - full sweep, appending one queue entry per dead anchor or stale judge, printing
`added = N` and `added_by = refresh`;
`audit` - every page plus every `docs/` prose doc whose anchors can be resolved, printing
`fail = N`;
`history` - churn per page since a ref, printing `moved = N`, for the "what happened to X" question.

Contract: required args are the wiki root, the raw root and the tree root; the judge vocabulary is
read from `docs/agent/verification.md` and its bullet count printed. Reports to `tmp/`; the queue
file it appends to is printed with its resolved path.

Stop condition: `-Help` prints **5 of 5** subcommands, touches no file, and the run prints the
judge-vocabulary count it read (`rule_sections = 4 of 4`).

## Self-test wiki-reconcile

One run over an empty wiki, enumerated by id: `anchors`, `verify`, `refresh`, `audit`, `history`.
Expected: all counts zero, and the script must print `vacuity: pages=0` - §9's guard, because a
`wiki-reconcile` with no pages and a `fail=0` is the shape that makes staleness disappear, and that
disappearance is the F1 failure mode this design exists to catch.

Input verified: the script exists and printed 5 of 5 plus the vocabulary count from the item above.

Stop condition: the run prints `pages = 0`, `vacuity: pages=0`, `VERDICT fail=0`, and **appends
nothing** to `docs/raw/pending.md` - the queue file's entry count re-reads unchanged, which is the
one thing that must be true of an audit that finds nothing.

## Write wiki-lint

Input verified: `docs/wiki/schema.md` exists from the governance layer, and `wiki-ref-check` exists.

One write: `tools/wiki-lint/wiki-lint.ps1`. Subcommands, enumerated by id:
`schema` - every page's fields are legal against `docs/wiki/schema.md`, printing
`pages = N` and `illegal_fields = N`;
`provenance` - no page contains a secret, a URL, or an absolute path, printing `violations = N`;
`freshness` - every claim page has a `LastVerified:` stamp that is not older than its newest
judge, printing `unstamped = N` and `stale = N`;
`purpose` - every folder under `docs/wiki/` has a `purpose.md` and every `docs/` folder has one
(repo rule), printing `folders = N` and `missing_purpose = N`;
`size` - every page is under the budget in `schema.md`, printing `over_budget = N`.

Contract: required arg is the wiki root; `schema` also reads `docs/wiki/schema.md` and prints its
path. Reports to `tmp/`.

Stop condition: `-Help` prints **5 of 5** subcommands and touches no file.

## Self-test wiki-lint

One run over the wiki as it stands, enumerated by id: `schema`, `provenance`, `freshness`,
`purpose`, `size`. Expected: `pages = 0`, every count zero **except** `purpose`, which must report
the folders that exist without a `purpose.md` - and there must be none, because
`Layer scaffolding and folders` wrote five. A `purpose` count of zero here is the one signal this
self-test can get out of an empty wiki.

Input verified: the script exists and printed 5 of 5 from the item above; the five folders and their
purpose files exist from `Layer scaffolding and folders`.

Stop condition: the run prints `VERDICT fail=0` with `missing_purpose = 0 of 5` and `pages = 0`;
pointing `schema` at a page with an invented field prints `illegal_fields >= 1` and `VERDICT fail=1`.
