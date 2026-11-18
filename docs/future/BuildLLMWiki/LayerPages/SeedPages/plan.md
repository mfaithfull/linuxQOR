---
Type: Plan
Title: "Seed the core lib pages"
Parent: "../plan.md"
Level: 2
Items:
  - Title: Seed core pages slice 1
    Status: Atomic
    Content:
  - Title: Seed core pages slice 2
    Status: Atomic
    Content:
---

# Seed the core lib pages

## Goal

One page per core QOR lib target, each carrying its anchors and its target's build command as the
thing that would falsify it.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: both slices closed - this plan is terminal` - the 17 pages exist and every stop condition
  printed: `pages = 17`, `illegal_fields = 0`, `anchors = 40 of 40` with `dead = 0`, and
  `coverage` prints `coverage = 17 of 90` / `uncovered_targets = 73` / `dangling_module_pages = 0`.
- Measured while writing slice 1: `Type: WikiPage` + `PageType: module` is what `schema.md` declares
  (the item text's `Type: wiki-module` predates the governance file), and `VerifiedAgainst:` is **not**
  in the field table - writing it would print `illegal_fields > 0`. `Judges:` are run against
  `cmakebuild`, not `build`: `build` fails the `updatebuildnumber.cmake` rule for want of a
  `BUILD_NUMBER` cache entry. `qor_textshaping` is declared but its `add_subdirectory(shaping)` is
  commented out (`src/qor/essentials/text/CMakeLists.txt`), so its judge cannot go green.

## Context

Inherited at creation - re-read rather than trusted, the tree has moved since:

- **Parent goal**: populate the wiki bounded by 90 distinct build targets.
- **Parent locked decisions**: the unit of wiki work is a build target; the qmd rung is not built;
  pages are checked by instruments that already run; the target list is verified before any page is
  written (`Reconcile the target list`, the parent's first item).
- **Sibling plan titles**: `FrameworkLibPages`, `ComponentsLibPages`, `ClaimCarryingPages`.
- **Measured**: 17 core targets = 14 `src/qor` + 3 `src/platform`.

## Decisions

- **One page per target, in `docs/wiki/modules/`** - §5. The page name is the target name, which is
  what lets the hook map a diff to a page without an index of aliases.
- **`Judges:` is the target's build command, not a wiki command** - §13 Q2(iii). A page can be made
  red by the same command that makes its library red, so there is no wiki-specific notion of green.

## Seed core pages slice 1

Ten pages, one per target, enumerated by id: `qor_module`, `qor_current`, `qor_reporting`,
`qor_text`, `qor_textshaping`, `qor_unicode`, `qor_error`, `qor_interception`, `qor_issue`,
`qor_log`. Each page: front matter with `Type: wiki-module`, `Target:`, `Anchors:` (the target's
own `CMakeLists.txt` and its source dir, each with `path:`/`pin:`/`verified:`), `Judges:` (the
target's build command from `docs/agent/verification.md`), `LastVerified:`,
`VerifiedAgainst:`; body with the purpose line, the dependency lines, and the failure modes the
target is known for.

Input verified: the target list printed `targets = 90 of 90` by the parent's reconciling item.

Stop condition: `wiki-census pages` re-reads `pages = 10` and `wiki-lint schema` prints
`illegal_fields = 0` over those 10; `wiki-ref-check anchors` prints `anchors = N of N` with
`dead = 0`.

## Seed core pages slice 2

Seven pages, one per target, enumerated by id: `qor_tef`, `qor_injection`, `qor_threadmemory`,
`qor_property`, `qor_platform`, `qor_arch`, `qor_compiler`. Same shape as slice 1.

Input verified: the target list, and the 10 pages of slice 1 (which is what makes `wiki-lint`
meaningful - it has something to check).

Stop condition: `wiki-census pages` re-reads `pages = 17 of 17` for the core region, and the pair
`coverage` prints `uncovered_targets = 73` - the count every later leaf is measured against.
