---
Type: Plan
Title: "Large TU batch region checks"
Parent: "../plan.md"
Level: 1
Items:
  - Title: Large TU batch L1
    Status: Atomic
    Content:
  - Title: Large TU batch L2
    Status: Atomic
    Content:
  - Title: Large TU batch L3
    Status: Atomic
    Content:
  - Title: Window TU batch
    Status: Atomic
    Content:
---

# Large TU batch region checks

Slug: `LargeTUBatchRegionChecks` - from title "Large TU batch region checks" (no drop-words, PascalCased, exactly 24 chars, at the cap). Mirrors the sibling tree's `BatchLargeTUs` boundaries (same 19 TUs, same L1/L2/L3 + `window.cpp` order and counts). Uniqueness is stored here, not recomputed.

## Goal

Land `CheckReturn< sdkRetType, Check >::TType` at exactly the `checked` verdicts of the **473 sites over the 19 largest TUs** (L1 90 + L2 153 + L3 139 + `window.cpp` 86) of `src/platform/os/windows/api_layer/user`, densest-last, and record every `unchecked` verdict as a written reason.

## Resume

Cursor: item 1 **Large TU batch L1** (`Atomic`). **Hard precondition: the parent's item 2 must have filled these rows' verdicts**, and the small-TU region plan must have landed first so the cheap batches' shape is proven before the dense TUs are touched.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call; site comment carries the verbatim doc quote and the class; TU banner records the verdict counts; per-TU re-census at each batch, never at the close. Sibling titles: Site census tooling, Endpoint return semantics, Check class vocabulary, Small TU batch region checks, Folder gate and census, Records and commit.

Disk ledger (from `tmp/_tmp_u2_tree_verify.ps1`): L1 90 = `cursor` 15, `icon` 14, `touch` 14, `windowclass` 14, `scrollbar` 12, `rectangle` 11, `mouse` 10; L2 153 = `painting` 25, `message` 24, `misc` 23, `dialog` 22, `dpi` 21, `clipboard` 20, `desktop` 18; L3 139 = `menu` 39, `keyboard` 34, `strings` 34, `pointer` 32; `window.cpp` 86.

The `BOOL`-heavy TUs here (`dialog`, `message`, `painting`, `misc`, `pointer`, `keyboard`, `strings`, `menu`) are where the "FALSE is an answer" rejections concentrate - `EqualRect`, `IsRectEmpty`, `PtInRect`, `IntersectRect`, `UnionRect`, `TrackDrag`, `NotifyLinkStatusChange`-shaped predicates answer FALSE legitimately and must land `unchecked` with the reason, not checked.

Nonzero in-tree caller counts make these batches riskier than the small region: the table's `inTreeCallers` column is an **upper bound** (it counts comments and other libraries' declarations), so each `checked` row with a nonzero count needs a site-level read before the token moves. Heaviest: `GetDlgCtrlID` 8, `CopyIcon` 6, `SetWindowPos` 5, `GetDC` 4, `ReleaseDC` 3, `GetComboBoxInfo` 3, `PostQuitMessage` 3.

## Large TU batch L1

90 sites over `cursor`, `icon`, `touch`, `windowclass`, `scrollbar`, `rectangle`, `mouse`. The rectangle/scrollbar families are the answer-shaped `BOOL` and zero-is-legal count region - expect a high `unchecked` ratio here and a written reason per row. `cursor`'s `HCURSOR` rows (`CreateCursor`/`CreateIconFromDescriptor`/`GetCursorPos`-adjacent handles) decide on documented NULL-as-failure rows only.

## Large TU batch L2

153 sites over `painting`, `message`, `misc`, `dialog`, `dpi`, `clipboard`, `desktop`. `clipboard`'s count-returning rows (`GetClipboardData` handle rows, `CopyExternalBitmap`-shaped) and `misc`'s `GetWindowsDirectory`-shaped count rows are the expected `checked` set; `message`/`dialog` are mostly answer-`BOOL` and stay `unchecked` with reasons.

## Large TU batch L3

139 sites over `menu` 39, `keyboard` 34, `strings` 34, `pointer` 32. `keyboard`'s `GetKeyboardState` is the library's only SAL-annotated prototype (`_Check_return_` at `winuser.h:5934`) - a hint, not a verdict. `strings` carries the `GetClassName`/`GetWindowText`-shaped count returns where the documented failure is zero and the copy count is the answer (`GetDlgCtrlID`'s 0-is-legal kin), so each row needs its page's exact wording before it lands.

## Window TU batch

86 sites in `window.cpp` - densest TU, most nonzero callers (`GetDlgCtrlID` 8, `SetWindowPos` 5, `GetDC` 4, `ReleaseDC` 3, `WindowFromPoint`'s NULL-is-legal answer). Every `checked` verdict here carries the caller-audit note from `registers/u19.md` before the token moves. Last, so the whole folder's check shape is proven on cheaper TUs first.
