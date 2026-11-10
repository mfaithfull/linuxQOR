---
Type: Plan
Title: "Large TU batch L3"
Parent: "../plan.md"
Level: 2
Items:
  - Title: Insert handlers in pointer.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in keyboard.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in strings.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in menu.cpp
    Status: Atomic
    Content:
---

# Large TU batch L3

Slug: `LargeTUBatchL3` - from title "Large TU batch L3" (no drop-words, PascalCased, ASCII, 14 chars, inside the 24-char cap); no collision with `SmallTUBatchS3`. Uniqueness stored here.

## Goal

Give the four 32-39-site TUs of `src/platform/os/windows/api_layer/user` their call-site handlers: **139 sites over 4 files**, plus one include line per file.

## Context

Inherited from `../plan.md` (which inherits `../../plan.md` of the root): the U1 pass installs `Win32ErrorHandler _;` at all **597 sites over 53 TUs** of `src/platform/os/windows/api_layer/user`; the probe TU is `accelerator.cpp` (5), the small region is 33 TUs / 124, `LargeTUBatchL1` is 7 TUs / 90 and `LargeTUBatchL2` is 7 TUs / 153. This batch is **4 TUs / 139 sites**; entering it the folder must read **372 handlers over 48 TUs**, and leaving it (with the `window.cpp` sibling item) it must read **597 over 53 TUs**. Measured against the disk this pass; 0 handlers and 0 `errorhandler.h` includes exist in the folder yet.

Per-file procedure, identical for every item here:
1. Snapshot the file; **do not** write through any pipeline that can truncate - `if($null -eq $t) throw` and `if($out.Length -lt 1) throw` immediately before each write (the `record-editing.md` truncation trap).
2. Insert `Win32ErrorHandler _;` at every line whose **first non-whitespace token** on a **comment-stripped** line is `qor_pp_fcontext;` - never a substring match. Indentation **8 spaces**, statement **before** the `qor_pp_useswinapi*` line.
3. Add `#include "../kernel/errorhandler.h"` in the **generic** block right after `#include "src/qor/flyers/error/error.h"` and before the `//Windows specific headers must be last ...` marker - **only if absent**, so a re-run is a no-op.
4. Re-measure that TU: expect per-TU `qor_pp_fcontext; == qor_pp_useswinapi* == Library::*Call< == Win32ErrorHandler` at the TU's count below, and tick `registers/u1.md`'s working record for that TU the moment it verifies (U1 stays OPEN).
5. Keep the file **pure ASCII**; never touch the TU banner (protocol 7); never drive the inserter from a banner number.

These are the four largest TUs before `window.cpp`, so the per-file re-measure is where a missed or doubled site is cheapest to catch: do not start the next TU until the previous one's census is exact.

## Decisions

Inherited from the parent plans: **D50** is the closure record for the reversal of the settled 29 Oct 2026 "leave as-is" (handler half only); **U2 stays settled** - no `CheckReturn`/`CheckNonZeroHandle` anywhere; **`user32.h`/`wsprintfT` untouched**; **U1 stays OPEN** until the dated folder gate is clean; **banners never edited**; nothing appended to frozen or closed records (`tooling.md` frozen at session 34).

## Insert handlers in pointer.cpp

Atomic: one file, one scripted edit. `pointer.cpp` - **32 sites** (292 lines); census `32 / 32 / 32 / 32` plus the include. `pointer.cpp:37` is a banner line that quotes its own idiom counts ("Idiom counts: 32 qor_pp_fcontext, 32 definition heads qualified `User32::`, 31 `Library::Call` + 1 ...") - that "+1" is this TU's `Library::voidCall`, so an anchored `^\s*return Library::` census reads 31 here. Read the banner, never edit it.

## Insert handlers in keyboard.cpp

Atomic: one file, one scripted edit. `keyboard.cpp` - **34 sites** (316 lines); census `34 / 34 / 34 / 34` plus the include. It holds one `Library::voidCall` bare statement (same census caveat as `pointer.cpp`). Its banner (`:15` "34 wrappers - one per `user32.h:70-104` Keyboard Input functions declaration") agrees with the measured count; `:50` and `:56` carry A/W-width and contract prose that is not anchors.

## Insert handlers in strings.cpp

Atomic: one file, one scripted edit. `strings.cpp` - **34 sites** (328 lines); census `34 / 34 / 34 / 34` plus the include. No `Library::voidCall` and no comment mention of the anchor token in this TU - the plain anchor rule applies.

## Insert handlers in menu.cpp

Atomic: one file, one scripted edit. `menu.cpp` - **39 sites** (358 lines); census `39 / 39 / 39 / 39` plus the include. The largest TU before `window.cpp`. Its banner quotes tallies that are **not** anchors (`:53` "35 of 39 wrappers have a LAYER call site", `:83` "24 of the 39 wrappers return BOOL") - the inserter's only licence is the measured 39 code anchors.
