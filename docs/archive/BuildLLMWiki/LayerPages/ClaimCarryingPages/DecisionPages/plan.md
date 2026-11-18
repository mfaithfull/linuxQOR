---
Type: Plan
Title: "Decision pages"
Parent: "../plan.md"
Level: 3
Items:
  - Title: Probe the 55 D-entries
    Status: Atomic
    Content:
  - Title: Write the decisions folder purpose page
    Status: Atomic
    Content:
  - Title: Decision pages group 1
    Status: Refined
    Content: ./DecisionPagesGroup1/plan.md
  - Title: Decision pages group 2
    Status: Refined
    Content: ./DecisionPagesGroup2/plan.md
  - Title: Decision pages group 3
    Status: Refined
    Content: ./DecisionPagesGroup3/plan.md
  - Title: Decision pages group 4
    Status: Refined
    Content: ./DecisionPagesGroup4/plan.md
  - Title: Rebuild the index and close the pass
    Status: Atomic
    Content:
---

# Decision pages

## Goal

Fifty-six wiki pages, one per D-entry in `docs/agent/decisions/` (`D56` was added 18 Nov 2026 to
settle this pass's judge-schema question), each carrying the commit that
made its entry the only wiki page whose anchor is a commit rather than a symbol - plus the folder
purpose page, the probe that makes every page's inputs measured rather than invented, and the index
close.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `PASS CLOSED 18 Nov 2026 - all 56 pages landed (docs/wiki/decisions/D1.md..D56.md + purpose.md),
  Group1..Group4 all CLOSED; close-line: census `  decisions = 56`, wiki-index build
  `index_rows = 151 of 151 / unindexed = 0 / groups = 4`, ref-check anchors `516 of 516, dead = 0`
  (the D56 page carries NO anchor - its entry is uncommitted, anchor = NO-COMMIT, fill after the
  root Records and commit), schema `illegal_fields = 0`, freshness `stale = 0 / undated = 143
  informational`, every instrument VERDICT fail=0` -
- `Lesson recorded at Group4A close and re-applied at every Group4B leaf: a wrong-but-live commit
  anchor passes dead = 0 - each page copies the sha of its OWN probe row in tmp/_tmp_dp_probe.txt;
  judges are the entry's own recorded command (cl gate legs reproducible; tmp scripts/logs are
  provenance; command-less entries carry the D56 sentinel).`
- `History: probe 55 of 55 shas green (tmp/_tmp_dp_probe.ps1 -> .txt); 20 command-less entries found,
  resolved by the D56 sentinel (docs/agent/decisions/D56.md, uncommitted); the other 35 entries
  recorded their own verification line; purpose leaf landed first (purpose_files 5 -> 6).`
- **Probe slot** (`id | sha | verdict | candidate`, full sha per entry; head of `DECISIONS.md`
  index untouched): `D1-D8,D10,D11` share sha `8a538e49..751e9b87` (all judge-less); `D9`
  same sha, candidate `tmp/json_final_rel_*.log` (log, not command); `D12` `cmake --build
  cmakebuild --target winqor_api_shell --parallel 8`; `D13,D22,D23,D27-D30` `cl /EP`; `D21,D24,D26,
  D31,D32,D53` `/W4` leg; `D34,D37,D38,D51` `git diff` forms; `D40` `git status`;
  `D14-D20,D39,D46,D48-D50,D52,D54,D55` candidates are `tmp/` scripts/logs (recreate-by-slug
  provenance) - the gate line at `status.md:158-165` is the reproducible leg for the U-pass entries
  (D46-D55 family). Full table in `tmp/_tmp_dp_probe.txt`.
- **Supersession result (measured)** - **no D-entry supersedes another D-entry.** D50 reverses the
  29 Oct U1 verdict, which lives in `registers/u1.md`, not the ledger; D7 resolves an earlier
  hand-index assumption, also not a ledger entry; D18/D19/D26/D33 discuss superseded **SDK
  families**, and D40 corrects its own premise - none is a ledger supersession. Consequence: no
  decision page carries `SupersededBy:`; D50's and D7's pages state the reversal in prose against
  their registers/anchor.
- `Shape facts, measured 18 Nov 2026, governing every leaf` - `docs/agent/decisions/` holds
   **56** files `D1.md`..`D56.md` (`D56.md` uncommitted); `PageType: decision` is shipped vocabulary
  (tools/wiki-census/wiki-shape.ps1:33); `Decision:` / `Supersedes:` are not fields
  (wiki-shape.ps1:40-41), `SupersededBy:` is; required fields `Type, PageType, Title, Status,
  LastVerified, Judges` (wiki-shape.ps1:43); anchor
  `{ kind: in-tree, path: docs/agent/decisions/D<n>.md, anchor: commit:<probe sha>, verified: <date> }`
  resolves through `Resolve-WikiAnchor` (wiki-shape.ps1:423) via `git cat-file -e`, `dead` on a
  moved path - the history detector, no schema change.
- `Founding decisions unchanged` - slice by D-id; one page per D-entry; the commit anchor; groups
   are breadth-cap plumbing (56 leaves over four L4 groups x two L5 leaf-plans, cap 7 per plan;
   `Group4B` carries the 7th, `D56`).

## Context

Inherited at creation - re-read rather than trusted, the tree has moved since:

- **Parent goal**: write the pages that carry a falsification handle.
- **Parent locked decisions**: `docs/learning/` and `docs/wiki/` never sync; a claim page's
  `Judges:` are drawn from `docs/agent/verification.md`'s rule sections - for decision pages the
  judge is the entry's own recorded command (the gotcha precedent), never an invented one.
- **Worked-example siblings**: `GotchaPages` and `InvariantPages` (both `CLOSED` 18 Nov 2026) -
  leaf order (probe, purpose where the folder lacks one, one page per leaf, index close), the
  instrument invocation pattern, and the stop-pair style come from them.
- **Shape precedent**: `ComponentsLibPages` -> `ComponentsPagesGroup1..7` is the same one-page-per-
  leaf pattern under the breadth cap; this plan nests one level deeper only because 55 exceeds
  what L4 grouping alone can hold.

## Decisions

- **Slice by D-id, not by topic** - the ids are the artifact's only stable naming, and a topic cut
  would need a second instrument to prove it covered all 56.
- **One page per D-entry is the literal reading of §14 item 10** - flagged, not assumed; the cut is
  fixed by the same instrument either way.
- **The anchor is a commit, so the pin is a ref, not a path pin** - §6.2. A decision page is stale
  when the code it decided on has moved, and `wiki-reconcile history` - not `anchors` - detects
  that.
- **Groups are plumbing, not batches** - the L4/L5 nesting exists only because the breadth cap is 7;
  every leaf is still one page, and the stop line advances one decision at a time in D-id order
  across all eight leaf-plans (`decisions = 7` closes `Group1A`, `= 14` `Group1B`, ... `= 56`
  `Group4B`).

## Probe the 55 D-entries

Input verified: the 55 files measured above; `git` works in this tree.

One probe, no page writes: for each `D<n>.md`, record in the slot below - the entry's own commit sha
from `git --no-pager log -1 --format=%H -- docs/agent/decisions/D<n>.md` (each `sha` re-checked with
`git cat-file -e`), the entry's judge candidate - the command the entry itself records (its gate /
census / build line), or `none` with a named reason - and the supersession line: which entry (if any)
declares it superseded, so **that** page carries the legal `SupersededBy:` field and no page carries
`Supersedes:` (measured: not a schema field). D50 is known to reverse the 29 Oct leave-as-is verdict;
its probe line names the entry it reverses.

