---
Type: Plan
Title: "keyboard sites"
Parent: "../plan.md"
Level: 3
Items:
  - Title: keyboard slice 1
    Status: Atomic
    Content:
  - Title: keyboard slice 2
    Status: Atomic
    Content:
  - Title: keyboard slice 3
    Status: Atomic
    Content:
  - Title: keyboard slice 4
    Status: Atomic
    Content:
---

# keyboard sites

Slug: `KeyboardSites` - from title "keyboard sites" (no drop-words, PascalCased, 13 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **34** `keyboard.cpp` sites in 4 slices of at most 10 sites.

## Resume

Cursor: **terminal** - all 4 items `Atomic`, all 4 closed 12 Nov 2026. `keyboard.cpp` carries **10** `CheckReturn< >` tokens at exactly its 10 `checked` rows and **33** `//Return policy` blocks (the 10 checked + the 23 unchecked; the single `void` row `keybd_event` carries none, there being no return to state), with its one `../returncheck.h` include; its census `tmp/_tmp_u2_code_census.ps1` reads `rows=34 fcontext=34 useswinapi=34 (27 bare + 7 AW) LibraryCall=34 Win32ErrorHandler=34 CheckReturn-code=10 table-checked=10 table-unchecked=23 returncheck-includes=1  ok` (`tus=1 fail=0`). Terminal: all 4 items `Atomic`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, caller audit for nonzero counts, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: pointer sites, strings sites, menu sites. 29 of 34 rows have 0 in-tree callers; the other 5 need the site-level read before their token moves.

## keyboard slice 1

10 sites in `keyboard.cpp`: `ActivateKeyboardLayout`, `BlockInput`, `EnableWindow`, `GetActiveWindow`, `GetAsyncKeyState`, `GetFocus`, `GetKBCodePage`, `GetKeyboardLayout`, `GetKeyboardLayoutList`, `GetKeyboardLayoutName`. Land only the `checked` rows. Stop condition: re-read the sites, then re-census `keyboard.cpp` against its `checked` row count.

## keyboard slice 2

10 sites in `keyboard.cpp`: `GetKeyboardState`, `GetKeyboardType`, `GetKeyNameText`, `GetKeyState`, `GetLastInputInfo`, `IsWindowEnabled`, `keybd_event`, `LoadKeyboardLayout`, `MapVirtualKey`, `MapVirtualKeyEx`. `GetKeyState`/`GetAsyncKeyState` answer their value - expected `unchecked` with the page's wording. Stop condition: re-read the sites, then re-census `keyboard.cpp` - its counts must already match the slice-1 census.

## keyboard slice 3

10 sites in `keyboard.cpp`: `OemKeyScan`, `RegisterHotKey`, `SendInput`, `SetActiveWindow`, `SetFocus`, `SetKeyboardState`, `ToAscii`, `ToAsciiEx`, `ToUnicode`, `ToUnicodeEx`. Stop condition: re-read the sites, then re-census `keyboard.cpp` - its counts must already match the slice-2 census.

## keyboard slice 4

4 sites in `keyboard.cpp`: `UnloadKeyboardLayout`, `UnregisterHotKey`, `VkKeyScan`, `VkKeyScanEx`. Stop condition: re-read the sites, then re-census `keyboard.cpp` - its counts must already match the slice-3 census.

## Cursors

slice 1: {"tu":"keyboard.cpp","site_start":0,"site_len":10,"done":true}
slice 2: {"tu":"keyboard.cpp","site_start":10,"site_len":10,"done":true}
slice 3: {"tu":"keyboard.cpp","site_start":20,"site_len":10,"done":true}
slice 4: {"tu":"keyboard.cpp","site_start":30,"site_len":4,"done":true}

## Closed 12 Nov 2026 - evidence

- Tokens landed, keyed to the table's `checked` rows (verified by reading the owner signature above each token line): slice 1 `ActivateKeyboardLayout` -> `CheckReturn< HKL, CheckNonZeroHandle< HKL > >` (row `sdkRetType` blank so the base is the published `HKL`), `GetKeyboardLayoutList` -> `CheckReturn< int, CheckNonZero< int > >`, `GetKeyboardLayoutNameT` -> `CheckReturn< BOOL, BoolCheck >` over the AW binding (`sdkRetType` NO-PROTOTYPE); slice 2 `GetKeyboardState` -> `BoolCheck`, `GetLastInputInfo` -> `BoolCheck`, `LoadKeyboardLayoutT` -> `CheckNonZeroHandle< HKL >` (AW row, NO-PROTOTYPE so the published `HKL` is the base); slice 3 `RegisterHotKey` -> `BoolCheck`, `SetKeyboardState` -> `BoolCheck`; slice 4 `UnloadKeyboardLayout` -> `BoolCheck`, `UnregisterHotKey` -> `BoolCheck`. Token lines in the final file: 92, 208, 223, 239, 297, 336, 400, 463, 536, 553 - ten, one per `checked` row, no token at any `unchecked` or `void` row.
- Correction to this leaf's own mid-state arithmetic: the slice-1 cursor said slices 2-4 owed "3 + 3 + 1 = 7" tokens. Measured against the table they owed 3 + 2 + 2 = 7 (slice 3 has 2 `checked` rows, slice 4 has 2). The total was right; the per-slice split in that cursor was not, and is corrected here.
- Caller audit for the two `checked` rows with nonzero in-tree callers, both exactly 1, read at the site before the token moved: `RegisterHotKey` at `src/platform/os/windows/ui/gui/controllers/windowcontroller.cpp:82` and `UnregisterHotKey` at `:87`, both of the form `return User32::X(...) ? true : false;`. The wrapper is value-preserving at such a call: `returncheck.h:46` `operator TReturn()` returns `m_Param` unchanged, and `AutoErrorCheck::Check` at `:79-85` only calls `continuable("A checked parameter failed a test")` on a failed `Test` - it does not rewrite the value - so the caller still sees `false` for a failed call and the landed check adds the diagnostic path. Both audits are check-safe.
- The three other nonzero-caller rows in this TU (`EnableWindow` 1, `SetFocus` 1, `IsWindowEnabled` 1) are `unchecked` and stayed bare, so no audit was owed for a token there; `IsWindowEnabled`'s caller sits at `windowcontroller.cpp:77` in the same shape.
- No new check class was needed: every class used here (`BoolCheck` `returncheck.h:94`, `CheckNonZeroHandle<T>` `:137`, `CheckNonZero<T>` `:187`) was already in the header, so the `CheckClassVocabulary` leaf took no addition from this TU.
- `keybd_event` is the TU's single `void` row (`sdkRetType` VOID, `pubRetType` VOID) and stays bare with no policy block - the leaf's 33 blocks against 34 rows.
- Deviation from this leaf's `## Context`, which predicted `GetKeyState`/`GetAsyncKeyState` as the answer-shaped pair: measured, both are `unchecked` as predicted, but so are the four `ToAscii`/`ToAsciiEx`/`ToUnicode`/`ToUnicodeEx` rows (their pages make the return a character-count answer where 0 is a legal "no character produced" answer) and `GetKeyboardType`, `GetKBCodePage`, `SendInput`, `OemKeyScan`, `MapVirtualKey`/`MapVirtualKeyEx`, `VkKeyScan`/`VkKeyScanEx` - 23 unchecked in total, with `GetKeyboardState` and `GetLastInputInfo` checked on their plain status BOOLs.
- Stop condition met at every slice: the census moved 3 -> 6 -> 8 -> 10 tokens and reads `ok` only at the final 10/10.

