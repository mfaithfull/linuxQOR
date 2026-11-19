# schema - the governance of docs/wiki/

One file, because a rule that lives in two places runs the op two ways (F10). Every op re-reads
this file instead of remembering it; `tools/wiki-lint` reads it on every run and prints which
vocabulary was in force, so a wrong read is visible rather than silent.

Two halves, and the second is what makes the first worth writing: a rule stated here and printed by
no instrument is a wish, so every section below names the instrument and the count that closes it.
This file is a nav file of section 6, not a page: it carries no front matter, and the authority on
that sentence is `Get-WikiPageClass` in `tools/wiki-census/wiki-shape.ps1`.

## Fields

Front matter carries machine state; the body carries prose (the same split as
`docs/future/PLAN-FORMAT.md`). A field outside this table is an offence, and so is a missing one.

| Field | Type | Required | Says | Judged by |
|---|---|---|---|---|
| `Type` | scalar | yes | the literal WikiPage: this file is a page | wiki-lint schema |
| `PageType` | enum | yes | which of the seven kinds of page it is | wiki-lint schema |
| `Title` | scalar | yes | the line a reader sees in index.md | wiki-index |
| `Anchors` | list of map | yes in practice | what the page is about: kind, path, symbol or anchor, pin, verified | wiki-ref-check anchors, wiki-reconcile anchors |
| `Sources` | list | no | queue ids this page consumed - the entries are gone, the ids are the archive | wiki-census pending, wiki-retire plan (F12) |
| `GraphNodes` | list | no | ids into .ua/knowledge-graph.json: the wiki sits above the graph | not judged here by design (section 11) |
| `Status` | enum | yes | current, stale, superseded, draft, retired | wiki-lint schema; only wiki-reconcile may set stale (F6) |
| `SupersededBy` | scalar | no | the page that replaced this one | wiki-retire, wiki-ref-check refs |
| `Claims` | int | no | how many claims the body carries | informational; wiki-lint provenance reads the body |
| `LastVerified` | date | yes | the date the Judges were last weighed, YYYY-MM-DD | wiki-reconcile verify, wiki-lint freshness |
| `Judges` | list | yes | the commands that would falsify this page | wiki-reconcile verify |
| `Summary` | scalar | no | the one line index.md prints; the LLM writes it, the tool writes the catalog | wiki-index |
| `Target` | scalar | no | the build target this page is about: the unit of wiki work | wiki-census coverage |

Vocabulary, and the instruments read it from `wiki-shape.ps1` rather than from prose:

- PageType: module, symbol, concept, invariant, decision, gotcha, question.
- Status: current, stale, superseded, draft, retired.
- Anchor kind (inside `Anchors`): in-tree, out-of-tree, event, artifact.
- A date is YYYY-MM-DD. A stamp that will not parse is undated, not fresh.
- `Claims:` counts the body's bulleted claims - the intro paragraph and section prose are not
  bullets, so they are not counted. The number is informational, which is why `wiki-lint provenance`
  reads the body and judges the provenance rules rather than the count.

Required means present and non-empty: `Type`, `PageType`, `Title`, `Status`, `LastVerified`,
`Judges`. Stop condition for this section: `wiki-lint schema` prints `fields` as a full pair with
`illegal_fields = 0`.

## Pages

A page lives in the folder that matches its PageType, and a file in the wrong folder is a
`WRONG-FOLDER` offence:

| Folder | PageType | One page is |
|---|---|---|
| modules/ | module | one qor_ build target (91 declarations over 90 distinct names, measured 14 Nov 2026) |
| symbols/ | symbol | a symbol that earned the page, below |
| concepts/ | concept | one idea the tree assumes and no file states |
| invariants/ | invariant | one claim that must hold plus the handle that falsifies it |
| decisions/ | decision | a pointer into docs/agent/decisions/D<n>.md: title, one line, never the body |
| gotchas/ | gotcha | a pointer into memory-bank/gotchas/ plus what the trap now costs |
| questions/ | question | a filed answer, named date-slug |
| archive/ | - | retired pages only: a lifecycle folder, not a category |

- Every folder of the taxonomy carries a purpose.md. A folder without one is a
  `MISSING-PURPOSE` offence (wiki-lint purpose).
- Significance rule (section 5.3, the god-node guard): a symbol earns a page at three claims or at
  above-median in-degree. Everything else is a section of its module page (F5).
- What may not be a page: the body of a D-entry, the dependency list, the graph re-stated in
  prose. Link the artifact, add the why the generator cannot state (section 11, F4).

## Links

- A link is [[Target]] or [[Target|text]], or a markdown link whose target ends .md.
- Resolution is deterministic and tries, in order: the page's own folder, then each folder of the
  taxonomy, then the wiki root. A link that resolves nowhere prints a BROKEN line and fails the run
  (wiki-ref-check refs).
- `SupersededBy` is resolved as a link, so a page cannot point its successor at nothing.
- A claim links an anchor; a page links a page. A sentence about code that links neither is
  gossip - see Provenance.

## Provenance

