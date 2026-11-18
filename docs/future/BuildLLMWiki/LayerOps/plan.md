---
Type: Plan
Title: "Layer ops"
Parent: "../plan.md"
Level: 1
Items:
  - Title: Write the ingest skill
    Status: Atomic
    Content:
  - Title: Write the file skill
    Status: Atomic
    Content:
---

# Layer ops

## Goal

The two skills - the only prose an LLM ever runs in this system - each short because the
instruments do the work.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: not started` - 2 `Atomic`, in item order.

## Context

Inherited at creation - re-read rather than trusted, the tree has moved since:

- **Parent goal**: the three layers, maintained by instruments rather than an LLM deciding freshness.
- **Parent locked decisions**: `docs/raw/` is a queue consumed to empty; the unit of wiki work is a
  build target; two catalogs that never sync (`docs/learning/` keeps its own index).
- **Sibling plan titles**: `Layer scaffolding and folders`, `Layer instruments`,
  `Layer governance` (all run before this one), `Layer pages`, `Layer gate proof`.
- **Where skills live in this repo**: `docs/skills/` holds `QORconventions.md` today. These two
  skills land beside it - `docs/skills/wiki-ingest.md`, `docs/skills/wiki-file.md` - so the repo's
  own convention is followed rather than a new folder invented.

## Decisions

- **`wiki-ingest` is ~30 lines** - §11. Its whole content is "read the schema, drain the queue,
  write the page, run the gates, stamp" plus the drain order. The instruments carry everything else.
- **The drain order is stated in the skill, not implied** - §4.4, F12. `carry` then `print` then
  `delete`, in that order, because the order is the thing that makes provenance survive deletion.
- **`wiki-file` never calls `wiki-reconcile`** - §5. One answers "what to read", the other answers
  "is what is compiled still true". They share the parse and never share a verdict.

## Write the ingest skill

Input verified: the six instruments exist and passed their self-tests (their reports are the counts
this skill reads), and the queue file exists from the scaffolding layer.

One write: `docs/skills/wiki-ingest.md`. Steps, enumerated by id: `1 read schema` (re-read
`docs/wiki/schema.md`); `2 read queue` (run `wiki-census pending` and take the entries);
`3 carry` (copy each entry's `path:`/`pin:`/`verified:` into the page being written, and each
`[[id]]` into a `See also` line); `4 print` (write the page, claims carrying anchors and
`Judges:`); `5 delete` (remove the consumed entries from `docs/raw/pending.md`);
`6 gates` (run `wiki-ref-check`, `wiki-lint`, `wiki-reconcile`); `7 stamp` (set
`LastVerified:` and `VerifiedAgainst:` to the current commit); `8 commit` (one
`wiki:`-prefixed commit carrying the drain's verdict line, §8 protocol 1).

The file states explicitly: it never archives a reference, and it never deletes an entry before
step 4 has written it.

Stop condition: the file re-reads with **8 of 8** numbered steps, its body is under 30 lines
excluding the front matter, and the three verbs `carry` / `print` / `delete` appear in that order.

## Write the file skill

Input verified: the six instruments exist and passed their self-tests; `docs/wiki/index.md` exists
from the instruments layer.

One write: `docs/skills/wiki-file.md`. Steps, enumerated by id: `1 read index`
(run `wiki-index lookup` for the target or topic); `2 read page` (read the page, take its
`Anchors:` and `Judges:`); `3 read target` (read only the referenced files);
`4 check freshness` (run `wiki-reconcile anchors` and report staleness as a **fact**, not a guess);
`5 answer` (answer from the page, citing the anchor ids); `6 queue` (if the answer needed a unit the
wiki has no page for, append one `added_by: manual` queue entry rather than inventing a page).

Stop condition: the file re-reads with **6 of 6** numbered steps, and the step-6 line names
`docs/raw/pending.md` - the skill must be able to make the wiki behind, which is what keeps the
queue non-empty when the wiki is asked something it cannot answer.
