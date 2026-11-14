---
Type: Plan
Title: "Source comment shadow tree"
Parent: ""
Level: 0
Items:
  - Title: Pin the comment and shadow-doc rules
    Status: Atomic
    Content:
  - Title: Extract the record before deleting
    Status: Atomic
    Content:
  - Title: Author the shadow docs
    Status: Refined
    Content: ./ShadowDocs/plan.md
  - Title: Delete the runs from the sources
    Status: Atomic
    Content:
  - Title: Folder gate after the sweep
    Status: Atomic
    Content:
  - Title: Records and commit
    Status: Atomic
    Content:
---

# Source comment shadow tree

Slug: `SourceCommentShadowTree` - from the request title "Source comment shadow tree" (no drop-words, PascalCased, ASCII, 23 chars, inside the 24-char cap; no collision with `User32ReturnCheckPass` or `User32ErrorHandlerPass`). Uniqueness is stored here, not recomputed.

## Goal

End with **no comment run longer than 2 lines anywhere in `src/platform/os/windows/api_layer/user`** and with the reasoning those runs carried preserved in a shadow doc tree under `docs/src/` - one markdown file per source file, found by path convention rather than by a reference written into the source.

## Resume (state slot - the only mutable section of this plan)

Cache, not record: the front matter owns structure, the disk owns truth, this slot only makes the resume cheap. Read the front matter and this slot, re-derive from disk, rewrite this slot if they disagree. Rotate in place - never append below this block.

Cursor: item 1 **Pin the comment and shadow-doc rules** (`Atomic`) - this plan's first action. Nothing has been executed: `docs/src` does not exist, `docs/agent/COMMENT-FORMAT.md` does not exist, and the 579 comment runs measured below are still in the sources. Terminal: all 6 items `Atomic` except item 3 `Refined` with `./ShadowDocs/plan.md` on disk.

## Decisions (locked - from the request, not re-derived)

1. **Delete the runs outright from the sources.** The value in them is real but does not belong in code.
2. **Remove from any comment that survives:** references to `tmp/` artifacts, caller-audit information, line counts, and references to the completed plan (`User32ReturnCheckPass`). Maintaining such references across thousands of files is untenable, and every `tmp/` pointer in the tree is already dead - `tmp/` was emptied (3,878 files) on 12 Nov 2026.
3. **Keep in a comment that survives:** the decision made, and what the function does. Nothing else.
4. **Refer to a function by namespace + class + function name** so the reference is unique in the workspace. Measured spelling for this module: `keyboard.cpp` opens `namespace qor { namespace win { namespace api {`, so the form is `qor::win::api::<Class>::<Function>`.
5. **Shadow tree:** mirror the source-root-relative path under `docs/`, basename with the extension's dot replaced by `_`. Measured pair establishing the rule: `src/platform/os/windows/api_layer/user/accelerator.cpp` -> `docs/src/platform/os/windows/api_layer/user/accelerator_cpp.md`. The source carries no pointer to its doc - the convention is the pointer.
6. **Pin the convention** in `docs/agent/COMMENT-FORMAT.md` and reference it from `memory-bank/codingConventions.md` (which today states only `codingConventions.md:36`: "More than 2 lines is not a comment it belongs in documentation. Source code is not a log of work. Logs belong in docs/past/logs").
7. **Persist a plan only for now** - this plan is the deliverable of the current turn.

## Context

Inherited from `User32ReturnCheckPass` (closed 12 Nov 2026, commit `d2031a93`): that pass added 352 live `CheckReturn< >` handlers and, alongside them, the work-log comments this plan exists to remove.

Measured starting state (12 Nov 2026, whole `src/platform/os/windows/api_layer/user`):

- **54 `.cpp` files, every one with a comment run > 2 lines**; 53 of 54 carry the pass marker `Return policy (U2`.
- **574 comment runs > 2 lines in the `.cpp` set**; classified by content: **566 are this pass's work-log blocks** (match `U2` / `tmp/` / `AUDIT` / `table row`), **8 are not** - the sweep must classify per run, not per length, or it deletes pre-existing descriptive comments the decisions say to keep.
- **1 header file (`user32.h`) with 5 runs > 2 lines** - 579 runs and 55 shadow docs in total.
- Worst offenders: `window.cpp` 51 runs, `strings.cpp` 35, `keyboard.cpp` 34, `menu.cpp` 32, `pointer.cpp` 32, `painting.cpp` 26, `message.cpp` 24, `misc.cpp` 23, `dialog.cpp` 23, `dpi.cpp` 22, `clipboard.cpp` 21, `desktop.cpp` 19 (those 12 files hold 342 of the 574; the other 42 hold 232).
- `d2031a93` itself: 5,118 inserted lines, 4,151 of them comments (81%), 967 code - **11.1 comment lines per handler site**, worst single run 67 lines.
- `docs/src` does not exist; nor does `docs/past` - the `docs/past/logs` location named by `codingConventions.md:36` is not on disk (dirs under `docs/`: `agent`, `archive`, `articles`, `future`, `learning`, `skills`, `ui`).

