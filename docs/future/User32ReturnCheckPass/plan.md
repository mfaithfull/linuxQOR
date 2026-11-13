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
    Status: Refined
    Content: ./EndpointReturnSemantics/plan.md
  - Title: Check class vocabulary
    Status: Refined
    Content: ./CheckClassVocabulary/plan.md
  - Title: Small TU batch region checks
    Status: Refined
    Content: ./SmallTUBatchRegionChecks/plan.md
  - Title: Large TU batch region checks
    Status: Refined
    Content: ./LargeTUBatchRegionChecks/plan.md
  - Title: Folder gate and census
    Status: Atomic
    Content:
  - Title: Records and commit
    Status: Atomic
    Content:
---

# Resolve U2 - return checks at the winqor_api_user wrapper call sites

Slug: `User32ReturnCheckPass` - from the request title "Resolve U2 - return checks at the winqor_api_user wrapper call sites" (drop-words `the`/`a`/`an`/`of` removed from the request phrase, PascalCased, ASCII, 21 chars, inside the 24-char cap; no collision with the sibling `User32ErrorHandlerPass`). Uniqueness is stored here, not recomputed.

## Goal

Make the U2 return-check policy resolve at **all 597 wrapper call sites** of `winqor_api_user` - each site ending in one of exactly three states (checked with a named `CheckReturn< ret, Check >`, deliberately unchecked with a written reason tied to its SDK failure value, or `void` with nothing to check) - with the decision for every site carried in one reusable per-endpoint table, so the code set lands in TU batches behind the U1 tree's proven boundaries.

## Resume (state slot - the only mutable section of this plan)

Cache, not record: the front matter owns structure, the disk owns truth, this slot only makes the resume cheap. Read the front matter and this slot, re-derive from disk, rewrite this slot if they disagree. Rotate in place - never append below this block.

