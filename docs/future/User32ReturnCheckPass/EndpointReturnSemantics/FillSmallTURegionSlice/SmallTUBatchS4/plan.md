---
Type: Plan
Title: "Small TU batch S4"
Parent: "../plan.md"
Level: 3
Items:
  - Title: systeminfo rows
    Status: Atomic
    Content:
  - Title: text rows
    Status: Atomic
    Content:
  - Title: help rows
    Status: Atomic
    Content:
  - Title: ime rows
    Status: Atomic
    Content:
  - Title: monitor rows
    Status: Atomic
    Content:
  - Title: processthread rows
    Status: Atomic
    Content:
  - Title: windowproperties rows
    Status: Atomic
    Content:
---

# Small TU batch S4

Slug: `SmallTUBatchS4` - from title "Small TU batch S4" (no drop-words, PascalCased, 14 chars, ASCII); the sibling U1 tree's name. Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **33** table rows of these 7 small TUs, one item per TU, each item one fetch batch.

## Resume

Cursor: **all 7 items done** - this plan reached its terminal state on 12 Nov 2026. Measured at that write: all **33** rows carry the verdict triple (`tmp/_tmp_u2_batch_verify.txt`: the 7 TUs read against the `ITEM <tu> rows=n` ledger of `tmp/_tmp_u2_slice_stems.txt`, `tus=7 slice_total=33 rows=33 fail=0`, 0 class-base mismatch against `sdkRetType`, 0 quote that is not a prefix of its cached section), verdicts `checked 12 / unchecked 20 / void 1` (`tmp/_tmp_u2_batch_counts.txt` S4 TOTAL) - `DisableProcessWindowsGhosting` is the `void` row, `AttachThreadInput`/`GetGuiResources`/`UserHandleGrantAccess` were filled from the source seeds and never re-decided, and `WaitForInputIdle` is the inverted `0 = wait satisfied` rejection. Table after the write: **88** of 597 rows decided, 28 TUs `full` / 0 `partial` / 0 `empty` (`tmp/_tmp_u2_filled.txt`), `tmp/_tmp_u2_patch.out.txt` `decided = 88` `class-shape violations = 0` `decisions with no table row = 0` `fail=0`. Next in the parent: item 5 **Small TU batch S5**.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the grandparent's (`../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: Small TU batch S1, Small TU batch S2, Small TU batch S3, Small TU batch S5.

Rows and stems per item are listed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM <tu> slice 1`.

## systeminfo rows

4 rows: `GetSysColor`, `GetSystemMetrics`, `SetSysColors`, `SystemParametersInfo`. One fetch batch, then write. `GetSystemMetrics` answers 0 legitimately - that row is the expected `unchecked`.

## text rows

4 rows: `DrawText`, `DrawTextEx`, `GetTabbedTextExtent`, `TabbedTextOut`. One fetch batch, then write.

## help rows

5 rows: `SetMenuContextHelpId`, `GetMenuContextHelpId`, `SetWindowContextHelpId`, `GetWindowContextHelpId`, `WinHelp`. One fetch batch, then write.

## ime rows

5 rows: `DisableProcessWindowsGhosting`, `SendIMEMessageEx`, `WINNLSGetIMEHotkey`, `WINNLSEnableIME`, `WINNLSGetEnableStatus`. One fetch batch, then write. `WINNLS*` pages are not on the `nf-winuser-` pattern for every stem - the 404 fallback leg is load-bearing here.

## monitor rows

5 rows: `EnumDisplayMonitors`, `GetMonitorInfo`, `MonitorFromPoint`, `MonitorFromRect`, `MonitorFromWindow`. One fetch batch, then write.

## processthread rows

5 rows: `AttachThreadInput`, `GetGuiResources`, `IsWow64Message`, `UserHandleGrantAccess`, `WaitForInputIdle`. 3 are decided in source (`AttachThreadInput`, `GetGuiResources`, `UserHandleGrantAccess`) - fill those rows from source, fetch the other 2. `WaitForInputIdle` is the inverted `WAIT_OBJECT_0` case: `unchecked` with the page's wording; then write the triple for all 5 rows.

## windowproperties rows

5 rows: `EnumProps`, `EnumPropsEx`, `GetProp`, `RemoveProp`, `SetProp`. One fetch batch, then write.
