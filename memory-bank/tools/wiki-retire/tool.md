---
Type: Tool
Scripts:
  - File: tools/wiki-retire/wiki-retire.ps1
    Usage: "-Command <retire|stamp|restore|refuse> (required) -WikiRoot <dir> (required) -Page <path> (required, comma-separated for refuse) -Report <file> (required); -Reason, -Evidence and -Commit additionally for retire (the three stamp legs) and for stamp; -RawRoot additionally for refuse, and accepted by the others, where it strengthens the gate. [-Successor <path>] [-TreeRoot <dir>]. -Help prints the subcommands and touches no file."
    Output: "the -Report file and the same lines on stdout; the report path is printed as 'report = <path>'. Every file the run writes is printed as 'wrote = <path>' and, for a move, 'deleted = <path>' - the pair is on the record or neither half happened. Nothing is written outside the -WikiRoot it was given; reports belong in tmp/."
  - File: tools/wiki-census/wiki-shape.ps1
    Usage: "dot-sourced, never invoked: the parse, the page listing and the run plumbing this script reuses."
    Output: "nothing - defining functions is its whole effect."
---

# wiki-retire

## What it does

The one wiki op that **moves content**, so the move is stamped or it does not happen
(`docs/WikiDesign.md` §8.1 op 14). Four subcommands:

- `retire` - move one page into `<WikiRoot>/archive/`, write the stamp, point a `-Successor` at it,
  and rewrite the inbound links that named the moved path. Prints `retired = N of N`,
  `stamp_legs = 3 of 3 [reason evidence commit]`, `inbound_rewrites = N file(s), M link(s)`.
- `stamp` - write or check the triplet `reason:` / `evidence:` / `commit:` on a page already in the
  archive. A missing leg prints `unstamped = N` with `VERDICT fail=1`; the run writes no partial
  stamp.
- `restore` - pull a page back out of the archive to its `from:` folder, clearing the supersession
  and setting `Status: draft`, because a restored page is not verified: `restored = N of N`.
- `refuse` - the guard made visible: try it on a queue file, a nav file, a `purpose.md`, an archived
  page and a live page. Prints `REFUSED [raw|nav|purpose|archive] <path> : <reason>`,
  `refused = N of N`, `guard_leaks = N`.

**Pages retire, references never do** (§4.4). A superseded page holds prose worth reading; a
superseded reference held only a pointer, and the page that ingested it copied that pointer, so the
page *is* the archive. The refusal is structural: the script acts only inside `-WikiRoot`, so a
`docs/raw/` entry is not a path it can move at all.

## Contract

Required args, no defaults: `-Command`, `-WikiRoot`, `-Page`, `-Report`; the three stamp legs for
`retire` and `stamp`; `-RawRoot` for `refuse`. A missing or unreadable one prints `USAGE`,
`MISSING args=<name>`, `VERDICT fail=1`, exits `1`, moves nothing.

`stamp` deliberately does **not** refuse a missing leg at the binder: the leg is the guard's
business, so the run must reach the guard and print `unstamped = 1` rather than a usage line - a
gate that cannot be reached cannot be tested. `retire` refuses earlier for the same three, because
a move without a stamp is F11 and this script makes that unrepresentable rather than reviewable.

```
retired = 1 of 1
  from = modules/qor_old.md
  to = tmp/_tmp_retire_fixture/wiki/archive/qor_old.md
  stamp_legs = 3 of 3 [reason evidence commit]
  rewrote 2 link(s) in modules/qor_new.md
refused = 4 of 5
guard_leaks = 0  (gate verdict and classification disagree: the guarantee broken)
```

The stamp is three body lines under `## Retirement`, not front matter: schema.md's field list has
no retirement field, so retiring in front matter would make a page a schema violation the moment it
was retired. `from:` is written as a fourth line so `restore` never has to guess a folder.

## Provenance

Written 14 Nov 2026 for `docs/future/BuildLLMWiki/LayerInstruments/RetireInstrument/plan.md`.
Self-test: `tmp/_tmp_llmwiki_retire_verify.ps1` -> **109 checks, 0 failed** - every move leg runs in
a `tmp/_tmp_retire_fixture/` wiki (the archive discipline is never proven by damaging a tracked
page), including the stamp's missing leg, `restore`, `refuse` over the five paths above, and the
guard's own consistency; plus the contract legs (a `retire` without `-Commit` cites F11 and moves
nothing) and the final assertion that the run moved nothing under `docs/wiki` or `docs/raw`.

Bugs it found by being run:

- `Resolve-WikiWikilink` in the shared shape has to return a **wiki-root-relative** path, because
  that is the coordinate every page's `Rel` uses. Repo-relative made inbound-link matching silently
  impossible: a move would report `inbound_rewrites = 0` and leave live pages pointing at the path
  that had just been moved, with no miss printed. The shape resolves and returns wiki-relative and
  says so at the `Test-Path`; the self-test asserts both the rewrite count and that `[[stem]]` forms
  are untouched.
- Inbound links are rewritten only where the **path** was named: `[[stem]]` still resolves, because
  `archive/` is a category folder of the shape, and rewriting it would churn pages for nothing.
  Both counts print (`rewrote N link(s)`, `wikilinks left alone = N`) so the difference is visible
  rather than inferred.
- `restore` must not carry the old `Status:` back out: a restored page is `draft` until something
  re-verifies it, or a retired claim returns to the corpus wearing the green of evidence it no
  longer has.
- A folder-scoped resolver must not scan folders for a target that already names one. The fixture
  deliberately holds **both** `archive/qor_half.md` and `modules/qor_half.md`: a resolver that
  fell back to scanning category folders for `archive/qor_half` could return the live page, and
  `refuse` would then report a movable page where the guarantee says already-archived. `guard_leaks`
  counts exactly that disagreement between the gate's verdict and the page classification, and it is
  asserted at 0.
