---
Type: Plan
Title: "Layer scaffolding and folders"
Parent: "../plan.md"
Level: 1
Items:
  - Title: Write the queue header
    Status: Atomic
    Content:
  - Title: Write the folder purposes
    Status: Atomic
    Content:
  - Title: Track the layers
    Status: Atomic
    Content:
  - Title: Correct the library count
    Status: Atomic
    Content:
---

# Layer scaffolding and folders

## Goal

Make the three layers exist and be tracked, so that every instrument path written later resolves
through folders that are already on disk.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: CLOSED 14 Nov 2026` - 4 of 4 `Atomic` items landed. Measured by
  `tmp/_tmp_llmwiki_scaffold_verify.ps1` -> `.txt` (recreate by slug): `entries = 0 of 0`,
  `header fields = 9 of 9`, `producers = 5 of 5`, `dirs = 5  purpose = 5 of 5`,
  `pattern_lines = 41 (baseline 41)  docs_patterns = 0 of 0  comment_line = 1 of 1`,
  `header_has_measured_counts = True  index_rows = 88 of 88  stale_33_claims = 0 of 0`,
  `cmakelists = 208  declarations = 91 of 91  distinct = 90 of 90`, `VERDICT fail=0`.
- On disk: `docs/raw/pending.md` (header, zero entries) and `purpose.md` in the five folders
  `docs/raw/`, `docs/wiki/`, `docs/wiki/modules/`, `docs/wiki/invariants/`, `docs/wiki/archive/`;
  `.gitignore` carries the TRACKED, NOT IGNORED note at `/tmp`; `docs/agent/repo_map.md`'s Library
  index header reads `91 declarations / 90 distinct / 88 indexed rows` with a dated correction note,
  and its `Symbol index plan` line no longer claims 33. **Nothing is committed** - the one `wiki:`
  commit is the root plan's last item. Next in item order: root item 2, `Layer instruments`.
- `Correction to Context`: `.gitignore` holds **41** pattern lines (14 of them root-directory), not
  "exactly two (`/.ua`, `/tmp`)". The load-bearing half of that measurement is true and re-checked:
  **0 patterns match anything under `docs/`**, and `git check-ignore` exits 1 on both layers - they
  are tracked, and §13 answer 3 still costs nothing.

## Context

Inherited at creation - re-read rather than trusted, the tree has moved since:

- **Parent goal**: land the three layers of `docs/WikiDesign.md` so maintenance is instruments, not
  an LLM deciding freshness.
- **Parent locked decisions**: all three layers sit under `docs/`; every layer is **tracked**;
  `docs/wiki/schema.md` is **one file**, not a folder; `docs/raw/` is a **queue** that ingest
  consumes to empty.
- **Sibling plan titles**: `Layer instruments`, `Layer governance`, `Layer pages`, `Layer ops`,
  `Layer gate proof`. This plan runs first - nothing else resolves a path until these folders exist.
- **Measured at 14 Nov 2026**: `docs/raw/` and `docs/wiki/` do not exist; `.gitignore` holds exactly
  two patterns (`/.ua`, `/tmp`); `.gitattributes` holds one text/spec rule.

## Decisions

- **`docs/wiki/schema.md` is a file at the wiki root**, not a `schema/` folder - `docs/WikiDesign.md`
  §13.1. The repo rule "every new folder needs a `purpose.md`" would otherwise demand a folder with
  one member.
- **No `archive/` for raw** - §4.4. References are consumed, not archived; only pages retire.

## Write the queue header

Input verified: none - this is the tree's first write; §2.1 of `docs/WikiDesign.md` supplies the
header it must copy.

One write: `docs/raw/pending.md` - create `docs/raw/` and write the file's header block verbatim
from §4.4: the entry shape (`id:`, `added_by:`, `kind:`, `path:`, `anchor:`, `pin:`,
`verified:`, `target:`, `why:`), the five producers (`hook`, `manual`, `learning`, `refresh`,
`reconcile`), and the one consumer (`wiki-ingest`). The file is created with **zero entries** - the
header is the contract, and an empty queue is a legal steady state.

Stop condition: `wiki-census` does not exist yet, so the check is a re-read that prints the entry
count: `Select-String -Path docs/raw/pending.md -Pattern '^- id: '` -> **0 of 0**, and the header
fields re-read 9 of 9.

## Write the folder purposes

Input verified: the folders named here do not exist yet; `docs/future/*/purpose.md` supplies the
pattern this item copies.

Five folders and five `purpose.md` files, enumerated by id: `docs/raw/` (a queue, consumed to
empty); `docs/wiki/` (prose, every claim carries an anchor and a falsification handle);
`docs/wiki/modules/` (one page per QOR lib target); `docs/wiki/invariants/` (the repo's rules
written as assertions); `docs/wiki/archive/` (retired pages only - references are never archived).

Stop condition: a directory listing prints `dirs=5` and `purpose=5 of 5`.

## Track the layers

One edit to `.gitignore`: add the comment line that records why the layers are tracked -
`docs/wiki/` and `docs/raw/` are **not** ignored, and the ingest hook that enqueues from a diff
**must exclude both** or the ingest's own commit re-enqueues forever (§13 Q3, F9). The edit adds
no ignore pattern; it records the non-pattern, which is the thing a later reader will otherwise
assume.

Input verified: item `Write the queue header` and `Write the folder purposes` run before this one,
and the folder-purpose listing prints the two layer folders as present.

Stop condition: `Select-String -Path .gitignore -Pattern '^(?!/#).+'` re-reads **2 of 2** patterns
and the comment line re-reads once.

## Correct the library count

Input verified: `wiki-census targets` printed 91 declarations / 90 distinct, and the 88 rows are
measured in `docs/agent/repo_map.md`.

One edit to `docs/agent/repo_map.md`: its Library index header claims 33 libraries while its own
index lists 88 `- qor_*` rows and the CMake scan measures 91 `set(target qor_` declarations across
90 distinct targets (§13.1's measured correction). Correct the header to the measured counts and
date the correction, because the wiki's page count and the hook's unit count both derive from this
number, and prose counts are not the authority.

Stop condition: the header re-reads `91 declarations / 90 distinct / 88 indexed rows` and the index
re-scans to 88 rows.
