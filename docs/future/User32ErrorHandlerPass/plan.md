---
Type: Plan
Title: "User32 error handler pass"
Parent:
Level: 0
Items:
  - Title: Probe the accelerator TU
    Status: Atomic
    Content:
  - Title: Batch the small TUs
    Status: Refined
    Content: ./BatchSmallTUs/plan.md
  - Title: Batch the large TUs
    Status: Refined
    Content: ./BatchLargeTUs/plan.md
  - Title: Gate the folder
    Status: Atomic
    Content:
  - Title: Write the records
    Status: Refined
    Content: ./WriteRecords/plan.md
  - Title: Commit the pass
    Status: Atomic
    Content:
  - Title: All-libraries gate
    Status: Deferred
    Content:
---

# User32 error handler pass

Slug: `User32ErrorHandlerPass` - from title "User32 error handler pass" (no drop-words, PascalCased, ASCII, 22 chars, inside the 24-char cap). Uniqueness is stored here, not recomputed.

## Goal

Give every wrapper in `winqor_api_user` a call-site `Win32ErrorHandler _;` - **597 sites over the 53 wrapper TUs** of `src/platform/os/windows/api_layer/user` - so a Windows failure that today fails silently prints its decoded error text before it escalates, and close `U1` as decision record **D50** while leaving `U2` untouched.

## Resume (state slot - the only mutable section of this plan)

Cache, not record: the front matter owns structure, the disk owns truth, and this slot only makes the resume cheap. A resuming session reads the front matter and this slot and **nothing else**, then re-derives from disk and rewrites this slot if the two disagree. Rotate in place - never append below this block, never grow it, never restate a D-entry or a measured narrative here.

- `Cursor: plan.md -> Commit the pass`
  Root pre-order, so one chain stands in for a flag per item: everything left of the cursor is done, everything right of it is not. The pass runs in that order (foot of this plan), so the cursor moves only at a batch boundary, never mid-batch.
- `Measured at this write: handlers 597 / 597; TUs carrying the include 53 / 53; batches verified: accelerator probe, SmallTUBatchS1..S5, LargeTUBatchL1..L3, window.cpp`
  Rewrite this line the moment a batch re-census verifies. The disk, not this line, is the truth; the harness below compares the two on every run.
- `Next action: the commit`
  One commit of the 53 sources + the records, then stop - no read-back pass; `git show` is the audit. The folder gate and the census invariant both read complete, so the only root item left of `All-libraries gate` (Deferred) is this one.
- `Cursor check: tmp/_tmp_u1_tree_verify2.ps1` - walks the chain against the front matter, re-runs the census, prints the per-batch ledger, and reports any gap left of the cursor or any handler right of it.
- `Blocking precondition unchanged: no record write and no commit until the dated folder gate reads 54/54 exit 0 with 0 errors / 0 warnings` - the precondition line at the foot of `## Write the records` is the authority, not this slot.

Rewrite the `Measured at this write` line the moment a batch re-census verifies, and tick the matching row of the `registers/u1.md` working record in the same session - the U17 lesson carried at `BatchSmallTUs/plan.md:41`: deferring ticks to the close forced a whole pass to be re-derived. The cursor moves only at a batch boundary, never mid-batch.


## Context

Provenance: `docs/future/winqor_api_user_U1_plan.md` (written 10 Nov 2026, plan pass, no source touched) - superseded by this tree and kept as source narrative only. Every count below was **measured that pass, not inherited**.

**The policy conflict (read before executing).** `registers/u1.md:3-4` states "**U1 - no call-site error handler anywhere in this library. Policy settled 29 Oct 2026 (user): accepted as-is, not to be fixed TU by TU**", pointing at `archive/status-v1-2026-11-04.md:94-97` ("Return-check / error-handler policy (settled 29 Oct 2026, user decision, session 3) - leave as-is: no `Win32ErrorHandler _;`, no `CheckReturn`/`CheckNonZeroHandle` here, though `api_layer/shell` and `api_layer/kernel` use both and G6 makes `CheckReturn` always active"). This plan **reverses that settled decision for the handler half only**, so it must land with **D50** plus a closure line in `registers/u1.md` - otherwise the record contradicts the tree. It is defensible as a reporting fix because the measured delta is almost behaviour-neutral (below).