Stop condition printed 18 Nov 2026: `55 of 55` shas `git cat-file -e` green (`rows=55
sha_green=55`, `tmp/_tmp_dp_probe.ps1` -> `.txt`); `wiki-census pages` still prints `  decisions =
0`; the slot holds sha + judge candidate + supersession per id, with the **20 no-command entries**
named and the governance question they raise recorded as the pass blocker. Supersession came back
empty against the ledger - no page will carry `SupersededBy:`.

## Write the decisions folder purpose page

Input verified: probe slot filled; `docs/wiki/decisions/` absent.

One page: `docs/wiki/decisions/purpose.md`, the shape of `docs/wiki/gotchas/purpose.md` - why these
pages exist (a decision the tree can be held to), what makes one stale (its commit anchor: moved
path or absent sha), and that supersession is read from the D-entries' `SupersededBy:` chain, not
from prose.

Stop condition: `wiki-census pages` prints `purpose_files` moved up by one and `wiki-lint schema`
prints `illegal_fields = 0` with the new file.

## Decision pages group 1

See [`./DecisionPagesGroup1/plan.md`](./DecisionPagesGroup1/plan.md) - `D1`..`D14` in two leaf-plans
of 7, under the probe slot's per-id rows.

## Decision pages group 2

See [`./DecisionPagesGroup2/plan.md`](./DecisionPagesGroup2/plan.md) - `D15`..`D28` in two
leaf-plans of 7.

## Decision pages group 3

See [`./DecisionPagesGroup3/plan.md`](./DecisionPagesGroup3/plan.md) - `D29`..`D42` in two
leaf-plans of 7.

## Decision pages group 4

See [`./DecisionPagesGroup4/plan.md`](./DecisionPagesGroup4/plan.md) - `D43`..`D49` and
`D50`..`D56` in two leaf-plans of 7; the D50 reversal lives here.

## Rebuild the index and close the pass

Input verified: `Group4B` closed on `  decisions = 56`.

One generator run and the slot writes: `wiki-index build` (generated file, op 12 - never edited);
expected `groups = 4` (`decision` joins `gotcha`, `invariant`, `module`) and `index_rows` equal to the
census `pages` at that moment; then `wiki-ref-check anchors` re-read - the new commit anchors must
print `dead = 0`, and the D56 page may land without an anchor while its entry is uncommitted (its
leaf prints `anchor = NO-COMMIT`); `wiki-lint schema` must print `illegal_fields = 0` with the
sentinel judges landed - and `wiki-lint freshness` re-read (the judges name files only where they
are real commands; sentinel pages print `undated`, informational). This plan's close line goes into
this slot, and the parent's.

Stop condition: `wiki-census pages` prints `  decisions = 56`; `wiki-index build` prints
`index_rows = <pages of pages>` with `unindexed = 0`; `wiki-ref-check anchors` prints `dead = 0`;
each instrument `VERDICT fail=0`.
