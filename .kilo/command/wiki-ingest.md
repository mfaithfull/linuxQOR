---
description: Drain the docs/raw queue into the wiki per docs/skills/wiki-ingest.md
---
Drain the wiki queue. Current state, as printed by the instrument:

!`pwsh -NoProfile -File tools/wiki-census/wiki-census.ps1 -Command pending -WikiRoot docs/wiki -RawRoot docs/raw -Report tmp/_tmp_cmd_ingest_pending.txt`

If `pending = 0 of 0`, report that and stop - write nothing.

Otherwise run the `wiki-ingest` skill exactly as written in `docs/skills/wiki-ingest.md`, over
every queued entry: re-read `docs/wiki/schema.md` first, then **carry, print, delete** in that
order per entry, updating each touched page's claims against the current tree (a claim the
consumed commit contradicts is rewritten or anchored to its fix, never left standing).
Then run the gates step 6 names; a red gate other than the known pre-existing
`refs broken = 1` (qor_netserver.md -> maybe_unused) stops the commit, not the drain.
Close with the one `wiki:`-prefixed commit whose body carries the drain verdict line
(`pending=0 consumed=N carried=N uncarried=0 VERDICT fail=0`). `uncarried>0` is data loss:
never commit it.
