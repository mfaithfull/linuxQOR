---
Type: Plan
Title: "Resolve U2 - return checks at the winqor_api_user wrapper call sites"
Parent: ""
Level: 0
Items:
  - Title: Site census tooling
    Status: Atomic
    Content:
  - Title: Endpoint return semantics
    Status: Drafted
    Content: ./EndpointReturnSemantics/plan.md
  - Title: Check class vocabulary
    Status: Planning
    Content:
  - Title: Small TU batch region checks
    Status: Planning
    Content:
  - Title: Large TU batch region checks
    Status: Planning
    Content:
  - Title: Folder gate and census
    Status: Atomic
    Content:
  - Title: Records and commit
    Status: Planning
    Content:
---

# Resolve U2 - return checks at the winqor_api_user wrapper call sites

Slug: `User32ReturnCheckPass` - from the request title "Resolve U2 - return checks at the winqor_api_user wrapper call sites" (drop-words `the`/`a`/`an`/`of` removed from the request phrase, PascalCased, ASCII, 21 chars, inside the 24-char cap; no collision with the sibling `User32ErrorHandlerPass`). Uniqueness is stored here, not recomputed.

## Goal

Make the U2 return-check policy resolve at **all 597 wrapper call sites** of `winqor_api_user` - each site ending in one of exactly three states (checked with a named `CheckReturn< ret, Check >`, deliberately unchecked with a written reason tied to its SDK failure value, or `void` with nothing to check) - with the decision for every site carried in one reusable per-endpoint table, so the code set lands in TU batches behind the U1 tree's proven boundaries.

## Resume (state slot - the only mutable section of this plan)

Cache, not record: the front matter owns structure, the disk owns truth, this slot only makes the resume cheap. Read the front matter and this slot, re-derive from disk, rewrite this slot if they disagree. Rotate in place - never append below this block.

- `Cursor: plan.md -> Endpoint return semantics -> ./EndpointReturnSemantics/plan.md -> Fill the small TU region slice`
  Root pre-order: item 1 `Site census tooling` is Atomic and its instrument is measured clean; item 2 is `Drafted`, so the descent inside it is the frontier. Everything left of the cursor is done, everything right of it is not.
- `Measured at this write: sites 597 / 597 anchors; stems 597 (479 useswinapi + 118 AW); NO-PROTOTYPE 125; SAL return annotations on bound prototypes 1; live CheckReturn sites 5 over 3 TUs; table rows 597 with verdict columns empty; caller counts 396 zero / 201 nonzero`
  Rewrite this line the moment a re-census verifies, and tick the matching row of the `registers/u2.md` working record in the same session (the U17 lesson).
- `Next action: fill the small-TU-region slice of the table` (124 rows), not the code - no `CheckReturn` token moves before its row has a verdict with a verbatim quote.
- `Cursor check: tmp/_tmp_u2_tree_verify.ps1` - walks the chain against the front matter, re-runs the site census, prints the per-batch ledger, and reports any gap left of the cursor or any checked site right of it.
- `Blocking precondition: no record write and no commit until the dated folder gate reads 54/54 exit 0 with 0 errors / 0 warnings` - the precondition line at the foot of `## Records and commit` is the authority, not this slot.

## Context

