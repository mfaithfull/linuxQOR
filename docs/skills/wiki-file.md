# wiki-file

Answer **from the wiki**, never from the tree alone, and make the wiki's behind-ness visible when it
cannot answer. It shares the parse with the instruments and never a verdict: it reports staleness
as a fact and never repairs it.

1. `read index` - `pwsh -NoProfile -File tools/wiki-index/wiki-index.ps1 -Command lookup
   -WikiRoot docs/wiki -Query <target-or-topic> -Report tmp/_tmp_file_lookup.txt`; the candidate
   page paths it prints. `lookup` reports; it does not judge.
2. `read page` - read each candidate page and take its `Anchors:` and `Judges:`.
3. `read target` - open **only** the files the anchors name; the page decides what is worth reading.
4. `check freshness` - `pwsh -NoProfile -File tools/wiki-ref-check/wiki-ref-check.ps1 -Command
   anchors -WikiRoot docs/wiki -RawRoot docs/raw -TreeRoot . -Report tmp/_tmp_file_anchors.txt`;
   report a `dead` anchor as a **fact**, not a guess.
5. `answer` - answer from the page, citing the anchor ids it carries, and say what the page cannot
   settle.
6. `queue` - if the answer needed a unit the wiki has no page for, append one `added_by: manual`
   entry to `docs/raw/pending.md` in the nine-field entry shape. Queue an entry; **never invent a
   page**.

A run that answers without touching step 6 while the wiki lacked the unit is the failure mode: the
queue must record what the wiki could not answer.
