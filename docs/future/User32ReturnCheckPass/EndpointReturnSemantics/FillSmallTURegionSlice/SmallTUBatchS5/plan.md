---
Type: Plan
Title: "Small TU batch S5"
Parent: "../plan.md"
Level: 3
Items:
  - Title: systemshutdown rows
    Status: Atomic
    Content:
  - Title: displayconfig rows
    Status: Atomic
    Content:
  - Title: rawinput rows
    Status: Atomic
    Content:
  - Title: caret rows
    Status: Atomic
    Content:
  - Title: devicecontext rows
    Status: Atomic
    Content:
---

# Small TU batch S5

Slug: `SmallTUBatchS5` - from title "Small TU batch S5" (no drop-words, PascalCased, 14 chars, ASCII); the sibling U1 tree's name. Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **36** table rows of these 5 small TUs, one item per TU, each item one fetch batch (the cap case: 8 URLs).

## Resume

Cursor: this plan is **terminal** - all 5 items CLOSED **12 Nov 2026** (`systemshutdown rows`, `displayconfig rows`, `rawinput rows`, `caret rows`, `devicecontext rows`): all **36** rows of the batch carry the verdict triple - **checked 34 / unchecked 2 / void 0** (`tmp/_tmp_u2_batch_counts.txt` S5 TOTAL tus=5 rows=36 filled=36 checked=34 unchecked=2 void=0 empty=0), every quote verbatim from `tmp/_tmp_u2_probe_cache.txt` and appended to `tmp/_tmp_u2_docquotes.txt`. `systemshutdown`'s `ShutdownBlockReasonDestroy` row was filled from D51's landed site, not from a page (the 5 seeds are never re-decided). Stop condition met per TU: `tmp/_tmp_u2_filled.ps1` -> `tmp/_tmp_u2_filled.txt` reads those 5 TUs `full`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the grandparent's (`../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: Small TU batch S1, Small TU batch S2, Small TU batch S3, Small TU batch S4.

Rows and stems per item are listed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM <tu> slice 1`. These are the largest small TUs (8 rows) - 8 URLs is still one `fetch_web_content` call, so no TU here is split.

## systemshutdown rows

6 rows: `ExitWindowsEx`, `CancelShutdown`, `LockWorkStation`, `ShutdownBlockReasonCreate`, `ShutdownBlockReasonDestroy`, `ShutdownBlockReasonQuery`. `ShutdownBlockReasonDestroy` is D51's - fill from `docs/agent/decisions/D51.md` and the landed site, no fetch. Fetch the other 5 in one call; then write the triple for all 6 rows.

## displayconfig rows

7 rows: `DisplayConfigGetDeviceInfo`, `DisplayConfigSetDeviceInfo`, `GetDisplayAutoRotationPreferences`, `GetDisplayConfigBufferSizes`, `QueryDisplayConfig`, `SetDisplayAutoRotationPreferences`, `SetDisplayConfig`. One fetch batch, then write. The `DisplayConfig*` pages are the ones whose failure value is an error code, not a sentinel - the class is `TCheckFailureValue< >`-shaped, not `CheckNonZero< >`.

## rawinput rows

7 rows: `DefRawInputProc`, `GetRawInputBuffer`, `GetRawInputData`, `GetRawInputDeviceInfo`, `GetRawInputDeviceList`, `GetRegisteredRawInputDevices`, `RegisterRawInputDevices`. One fetch batch, then write.

## caret rows

8 rows: `CreateCaret`, `DestroyCaret`, `GetCaretBlinkTime`, `GetCaretPos`, `HideCaret`, `SetCaretBlinkTime`, `SetCaretPos`, `ShowCaret`. One fetch batch of 8, then write.

## devicecontext rows

8 rows: `ChangeDisplaySettings`, `ChangeDisplaySettingsEx`, `EnumDisplayDevices`, `EnumDisplaySettings`, `EnumDisplaySettingsEx`, `GetDC`, `GetDCEx`, `ReleaseDC`. One fetch batch of 8, then write. `GetDC`/`GetDCEx` return NULL on failure where NULL is also a legal answer for the query shape - each row needs its page's exact wording before it decides.
