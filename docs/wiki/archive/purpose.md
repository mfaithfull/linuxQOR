# Purpose - docs/wiki/archive

**Retired pages only.** A page that a newer page supersedes keeps its prose - it was about code that
existed, and that reasoning is what a later reader wants - so it moves here instead of being
deleted. `tools/wiki-retire` is the only writer.

- **References are never archived.** `docs/raw/` is a queue: an entry is a pointer, and the page that
  consumed it already holds its anchor, `pin:` and `verified:` stamp (§4.4). Archiving a pointer
  preserves nothing, so this folder never receives one.
- A page lands here with `Status: superseded` and `SupersededBy:` naming the page that replaced it;
  its anchors stay resolvable, and `tools/wiki-ref-check` still reads it.
- Nothing is authored here. A page that needs correcting is corrected where it stands; a page that
  needs retiring is retired by the instrument, which prints the move before making it.
- Not the repo's general archive: that is `docs/archive/`. This folder holds wiki pages, because the
  wiki's own lifecycle - current, stale, superseded - is a page property (§5.1).
