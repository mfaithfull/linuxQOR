---
Type: Plan
Title: "Fill the small TU region slice"
Parent: "../plan.md"
Level: 2
Items:
  - Title: Small TU batch S1
    Status: Refined
    Content: ./SmallTUBatchS1/plan.md
  - Title: Small TU batch S2
    Status: Refined
    Content: ./SmallTUBatchS2/plan.md
  - Title: Small TU batch S3
    Status: Refined
    Content: ./SmallTUBatchS3/plan.md
  - Title: Small TU batch S4
    Status: Refined
    Content: ./SmallTUBatchS4/plan.md
  - Title: Small TU batch S5
    Status: Refined
    Content: ./SmallTUBatchS5/plan.md
---

# Fill the small TU region slice

Slug: `FillSmallTURegionSlice` - from title "Fill the small TU region slice" (drop-word `the` removed, PascalCased, 22 chars, ASCII, inside the 24-char cap). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple (`verdict | checkClass | docQuote`) for the **124** table rows of the 33 small TUs, smallest-first, so that no item ever asks for more than one fetch batch of **at most 10** URLs.

## Resume

Cursor: this plan is **terminal** - all 5 items CLOSED (`Small TU batch S1` and `S2` on 11 Nov 2026, `S3` and `S4` on 12 Nov 2026, `S5` on 12 Nov 2026): all **124** rows of the small region over **33 full TUs** carry the verdict triple - `checked 79 / unchecked 42 / void 3`, and those are the table's only filled rows (`tmp/_tmp_u2_table_audit.ps1`: `total=597 filled=124 checked=79 unchecked=42 void=3`) (`tmp/_tmp_u2_filled.ps1` -> `tmp/_tmp_u2_filled.txt`: 33 `full` / 0 `partial` / 0 `empty`, `filled=124` of `rows=597`; `tmp/_tmp_u2_batch_counts.ps1` -> `tmp/_tmp_u2_batch_counts.txt` per-batch totals S1 10 / S2 18 / S3 27 / S4 33 / S5 36). Verdict-side verifies re-read at fail=0: `tmp/_tmp_u2_leaf_verify.txt` (S3 `tus=7 fail=0`), `tmp/_tmp_u2_batch_verify.txt` (S4 `tus=7 slice_total=33 rows=33 fail=0`), `tmp/_tmp_u2_patch.out.txt` (`class-shape violations = 0`, `decisions with no table row = 0`). The 7 filled rows with no page are the no-Learn-page rows the probe named (`CancelShutdown`, the legacy bucket) - each carries the probe's reason instead of a quote.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call, so a verdict is only as good as its page. **The slice contract the leaves obey is the parent's `## Decisions` - read it, do not restate it here.** Sibling titles in the parent: Fill the large TU region slice, Fill the window slice, Fill the accelerator rows, Audit the verdicts.

Measured by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` (recreate by slug): the small region is **33 items over 33 TUs = 33 fetch slices**; the largest small TU is 8 rows (`caret`, `devicecontext`), so **no small TU needs splitting** and one item = one TU = one fetch batch. Region row total 124 = S1 10 + S2 18 + S3 27 + S4 33 + S5 36, matching the U1 tree's ledger with 0 TU delta.

## Small TU batch S1

10 rows over `bitmap` 1, `brush` 1, `console` 1, `printing` 1, `devicemanagement` 2, `resource` 2, `security` 2 - one item per TU, each a single fetch batch. Child plan: `./SmallTUBatchS1/plan.md` (Level 2 -> 3, 7 `Atomic` items, terminal).

## Small TU batch S2

18 rows over `synchronization` 2, `timer` 2, `windowprocedure` 2, `buttoncontrol` 3, `combobox` 3, `coordtransform` 3, `filledshapes` 3. Child plan: `./SmallTUBatchS2/plan.md` (7 `Atomic` items, terminal).

## Small TU batch S3

27 rows over `listbox` 3, `accessibility` 4, `errorhandling` 4, `hook` 4, `legacy` 4, `mdi` 4, `powermanagement` 4. `errorhandling`'s `MessageBeep` row is decided in source and needs no fetch. Child plan: `./SmallTUBatchS3/plan.md` (7 `Atomic` items, terminal).

## Small TU batch S4

33 rows over `systeminfo` 4, `text` 4, `help` 5, `ime` 5, `monitor` 5, `processthread` 5, `windowproperties` 5. `processthread` carries 3 source-decided rows (`AttachThreadInput`, `GetGuiResources`, `UserHandleGrantAccess`). Child plan: `./SmallTUBatchS4/plan.md` (7 `Atomic` items, terminal).

## Small TU batch S5

36 rows over `systemshutdown` 6, `displayconfig` 7, `rawinput` 7, `caret` 8, `devicecontext` 8 - the 8-row TUs are the cap case: one fetch batch of 8 URLs. `systemshutdown`'s `ShutdownBlockReasonDestroy` row is D51's, decided. Child plan: `./SmallTUBatchS5/plan.md` (5 `Atomic` items, terminal).
