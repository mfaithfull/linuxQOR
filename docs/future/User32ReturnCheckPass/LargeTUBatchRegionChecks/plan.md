---
Type: Plan
Title: "Large TU batch region checks"
Parent: "../plan.md"
Level: 1
Items:
  - Title: Large TU batch L1
    Status: Refined
    Content: ./LargeTUBatchL1/plan.md
  - Title: Large TU batch L2
    Status: Refined
    Content: ./LargeTUBatchL2/plan.md
  - Title: Large TU batch L3
    Status: Refined
    Content: ./LargeTUBatchL3/plan.md
  - Title: Window TU batch
    Status: Refined
    Content: ./WindowTUBatch/plan.md
  - Title: Accelerator TU sites
    Status: Atomic
    Content:
---

# Large TU batch region checks

Slug: `LargeTUBatchRegionChecks` - from title "Large TU batch region checks" (no drop-words, PascalCased, exactly 24 chars, at the cap). Mirrors the sibling tree's `BatchLargeTUs` boundaries (same 19 TUs, same L1/L2/L3 + `window.cpp` order and counts). Uniqueness is stored here, not recomputed.

## Goal

Land `CheckReturn< sdkRetType, Check >::TType` at exactly the `checked` verdicts of the **468 sites over the 19 largest TUs** (L1 90 + L2 153 + L3 139 + `window.cpp` 86) of `src/platform/os/windows/api_layer/user`, densest-last, plus the **5 `accelerator.cpp` sites** this plan owns as its item 5 (U1's `BatchLargeTUs` boundaries omit that TU - see the gap note in the parent's `## Resume`), so **473** sites in all, and record every `unchecked` verdict as a written reason.

## Resume

Cursor: **terminal** - all 5 items closed (4 `Refined` with children on disk, 1 `Atomic`). Item 5 **Accelerator TU sites** (`Atomic`) **CLOSED 13 Nov 2026** - the TU U1's `BatchLargeTUs` boundaries omit, now owned here: 3 tokens over its 5 rows, census `ok` (see the closure under its item body). Item 4 **Window TU batch** (`Refined`) -> `./WindowTUBatch/plan.md` **CLOSED 13 Nov 2026** at all 3 items (`WindowBOOLSites` 37 tokens / 55 rows, `WindowHWNDSites` 5 / 17, `WindowOtherSites` 8 tokens + 5 bare + 1 void / 14); all **86** window sites in their end state and `window.cpp` carries **50** `CheckReturn<` = the table's 50 `checked` rows there, one `../returncheck.h` include. Item 3 **Large TU batch L3** is **CLOSED 13 Nov 2026** - `./LargeTUBatchL3/plan.md` terminal at all 4 items closed; all **139** L3 sites in their end state and the 4 TUs' censuses read `ok` (`pointer 28/28`, `keyboard 10/10`, `strings 10/10`, `menu 31/31` tokens against their `checked` rows, one include each). Item 2 **Large TU batch L2** is **CLOSED 12 Nov 2026** - `./LargeTUBatchL2/plan.md` terminal at all 7 items closed; all **153** L2 sites in their end state, censuses `ok` (`desktop 15/15`, `clipboard 10/10`, `dpi 6/6`, `dialog 13/13`, `misc 13/13`, `message 10/10`, `painting 15/15`). Item 1 **Large TU batch L1** is **CLOSED 12 Nov 2026** - `./LargeTUBatchL1/plan.md` terminal at all 7 items closed, censuses `ok` (`mouse 4/4`, `rectangle 5/5`, `scrollbar 7/7`, `icon 13/13`, `touch 12/12`, `windowclass 6/6`, `cursor 12/12`). Layer tally at this closure: `tmp/_tmp_u2_ledger_check.ps1` `large owned: tus=20 rows=473 checked=273 / orphans TUs=0 / VERDICT fail=0`; folder-wide `tmp/_tmp_u2_live_checks.ps1` `tus_with_checks=48 code_sites=352 tus_with_returncheck_include=48` (small 79 + large 273 = 352 = the table's 352 `checked` rows) and `tmp/_tmp_u2_code_census.ps1` `tus=54 fail=0`. Terminal: all 5 items closed.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call; site comment carries the verbatim doc quote and the class; TU banner records the verdict counts; per-TU re-census at each batch, never at the close. Sibling titles: Site census tooling, Endpoint return semantics, Check class vocabulary, Small TU batch region checks, Folder gate and census, Records and commit.

