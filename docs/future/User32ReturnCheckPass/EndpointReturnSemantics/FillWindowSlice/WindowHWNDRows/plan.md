---
Type: Plan
Title: "window HWND rows"
Parent: "../plan.md"
Level: 3
Items:
  - Title: window HWND slice 1
    Status: Atomic
    Content:
  - Title: window HWND slice 2
    Status: Atomic
    Content:
---

# window HWND rows

Slug: `WindowHWNDRows` - from title "window HWND rows" (no drop-words, PascalCased, 14 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **17** `HWND` rows of `window.cpp`, in 2 slices of at most 10 URLs.

## Resume

Cursor: **terminal** - both items `Atomic`, both closed 12 Nov 2026. All **17** `HWND` rows of `window.cpp` carry the verdict triple: `checked 5 / unchecked 12 / void 0`, measured off the table (`window.cpp` + `pubRetType HWND` -> `rows=17 checked=5 unchecked=12 void=0 blank=0 shape_bad=0`); the folder single-pass audit `tmp/_tmp_u2_folder_audit.ps1` reads `rows=597 filled=578 checked=341 unchecked=229 void=8 class-shape violations=0 filled rows with no docQuote=0`, and `window.cpp blank=14` is the `window other rows` leaf alone.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the parent of the parent (`../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: window BOOL rows, window other rows.

Slice boundaries fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM window-HWND`: 17 rows in slices of 10 and 7 in table order. The class, where a row is `checked`, is `HandleCheck`/`HandleNullCheck`-shaped and its base must equal the row's `sdkRetType`.

## window HWND slice 1

Rows 1-10: `ChildWindowFromPoint`, `ChildWindowFromPointEx`, `CreateWindowEx`, `FindWindow`, `FindWindowEx`, `GetAncestor`, `GetDesktopWindow`, `GetForegroundWindow`, `GetLastActivePopup`, `GetParent`. One fetch batch of 10, then write those 10 rows.

## window HWND slice 2

Rows 11-17: `GetShellWindow`, `GetTopWindow`, `GetWindow`, `RealChildWindowFromPoint`, `SetParent`, `WindowFromPhysicalPoint`, `WindowFromPoint`. One fetch batch of 7, then write. `WindowFromPoint`'s NULL is a documented failure **and** a legal "nothing there" answer - that row is expected `unchecked` with the page's wording.

## Cursors

window HWND slice 1: {"tu":"window.cpp","rows":10,"slice_start":0,"slice_len":10,"done":true,"closed":"2026-11-12"}
window HWND slice 2: {"tu":"window.cpp","rows":17,"slice_start":10,"slice_len":7,"done":true,"closed":"2026-11-12"}

## Closed 12 Nov 2026 - evidence

- Slice 1 (rows 1-10) in one fetch batch: `tmp/_tmp_u2_wh_fetch.out.txt` - 10 `FETCH ... match-cache http=200` lines with 3 `CORRECTED` legs, `fetched=10 of 10 bad=0 no-page=0`, `VERDICT fail=0`.
- Slice 2 (rows 11-17) in one fetch batch: `tmp/_tmp_u2_wh_fetch_s2.out.txt` - 7 `match-cache` at HTTP 200, `fetched=7 of 7 bad=0 no-page=0`, `VERDICT fail=0`.
- Instrument note: `window.cpp` rows sit in **alphabetical** table order, not grouped by family, so the per-TU table-offset slicing of the earlier leaves cannot address these rows. This leaf uses `tmp/_tmp_u2_wh_fetch.ps1`, which reads the 17 stems from `tmp/_tmp_u2_wh_stems.txt` (table order, matching `tmp/_tmp_u2_slice_stems.txt` under `ITEM window-HWND`) and slices by `take`/`skip` over that list.
- 17 verdict lines appended to `tmp/_tmp_u2_decisions.txt` (lines 570-586) and written into the table by `tmp/_tmp_u2_patch.ps1` (`decisions with no table row = 0`, `class-shape violations = 0`, `fail=0`).
- The 3 rows the probe booked `no-page` with a corrected slug (`CreateWindowEx`, `FindWindow`, `FindWindowEx`) were fetched at the suffixed ANSI twin (`...exa`, `...a`, `...exa`) through the instrument's `resolved-via` leg - the contract's "filled once from the probe log's corrected slug" rule, with no slug guessed inside the leaf.
- `checked` 5: `CreateWindowEx`, `FindWindow`, `FindWindowEx`, `GetAncestor`, `SetParent` - all `CheckNonZeroHandle< HWND >`, the class already booked for the `dialog.cpp` / `mdi.cpp` handle rows, so **no new check class** was needed. `GetAncestor` and `SetParent` carry 1 in-tree caller each and therefore carry the caller-audit note plus the `registers/u19.md` pointer in `notes` before any code lands; the other 3 have 0 callers (change-safe).
- `unchecked` 12, and every one of them is the NULL-as-legal-answer shape: `ChildWindowFromPoint` / `ChildWindowFromPointEx` (NULL for a point outside the parent - the Ex row also names NULL as failure, so the value is conflated and the session-27 test fails at it), `GetShellWindow` (NULL when no Shell process is present), `GetTopWindow` (NULL when the window has no children), `GetWindow` (NULL when no window has the requested relationship), `RealChildWindowFromPoint` and `GetDesktopWindow` (pages that name no failure value at all), `GetForegroundWindow` ("can be NULL in certain circumstances, such as when a window is losing activation"), `GetLastActivePopup` (degenerately answers with the `hWnd` the caller passed), `GetParent`, `WindowFromPhysicalPoint` and `WindowFromPoint` (NULL when no window exists at the point).
- `GetParent` is the leaf's arbitration row: its page does say "If the function fails, the return value is NULL", but its own failure list reads "The window is a top-level window that is unowned or does not have the WS_POPUP style", a legal window state, so NULL answers "this window has no parent" and a landed `CheckNonZeroHandle< HWND >` would report a legal parentless window as a failure under G6 - hence `unchecked`, while `GetAncestor` (whose page documents no legal NULL) is `checked`.
- Deviation from this leaf's `## Context`, which said the class "where a row is checked" would be `HandleCheck`/`HandleNullCheck`-shaped: measured against `returncheck.h`, `HandleCheck` (`:112`) keys only on `(HANDLE)(-1)` and `HandleNullCheck` (`:124`) has a fixed `AutoErrorCheck< HANDLE >` base that mismatches these rows' `sdkRetType HWND` (the audit's class-base rule), so `CheckNonZeroHandle< HWND >` (`:137`, `template< typename THandle > : public AutoErrorCheck< THandle >`, `Test` fails at `0x00000000`) is the class that fits both the NULL-failure reading and the base rule.
- Deviation from this leaf's `## Context` on the expected `unchecked` set: `WindowFromPoint` was predicted unchecked and the measured page confirms it ("If no window exists at the given point, the return value is NULL" - the page never says "if the function fails"), but `SetParent` is the only `checked` row in slice 2, and slice 1's `CreateWindowEx` / `FindWindow` / `FindWindowEx` trio is checked on the same NULL-reserved-for-failure reading.
- No code change is proposed by this leaf; it fills the verdict triple only. 10 of the 17 rows carry nonzero in-tree callers (1 each) and every one of them carries its count in `notes` as the audit-before-landing marker, as `registers/u19.md` stays the call-site authority.

