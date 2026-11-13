---
Type: Plan
Title: "Endpoint return semantics"
Parent: "../plan.md"
Level: 1
Items:
  - Title: Verify the doc refs
    Status: Atomic
    Content:
  - Title: Fill the small TU region slice
    Status: Refined
    Content: ./FillSmallTURegionSlice/plan.md
  - Title: Fill the large TU region slice
    Status: Refined
    Content: ./FillLargeTURegionSlice/plan.md
  - Title: Fill the window slice
    Status: Refined
    Content: ./FillWindowSlice/plan.md
  - Title: Fill the accelerator rows
    Status: Atomic
    Content:
  - Title: Audit the verdicts
    Status: Atomic
    Content:
---

# Endpoint return semantics

Slug: `EndpointReturnSemantics` - from title "Endpoint return semantics" (no drop-words, PascalCased, 22 chars, ASCII).

## Goal

Fill the verdict triple (`verdict | checkClass | docQuote`) for **all 597** rows of `tmp/_tmp_u2_endpoint_table.csv` - each row ending as `checked` with a named class, or `unchecked` with the documented reason its failure value is also a legal answer, or `void` - so that the code batches read a decision instead of a page.

## Resume

Cursor: **terminal** - all 6 items closed, item 6 **Audit the verdicts** (`Atomic`) **CLOSED 12 Nov 2026** as the last of them and the table now complete at **597 of 597** rows carrying the verdict triple (`checked 352 / unchecked 236 / void 9`); the audit `tmp/_tmp_u2_verdict_audit.ps1` -> `.out.txt` re-reads at **fail=0** with its three stop-condition counts at **597 / 0 / 0** (verdict rows / class-base mismatches / docQuote texts absent from `tmp/_tmp_u2_docquotes.txt`), with `typedef-alias matches = 0` meaning every `checked` row's class base is spelled exactly the row's `sdkRetType` (the **125** `NO-PROTOTYPE` and blank-`sdkRetType` rows matched against `pubRetType`), and `rows with no page quote = 23` being the 13 no-Learn-page rows plus the 9 `void` rows plus 1 row whose page states its Return value section as the single word `None`. Items 1-5 are **CLOSED 12 Nov 2026**, item 5 **Fill the accelerator rows** being the last of them and the table now complete at **597 of 597** rows carrying the verdict triple (`checked 352 / unchecked 236 / void 9`, `class-shape violations = 0`, `filled rows with no docQuote = 0`). Item 3 **Fill the large TU region slice** is closed at all **382** rows over `./FillLargeTURegionSlice/plan.md` (L1 90, L2 153, L3 139), item 4 **Fill the window slice** at **86**, item 2 **Fill the small TU region slice** at **124**. Item 1 **Verify the doc refs** is **CLOSED 11 Nov 2026** (`tmp/_tmp_u2_docref_probe.txt`: `probed = 597 of 597`, `resolved = 474`, `no-page = 123` of which 110 carry a corrected slug and 13 have no Learn page; `tmp/_tmp_u2_docref_verify.ps1` re-reads at **fail=0**, `cache rows = 584`). Inside item 2, batches S1-S5 are **CLOSED** (11 Nov 2026 for S1/S2, 12 Nov 2026 for S3/S4/S5): **124** of the region's 124 rows carry the verdict triple over 33 `full` TUs (`tmp/_tmp_u2_batch_counts.txt`: S1 10 / S2 18 / S3 27 / S4 33 / S5 36, region `checked 79 / unchecked 42 / void 3`), and the table itself re-reads at `class-shape violations = 0` / `fail=0` (`tmp/_tmp_u2_patch.out.txt`). Inside item 3 the large region is closed at all **382** rows (L1 90 / L2 153 / L3 139) over 92 slices, and the folder-wide table re-reads at `tmp/_tmp_u2_folder_audit.ps1` `rows=597 filled=597 checked=352 unchecked=236 void=9 sum=597 class-shape violations=0`. Terminal plans in this plan: **Fill the accelerator rows**, **Audit the verdicts** (both `Atomic`).

## Context

The table is already generated: `tmp/_tmp_u2_endpoint_table.ps1` (recreate by slug) joins `tmp/_tmp_u2_site_census.csv` (597 rows), `tmp/_tmp_u2_sal_map.txt` (the SAL prefilter - **1** of 597 stems annotated) and a `User32::` reference scan (396 rows at 0 callers, 201 nonzero). The verdict columns are empty. The doc column is a URL per row and the pattern is verified end-to-end (`nf-winuser-getclassname`, `nf-winuser-getdlgctrlid`, `nf-winuser-shutdownblockreasondestroy` all resolve), so a fetch batch is a straight read of that column.

