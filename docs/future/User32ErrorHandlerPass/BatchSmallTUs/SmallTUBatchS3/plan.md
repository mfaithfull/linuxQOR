---
Type: Plan
Title: "Small TU batch S3"
Parent: "../plan.md"
Level: 2
Items:
  - Title: Insert handlers in listbox.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in accessibility.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in errorhandling.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in hook.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in legacy.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in mdi.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in powermanagement.cpp
    Status: Atomic
    Content:
---

# Small TU batch S3

Slug: `SmallTUBatchS3` - from title "Small TU batch S3" (no drop-words, PascalCased, 14 chars, ASCII, inside the 24-char cap). Uniqueness is stored here, not recomputed.

## Goal

Give the third seven smallest wrapper TUs of `src/platform/os/windows/api_layer/user` their call-site handlers: **27 sites over 7 files**, plus one include line per file - so when this batch closes the small region's census reads **55 sites over 21 TUs** (S1 10/7 + S2 18/7 + S3 27/7).

## Context

Inherited from `../../BatchSmallTUs/plan.md` (which inherits the root `../plan.md`): the U1 pass installs `Win32ErrorHandler _;` at all **597 sites over the 53 wrapper TUs** (650 edits incl. 53 include lines). This batch is 7 TUs / **27 sites**; the counts below were re-measured, not inherited.

Baseline measured on disk at this resume (not inherited): folder = **54 `.cpp` sources**; anchored `^\s*qor_pp_fcontext;` = **597** against raw substring **598** (the anchor trap confirmed); code hits of `Win32ErrorHandler` = **0** - all 41 raw hits are banner prose; TUs including `errorhandler.h` = **0**. S1 and S2 are `Refined` (planned), so this batch still starts from a clean baseline.

**Census-pattern warning (measured here, applies to the whole pass).** The `Library::*Call<` census column is **not** an anchored count: folder-wide `Library::` reads **636** as a raw substring, **593** as `^\s*(return\s+)?Library::`, and **589** as `^\s*return Library::`. The gap to 597 is the **4 existing `CheckReturn` sites**, where the call is not at line start: `errorhandling.cpp:81`, `processthread.cpp:85`, `:94`, `:110`. Count that column **comment-stripped and by substring**, never anchored - otherwise the re-census invariant reads 593 and looks like a missed insert. `Library::voidCall` counts as a call site and is present in this batch (`accessibility.cpp:46`, `errorhandling.cpp:89`).

Per-file procedure, identical for every item here:
1. Snapshot the file; do not write through any pipeline that can truncate - `if($null -eq $t) throw` and `if($out.Length -lt 1) throw` immediately before each write (the `record-editing.md` truncation trap).
2. Insert `Win32ErrorHandler _;` at every line whose **first non-whitespace token** on a **comment-stripped** line is `qor_pp_fcontext;` - never a substring match. Indentation **8 spaces**, statement **after** the anchor and **before** the `qor_pp_useswinapi*` line.
3. Add `#include "../kernel/errorhandler.h"` in the **generic** block right after `#include "src/qor/flyers/error/error.h"` and **before** the `//Windows specific headers must be last ...` marker - **only if absent**, so a re-run is a no-op.
4. Re-measure that TU the moment it is written: expect per-TU `qor_pp_fcontext; == qor_pp_useswinapi* == Library::*Call< == Win32ErrorHandler` at the TU's site count (the last column measured on comment-stripped substring lines), and tick `registers/u1.md`'s working record for that TU immediately (U1 stays OPEN).
5. Keep the file **pure ASCII**; never touch the TU banner (protocol 7).

Measured anchors for the seven TUs of this batch - line numbers are provenance only; the inserter anchors on content, never on line numbers:

| TU | sites | `qor_pp_fcontext;` code at | prose `qor_pp_fcontext` hits | banner `Win32ErrorHandler` at | Windows marker |
|---|--:|---|---|---|--:|
| listbox.cpp | 3 | 81, 91, 100 | none | 54 | 70 |
| accessibility.cpp | 4 | 23, 30, 37, 44 | none | none | 9 |
| errorhandling.cpp | 4 | 65, 72, 79, 87 | none | none | 9 |
| hook.cpp | 4 | 81, 88, 95, 102 | none | none | 9 |
| legacy.cpp | 4 | 126, 136, 146, 156 | 65 | 73 | 9 |
| mdi.cpp | 4 | 85, 92, 99, 106 | none | 64 | 9 |
| powermanagement.cpp | 4 | 120, 128, 140, 150 | 64, 83 | 39, 65 | 111 |

The pre-batch invariant already holds per TU (`fcontext == useswinapi == Library::` at the counts in the table), so `Win32ErrorHandler` is the only column that moves.

Two of the parent's cautions resolve against measurement, one reverses:

