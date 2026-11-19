# Queue - docs/raw/pending.md

One entry per reference awaiting ingestion. `docs/WikiDesign.md` §4 and §4.4 own this design. The
layer is a **queue**, not a stock: `wiki-ingest` carries an entry's anchor into a page and then
**deletes** the entry. Nothing here is archived, because an entry is a pointer into something git
already versions - the page that consumed it is its archive. An empty queue is the legal steady
state, and the only state in which the wiki may claim to be current.

## Hard rules

- Pointers and annotations only. No copied prose, no pasted code, no screenshots, no summaries. An
  annotation longer than three lines is a `docs/wiki/` page that cites the entry instead.
- No `tmp/` anchor. `tmp/` is gitignored and emptied per task, so an anchor into it resolves to
  nothing exactly when it is needed - §4.3 rule 3, and `memory-bank/gotchas/` holds the scar tissue.
- Relative paths only, and a named anchor beats a numeric one: `path#symbol` may stand alone,
  `path:120-131` may not - §4.3 rule 1.
- This file never records its own entry count. `tools/wiki-census` prints it as `pending = N of N`,
  and prints `0 of 0` as the vacuity it is rather than as a pass.

## Entry shape - nine fields, in this order

    id:         unique slug. It survives into the consuming page's `Sources:` and into the
                append-only `docs/wiki/log.md` line for that ingest, which is what makes deleting
                the entry survivable - §4.4 consequence 4.
    added_by:   hook | manual | learning | refresh | reconcile - the five producers, named below.
    kind:       in-tree | out-of-tree | event | artifact. Decides how `wiki-ref-check` re-resolves
                the anchor: re-open, HEAD + fingerprint, re-read, re-generate - §4.2.
    path:       repo-relative file, or the URL when `kind` is out-of-tree. Never absolute.
    anchor:     where inside `path:` - `#symbol`, `:line-line`, a `commit:sha`, a `D<n>`, a CMake
                target id. The thing re-resolution looks for.
    pin:        the version the reference implies: SDK triple, `_WIN32_WINNT`/`NTDDI_VERSION`,
                toolchain build, git sha. What makes an out-of-tree answer reproducible - §4.3 rule 4.
    verified:   YYYY-MM-DD the anchor was last re-resolved, by a human or an instrument - not the
                day the entry was written.
    target:     the QOR lib build target the entry is about: the unit of wiki work - §13 Q2. `hook`
                entries carry the targets their commit touched; `none` is reserved for references no
                target owns, and is a counted case, not an unchecked one.
    why:        at most three lines: what this reference is good for and what it would settle.

Nine fields, not §4.2's ten: `path:` and `anchor:` split what §4.2 wrote as one `anchor:`, `why:`
replaces `use_for:` and `notes:`, and `target:` is what target granularity demands. `judge:` and
`closes:` are page-side - a carried entry's judge is the consuming page's `Judges:` line, and its
gap row is written by the op that consumed it. This is the shape the six instruments parse;
`docs/wiki/schema.md` holds the page rules and does not restate this list.

## Producers - five, all writing the same shape

    hook        The post-commit hook: diff -> targets -> one entry per touched target. It must
                exclude `docs/wiki/` and `docs/raw/` from the diff, or the ingest's own `wiki:`
                commit re-enqueues forever - §9 3b, F9. The note in `.gitignore` records this.
    manual      A human drops a reference, including `wiki-file` when the wiki is asked something it
                has no page for: queue an entry, never invent a page.
    learning    The `docs/learning/` track supplies an anchor on request. The two catalogs never
                sync - §13 Q4 - so this is the only channel between them.
    refresh     `wiki-reconcile refresh`: one entry per dead anchor or stale judge. It appends here
                and never edits a page.
    reconcile   `wiki-reconcile`'s coverage gap: a changed target with no page. What §9 3b called
                `gapfill` splits across `refresh` and this.

## Consumer - one

`wiki-ingest` drains the queue, and its drain order is the contract because it is what makes a
deletion safe: **carry** the entry's `path:`/`anchor:`/`pin:`/`verified:` into the page, **print**
the page written, **delete** the entry - in that order, never deleting before the carry. It prints
`pending=0 consumed=N carried=N uncarried=0 VERDICT fail=0`, and `uncarried>0` is data loss.

## Parsing

An entry is nine lines whose first is `- id:` at column 0. Nothing else in this file may start that
way, which is what makes `Select-String -Pattern '^- id: '` the entry count.

## Queue

<!-- Entries are appended below this line. An empty section is the steady state, not a missing
     file. The count belongs to `tools/wiki-census pending`, never to prose in this file. -->
- id: hook-cbffd54d10-qor_workflow
  added_by: hook
  kind: in-tree
  path: src/framework/app/workflow/CMakeLists.txt
  anchor: #qor_workflow
  pin: commit:cbffd54d10152cc94089b76550cb691b5414c725
  verified: 2026-11-18
  target: qor_workflow
  why: post-commit hook on cbffd54d10 touched 2 file(s): src/framework/app/workflow/iworkflow.h, src/framework/app/workflow/workflow.cpp