- `Cursor: plan.md -> TERMINAL (all 7 items closed 12-13 Nov 2026; U2 CLOSED)` - item 6 **Folder gate and census** CLOSED 13 Nov 2026 (`tmp/_tmp_u2_gate_final.ps1` -> `.txt` `VERDICT fail=0`: 13-flag standing line matched at `status.md:158-165` over the disk-listed 54 `.cpp`, `w4_standalone` legs for `window.cpp` and `accelerator.cpp` exit 0 / 0 diags / obj newer, `ep_binding_literal` `EP LibraryCall=86 of_which_CheckReturn=50` = source `bindings=86 source CheckReturn=50`, `dating_54_of_54` `srcs=54 rsp=54 objs_missing=0 objs_stale=0 FOLDER_EXIT=0 log_diags=0`, census `heads=fcontext=useswinapi=LibraryCall=Win32ErrorHandler=597` with per-TU fail-lines=0 and `code CheckReturn sites=352 = table checked rows=352 over tus_with_checks=48`); item 7 **Records and commit** CLOSED 13 Nov 2026 at the six writes (`docs/agent/decisions/D52..D55.md` plus the `DECISIONS.md` index, `registers/u2.md` head CLOSE LINE, `status.md` boot line, `completed_work.md`, `registers/README.md`, `memory-bank/activeContext.md` rotated with the prior block archived to `memory-bank/archive/completed-context.md`) and the one `git commit` (protocol 1; `git log -1` re-reads it). Everything upstream is closed: the verdict side is **CLOSED in full** (`tmp/_tmp_u2_folder_audit.ps1` `rows=597 filled=597 checked=352 unchecked=236 void=9 sum=597 class-shape violations=0 filled rows with no docQuote=0`, the `EndpointReturnSemantics` subtree terminal at 6 items with **Audit the verdicts** last), `CheckClassVocabulary` terminal (3 `Atomic` closed; only genuinely missing classes written; `LongSuccessCheck` hazard discharged/routed), the code-side small region `SmallTUBatchRegionChecks` terminal (5 items CLOSED 12 Nov 2026, 124 sites over 33 TUs), and the code-side large region `LargeTUBatchRegionChecks` terminal (all 5 items CLOSED 12-13 Nov 2026, including item 5 **Accelerator TU sites** - the 5-site gap that had no owner is now owned there and closed with 3 tokens landed).
- `Measured at this rewrite (13 Nov 2026, re-derived from disk): tmp/_tmp_u2_folder_audit.ps1 rows=597 filled=597 checked=352 unchecked=236 void=9; tmp/_tmp_u2_ledger_check.ps1 rows total=597, small owned tus=33 rows=124 checked=79, large owned tus=20 rows=473 checked=273, orphan TUs=0, folder cpp=54, VERDICT fail=0; tmp/_tmp_u2_live_checks.ps1 tus_with_checks=48 code_sites=352 tus_with_returncheck_include=48 (79 small + 273 large = 352 = the table's 352 checked rows, so the landing is exactly the table); tmp/_tmp_u2_code_census.ps1 tus=54 fail=0` - every TU row reads `ok` and each TU that gained a check carries exactly one `../returncheck.h` include.
- `Next action: none - the root tree is terminal and U2 is CLOSED`; the blocking precondition at `## Records and commit` was discharged before any record write: the dated folder gate read `VERDICT fail=0` at 54/54 with `FOLDER_EXIT=0 log_diags=0`, and the census invariants re-read equal (heads/fcontext/useswinapi/LibraryCall/Win32ErrorHandler all 597, `CheckReturn<` code sites 352 = the table's 352 `checked` rows over 48 TUs with 48 `../returncheck.h` includes).
- `Slice arithmetic (tmp/_tmp_u2_slice_stems.ps1 -> .txt): 597 rows = 92 fetch slices over 92 Atomic fill items, each slice at most 10 URLs - small region 33 items/33 slices, L1 13, L2 19, L3 16, window 10, accelerator 1; claimed 597 of 597, no row owned twice`
- `Cursor check: tmp/_tmp_u2_tree_verify.ps1` - discovers every plan on disk (no hardcoded list), walks the chain both ways, checks Level = parent + 1, status/content consistency per item, and that every item has a body section.
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

Child plan: `./EndpointReturnSemantics/plan.md` (Level 1, **6** items: `Verify the doc refs` as the `Atomic` input gate + 3 `Refined` fill children + `Fill the accelerator rows` and `Audit the verdicts` as `Atomic`). Each fill child carries a subtree to depth 4 - region -> batch -> TU/family -> slice of at most 10 rows -> one `Atomic` item per slice, **92** slices over the 597 rows. The refinement rule that made this necessary: an `Atomic` item is one bounded action an agent can finish without re-planning, so an item whose body names N doc lookups is only atomic at N <= one fetch batch; a "fill this region" item (124 lookups) is a `Refined` item, never a step. The same lesson's second half is the input rule: the probe item exists because the old 124-lookup step also carried URLs nothing had validated, so part of its budget was spent on 404s.

Doc reference pattern is verified, not guessed: `https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-<stem-lower>` resolves for the bare stems the AW bindings use - re-fetched this pass: `.../nf-winuser-getclassname` ("If the function fails, the return value is zero", success = characters copied) and `.../nf-winuser-getdlgctrlid` ("If the function fails, the return value is zero. An invalid value for the hwndCtl parameter ... will cause the function to fail"). So one URL per row is fetchable as-is; a 404 falls back to the `W` spelling.

Seed verdicts measured or fetched this pass (the table's first rows to trust): `ShutdownBlockReasonDestroy` checked `CheckReturn< BOOL, BoolCheck >` (fetched page, 0 callers, D51, landed at `systemshutdown.cpp:134`); `MessageBeep`/`AttachThreadInput`/`UserHandleGrantAccess` checked `CheckReturn< BOOL, BoolCheck >` and `GetGuiResources` checked `CheckReturn< DWORD, CheckNonZero< DWORD > >` (already in source - the class precedent for `DWORD` counts); `GetClassName` (AW, binds W) checked `CheckReturn< int, CheckNonZero< int > >` on the fetched zero-on-failure wording; `GetDlgCtrlID` a **candidate** `CheckReturn< int, CheckNonZero< int > >` with **8** in-tree references - the caller audit must run before it lands. `GetKeyboardState` is the only stem with an SAL return annotation (`_Check_return_`), which is a hint, not a verdict.

## Check class vocabulary

Reconcile `returncheck.h` against the measured return mix before any batch lands: name the class each verdict needs, write only the missing ones, and discharge the latent signature hazard seen while reading the header (`LongSuccessCheck : AutoErrorCheck< long >` declares `Test (intptr_t&)`, which on x64 where `intptr_t` is `long long` does **not** override `Test (long&)`, so the base's always-false `Test` would fire - relevant because it is the class `shell`/`kernel` use at their `HRESULT` sites and any api_user `LONG` verdict would otherwise inherit it).

Child plan: `./CheckClassVocabulary/plan.md` (Level 1, 3 `Atomic` items, terminal). It carries the class inventory, the residual-set rule, and the `LongSuccessCheck` routing decision with its measured probe (`tmp/_tmp_u2_ovr_probe_neg*.exe` -> `BASE(Test 1) -> false`).

## Small TU batch region checks

The 33 small-TU region (**124** sites) in the U1 tree's batch order S1-S5; each batch's sites are read from the table, only the `checked` verdicts are edited, and the batch ends at its per-TU re-census.

Child plan: `./SmallTUBatchRegionChecks/plan.md` (Level 1, 5 `Refined` items S1-S5 -> 33 per-TU `Atomic` leaves at Level 2, each leaf one edit pass of at most 8 sites) - it carries the per-TU site counts from the disk ledger, the 5 already-decided sites these batches must not re-edit, and the per-TU re-census invariant.

## Large TU batch region checks

The 19 large-TU region (**468** sites) in L1/L2/L3 order with `window.cpp` (86 sites, 55 `BOOL` + 17 `HWND` + 14 numeric/`void`) last, mirroring the U1 tree.

Child plan: `./LargeTUBatchRegionChecks/plan.md` (Level 1, 4 `Refined` items L1/L2/L3/`window.cpp` -> 58 per-slice `Atomic` leaves at Level 3, each at most 10 sites) - it carries the L1/L2/L3/window ledger counts (90/153/139/86), the answer-shaped `BOOL` families expected to land `unchecked`, and the caller-audit precondition for the nonzero-caller rows.

## Folder gate and census

Six legs in one named run: `gate_banner`, `w4_standalone`, `ep_binding_literal`, `dating_54_of_54`, `heads_count`, `checkreturn_count` - the standing line at `status.md:158-165` over the 54 `.cpp` of this folder (response file from the disk listing), the two standalone `/W4` legs, the `cl /EP` binding-literal leg, the 54/54 dating check, and the census invariants (heads 597, `Library::*Call<` 597, `Win32ErrorHandler _;` 597, `CheckReturn<` code sites = the table's `checked` count). Stop condition: the response file re-reads with each leg at its expected count, and the census invariants re-read equal to one another.

## Records and commit

Six writes then one commit: `decisions_d_entry`, `registers_head_line`, `status_boot_line`, `completed_work_entry`, `active_context_entry`, `git_commit` - a D-entry per landed batch region plus the close, the `registers/u2.md` head closure line, the `status.md` boot line and Open list, the `DECISIONS.md` index, `registers/README.md`, `completed_work.md`, `memory-bank/activeContext.md`, then `git commit` (protocol 1). Stop condition: `git log -1` re-reads the recorded hash, and the head closure line re-reads from the disk rather than from memory.

Blocking precondition: no record write and no commit until the dated folder gate reads 54/54 exit 0 with 0 errors / 0 warnings.

## Handoff

The root tree is **terminal** (4 `Refined` with child plans on disk, 3 `Atomic`), so this plan's product - structure and decisions - is complete and the work itself belongs to another agent. Measured at this write: `tmp/_tmp_u2_tree_verify.ps1` -> `tmp/_tmp_u2_tree_verify.txt` and `tmp/_tmp_u2_gate_verify.ps1` -> `tmp/_tmp_u2_gate_verify.txt`, **planFiles 65**, **chain children 64**, levels 0..4, `fail=0` on both instruments, ledger 597 over 53 TUs with 0 delta, **0 of 597 verdicts filled**, 5 live `CheckReturn` sites (the seeds decided outside the table).

Take `/implement` with the deepest complete plan for the item the user picks; `/to-tickets` instead if the batches are to be spread across sessions. In item order, the deepest complete plans are:

| Pick | Deepest complete plan | What it lands |
|---|---|---|
| the table's verdicts (**the frontier - everything else waits on it**) | `./EndpointReturnSemantics/plan.md` (Level 1, 6 items: 3 `Refined` + 3 `Atomic`) | `verdict / checkClass / docQuote` for 597 rows, slice by slice (92 slices of at most 10 URLs) behind one probe item that validates the URLs first, then the verdict audit |
| the vocabulary | `./CheckClassVocabulary/plan.md` (Level 1, 3 `Atomic`) | the class per return type, only genuinely missing classes in `returncheck.h` (which puts the 122-source all-libraries gate in play), and the `LongSuccessCheck` routing decision |
| the small-TU code | `./SmallTUBatchRegionChecks/plan.md` (Level 1, 5 `Refined` -> 33 `Atomic` leaves) | `CheckReturn< >` at the `checked` rows of 124 sites over 33 TUs, one TU per leaf with a per-TU re-census |
| the large-TU code | `./LargeTUBatchRegionChecks/plan.md` (Level 1, 4 `Refined` -> 58 `Atomic` leaves) | the same over 473 sites, densest-last, one slice of at most 10 sites per leaf, caller audit before each nonzero-caller row lands |
| the gate and census | root item `Folder gate and census` (`Atomic`) | the standing 54/54 folder gate, the `/EP` and dating legs, the census invariants |
| the records | root item `Records and commit` (`Atomic`) | D-entry per landed batch region, the `registers/u2.md` head closure line, then `git commit` |

Order the executor must respect: the table's verdicts first (their audit item last inside that plan), the vocabulary before any batch that needs a class it writes, then the batch plans small-region-before-large-region, and no record write and no commit until the dated folder gate reads 54/54 exit 0 with 0 errors / 0 warnings. **U2 stays open until that gate and the census read 597 sites resolved into the three end states.**

