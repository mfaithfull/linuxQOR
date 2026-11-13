---
Type: Plan
Title: "windowclass rows"
Parent: "../plan.md"
Level: 4
Items:
  - Title: windowclass slice 1
    Status: Atomic
    Content:
  - Title: windowclass slice 2
    Status: Atomic
    Content:
---

# windowclass rows

Slug: `WindowclassRows` - from title "windowclass rows" (no drop-words, PascalCased, 15 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **14** `windowclass.cpp` rows in 2 slices of at most 10 URLs.

## Resume

This plan is **terminal** - both items `Atomic` and both **CLOSED 12 Nov 2026**. Item 1 **windowclass slice 1** is **CLOSED 12 Nov 2026**: rows 1-10 decided **checked 5 / unchecked 5 / void 0** on one fetch batch of exactly 10 URLs (`tmp/_tmp_u2_windowclass_fetch.out.txt` - 10 `FETCH` rows at `http=200`, `fetched=10 of 10`, `bad=0`, `VERDICT fail=0`, every `Return value` section `match-cache` against `tmp/_tmp_u2_probe_cache.txt`; 8 of the 10 URLs are the probe's corrected `-a` slugs, which the gate lets a later leaf use). The 5 `checked` rows: `GetClassInfo` and `GetClassInfoEx` take `BoolCheck` on the plain status wording ("If the function finds a matching class and successfully copies the data, the return value is nonzero. If the function fails, the return value is zero") over the published `BOOL` - the row is `NO-PROTOTYPE` so the class base is the published spelling; `GetClassName` takes `CheckNonZero< int >` since success returns "the number of characters copied to the buffer" and zero is reserved; `RegisterClass`/`RegisterClassEx` take `CheckNonZero< ATOM >` because success returns "a class atom that uniquely identifies the class being registered", so no legal answer is zero (`Dimm.h:258 typedef WORD ATOM` - the published ATOM is the only type the wrapper can hold). The 5 `unchecked` rows all answer zero legitimately: `GetClassLongPtr` and `GetClassWord` return "the requested value"/"the requested 16-bit value", `GetWindowLong` states the legal zero outright ("If SetWindowLong has not been called previously, GetWindowLong returns zero for values in the extra window or class memory"), and `SetClassLongPtr`/`SetClassWord` are previous-state answers ("the return value is the previous value of the specified offset. If this was not previously set, the return value is zero"). `GetClassWord` and `SetClassWord` carry 1 in-tree caller each in `notes` (audit before any code lands) - both end `unchecked`, so nothing lands. Item 2 **windowclass slice 2** is **CLOSED 12 Nov 2026**: rows 11-14 decided **checked 1 / unchecked 3 / void 0** on one fetch batch of exactly 4 URLs (`tmp/_tmp_u2_windowclass_fetch_s2.out.txt` - 4 `FETCH` rows at `http=200`, `fetched=4 of 4`, `bad=0`, `VERDICT fail=0`, all 4 `match-cache`, all 4 the probe's corrected `-a` slugs). `SetWindowLong` and `SetWindowLongPtr` are the documented indeterminate pair - "If the previous value of the specified 32-bit integer is zero, and the function succeeds, the return value is zero, but the function does not clear the last error information. This makes it difficult to determine success or failure." - so zero is a legal answer and the failure value is not separable without `SetLastError` choreography the caller owns; `GetWindowLongPtr` states its legal zero ("If SetWindowLong or SetWindowLongPtr has not been called previously, GetWindowLongPtr returns zero for values in the extra window or class memory"); the one `checked` row is `UnregisterClass` with `BoolCheck`, whose zero branch is the documented failure with extended error info ("If the class could not be found or if a window still exists that was created with the class, the return value is zero. To get extended error information, call GetLastError") and matches the `Unregister*` family already decided `BoolCheck`. Stop condition met: `tmp/_tmp_u2_tu_state.ps1 windowclass.cpp` re-reads `tu=windowclass.cpp rows=14 checked=6 unchecked=8 void=0 blank=0 shape_bad=0`, matching this plan's 14-row ledger; `tmp/_tmp_u2_patch.out.txt` re-reads `filled-verdict = 199`, `checked = 126 / unchecked = 69 / void = 4`, `class-shape violations = 0`, `decisions with no table row = 0`, **fail=0**; `tmp/_tmp_u2_docquotes.txt` at **192** rows (14 new) and `tmp/_tmp_u2_decisions.txt` at **199** decision lines.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: mouse rows, rectangle rows, scrollbar rows, icon rows, touch rows, cursor rows.

Slices fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM windowclass`: 14 rows in slices of 10 and 4 in table order. 12 of these rows are `NO-PROTOTYPE`, so their class base must come from the fetched page's documented SDK type, not the `NO-PROTOTYPE` column value. `GetClassName` (AW) is seeded `checked` `CheckReturn< int, CheckNonZero< int > >` from a fetched page - the quote must still land in `tmp/_tmp_u2_docquotes.txt`.

## windowclass slice 1

Rows 1-10: `GetClassInfo`, `GetClassInfoEx`, `GetClassLongPtr`, `GetClassName`, `GetClassWord`, `GetWindowLong`, `RegisterClass`, `RegisterClassEx`, `SetClassLongPtr`, `SetClassWord`. One fetch batch of 10, then write those 10 rows.

## windowclass slice 2

Rows 11-14: `SetWindowLong`, `SetWindowLongPtr`, `GetWindowLongPtr`, `UnregisterClass`. One fetch batch of 4, then write those 4 rows.