## Pin the comment and shadow-doc rules

Write `docs/agent/COMMENT-FORMAT.md`: the 2-line cap, what a surviving comment may and may not contain (decision + what the function does; no `tmp/` refs, no caller audit, no line counts, no plan refs), the function-reference spelling (`qor::win::api::<Class>::<Function>`), and the shadow-tree naming rule with the measured `accelerator.cpp` -> `accelerator_cpp.md` pair. Then add the one-line reference under the `## Comments` heading of `memory-bank/codingConventions.md`. Stop condition: both files re-read with the rule text present and `codingConventions.md` still under its authoritative-source header (`docs/skills/QORconventions.md` wins on conflict).

## Extract the record before deleting

Extend `tools/sanity/` (do not clone a new instrument - the tooling rule in `memory-bank/tools.md` says refine the indexed tool): sweep the 55 source files, classify each run > 2 lines as work-log or descriptive, and emit per-source-file fragments holding the run text verbatim plus its file and the nearest following declaration. The fragments are the record the shadow docs are built from; they exist so that nothing is lost before the deletion step runs. Stop condition: fragment count equals 566 work-log runs and the 8 unclassified runs are printed for a decision, none silently dropped.

## Author the shadow docs

Refined - see `./ShadowDocs/plan.md`. Each shadow doc holds, per function in its source file, the decision and the evidence that the deleted run carried (docQuote verbatim, the SDK failure value, why that value is or is not legal here), written as documentation about the function - not as a dated work log and not as a pointer into `tmp/` or into the plan tree. Function entries are keyed by the qualified name (`qor::win::api::<Class>::<Function>`).

## Delete the runs from the sources

With the shadow docs on disk, delete the 566 work-log runs from the 55 source files. A run is deleted only if it is work-log classified; the 8 unclassified runs keep their own decision. Where a run carries a decision the decisions block says to keep, replace it with at most 1 line naming the decision and the qualified function - never extend an existing comment (`codingConventions.md:36`). Stop condition: the sweep re-reads with **0 runs > 2 lines** in the 55 files and the `CheckReturn` token count unchanged at its pre-sweep value.

## Folder gate after the sweep

Re-run the U2 folder gate on the swept tree: the gate counts `CheckReturn` tokens, not comments, so it must report the same live-check count and `real-diag-lines=0` as before the sweep. Any change means the sweep touched code, not comments - stop and revert.

## Records and commit

Update `src/platform/os/windows/api_layer/user/status.md` and `completed_work.md` with the measured after-state (runs > 2 lines = 0, shadow docs = 55) and the register `registers/u2.md` with a one-line note that the per-site why now lives in the shadow docs. Commit the source deletions and the shadow docs in one commit so the tree is never in a state where the why exists in neither place.

## Problems foreseen (why this plan is ordered the way it is)

1. **Ordering is load-bearing:** extract -> author -> delete. The 566 work-log runs are the *only* surviving copy of the per-site reasoning (the `tmp/` table they cite was deleted), so any deletion before extraction loses it.
2. **Length-based deletion is wrong:** 8 of 574 runs are not this pass's blocks and may be descriptive comments the decisions say to keep. Classification must be per run.
3. **Two doc locations now compete:** `codingConventions.md:36` sends logs to `docs/past/logs` (a folder that does not exist on disk) while this plan puts per-file documentation in `docs/src/...`. `COMMENT-FORMAT.md` must state the split explicitly - shadow doc = documentation about a file; `docs/past/logs` = chronological work record - or the shadow tree will read as a log and be stale by the same rule it replaces.
4. **Naming is untested at scale:** 55 sources -> 55 md names must be asserted unique by the sweep (`user32.h` -> `user32_h.md`; same-basename files in different folders are disambiguated by the mirrored path). A collision would silently overwrite a doc.
5. **The volume is script work, not hand work:** 579 runs and 55 files. The sweep must extend `tools/sanity/` rather than clone another instrument - cloning is where this workspace's recurring instrument bugs came from.
6. **Scale of the convention beyond this module:** the rule is being pinned from a 55-file sample in `api_layer/user`. Other modules may hold long comments that are pre-existing and deliberate; the plan's gate is scoped to this module, and widening it is a separate decision, not a side quest.
7. **The shadow docs re-create the deleted record's cost:** 566 entries of prose is roughly the 4,151 comment lines relocated, not removed. If the goal were only "smaller sources", the cheaper option is deleting the why outright; this plan keeps it because the reasoning is what makes the 236-odd unchecked verdicts defensible.