Disk ledger (from `tmp/_tmp_u2_tree_verify.ps1`): L1 90 = `cursor` 15, `icon` 14, `touch` 14, `windowclass` 14, `scrollbar` 12, `rectangle` 11, `mouse` 10; L2 153 = `painting` 25, `message` 24, `misc` 23, `dialog` 22, `dpi` 21, `clipboard` 20, `desktop` 18; L3 139 = `menu` 39, `keyboard` 34, `strings` 34, `pointer` 32; `window.cpp` 86.

The `BOOL`-heavy TUs here (`dialog`, `message`, `painting`, `misc`, `pointer`, `keyboard`, `strings`, `menu`) are where the "FALSE is an answer" rejections concentrate - `EqualRect`, `IsRectEmpty`, `PtInRect`, `IntersectRect`, `UnionRect`, `TrackDrag`, `NotifyLinkStatusChange`-shaped predicates answer FALSE legitimately and must land `unchecked` with the reason, not checked.

Nonzero in-tree caller counts make these batches riskier than the small region: the table's `inTreeCallers` column is an **upper bound** (it counts comments and other libraries' declarations), so each `checked` row with a nonzero count needs a site-level read before the token moves. Heaviest: `GetDlgCtrlID` 8, `CopyIcon` 6, `SetWindowPos` 5, `GetDC` 4, `ReleaseDC` 3, `GetComboBoxInfo` 3, `PostQuitMessage` 3.

## Decisions

The landing contract every leaf in this subtree obeys - children point here, they do not restate it.

