# Purpose - docs/future/BuildLLMWiki

Plan tree for building the LLM wiki designed in `docs/WikiDesign.md` (§1-16). One folder per plan,
`plan.md` is the index, `Content:` points down and `Parent:` points back.

The tree builds three layers at `docs/raw/` (a queue ingest consumes - empty means nothing is
un-compiled), `docs/wiki/` (prose whose every claim carries an anchor and a falsification handle)
and `docs/wiki/schema.md` (the governance one file every op re-reads). Six instruments and two
skills make the maintenance mechanical; nothing about freshness is ever an LLM's judgement.

Order the tree enforces, and why: instruments exist **before** pages, because prose with no
instrument to judge it is the failure this design exists to prevent. Pages exist **before** the
gates are proven red, because an instrument that has never seen a red case is untested.

Status: charted 14 Nov 2026, tree terminal at 16 plans. Implementation: root item 1
`Layer scaffolding and folders` landed 14 Nov 2026 - `docs/raw/` and `docs/wiki/` (with `modules/`,
`invariants/`, `archive/`) exist, each with its `purpose.md`, and `docs/raw/pending.md` carries the
entry contract at zero entries. Items 2-7 have not run, so **no instrument exists yet to judge any
of it**: nothing here prints `pending=0` or `reconcile fail=0` until `Layer instruments` lands.