Corroborating state measured that pass:

| Fact | Measured |
|---|---|
| `status.md:149` "Open:" list | `U1, U2` - the only two items left open, both deliberately not fixed |
| Code occurrences of `Win32ErrorHandler` in the folder | **0** (41 raw hits, every one prose inside a TU banner) |
| TUs including `../kernel/errorhandler.h` | **0** |
| TU banners asserting "no `Win32ErrorHandler` ... U1/U2 policy" | ~35 banners (`bitmap.cpp:74`, `menu.cpp:80`, `touch.cpp:76`, `window.cpp:71`, ...) |

**Population (re-derived that pass).** `u1.md`'s "0 of 38 TUs" is stale - 38 was the review-queue size. The folder is now **54 sources**: 53 carry wrappers, `winqor_api_user.cpp` is the module marker (0 wrappers). Comment-stripped census (code lines only): **597 wrappers = 597 `qor_pp_fcontext;` code sites = 597 `qor_pp_useswinapi*` binding lines = 597 `Library::Call`/`voidCall`/`varCall` call sites.** Total edit count for the pass: **597 statement insertions + 53 include lines = 650 changes over 53 files.**

Cross-check against the folder's standing census: `user32.h` 817 lines / 598 member declarations / **597 bindings** (post-P3, 12 Nov 2026) - the binding count agrees exactly, so the census anchor is the whole population and no wrapper is missing a `qor_pp_fcontext;`.

Per-file site list - the exact count the inserter must hit per TU (alphabetical; sums to **597** over the 53 wrapper TUs):

| TU | sites | TU | sites | TU | sites | TU | sites |
|---|--:|---|--:|---|--:|---|--:|
| accelerator.cpp | 5 | accessibility.cpp | 4 | bitmap.cpp | 1 | brush.cpp | 1 |
| buttoncontrol.cpp | 3 | caret.cpp | 8 | clipboard.cpp | 20 | combobox.cpp | 3 |
| console.cpp | 1 | coordtransform.cpp | 3 | cursor.cpp | 15 | desktop.cpp | 18 |
| devicecontext.cpp | 8 | devicemanagement.cpp | 2 | dialog.cpp | 22 | displayconfig.cpp | 7 |
| dpi.cpp | 21 | errorhandling.cpp | 4 | filledshapes.cpp | 3 | help.cpp | 5 |
| hook.cpp | 4 | icon.cpp | 14 | ime.cpp | 5 | keyboard.cpp | 34 |
| legacy.cpp | 4 | listbox.cpp | 3 | mdi.cpp | 4 | menu.cpp | 39 |
| message.cpp | 24 | misc.cpp | 23 | monitor.cpp | 5 | mouse.cpp | 10 |
| painting.cpp | 25 | pointer.cpp | 32 | powermanagement.cpp | 4 | printing.cpp | 1 |
| processthread.cpp | 5 | rawinput.cpp | 7 | rectangle.cpp | 11 | resource.cpp | 2 |
| scrollbar.cpp | 12 | security.cpp | 2 | strings.cpp | 34 | synchronization.cpp | 2 |
| systeminfo.cpp | 4 | systemshutdown.cpp | 6 | text.cpp | 4 | timer.cpp | 2 |
| touch.cpp | 14 | window.cpp | 86 | windowclass.cpp | 14 | windowprocedure.cpp | 2 |
| windowproperties.cpp | 5 | winqor_api_user.cpp | 0 | | | | | |

