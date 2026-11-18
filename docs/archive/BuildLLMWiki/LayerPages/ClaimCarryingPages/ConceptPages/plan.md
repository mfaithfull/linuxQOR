---
Type: Plan
Title: "Concept pages"
Parent: "../plan.md"
Level: 3
Items:
  - Title: Write the concepts folder purpose page
    Status: Atomic
    Content:
  - Title: Write the RCU concept page
    Status: Atomic
    Content:
  - Title: Write the libguarded concept page
    Status: Atomic
    Content:
  - Title: Write the libguarded-contract concept page
    Status: Atomic
    Content:
  - Title: Write the forum concept page
    Status: Atomic
    Content:
  - Title: Write the community concept page
    Status: Atomic
    Content:
  - Title: Rebuild the index and close the pass
    Status: Atomic
    Content:
---

# Concept pages

## Goal

Five concept pages, one per external row of `docs/learning/RCU/RESOURCES.md`, plus the folder
purpose page and the index close. A concept page describes a domain, not this tree; under D56 it
carries the sentinel judge and no anchor.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `CLOSED 18 Nov 2026 - purpose page + five concept pages landed (docs/wiki/concepts/purpose.md,
  rcu.md, cs-libguarded.md, libguarded-contract.md, copperspice-forum.md, cpp-community.md);
  close-line: census `  concepts = 5` `purpose_files = 7`, wiki-index build `index_rows = 156 of
  156 / unindexed = 0 / groups = 5`, ref-check anchors `516 of 516, dead = 0`, schema
  `illegal_fields = 0`, freshness `stale = 0 / undated = 148` informational, all fail=0.
  wiki-ref-check refs run at the close prints `broken = 1` - the SAME pre-existing finding
  (`modules/qor_netserver.md -> maybe_unused`, recorded by the GotchaPages pass); every concept
  [[link]] resolves (683 of 684).`
- `Page contract (D56 + shipped shape): Type: WikiPage, PageType: concept, Title, Status:
  current, LastVerified, Claims, Summary, Judges: none - this page describes a domain, not this
  tree; it is not falsifiable against a build, NO Anchors:, body carries the domain-not-tree
  sentence, See also links only concept siblings as [[wiki-links]]; learning ids are named in
  prose (docs/learning/ and docs/wiki/ never sync - never [[link]] them).`
- `Stop counts: after leaf 1 purpose_files = 7; after page n, census `  concepts = n` (1..5);
  the close runs wiki-index build (groups = 5), ref-check anchors (dead = 0, no new anchors),
  schema (illegal_fields = 0), freshness (stale = 0, the five undated informational).`

## Context

Inherited from `../plan.md` (Level 2) - re-read rather than trusted. The item's old prose
(`Type: wiki-concept`, "no Judges:") is instrument-illegal and was resolved by D56
(`docs/agent/decisions/D56.md`): `PageType: concept` is shipped vocabulary (wiki-shape.ps1:33) and
the sentinel judge passes the required-field rule; freshness leaves such pages `undated`. The
worked-example siblings are `GotchaPages` and `InvariantPages` (leaf order: purpose where the
folder lacks one, one page per leaf, index close as the last leaf).

## Decisions

- **External rows only** - a `Local:` row describes this tree and already has module/gotcha
  handles; a concept page for it would double-describe and be falsifiable, which is not what a
  concept is.
- **The row's "Use for:" text is the claim** - each page summarizes what the resource is authority
  for, plus the date the row records for its fetch where it names one; nothing outside the row is
  asserted about the domain.
- **Seven leaves is the breadth cap exactly** - no probe leaf: the probe is these slot rows,
  already measured.

## Write the concepts folder purpose page

Input verified: `docs/wiki/concepts/` absent (measured); shape of `docs/wiki/gotchas/purpose.md`.

One page: `docs/wiki/concepts/purpose.md` - why concept pages exist (domain vocabulary the wiki
carries without pretending it is a claim about this tree), that they carry no anchor and the
sentinel judge (D56), and that they are never sync targets for `docs/learning/`.

Stop condition: `wiki-census pages` prints `purpose_files` moved up by one and `wiki-lint schema`
prints `illegal_fields = 0` with the new file.

## Write the RCU concept page

Input verified: probe row 1 (`RESOURCES.md` L5, fetched and verified 2026-10-24); through leaf 1.

One page: `docs/wiki/concepts/rcu.md` - the read-copy-update domain: the canonical five-step
contract, updaters cannot block readers or force retry, the retire/deallocate phase, RCU against
reader-writer locking; the Wikipedia row named in prose.

Stop condition: census `  concepts = 1`, schema `illegal_fields = 0`.

## Write the libguarded concept page

Input verified: probe row 2 (`RESOURCES.md` L10); through leaf 2.

One page: `docs/wiki/concepts/cs-libguarded.md` - the upstream domain: header-only, C++20,
BSD 2-clause, copyright 2015-2018 Geller / Sermersheim, the file header both QOR copies carry,
what "upstream" means when reasoning about the copy.

Stop condition: census `  concepts = 2`, schema `illegal_fields = 0`.

## Write the libguarded-contract concept page

Input verified: probe row 3 (`RESOURCES.md` L16, Doxygen v2.0.1); through leaf 3.

One page: `docs/wiki/concepts/libguarded-contract.md` - the intended upstream contract domain:
`rcu_guarded` and the wrapped type's `rcu_read_lock` / `rcu_read_unlock` / `rcu_write_lock` /
`rcu_write_unlock` members as the upstream defines them.

Stop condition: census `  concepts = 3`, schema `illegal_fields = 0`.

## Write the forum concept page

Input verified: probe row 4 (`RESOURCES.md` L35); through leaf 4.

One page: `docs/wiki/concepts/copperspice-forum.md` - the upstream-community domain: where
cs_libguarded-specific questions belong; what a forum answer is and is not (community, not spec).

Stop condition: census `  concepts = 4`, schema `illegal_fields = 0`.

## Write the community concept page

Input verified: probe row 5 (`RESOURCES.md` L37); through leaf 5.

One page: `docs/wiki/concepts/cpp-community.md` - the mainstream-practice domain: r/cpp as the
place an RCU design gets reviewed against mainstream C++ concurrency practice; community review is
context, not a build verdict.

Stop condition: census `  concepts = 5`, schema `illegal_fields = 0`.

## Rebuild the index and close the pass

Input verified: five concept pages landed, census `  concepts = 5`.

One generator run and the slot writes: `wiki-index build` - expected `groups = 5` (`concept`
joins `decision`, `gotcha`, `invariant`, `module`) and `index_rows` equal to census `pages`;
then `wiki-ref-check anchors` re-read (`dead = 0` - no concept page carries an anchor),
`wiki-lint schema` (`illegal_fields = 0`), `wiki-lint freshness` (`stale = 0`, the five concept
pages `undated` informational). This plan's close line goes into this slot, and the parent's.

Stop condition: `  concepts = 5`; `wiki-index build` `index_rows = <pages>` with `unindexed = 0`
and `groups = 5`; `wiki-ref-check anchors` `dead = 0`; each instrument `VERDICT fail=0`.
