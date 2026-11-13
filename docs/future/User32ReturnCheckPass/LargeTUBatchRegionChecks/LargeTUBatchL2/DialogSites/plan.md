---
Type: Plan
Title: "dialog sites"
Parent: "../plan.md"
Level: 3
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

# dialog sites

Slug: `DialogSites` - from title "dialog sites" (no drop-words, PascalCased, 11 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **22** `dialog.cpp` sites in 3 slices of at most 10 sites.

## Resume

Cursor: **terminal** - all 3 items `Atomic`, all 3 closed 12 Nov 2026. All **22** `dialog.cpp` sites carry their end state: the **13** `checked` rows carry their `CheckReturn` token and the **9** `unchecked` rows carry their policy comment; `dialog.cpp` re-censuses `rows=22 fcontext=22 useswinapi=22 (9 bare + 13 AW) LibraryCall=22 Win32ErrorHandler=22 CheckReturn-code=13 table-checked=13 table-unchecked=9 returncheck-includes=1 ok` - the chain is exact and `CheckReturn-code=13 == table-checked=13`, so the leaf's stop condition is met with no gap left. Item 2 **dialog slice 2** is **CLOSED 12 Nov 2026**: all 6 `checked` rows among slice 2's 10 sites landed (`GetNextDlgGroupItem`, `GetNextDlgTabItem` -> `CheckNonZeroHandle< HWND >`; `MapDialogRect`, `MessageBox`, `MessageBoxEx`, `MessageBoxIndirect` -> `CheckNonZero< int >`), and `dialog.cpp` re-censuses `rows=22 fcontext=22 useswinapi=22 (9 bare + 13 AW) LibraryCall=22 Win32ErrorHandler=22 CheckReturn-code=11 table-checked=13 table-unchecked=9 returncheck-includes=1` (`11 == 5 + 6`, gap 2 = slice 3, chain exact; `check-FAIL` the expected mid-leaf state). Caller audits done before landing: `MessageBox`'s table `0` counts the bare stem only and a T-tolerant re-count finds **1** site (`ui/gui/windows/i_m/messagebox.cpp:41` `m_result = User32::MessageBoxT(...)`, stored in `m_result` and handed back by `MessageBox::Result()` at `:57`), recorded at that site; `MessageBoxIndirectT` likewise has **1** site at `messagebox.cpp:52` (u19.md:77-78 records the trio `MessageBoxT` `:41` / `MessageBoxTimeoutT` `:46` / `MessageBoxIndirectT` `:52`), recorded at that site; `MessageBoxExT` re-measured T-tolerant today and appears nowhere but its definition, so 0 is honest there. `MessageBoxTimeout` stayed `unchecked` (no Learn page - the probe booked NO-PAGE, docQuote `-`) and carries no token. Item 1 **dialog slice 1** is **CLOSED 12 Nov 2026**: the 5 `checked` rows among slice 1's 10 sites landed (`CreateDialogIndirectParam`, `CreateDialogParam` -> `CheckNonZeroHandle< HWND >`; `EndDialog` -> `BoolCheck`; `GetDlgCtrlID` -> `CheckNonZero< int >`; `GetDlgItem` -> `CheckNonZeroHandle< HWND >`) plus the single include `#include "../returncheck.h"` at `dialog.cpp:12`, and `dialog.cpp` re-censuses `rows=22 fcontext=22 useswinapi=22 (9 bare + 13 AW) LibraryCall=22 Win32ErrorHandler=22 CheckReturn-code=5 table-checked=13 table-unchecked=9 returncheck-includes=1` (gap 8 = slice 2's 6 + slice 3's 2, chain exact; `check-FAIL` is the expected mid-leaf state). Caller audits done before landing: `GetDlgCtrlID`'s 8 sites re-read today (the register's `buttoncontroller.cpp:107` has moved to `:126`, so the count was re-measured) and `EndDialog`'s 2 statement-level discarding sites at `ui/gui/windows/a_h/dialog.cpp:58`/`:65`. Terminal: all 3 items `Atomic`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, caller audit for nonzero counts, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: desktop sites, clipboard sites, dpi sites, misc sites, message sites, painting sites. `GetDlgCtrlID` carries **8** in-tree callers (the tree's heaviest nonzero row) - re-read its call sites before its token moves. The `MessageBox*` family answers the entered value or FALSE - expected `unchecked` with the page's wording.