What a page may not contain, each counted by wiki-lint provenance as `violations`:

- No URL. The queue at docs/raw/ holds the reference; a page cites the id it consumed.
- No secret. A tracked file is a published file.
- No absolute path. An anchor is a relative path, so it resolves on a machine nobody has used yet.

What a claim must carry, from section 4.3, counted as `unanchored` where the tooling grows the
check and counted by hand until then (F3):

- A named anchor beats a numeric one: path#symbol over path:123. A line number may be recorded and
  may not be the only anchor - it is printed as derived_only, never as resolved by itself.
- Every anchor carries verified: and, for out-of-tree, pin:. A missing pin is `unpinned` and a hard
  fail (F11: the count of an SDK header changes with the triple that nobody wrote down).
- No anchor into tmp/ - it is gitignored and deleted per task, and this repo has already lost
  reasoning that way. Printed as tmp_refs, and a hard fail (F2).
- A reference is only ever a pointer. A page that copies prose, code or a screenshot has forked
  the truth (section 4).


## Archive

Two rules that look like one and are not (section 4.4):

- A superseded **page** is archived, never deleted: `Status: retired`, `SupersededBy` set to the
  successor, the file moved to archive/. Provenance survives because the file survives, and
  `tools/wiki-retire` prints retired = N of N with moved = N and deleted = 0 - a retire that
  deletes is a failure of the tool, not a shortcut (the rule is `TOOL-FORMAT.md`'s own).
- A superseded **reference** has no archive: `docs/raw/` is a queue, and ingest consumes its entry
  once the anchor has been carried into a page. Deleting a reference that no page cites is `F12`,
  a hard fail, and the reason wiki-retire prints a plan and refuses to act on an uncited id.

Retirement is the one op that writes, so it is the one op whose every step is a printed count, and
it never touches a page it was not pointed at.

## Budget

The budget for one page is 200 lines, measured by wiki-lint size, and it is stated here so the
number has one home: a page that outgrows it either splits along a claim boundary or the section
moves back into the module page it belongs to.

- The measure counts the file, front matter included, because front matter is what makes the file
  machine-readable and is not free.
- Over the budget is an offence, not a warning: length is how a page quietly becomes
  unfalsifiable, and an unfalsifiable page is worse than a short one (F3).
- The qmd rung of section 6 is **not built**. Navigation is `index.md` alone, regenerated by
  `tools/wiki-index` from front matter, which prints indexed = N of N. The wiki holds single-digit
  page counts today and target granularity (section 13) caps the ceiling near one page per qor_
  target over a 90-target tree. Revisit the rung when `wiki-census pages` prints a count so large
  that `index.md` stops being one screen per category - the trigger is a printed number, not a
  feeling.

## Ops

Five ops, one row each. A row states the order the op drains in, the single line that closes it, and
the failure mode of section 12 it exists to prevent: an op with no verdict line is an op no instrument can
close, and `wiki-lint schema` prints it as `schema_ops_no_verdict`. The keys below are the keys the
instrument prints, not a paraphrase of one.

| Op | Drain order | Verdict line | Guards |
|---|---|---|---|
| `ingest` | carry -> print -> delete | `pending=0 consumed=N carried=N uncarried=0 VERDICT fail=0` | F12, F9 |
| `refresh` | sweep -> dedupe -> append | `findings=N added=N skipped_duplicate=N VERDICT fail=0` | F8, F6 |
| `wiki-reconcile` | resolve -> flag -> stop | `anchors=R of T dead=0 tmp_refs=0 unpinned=0 stale=0 VERDICT fail=0` | F1, F6, F11 |
| `wiki-file` | index -> page -> target -> answer -> queue | `pages=N anchors=R of T stale=N queue_added=N VERDICT fail=0` | F3, F5 |
| `wiki-retire` | gate -> move -> stamp -> relink | `retired=N of N stamp_legs=3 of 3 deleted=0 VERDICT fail=0` | F5, F11 |

- The order in the `ingest` row is the whole guard for F12: the anchor is copied into the page before
  the page is written, and the page is written before its queue entry is deleted. A drain that deletes
  first cannot be repaired by any later step, which is why the order is a row value and not prose.
- `ingest` never schedules `wiki-lint` and never re-enters itself: the queue's own `added_by:` names
  the producer, and the hook ignores a diff that touches only `docs/wiki/` or `docs/raw/` (F9).
- `refresh` appends and never rewrites a page: a finding becomes a queue row, and only the reconcile
  sweep may set `Status: stale` (F6). `added = 0` on a run that printed `findings > 0` is the dedupe
  doing its work, not a failure; the failure is a finding that appears in neither count.
- `wiki-file` answers from the page and cites the anchor id, and when the wiki has no page for what
  the question needed it appends one `added_by: manual` entry instead of inventing a page (F3, F5).
  It reads `wiki-reconcile anchors` as a fact about freshness and never acts on it.
- `wiki-retire` is the only op that moves a file, so it is the only op whose refusal is structural:
  no subcommand takes a `docs/raw/` root as an argument, and `deleted = 0` is a printed row value
  rather than an intention.

