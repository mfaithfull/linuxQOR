---
Type: Plan
Title: "Retire instrument"
Parent: "../plan.md"
Level: 2
Items:
  - Title: Write wiki-retire
    Status: Atomic
    Content:
  - Title: Self-test wiki-retire
    Status: Atomic
    Content:
---

# Retire instrument

## Goal

One script that moves retired **pages** to the archive with a written reason, and refuses to move
anything else.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: closed` - 2 of 2 `Atomic` done 15 Nov 2026. `tools/wiki-retire/wiki-retire.ps1`
  (`retire|stamp|restore|refuse`, `-Help` prints **4 of 4** and touches no file); self-test
  `tmp/_tmp_llmwiki_retire_verify.ps1` -> **109 of 109, failed = 0**, and it prints this plan's three
  stop conditions verbatim: `retired = 1 of 1` on the fixture, the archive file re-reading with all
  three stamp legs, and the missing-leg run printing `unstamped = 1` with `VERDICT fail=1`. The
  refusal is structural as the Decisions require: `refuse` classifies 5 candidates and prints
  `refused = 4 of 5` - the queue file, the generated `index.md`, a `purpose.md` and an already-archived
  page - leaving `pages_ok_to_retire = 1` and `guard_leaks = 0`, and no subcommand takes a `docs/raw/`
  root as an argument at all. Extra guards the run earned:
  retiring `purpose.md`, `index.md`, an already-archived page or a missing page each prints
  `retired = 0 of 1` with `VERDICT fail=1`, and a `restore` with no `from:` leg, an occupied origin,
  or a live page each prints `restored = 0 of 1`. Nothing moved under `docs/wiki/` or `docs/raw/` -
  asserted by the self-test, which scans both trees before and after, not assumed.

## Context

Inherited at creation - re-read rather than trusted, the tree has moved since:

- **Parent goal**: the six scripts that make wiki maintenance mechanical.
- **Parent locked decisions**: one shared shape, no instrument opens prose; verdict lines not exit
  codes; `docs/raw/` is a queue consumed to empty.
- **Sibling plan titles**: `Census instrument`, `Ref and index instruments`,
  `Reconcile and lint instruments`, `Register the six tools`.

## Decisions

- **Pages retire, references never do** - §4.4. A superseded page holds prose worth reading; a
  superseded reference held only a pointer, and the page that ingested it already copied that
  pointer. The archive discipline therefore applies to `docs/wiki/` only.
- **Retirement is stamped, not deleted** - §6.3. Reason, evidence, commit. F11 is the failure mode
  of an unstamped retirement, and the script makes it unrepresentable rather than reviewable.

## Write wiki-retire

Input verified: `wiki-ref-check` exists from a sibling plan, and the archive layout exists from the
scaffolding layer.

One write: `tools/wiki-retire/wiki-retire.ps1`. Subcommands, enumerated by id:
`retire` - move one page from a wiki root into `<wiki root>/archive/`, appending the stamp, and
print `retired = N`;
`stamp` - write the triplet `reason:` / `evidence:` / `commit:` into the moved page, refusing if
any leg is missing and printing `unstamped = N` (F11);
`restore` - pull a retired page back out of the archive, printing `restored = N`;
`refuse` - reject any request to archive a `docs/raw/` entry, printing `refused = N` and the reason.

Contract: required args are the wiki root, the page path, and the three stamp legs; the script
resolves and prints every path it writes. `docs/raw/` is not an argument it accepts - the refusal
is structural, not a convention.

Stop condition: `-Help` prints **4 of 4** subcommands and touches no file.

## Self-test wiki-retire

Three units in a fixture tree under `tmp/`, enumerated by id: write a fixture page with a
supersession header; run `retire` on it and read the archive line; run `stamp` with one leg missing
and read the refusal. Nothing in this item may move a page under `docs/wiki/` - the archive
discipline is proven on a fixture, never by damaging a tracked page.

Input verified: the script exists and printed 4 of 4 from the item above; the fixture page is
created by this item's first unit, so it is not an unverified input.

Stop condition: the fixture tree prints `retired = 1 of 1`, the archive file re-reads with all three
stamp legs present, and the missing-leg run prints `unstamped = 1` with `VERDICT fail=1`. A
`retire` that succeeds without a stamp is a design failure, not a run failure, and is reported as
such.
