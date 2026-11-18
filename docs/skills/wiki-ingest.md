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
   line.
4. `print` - write the page: claims with anchors, a non-empty `Judges:` (the D56 sentinel only
   where the claim is not falsifiable against a build), within the schema budget.
5. `delete` - remove **only** the consumed entries from `docs/raw/pending.md`; nothing else.
6. `gates` - `wiki-ref-check anchors` and `refs`, `wiki-lint schema` / `provenance` / `freshness`,
   `wiki-reconcile verify`; a gate that prints red stops the commit, not the queue.
7. `stamp` - set `LastVerified:` on each touched page to the run date; `VerifiedAgainst:` is not a
   schema field - do not add it.
8. `commit` - one `wiki:`-prefixed commit carrying the drain's verdict line
   (`pending=0 consumed=N carried=N uncarried=0 VERDICT fail=0`); `uncarried>0` is data loss and
   forbids the commit. `docs/wiki/` and `docs/raw/` are hook-excluded, so this commit never
   re-enqueues.

The drain order - **carry, print, delete** - is the contract: it is what makes a deletion safe.
