---
Type: Tool
Scripts:
  - File: tools/wiki-enqueue/wiki-enqueue.ps1
    Usage: "[-Commit <git-ref>] (default HEAD) -TreeRoot <dir> (required) -RawRoot <dir> (required) -Report <file> (required) [-Excludes \"p1,p2,...\"] (overrides the four default prefixes) [-DryRun]. <TreeRoot>/src is the only derived path - the set(target qor_*) declarations that map a changed file to its owning target. -Help prints the contract and touches no file."
    Output: "the -Report file and the same lines on stdout. Appends one nine-field entry per touched target to <RawRoot>/pending.md with added_by: hook, deduped against the queue, verified by re-reading (queued = N only when the queue grew by N). The git hook .git/hooks/post-commit calls this after every commit and always exits 0."
  - File: .git/hooks/post-commit
    Usage: "untracked git hook (sh): resolves the repo root and runs the tool over HEAD, report to tmp/wiki-enqueue-post-commit.txt. Not versioned by git - recreate it with the script text in this doc's Provenance if the clone is fresh."
    Output: "the tool's verdict lines appended to the report; exit 0 whatever happens, so hook silence is visible in the report, not in a failed commit."
---

# wiki-enqueue

## What it does

The queue half of `ingest-commit` (`docs/WikiDesign.md` §9(3), §9(3b), §14 item 13 - the item the
BuildLLMWiki tree omitted). One commit in: read its changed files, drop the excluded prefixes, map
every remaining file to the `set(target qor_X)` whose CMakeLists.txt owns its directory (nearest
declaration wins), and append one queue entry per touched target - `added_by: hook`, `path:` the
owning CMakeLists.txt, `anchor: #<target>`, `pin: commit:<sha>`. Queue, do not act: `wiki-ingest`
is the only consumer.

The exclusion guard is the whole anti-loop contract (§9(3b), §13.1(3), F9): a diff whose files all
sit under `docs/wiki/`, `docs/raw/`, `docs/future/` or `docs/archive/` enqueues nothing - the wiki's
own commits, plan trees and archived plans are not code churn - and a commit whose subject starts
`wiki:` enqueues nothing whatever it touched. Both prints name the guard that fired, so
`queued = 0` is distinguishable from a missed diff.

Candidate files owned by no qor target are printed as `UNMAPPED` and counted, never silently
enqueued as `target: none`.

## Contract

```
commit = cbffd54d10  subject = Fixed some issue with qor_workflow
files = 2  excluded = 0  candidates = 2
excluded_by_prefix: docs/wiki/=0  docs/raw/=0  docs/future/=0  docs/archive/=0
targets_touched = 1  [qor_workflow]
  QUEUE - id: hook-cbffd54d10-qor_workflow : qor_workflow (anchor src/framework/app/workflow/CMakeLists.txt#qor_workflow)
queued = 1  skipped_duplicate = 0  unmapped_files = 0  dry_run = 0  pending = 1
VERDICT fail=0
```

A wiki-only commit prints `exclusion_guard = ...the guard working, not a missed diff`; a
`wiki:`-prefixed one prints `loop_guard = subject starts wiki:`; an empty or merge diff prints a
note, never a fake zero. Missing/unreadable args: `USAGE`, `MISSING args=<name>`,
`VERDICT fail=1`, exit 1, nothing written. A queue that did not grow by `queued` is a FAIL -
an append that did not land is never reported as one.

Red cases proven on the real tree (18 Nov 2026): `15310e9e` (docs/wiki only) → guard fires
`queued = 0`; `e4835ef3` (`Wiki:` subject, 177 files) → loop guard, 171 counted under the four
prefixes; rerun over `cbffd54d` after the append → `skipped_duplicate = 1`; the appended entry
re-resolves under `wiki-reconcile anchors` (`queue_anchors = 1 of 1  queue_dead = 0`) and parses
under `wiki-census pending` (`pending = 1 of 1`).

## Provenance

Written 18 Nov 2026 to close the gap the archived `docs/archive/BuildLLMWiki/plan.md` left: its
tree landed every §14 item except item 13, the staleness gate + post-commit hook. Reuses the shared
shape (`wiki-census/wiki-shape.ps1`): `Get-WikiTargetDeclaration`, `Get-WikiQueueEntry`, the
nine-field append shape from `wiki-reconcile refresh`. Hook file is untracked by git; its text
lives in this doc and in the hook's own header. Drain it with `wiki-ingest`; `pending = 0` plus
`wiki-reconcile` green is the only claimable state.
