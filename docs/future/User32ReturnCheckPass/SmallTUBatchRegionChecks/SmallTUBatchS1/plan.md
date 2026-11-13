---
Type: Plan
Title: "Small TU batch S1"
Parent: "../plan.md"
Level: 2
Items:
  - Title: printing sites
    Status: Atomic
    Content:
  - Title: security sites
    Status: Atomic
    Content:
  - Title: devicemanagement sites
    Status: Atomic
    Content:
  - Title: brush sites
    Status: Atomic
    Content:
  - Title: bitmap sites
    Status: Atomic
    Content:
  - Title: resource sites
    Status: Atomic
    Content:
  - Title: console sites
    Status: Atomic
    Content:
---

# Small TU batch S1

Slug: `SmallTUBatchS1` - from title "Small TU batch S1" (no drop-words, PascalCased, 14 chars, ASCII); the same folder name the verdict-side tree uses, so the code batch and the fetch batch cover identical row sets. Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **10 sites** of the 7 smallest TUs, one item per TU, each item closing with that TU's re-census.

## Resume

Cursor: **all 7 items done** - this plan reached its terminal state on 12 Nov 2026. Measured at that write: 10 sites decided, **8** `CheckReturn< sdkRetType, Check >::TType` landed (`PrintWindow` `BoolCheck`, `GetUserObjectSecurity`/`SetUserObjectSecurity` `BoolCheck`, `RegisterDeviceNotification` `CheckNonZeroHandle< HDEVNOTIFY >`, `UnregisterDeviceNotification` `BoolCheck`, `LoadBitmap` `CheckNonZeroHandle< HBITMAP >`, `CopyImage`/`LoadImage` `CheckNonZeroHandle< HANDLE >`) and **2** `unchecked` rows carrying the written reason at the site (`GetSysColorBrush`, `ConsoleControl`); per-TU re-census `tmp/_tmp_u2_code_census.ps1` -> `tmp/_tmp_u2_code_census.txt` read `tus=7 fail=0`; standalone `/W4` compile `tmp/_tmp_u2_gate_tus.ps1` -> `tmp/_tmp_u2_gate_tus.out.txt` read `tus=7 fail=0` at exit 0 / 0 diagnostics; the `registers/u2.md` row was ticked in the same session. Next in the parent: item 2 **Small TU batch S2**.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the parent's (`../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, verbatim quote in the site comment, one include per TU, the 5 seeds never re-edited, the per-TU re-census stop condition. Sibling titles in the parent: Small TU batch S2, Small TU batch S3, Small TU batch S4, Small TU batch S5.

Row sets per TU are the same rows the verdict-side `SmallTUBatchS1` filled (`tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt`, `ITEM <tu> slice 1`); `inTreeCallers` counts here are all 0, so no caller audit gates these items. Sites within a TU are edited in source line order.

## printing sites

1 site: `PrintWindow` (`printing.cpp`). Land only if the row is `checked`; otherwise write the reason. Re-census `printing.cpp` after.

## security sites

2 sites: `GetUserObjectSecurity`, `SetUserObjectSecurity` (`security.cpp`). Same procedure, then re-census that TU.

## devicemanagement sites

2 sites: `RegisterDeviceNotification`, `UnregisterDeviceNotification` (`devicemanagement.cpp`). Same procedure, then re-census that TU.

## brush sites

1 site: `GetSysColorBrush` (`brush.cpp`). The documented failure value here is also a legal answer, so the expected outcome is no token plus the written reason. Re-census after.

## bitmap sites

1 site: `LoadBitmap` (`bitmap.cpp`). `HBITMAP` handle row - the token lands only if the page named NULL as failure and NULL is not a legal answer. Re-census after.

## resource sites

2 sites: `CopyImage`, `LoadImage` (`resource.cpp`). Same procedure, then re-census that TU.

## console sites

1 site: `ConsoleControl` (`console.cpp`). Same procedure, then re-census `console.cpp`.
