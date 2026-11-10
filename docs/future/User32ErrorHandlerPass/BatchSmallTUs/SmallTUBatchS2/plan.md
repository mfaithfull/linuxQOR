---
Type: Plan
Title: "Small TU batch S2"
Parent: "../plan.md"
Level: 2
Items:
  - Title: Insert handlers in synchronization.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in timer.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in windowprocedure.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in buttoncontrol.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in combobox.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in coordtransform.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in filledshapes.cpp
    Status: Atomic
    Content:
---

# Small TU batch S2

Slug: `SmallTUBatchS2` - from title "Small TU batch S2" (no drop-words, PascalCased, 14 chars, ASCII, inside the 24-char cap). Uniqueness is stored here, not recomputed.

## Goal

Give the next seven smallest wrapper TUs of `src/platform/os/windows/api_layer/user` their call-site handlers: **18 sites over 7 files**, plus one include line per file - so when this batch closes the small region's census reads **28 sites over 14 TUs** (S1's 10/7 plus this 18/7).

## Context

Inherited from `../../BatchSmallTUs/plan.md` (which inherits the root `../plan.md`): the U1 pass installs `Win32ErrorHandler _;` at all **597 sites over the 53 wrapper TUs** (650 edits incl. 53 include lines). This batch is 7 TUs / **18 sites** of the small region's 33 TUs / 124 sites; the site counts below were re-measured, not inherited.

Baseline measured on disk at this resume (not inherited): folder = **54 `.cpp` sources**; raw substring `qor_pp_fcontext;` = **598** against anchored `^\s*qor_pp_fcontext;` = **597** (the anchor trap confirmed); code occurrences of `Win32ErrorHandler` = **0** - all 41 raw hits are banner prose; TUs including `errorhandler.h` = **0**. So **no part of the pass has landed yet**: S1's `Refined` records planning, not execution, and this batch starts from a clean baseline.

Per-file procedure, identical for every item here:
1. Snapshot the file; do not write through any pipeline that can truncate - `if($null -eq $t) throw` and `if($out.Length -lt 1) throw` immediately before each write (the `record-editing.md` truncation trap).
2. Insert `Win32ErrorHandler _;` at every line whose **first non-whitespace token** on a **comment-stripped** line is `qor_pp_fcontext;` - never a substring match. Indentation **8 spaces**, statement **after** the anchor and **before** the `qor_pp_useswinapi*` line.
3. Add `#include "../kernel/errorhandler.h"` in the **generic** block right after `#include "src/qor/flyers/error/error.h"` and **before** the `//Windows specific headers must be last ...` marker - **only if absent**, so a re-run is a no-op.
4. Re-measure that TU the moment it is written: expect per-TU `qor_pp_fcontext; == qor_pp_useswinapi* == Library::*Call< == Win32ErrorHandler` at the TU's site count, and tick `registers/u1.md`'s working record for that TU immediately (U1 stays OPEN).
5. Keep the file **pure ASCII**; never touch the TU banner (protocol 7).

Measured anchors for the seven TUs of this batch - line numbers are provenance only; the inserter anchors on content, never on line numbers:

| TU | sites | `qor_pp_fcontext;` at | prose hits to skip | banner `Win32ErrorHandler` at |
|---|--:|---|---|---|
| synchronization.cpp | 2 | 116, 128 | - | 70 |
| timer.cpp | 2 | 20, 27 | - | - |
| windowprocedure.cpp | 2 | 77, 84 | 22, 65 | 70 |
| buttoncontrol.cpp | 3 | 89, 100, 111 | - | 62 |
| combobox.cpp | 3 | 39, 46, 53 | - | - |
| coordtransform.cpp | 3 | 37, 44, 51 | 18 | - |
| filledshapes.cpp | 3 | 79, 86, 93 | - | 38 |

The pre-batch invariant already holds per TU (`fcontext == useswinapi == Library::` at the counts in the table), so `Win32ErrorHandler` is the only column that moves - a TU whose post-batch count is short by one means the inserter missed an anchor, and a count above the site count means it hit prose.

