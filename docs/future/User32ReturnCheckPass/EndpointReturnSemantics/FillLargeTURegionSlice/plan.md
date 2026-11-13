---
Type: Plan
Title: "Fill the large TU region slice"
Parent: "../plan.md"
Level: 2
Items:
  - Title: Large TU batch L1
    Status: Refined
    Content: ./LargeTUBatchL1/plan.md
  - Title: Large TU batch L2
    Status: Refined
    Content: ./LargeTUBatchL2/plan.md
  - Title: Large TU batch L3
    Status: Refined
    Content: ./LargeTUBatchL3/plan.md
---

# Fill the large TU region slice

Slug: `FillLargeTURegionSlice` - from title "Fill the large TU region slice" (drop-word `the` removed, PascalCased, 22 chars, ASCII, inside the 24-char cap). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **382** table rows of the 18 large TUs (L1 90 / L2 153 / L3 139), densest-last, with every item capped at one fetch batch of **at most 10** URLs - which forces a second cut inside the TUs of 11+ rows.

## Resume

Cursor: **terminal** - all 3 items `Refined` with children on disk, all 3 closed. Item 3 **Large TU batch L3** is **CLOSED 13 Nov 2026** - all **139** rows of its 4 TUs carry the verdict triple (`checked 79 / unchecked 59 / void 1`: menu 31/8/0, keyboard 10/24/0, strings 10/24/0, pointer 28/11/0), over 14 fetch slices of at most 10 URLs at `VERDICT fail=0` with every `Return value` section `match-cache`; `./LargeTUBatchL3/plan.md` terminal at 4 closed `Refined` children (each terminal at its `Atomic` slice items). Item 2 **Large TU batch L2** is **CLOSED 12 Nov 2026** - all **153** rows of its 7 TUs carry the verdict triple (`checked 82 / unchecked 69 / void 2`: desktop 15/3/0, clipboard 10/10/0, dpi 6/15/0, dialog 13/9/0, misc 13/9/1, message 10/13/1, painting 15/10/0), over 19 fetch slices of at most 10 URLs at `VERDICT fail=0` with every `Return value` section `match-cache`; `./LargeTUBatchL2/plan.md` terminal at 7 closed `Refined` children. Item 1 **Large TU batch L1** is **CLOSED 12 Nov 2026** - all **90** rows of its 7 TUs carry the verdict triple (`checked 59 / unchecked 30 / void 1`), over 13 fetch slices of at most 10 URLs at `VERDICT fail=0` with every `Return value` section `match-cache`; `./LargeTUBatchL1/plan.md` is terminal at 7 closed items. Folder-wide at the L3 close: `tmp/_tmp_u2_folder_audit.ps1` reads `rows=597 filled=597 checked=352 unchecked=236 void=9 sum=597 class-shape violations=0 filled rows with no docQuote=0`. This plan is terminal: all 3 items `Refined` with children on disk.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the leaves obey is the parent's `## Decisions` - read it, do not restate it.** Sibling titles in the parent: Fill the small TU region slice, Fill the window slice, Fill the accelerator rows, Audit the verdicts.

Measured by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` (recreate by slug): the large region is **18 TU items = 48 fetch slices** of at most 10 URLs - L1 13 (mouse 10 -> 1 slice, rectangle 11 -> 2, scrollbar 12 -> 2, icon 14 -> 2, touch 14 -> 2, windowclass 14 -> 2, cursor 15 -> 2); L2 19 (desktop 18 -> 2, clipboard 20 -> 2, dpi 21 -> 3, dialog 22 -> 3, misc 23 -> 3, message 24 -> 3, painting 25 -> 3); L3 16 (pointer 32 -> 4, keyboard 34 -> 4, strings 34 -> 4, menu 39 -> 4). A TU of 11+ rows is **not** one item: it gets its own child plan, one item per slice.

## Large TU batch L1

90 rows over `mouse` 10, `rectangle` 11, `scrollbar` 12, `icon` 14, `touch` 14, `windowclass` 14, `cursor` 15. `mouse` (10 rows) is one item; the other six get a child plan each. Child plan: `./LargeTUBatchL1/plan.md` (Level 2 -> 3, 7 items: 1 `Atomic`, 6 `Refined`).

## Large TU batch L2

153 rows over `desktop` 18, `clipboard` 20, `dpi` 21, `dialog` 22, `misc` 23, `message` 24, `painting` 25 - every TU here exceeds the cap, so all 7 get a child plan. Child plan: `./LargeTUBatchL2/plan.md` (7 `Refined` items).

## Large TU batch L3

139 rows over `pointer` 32, `keyboard` 34, `strings` 34, `menu` 39 - the four densest TUs, 4 slices each; last so the folder's check shape is proven on cheaper TUs first. Child plan: `./LargeTUBatchL3/plan.md` (4 `Refined` items).
