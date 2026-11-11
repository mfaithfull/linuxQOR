---
Type: Plan
Title: "Check class vocabulary"
Parent: "../plan.md"
Level: 1
Items:
  - Title: Name the classes the verdicts need
    Status: Atomic
    Content:
  - Title: Write the missing check classes
    Status: Atomic
    Content:
  - Title: Discharge the LongSuccessCheck override hazard
    Status: Atomic
    Content:
---

# Check class vocabulary

Slug: `CheckClassVocabulary` - from title "Check class vocabulary" (no drop-words, PascalCased, 20 chars, ASCII, inside the 24-char cap). Uniqueness is stored here, not recomputed.

## Goal

Have a check class that fits every `checked` verdict before any batch lands a `CheckReturn< >` - every row's class named from `src/platform/os/windows/api_layer/returncheck.h`, only genuinely missing classes written, and the header's latent signature hazard discharged or routed around.

## Resume

Cursor: item 1 **Name the classes the verdicts need** (`Atomic`) - the first action of this plan. This plan is terminal as planned: its three items are each one concrete action on one named artifact, so the frontier stays in the parent's item 2 until the table's verdict columns are filled.

## Context

Inherited from the parent (`../plan.md`) "Check class vocabulary" item and its locked decisions: the rule is D51's decision process gated by the session-27 soundness test; G6 holds (`qor_pp_parameter_checking`/`qor_pp_yes` defined nowhere, so `returncheck.h:19`'s `#if` reads `0 == 0` and `::TType` is the **checked** wrapper type) - a check landed anywhere is live on every call, so a class that does not fit is a runtime behaviour change, not a comment.

Measured return mix over the 597 sites (from `tmp/_tmp_u2_site_census.csv`): BOOL 323, int 53, HWND 34, UINT 30, DWORD 22, LONG 15, LRESULT 13, void 9, then HMENU/WORD 7 each, HICON/LPSTR/HANDLE 6, HCURSOR/HDESK 5, HDC/LPWSTR/SHORT/DPI_AWARENESS_CONTEXT 4, HKL/DPI_HOSTING_BEHAVIOR/HWINSTA/HMONITOR 3 and 30 types at 1-2 each. Sibling titles in the parent: Site census tooling, Endpoint return semantics, Small TU batch region checks, Large TU batch region checks, Folder gate and census, Records and commit.

`returncheck.h` already carries: `BoolCheck`, `HandleCheck` (fails on `INVALID_HANDLE_VALUE`), `HandleNullCheck` (fails on -1 or 0), `CheckNonZeroHandle<T>`, `SuccessCheck`, `LongSuccessCheck`, `TSuccessCheck<T>`, `CheckNonZero<T>`, `CheckFailureInt<iFailure>`, `TCheckFailureValue<T,iFailure>`, `TRangeCheck<T,Min,Max>`, `TCheckNotLess<T,Min>`. Measured precedent in this library: `CheckReturn< DWORD, CheckNonZero< DWORD > >` at `processthread.cpp:87`/`:97`/`:115` and `CheckReturn< BOOL, BoolCheck >` at `errorhandling.cpp:85` and `systemshutdown.cpp:134`.

## Name the classes the verdicts need

One pass over the filled verdict columns of `tmp/_tmp_u2_endpoint_table.csv`: group the `checked` rows by `sdkRetType`, name the class each needs from the list above, and print the residual set - the return types with **no** class that fits. Expected from the mix: `BOOL` -> `BoolCheck`; `HWND`/handle types -> `HandleCheck`/`HandleNullCheck` (only where NULL or `INVALID_HANDLE_VALUE` is documented as failure and not a legal answer); numeric counts -> `CheckNonZero< T >`; documented-sentinel APIs -> `TCheckFailureValue< T, iFailure >`. Print the residual against `tmp/_tmp_u2_vocab_needs.txt` (recreate by slug).

## Write the missing check classes

Only the residual set gets new classes, written in `returncheck.h` in the file's own shape (`class X : public AutoErrorCheck< sdk_return_type > { public: virtual bool Test (sdk_return_type& value) { ... } };`) - the base must equal the row's `sdkRetType`, never the published spelling. **This item moves a shared header, so the all-libraries gate comes into play at the close** (the U1 tree deferred it because 0 headers moved); the parent's "Folder gate and census" item must then run the 122-source gate as well as the 54-source folder gate. If the residual set is empty, this item lands 0 edits and the all-libraries gate stays deferred.

## Discharge the LongSuccessCheck override hazard

Empirically measured, not inferred: `tmp/_tmp_u2_ovr_probe_neg*.cpp`/`.exe` prints `BASE(Test 1) -> false` for `LongSuccessCheck : public AutoErrorCheck< long >`, because its `Test (std::intptr_t&)` does **not** override the base's `Test (long&)` on x64 (`intptr_t` is `long long`). Decision: **route the 15 `LONG` sites away from `LongSuccessCheck`** - use `TSuccessCheck< long >` or `CheckNonZero< long >` whichever the row's documented failure value supports - and do **not** edit that class, since it is `shell`/`kernel`'s and editing it would change their behaviour and force the all-libraries gate. Record the hazard itself as a new open item only if a `LONG` verdict turns out to need the exact `== SUCCEEDED` shape, in which case the new class is written under the item above rather than the base being patched.