- **Token**: land `CheckReturn< sdkRetType, Check >::TType` at exactly the `checked` rows; the class base is the row's `sdkRetType`, never the published spelling. `unchecked` rows get the written reason in the site comment and no token; `void` rows get neither.
- **Quote**: the site comment carries the row's verbatim `docQuote` and the class - the table is the authority, not the page re-read at edit time.
- **Caller audit**: the table's `inTreeCallers` column is an upper bound; a `checked` row with a nonzero count needs the site-level read (`registers/u19.md` for `window.cpp`) before its token moves.
- **Include**: one `../returncheck.h` include per TU that gains its first check, never a second.
- **Leaf budget**: one leaf = one slice of at most **10** rows of one TU = at most 10 site edits. Slice boundaries are fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt`, the same cut the verdict side used, so the two passes stay aligned; prose counts are not the authority.
- **Stop condition per leaf**: re-read the edited sites, then run that TU's re-census - `qor_pp_fcontext; == qor_pp_useswinapi* == Library::*Call< == Win32ErrorHandler _; == n`, with `CheckReturn<` code sites counted separately against that TU's `checked` rows in the table. Tick the `registers/u2.md` row in the same session (the U17 lesson), never at the close.

## Large TU batch L1

90 sites over `cursor` 15, `icon` 14, `touch` 14, `windowclass` 14, `scrollbar` 12, `rectangle` 11, `mouse` 10. The rectangle/scrollbar families are the answer-shaped `BOOL` and zero-is-legal count region - expect a high `unchecked` ratio and a written reason per row. `cursor`'s `HCURSOR` rows decide on documented NULL-as-failure rows only. Every TU here but `mouse` exceeds the leaf budget, so it gets its own child plan. Child plan: `./LargeTUBatchL1/plan.md` (Level 1 -> 2, 7 items: 1 `Atomic`, 6 `Refined`).

## Large TU batch L2

153 sites over `painting` 25, `message` 24, `misc` 23, `dialog` 22, `dpi` 21, `clipboard` 20, `desktop` 18. `clipboard`'s count-returning rows (`GetClipboardData` handle rows, `CopyExternalBitmap`-shaped) and `misc`'s count rows are the expected `checked` set; `message`/`dialog` are mostly answer-`BOOL` and stay `unchecked` with reasons. Every TU here exceeds the leaf budget, so all 7 get a child plan. Child plan: `./LargeTUBatchL2/plan.md` (Level 1 -> 2, 7 `Refined` items).

## Large TU batch L3

139 sites over `menu` 39, `keyboard` 34, `strings` 34, `pointer` 32. `keyboard`'s `GetKeyboardState` is the library's only SAL-annotated prototype (`_Check_return_` at `winuser.h:5934`) - a hint, not a verdict. `strings` carries the `GetClassName`/`GetWindowText`-shaped count returns where the documented failure is zero and the copy count is the answer (`GetDlgCtrlID`'s 0-is-legal kin), so each row needs its page's exact wording before it lands. Every TU here needs 3-4 slices, so all 4 get a child plan. Child plan: `./LargeTUBatchL3/plan.md` (Level 1 -> 2, 4 `Refined` items).

## Window TU batch

86 sites in `window.cpp` - densest TU, most nonzero callers (`GetDlgCtrlID` 8, `SetWindowPos` 5, `GetDC` 4, `ReleaseDC` 3, `WindowFromPoint`'s NULL-is-legal answer). Every `checked` verdict here carries the caller-audit note from `registers/u19.md` before the token moves. Last, so the whole folder's check shape is proven on cheaper TUs first. Cut by documented return family (BOOL 55 / HWND 17 / other 14) then by the leaf budget, so 10 slices. Child plan: `./WindowTUBatch/plan.md` (Level 1 -> 2, 3 `Refined` items).

## Accelerator TU sites

5 sites in `accelerator.cpp` - the TU U1's `BatchLargeTUs` boundaries omit, so nothing else in the tree owns it (measured: `accelerator.cpp` is present in the source folder with 5 `qor_pp_fcontext;` sites and 0 checks; the code-side ledger read 592 of the table's 597 rows). One slice of 5 rows, inside the 10-row leaf budget, so no child plan. The table's accelerator block is CLOSED (verdict side item 5 **Fill the accelerator rows**): **checked 3** - `CreateAcceleratorTable` and `LoadAccelerators` at `CheckNonZeroHandle< HACCEL >` (both rows are AW with `sdkRetType` NO-PROTOTYPE, so the class base is the published `HACCEL`), `TranslateAccelerator` at `CheckNonZero< int >`; **unchecked 2** - `CopyAcceleratorTable` (the count is the answer the caller asks for, the page documents no failure value and never tells the caller to call `GetLastError`) and `DestroyAcceleratorTable` (zero is a legal outcome when a table was loaded more times than it was destroyed, so `BoolCheck` would report a legal partial destroy as a failure); **void 0**. Four of the 5 rows carry 0 in-tree callers (change-safe); `DestroyAcceleratorTable` carries 1 but lands unchecked, so no caller audit blocks this leaf. Landing needs exactly one `../returncheck.h` include and lands exactly 3 `CheckReturn<` tokens - the TU's re-census must read `fcontext=5 == useswinapi*=5 == Library::*Call<=5 == Win32ErrorHandler _;=5`, `CheckReturn-code=3 table-checked=3`, `returncheck-includes=1`.
**CLOSED 13 Nov 2026** - one slice of 5 rows landed in one edit pass: 3 `CheckReturn< >` tokens (`CreateAcceleratorTable` and `LoadAccelerators` at `CheckNonZeroHandle< HACCEL >`, `TranslateAccelerator` at `CheckNonZero< int >`), each preceded by the per-row policy comment carrying the table's verbatim docQuote and the class, and the 2 `unchecked` rows (`CopyAcceleratorTable`, `DestroyAcceleratorTable`) carrying their written reason with no token; one `#include "../returncheck.h"` at `accelerator.cpp:12`. Stop condition re-read: `tmp/_tmp_u2_code_census.ps1` `accelerator.cpp rows=5 fcontext=5 useswinapi=5 (1 bare + 4 AW) LibraryCall=5 Win32ErrorHandler=5 CheckReturn-code=3 table-checked=3 table-unchecked=2 returncheck-includes=1  ok` and `tmp/_tmp_u2_live_checks.ps1` `accelerator.cpp checks=3 includes=1`. The ledger `tmp/_tmp_u2_ledger_check.ps1` (fixed this session: `$m.Text` is empty in this pwsh build, the collector now uses `$m.ToString()`, and the STALE leg only flags names that are real folder `.cpp`) reads `stus=53 rows total=597 / small owned: tus=33 rows=124 checked=79 / large owned: tus=20 rows=473 checked=273 / orphans TUs=0 / mentioned-but-no-rows=0 prose-only=8 / VERDICT fail=0` - the 8 prose-only names (`buttoncontroller displaydevices messagebox messagehandler rect scrollbarcontroller windowcontroller windowimpl`) are citations of the U1 tree's TUs, none of them a TU of this folder; the folder holds 54 `.cpp` and the one outside the table is `winqor_api_user.cpp`, the module TU with no wrapper sites.

