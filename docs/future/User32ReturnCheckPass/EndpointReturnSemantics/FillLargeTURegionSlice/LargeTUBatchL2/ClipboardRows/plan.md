---
Type: Plan
Title: "clipboard rows"
Parent: "../plan.md"
Level: 4
Items:
  - Title: clipboard slice 1
    Status: Atomic
    Content:
  - Title: clipboard slice 2
    Status: Atomic
    Content:
---

# clipboard rows

Slug: `ClipboardRows` - from title "clipboard rows" (no drop-words, PascalCased, 13 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **20** `clipboard.cpp` rows in 2 slices of at most 10 URLs.

## Resume

This plan is **terminal** - both items `Atomic` and both **CLOSED 12 Nov 2026**. Item 1 **clipboard slice 1** is **CLOSED 12 Nov 2026**: rows 1-10 decided **checked 5 / unchecked 5 / void 0** over one fetch batch of 10 URLs at `VERDICT fail=0` - the 5 `checked` are `AddClipboardFormatListener`, `CloseClipboard`, `EmptyClipboard` (`BoolCheck`, `returncheck.h:94`; the pages' status lines reserve FALSE/zero and the BOOL carries no answer), `GetClipboardData` (`CheckNonZeroHandle< HANDLE >`, `returncheck.h:136` - the page reads "If the function succeeds, the return value is the handle to a clipboard object in the specified format. If the function fails, the return value is NULL", so NULL is reserved and this plan's worry that NULL is also a legal "no data" answer does **not** hold for this member: the no-data case is `GetClipboardOwner`'s NULL, a different row) and `GetClipboardFormatName` (`CheckNonZero< int >` - the length of a format name is never the empty string, so the documented 0 failure value is reserved; AW binding with `sdkRetType NO-PROTOTYPE`, so the class base is the published `int`). The 5 `unchecked` carry the page line that puts a legal answer at the tested value: `ChangeClipboardChain` (a chain member typically answers FALSE on a *successful* walk), `CountClipboardFormats` (an open clipboard with no formats answers 0), `EnumClipboardFormats` (end of enumeration is 0 with `GetLastError` = `ERROR_SUCCESS`, and the layer never consults `GetLastError`), `GetClipboardOwner` (unowned clipboard answers NULL), `GetClipboardSequenceNumber` (0 is a legal sequence answer). Item 2 **clipboard slice 2** is **CLOSED 12 Nov 2026**: rows 11-20 decided **checked 5 / unchecked 5 / void 0** over one fetch batch of 10 URLs at `VERDICT fail=0` - `checked`: `GetUpdatedClipboardFormats`, `OpenClipboard`, `RemoveClipboardFormatListener` (`BoolCheck`), `RegisterClipboardFormat` (`CheckNonZero< UINT >`, `returncheck.h:186`, AW `NO-PROTOTYPE` so the base is the published `UINT`), `SetClipboardData` (`CheckNonZeroHandle< HANDLE >`); `unchecked`: `GetClipboardViewer` (end of the viewer chain answers NULL), `GetOpenClipboardWindow` (no window owns the clipboard), `GetPriorityClipboardFormat` (two legal sentinels - NULL for an empty clipboard, -1 for "none of the listed formats"), `IsClipboardFormatAvailable` (the zero answers the caller's question), `SetClipboardViewer` (NULL means both error and end of chain). The TU now reads `tmp/_tmp_u2_tu_state.ps1 clipboard.cpp` `rows=20 checked=10 unchecked=10 void=0 blank=0 shape_bad=0` and `tmp/_tmp_u2_filled.txt` `clipboard.cpp rows=20 filled=20 full`; its code census reads `rows=20 fcontext=20 useswinapi=20 (18 bare + 2 AW) LibraryCall=20 Win32ErrorHandler=20 CheckReturn-code=10 table-checked=10 table-unchecked=10 returncheck-includes=1 ok` and the folder gate `tmp/_tmp_u2_gate54.bat` reads exit 0 at `tmp/_tmp_u2_gatechk.ps1` `srcs=54 not-in-log=0 stale=0 real-diag-lines=0`. Terminal: both items `Atomic`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: desktop rows, dpi rows, dialog rows, misc rows, message rows, painting rows.

Slices fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM clipboard`: 20 rows in slices of 10 and 10 in table order. `GetClipboardData` returns NULL on failure where NULL is also a legal "no format" answer; the count rows (`CountClipboardFormats`, `GetUpdatedClipboardFormats`) are the expected `checked` set.

## clipboard slice 1

Rows 1-10: `AddClipboardFormatListener`, `ChangeClipboardChain`, `CloseClipboard`, `CountClipboardFormats`, `EmptyClipboard`, `EnumClipboardFormats`, `GetClipboardData`, `GetClipboardFormatName`, `GetClipboardOwner`, `GetClipboardSequenceNumber`. One fetch batch of 10, then write those 10 rows.

## clipboard slice 2

Rows 11-20: `GetClipboardViewer`, `GetOpenClipboardWindow`, `GetPriorityClipboardFormat`, `GetUpdatedClipboardFormats`, `IsClipboardFormatAvailable`, `OpenClipboard`, `RegisterClipboardFormat`, `RemoveClipboardFormatListener`, `SetClipboardData`, `SetClipboardViewer`. One fetch batch of 10, then write those 10 rows.
