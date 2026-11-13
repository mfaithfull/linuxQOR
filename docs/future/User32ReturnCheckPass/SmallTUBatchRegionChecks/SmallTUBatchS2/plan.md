---
Type: Plan
Title: "Small TU batch S2"
Parent: "../plan.md"
Level: 2
Items:
  - Title: filledshapes sites
    Status: Atomic
    Content:
  - Title: buttoncontrol sites
    Status: Atomic
    Content:
  - Title: synchronization sites
    Status: Atomic
    Content:
  - Title: combobox sites
    Status: Atomic
    Content:
  - Title: timer sites
    Status: Atomic
    Content:
  - Title: windowprocedure sites
    Status: Atomic
    Content:
  - Title: coordtransform sites
    Status: Atomic
    Content:
---

# Small TU batch S2

Slug: `SmallTUBatchS2` - from title "Small TU batch S2" (no drop-words, PascalCased, 14 chars, ASCII); the same folder name the verdict-side tree uses, so the code batch and the fetch batch cover identical row sets. Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **18 sites** of these 7 small TUs, one item per TU, each item closing with that TU's re-census.

## Resume

Cursor: this plan is **terminal** - all 7 items CLOSED **12 Nov 2026** (`filledshapes`, `buttoncontrol`, `synchronization`, `combobox`, `timer`, `windowprocedure`, `coordtransform`): **13** `CheckReturn< sdkRetType, Check >::TType` tokens landed over the 6 TUs that own a `checked` row, **5** `unchecked` rows carrying their written reason at the site, `windowprocedure.cpp` gaining 0 tokens and 0 include (its 2 rows are `unchecked`), and one `../returncheck.h` include on each of the other 6 TUs. Hard precondition met: the batch's 18 rows carry verdicts (`tmp/_tmp_u2_batch_counts.txt` S2 TOTAL rows=18 filled=18 checked=13 unchecked=5). Re-census `tmp/_tmp_u2_code_census.ps1` -> `tmp/_tmp_u2_code_census.txt` `tus=7 fail=0` and standalone `/W4` compile `tmp/_tmp_u2_gate_tus.ps1` -> `tmp/_tmp_u2_gate_tus.out.txt` `tus=7 fail=0` at exit 0 / 0 diagnostics, both over these 7 TUs; `registers/u2.md` ticked the same session.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the parent's (`../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, verbatim quote in the site comment, one include per TU, the 5 seeds never re-edited, the per-TU re-census stop condition. Sibling titles in the parent: Small TU batch S1, Small TU batch S3, Small TU batch S4, Small TU batch S5.

Row sets per TU are the same rows the verdict-side `SmallTUBatchS2` filled (`tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt`, `ITEM <tu> slice 1`). `GetComboBoxInfo` carries **3** in-tree callers - its `checked` row needs the site-level read before the token moves. Sites within a TU are edited in source line order.

## filledshapes sites

3 sites: `FillRect`, `FrameRect`, `InvertRect` (`filledshapes.cpp`). Same procedure, then re-census that TU.

## buttoncontrol sites

3 sites: `CheckDlgButton`, `CheckRadioButton`, `IsDlgButtonChecked` (`buttoncontrol.cpp`). Same procedure, then re-census that TU.

## synchronization sites

2 sites: `MsgWaitForMultipleObjects`, `MsgWaitForMultipleObjectsEx` (`synchronization.cpp`). The documented failure is `0` with the waited-for value as the answer - expect no token plus the written reason. Re-census after.

## combobox sites

3 sites: `DlgDirListComboBox`, `DlgDirSelectComboBoxEx`, `GetComboBoxInfo` (`combobox.cpp`). `GetComboBoxInfo` is a **3-caller** row - re-read the call sites first. Re-census after.

## timer sites

2 sites: `KillTimer`, `SetTimer` (`timer.cpp`). Same procedure, then re-census that TU.

## windowprocedure sites

2 sites: `CallWindowProc`, `DefWindowProc` (`windowprocedure.cpp`). These take `LPARAM`/`LRESULT` - the class base must be the row's `sdkRetType`, not the published spelling. Re-census after.

## coordtransform sites

3 sites: `ClientToScreen`, `ScreenToClient`, `MapWindowPoints` (`coordtransform.cpp`). Same procedure, then re-census that TU.
