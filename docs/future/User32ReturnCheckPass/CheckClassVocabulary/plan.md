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

Cursor: **terminal** - all 3 items `Atomic`, all 3 closed **12 Nov 2026**, and the whole item 2 of the parent is what made this plan runnable: the table's verdict columns are complete at **597 of 597**.

Item 1 **Name the classes the verdicts need** `CLOSED 12 Nov 2026` - `tmp/_tmp_u2_vocab_needs.ps1` -> `tmp/_tmp_u2_vocab_needs.txt` (recreate by slug) re-reads at **fail=0**: `checked rows = 352`, `distinct row types = 27`, `distinct classes in use = 6`, and the residual line reads **`classes to write (residual) = none - every checked row names a class already in returncheck.h`** / `rows naming a class absent from the header = 0`. The six classes in use, with the base spellings the audit measured: `BoolCheck` 236 rows (base `BOOL`), `CheckNonZeroHandle` 54 (all 18 bases: `HWND` x11, `HICON` x6, `HDESK` x5, `HMENU` x4, `HDC` x4, `HANDLE` x4, `HCURSOR` x3, `HWINSTA` x3, `HKL` x2, `HDWP` x2, `HPOWERNOTIFY` x2, `HACCEL` x2, `HBITMAP` x1, `HDEVNOTIFY` x1, `HHOOK` x1, `HSYNTHETICPOINTERDEVICE` x1, `HWINEVENTHOOK` x1, `DPI_AWARENESS_CONTEXT` x1), `CheckNonZero` 35 (`int` x20, `UINT` x5, `WORD` x3, `DWORD` x2, `UINT_PTR` x2, `LRESULT` x1, `ATOM` x2), `TCheckFailureValue` 19 (`UINT` x8, `DWORD` x3, `int` x3, `BOOL` x2, `long` x2, `LRESULT` x1), `TSuccessCheck` 7 (`LONG` x7), `TRangeCheck` 1 (`UINT` x1) - so 6 of the header's 12 classes are used and 6 (`HandleCheck`, `HandleNullCheck`, `SuccessCheck`, `LongSuccessCheck`, `CheckFailureInt`, `TCheckNotLess`) are unused by any row.

Item 2 **Write the missing check classes** `CLOSED 12 Nov 2026` with **0 edits** - the residual set is empty, so `returncheck.h` is untouched and, per this item's own rule, **the all-libraries gate stays deferred** and the parent's "Folder gate and census" item runs only the 54-source folder gate.

Item 3 **Discharge the LongSuccessCheck override hazard** `CLOSED 12 Nov 2026` - `tmp/_tmp_u2_ovr_probe_neg2.exe` re-prints **`BASE(Test 1) -> false`** (plus `call bound to Test, passed=0`), confirming `LongSuccessCheck`'s `Test (intptr_t&)` does not override the base's `Test (long&)` on x64; the class re-reads **unedited** at `returncheck.h:161-170` (`class LongSuccessCheck : public AutoErrorCheck< long >` with `virtual bool Test(intptr_t& value)`), and no row anywhere names it - the table's `checkClass` column holds `LongSuccessCheck` **0** times (the 2 string hits are `notes` prose), so the routing is honoured: all **15** `LONG`-typed rows end as 7 `TSuccessCheck< LONG >` (the `ERROR_SUCCESS` / `DISP_CHANGE_SUCCESSFUL` APIs, where success **is** zero and `TSuccessCheck`'s `Test` at `:181` passes only at 0 - the fit re-checked against the pages, not assumed), 2 `TCheckFailureValue< long, -1 >` (the `BroadcastSystemMessage` pair, -1 reserved), 6 `unchecked`. No new open item was needed: no `LONG` verdict requires the exact `== SUCCEEDED` shape.

## Context

Inherited from the parent (`../plan.md`) "Check class vocabulary" item and its locked decisions: the rule is D51's decision process gated by the session-27 soundness test; G6 holds (`qor_pp_parameter_checking`/`qor_pp_yes` defined nowhere, so `returncheck.h:19`'s `#if` reads `0 == 0` and `::TType` is the **checked** wrapper type) - a check landed anywhere is live on every call, so a class that does not fit is a runtime behaviour change, not a comment.

