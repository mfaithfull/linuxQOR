---
Type: Plan
Title: "dpi rows"
Parent: "../plan.md"
Level: 4
Items:
  - Title: dpi slice 1
    Status: Atomic
    Content:
  - Title: dpi slice 2
    Status: Atomic
    Content:
  - Title: dpi slice 3
    Status: Atomic
    Content:
---

# dpi rows

Slug: `DpiRows` - from title "dpi rows" (no drop-words, PascalCased, 7 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **21** `dpi.cpp` rows in 3 slices of at most 10 URLs.

## Resume

Cursor: all 3 items **CLOSED 12 Nov 2026** - item 3 **dpi slice 3** filled row 21 (`SystemParametersInfoForDpi`) `checked / BoolCheck` from the fetched Return value section (verbatim, appended to `tmp/_tmp_u2_docquotes.txt`), so this plan is terminal and `dpi rows` is CLOSED at 21/21. Item 1 **dpi slice 1** is **CLOSED 12 Nov 2026**: rows 1-10 decided **checked 1 / unchecked 9 / void 0** over one fetch batch of 10 URLs at `VERDICT fail=0`, every page `match-cache` against `tmp/_tmp_u2_probe_cache.txt`. The one `checked` row is `EnableNonClientDpiScaling` (`BoolCheck`, `returncheck.h:94` - the page's status line reserves zero and the member answers nothing besides whether scaling was enabled for `hwnd`). The 9 `unchecked` each carry the page line that keeps the tested value legal: `AreDpiAwarenessContextsEqual` (the FALSE answers the caller's equality question and no failure value is named), `GetAwarenessFromDpiAwarenessContext` (the only sentinel is what an invalid *parameter* answers, and `DPI_AWARENESS_INVALID` is also a legal no-awareness answer), `GetDialogControlDpiChangeBehavior` / `GetDialogDpiChangeBehavior` (zero is the no-flags state; the documented zero is an invalid handle parameter and the distinction lives in `GetLastError`, which the layer never consults), `GetDpiAwarenessContextForProcess` / `GetDpiForSystem` / `GetDpiFromDpiAwarenessContext` / `GetSystemDpiForProcess` (their `Return value` sections are one sentence naming no failure value at all, so D51 step 2 reads no-check), and `GetSystemMetricsForDpi` - the row this leaf had to arbitrate: the page's generic status template reserves zero, but the value the member answers is a metric and 0 is a legal metric, which the reviewed `dpi.cpp` banner states at :40, so the session-27 soundness test fails at zero and the row stays `unchecked` with that reason. The TU now reads `tmp/_tmp_u2_tu_state.ps1 dpi.cpp` `rows=21 checked=6 unchecked=15 void=0 blank=0 shape_bad=0` and `tmp/_tmp_u2_filled.txt` `dpi.cpp rows=21 filled=21 full`; its code census reads `rows=21 fcontext=21 useswinapi=21 (21 bare + 0 AW) LibraryCall=21 Win32ErrorHandler=21 CheckReturn-code=6 table-checked=6 table-unchecked=15 returncheck-includes=1 ok` (the 6th token landed the same day at the `DpiSites` code-side leaf), the folder gate reads exit 0 at `srcs=54 not-in-log=0 stale=0 real-diag-lines=0`, the standalone leg `tmp/_tmp_u2_one_tu.bat` with the TU's absolute path reads `TU_EXIT=0` at `log_lines=1 diags=0 obj-newer`, and the docquote audit `tmp/_tmp_u2_dqcheck_tu.ps1` -> `tmp/_tmp_u2_dqcheck_tu.txt` reads `filled=21 docquotes-rows=21 cache-pages=21 mismatch=0`. Item 2 **dpi slice 2** is **CLOSED 12 Nov 2026**: rows 11-20 decided **checked 4 / unchecked 6 / void 0** over one fetch batch of 10 URLs, all 10 pages `match-cache`, `VERDICT fail=0`. The 4 `checked`: `GetWindowDpiAwarenessContext` takes `CheckNonZeroHandle< DPI_AWARENESS_CONTEXT >` (returncheck.h:136, Test at :144 is `(value == 0x00000000) ? false : true`) - the page's "If the window is not valid, the return value is NULL ." reserves NULL and a valid window always answers a context handle, so the session-27 test passes there, and unlike the `GetClipboardOwner` / `GetCursor` rows whose NULL names a legal empty state, this NULL is the only value a call that could not do its job answers; and `SetDialogControlDpiChangeBehavior` / `SetDialogDpiChangeBehavior` / `SetProcessDpiAwarenessContext` take `BoolCheck` (returncheck.h:94) on status BOOLs whose mask/values or context arguments are parameters rather than the return, with the page naming `ERROR_INVALID_HANDLE` / `ERROR_ACCESS_DENIED` / `ERROR_INVALID_PARAMETER` as failures - `SetProcessDpiAwarenessContext`'s page also calls a late set a failure ("Once API awareness is set for an app, any future calls to this API will fail"), so FALSE there is a documented failure rather than an answer. The 6 `unchecked`: the two thread `Get*` rows and the two hosting `Get*` rows whose `Return value` sections are one sentence naming no failure value at all (D51 step 2 reads no-check), `IsValidDpiAwarenessContext` (the FALSE answers the caller's question about a context it already holds - same shape as `AreDpiAwarenessContextsEqual` in this TU and `IsClipboardFormatAvailable` in clipboard.cpp), and the two `SetThread*` rows, which return the PREVIOUS context / behaviour (D51's previous-state shape) with the documented sentinel answering an invalid *parameter* - for the hosting one that sentinel `DPI_HOSTING_BEHAVIOR_INVALID` is an enumerator of its own return type (`-1`, verified live in the preprocessed SDK header), so a legal value of the return type collides with the tested value and the session-27 test fails at it.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: desktop rows, clipboard rows, dialog rows, misc rows, message rows, painting rows.

Slices fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM dpi`: 21 rows in slices of 10/10/1 in table order. 21 of 21 rows here have 0 in-tree callers. The `DPI_AWARENESS_CONTEXT` rows carry the library's `DPI_AWARENESS_CONTEXT`/`DPI_*_DPI_CHANGE_BEHAVIORS` types - the class for those is `HandleNullCheck`-shaped only where the page documents NULL as failure.

## dpi slice 1

Rows 1-10: `AreDpiAwarenessContextsEqual`, `EnableNonClientDpiScaling`, `GetAwarenessFromDpiAwarenessContext`, `GetDialogControlDpiChangeBehavior`, `GetDialogDpiChangeBehavior`, `GetDpiAwarenessContextForProcess`, `GetDpiForSystem`, `GetDpiFromDpiAwarenessContext`, `GetSystemDpiForProcess`, `GetSystemMetricsForDpi`. One fetch batch of 10, then write those 10 rows.

## dpi slice 2

Rows 11-20: `GetThreadDpiAwarenessContext`, `GetThreadDpiHostingBehavior`, `GetWindowDpiAwarenessContext`, `GetWindowDpiHostingBehavior`, `IsValidDpiAwarenessContext`, `SetDialogControlDpiChangeBehavior`, `SetDialogDpiChangeBehavior`, `SetProcessDpiAwarenessContext`, `SetThreadDpiAwarenessContext`, `SetThreadDpiHostingBehavior`. One fetch batch of 10, then write those 10 rows.

## dpi slice 3

Row 21: `SystemParametersInfoForDpi`. One fetch, then write one row.
