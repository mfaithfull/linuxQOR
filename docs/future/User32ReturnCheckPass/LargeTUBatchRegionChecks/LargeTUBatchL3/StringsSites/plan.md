---
Type: Plan
Title: "strings sites"
Parent: "../plan.md"
Level: 3
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

# strings sites

Slug: `StringsSites` - from title "strings sites" (no drop-words, PascalCased, 12 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **34** `strings.cpp` sites in 4 slices of at most 10 sites.

## Resume

Cursor: **terminal** - all 4 items `Atomic`, all 4 closed 12 Nov 2026. Item 4 **strings slice 4** is **CLOSED 12 Nov 2026**: both `checked` rows (`OemToCharBuffA`, `OemToCharBuffW`) carry the `CheckReturn< BOOL, BoolCheck >` token and both `wvsprintf*` rows carry a policy block with a bare return - `strings.cpp` now reads `lines=562 policy-blocks=34 tokens=10 bare-Call-lines=24 real-returncheck-includes=1` and its census reads `rows=34 fcontext=34 useswinapi=34 (34 bare + 0 AW) LibraryCall=34 Win32ErrorHandler=34 CheckReturn-code=10 table-checked=10 table-unchecked=24 returncheck-includes=1 ok` (`tus=1 fail=0`) - 10 tokens + 24 bare = 34 = rows, so the leaf is complete. Item 3 **strings slice 3** is **CLOSED 12 Nov 2026**: all 10 slice-3 sites carry a `//Return policy` block and the 4 `checked` rows there (`LoadStringA`, `LoadStringW`, `OemToCharA`, `OemToCharW`) carry the token - `strings.cpp` now reads `lines=529 policy-blocks=30 tokens=8 real-returncheck-includes=1` and its census reads `rows=34 fcontext=34 useswinapi=34 (34 bare + 0 AW) LibraryCall=34 Win32ErrorHandler=34 CheckReturn-code=8 table-checked=10 table-unchecked=24 returncheck-includes=1 check-FAIL` (`tus=1 fail=1`) - the 2/10 gap is owed by item 4 alone (`OemToCharBuffA`, `OemToCharBuffW`). Item 2 **strings slice 2** is **CLOSED 12 Nov 2026**: all 10 slice-2 sites carry a `//Return policy` block and the 4 `checked` rows (`CharToOemA`, `CharToOemW`, `CharToOemBuffA`, `CharToOemBuffW`) carry the token - `strings.cpp` now reads `lines=468 policy-blocks=20 tokens=4 bare-Call-lines=30 real-returncheck-includes=1` and its census reads `rows=34 fcontext=34 useswinapi=34 (34 bare + 0 AW) LibraryCall=34 Win32ErrorHandler=34 CheckReturn-code=4 table-checked=10 table-unchecked=24 returncheck-includes=1 check-FAIL` (`tus=1 fail=1`) - the 4/10 gap is this leaf's mid-state, owed by slices 3-4 (4 + 2 = 6 tokens), and the TU's one include landed at `strings.cpp:12` here, its first token. Item 1 **strings slice 1** is **CLOSED 12 Nov 2026**: all 10 slice-1 sites carry a `//Return policy` block and **none** carries a token, because all 10 rows read `unchecked` (the `CharLowerA/W` pair state "There is no indication of success or failure. Failure is rare. There is no extended error information for this function; do not call GetLastError ."; `CharLowerBuffA/W` return a count whose zero is a legal empty-buffer answer; the four `CharNext*`/`CharPrev*` rows return a position answer with no failure value named), so the TU gained no include there. Terminal: all 4 items `Atomic`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: pointer sites, keyboard sites, menu sites. All 34 rows here have 0 in-tree callers. The `IsChar*` rows are answer-`BOOL` and the `Char*` rows return the copy count, so most rows here stay `unchecked` with the reason.

## strings slice 1

10 sites in `strings.cpp`: `CharLowerA`, `CharLowerW`, `CharLowerBuffA`, `CharLowerBuffW`, `CharNextA`, `CharNextW`, `CharNextExA`, `CharPrevA`, `CharPrevW`, `CharPrevExA`. Land only the `checked` rows. Stop condition: re-read the sites, then re-census `strings.cpp` against its `checked` row count.

## strings slice 2

10 sites in `strings.cpp`: `CharToOemA`, `CharToOemW`, `CharToOemBuffA`, `CharToOemBuffW`, `CharUpperA`, `CharUpperW`, `CharUpperBuffA`, `CharUpperBuffW`, `IsCharAlphaA`, `IsCharAlphaW`. Stop condition: re-read the sites, then re-census `strings.cpp` - its counts must already match the slice-1 census.

## strings slice 3

10 sites in `strings.cpp`: `IsCharAlphaNumericA`, `IsCharAlphaNumericW`, `IsCharLowerA`, `IsCharLowerW`, `IsCharUpperA`, `IsCharUpperW`, `LoadStringA`, `LoadStringW`, `OemToCharA`, `OemToCharW`. Stop condition: re-read the sites, then re-census `strings.cpp` - its counts must already match the slice-2 census.

## strings slice 4

4 sites in `strings.cpp`: `OemToCharBuffA`, `OemToCharBuffW`, `wvsprintfA`, `wvsprintfW`. Stop condition: re-read the sites, then re-census `strings.cpp` - its counts must already match the slice-3 census.

## Cursors

strings slice 1: {"tu":"strings.cpp","slice_start":0,"slice_len":10,"done":true}
strings slice 2: {"tu":"strings.cpp","slice_start":10,"slice_len":10,"done":true}
strings slice 3: {"tu":"strings.cpp","slice_start":20,"slice_len":10,"done":true}
strings slice 4: {"tu":"strings.cpp","slice_start":30,"slice_len":4,"done":true}

## Closed 12 Nov 2026 - evidence

- Leaf terminal: all 34 `strings.cpp` sites carry a `//Return policy` block; the 10 `checked` rows carry the token; the 24 `unchecked` rows keep a bare return; the TU's one `returncheck.h` include sits at `strings.cpp:12` and was already in place from slice 2.
- Census green at the leaf: `strings.cpp rows=34 fcontext=34 useswinapi=34 (34 bare + 0 AW) LibraryCall=34 Win32ErrorHandler=34 CheckReturn-code=10 table-checked=10 table-unchecked=24 returncheck-includes=1 ok`, `tus=1 fail=0`. Arithmetic closes: 10 tokens + 24 bare-call lines = 34 = rows = policy blocks.
- Slice 3 needed a second pass after the first landing: the census read `CheckReturn-code=7` against the expected 8, and the gap was `OemToCharW` (`strings.cpp:482` at that point) - it had neither a policy block nor a token. Landing it took `policy-blocks` 29 -> 30 and `tokens` 7 -> 8. The recorded per-slice arithmetic (4 + 4 + 2 across slices 2-4) is what surfaced the gap rather than the file's own shape, so the census - not the line count - is the arbiter here.
- Class shapes landed in this TU: `BoolCheck` (returncheck.h:94) on the 8 `CharToOem*` / `OemToChar*` rows, and `CheckNonZero< int >` (returncheck.h:187) on `LoadStringA/W`, whose pages state "the return value is the number of characters stored in the buffer" style counts where zero is reserved as "no such message" - the row's `sdkRetType` is `int` so the class base is `int`, matching the `processthread.cpp:87`-style precedent.
- The `wvsprintfA/W` pair is the TU's only "failure without a reserved value" reading: "the return value is less than the length of the expected output" is a comparison against a length the call never takes (the unbounded `vsprintf` shape - no buffer-length argument, which is also why the SDK marks the destination `_Out_`), so no class fits and the rows stay unchecked with a bare return.
- All 34 rows carry 0 in-tree callers for their exact stem, so every block's `//AUDIT` line reads change-safe and no call-site audit was owed in this TU.
- Deviation from the leaf's `## Context`: it predicted the `IsChar*` octet as `unchecked` "answer-shaped BOOL" - measured, all eight do read unchecked, but on the stronger reading that the page names no failure value at all (the BOOL is the answer to "is this character X"), so they join the unchecked set without the soundness test ever failing at a documented sentinel.

