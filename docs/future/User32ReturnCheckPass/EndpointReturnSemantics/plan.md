---
Type: Plan
Title: "Endpoint return semantics"
Parent: "../plan.md"
Level: 1
Items:
  - Title: Fill the small TU region slice
    Status: Atomic
    Content:
  - Title: Fill the large TU region slice
    Status: Atomic
    Content:
  - Title: Fill the window slice
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

Terminal as planned: all four items are `Atomic` (each is one bounded pass over the named artifact `tmp/_tmp_u2_endpoint_table.csv` or one audit over it), so this plan holds no child and no frontier. The frontier for the whole tree sits in the **parent's** item order - a session executing this plan takes item 1 first, and the parent's ordering plus its "hard precondition" lines decide when this plan may run at all.

## Context

The table is already generated: `tmp/_tmp_u2_endpoint_table.ps1` (recreate by slug) joins `tmp/_tmp_u2_site_census.csv` (597 rows), `tmp/_tmp_u2_sal_map.txt` (the SAL prefilter - **1** of 597 stems annotated) and a `User32::` reference scan (396 rows at 0 callers, 201 nonzero). The verdict columns are empty. The doc column is a URL per row and the pattern is verified end-to-end (`nf-winuser-getclassname`, `nf-winuser-getdlgctrlid`, `nf-winuser-shutdownblockreasondestroy` all resolve), so a fetch batch is a straight read of that column.

The row count per slice matches the U1 tree's batch arithmetic, which is also the reason the code batches and the fetch batches stay aligned: the 5 pre-existing checked sites (`MessageBeep`, `AttachThreadInput`, `GetGuiResources`, `UserHandleGrantAccess` plus D51's `ShutdownBlockReasonDestroy`) are already decided, and the remaining **592** sites split small region 124 / L1 90 / L2 153 / L3 139 / `window.cpp` 86.

## Fill the small TU region slice

**124** sites over the 33 small TUs in the S1-S5 order of the U1 tree. For each slice: read its rows from the table, fetch the rows' `msdnDocRef` URLs in parallel `fetch_web_content` batches (one query per URL asking for the Return value section verbatim), then write per row: the failure value, whether it is also a legal success value, the class from `returncheck.h` or `none`, and the quote. `A`/`W` pairs bind one bare stem, so one page per row already covers the pair.

## Fill the large TU region slice

**382** sites over L1 (90) / L2 (153) / L3 (139) in the same order. Same procedure; the `BOOL`-heavy TUs (`dialog`, `message`, `painting`, `misc`, `pointer`, `keyboard`, `strings`, `menu`) are where the "FALSE is an answer" rejections concentrate, so those rows mostly end `unchecked` with a reason rather than a class.

## Fill the window slice

**86** sites in `window.cpp` - the densest TU and the one with the most nonzero caller counts (`GetDlgCtrlID` 8, `SetWindowPos` 5, `GetDC` 4, `ReleaseDC` 3). Every `checked` verdict here must carry the caller audit note from `registers/u19.md` before the code batch lands it.

## Audit the verdicts

One pass over the filled table before any code lands: every `checked` row names a class whose `AutoErrorCheck< >` base equals the row's `sdkRetType` (not the published spelling), every `docQuote` is verbatim from a fetched page rather than a paraphrase, and the row count with a verdict equals 597. The audit is what keeps a wrong check from becoming a live runtime behaviour change under G6.