Row arithmetic measured off the disk by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` (recreate by slug): **597** rows = small region **124** (S1 10 / S2 18 / S3 27 / S4 33 / S5 36) + L1 **90** + L2 **153** + L3 **139** + `window.cpp` **86** + `accelerator.cpp` **5**. The 5 accelerator rows are in **no** U1 batch - the U1 tree's batch arithmetic covers 592, and `accelerator.cpp` is the gap the earlier prose called "the remaining 592 sites". 5 rows are already decided in source (`MessageBeep`, `AttachThreadInput`, `GetGuiResources`, `UserHandleGrantAccess` plus D51's `ShutdownBlockReasonDestroy` - all inside the small region) and need no fetch.

## Decisions

The slice contract every leaf in this subtree obeys - children point here, they do not restate it.

- **Leaf budget**: one leaf = one slice of at most **10** rows = one `fetch_web_content` batch of at most 10 URLs. Boundaries are fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` (`claimed = 597 of 597`, every row owned by exactly one item, 92 slices); prose counts are not the authority.
- **Fetch**: one query per URL asking for the Return value section verbatim; append the page text to `tmp/_tmp_u2_docquotes.txt` (the audit re-reads this file, so it is load-bearing), then write the triple into the row and re-read the row before moving on. A leaf may fetch **only** URLs the `Verify the doc refs` item marked `resolved` - a leaf never re-derives a slug.
- **Verdict per row**: exactly one of `checked` + a class named from `returncheck.h`, `unchecked` + the reason the documented failure value is also a legal answer, or `void`. The reason quotes the page.
- **Stop condition per leaf**: re-read the slice's rows and confirm each carries the triple, and that the count of rows with a verdict for that TU equals that TU's row count in `tmp/_tmp_u2_slice_stems.txt`. A slice with an empty verdict column is not closed.
- **Never re-decide the 5 seeds**: `MessageBeep`, `AttachThreadInput`, `GetGuiResources`, `UserHandleGrantAccess`, `ShutdownBlockReasonDestroy` - their rows are inside the counts and are filled from source precedent, not from a page.
- **`accelerator.cpp`**: 5 rows in no batch of the U1 tree; they are filled by the parent's own `Fill the accelerator rows` item (1 slice) so the table reaches 597 rather than 592.
- **Class base**: a `checked` row's class base must equal the row's `sdkRetType`, never the published spelling (`LRESULT`/`HWND`/`HDWP`/`AW` rows). Exception measured at this write: the **125** rows whose `sdkRetType` is `NO-PROTOTYPE` have no SDK type to match - the installed `um\WinUser.h` carries no prototype for them, so the only type the wrapper can hold is the published spelling, and the audit treats `NO-PROTOTYPE` as `pubRetType`.
- **Caller counts**: a row with nonzero in-tree callers must carry its count in `notes` before any code lands it (`registers/u19.md` stays the authority for the call-site list); zero-caller rows are change-safe.
- **Verified input**: a fill leaf consumes only `msdnDocRef` URLs the probe marked `resolved`. A row probed `no-page` is filled once, from the probe log's corrected slug, by a later leaf - never by guessing inside a fetch leaf. This is the tightened gate's input rule: the old shape spent the first step's budget on 404s.

## Verify the doc refs

The input gate every fill leaf depends on, and it is one unit of hand work - one instrument, not 597 lookups. Write and run `tmp/_tmp_u2_docref_probe.ps1` (the file does not exist yet; this item creates it): it walks the `msdnDocRef` column of all **597** rows, asks each URL once whether the page resolves, and writes per row `resolved` or `no-page` to `tmp/_tmp_u2_docref_probe.txt`, listing any `no-page` row with the corrected slug where the stem's page lives under a different name. Stop condition: the log prints `probed = 597 of 597` and a `no-page=N` count, and the audit re-reads that pair. A row the probe marks `no-page` is filled exactly once, from the probe log's corrected slug, by the item that owns that row - no fill leaf ever guesses a slug. This is the tightened gate's input rule: in the old shape the first step spent part of its budget on 404s, which is why it was not a step at all.

## Fill the small TU region slice

**124** sites over the 33 small TUs in the S1-S5 order of the U1 tree. Child plan: `./FillSmallTURegionSlice/plan.md` (Level 1 -> 2, 5 items: the batches S1-S5, each `Refined` with its own Level 3 plan of one item per TU). No TU here exceeds 8 rows, so every item is one fetch batch.

## Fill the large TU region slice

