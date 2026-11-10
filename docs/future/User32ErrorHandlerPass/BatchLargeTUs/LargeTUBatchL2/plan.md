---
Type: Plan
Title: "Large TU batch L2"
Parent: "../plan.md"
Level: 2
Items:
  - Title: Insert handlers in desktop.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in clipboard.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in dpi.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in dialog.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in misc.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in message.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in painting.cpp
    Status: Atomic
    Content:
---

# Large TU batch L2

Slug: `LargeTUBatchL2` - from title "Large TU batch L2" (no drop-words, PascalCased, ASCII, 14 chars, inside the 24-char cap); no collision with `SmallTUBatchS2`. Uniqueness stored here.

## Goal

Give the seven 18-25-site TUs of `src/platform/os/windows/api_layer/user` their call-site handlers: **153 sites over 7 files**, plus one include line per file.

## Context

Inherited from `../plan.md` (which inherits `../../plan.md` of the root): the U1 pass installs `Win32ErrorHandler _;` at all **597 sites over 53 TUs** of `src/platform/os/windows/api_layer/user`; the probe TU is `accelerator.cpp` (5), the small region is 33 TUs / 124, the large region is 19 TUs / 468. This batch is **7 TUs / 153 sites** and runs after `LargeTUBatchL1` (90 sites over 7 TUs), so the running folder total entering it is **219 handlers over 41 TUs** and leaving it must be **372 over 48 TUs**. Measured against the disk this pass; 0 handlers and 0 `errorhandler.h` includes exist in the folder yet.

**This batch holds the folder's only semicolon-bearing prose anchor: `painting.cpp:74`** - `//Call-site idiom: 25/25 carry qor_pp_fcontext; and 25/25 spell explicit Call<> template arguments`. Raw substring hits for `qor_pp_fcontext;` in `painting.cpp` read **26** against **25** code anchors. `dpi.cpp:54` is a second comment mention of the token (no semicolon).

Per-file procedure, identical for every item here:
1. Snapshot the file; **do not** write through any pipeline that can truncate - `if($null -eq $t) throw` and `if($out.Length -lt 1) throw` immediately before each write (the `record-editing.md` truncation trap).
2. Insert `Win32ErrorHandler _;` at every line whose **first non-whitespace token** on a **comment-stripped** line is `qor_pp_fcontext;` - never a substring match. Indentation **8 spaces**, statement **before** the `qor_pp_useswinapi*` line.
3. Add `#include "../kernel/errorhandler.h"` in the **generic** block right after `#include "src/qor/flyers/error/error.h"` and before the `//Windows specific headers must be last ...` marker - **only if absent**, so a re-run is a no-op.
4. Re-measure that TU: expect per-TU `qor_pp_fcontext; == qor_pp_useswinapi* == Library::*Call< == Win32ErrorHandler` at the TU's count below, and tick `registers/u1.md`'s working record for that TU the moment it verifies (U1 stays OPEN).
5. Keep the file **pure ASCII**; never touch the TU banner (protocol 7); never drive the inserter from a banner number.

No gate runs per TU or per batch - the only standalone gate is the probe TU's, and the folder gate (**54/54 exit 0**, `/Fo` attached, dated by `cmd /c dir /T W`) is a sibling item of the root.

## Decisions

Inherited from the parent plans: **D50** is the closure record for the reversal of the settled 29 Oct 2026 "leave as-is" (handler half only); **U2 stays settled** - no `CheckReturn`/`CheckNonZeroHandle` anywhere; **`user32.h`/`wsprintfT` untouched**; **U1 stays OPEN** until the dated folder gate is clean; **banners never edited**; nothing appended to frozen or closed records (`tooling.md` frozen at session 34).

## Insert handlers in desktop.cpp

Atomic: one file, one scripted edit. `desktop.cpp` - **18 sites** (184 lines); after the pass its census reads `18 / 18 / 18 / 18` and it carries the include.

## Insert handlers in clipboard.cpp

Atomic: one file, one scripted edit. `clipboard.cpp` - **20 sites** (177 lines); census `20 / 20 / 20 / 20` plus the include. Its banner (`clipboard.cpp:15` "Twenty wrappers, one per declaration in the Clipboard functions section") agrees with the measured count - coincidence, not a licence to trust the banner.

## Insert handlers in dpi.cpp

Atomic: one file, one scripted edit. `dpi.cpp` - **21 sites** (271 lines); census `21 / 21 / 21 / 21` plus the include. `dpi.cpp:54` is a comment line naming `qor_pp_fcontext` without the semicolon ("Binding (D13): 21 qor_pp_useswinapi sites, all bare-stem, 21 qor_pp_fcontext, 21/21 Library::Call ...") - comment-stripping must remove it before the inserter runs.

## Insert handlers in dialog.cpp

Atomic: one file, one scripted edit. `dialog.cpp` - **22 sites** (191 lines); census `22 / 22 / 22 / 22` plus the include.

## Insert handlers in misc.cpp

Atomic: one file, one scripted edit. `misc.cpp` - **23 sites** (255 lines); census `23 / 23 / 23 / 23` plus the include. It holds one `Library::voidCall` bare statement, so an anchored `^\s*return Library::` census reads 22 for this TU - the invariant must be counted comment-stripped and by substring.

## Insert handlers in message.cpp

Atomic: one file, one scripted edit. `message.cpp` - **24 sites** (273 lines); census `24 / 24 / 24 / 24` plus the include. It holds one `Library::voidCall` bare statement (same census caveat as `misc.cpp`).

## Insert handlers in painting.cpp

Atomic: one file, one scripted edit. `painting.cpp` - **25 sites** (260 lines); census `25 / 25 / 25 / 25` plus the include. **The anchor trap's home:** raw grep for `qor_pp_fcontext;` in this TU reads **26** because `painting.cpp:74` spells the anchor with its semicolon inside the banner prose. After the pass this TU must still read **25** handlers, not 26 - a handler inserted at the prose line would also break the banner (protocol 7).
