---
Type: Plan
Title: "strings rows"
Parent: "../plan.md"
Level: 4
Items:
  - Title: strings slice 1
    Status: Atomic
    Content:
  - Title: strings slice 2
    Status: Atomic
    Content:
  - Title: strings slice 3
    Status: Atomic
    Content:
  - Title: strings slice 4
    Status: Atomic
    Content:
---

# strings rows

Slug: `StringsRows` - from title "strings rows" (no drop-words, PascalCased, 11 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **34** `strings.cpp` rows in 4 slices of at most 10 URLs.

## Resume

Cursor: **terminal** - all 4 items `Atomic`, all 4 closed 12 Nov 2026. All **34** rows of `strings.cpp` carry the verdict triple: `checked 10 / unchecked 24 / void 0`, `tmp/_tmp_u2_tu_state.ps1 strings.cpp` `rows=34 checked=10 unchecked=24 void=0 blank=0 shape_bad=0`; the folder single-pass audit `tmp/_tmp_u2_folder_audit.ps1` `rows=597 filled=467 checked=268 unchecked=191 void=8 sum=467 class-shape violations=0 filled rows with no docQuote=0`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: pointer rows, keyboard rows, menu rows.

Slices fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM strings`: 34 rows in slices of 10/10/10/4 in table order. All 34 rows here have 0 in-tree callers. The `A`/`W` pairs (`CharLowerA`/`CharLowerW`, `OemToCharA`/`OemToCharW`, ...) bind one bare stem each, so one page covers the pair per D18 - but **both** rows still need their own verdict and quote written. The `IsChar*` rows are answer-`BOOL` and the `Char*` rows return the copy count, so most rows here end `unchecked` with the page's wording.

## strings slice 1

Rows 1-10: `CharLowerA`, `CharLowerW`, `CharLowerBuffA`, `CharLowerBuffW`, `CharNextA`, `CharNextW`, `CharNextExA`, `CharPrevA`, `CharPrevW`, `CharPrevExA`. One fetch batch of 10, then write those 10 rows.

## strings slice 2

Rows 11-20: `CharToOemA`, `CharToOemW`, `CharToOemBuffA`, `CharToOemBuffW`, `CharUpperA`, `CharUpperW`, `CharUpperBuffA`, `CharUpperBuffW`, `IsCharAlphaA`, `IsCharAlphaW`. One fetch batch of 10, then write those 10 rows.

## strings slice 3

Rows 21-30: `IsCharAlphaNumericA`, `IsCharAlphaNumericW`, `IsCharLowerA`, `IsCharLowerW`, `IsCharUpperA`, `IsCharUpperW`, `LoadStringA`, `LoadStringW`, `OemToCharA`, `OemToCharW`. One fetch batch of 10, then write those 10 rows.

## strings slice 4

Rows 31-34: `OemToCharBuffA`, `OemToCharBuffW`, `wvsprintfA`, `wvsprintfW`. One fetch batch of 4, then write those 4 rows.


## Cursors

strings slice 1: {"tu":"strings.cpp","slice_start":0,"slice_len":10,"done":true}
strings slice 2: {"tu":"strings.cpp","slice_start":10,"slice_len":10,"done":true}
strings slice 3: {"tu":"strings.cpp","slice_start":20,"slice_len":10,"done":true}
strings slice 4: {"tu":"strings.cpp","slice_start":30,"slice_len":4,"done":true}

## Closed 12 Nov 2026 - evidence

- Four fetch batches, all `bad=0 no-page=0 VERDICT fail=0`, every row `match-cache` at HTTP 200: `tmp/_tmp_u2_str_fetch.out.txt` (10), `_s2` (10), `_s3` (10), `_s4` (4). New instrument `tmp/_tmp_u2_str_fetch.ps1` cloned from the keyboard one with `want=strings.cpp`, report slug `str`, and the fixed `[Math]::Floor($skip / $take) + 1` report tag.
- 34 verdict lines appended to `tmp/_tmp_u2_decisions.txt` (the `strings.cpp` block, 5 fields each) and written by `tmp/_tmp_u2_patch.ps1` (`decisions with no table row = 0`).
- The `A`/`W` twins bind one page each (D18): `CharLowerA`/`CharLowerW`, `CharLowerBuffA`/`CharLowerBuffW`, `CharNextA`/`CharNextW`, `CharPrevA`/`CharPrevW`, `CharToOemA`/`CharToOemW`, `CharToOemBuffA`/`CharToOemBuffW`, `CharUpperA`/`CharUpperW`, `CharUpperBuffA`/`CharUpperBuffW`, `IsCharAlpha*`, `IsCharAlphaNumeric*`, `IsCharLower*`, `IsCharUpper*`, `LoadString*`, `OemToChar*`, `OemToCharBuff*`, `wvsprintf*` - each pair's Return value section is word-for-word identical on the page, and **both** rows carry their own verdict and docQuote. The `ExA` variants (`CharNextExA`, `CharPrevExA`) have their own pages with the `lpCurrentChar`/`lpStart` parameter naming.
- Deviation from this leaf's `## Context`, which predicted "most rows here end unchecked": measured, **10** rows are checked, not "most unchecked". The checked set is the `CharToOem`/`OemToChar` family (8 rows, `BoolCheck`) - their shared page reserves zero outright: "The return value is always nonzero except when you pass the same address to lpszSrc and lpszDst in the wide-character version of the function. In this case the function returns zero and GetLastError returns ERROR_INVALID_ADDRESS ." - plus `LoadStringA`/`LoadStringW` (`CheckNonZero< int >`, the count with "Zero if the string resource does not exist").
- The pages split the family by whether GetLastError is allowed: the `CharLower`/`CharUpper` pages say "There is no indication of success or failure. Failure is rare. There is no extended error information for this function; do not call GetLastError ." - so D51 step 2 reads no-check outright, not a judgement - while the `*ToOem*` pages name `ERROR_INVALID_ADDRESS` and take a check.
- All six `IsChar*` rows are unchecked: the zero answers the caller's question about the character it passed (punctuation, a digit, or the wrong case is a legal state), so keying `BoolCheck` would report a legal answer as a failure - same shape as the `IsClipboardFormatAvailable`, `IsWindowArranged` and `TranslateMessage` rows. The `Char*Buff*` count rows are unchecked on the opposite half: the page names **no** failure value at all ("The return value is the number of characters processed"), whereas `LoadString`'s page does name one.
- Two vocabulary needs flagged for the `CheckClassVocabulary` leaf from this TU: `wvsprintfA`/`wvsprintfW` - "If the function fails, the return value is less than the length of the expected output" is a **runtime** bound the caller derives from its format string, so `TRangeCheck` (`returncheck.h:232`) and `TCheckNotLess` (`returncheck.h:251`), which take compile-time template `Min`/`Max`, cannot key on it; and the `LoadString*` empty-resource caveat recorded in both rows' `notes` for the code-side leaf.
- No code change is proposed here; only the verdict triple. All 34 rows report 0 in-tree callers, so every row is booked change-safe and no call-site audit is owed.