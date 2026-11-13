---
Type: Plan
Title: "cursor rows"
Parent: "../plan.md"
Level: 4
Items:
  - Title: cursor slice 1
    Status: Atomic
    Content:
  - Title: cursor slice 2
    Status: Atomic
    Content:
---

# cursor rows

Slug: `CursorRows` - from title "cursor rows" (no drop-words, PascalCased, 10 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **15** `cursor.cpp` rows in 2 slices of at most 10 URLs.

## Resume

This plan is **terminal** - both items `Atomic` and both **CLOSED 12 Nov 2026**. Item 1 **cursor slice 1** is **CLOSED 12 Nov 2026**: rows 1-10 decided **checked 9 / unchecked 1 / void 0** on one fetch batch of exactly 10 URLs (`tmp/_tmp_u2_cursor_fetch.out.txt` - 10 `FETCH` rows at `http=200`, `fetched=10 of 10`, `bad=0`, `VERDICT fail=0`, every `Return value` section `match-cache` against `tmp/_tmp_u2_probe_cache.txt`; the 2 AW rows `LoadCursor` and `LoadCursorFromFile` fetched off their corrected `a`-slug pages, carried by the probe's `resolvedUrl` - the two `CORRECTED` lines). The 3 handle rows `CreateCursor`, `LoadCursor`, `LoadCursorFromFile` take `CheckNonZeroHandle< HCURSOR >` because each page reserves NULL for failure with "To get extended error information, call GetLastError" and a handle answer is never NULL; the 6 status rows `ClipCursor`, `DestroyCursor`, `GetClipCursor`, `GetCursorInfo`, `GetCursorPos`, `GetPhysicalCursorPos` take `BoolCheck` on the plain status BOOL wording, and `GetCursor` is `unchecked` because NULL is an answer - "The return value is the handle to the current cursor. If there is no cursor, the return value is NULL" names a window state and the page gives no failure branch and no extended error info. Item 2 **cursor slice 2** is **CLOSED 12 Nov 2026**: rows 11-15 decided **checked 3 / unchecked 2 / void 0** on one fetch batch of exactly 5 URLs (`tmp/_tmp_u2_cursor_fetch_s2.out.txt` - 5 `FETCH` rows at `http=200`, `fetched=5 of 5`, `bad=0`, `VERDICT fail=0`, every section `match-cache`). The 3 `checked` are `SetCursorPos`, `SetPhysicalCursorPos`, `SetSystemCursor` - `BoolCheck` on the plain status BOOL wording ("Returns nonzero if successful or zero otherwise", "TRUE if successful; otherwise FALSE", "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero"). The 2 `unchecked`: `SetCursor` answers the previous handle and NULL is a legal state ("If there was no previous cursor, the return value is NULL" - the page names no failure branch), and `ShowCursor`'s `int` is an answer, not a status - "The return value specifies the new display counter" with the page making 0 reachable ("If a mouse is installed, the initial display count is 0. If no mouse is installed, the display count is -1"). TU outcome **checked 12 / unchecked 3 / void 0**; every row carries its 1 in-tree caller in `notes` (audit before any code lands). Measured at this write: `tmp/_tmp_u2_patch.out.txt` `decided = 214`, `checked = 138 / unchecked = 72 / void = 4`, `class-shape violations = 0`, `filled rows with no page = 7`, `decisions with no table row = 0`, **fail=0**; `tmp/_tmp_u2_tu_state.ps1 cursor.cpp` `rows=15 checked=12 unchecked=3 void=0 blank=0 shape_bad=0`; folder-wide `tmp/_tmp_u2_filled.txt` `tus=53 rows=597 filled=214 full-tus=40 / partial=0 / empty=0`; `tmp/_tmp_u2_docquotes.txt` `207` rows.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: mouse rows, rectangle rows, scrollbar rows, icon rows, touch rows, windowclass rows.

Slices fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM cursor`: 15 rows in slices of 10 and 5 in table order. The `HCURSOR` rows decide on documented NULL-as-failure only; `GetCursorPos`-adjacent count rows answer 0 legitimately.

## cursor slice 1

Rows 1-10: `ClipCursor`, `CreateCursor`, `DestroyCursor`, `GetClipCursor`, `GetCursor`, `GetCursorInfo`, `GetCursorPos`, `GetPhysicalCursorPos`, `LoadCursor`, `LoadCursorFromFile`. One fetch batch of 10, then write those 10 rows.

## cursor slice 2

Rows 11-15: `SetCursorPos`, `SetPhysicalCursorPos`, `ShowCursor`, `SetCursor`, `SetSystemCursor`. One fetch batch of 5, then write those 5 rows.