Measured return mix over the 597 sites (from `tmp/_tmp_u2_site_census.csv`): BOOL 323, int 53, HWND 34, UINT 30, DWORD 22, LONG 15, LRESULT 13, void 9, then HMENU/WORD 7 each, HICON/LPSTR/HANDLE 6, HCURSOR/HDESK 5, HDC/LPWSTR/SHORT/DPI_AWARENESS_CONTEXT 4, HKL/DPI_HOSTING_BEHAVIOR/HWINSTA/HMONITOR 3 and 30 types at 1-2 each. Sibling titles in the parent: Site census tooling, Endpoint return semantics, Small TU batch region checks, Large TU batch region checks, Folder gate and census, Records and commit.

`returncheck.h` already carries: `BoolCheck`, `HandleCheck` (fails on `INVALID_HANDLE_VALUE`), `HandleNullCheck` (fails on -1 or 0), `CheckNonZeroHandle<T>`, `SuccessCheck`, `LongSuccessCheck`, `TSuccessCheck<T>`, `CheckNonZero<T>`, `CheckFailureInt<iFailure>`, `TCheckFailureValue<T,iFailure>`, `TRangeCheck<T,Min,Max>`, `TCheckNotLess<T,Min>`. Measured precedent in this library: `CheckReturn< DWORD, CheckNonZero< DWORD > >` at `processthread.cpp:87`/`:97`/`:115` and `CheckReturn< BOOL, BoolCheck >` at `errorhandling.cpp:85` and `systemshutdown.cpp:134`.

## Name the classes the verdicts need

One pass over the filled verdict columns of `tmp/_tmp_u2_endpoint_table.csv`: group the `checked` rows by `sdkRetType`, name the class each needs from the list above, and print the residual set - the return types with **no** class that fits. Expected from the mix: `BOOL` -> `BoolCheck`; `HWND`/handle types -> `HandleCheck`/`HandleNullCheck` (only where NULL or `INVALID_HANDLE_VALUE` is documented as failure and not a legal answer); numeric counts -> `CheckNonZero< T >`; documented-sentinel APIs -> `TCheckFailureValue< T, iFailure >`. Print the residual against `tmp/_tmp_u2_vocab_needs.txt` (recreate by slug). Stop condition: that file re-reads with every `checked` row carrying a named class and the residual set printed empty or type by type.

## Write the missing check classes

Only the residual set gets new classes, drawn from `BoolCheck`, `HandleCheck`, `HandleNullCheck`, `CheckNonZero`, `TCheckFailureValue`, `TSuccessCheck` and written in `returncheck.h` in the file's own shape (`class X : public AutoErrorCheck< sdk_return_type > { public: virtual bool Test (sdk_return_type& value) { ... } };`) - the base must equal the row's `sdkRetType`, never the published spelling. **This item moves a shared header, so the all-libraries gate comes into play at the close** (the U1 tree deferred it because 0 headers moved); the parent's "Folder gate and census" item must then run the 122-source gate as well as the 54-source folder gate. If the residual set is empty, this item lands 0 edits and the all-libraries gate stays deferred. Stop condition: the header re-reads with each written class present in the file's own shape, and the gate leg that applied re-reads at its expected source count.

## Discharge the LongSuccessCheck override hazard

Empirically measured, not inferred: `tmp/_tmp_u2_ovr_probe_neg*.cpp`/`.exe` prints `BASE(Test 1) -> false` for `LongSuccessCheck : public AutoErrorCheck< long >`, because its `Test (std::intptr_t&)` does **not** override the base's `Test (long&)` on x64 (`intptr_t` is `long long`). Decision: **route the 15 `LONG` sites away from `LongSuccessCheck`** - use `TSuccessCheck< long >` or `CheckNonZero< long >` whichever the row's documented failure value supports - and do **not** edit that class, since it is `shell`/`kernel`'s and editing it would change their behaviour and force the all-libraries gate. Record the hazard itself as a new open item only if a `LONG` verdict turns out to need the exact `== SUCCEEDED` shape, in which case the new class is written under the item above rather than the base being patched. Stop condition: the probe binary's printed line re-reads `BASE(Test 1) -> false` and the class itself re-reads unedited.
