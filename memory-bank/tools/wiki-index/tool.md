---
Type: Tool
Scripts:
  - File: tools/wiki-index/wiki-index.ps1
    Usage: "-Command <build|check|lookup|diff> (required) -WikiRoot <dir> (required) -Report <file> (required) -Query <text> (required for lookup) -Ref <git-ref> (required for diff, and it must resolve to a commit). -Help prints the subcommands and touches no file."
    Output: "build writes <WikiRoot>/index.md - the only file this tool owns - check writes nothing, and every command writes its report to -Report (tmp/); the report path is printed as 'report = <path>'."
  - File: tools/wiki-census/wiki-shape.ps1
    Usage: "dot-sourced, never invoked: the parse and the page listing this script reuses."
    Output: "nothing - defining functions is its whole effect."
---

# wiki-index

## What it does

Generates the wiki's index and answers the two questions an index is for. Four subcommands:

- `build` - regenerates `<WikiRoot>/index.md` **from page front matter alone** (topic, id, one
  line), grouped by `PageType`. Prints `index_rows = N of N` counted in the file that was just
  written, plus `files`, `pages`, `groups`, `indexed`/`unindexed` and a `NO_SUMMARY` line per page
  whose row would be id-only.
- `check` - the drift gate: re-assembles the catalog from front matter and compares it to
  `<WikiRoot>/index.md` **without writing anything**. Prints `drift = D of N`, the pages whose row on
  disk is not the row their current front matter would generate, plus `catalog_rows` (what a build
  would write), `index_rows` (what is on disk) and `orphans` (disk rows naming no page). A `DRIFT
  <page>` line names each one. Unlike `lookup`/`diff`, it judges: `drift > 0` (or `orphans > 0`) is a
  hard `fail=1`, because a stale row means the index lies about a page it names.
- `lookup` - the page for a target id or a topic phrase: `-Query` is matched against `Target:`, the
  stem, `Title:` and `Summary:`, and prints `hits = N of M` with one `HIT <page> (matched <field>)`
  line each.
- `diff` - the pages whose file changed since `-Ref`, printed as `moved = N of M`, splitting
  `modified since <ref>` (git diff) from `untracked (never committed)` (git ls-files) - a page that
  has never been committed is new content git diff cannot see.

The index is generated, never maintained (`docs/WikiDesign.md` 6): a hand edit of `index.md` is a
contract violation, so there is no hand-written index to keep in step, and the next `build` erases
it. That is asserted in the self-test rather than trusted. `check` closes the other half of the gap:
a page whose front matter changed but whose index was left un-rebuilt is a row that quietly lies, and
`check` turns that silent lie into `drift = D of N` red so a forgetful drain fails a gate instead of
shipping a stale index.

## Contract

Required args, no defaults: `-Command`, `-WikiRoot`, `-Report`, plus `-Query` for `lookup` and
`-Ref` for `diff`. A `-Ref` that does not resolve to a commit is a **usage** failure
(`MISSING args=Ref (does not resolve to a commit: ...)`), never a run that prints `moved = 0` - an
empty diff and a nonsense ref must not share a print. A missing or unreadable arg prints `USAGE`,
`MISSING args=<name>`, `VERDICT fail=1`, exits `1`, touches no file.

```
index = docs/wiki/index.md  (6 lines written and re-read)
files = 5  (of which 0 stray .md are not indexed: a stray is wiki-lint to judge, not a row)
pages = 0
index_rows = 0 of 0  (row lines counted in the file that was just written, against the pages on disk)
indexed = 0 unindexed = 0
groups = 0  (one per PageType; a page with no PageType lands in (no PageType))
vacuity: pages=0 - index_rows = 0 of 0 is an empty index, not a working one (9)
VERDICT fail=0
```

`fail=1` means the index does not match the pages: for `build`, `index_rows` below `pages`; for
`check`, any `drift` or any orphan. `lookup` and `diff` report and do not judge - deciding that a
moved page must be re-read is `wiki-reconcile` (5). `check` does judge: a row on disk the front
matter no longer generates is exactly the drift op 12 permits, so it goes red. `build` re-reads the
file it wrote and counts the rows there, so the number is the file's, not the list it meant to write.

## Provenance

Written 14 Nov 2026 for
`docs/future/BuildLLMWiki/LayerInstruments/RefIndexInstruments/plan.md`. Self-test:
`tmp/_tmp_llmwiki_index_verify.ps1` -> **96 checks, 0 failed** - green legs over the real (empty)
wiki; the agreement the plan asks for, `index_rows` equal to the page count `wiki-census` reports
from the same disk state (and the census classifying the generated file as nav, dropping
`nav_missing` to 4 of 5, which is one shared shape proven rather than claimed); a fixture wiki in
**its own git repo** under `tmp/` (the outer repo ignores `tmp/`, so `moved >= 1` is otherwise
unobservable) proving stray-exclusion, `NO_SUMMARY`, a hand edit erased by rebuild, lookup by
target / by summary-only word / a miss, and `moved` over one modified plus one untracked page; and
the contract and `-Help` legs.

Bugs it found by being run:

- A git fixture must be **its own repo**. `git ls-files --others --exclude-standard` inside the
  outer repo honours `/tmp` in `.gitignore`, so a fixture under `tmp/` reports zero changes forever -
  a `moved = 0` that looks like a clean wiki and is really an invisible one.
- The expectations, not the instrument, were wrong twice: adding a page grows the page count, so
  `moved = 2 of 3 / unchanged = 1`, and the generated `index.md` is nav, so it never appears as a
  moved page. Both are asserted explicitly so the next reader does not "fix" the instrument.
