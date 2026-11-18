# Purpose - docs/raw

The reference **queue**. One file, `pending.md`, holding one entry per reference the wiki has not
yet compiled into a page. `docs/WikiDesign.md` §4 sets the rule and §4.4 the lifecycle; the entry
shape is written once, in `pending.md`'s header, and this file does not restate it.

- It is a **queue, consumed to empty**. `wiki-ingest` deletes an entry once the entry's anchor has
  been carried into a page. Empty means nothing is un-compiled; non-empty names exactly what is.
- **Nothing here is archived.** An entry is a pointer into something git already versions, so
  consuming it destroys nothing. Only pages retire, and they retire to `docs/wiki/archive/`.
- Nothing else lives here - no reference sets, no notes, no drafts of anything. A reference worth
  prose is a page in `docs/wiki/` that cites the entry.
- Instruments: `tools/wiki-census pending` counts the queue, `tools/wiki-ref-check` re-resolves
  every anchor in it, `tools/wiki-ingest` is its only consumer.
- Tracked on purpose. The `hook` producer must exclude this folder from its diff or the ingest's own
  commit re-enqueues forever - see the note in `.gitignore`.
