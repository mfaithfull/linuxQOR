---
Type: Plan
Title: "Small TU batch S1"
Parent: "../plan.md"
Level: 3
Items:
  - Title: bitmap rows
    Status: Atomic
    Content:
  - Title: brush rows
    Status: Atomic
    Content:
  - Title: console rows
    Status: Atomic
    Content:
  - Title: printing rows
    Status: Atomic
    Content:
  - Title: devicemanagement rows
    Status: Atomic
    Content:
  - Title: resource rows
    Status: Atomic
    Content:
  - Title: security rows
    Status: Atomic
    Content:
---

# Small TU batch S1

Slug: `SmallTUBatchS1` - from title "Small TU batch S1" (no drop-words, PascalCased, 14 chars, ASCII); the same folder name the sibling U1 tree uses, so the two passes re-census against one another. Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **10** table rows of the 7 smallest TUs, one item per TU, each item one fetch batch.

## Resume

Cursor: **all 7 items done** - this plan reached its terminal state on 11 Nov 2026. Measured at that write: all **10** rows carry the verdict triple (`tmp/_tmp_u2_filled.txt`: the 7 TUs read `rows=n filled=n full`), verdicts `checked 8 / unchecked 2` (`GetSysColorBrush` and `ConsoleControl` are the two written-reason rejections), every quote verbatim from `tmp/_tmp_u2_probe_cache.txt` and appended to `tmp/_tmp_u2_docquotes.txt`. Next in the parent: item 3 **Small TU batch S3**.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the grandparent's (`../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: Small TU batch S2, Small TU batch S3, Small TU batch S4, Small TU batch S5.

Rows and stems per item are listed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM <tu> slice 1`. Every item here is 1-2 rows, so one `fetch_web_content` call covers it and the 404 fallback leg can fire at most once per row.

## bitmap rows

1 row: `LoadBitmap`. Fetch its `msdnDocRef`, append the page's Return-value text to `tmp/_tmp_u2_docquotes.txt`, write the triple, re-read the row.

## brush rows

1 row: `GetSysColorBrush`. Same one-batch procedure, then write the triple for each row and re-read it.

## console rows

1 row: `ConsoleControl`. Same procedure, then write the triple for each row and re-read it.

## printing rows

1 row: `PrintWindow`. Same procedure, then write the triple for each row and re-read it.

## devicemanagement rows

2 rows: `RegisterDeviceNotification`, `UnregisterDeviceNotification`. Same procedure, both URLs in one fetch call, then write the triple for each row and re-read it.

## resource rows

2 rows: `CopyImage`, `LoadImage`. Same procedure, then write the triple for each row and re-read it.

## security rows

2 rows: `GetUserObjectSecurity`, `SetUserObjectSecurity`. Same procedure, then write the triple for each row and re-read it.