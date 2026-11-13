---
Type: Plan
Title: "dialog rows"
Parent: "../plan.md"
Level: 4
Items:
  - Title: dialog slice 1
    Status: Atomic
    Content:
  - Title: dialog slice 2
    Status: Atomic
    Content:
  - Title: dialog slice 3
    Status: Atomic
    Content:
---

# dialog rows

Slug: `DialogRows` - from title "dialog rows" (no drop-words, PascalCased, 10 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **22** `dialog.cpp` rows in 3 slices of at most 10 URLs.

## Resume

Cursor: **terminal** - all 3 items `Atomic`, all 3 closed 12 Nov 2026. All **22** rows of `dialog.cpp` carry the verdict triple: `checked 13 / unchecked 9 / void 0`, `tmp/_tmp_u2_tu_state.ps1 dialog.cpp` `rows=22 checked=13 unchecked=9 void=0 blank=0 shape_bad=0`; the folder audit `tmp/_tmp_u2_patch.out.txt` `VERDICT fail=0`, `class-shape violations = 0`, `decisions with no table row = 0`, `filled rows with no page = 8` (up one from 7 - `MessageBoxTimeout`, the only row here with no Learn page).

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: desktop rows, clipboard rows, dpi rows, misc rows, message rows, painting rows.

Slices fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM dialog`: 22 rows in slices of 10/10/2 in table order. 20 of 22 rows here have 0 in-tree callers; `GetDlgCtrlID` is the tree's heaviest nonzero row (**8** in-tree references) - its row must carry the caller count in `notes`, and `registers/u19.md` stays the authority for the call-site list before any code lands there. The `MessageBox*` family answers the entered value or FALSE legitimately - expected `unchecked` with the page's wording.

## dialog slice 1

Rows 1-10: `CreateDialogIndirectParam`, `CreateDialogParam`, `DefDlgProc`, `DialogBoxIndirectParam`, `DialogBoxParam`, `EndDialog`, `GetDialogBaseUnits`, `GetDlgCtrlID`, `GetDlgItem`, `GetDlgItemInt`. One fetch batch of 10, then write those 10 rows.

## dialog slice 2

Rows 11-20: `GetDlgItemText`, `GetNextDlgGroupItem`, `GetNextDlgTabItem`, `IsDialogMessage`, `MapDialogRect`, `MessageBox`, `MessageBoxEx`, `MessageBoxIndirect`, `MessageBoxTimeout`, `SendDlgItemMessage`. One fetch batch of 10, then write those 10 rows.

## dialog slice 3

Rows 21-22: `SetDlgItemInt`, `SetDlgItemText`. One fetch batch of 2, then write those 2 rows.

## Cursors

dialog slice 1: {"tu":"dialog.cpp","slice_start":0,"slice_len":10,"done":true}
dialog slice 2: {"tu":"dialog.cpp","slice_start":10,"slice_len":10,"done":true}
dialog slice 3: {"tu":"dialog.cpp","slice_start":20,"slice_len":2,"done":true}

## Closed 12 Nov 2026 - evidence

- Slice 1 (rows 1-10) in one fetch batch: `tmp/_tmp_u2_dialog_fetch.out.txt`, 10 lines all `match-cache` at HTTP 200.
- Slice 2 (rows 11-20) in one fetch batch: `tmp/_tmp_u2_dialog_fetch_s2.out.txt`, 10 lines all `match-cache` at HTTP 200 - including `MessageBoxTimeout` booked `NO-PAGE` (the Learn index has no slug for it), so its docQuote stays `-` and its verdict is page-free.
- Slice 3 (rows 21-22) in one fetch batch: `tmp/_tmp_u2_dialog_fetch_s3.out.txt`, 2 lines `match-cache` at HTTP 200.
- 22 verdict lines appended to `tmp/_tmp_u2_decisions.txt` (lines 274-295), written into the table by `tmp/_tmp_u2_patch.ps1`; every `checked` row names a class, every `unchecked` row names none, and the docQuote column carries the page's verbatim Return value text from the probe cache.
- Check classes used here all exist in `api_layer/returncheck.h` (`BoolCheck` line 94, `CheckNonZero< int >` line 186, `CheckNonZeroHandle< HWND >` line 199) - this TU adds no check-class vocabulary item.
- Class bases match the rows' types: `HWND` rows take `CheckNonZeroHandle< HWND >`, the `int` rows take `CheckNonZero< int >`, the `BOOL` rows take `BoolCheck`; the `NO-PROTOTYPE` rows (`CreateDialog*`, `MessageBox*`, `SetDlgItemText`, `IsDialogMessage`) take the published type from the table's `pubRetType` column, which is what the AW wrapper holds.
- Deviation from this leaf's `## Context` hint: it predicted the `MessageBox*` family would be `unchecked` ("answers the entered value or FALSE legitimately"). The pages put the success answer at the enumerated menu-item values (`IDOK 1` .. `IDCONTINUE 11`) and reserve zero ("If the function fails, the return value is zero." for `MessageBox`/`MessageBoxEx`, "If there is not enough memory to create the message box, the return value is zero." for `MessageBoxIndirect`), so `MessageBox` / `MessageBoxEx` / `MessageBoxIndirect` are `checked / CheckNonZero< int >`; only `MessageBoxTimeout` is `unchecked`, and it is `unchecked` because it has no Learn page at all (its zero is the timeout answer, which no page documents). The "answers the entered value or FALSE legitimately" shape is the `nResult` rows - `DialogBoxParam`, `DialogBoxIndirectParam` and `EndDialog`'s caller side - and those two `DialogBox*` rows are `unchecked` on exactly that reading.
- `GetDlgCtrlID`'s row carries its 8-caller count and the `registers/u19.md` pointer in `notes` as this leaf required; no code change here is proposed, only the verdict triple.
- `DialogBoxIndirectParam` / `DialogBoxParam` are `unchecked` because the documented failure value (zero, and -1 for other failures) is also a legal success answer (the `nResult` the caller's DLGPROC hands to `EndDialog`, routinely `FALSE`); the session-27 soundness test fails at that value, so D51's no-check reading is the only admissible one.
- `GetDlgItemInt` is `unchecked` on the page's own words ("because zero is a possible translated value, a return value of zero does not by itself indicate failure"); the real failure signal is the BOOL the `lpTranslated` parameter points at, which the layer never reads.
- `GetDlgItemText` is `unchecked` on the same zero-is-legal-count reading as the `GetWindowTextLength` row D51 names (a control with an empty title answers 0 characters), and deliberately unlike the `GetClassName` row, which is checked because a window class always has a name while a control never must.