**The anchor trap.** Raw grep for `qor_pp_fcontext;` over the folder reads **598** - one above the 597 code sites, because exactly one TU banner spells the anchor with its semicolon in prose. The inserter must anchor on lines whose **first non-whitespace token** is `qor_pp_fcontext;` on a **comment-stripped** line, never on a substring match; that prose hit must be instrumented before the inserter is trusted. Banner text is never edited (protocol 7).

**The idiom, copied from `kernel`/`shell` (not invented).**
- Include: a `user/` TU must spell **`#include "../kernel/errorhandler.h"`** - the form `shell/drag.cpp:11` uses (`kernel/kconsole.cpp:8` spells the bare `"errorhandler.h"` because that TU lives in `kernel`). Place it in the **generic** block, immediately after `#include "src/qor/flyers/error/error.h"` and before the `//Windows specific headers must be last ...` comment; `errorhandler.h` is a generic QOR header (it pulls `src/qor/flyers/error/defaulterrorhandler.h` at its line 28), so it must not go below the Windows-specific marker. Add it **only if absent** so a re-run is a no-op.
- Statement: `Win32ErrorHandler _;` **immediately after `qor_pp_fcontext;`, before the `qor_pp_useswinapi*` line**, at 8-space indentation. Target shape (`accelerator.cpp:22-27`):

    BOOL User32::CopyAcceleratorTableT(HACCEL hAccelSrc, LPACCEL lpAccelDst, int cAccelEntries)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, CopyAcceleratorTable);
        return Library::Call<int, HACCEL, LPACCEL, int>(pFunc, hAccelSrc, lpAccelDst, cAccelEntries);
    }

**Measured run-time delta (what the record must state).** Chain, read that pass: `IssueHandler<T>`'s ctor `Push()`es the handler onto the flyer chain and its dtor `Pop()`s (`src/qor/flyers/error/handler.h:37-45`), so the handler covers the wrapper's dynamic extent; `kernel/errorhandler.cpp:64`+ emits `OutputDebugStringA` with the issue text plus the decoded Windows last-error string and returns **false** for `Continuable_Error`/`Serious_Error`/`Fatal_Error` (**true** only for `Note`/`Warning`); `Issue::Resolve(bool)` (`src/qor/flyers/issue/issue.h:126-132`) turns `Resolve(false)` into `Escalate()` -> `throw(*this)`.

| Path | Today (no handler) | With `Win32ErrorHandler _;` |
|---|---|---|
| null binding / SEH fault (`serious`) | `Serious::Handle()` else-branch -> `Escalate()` -> `throw` | handler runs, emits the Windows error string, returns false -> `Resolve(false)` -> `Escalate()` -> **the same `throw`** |
| the 4 existing `CheckReturn` sites (`continuable`) | escalate | emit + escalate |
| `Note`/`Warning` raises | none exist on this library's call path | would be resolved (not escalated) - **measure, do not assert** |

**Delta: control flow and the exception are unchanged; the gain is the diagnostic.** Link is already covered - `user/CMakeLists.txt` ends `target_link_libraries(${target} qor_module qor_error qor_issue winqor_api_kernel winqor_sharedbootstrap)`, so the `Win32ErrorHandler` vtable and `Handle` definition resolve from `winqor_api_kernel` (a read-verification, not gate proof; the gate is compile-only). `user32.h:11` already pulls `../library.h`, so the include graph gains only `errorhandler.h`.

**Known unknowns - settle during the probe, do not guess.**
- `/W4` on an unused named object: `kernel`/`shell` already carry ~1,500 `Win32ErrorHandler _;` sites at `/W4` exit 0, so `C4101` (unreferenced local) is **not** emitted by MSVC - but the `accelerator.cpp` probe proves it for this folder rather than inheriting the claim.
- `qor_pp_fcontext` interaction: the context object's dynamic extent must **outlast** the handler for the issue's `Where`/`When` payload to stay valid; `kernel`/`shell` place the handler after `qor_pp_fcontext;`, which is the ordering this plan copies - if the probe prints an ordering diagnostic, that is where the fix belongs.
- Destruction order at scope exit: handler dtor `Pop()`s before the context object dtor runs - same as the sibling layers, but confirm at the probe that no issue raised during unwinding reads a popped context.
- The one prose `qor_pp_fcontext;` hit: its exact location must be instrumented before the inserter is trusted.


