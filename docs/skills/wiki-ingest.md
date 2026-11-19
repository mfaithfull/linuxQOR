# wiki-ingest

Drain the `docs/raw/` queue into wiki pages. The instruments do the judging; this skill only orders
the work. It **never archives a reference**, and it **never deletes an entry before step 4 has
written it**.

1. `read schema` - re-read `docs/wiki/schema.md`; its field list, page rules and op table govern
   every write below.
2. `read queue` - `pwsh -NoProfile -File tools/wiki-census/wiki-census.ps1 -Command pending
   -WikiRoot docs/wiki -RawRoot docs/raw -Report tmp/_tmp_ingest_pending.txt`; the entries it lists.
3. `carry` - copy each entry's `path:`/`anchor:`/`pin:`/`verified:` into the consuming page's
   `Anchors:`, record the entry `id:` in its `Sources:`, and carry each `[[id]]` into a `See also`
   line. A `pin: commit:<sha>` carries as `anchor: commit:<sha>` on the entry's `path:` in
   `Anchors:` - the pin dates the claim to the tree, it is not a second path.
4. `print` - write the page: claims with anchors, a non-empty `Judges:` (the D56 sentinel only
   where the claim is not falsifiable against a build), within the schema budget. Claim prose is
   present tense against the current tree - a `commit:` anchor bounds a claim, it does not narrate
   a fix - because git plus `Sources:` is the archive and page prose is the copy that gets trimmed.
5. `delete` - remove **only** the consumed entries from `docs/raw/pending.md`; nothing else.
6. `catalog` - regenerate the generated index so a drained page cannot leave a lying row:
   `pwsh -NoProfile -File tools/wiki-index/wiki-index.ps1 -Command build -WikiRoot docs/wiki -Report tmp/_tmp_ingest_catalog.txt`,
   then `pwsh -NoProfile -File tools/wiki-index/wiki-index.ps1 -Command check -WikiRoot docs/wiki -Report tmp/_tmp_ingest_check.txt`.
   `check` must print `drift = 0 of N`; any non-zero drift means `index.md` still carries a row the
   pages' front matter no longer generates, and it stops the drain exactly as a red gate would.
7. `gates` - run each named gate with its full argument set:
   `pwsh -NoProfile -File tools/wiki-ref-check/wiki-ref-check.ps1 -Command <anchors|refs> -WikiRoot docs/wiki -RawRoot docs/raw -TreeRoot . -Report tmp/_tmp_ingest_<command>.txt`;
   `pwsh -NoProfile -File tools/wiki-lint/wiki-lint.ps1 -Command <schema|provenance|freshness> -WikiRoot docs/wiki -RawRoot docs/raw -TreeRoot . -Schema docs/wiki/schema.md -Report tmp/_tmp_ingest_<command>.txt`;
   `pwsh -NoProfile -File tools/wiki-reconcile/wiki-reconcile.ps1 -Command verify -WikiRoot docs/wiki -RawRoot docs/raw -TreeRoot . -Report tmp/_tmp_ingest_verify.txt`;
   a gate that prints red stops the commit, not the queue.
8. `stamp` - set `LastVerified:` on each touched page to the run date; `VerifiedAgainst:` is not a
   schema field - do not add it.
9. `commit` - one `wiki:`-prefixed commit carrying the drain's verdict line
   (`pending=0 consumed=N carried=N uncarried=0 VERDICT fail=0`); `uncarried>0` is data loss and
   forbids the commit. The regenerated `docs/wiki/index.md` is committable generated output and rides
   in this commit (it is what `catalog` just wrote). `docs/wiki/` and `docs/raw/` are hook-excluded,
   so this commit never re-enqueues.

The drain order - **carry, print, delete** - is the contract: it is what makes a deletion safe.
`catalog` runs after the delete and regenerates `docs/wiki/index.md` from the front matter the drain
just wrote, so the index never trails the pages it claims to list.
