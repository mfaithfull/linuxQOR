---
Type: Tool
Scripts:
  - File: tools/wiki-reconcile/wiki-reconcile.ps1
    Usage: "-Command <anchors|verify|refresh|audit|history> (required) -WikiRoot <dir> (required) -RawRoot <dir> (required) -TreeRoot <dir> (required) -Report <file> (required) [-Ref <git-ref>] (required for history) [-DryRun] [-RunJudges]. <TreeRoot>/docs/agent/verification.md is the only derived path - it is the judge vocabulary, resolved and printed on every run. -Help prints the subcommands and touches no file."
    Output: "the -Report file and the same lines on stdout; the report path is printed as 'report = <path>'. refresh appends entries to <RawRoot>/pending.md and prints each as 'queued = <id>'; no other subcommand writes a tracked file. Reports belong in tmp/."
  - File: tools/wiki-census/wiki-shape.ps1
    Usage: "dot-sourced, never invoked: the parse, the page listing, the anchor resolver and the judge evidence this script reuses - so 'dead' and 'stale' mean one thing in all six instruments."
    Output: "nothing - defining functions is its whole effect."
---

# wiki-reconcile

## What it does

Answers "what is compiled still matches the tree" - the half `wiki-census`'s `pending = 0` cannot
imply (`docs/WikiDesign.md` §5). Five subcommands, ascending eagerness: `anchors` re-resolves every
page and queue anchor and prints dead, unpinned and tmp ones; `verify` reads each page's judge
evidence and stamps a page stale whose newest evidence post-dates its `LastVerified:`; `refresh`
runs both and appends one nine-field queue entry per finding, deduped against what is already
queued; `audit` sweeps prose across `docs/` for backticked anchors that no longer resolve;
`history` prints churn since a ref - which pages the changed files touch, and which pages nobody
links.

Freshness is read, not executed: a judge's evidence is the artifact its own text names (`build/...`
exists? the exe's date?) so the sweep costs nothing and can run at every session start (§9(2)).
`-RunJudges` is the opt-in that actually runs them; the difference is printed as `judges_run = 0|N`.

## Contract

Required args, no defaults: `-Command`, `-WikiRoot`, `-RawRoot`, `-TreeRoot`, `-Report`, plus `-Ref`
for `history` - an empty ref is refused because churn measured against nothing is not zero churn.
Missing or unreadable: `USAGE`, `MISSING args=<name>`, `VERDICT fail=1`, exit `1`, no file touched.

The judge vocabulary is **read from `docs/agent/verification.md`**, never copied: the run prints
`rule_sections = N of N`, the heads it extracted, and the builtin allowance (`dumpbin`) separately,
because "the rule is not copied, it is read" is §13 Q2 and a copy is what rotted before.

```
pages = 6 of 11
anchors = 7 of 9  dead = 1  unpinned = 1  tmp_refs = 1  derived_only = 1
stale = 1  (newest judge evidence is newer than the LastVerified: stamp)
queued = fx-refresh-stale-1  added_by = refresh
skipped_duplicate = 1  (an id naming the same finding already in the queue)
prose_docs = 220  prose_anchors = 473 of 536  prose_dead = 63
churn_since = HEAD~5  churn_files = 20  pages_touched = 0  orphans = 0  moved = 0
```

`refresh` is the only writer: the queue digest is taken before and after, and `added = N of N` is
verified by re-reading the file, so an append that did not land is never reported as one.
`-DryRun` prints the rows and writes nothing. A stale page is *flagged*, never rewritten (§9(2)).

## Provenance

Written 14 Nov 2026 for `docs/future/BuildLLMWiki/LayerInstruments/ReconcileLintInstruments/plan.md`;
the anchor half reuses the shared shape's `Resolve-WikiAnchor` so "dead" means one thing in both
this tool and `wiki-ref-check`. Self-test: `tmp/_tmp_llmwiki_reconcile_verify.ps1`, 110 checks.

Bugs it fixes, each found by running it against the real tree:

- `Set-Content -LiteralPath X -Value @($rows) -NoNewline` on a file that already holds rows writes
  them **all on one line** - the queue survives and parses as nothing, so dedup breaks on the next
  run. An append is `$existing + $rows | Set-Content -NoNewline`, and the shape exposes
  `Add-WikiQueueEntry` for exactly that reason (proven by probe, not by reading docs).
- A `[Parameter(Mandatory)][string]` inner parameter does not make a required arg: passing `''`
  throws a *non-terminating* bind error, the script prints the error and **runs on** - a false
  green. Requiredness is tested before the call (`-Usage` is passed down so the failure prints the
  contract). `$Args` is a PowerShell automatic variable: never use it as a parameter name.
- `history -Ref ''` resolved as "no ref" and printed churn of nothing. An empty required value is
  now a `MISSING args=` failure at the dispatcher.
- `audit` excludes `.worktrees/**`: a worktree is a snapshot of another tree, and its stale plan
  references are not this tree's rot (it added 27 false deaths on the first run).
- `history` measures **the wiki's own churn**, not the repo's: 38 repo-wide files touched 0 pages,
  which is the fact the staleness gate acts on.