## Decisions

Locked for this plan; linked, not restated.

- **D50 - to be written at the close** (`docs/agent/DECISIONS.md` + `docs/agent/decisions/D50.md`): reverses the settled 29 Oct 2026 "leave as-is" for the **handler half only**. `docs/agent/DECISIONS.md` currently ends at **D49**, so D50 is the next free id - re-measure before writing.
- **U2 stays settled and untouched** - no `CheckReturn`/`CheckNonZeroHandle` is introduced anywhere, and the 4 existing `CheckReturn` sites keep their semantics.
- **`wsprintfT` is excluded** - the header-inline variadic member (`user32.h:799`+) stays as it is under the D20 variadic-wrapper-shape policy. Consequence: **`user32.h` is not edited**, so the 122-source all-libraries gate is **not** in play and the folder gate is the audit.
- **U1 stays OPEN until the whole pass is done** - do not touch `registers/u1.md`'s head, `status.md`'s `Open:` list, or `registers/README.md`'s closed list until all 53 TUs / 597 wrappers carry the handler and the final folder gate is dated clean. Tick the working record per TU as it is measured; close only at the end.
- **Never edit the ~35 TU banners** - protocol 7 freezes review banners; the stale "0 `Win32ErrorHandler`" wording is corrected in `u1.md`/`D50`, per the U17/D40 precedent (see `registers/u17.md`'s working-record shape, which this pass's record copies).
- Gate discipline from `status.md:158-165` (see the folder-gate item): `/Fo` attached, dating by `cmd /c dir /T W`, never pwsh. The `D21`/`D46`/`D48` residuals stay: no project-wide `UNICODE`/`WINVER`/`_WIN32_WINNT` pin, no `NO_STRICT` change.
- **Honesty flag:** the source narrative quotes the 122-source baseline as **166** `warning C|error C` lines while `status.md:171` says **165**. Do not inherit either - re-measure if that gate ever runs (it must not, this pass).
- **Out of scope (what this pass does NOT do):** no `CheckReturn`/`CheckNonZeroHandle` anywhere (U2 untouched, its policy stays settled); no `user32.h` edit, no `wsprintfT` handler, no shared-header edit; no TU banner edits and no append to frozen/closed records (`tooling.md` is frozen at session 34); no project-wide `UNICODE`/`WINVER`/`_WIN32_WINNT` pin and no `NO_STRICT` change (D21/D46 residuals stay); no change to the 4 existing `CheckReturn` sites' semantics and none to `errorhandler.h`/`errorhandler.cpp`.

## Probe the accelerator TU

Atomic: one file, one edit, one gate run. Probe **`accelerator.cpp`** (5 sites) plus its include line, then run the standalone `/W4` gate on it. This is the only step that can surface a new diagnostic (`C4101`/`C4189` on the unused `_`, or an include-order `C1083` class), and it is also where the four unknowns above get measured. Resolve anything it prints **before** mass-applying. Date the standalone leg by `tmp/obj/accelerator.obj` being newer than the source.

## Batch the small TUs

33 TUs / **124 sites** - the source's B1 (1-3 sites, 15 TUs, 31 sites) and B2 (4-8 sites, 19 TUs, 98 sites) minus `accelerator.cpp`'s 5. Both source batches exceed the 7-item breadth cap, so the region is re-cut into **five batches of <= 7 TUs** (S1-S5); the re-sum is recorded here rather than silently inherited. Smallest-first, so a cheap batch failure surfaces early.

