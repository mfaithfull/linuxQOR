---
Type: Plan
Title: "Small TU batch S2"
Parent: "../plan.md"
Level: 3
Items:
  - Title: synchronization rows
    Status: Atomic
    Content:
  - Title: timer rows
    Status: Atomic
    Content:
  - Title: windowprocedure rows
    Status: Atomic
    Content:
  - Title: buttoncontrol rows
    Status: Atomic
    Content:
  - Title: combobox rows
    Status: Atomic
    Content:
  - Title: coordtransform rows
    Status: Atomic
    Content:
  - Title: filledshapes rows
    Status: Atomic
    Content:
---

# Small TU batch S2

Slug: `SmallTUBatchS2` - from title "Small TU batch S2" (no drop-words, PascalCased, 14 chars, ASCII); the sibling U1 tree's name, kept identical so the two passes re-census against one another. Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **18** table rows of these 7 small TUs, one item per TU, each item one fetch batch.

## Resume

Cursor: **all 7 items done** - this plan reached its terminal state on 11 Nov 2026. Measured at that write: all **18** rows carry the verdict triple (`tmp/_tmp_u2_leaf_verify.ps1` -> `tmp/_tmp_u2_leaf_verify.txt`: 7 TUs at `slice_rows = table_rows = filled`, `fail=0`), verdicts `checked 13 / unchecked 5`, class-base violations 0. The two `windowprocedure` rows (`CallWindowProc`, `DefWindowProc`) end `unchecked`: their `sdkRetType` is `NO-PROTOTYPE`, the published spelling is `LRESULT`, and the page gives the value as an answer ("The return value specifies the result of the message processing and depends on the message sent") rather than reserving a failure value. Next in the parent: item 3 **Small TU batch S3**.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the grandparent's (`../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: Small TU batch S1, Small TU batch S3, Small TU batch S4, Small TU batch S5.

Rows and stems per item are listed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM <tu> slice 1`. Note: `windowprocedure`'s rows are 2 of the 5 `LRESULT`/`LPARAM` spellings whose published type differs from the SDK type - the class base must be the row's `sdkRetType`.

## synchronization rows

2 rows: `MsgWaitForMultipleObjects`, `MsgWaitForMultipleObjectsEx`. One fetch batch, then write.

## timer rows

2 rows: `KillTimer`, `SetTimer`. One fetch batch, then write.

## windowprocedure rows

2 rows: `CallWindowProc`, `DefWindowProc`. One fetch batch, then write.

## buttoncontrol rows

3 rows: `CheckDlgButton`, `CheckRadioButton`, `IsDlgButtonChecked`. One fetch batch, then write.

## combobox rows

3 rows: `DlgDirListComboBox`, `DlgDirSelectComboBoxEx`, `GetComboBoxInfo`. One fetch batch, then write. `GetComboBoxInfo` is a nonzero-caller row - record the count in `notes`, the audit re-reads it before any code lands.

## coordtransform rows

3 rows: `ClientToScreen`, `ScreenToClient`, `MapWindowPoints`. One fetch batch, then write.

## filledshapes rows

3 rows: `FillRect`, `FrameRect`, `InvertRect`. One fetch batch, then write.