Two TUs carry **prose `qor_pp_fcontext`** that a substring inserter would over-hit: `windowprocedure.cpp:22` and `:65`; `coordtransform.cpp:18` carries prose `qor_pp_useswinapi`. The comment-stripped first-token rule is mandatory, not optional.

Include-block anchors measured: `#include "src/qor/flyers/error/error.h"` sits at **line 7** in all seven; the Windows-specific marker sits at **line 9** in `timer`/`windowprocedure`/`combobox`/`coordtransform`/`filledshapes`, at **line 77** in `buttoncontrol.cpp`, at **line 100** in `synchronization.cpp`. Insert between the two - marker-relative, never a fixed line number.

Banners that quote counts and will read stale after the pass (they must stay stale; the correction belongs in `registers/u1.md` and **D50**): `filledshapes.cpp:38` states "3 call sites, 0 handlers, 0 checks", `windowprocedure.cpp:22` states "2/2 members carry qor_pp_fcontext (:78, :85)".

`coordtransform.cpp:38` spells the bare `qor_pp_useswinapi(user32, ClientToScreen)` under D13 - the handler insert is independent of that spelling.

## Decisions

Inherited from `../../BatchSmallTUs/plan.md` and the root `../plan.md`: **D50** is the closure record for reversing the settled 29 Oct 2026 "leave as-is" for the **handler half only**; **U2 stays settled** - no `CheckReturn`/`CheckNonZeroHandle` anywhere in this library; **`user32.h` and `wsprintfT` untouched**, so the 122-source all-libraries gate is not in play and the folder gate is the audit; **U1 stays OPEN** until all 53 TUs / 597 wrappers carry the handler and the dated folder gate reads 54/54 exit 0; **banners never edited** (protocol 7); **no gate runs per TU** in this batch - the only gate legs are the probe TU's standalone `/W4` run (`accelerator.cpp`, a sibling of the parent plan) and the root's folder gate.

## Insert handlers in synchronization.cpp

Atomic: one file, one scripted edit. `synchronization.cpp` - **2 sites**; after the edit its census reads `2 / 2 / 2 / 2` and it carries the include. Its banner hit at `:70` ("0 Win32ErrorHandler and 0 CheckReturn in this TU") is prose - read it, do not edit it. Note this TU's Windows-specific marker sits at `:100`, not `:9`.

## Insert handlers in timer.cpp

Atomic: one file, one scripted edit. `timer.cpp` - **2 sites**; census `2 / 2 / 2 / 2` plus the include. No prose anchor hits in this TU.

## Insert handlers in windowprocedure.cpp

Atomic: one file, one scripted edit. `windowprocedure.cpp` - **2 sites**; census `2 / 2 / 2 / 2` plus the include. Highest over-hit risk of the batch: prose `qor_pp_fcontext` at `:22` and `:65` must not be touched, and its banner quotes member/anchor counts that stay stale.

## Insert handlers in buttoncontrol.cpp

Atomic: one file, one scripted edit. `buttoncontrol.cpp` - **3 sites**; census `3 / 3 / 3 / 3` plus the include. Banner at `:62` asserts "no CheckReturn/Win32ErrorHandler is introduced" - prose only; marker sits at `:77`.

## Insert handlers in combobox.cpp

Atomic: one file, one scripted edit. `combobox.cpp` - **3 sites**; census `3 / 3 / 3 / 3` plus the include.

## Insert handlers in coordtransform.cpp

Atomic: one file, one scripted edit. `coordtransform.cpp` - **3 sites**; census `3 / 3 / 3 / 3` plus the include. Prose `qor_pp_useswinapi` at `:18` must not be touched; the bare `qor_pp_useswinapi` spelling (D13) does not change the handler insert.

## Insert handlers in filledshapes.cpp

Atomic: one file, one scripted edit. `filledshapes.cpp` - **3 sites**; census `3 / 3 / 3 / 3` plus the include. Last of the batch: after it, close the batch by re-censusing the region - the small region must read **28 sites over 14 TUs** (S1 + S2), which is the gate for S3 starting.