Per-TU edit mechanics for every batch: scripted insertion; snapshot the file first, then `if($null -eq $t) throw` and `if($out.Length -lt 1) throw` immediately before any write (the `record-editing.md` truncation trap); re-measure after each file; keep sources **pure ASCII**; keep indentation exact; add the include only if absent so a re-run is a no-op.

Re-census after every batch: the invariant is per-TU `qor_pp_fcontext; == qor_pp_useswinapi* == Library::*Call< == Win32ErrorHandler`, and folder totals `597 / 597 / 597 / 597` with 53 TUs carrying the include. Any batch that re-sums differently means the inserter missed or double-hit a site - stop and re-measure, do not proceed.


## Batch the large TUs

19 TUs / **468 sites** - the source's B3 (10-15 sites, 7 TUs, 90 sites), B4 (18-25, 7 TUs, 153), B5 (32-39, 4 TUs, 139) and B6 (`window.cpp` alone, 86). Batch totals across both regions must re-sum to **53 TUs / 597 sites** (`accelerator.cpp`'s 5 counted in the probe item). Same mechanics and the same re-census invariant as the small region.

Settled into `./BatchLargeTUs/plan.md`: re-cut for the 7-item breadth cap into **L1 7 TUs / 90**, **L2 7 TUs / 153**, **L3 4 TUs / 139** and `window.cpp` **86** as an Atomic item (19 TUs / 468, re-summed against the disk this pass). Measured there: the region starts at **0 handlers / 0 includes**, its only semicolon-bearing prose anchor is **`painting.cpp:74`** (raw 26 against 25 code anchors), it holds **0 `CheckReturn` code sites** and **6 `Library::voidCall`** bare statements (`keyboard`/`message`/`misc`/`mouse`/`pointer`/`window`), and `window.cpp`'s frozen banner claims 85 wrappers against a **86**-anchor disk census.

## Gate the folder

Atomic: one gate run over the folder. Edited TU standalone at `/W4` exit 0, then the folder gate **54/54 exit 0, 0 errors, 0 warnings**. Recreate `tmp/_tmp_u1_gate54.bat` by slug from the standing command line (`.bat` files are scratch and uncommitted; `tmp/_tmp_u8_gate54.bat` is the shape to copy). `/Fo` must be **attached** to the switch - the spaced form writes `.obj` into the current directory and prints `D9024`/`D9027`. Date the run with `cmd /c dir /T W`, never pwsh (`Get-ChildItem` returns an empty `LastModifyDate` on this tree). Run `cl /EP` after the last edit and confirm the 597 binding literals are byte-unchanged - `/EP` output is one line per source line, so the literal survives while its line number shifts (banner weight of this change: +597 statement lines +53 include lines).

## Write the records

At the close only, never mid-pass. One record per file, each a concrete edit:

- **D50** - `docs/agent/DECISIONS.md` index line + `docs/agent/decisions/D50.md` entry: the reversal of the 29 Oct "leave as-is" for the handler half only, with the measured delta (control flow and the exception unchanged; the gain is the diagnostic).
- **Working record in `registers/u1.md`** in `u17.md`'s shape (checklist + tick state + measured post-pass figures), plus the **closure line** correcting the stale "0 `Win32ErrorHandler`" wording.
- **`status.md`** - boot line and its `Open:` list: `U1, U2` -> `U2` (U1 closed with D50; U2 stays open).
- **`registers/README.md`** closed list; **`completed_work.md`**; **`memory-bank/now.md`**.
- Append nothing to frozen or closed records (`tooling.md` is frozen at session 34); never edit the ~35 TU banners.

