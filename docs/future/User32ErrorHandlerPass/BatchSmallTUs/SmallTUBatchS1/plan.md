---
Type: Plan
Title: "Small TU batch S1"
Parent: "../plan.md"
Level: 2
Items:
  - Title: Insert handlers in bitmap.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in brush.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in console.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in printing.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in devicemanagement.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in resource.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in security.cpp
    Status: Atomic
    Content:
---

# Small TU batch S1

Slug: `SmallTUBatchS1` - from title "Small TU batch S1" (no drop-words, PascalCased, 14 chars, ASCII).

## Goal

Give the seven smallest wrapper TUs of `src/platform/os/windows/api_layer/user` their call-site handlers: **10 sites over 7 files**, plus one include line per file.

## Context

Inherited from `../../BatchSmallTUs/plan.md` (which inherits `../plan.md` of the root): the U1 pass installs `Win32ErrorHandler _;` at all **597 sites / 53 TUs**; this batch is 10 of those sites over `bitmap.cpp` 1, `brush.cpp` 1, `console.cpp` 1, `printing.cpp` 1, `devicemanagement.cpp` 2, `resource.cpp` 2, `security.cpp` 2.

Per-file procedure, identical for every item here:
1. Snapshot the file; **do not** write through any pipeline that can truncate (`if($null -eq $t) throw` and `if($out.Length -lt 1) throw` immediately before each write).
2. Insert `Win32ErrorHandler _;` at every line whose **first non-whitespace token** on a comment-stripped line is `qor_pp_fcontext;` - never a substring match, because one TU banner in the folder spells the anchor in prose. Indentation 8 spaces, statement **before** the `qor_pp_useswinapi*` line.
3. Add `#include "../kernel/errorhandler.h"` in the **generic** block right after `#include "src/qor/flyers/error/error.h"` and before the `//Windows specific headers must be last ...` marker - **only if absent**, so a re-run is a no-op.
4. Re-measure that TU: expect per-TU `qor_pp_fcontext; == qor_pp_useswinapi* == Library::*Call< == Win32ErrorHandler` at the TU's site count above, and tick `registers/u1.md`'s working record for that TU the moment it verifies (U1 stays OPEN).
5. Keep the file **pure ASCII**; never touch the TU banner (protocol 7).

No gate runs per TU in this batch - the probe TU (`accelerator.cpp`, a sibling of the parent plan) is the only standalone-gate leg; the folder gate (54/54 exit 0, `/Fo` attached, dated by `cmd /c dir /T W`) is a sibling item of the root.

## Decisions

Inherited from the parent plans: **D50** closes U1 as the reversal of the settled 29 Oct 2026 "leave as-is" (handler half only); **U2 stays settled** - no `CheckReturn`/`CheckNonZeroHandle`; **`user32.h`/`wsprintfT` untouched**; **U1 stays OPEN** until the dated folder gate is clean; **banners never edited**.

## Insert handlers in bitmap.cpp

Atomic: one file, one scripted edit. `bitmap.cpp` - **1 site**; after the pass its census reads `1 / 1 / 1 / 1` and it carries the include. Note its banner (`bitmap.cpp:74`) asserts the stale no-handler policy - read it, do not edit it.

## Insert handlers in brush.cpp

Atomic: one file, one scripted edit. `brush.cpp` - **1 site**; census `1 / 1 / 1 / 1` plus the include.

## Insert handlers in console.cpp

Atomic: one file, one scripted edit. `console.cpp` - **1 site**; census `1 / 1 / 1 / 1` plus the include.

## Insert handlers in printing.cpp

Atomic: one file, one scripted edit. `printing.cpp` - **1 site**; census `1 / 1 / 1 / 1` plus the include.

## Insert handlers in devicemanagement.cpp

Atomic: one file, one scripted edit. `devicemanagement.cpp` - **2 sites**; census `2 / 2 / 2 / 2` plus the include.

## Insert handlers in resource.cpp

Atomic: one file, one scripted edit. `resource.cpp` - **2 sites**; census `2 / 2 / 2 / 2` plus the include.

## Insert handlers in security.cpp

Atomic: one file, one scripted edit. `security.cpp` - **2 sites**; census `2 / 2 / 2 / 2` plus the include.
