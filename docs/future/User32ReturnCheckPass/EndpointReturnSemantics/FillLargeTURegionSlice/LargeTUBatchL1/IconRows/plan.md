---
Type: Plan
Title: "icon rows"
Parent: "../plan.md"
Level: 4
Items:
  - Title: icon slice 1
    Status: Atomic
    Content:
  - Title: icon slice 2
    Status: Atomic
    Content:
---

# icon rows

Slug: `IconRows` - from title "icon rows" (no drop-words, PascalCased, 8 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **14** `icon.cpp` rows in 2 slices of at most 10 URLs.

## Resume

This plan is **terminal** - both items `Atomic` and both **CLOSED 12 Nov 2026**. Item 1 **icon slice 1** is **CLOSED 12 Nov 2026**: rows 1-10 decided **checked 10 / unchecked 0 / void 0** on one fetch batch of exactly 10 URLs (`tmp/_tmp_u2_icon_fetch.out.txt` - 10 `FETCH` rows at `http=200`, `fetched=10 of 10`, `bad=0`, `VERDICT fail=0`, every `Return value` section `match-cache` against `tmp/_tmp_u2_probe_cache.txt`; the AW row `GetIconInfoEx` fetched off its corrected `a`-slug page `nf-winuser-geticoninfoexa`, carried by the probe's `resolvedUrl` - `CORRECTED GetIconInfoEx` line). The 5 handle rows `CopyIcon`, `CreateIcon`, `CreateIconFromResource`, `CreateIconFromResourceEx`, `CreateIconIndirect` are `checked` `CheckNonZeroHandle< HICON >` because each page reserves NULL ("If the function fails, the return value is NULL") and a handle answer is never NULL; the 5 status rows `DestroyIcon`, `DrawIcon`, `DrawIconEx`, `GetIconInfo` are `checked` `BoolCheck` on "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero", and `GetIconInfoEx` likewise on the a-slug page's "Type: BOOL TRUE indicates success, FALSE indicates failure." - `DrawIconEx` and `GetIconInfoEx` are AW census rows with NO-PROTOTYPE `sdkRetType`, and `BoolCheck` is unparameterised so no class base is picked. Every one of the 10 rows took the session-27 test: no page gives 0/NULL as a legal answer. Item 2 **icon slice 2** is **CLOSED 12 Nov 2026**: rows 11-14 decided **checked 3 / unchecked 1 / void 0** on one fetch batch of exactly 4 URLs (`tmp/_tmp_u2_icon_fetch_s2.out.txt` - 4 `FETCH` rows at `http=200`, `fetched=4 of 4`, `bad=0`, `VERDICT fail=0`, every `Return value` section `match-cache` against `tmp/_tmp_u2_probe_cache.txt`; the AW rows `LoadIcon` and `PrivateExtractIcons` fetched off their corrected `a`-slug pages `nf-winuser-loadicona` and `nf-winuser-privateextracticonsa`, carried by the probe's `resolvedUrl` - the two `CORRECTED` lines). `LoadIcon` is `checked` `CheckNonZeroHandle< HICON >` on "If the function succeeds, the return value is a handle to the newly loaded icon. If the function fails, the return value is NULL" with the class base taken from the published `HICON` because its `sdkRetType` is NO-PROTOTYPE; `LookupIconIdFromDirectory` and `LookupIconIdFromDirectoryEx` are `checked` `CheckNonZero< int >` on "If the function fails, the return value is zero" where the answer is an `RT_ICON`/`RT_CURSOR` resource identifier that is never 0; `PrivateExtractIcons` is `unchecked` because its page reserves two failure values - "If the function fails then the return value is 0" and "the return value is 0xFFFFFFFF if the file is not found" - while 0 is also the legal count when nothing is extracted, and no existing class covers 0 and 0xFFFFFFFF. Re-census `tmp/_tmp_u2_filled.txt` `icon.cpp rows=14 filled=14 full`; folder-wide `filled = 171` of `597`, `full-tus=37 / partial=0 / empty=0`; `tmp/_tmp_u2_patch.out.txt` `decided = 171`, `checked = 108 / unchecked = 59 / void = 4`, `class-shape violations = 0`, `decisions with no table row = 0`, **fail=0**; `tmp/_tmp_u2_docquotes.txt` `164` rows. Terminal: both items `Atomic`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: mouse rows, rectangle rows, scrollbar rows, touch rows, windowclass rows, cursor rows.

Slices fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM icon`: 14 rows in slices of 10 and 4 in table order. The handle rows here (`CreateIconIndirect`, `GetIconInfo`-shaped) decide only where the page documents NULL/`INVALID_HANDLE_VALUE` as failure and not a legal answer.

## icon slice 1

Rows 1-10: `CopyIcon`, `CreateIcon`, `CreateIconFromResource`, `CreateIconFromResourceEx`, `CreateIconIndirect`, `DestroyIcon`, `DrawIcon`, `DrawIconEx`, `GetIconInfo`, `GetIconInfoEx`. One fetch batch of 10, then write those 10 rows.

## icon slice 2

Rows 11-14: `LoadIcon`, `LookupIconIdFromDirectory`, `LookupIconIdFromDirectoryEx`, `PrivateExtractIcons`. One fetch batch of 4, then write those 4 rows.
