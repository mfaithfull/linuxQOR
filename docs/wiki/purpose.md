# Purpose - docs/wiki

The prose layer about this repo, written for an **LLM reader**. Every claim in it carries an anchor
into the versioned tree and the handle that would falsify it, so that "is this still true?" is a
command rather than an opinion. `docs/WikiDesign.md` §5 is the design; `docs/wiki/schema.md` is the
governance file every op re-reads, and where this folder's rules actually live.

- **Claims are refutable or they are gossip.** No anchor, no claim; every `invariant` page carries a
  `Judges:` command. `tools/wiki-lint` counts the unanchored ones; that count is the page's debt.
- **Audience, by contrast**: `docs/learning/` teaches a human things the repo does not contain.
  These are two catalogs that never sync - §13 Q4 - joined only by an anchor id a page borrows.
- Subfolders are the taxonomy of §5, and only the ones with pages in them exist: `modules/`,
  `invariants/`, `archive/` now; `symbols/`, `concepts/`, `decisions/`, `gotchas/`, `questions/`
  arrive with the pages that need them, each with its own `purpose.md` (repo rule).
- Generated, never hand-written: `index.md`, `log.md`, `meta.md`, `gaps.md`. Their generators are
  `tools/wiki-index` and the ops that stamp them; an LLM edit of a catalog is one more way to drift.
- Instruments: `wiki-census`, `wiki-ref-check`, `wiki-index`, `wiki-reconcile`, `wiki-lint`,
  `wiki-retire`. Two skills run them: `wiki-ingest`, `wiki-file`.
- Tracked on purpose - wiki drift must show in the same diff as code drift - and therefore excluded
  from the staleness hook's diff, like `docs/raw/`.