- U2 register `src/platform/os/windows/api_layer/user/registers/u2.md` - policy settled 29 Oct 2026 as "accepted as-is, no `CheckReturn` introduced by a review session"; the experiment 12 Nov 2026 (**D51**) opened the return-check half **only where the SDK reserves a failure value** and landed one site (`ShutdownBlockReasonDestroy`, `systemshutdown.cpp:134`, `CheckReturn< BOOL, BoolCheck >::TType`). **U2 stays open** at this writing: 596 sites unchecked.
- The decision process D51 applies (the only rule this tree enforces, never restated below): `void`/`VOID` -> no check; an API that reserves **no** return value for errors -> no check; otherwise yes, and the class is the `AutoErrorCheck< sdk_return_type >` subclass whose `Test` returns true for passing values and false for failing ones - `BOOL` with FALSE meaning error takes `CheckReturn< BOOL, BoolCheck >`, and a missing class gets written. The session-27 soundness test is the gate inside step 2: a check is sound only where the SDK's failure value is **not** also a legal success value.
- Measured 12 Nov 2026 with `tmp/_tmp_u2_site_census.ps1` -> `tmp/_tmp_u2_site_census.csv` / `.txt` (recreate by slug): **597** definition heads over the 53 wrapper TUs == the 597 anchor invariant, **597** distinct bound stems (479 `qor_pp_useswinapi` + 118 `qor_pp_useswinapiAW`); return mix **BOOL 323, int 53, HWND 34, UINT 30, DWORD 22, LONG 15, LRESULT 13, void 9**, then HMENU/WORD 7 each, HICON/LPSTR/HANDLE 6, HCURSOR/HDESK 5, HDC/LPWSTR/SHORT/DPI_AWARENESS_CONTEXT 4, HKL/DPI_HOSTING_BEHAVIOR/HWINSTA/HMONITOR 3, and 30 types at 1-2 (UINT_PTR, INT_PTR, ULONG_PTR, LONG_PTR, LPARAM, ATOM, HDWP, HACCEL, HPOWERNOTIFY, NTSTATUS, HHOOK, HWINEVENTHOOK, HBRUSH, HBITMAP, HDEVNOTIFY, DPI_AWARENESS, DIALOG_*_DPI_CHANGE_BEHAVIORS, HSYNTHETICPOINTERDEVICE ...).
- **The SDK header is not a shortcut, measured twice**: over the 597 bound stems only **1** carries an SAL return annotation (`GetKeyboardState`, `_Check_return_` at `winuser.h:5934`), and the whole `um` header has only **5** annotated names in total (`_Check_return_`/`_Ret_...`) - instruments `tmp/_tmp_u2_sal_map.ps1` -> `tmp/_tmp_u2_sal_map.txt` and `tmp/_tmp_u2_site_census.ps1` -> `tmp/_tmp_u2_site_census.csv`/`.txt` (recreate by slug). So the Microsoft page per endpoint is the authority for **596 of 597** stems, and the table's doc reference is load-bearing, not decorative. The header does carry `WINAPI` parameter SAL (`_In_`, `_Out_writes_`) plus the `_Ret_range_` family on a handful of `UINT`/`int` counts, which is a hint to read but never a verdict.
- The prototype index now walks back to a blank line or `#` (the fixed 9-line window mis-attributed neighbours' SALs); with that fix **NO_PROTOTYPE = 125** of 597 (the folder's own coverage census `_tmp_u18_coverage4.ps1` puts the no-prototype exclusion bucket at 286 of 1,018 exports), and the 472 resolved prototypes are the source of the `sdkRetType` column.
- `returncheck.h` already carries the vocabulary: `BoolCheck`, `HandleCheck` (fails on `INVALID_HANDLE_VALUE`), `HandleNullCheck` (fails on -1 or 0), `CheckNonZeroHandle<T>`, `SuccessCheck`, `LongSuccessCheck`, `TSuccessCheck<T>`, `CheckNonZero<T>`, `CheckFailureInt<iFailure>`, `TCheckFailureValue<T,iFailure>`, `TRangeCheck<T,Min,Max>`, `TCheckNotLess<T,Min>` - so most verdicts name an **existing** class and the "write a new class" branch is expected to be rare.
- G6 holds (`qor_pp_parameter_checking`/`qor_pp_yes` defined nowhere, so `returncheck.h:19`'s `#if` evaluates `0 == 0` and `::TType` is the **checked** wrapper type): a check added anywhere is live on **every** call, so a wrong check is a runtime behaviour change, not a comment.
- U1's tree `docs/future/User32ErrorHandlerPass/plan.md` is the sibling that established the batch regions (33 small TUs / 124 sites, 19 large TUs / 468 sites, `window.cpp` 86) and the gate line at `status.md:158-165`; this tree reuses those boundaries so the two passes are comparable, and D50 already put `Win32ErrorHandler _;` at all 597 sites - the check half is the only remaining half.
- 201 of the 597 bound stems have **at least one** in-tree reference and **396** have none (measured 12 Nov 2026 by `tmp/_tmp_u2_endpoint_table.ps1`, scanning `src`/`ui`/`opengl` for `User32::` outside this folder; counts include comments and other libraries' declarations, so they are an upper bound). Only the 0-caller surface (the `ShutdownBlockReason*` family as in D51) is change-safe without a caller audit, so the caller column is load-bearing; `registers/u19.md` stays the authority for the call-site list itself.

## Decisions

- **D51** (`docs/agent/decisions/D51.md`) is the settled rule and the proven site shape; every later D-entry extends it rather than restating it.
- A site is **not** checked unless its own SDK page documents a failure value that is not a legal success value. The expected outcome is therefore a **small** code set against 597 sites: the zero-is-legal counts/positions/handles (`GetScrollPos`, `GetWindowTextLength`, `WindowFromPoint`), the answer-shaped `BOOL` (`EqualRect`, `IsRectEmpty`, `PtInRect`), the previous-state returns (`ShowWindow`, `SetScrollPos`, `FlashWindow`) and the inverted `WAIT_OBJECT_0` (`WaitForInputIdle`) stay **unchecked with a written reason** - the resolution is the table plus the reason, not a blanket check.
- The table is one artifact per endpoint (one row per **stem**, not per spelling: the `A`/`W` pair has identical error semantics per D18, so one lookup covers the pair), and it is the reusable tooling the next pass reads - the plan's efficiency comes from the table, not from re-reading 597 MSDN pages twice.
- Checks are landed TU by TU inside the U1 batch regions, each batch ending at its own re-census (the U17 lesson: never defer ticks to the close), with the folder gate only at the end.
- Frozen history stays frozen (protocol 7): the U2 instance blocks in `registers/u2.md` keep their stale "0 `CheckReturn`" wording, corrected by a head line at the close.


## Site census tooling

The reusable instrument, already written and run this pass: `tmp/_tmp_u2_site_census.ps1` -> `tmp/_tmp_u2_site_census.csv` (597 rows + header) / `tmp/_tmp_u2_site_census.txt` (summary), plus `tmp/_tmp_u2_sal_map.ps1` -> `tmp/_tmp_u2_sal_map.txt` for the per-stem SAL prefilter. Columns: `tu | wrapper | retType | boundStem | binding | hasCheck | callShape | sdkRetType | salReturnQualifiers`. Measured: **SITES=597** (== the 597 anchor invariant), **DISTINCT_STEMS=597**, binding split 479 `useswinapi` + 118 `useswinapiAW`, **HAS_CHECKRETURN=5** matching the live source count exactly, **NO_PROTOTYPE=125**, **1** stem with an SAL return annotation.

Three instrument rules the next pass must keep (each was a live defect this pass): the definition-head pattern must accept the `T *User32::` and pointer-suffix spellings (the old one missed them); the prototype index must walk back to a blank line or `#` and not a fixed 9-line window (neighbour SALs leaked into the column, which is what made the "2 of 597" reading wrong); `hasCheck` must test `CheckReturn` minus a `^\s*//` line, not `^\s*[^/\s].*CheckReturn` (the character class ate the `C` of the token and reported **0** where the truth is **5**). The 5 live checks and their classes - the precedent the table's class column reuses: `MessageBeep` `errorhandling.cpp:85` `CheckReturn< BOOL, BoolCheck >`; `AttachThreadInput` `processthread.cpp:87` same; `GetGuiResources` `processthread.cpp:97` `CheckReturn< DWORD, CheckNonZero< DWORD > >`; `UserHandleGrantAccess` `processthread.cpp:115` `CheckReturn< BOOL, BoolCheck >`; `ShutdownBlockReasonDestroy` `systemshutdown.cpp:134` `CheckReturn< BOOL, BoolCheck >` (D51).

## Endpoint return semantics

The reusable per-endpoint lookup table, generated this pass and ready to fill: `tmp/_tmp_u2_endpoint_table.ps1` -> `tmp/_tmp_u2_endpoint_table.csv` (**597 rows + header**, 12 columns `stem | tu | binding | pubRetType | sdkRetType | salReturnAnnotation | msdnDocRef | inTreeCallers | verdict | checkClass | docQuote | notes`) and `tmp/_tmp_u2_endpoint_table.txt` (summary: **ROWS=597 STEMS=597 ZERO_CALLERS=396 WITH_CALLERS=201 CALLED_NAMES_SCANNED=266**). The generator joins the site census, the SAL map and one repo-wide `User32::` reference scan over the built trees (`src`, `ui`, `opengl`, this folder excluded), so the only column that needs human work is the verdict triple (`verdict | checkClass | docQuote`) - the other nine are mechanical.

Measured against the table: **396 of 597** bound stems have **0** in-tree references (the D14 published surface - change-safe, the D51 case), **201** have at least one, the heaviest being `GetDlgCtrlID` 8, `CopyIcon` 6, `SetWindowPos` 5, `GetDC` 4, `ReleaseDC` 3, `GetComboBoxInfo` 3, `PostQuitMessage` 3. Caller counts are an **upper bound**: the scan counts comments and other libraries' declarations, so a nonzero count must be re-read at the site before a check is landed there.

Doc reference pattern is verified, not guessed: `https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-<stem-lower>` resolves for the bare stems the AW bindings use - re-fetched this pass: `.../nf-winuser-getclassname` ("If the function fails, the return value is zero", success = characters copied) and `.../nf-winuser-getdlgctrlid` ("If the function fails, the return value is zero. An invalid value for the hwndCtl parameter ... will cause the function to fail"). So one URL per row is fetchable as-is; a 404 falls back to the `W` spelling.

Seed verdicts measured or fetched this pass (the table's first rows to trust): `ShutdownBlockReasonDestroy` checked `CheckReturn< BOOL, BoolCheck >` (fetched page, 0 callers, D51, landed at `systemshutdown.cpp:134`); `MessageBeep`/`AttachThreadInput`/`UserHandleGrantAccess` checked `CheckReturn< BOOL, BoolCheck >` and `GetGuiResources` checked `CheckReturn< DWORD, CheckNonZero< DWORD > >` (already in source - the class precedent for `DWORD` counts); `GetClassName` (AW, binds W) checked `CheckReturn< int, CheckNonZero< int > >` on the fetched zero-on-failure wording; `GetDlgCtrlID` a **candidate** `CheckReturn< int, CheckNonZero< int > >` with **8** in-tree references - the caller audit must run before it lands. `GetKeyboardState` is the only stem with an SAL return annotation (`_Check_return_`), which is a hint, not a verdict.

## Check class vocabulary

Reconcile `returncheck.h` against the measured return mix before any batch lands: name the class each verdict needs, write only the missing ones, and discharge the latent signature hazard seen while reading the header (`LongSuccessCheck : AutoErrorCheck< long >` declares `Test (intptr_t&)`, which on x64 where `intptr_t` is `long long` does **not** override `Test (long&)`, so the base's always-false `Test` would fire - relevant because it is the class `shell`/`kernel` use at their `HRESULT` sites and any api_user `LONG` verdict would otherwise inherit it).

## Small TU batch region checks

The 33 small-TU region (**124** sites) in the U1 tree's batch order S1-S5; each batch's sites are read from the table, only the `checked` verdicts are edited, and the batch ends at its per-TU re-census.

## Large TU batch region checks

The 19 large-TU region (**468** sites) in L1/L2/L3 order with `window.cpp` (86 sites, 53 `BOOL` + 34 `HWND` + the numeric mix) last, mirroring the U1 tree.

## Folder gate and census

One named run: the standing line at `status.md:158-165` over all 54 `.cpp` (response file from the disk listing), plus the standalone `/W4` legs, the `cl /EP` binding-literal leg, the 54/54 dating check, and the census invariants (heads 597, `Library::*Call<` 597, `Win32ErrorHandler _;` 597, `CheckReturn<` code sites = the table's checked count).

## Records and commit

D-entry per landed batch region plus the close, `registers/u2.md` head closure line, `status.md` boot line and Open list, `DECISIONS.md` index, `registers/README.md`, `completed_work.md`, `memory-bank/activeContext.md`, then `git commit` (protocol 1).