- `errorhandling.cpp` is **not** the TU that names the handler class in prose - `findstr /I errorhandler` over that TU returns **no matches at all**. What it is instead: the TU holding **one of the folder's 4 live `CheckReturn` sites** (`:81` `CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, UINT >(...)`), whose semantics must not change, and a bare-statement `Library::voidCall< DWORD, DWORD >(...)` at `:89` that no anchored census pattern counts.
- `legacy.cpp:65` banner prose states "the body carries no `qor_pp_fcontext` because `user32.h` does not include the functioncontext flyer" while the TU has **4 code anchors** at 126/136/146/156. A substring inserter would treat that prose as a site; an absence-reading inserter would skip the TU. Both are wrong - the comment-stripped first-token rule decides.
- `powermanagement.cpp` carries prose at `:64` and `:83` quoting "2 `qor_pp_useswinapi` / 2 `qor_pp_fcontext` / 2 `Library::Call`" for a 4-site TU (the pair rows are per-member-set, not per-TU); those lines are skip-only, and its Windows marker sits at `:111`, not `:9`.

## Decisions

Inherited from `../../BatchSmallTUs/plan.md` and the root `../plan.md`: **D50** is the closure record for reversing the settled 29 Oct 2026 "leave as-is" for the **handler half only**; **U2 stays settled** - no `CheckReturn`/`CheckNonZeroHandle` is introduced anywhere, and `errorhandling.cpp:81` keeps its existing `CheckReturn` semantics untouched (the pass only adds the handler beside it, so that wrapper goes from "escalate" to "emit + escalate" - the root's measured delta row); **`user32.h` and `wsprintfT` untouched**, so the 122-source all-libraries gate is not in play and the folder gate is the audit; **U1 stays OPEN** until all 53 TUs / 597 wrappers carry the handler and the dated folder gate reads 54/54 exit 0; **banners never edited** (protocol 7 - this batch has 5 such prose hits, listed above); **no gate runs per TU** in this batch - the only gate legs are the probe TU's standalone `/W4` run (`accelerator.cpp`, a sibling of the parent plan) and the root's folder gate.

## Insert handlers in listbox.cpp

Atomic: one file, one scripted edit. `listbox.cpp` - **3 sites** (anchors 81, 91, 100); after the edit its census reads `3 / 3 / 3 / 3` and it carries the include. Banner hit at `:54` ("none of the three installs a Win32ErrorHandler") is prose; its Windows marker sits at `:70`.

## Insert handlers in accessibility.cpp

Atomic: one file, one scripted edit. `accessibility.cpp` - **4 sites** (anchors 23, 30, 37, 44); census `4 / 4 / 4 / 4` plus the include. The fourth wrapper is `Library::voidCall` at `:46` - a void-returning wrapper still gets the handler, which is what the sibling layers do.

## Insert handlers in errorhandling.cpp

Atomic: one file, one scripted edit - and the one TU of this batch with a live interaction to watch. `errorhandling.cpp` - **4 sites** (anchors 65, 72, 79, 87); census `4 / 4 / 4 / 4` plus the include. The handler inserted at the `:79` anchor sits in the same scope as the pre-existing `CheckReturn` at `:81` (one of the folder's 4) - do not reformat, retype or reorder that line; the `:87` anchor pairs with the bare `Library::voidCall` at `:89`. Its banner never mentions the handler class, so no prose anchor risk here.

## Insert handlers in hook.cpp

Atomic: one file, one scripted edit. `hook.cpp` - **4 sites** (anchors 81, 88, 95, 102); census `4 / 4 / 4 / 4` plus the include. No prose anchor hits.

## Insert handlers in legacy.cpp

Atomic: one file, one scripted edit. `legacy.cpp` - **4 sites** (anchors 126, 136, 146, 156); census `4 / 4 / 4 / 4` plus the include. Prose `qor_pp_fcontext` at `:65` claims the TU has none - skip it, and do not let an absence-reading inserter skip the TU. Banner `Win32ErrorHandler` at `:73`.

## Insert handlers in mdi.cpp

Atomic: one file, one scripted edit. `mdi.cpp` - **4 sites** (anchors 85, 92, 99, 106); census `4 / 4 / 4 / 4` plus the include. Banner `Win32ErrorHandler` at `:64`.

## Insert handlers in powermanagement.cpp

Atomic: one file, one scripted edit. `powermanagement.cpp` - **4 sites** (anchors 120, 128, 140, 150); census `4 / 4 / 4 / 4` plus the include. Highest prose density of the batch: `qor_pp_fcontext` prose at `:64` and `:83`, `Win32ErrorHandler` prose at `:39` and `:65`, and its Windows marker sits at `:111`. Last of the batch: after it, re-census the region - the small region must read **55 sites over 21 TUs** (S1 + S2 + S3), which is the gate for S4 starting.

