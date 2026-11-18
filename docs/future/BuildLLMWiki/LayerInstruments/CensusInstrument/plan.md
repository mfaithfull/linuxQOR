---
Type: Plan
Title: "Census instrument"
Parent: "../plan.md"
Level: 2
Items:
  - Title: Write wiki-census
    Status: Atomic
    Content:
  - Title: Self-test wiki-census
    Status: Atomic
    Content:
---

# Census instrument

## Goal

One script that reports the wiki's state as counts, so that "up to date" is a printed line rather
than a sentence.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: closed` - 2 of 2 `Atomic` done 14 Nov 2026. `wiki-census.ps1 -Help` prints 4 of 4; the
  self-test `tmp/_tmp_llmwiki_census_verify.ps1` reads **27 of 27, failed = 0** (4 subcommands over
  the real tree + the usage-failure leg). One deviation recorded: the shape the other five read was
  lifted out of the script into `tools/wiki-census/wiki-shape.ps1`, so the dispatcher and the shared
  parse are separate files and the census dot-sources its own shape. `tools/wiki-census/` and
  `memory-bank/tools/wiki-census/tool.md` carry it.

## Context

Inherited at creation - re-read rather than trusted, the tree has moved since:

- **Parent goal**: the six scripts that make wiki maintenance mechanical; census is first because
  the other five read its shape.
- **Parent locked decisions**: every instrument reads one shared shape and no instrument opens
  prose; instruments print verdict lines rather than exit codes; the unit of wiki work is a build
  target.
- **Sibling plan titles**: `Ref and index instruments`, `Reconcile and lint instruments`,
  `Retire instrument`, `Register the six tools`.
- **Inputs on disk**: `docs/raw/pending.md` (header only, zero entries) and the five layer folders,
  both created by `Layer scaffolding and folders`.

## Decisions

- **Four subcommands, one per question the wiki is asked** - §9. `pending` (is anything un-compiled),
  `targets` (which build units exist), `coverage` (which units have a page), `pages` (what exists and
  where it sits).
- **`0 of 0` is not a pass** - §9's vacuity line. The script prints the claimed and the seen count
  separately so that an empty read is visible rather than silently green.

## Write wiki-census

One write: `tools/wiki-census/wiki-census.ps1`. Subcommands, enumerated by id:
`pending` - parse `docs/raw/pending.md` entries, print `pending = N of N` and `consumed = N`;
`targets` - scan `set(target qor_` across `src/**/CMakeLists.txt`, print the target count;
`coverage` - intersect targets with page front matter, print `uncovered_targets = N`;
`pages` - list every page with its type and folder, print `pages = N` and `strays = N`.
Contract per `docs/agent/TOOL-FORMAT.md`: required args are the wiki root and the raw root (no
default may encode a task); reports go to `tmp/`, the script is tracked.

Input verified: the queue file and the five folders exist, from `Layer scaffolding and folders`.

Stop condition: `wiki-census.ps1 -Help` prints **4 of 4** subcommands and touches no file.

## Self-test wiki-census

One run over an empty wiki, enumerated by id: `pending`, `targets`, `coverage`, `pages`. The
expected print is `pending = 0 of 0`, `targets = 90 of 90` (the measured CMake scan),
`coverage` reporting every target uncovered, `pages = 0`. A `0 of 0` on `targets` is a **failure**
- the target scan has real input on disk, so a zero there means the scan is broken.

Input verified: the script exists and printed 4 of 4 subcommands from the item above.

Stop condition: the run's report line re-reads `VERDICT fail=0` with `targets` claimed equal to the
measured 90, and the same run against a deliberately unreadable path prints `VERDICT fail=1`
without touching a file (the tool-format contract: `USAGE` + `MISSING args=` + exit 1).
