---
Type: Plan
Title: "clipboard sites"
Parent: "../plan.md"
Level: 3
Items:
  - Title: clipboard slice 1
    Status: Atomic
    Content:
  - Title: clipboard slice 2
    Status: Atomic
    Content:
---

# clipboard sites

Slug: `ClipboardSites` - from title "clipboard sites" (no drop-words, PascalCased, 14 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **20** `clipboard.cpp` sites in 2 slices of at most 10 sites.

## Resume

Cursor: **terminal** - both items `Atomic`, both closed 12 Nov 2026. All **20** `clipboard.cpp` sites read: the **10** `checked` rows carry their token with the table's class and the **10** `unchecked` rows carry their written reason; one `../returncheck.h` include. The TU's re-census `tmp/_tmp_u2_code_census.ps1` reads `clipboard.cpp rows=20 fcontext=20 useswinapi=20 (18 bare + 2 AW) LibraryCall=20 Win32ErrorHandler=20 CheckReturn-code=10 table-checked=10 table-unchecked=10 returncheck-includes=1 ok`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, caller audit for nonzero counts, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: desktop sites, dpi sites, dialog sites, misc sites, message sites, painting sites. `GetClipboardData` answers NULL both as failure and as "no data" - that row stays `unchecked` with the page's wording.

## clipboard slice 1

10 sites in `clipboard.cpp`: `AddClipboardFormatListener`, `ChangeClipboardChain`, `CloseClipboard`, `CountClipboardFormats`, `EmptyClipboard`, `EnumClipboardFormats`, `GetClipboardData`, `GetClipboardFormatName`, `GetClipboardOwner`, `GetClipboardSequenceNumber`. Land only the `checked` rows. Stop condition: re-read the sites, then re-census `clipboard.cpp` against its `checked` row count.

## clipboard slice 2


## Closed 12 Nov 2026 - evidence

- Slice 1 (10 sites) and slice 2 (10 sites) both read; every site now carries one of the three end states, and the TU's census is `ok` at the first reading - no gap left by either slice.
- Verified per-site token/class correspondence (all 10 exact against the table's `checkClass`): `BoolCheck` on `AddClipboardFormatListener`, `CloseClipboard`, `EmptyClipboard`, `GetUpdatedClipboardFormats`, `OpenClipboard`, `RemoveClipboardFormatListener`; `CheckNonZeroHandle< HANDLE >` on `GetClipboardData`, `SetClipboardData`; `CheckNonZero< int >` on `GetClipboardFormatNameT`; `CheckNonZero< UINT >` on `RegisterClipboardFormatT`. The two AW rows take the published base as their `sdkRetType` is `NO-PROTOTYPE`.
- `git diff --stat` against the U1 commit shows `163 insertions(+), 10 deletions(-)` for this TU - the 10 deletions are exactly the 10 bare `return Library::Call< >` lines the U2 pass replaced with a `CheckReturn< >` temporary plus a return, so the tokens are this pass's landing, not pre-existing code.
- `GetClipboardData` is the exception this leaf's `## Context` flags, and the table keeps it **checked** with `CheckNonZeroHandle< HANDLE >`: its page reserves NULL for failure and the "no data" reading belongs to `GetClipboardOwner` / `GetOpenClipboardWindow` (both unchecked on the answer-shaped NULL), so NULL is not a legal success answer for the handle the caller passes to `GlobalLockMemory`/`CopyProps`-style consumers. Recorded at the site.
- The 10 `unchecked` reasons split three shapes: callback/answer BOOL (`ChangeClipboardChain` - "the return value from ChangeClipboardChain is typically FALSE", `IsClipboardFormatAvailable` - a question about format availability), zero-is-legal counts (`CountClipboardFormats`, `EnumClipboardFormats`, `GetPriorityClipboardFormat`, `GetClipboardSequenceNumber`), and answer-shaped handles (`GetClipboardOwner`, `GetClipboardViewer`, `GetOpenClipboardWindow`, `SetClipboardViewer` - NULL answers "no viewer in the chain" / "no window has the clipboard open" legitimately).
- All 20 rows report 0 in-tree callers, so no G6 caller audit gated this TU; every reason carries that count as the change-safe marker.
- Vocabulary unchanged: `BoolCheck` (:94), `CheckNonZeroHandle` (:137), `CheckNonZero` (:187) all already in `api_layer/returncheck.h`.

10 sites in `clipboard.cpp`: `GetClipboardViewer`, `GetOpenClipboardWindow`, `GetPriorityClipboardFormat`, `GetUpdatedClipboardFormats`, `IsClipboardFormatAvailable`, `OpenClipboard`, `RegisterClipboardFormat`, `RemoveClipboardFormatListener`, `SetClipboardData`, `SetClipboardViewer`. Stop condition: re-read the sites, then re-census `clipboard.cpp` - its counts must already match the slice-1 census.