Settled into `./WriteRecords/plan.md` (Level 1): **7 Atomic items, one per record file**, in the order `D49.md:32`'s `## Record` paragraph leaves it - `docs/agent/decisions/D50.md`, `docs/agent/DECISIONS.md` index, `status.md`, `registers/README.md`, `registers/u1.md`, `completed_work.md`, `memory-bank/now.md`. Measured this pass against the disk: `DECISIONS.md` is 102 lines and its index ends at **D49** (`File: ./decisions/D49.md` at `:102`), `docs/agent/decisions/` holds **49** files `D1.md`-`D49.md` with **no D50**, so D50 is the next free id; `status.md` is 180 lines whose Open list at `:149` reads exactly `- Open: U1, U2.`; `registers/u1.md` is 10 lines whose `:5` says "0 of 38 TUs"; `registers/README.md:12` owns the closed list (its `Lines`/`Was registers.md` table is retired and not to be trusted); `completed_work.md:71`/`:73` is the closed-ids row; `now.md:3` is the focus entry. `registers/u17.md:3-21` is the working-record shape this pass copies. The child is **terminal on creation** (all 7 items Atomic), so this root item is Refined.

**Precondition measured this pass, stated so a resuming session cannot skip it:** the code pass has **not started** - `git status --porcelain` lists only `docs/future/TODO.md` and `docs/future/...` (0 sources touched), and the folder census reads **0** `Win32ErrorHandler _;` code sites (41 prose banner hits) and **0** `errorhandler.h` includes over the 54 `.cpp`. So none of these seven record writes is legal until the code pass and the dated folder gate are clean; the records carry figures measured then, never the ones here.

## Commit the pass

Atomic: one commit of the 53 sources + the records, then stop - no read-back pass; `git show` is the audit.

## All-libraries gate

Deferred, not run: decision 2 excludes `wsprintfT`, so `user32.h` and every shared header stay untouched and the folder gate is the whole audit. If a later session edits `user32.h`, the 122-source diff becomes mandatory - run **once**, diff not exit code, baseline 1 `C1083` at `opengl\wgl.cpp:31`, exit **2** (warning-line count: see the honesty flag above).

## Handoff

The root is **terminal** - every item is `Refined` (2 regions + this pass's records item), `Atomic` (`Probe the accelerator TU`, `Gate the folder`, `Commit the pass`) or `Deferred` (`All-libraries gate`) - so the tree is done and this skill's product (structure and decisions) is complete. **No further deep-planning node is needed for this request.**

The work itself belongs to another skill. Hand off to **`/implement`** for the pass, or `/to-tickets` first if the 650 edits are to be ticketed and claimed one at a time. The deepest complete plans, one per execution unit, all terminal:

| Execution unit (root pre-order) | Deepest complete plan | Items |
|---|---|---|
| `Probe the accelerator TU` | none - Atomic at the root | `accelerator.cpp` 5 sites + include + standalone `/W4` gate |
| `Batch the small TUs` | `BatchSmallTUs/plan.md` -> `SmallTUBatchS1`/`S2`/`S3`/`S4`/`S5` | 7 + 7 + 7 + 7 + 5 Atomic, one per TU |
| `Batch the large TUs` | `BatchLargeTUs/plan.md` -> `LargeTUBatchL1`/`L2`/`L3`, plus the root's Atomic `window.cpp` | 7 + 7 + 4 Atomic, then 86 sites in one scripted pass |
| `Gate the folder` | none - Atomic at the root | 54/54 exit 0, 0 errors, 0 warnings, dated by `cmd /c dir /T W` |
| `Write the records` | `WriteRecords/plan.md` | 7 Atomic, one per record file |
| `Commit the pass` | none - Atomic at the root | one commit of the 53 sources + the records, then stop |

Run in that order, re-checking the census invariant after every batch (per-TU `qor_pp_fcontext;` == `qor_pp_useswinapi*` == `Library::*Call<` == `Win32ErrorHandler`; folder totals `597 / 597 / 597 / 597` over 53 TUs carrying the include). A batch that re-sums differently means the inserter missed or double-hit a site - stop and re-measure, do not proceed. Nothing is committed and no record is written before the dated folder gate is clean.
