---
Type: Plan
Title: "icon sites"
Parent: "../plan.md"
Level: 3
Items:
  - Title: icon slice 1
    Status: Atomic
    Content:
  - Title: icon slice 2
    Status: Atomic
    Content:
---

# icon sites

Slug: `IconSites` - from title "icon sites" (no drop-words, PascalCased, 9 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **14** `icon.cpp` sites in 2 slices of at most 10 sites.

## Resume

Cursor: **terminal** - both items `Atomic`, both **CLOSED 12 Nov 2026**. Item 2 **icon slice 2** landed `LoadIcon`'s, `LookupIconIdFromDirectoryEx`'s and left `PrivateExtractIcons` (the TU's only `unchecked` row) untouched; the per-TU re-census stop condition reads `tmp/_tmp_u2_code_census.ps1 icon.cpp` `rows=14 fcontext=14 useswinapi=14 (11 bare + 3 AW) LibraryCall=14 Win32ErrorHandler=14 CheckReturn-code=13 table-checked=13 table-unchecked=1 returncheck-includes=1  ok` (`tus=1 fail=0`).

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, caller audit for nonzero counts, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: mouse sites, rectangle sites, scrollbar sites, touch sites, windowclass sites, cursor sites.

Slice boundaries fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM icon`. `CopyIcon` carries **6** in-tree callers - its token waits for the site-level read before it moves.

## icon slice 1

10 sites in `icon.cpp`: `CopyIcon`, `CreateIcon`, `CreateIconFromResource`, `CreateIconFromResourceEx`, `CreateIconIndirect`, `DestroyIcon`, `DrawIcon`, `DrawIconEx`, `GetIconInfo`, `GetIconInfoEx`. Land only the `checked` rows; re-read `CopyIcon`'s call sites before its token. Stop condition: re-read the sites, then re-census `icon.cpp` against its `checked` row count.

## icon slice 2

4 sites in `icon.cpp`: `LoadIcon`, `LookupIconIdFromDirectory`, `LookupIconIdFromDirectoryEx`, `PrivateExtractIcons`. Stop condition: re-read the sites, then re-census `icon.cpp` - its counts must already match the slice-1 census.
