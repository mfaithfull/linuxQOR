---
Type: Tool
Scripts:
  - File: tools/wiki-ref-check/wiki-ref-check.ps1
    Usage: "-Command <anchors|refs|orphans|drift> (required) -WikiRoot <dir> (required) -RawRoot <dir> (required) -TreeRoot <dir> (required) -Report <file> (required). -Help prints the subcommands and touches no file."
    Output: "the -Report file and the same lines on stdout; the report path is printed as 'report = <path>'. Reports belong in tmp/."
  - File: tools/wiki-census/wiki-shape.ps1
    Usage: "dot-sourced, never invoked: the parse, the page listing and the anchor resolver this script reuses."
    Output: "nothing - defining functions is its whole effect."
---

# wiki-ref-check

## What it does

Answers one question about the wiki's pointers: **does every pointer resolve** - and is every
pointer still the tree's. Four subcommands, one per question:

- `anchors` - every `Anchors:` entry of every page re-resolves against the tree; prints
  `anchors = N of N`, `dead`, `unpinned`, `tmp_refs`, `derived_only`, and a per-kind line.
- `refs` - every `[[target]]`, `[[target|text]]`, markdown `.md` link and `SupersededBy:` points at a
  page that exists; prints `links = N of N` and `broken = N`.
- `orphans` - which queued anchors no page holds yet, and which ids a page already cites while the
  entry survives: prints `orphans = N of N` and `carried_not_deleted = N`. The second is the leak
  that makes ingest's `carry` step verifiable (`docs/WikiDesign.md` 4.4: an entry is consumed).
- `drift` - pins older than the tree's current head for that path; prints `drift = N of N` over the
  queue pins and the in-tree page anchors that carry a pin.

It is **not** `wiki-reconcile`. This answers "every pointer resolves"; reconcile answers "what is
compiled still matches the tree" (5). They share the parse through `wiki-shape.ps1` and never share
a verdict. Nothing here fetches anything - out-of-tree anchors are judged on their `pin:` and
`verified:` alone, because reconcile has to stay free (9(2)).

## Contract

Required args, no defaults: `-Command`, `-WikiRoot`, `-RawRoot`, `-TreeRoot`, `-Report`. The tree
root is never a default - an anchor is re-resolved against the tree, so a baked root would re-check
whichever tree was current when the script was written. A missing or unreadable arg prints `USAGE`,
`MISSING args=<name>`, `VERDICT fail=1`, exits `1`, and touches no file.

```
pages = 0 of 4  (files that carry the WikiPage shape)
anchors = 0 of 0  (Anchors: entries that re-resolve against the tree)
dead = 0  (path, symbol or commit that no longer resolves)
unpinned = 0  (out-of-tree with no pin or verified date: F11)
tmp_refs = 0  (an anchor into tmp/ is a contract violation: 4.3 rule 3)
derived_only = 0  (line-anchor only - legal, never the only anchor: 4.3 rule 1)
in_tree=0 out_tree=0 event=0 artifact=0 unparsed=0  (nothing is fetched: reconcile stays free, 9(2))
vacuity: pages=0 - anchors = 0 of 0 here proves the parse ran, not that the wiki is clean (9)
VERDICT fail=0
```

`dead`, `unpinned`, `tmp_refs` and any unknown anchor kind each add to the verdict; `derived_only`
is legal and only reported. Over an empty wiki every count is printed with its denominator and a
`vacuity:` line, because `anchors = 0 of 0` looks exactly like a clean sweep over a full wiki.

## Provenance

Written 14 Nov 2026 for
`docs/future/BuildLLMWiki/LayerInstruments/RefIndexInstruments/plan.md`. Self-test:
`tmp/_tmp_llmwiki_refcheck_verify.ps1` -> **75 checks, 0 failed** - green legs over the real (empty)
wiki, red legs over a `tmp/_tmp_refcheck_fixture/` wiki holding one page with a dead path, a `tmp/`
anchor, an unpinned out-of-tree anchor, an old pin, a broken wikilink and a `Sources:` id that
matches a live queue entry, plus the contract leg and the `-Help` leg.

Bugs it found by being run:

- **A `[Parameter(Mandatory)]` argument on a shared helper is a false green.** Passing `''` to a
  Mandatory `[string]` throws a *non-terminating* bind error: the script prints the error and
  **carries on** with a `$null` root, then prints `VERDICT fail=0`. Every required arg is therefore
  tested in the dispatcher before it is passed anywhere, and the self-test asserts the absence of
  `Cannot bind` alongside the presence of `VERDICT fail=1`.
- An empty diff and a nonsense ref must not share a print: `refs` over an empty wiki and `refs` over
  an unreadable root both want `0 of 0`, so unreadable is a usage failure, never a zero.
- `Resolve-WikiAnchor` tries the full `qor::x::y` symbol and then its last `::` component, because a
  C++ qualified name is written both ways in prose; grepping only the qualified form reports live
  anchors as dead.
