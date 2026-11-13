---
Type: Plan
Title: "dpi sites"
Parent: "../plan.md"
Level: 3
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

# dpi sites

Slug: `DpiSites` - from title "dpi sites" (no drop-words, PascalCased, 8 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **21** `dpi.cpp` sites in 3 slices of at most 10 sites.

## Resume

Cursor: all 3 items **CLOSED 12 Nov 2026** - this plan is terminal. Item 3 **dpi slice 3** landed the 6th token at `SystemParametersInfoForDpi` (`dpi.cpp:443`: `CheckReturn< BOOL, BoolCheck >::TType bResult` over the TU's existing `Library::Call< BOOL, UINT, UINT, PVOID, UINT, UINT >`, policy comment above quoting the row's docQuote), closing the 1-site gap the verdict side opened when `DpiRows` filled row 21 `checked / BoolCheck`. Items 1-2 landed the other 5 tokens (`EnableNonClientDpiScaling`, `GetWindowDpiAwarenessContext`, `SetDialogControlDpiChangeBehavior`, `SetDialogDpiChangeBehavior`, `SetProcessDpiAwarenessContext`) and the TU's `../returncheck.h` include stays at 1 (`:98`). Stop-condition measurements after the landing: `tmp/_tmp_u2_code_census.ps1` (over `tmp/_tmp_u2_census_tus.txt` = `dpi.cpp`) reads `dpi.cpp rows=21 fcontext=21 useswinapi=21 (21 bare + 0 AW) LibraryCall=21 Win32ErrorHandler=21 CheckReturn-code=6 table-checked=6 table-unchecked=15 returncheck-includes=1 ok` at `tus=1 fail=0`; the standalone `/W4` leg `tmp/_tmp_u2_one_tu.bat` with the TU's absolute path reads `TU_EXIT=0` at `log_lines=1 diags=0`; the folder gate `tmp/_tmp_u2_gate54.bat` -> `tmp/_tmp_u2_gatechk.ps1` reads `srcs=54 not-in-log=0 stale=0 real-diag-lines=0` at `FOLDER_EXIT=0`; `tmp/_tmp_u2_live_checks.ps1` -> `.txt` reads `dpi.cpp checks=6 includes=1` at `tus_with_checks=30 code_sites=95`; and the docquote audit `tmp/_tmp_u2_dqcheck_tu.ps1` -> `tmp/_tmp_u2_dpi_dqcheck.txt` reads `filled=21 docquotes-rows=21 cache-pages=21 mismatch=0`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: desktop sites, clipboard sites, dialog sites, misc sites, message sites, painting sites. All 21 rows here have 0 in-tree callers. `IsValidDpiAwarenessContext` answers BOOL legitimately - expected `unchecked` with the page's wording.

## dpi slice 1

10 sites in `dpi.cpp`: `AreDpiAwarenessContextsEqual`, `EnableNonClientDpiScaling`, `GetAwarenessFromDpiAwarenessContext`, `GetDialogControlDpiChangeBehavior`, `GetDialogDpiChangeBehavior`, `GetDpiAwarenessContextForProcess`, `GetDpiForSystem`, `GetDpiFromDpiAwarenessContext`, `GetSystemDpiForProcess`, `GetSystemMetricsForDpi`. Land only the `checked` rows. Stop condition: re-read the sites, then re-census `dpi.cpp` against its `checked` row count.

## dpi slice 2

10 sites in `dpi.cpp`: `GetThreadDpiAwarenessContext`, `GetThreadDpiHostingBehavior`, `GetWindowDpiAwarenessContext`, `GetWindowDpiHostingBehavior`, `IsValidDpiAwarenessContext`, `SetDialogControlDpiChangeBehavior`, `SetDialogDpiChangeBehavior`, `SetProcessDpiAwarenessContext`, `SetThreadDpiAwarenessContext`, `SetThreadDpiHostingBehavior`. Stop condition: re-read the sites, then re-census `dpi.cpp` - its counts must already match the slice-1 census.

## dpi slice 3

1 site in `dpi.cpp`: `SystemParametersInfoForDpi`. Stop condition: re-read the site, then re-census `dpi.cpp` - its counts must already match the slice-2 census.
