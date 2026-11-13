---
Type: Plan
Title: "keyboard rows"
Parent: "../plan.md"
Level: 4
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

# keyboard rows

Slug: `KeyboardRows` - from title "keyboard rows" (no drop-words, PascalCased, 12 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **34** `keyboard.cpp` rows in 4 slices of at most 10 URLs.

## Resume

Cursor: **terminal** - all 4 items `Atomic`, all 4 closed 12 Nov 2026. All **34** rows of `keyboard.cpp` carry the verdict triple: `checked 10 / unchecked 23 / void 1`, `tmp/_tmp_u2_tu_state.ps1 keyboard.cpp` `rows=34 checked=10 unchecked=23 void=1 blank=0 shape_bad=0`; folder audit `rows=597 filled=433 checked=258 unchecked=167 void=8 sum=433 class-shape violations=0`.

## Cursors

keyboard slice 1: {"tu":"keyboard.cpp","slice_start":0,"slice_len":10,"done":true}
keyboard slice 2: {"tu":"keyboard.cpp","slice_start":10,"slice_len":10,"done":true}
keyboard slice 3: {"tu":"keyboard.cpp","slice_start":20,"slice_len":10,"done":true}
keyboard slice 4: {"tu":"keyboard.cpp","slice_start":30,"slice_len":4,"done":true}

## Closed 12 Nov 2026 - evidence

- Four fetch batches, one per slice: `tmp/_tmp_u2_kb_fetch.out.txt` (rows 1-10), `_s2` (11-20), `_s3` (21-30), `_s4` (31-34). All `fetched=N of N bad=0 no-page=0`, `VERDICT fail=0`; every `Return value` section `match-cache` except two rows flagged `differs-from-cache` (see below).
- The 404 fallback leg fired here: 8 of 34 rows fetched at a suffixed ANSI-twin slug printed `CORRECTED` (`getkeyboardlayoutnamea`, `getkeynametexta`, `loadkeyboardlayouta`, `mapvirtualkeya`, `mapvirtualkeyexa`, `vkkeyscana`, `vkkeyscanexa`, and `getkeyboardlayoutname` in slice 1).
- 34 verdict lines appended to `tmp/_tmp_u2_decisions.txt` (lines 407-440) and written by `tmp/_tmp_u2_patch.ps1` (`decisions with no table row = 0`, `class-shape violations = 0`, `filled rows with no docQuote = 0`).
- 5 of 34 rows carry 1 in-tree caller (`EnableWindow`, `IsWindowEnabled`, `RegisterHotKey`, `SetFocus`, `UnregisterHotKey`) - each carries its caller count in `notes` as the audit-before-landing marker; the other 29 are change-safe (0 callers).
- The single `void` row is `keybd_event` (`pubRetType VOID`, `sdkRetType VOID`, page Return value section "None").
- The 10 `checked` rows: `ActivateKeyboardLayout` (`CheckNonZeroHandle< HKL >` - previous locale always nonzero, zero reserved), `GetKeyboardLayoutList` (`CheckNonZero< int >` - count >= 1 both branches, zero reserved), `GetKeyboardLayoutName` / `GetKeyboardState` / `GetLastInputInfo` / `SetKeyboardState` / `RegisterHotKey` / `UnregisterHotKey` / `UnloadKeyboardLayout` (all `BoolCheck` on the plain status-BOOL wording), `LoadKeyboardLayout` (`CheckNonZeroHandle< HKL >` - success and no-match fallback both nonzero, NULL reserved).
- The `GetKeyboardState` row is the library's only SAL-annotated prototype (`_Check_return_` at `winuser.h:5934`); the annotation is a hint, and the page's status-BOOL wording independently confirms `checked`.
- The 23 `unchecked` rows, grouped by the page wording that reserves nothing: **question/state BOOLs** `BlockInput`, `EnableWindow`, `IsWindowEnabled` (zero = "already blocked"/"was not previously disabled"/"not enabled" - a legal state); **answer-shaped handles** `GetActiveWindow`, `GetFocus` (NULL = no active/focused window), and by extension the previous-handle Set rows `SetActiveWindow`, `SetFocus` (NULL answers "no window was previously active/focused", the GetActiveWindow row proving NULL is a legal no-window answer); **bitmask/answer SHORTs** `GetAsyncKeyState`, `GetKeyState` (up-key answers 0/negative legitimately); **count-with-legal-zero ints** `GetKeyboardType` (page says 0 valid for subtype), `GetKeyNameText` (empty-name length 0 = failure value), `ToAscii`/`ToAsciiEx`/`ToUnicode`/`ToUnicodeEx` (0/1/2 or negative/zero/positive are all legal conversion answers); **packed-translation with legal -1** `OemKeyScan`, `MapVirtualKey`/`MapVirtualKeyEx`, `VkKeyScan`/`VkKeyScanEx` (-1 / all-(-1) bytes answer "cannot be produced"/"no translation" as informational outcomes).
- Deviation from this leaf's `## Context`: it predicted `GetKeyState`/`GetAsyncKeyState`/`VkKeyScan*` unchecked (confirmed) but left `GetKeyboardState`/`SetKeyboardState`/`GetKeyboardLayoutList`/`LoadKeyboardLayout`/`ActivateKeyboardLayout`/`RegisterHotKey`/`UnregisterHotKey`/`UnloadKeyboardLayout`/`GetLastInputInfo`/`GetKeyboardLayoutName` as the checked set - the page wording confirmed all 10. `SendInput` and `OemKeyScan` are unchecked because their zero/-1 are legal answers (already-blocked input, cannot-be-produced), not reserved failures.
- The two `differs-from-cache` rows (`ToUnicode`, `ToUnicodeEx`) differ from the probe cache only in HTML-entity encoding - the fetched section carries `&lt;` / `&gt;` where the cache stores the decoded `<` / `>`; the wording is otherwise byte-identical, so the docQuote is verbatim-correct and the flag is a false positive of the section extractor (which decodes `&nbsp;`/`&amp;` but not `&lt;`/`&gt;`).
- No code change is proposed; only the verdict triple. `keyboard.cpp` currently has 0 live `CheckReturn` sites, so all 10 `checked` rows would be the first checks in this TU.


## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: pointer rows, strings rows, menu rows.

Slices fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM keyboard`: 34 rows in slices of 10/10/10/4 in table order. 29 of 34 rows have 0 in-tree callers; the other 5 need the caller count in `notes` before a check lands. `GetKeyboardState` is the library's only SAL-annotated prototype (`_Check_return_` at `winuser.h:5934`) - a hint, never a verdict. `GetKeyState`/`GetAsyncKeyState`/`VkKeyScan*` answer their value legitimately - expected `unchecked` with the page's wording.

## keyboard slice 1

Rows 1-10: `ActivateKeyboardLayout`, `BlockInput`, `EnableWindow`, `GetActiveWindow`, `GetAsyncKeyState`, `GetFocus`, `GetKBCodePage`, `GetKeyboardLayout`, `GetKeyboardLayoutList`, `GetKeyboardLayoutName`. One fetch batch of 10, then write those 10 rows.

## keyboard slice 2

Rows 11-20: `GetKeyboardState`, `GetKeyboardType`, `GetKeyNameText`, `GetKeyState`, `GetLastInputInfo`, `IsWindowEnabled`, `keybd_event`, `LoadKeyboardLayout`, `MapVirtualKey`, `MapVirtualKeyEx`. One fetch batch of 10, then write those 10 rows.

## keyboard slice 3

Rows 21-30: `OemKeyScan`, `RegisterHotKey`, `SendInput`, `SetActiveWindow`, `SetFocus`, `SetKeyboardState`, `ToAscii`, `ToAsciiEx`, `ToUnicode`, `ToUnicodeEx`. One fetch batch of 10, then write those 10 rows.

## keyboard slice 4

Rows 31-34: `UnloadKeyboardLayout`, `UnregisterHotKey`, `VkKeyScan`, `VkKeyScanEx`. One fetch batch of 4, then write those 4 rows.
