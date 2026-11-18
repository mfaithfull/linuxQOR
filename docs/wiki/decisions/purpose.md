# Purpose - docs/wiki/decisions

One page per file in `docs/agent/decisions/` - a decision the tree can be held to: what was
decided, what artifact it governs, and the commit that made the entry (`docs/wiki/schema.md`
section "Pages"). The prose stays in `docs/agent/decisions/`; a page that restates it has forked
the truth.

- The count is the ledger count, not a feeling: `wiki-census pages` prints `  decisions = N` and
  the day it diverges from `Get-ChildItem docs/agent/decisions -File` is a finding.
- These are the only wiki pages whose anchor is a **commit**, not a symbol:
  `{ kind: in-tree, path: docs/agent/decisions/D<n>.md, anchor: commit:<sha>, verified: <date> }`,
  resolved by `git cat-file -e <sha>^{commit}`. A decision page is stale when the code it decided
  on has moved, and `wiki-reconcile history` - not `wiki-ref-check anchors` - is what detects
  that; `anchors` only prints `dead` for a moved source path or an absent sha.
- `Judges:` is the verification the entry itself recorded (gate / build line / census). For an
  entry that names no command - a policy verdict - the page carries the sentinel
  `Judges: none - <reason>` under **D56**; freshness leaves such pages `undated`, by design,
  because nothing but the schema run re-checks them.
- Supersession is read from the ledger, not from prose: a superseded entry carries the legal
  `SupersededBy:` field and its page copies it. Measured 18 Nov 2026 no D-entry supersedes
  another, so no page here carries the field; the two reversals that exist (D50 against the
  29 Oct U1 verdict, D7 against the hand-index assumption) reverse **registers**, and their pages
  say so in prose.
- Not here: the entry's full rationale and evidence (those stay in `docs/agent/decisions/`),
  domain background (`concepts/`), or verdicts that live only in `registers/` - a page exists
  here exactly when a D-entry exists.
