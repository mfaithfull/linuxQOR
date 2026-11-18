---
Type: Tool
Scripts:
  - File: tools/wiki-lint/wiki-lint.ps1
    Usage: "-Command <schema|provenance|freshness|purpose|size> (required) -WikiRoot <dir> (required) -Report <file> (required) [-TreeRoot <dir>] (required for freshness - a judge names a build artifact relative to the tree root) [-Schema <file>] (default <WikiRoot>/schema.md, resolved and printed with whether it existed) [-RawRoot <dir>] (widens purpose over the queue folder). -Help prints the subcommands and touches no file."
    Output: "the -Report file and the same lines on stdout; the report path is printed as 'report = <path>'. Reports belong in tmp/. Writes no tracked file - lint reports, governance decides."
  - File: tools/wiki-census/wiki-shape.ps1
    Usage: "dot-sourced, never invoked: the parse, the page classification and the builtin field list this script reuses."
    Output: "nothing - defining functions is its whole effect."
---

# wiki-lint

## What it does

Answers "does every page obey the schema, and does every claim carry provenance" - the governance
rules of `docs/WikiDesign.md` §6/§11 as counts, on every run. Five subcommands:

- `schema` - every page's fields against the schema: undeclared field, missing required field, a
  value outside an enum, a page in a folder that does not match its `PageType:`. Prints
  `pages = N of M`, `strays = N`, `illegal_fields = N`, and judges the governance file itself -
  `schema_sections = 7 of 7`, `schema_ops = 5 of 5`, `schema_ops_no_verdict = N`,
  `governance_drift = N`.
- `provenance` - no page contains a URL, a secret, or an absolute path (the wiki cites, it never
  copies). Prints `lines_read = N`, `violations = N`.
- `freshness` - every page has a `LastVerified:` that is not older than its newest judge evidence.
  Prints `judges = N  evidenced = N`, `unstamped = N`, `stale = N`, `undated = N`.
- `purpose` - every folder under the wiki root carries a `purpose.md` (repo rule). Prints
  `purpose_roots`, `folders = N`, `missing_purpose = N of N`, and the wider
  `docs_debt = N of M` separately - the older debt is counted, not judged here.
- `size` - no page over the budget. Prints `budget_lines`, `max_lines`, `over_budget = N`.

## Contract

Required args, no defaults: `-Command`, `-WikiRoot`, `-Report`; `-TreeRoot` additionally for
`freshness`. A missing or unreadable one prints `USAGE`, `MISSING args=<name>`, `VERDICT fail=1`,
exits `1`, touches no file.

The schema authority is `<WikiRoot>/schema.md` (`-Schema` moves it). The governance layer wrote it on
15 Nov 2026, so the read is real on every run and the builtin vocabulary of `wiki-shape` is only the
fallback; every run prints which one it used:

```
schema = docs/wiki/schema.md  source = schema.md
schema_fields = 13  [Anchors Claims GraphNodes ...]
fields = 13 of 13  (page fields the shape carries that this file declares)
schema_read = 159 lines  declared_fields_read = 13
schema_sections = 7 of 7  [fields pages links provenance archive budget ops]
schema_ops = 5 of 5  [ingest refresh wiki-reconcile wiki-file wiki-retire]
schema_ops_no_verdict = 0
governance_drift = 0
budget_lines = 200
```

The last three lines are what judge the governance file itself: it declares 7 sections and 5 op rows,
each op row must carry a verdict line, and a shape field it does not declare is drift. A schema that
loses a section goes red instead of quietly narrowing the rules.

A lint that silently invented its own rules is the F1 shape, so `source` and `budget_source` are
printed on every run, and `illegal_fields = 0` over `pages = 0` carries a `vacuity:` line. When
`schema.md` does exist its read is printed too (`schema_read = N lines`,
`declared_fields_read = N`), so a tolerant reader that read nothing cannot pass as a read.
`purpose` is the one subcommand with real input in an empty wiki: the scaffolding layer wrote the
folder contracts, so `missing_purpose = 0 of 4` (or `of 5` with `-RawRoot`, which adds the queue
folder) is a measured fact, not an empty count.

## Provenance

Written 14 Nov 2026 for
`docs/future/BuildLLMWiki/LayerInstruments/ReconcileLintInstruments/plan.md`; extended 15 Nov 2026
for the governance layer so the schema file is read and judged rather than assumed. Self-test:
`tmp/_tmp_llmwiki_lint_verify.ps1` -> **126 checks, 0 failed** - green legs over the real wiki for
all five subcommands, red legs over a `tmp/_tmp_lint_fixture/` wiki with an invented field, a wrong
enum, a page in the wrong folder, a URL, an absolute path, a stale stamp and an over-budget page,
a governance fixture (a schema missing a section, an op row with no verdict line, a field it does
not declare), plus the contract and `-Help` legs.

Bugs it found by being run:

- An unmatched regex capture group is `$null`, and `$null -eq ''` is **false** in PowerShell. The
  budget reader matched either of two alternations and then tested both groups for emptiness, so a
  line of prose with a number in it became the budget. Tested with `[bool]` on the group, and the
  read is printed (`budget_source = file|builtin`) so a wrong read is visible.
- `purpose` must not judge the whole repo: `docs_debt = 107 of 115` is printed as debt because the
  rule predates the wiki. Folding it into the verdict would make every future run red and teach
  nobody anything.
- `freshness` here and `verify` in `wiki-reconcile` share the shape's evidence reader so `stale`
  means one thing; they differ in what they do about it - lint reports, reconcile queues.