**382** sites over L1 (90) / L2 (153) / L3 (139) in the same order. Child plan: `./FillLargeTURegionSlice/plan.md` (Level 1 -> 2, 3 items L1/L2/L3, each `Refined` with a Level 3 plan of one item per TU, and every TU of 11+ rows carrying its own Level 4 plan of slices). The `BOOL`-heavy TUs (`dialog`, `message`, `painting`, `misc`, `pointer`, `keyboard`, `strings`, `menu`) are where the "FALSE is an answer" rejections concentrate, so those rows mostly end `unchecked` with a reason rather than a class.

## Fill the window slice

**86** sites in `window.cpp` - the densest TU and the one with the most nonzero caller counts. Child plan: `./FillWindowSlice/plan.md` (Level 1 -> 2, 3 items cut by documented return family: `window BOOL rows` **55** rows / 6 slices, `window HWND rows` **17** rows / 2 slices, `window other rows` **14** rows / 2 slices - the family decides the verdict shape, so the cut is by family not by row count). Every `checked` verdict here must carry the caller audit note from `registers/u19.md` before the code batch lands it.

## Fill the accelerator rows

**5** rows of `accelerator.cpp` that sit in no U1 batch and are therefore invisible to the batch arithmetic: `CopyAcceleratorTable`, `CreateAcceleratorTable`, `DestroyAcceleratorTable`, `LoadAccelerators`, `TranslateAccelerator`. One fetch batch of 5, then write those 5 rows per the contract above. This item exists so the audit's row count reaches 597 rather than 592.

**CLOSED 12 Nov 2026** - one fetch batch of 5 URLs, `tmp/_tmp_u2_ac_fetch.ps1` -> `tmp/_tmp_u2_ac_fetch.out.txt`: 4 `CORRECTED` legs (the probe's `no-page` rows whose pages live under the suffixed ANSI-twin slugs `nf-winuser-copyacceleratortablea`, `nf-winuser-createacceleratortablea`, `nf-winuser-loadacceleratorsa`, `nf-winuser-translateacceleratora`) plus 1 clean `resolved`, all at `http=200` and all `match-cache`; `fetched=5 of 5 bad=0 no-page=0`, `VERDICT fail=0`. The 5 verdicts (`checked 3 / unchecked 2 / void 0`) are in `tmp/_tmp_u2_decisions.txt` and written into the table by `tmp/_tmp_u2_patch.ps1`: `CreateAcceleratorTable` and `LoadAccelerators` take `CheckNonZeroHandle< HACCEL >` (the pages reserve NULL and the handle is what the caller hands to `TranslateAccelerator`/`DestroyAcceleratorTable`; `sdkRetType NO-PROTOTYPE` so the base is the published `HACCEL`, the same class the `HICON`/`HDWP`/`HWND` rows use - no new check class), `TranslateAccelerator` takes `CheckNonZero< int >` (plain status int with zero reserved; the Remarks put the translation marker in the *sent* message's `wParam` high-order word, not in the return), `CopyAcceleratorTable` is `unchecked` because the page documents no failure value at all ("the return value specifies the number of accelerator-table entries ... or ... the number of accelerator-table entries that were copied" - the page's own purpose line is "to determine the size of the accelerator-table data", so the count is the answer), and `DestroyAcceleratorTable` is `unchecked` on its middle clause - "if the table has been loaded more than one call to LoadAccelerators, the function will return a nonzero value only when DestroyAcceleratorTable has been called an equal number of times" - so a legal partial destroy of a multiply-loaded table answers zero, and keying `BoolCheck` would report that legal outcome as a failure; that row carries 1 in-tree caller in `notes`.

The table is now complete: `tmp/_tmp_u2_folder_audit.ps1` prints `folder rows = 597  filled = 597  checked = 352  unchecked = 236  void = 9`, `sum of three verdicts = 597`, `class-shape violations = 0`, `filled rows with no docQuote = 0`. `accelerator.cpp` re-reads at `tmp/_tmp_u2_tu_state.ps1 accelerator.cpp` `rows=5 checked=3 unchecked=2 void=0 blank=0 shape_bad=0`.

## Audit the verdicts

One pass over the filled table before any code lands: every `checked` row names a class whose `AutoErrorCheck< >` base equals the row's `sdkRetType` (not the published spelling), every `docQuote` is verbatim from a fetched page rather than a paraphrase, and the row count with a verdict equals **597**. Stop condition: the audit re-reads the table and prints the three counts - rows carrying a verdict, rows whose class base mismatches, quotes absent from `tmp/_tmp_u2_docquotes.txt` - at 597 / 0 / 0. The audit is what keeps a wrong check from becoming a live runtime behaviour change under G6.