## dialog slice 1

10 sites in `dialog.cpp`: `CreateDialogIndirectParam`, `CreateDialogParam`, `DefDlgProc`, `DialogBoxIndirectParam`, `DialogBoxParam`, `EndDialog`, `GetDialogBaseUnits`, `GetDlgCtrlID`, `GetDlgItem`, `GetDlgItemInt`. Land only the `checked` rows, with the caller audit first for `GetDlgCtrlID`. Stop condition: re-read the sites, then re-census `dialog.cpp` against its `checked` row count.

## dialog slice 2

10 sites in `dialog.cpp`: `GetDlgItemText`, `GetNextDlgGroupItem`, `GetNextDlgTabItem`, `IsDialogMessage`, `MapDialogRect`, `MessageBox`, `MessageBoxEx`, `MessageBoxIndirect`, `MessageBoxTimeout`, `SendDlgItemMessage`. Stop condition: re-read the sites, then re-census `dialog.cpp` - its counts must already match the slice-1 census.

## dialog slice 3

2 sites in `dialog.cpp`: `SetDlgItemInt`, `SetDlgItemText`. Stop condition: re-read the sites, then re-census `dialog.cpp` - its counts must already match the slice-2 census.

## Closed 12 Nov 2026 - evidence

- Slice 3 (`SetDlgItemInt`, `SetDlgItemText`) landed both `checked` rows at `dialog.cpp:390` / `:405` with `CheckReturn< BOOL, BoolCheck >::TType bSet = ...`; the last two policy comments sit at `:379` / `:394`. Census moved `CheckReturn-code` 11 -> **13** = `table-checked=13`, so the leaf's stop condition (`counts must already match`) is met.
- Token/stem correspondence re-read across the whole file today (13 tokens, each inside its own wrapper): `:52` `CreateDialogIndirectParam`, `:69` `CreateDialogParam`, `:135` `EndDialog`, `:174` `GetDlgCtrlID`, `:191` `GetDlgItem`, `:240` `GetNextDlgGroupItem`, `:258` `GetNextDlgTabItem`, `:289` `MapDialogRect`, `:311` `MessageBox`, `:327` `MessageBoxEx`, `:346` `MessageBoxIndirect`, `:390` `SetDlgItemInt`, `:405` `SetDlgItemText` - each token's class base equals the table's `sdkRetType` and its check class equals the table's `checkClass`; measured split: 5 `CheckReturn< HWND, CheckNonZeroHandle< HWND > >`, 4 `CheckReturn< int, CheckNonZero< int > >`, 4 `CheckReturn< BOOL, BoolCheck >`.
- Brace balance re-read today: 26 open / 26 close. File is 409 lines; `Return policy (U2` blocks = **22** = one per row, so every site carries its end state; `#include ... returncheck` = 1 (the single-include-per-TU rule).
- Deviation from this leaf's `## Context`, which predicted the `MessageBox*` family as `unchecked` ("answers the entered value or FALSE legitimately"): measured, `MessageBox` / `MessageBoxEx` / `MessageBoxIndirect` are **checked** at `CheckNonZero< int >`, because their pages reserve zero - "If the function fails, the return value is zero." - and enumerate the success set with no zero in it ("If the function succeeds, the return value is one of the following menu-item values": `IDOK 1`, `IDCANCEL 2`, `IDABORT 3`, `IDRETRY 4`, `IDIGNORE 5`, `IDYES 6`, `IDNO 7`, `IDTRYAGAIN 10`, `IDCONTINUE 11`), so the session-27 test passes at every value. The phrased shape the leaf anticipated belongs instead to the `nResult` rows `DialogBoxParam` / `DialogBoxIndirectParam`, which are the `unchecked` pair ("If the function succeeds, the return value is the value of the nResult parameter specified in the call to the EndDialog function used to terminate the dialog box"). `MessageBoxTimeout` stayed `unchecked` on the NO-PAGE reading.
- No new check class was needed: `BoolCheck`, `CheckNonZero< int >`, `CheckNonZeroHandle< HWND >` all pre-exist in `returncheck.h` (lines 94 / 187 / 137).

