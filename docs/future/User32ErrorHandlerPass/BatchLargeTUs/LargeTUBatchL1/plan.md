---
Type: Plan
Title: "Large TU batch L1"
Parent: "../plan.md"
Level: 2
Items:
  - Title: Insert handlers in mouse.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in rectangle.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in scrollbar.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in icon.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in touch.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in windowclass.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in cursor.cpp
    Status: Atomic
    Content:
---

# Large TU batch L1

Slug: `LargeTUBatchL1` - from title "Large TU batch L1" (no drop-words, PascalCased, ASCII, 14 chars, inside the 24-char cap); no collision with the sibling `SmallTUBatchS1`. Uniqueness stored here.

## Goal

Give the seven 10-15-site TUs of `src/platform/os/windows/api_layer/user` their call-site handlers: **90 sites over 7 files**, plus one include line per file.

## Context

Inherited from `../plan.md` (which inherits `../../plan.md` of the root): the U1 pass installs `Win32ErrorHandler _;` at all **597 sites over 53 TUs** of `src/platform/os/windows/api_layer/user`; the probe TU is `accelerator.cpp` (5), the small region is 33 TUs / 124, the large region is 19 TUs / 468, of which this batch is **7 TUs / 90 sites**. Measured against the disk this pass, and 0 handlers / 0 `errorhandler.h` includes exist in the folder yet.

Per-file procedure, identical for every item here:
1. Snapshot the file; **do not** write through any pipeline that can truncate - `if($null -eq $t) throw` and `if($out.Length -lt 1) throw` immediately before each write (the `record-editing.md` truncation trap).
2. Insert `Win32ErrorHandler _;` at every line whose **first non-whitespace token** on a **comment-stripped** line is `qor_pp_fcontext;` - never a substring match, because `painting.cpp:74` (a sibling batch's TU) spells the anchor with its semicolon inside prose, and `rectangle.cpp:90` / `touch.cpp:75` name the token without the semicolon in comments. Indentation **8 spaces**, statement **before** the `qor_pp_useswinapi*` line.
3. Add `#include "../kernel/errorhandler.h"` in the **generic** block right after `#include "src/qor/flyers/error/error.h"` and before the `//Windows specific headers must be last ...` marker - **only if absent**, so a re-run is a no-op.
4. Re-measure that TU: expect per-TU `qor_pp_fcontext; == qor_pp_useswinapi* == Library::*Call< == Win32ErrorHandler` at the TU's count below, and tick `registers/u1.md`'s working record for that TU the moment it verifies (U1 stays OPEN).
5. Keep the file **pure ASCII**; never touch the TU banner (protocol 7); never drive the inserter from a banner number.

Running folder totals after this batch must read **5+124+90 = 219 handlers over 41 TUs** of the 53 (probe + small region + this batch). No gate runs per TU or per batch - the only standalone gate is the probe TU's, and the folder gate (**54/54 exit 0**, `/Fo` attached, dated by `cmd /c dir /T W`) is a sibling item of the root.

## Decisions

Inherited from the parent plans: **D50** is the closure record for the reversal of the settled 29 Oct 2026 "leave as-is" (handler half only); **U2 stays settled** - no `CheckReturn`/`CheckNonZeroHandle` anywhere; **`user32.h`/`wsprintfT` untouched**; **U1 stays OPEN** until the dated folder gate is clean; **banners never edited**; nothing appended to frozen or closed records (`tooling.md` frozen at session 34).

## Insert handlers in mouse.cpp

Atomic: one file, one scripted edit. `mouse.cpp` - **10 sites** (166 lines); after the pass its census reads `10 / 10 / 10 / 10` and it carries the include. It holds one `Library::voidCall` bare statement, so an anchored `^\s*return Library::` census would read 9 for this TU - count comment-stripped substrings.

## Insert handlers in rectangle.cpp

Atomic: one file, one scripted edit. `rectangle.cpp` - **11 sites** (173 lines); census `11 / 11 / 11 / 11` plus the include. Its banner (`rectangle.cpp:15` "Eleven wrappers") and `:90` ("Call-site idiom: 11/11 qor_pp_fcontext and 11/11 explicit Call<> ...") are prose mentions of the anchor token - read them, do not edit them, and do not anchor on them.

## Insert handlers in scrollbar.cpp

Atomic: one file, one scripted edit. `scrollbar.cpp` - **12 sites** (155 lines); census `12 / 12 / 12 / 12` plus the include.

## Insert handlers in icon.cpp

Atomic: one file, one scripted edit. `icon.cpp` - **14 sites** (168 lines); census `14 / 14 / 14 / 14` plus the include.

## Insert handlers in touch.cpp

Atomic: one file, one scripted edit. `touch.cpp` - **14 sites** (249 lines); census `14 / 14 / 14 / 14` plus the include. `touch.cpp:75` is a comment line that names `qor_pp_fcontext` without the semicolon - comment-stripping must remove it before the inserter runs.

## Insert handlers in windowclass.cpp

Atomic: one file, one scripted edit. `windowclass.cpp` - **14 sites** (160 lines); census `14 / 14 / 14 / 14` plus the include. Its banner (`windowclass.cpp:16` "14 wrappers") agrees with the measured count - that agreement is a coincidence, not a licence to drive the inserter from the banner.

## Insert handlers in cursor.cpp

Atomic: one file, one scripted edit. `cursor.cpp` - **15 sites** (158 lines); census `15 / 15 / 15 / 15` plus the include. The last item of the batch: after it, the batch census must re-sum to **90 sites over 7 TUs**.
