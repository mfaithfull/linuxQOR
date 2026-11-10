---
Type: Plan
Title: "Small TU batch S5"
Parent: "../plan.md"
Level: 2
Items:
  - Title: Insert handlers in systemshutdown.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in displayconfig.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in rawinput.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in caret.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in devicecontext.cpp
    Status: Atomic
    Content:
---

# Small TU batch S5

Slug: `SmallTUBatchS5` - from title "Small TU batch S5" (no drop-words, PascalCased, 14 chars, ASCII, inside the 24-char cap). Uniqueness is stored here, not recomputed.

## Goal

Give the last five TUs of the small region their call-site handlers: **36 sites over 5 files** (`systemshutdown.cpp` 6, `displayconfig.cpp` 7, `rawinput.cpp` 7, `caret.cpp` 8, `devicecontext.cpp` 8), plus one include line per file. Closing this batch closes the region: the small region must then read **124 sites over 33 TUs** (S1 10/7 + S2 18/7 + S3 27/7 + S4 33/7 + S5 36/5), which with the probe's 5 and the large region's 468 gives the pass total **597 over 53 TUs**.

## Context

Inherited from `../../BatchSmallTUs/plan.md` (which inherits the root `../plan.md`): the U1 pass installs `Win32ErrorHandler _;` at all **597 sites over the 53 wrapper TUs** of `src/platform/os/windows/api_layer/user` (650 edits incl. 53 include lines). This batch is 5 TUs / **36 sites** of the region's 33 TUs / 124 sites; the counts below were re-measured on disk at this resume, not inherited.

Baseline measured on disk at this resume (not inherited): folder = **54 `.cpp` sources**; anchored `^\s*qor_pp_fcontext;` = **597** against raw substring **598** (the anchor trap); TUs including `errorhandler.h` = **0**; `Win32ErrorHandler _;` occurs **once** folder-wide and that occurrence is banner prose (`desktop.cpp:56` - "No wrapper in this TU has a Win32ErrorHandler _; (0/18, G5)"), so **code occurrences are still 0**. S1-S4 are `Refined` (planned, not executed), so this batch still starts from a clean baseline.

Per-file procedure, identical for every item here:
1. Snapshot the file; do not write through any pipeline that can truncate - `if($null -eq $t) throw` and `if($out.Length -lt 1) throw` immediately before each write (the `record-editing.md` truncation trap).
2. Insert `Win32ErrorHandler _;` at every line whose **first non-whitespace token** on a **comment-stripped** line is `qor_pp_fcontext;` - never a substring match. Indentation **8 spaces**, statement **after** the anchor and **before** the `qor_pp_useswinapi*` line.
3. Add `#include "../kernel/errorhandler.h"` in the **generic** block right after `#include "src/qor/flyers/error/error.h"` and **before** the `//Windows specific headers must be last ...` marker - **only if absent**, so a re-run is a no-op.
4. Re-measure that TU the moment it is written: expect per-TU `qor_pp_fcontext; == qor_pp_useswinapi* == Library::*Call< == Win32ErrorHandler` at the TU's site count, and tick `registers/u1.md`'s working record for that TU immediately (U1 stays OPEN).
5. Keep the file **pure ASCII**; never touch the TU banner (protocol 7).

Measured anchors for the five TUs of this batch - line numbers are provenance only; the inserter anchors on content, never on line numbers:

| TU | sites | `qor_pp_fcontext;` anchors | prose `qor_pp_fcontext` (skip) | prose `qor_pp_useswinapi` (skip) | prose `Win32ErrorHandler` (skip) | `error.h` | Windows marker |
|---|--:|---|---|---|---|--:|--:|
| systemshutdown.cpp | 6 | 80, 91, 98, 105, 112, 119 | 70 | 26, 70 | 54, 76, 86 | 7 | 9 |
| displayconfig.cpp | 7 | 115, 126, 137, 149, 164, 174, 189 | 70 | 70 | 71 | 7 | 103 |
| rawinput.cpp | 7 | 87, 97, 108, 119, 129, 138, 151 | - | 23, 25, 43 | 49 | 7 | 74 |
| caret.cpp | 8 | 23, 30, 37, 44, 51, 58, 65, 72 | - | - | - | 7 | 9 |
| devicecontext.cpp | 8 | 59, 66, 73, 80, 87, 94, 101, 108 | - | 19, 20 | - | 7 | 9 |

Census findings measured for this batch - they run **opposite** to the S3/S4 warning, so do not inherit that rule:
- **No `CheckReturn` code line exists in any of these five TUs** (`^\s*CheckReturn` = 0 each; the folder's 4 live sites are `errorhandling.cpp:81` and `processthread.cpp:85`/`:94`/`:110`). The under-count trap that bit S3/S4 therefore does **not** bite here. The trap that does bite is the **other direction**: banner prose inflates a raw substring `Library::` census - `systemshutdown.cpp` reads **10** against its **6** code sites (prose at `:21`, `:42`, `:70`, `:88`) and `displayconfig.cpp` reads **8** against **7** (prose at `:70`). `caret` (8), `rawinput` (7) and `devicecontext` (8) read clean. Count that column **comment-stripped**, never raw.
- Two TUs place the Windows-specific marker far below the generic block: `displayconfig.cpp:103` and `rawinput.cpp:74` (`systemshutdown`, `caret`, `devicecontext` sit at `:9`). The include goes between `:7` and the marker - **marker-relative**, never a fixed line number.
- `systemshutdown.cpp:54` banner prose says "5/5 ... in all five wrappers" while the TU holds **6** code anchors - a stale count; `:70` says 6/6/6 and agrees. Both lines are skip-only prose; the correction belongs in `registers/u1.md` and **D50**, never in the banner.
- `qor_pp_useswinapiAW` spellings live in this batch too (`rawinput.cpp:120` and `devicecontext.cpp:60`/`:67`/`:74`/`:81`/`:88`) - the handler insert is independent of the macro spelling.
- `systemshutdown.cpp` has two zero-arg wrappers (`Library::Call< BOOL >(pFunc)` at `:93`, `:100`) - they still take the handler.
- `caret.cpp` is the only TU of this batch with **no prose hits at all** (77 lines, 8 anchors) - the cheapest, most-controlled edit in the region so far.

## Decisions

Inherited from `../../BatchSmallTUs/plan.md` and the root `../plan.md`: **D50** is the closure record for reversing the settled 29 Oct 2026 "leave as-is" for the **handler half only**; **U2 stays settled** - no `CheckReturn`/`CheckNonZeroHandle` is introduced anywhere (and none exists as code in any TU of this batch, so nothing here needs preserving around); **`user32.h` and `wsprintfT` untouched**, so the 122-source all-libraries gate is not in play and the folder gate is the audit; **U1 stays OPEN** until all 53 TUs / 597 wrappers carry the handler and the dated folder gate reads 54/54 exit 0; **banners never edited** (protocol 7); **no gate runs per TU** in this batch - the only gate legs are the probe TU's standalone `/W4` run (`accelerator.cpp`, a sibling of the parent plan) and the root's folder gate.

## Insert handlers in systemshutdown.cpp

Atomic: one file, one scripted edit. `systemshutdown.cpp` - **6 sites** (anchors 80, 91, 98, 105, 112, 119); after the edit its census reads `6 / 6 / 6 / 6` and it carries the include. Skip-only prose: `qor_pp_fcontext` at `:70`, `qor_pp_useswinapi` at `:26` and `:70`, `Win32ErrorHandler` at `:54`, `:76`, `:86`. Windows marker at `:9`. Two zero-arg wrappers (`Library::Call< BOOL >(pFunc)` at `:93`, `:100`) still take the handler. First of the batch.

## Insert handlers in displayconfig.cpp

Atomic: one file, one scripted edit. `displayconfig.cpp` - **7 sites** (anchors 115, 126, 137, 149, 164, 174, 189); census `7 / 7 / 7 / 7` plus the include. One prose line carries all three census words (`:70`) and `:71` carries `Win32ErrorHandler`; the Windows marker sits at `:103`, so the include goes between `:7` and `:103`.

## Insert handlers in rawinput.cpp

Atomic: one file, one scripted edit. `rawinput.cpp` - **7 sites** (anchors 87, 97, 108, 119, 129, 138, 151); census `7 / 7 / 7 / 7` plus the include. No prose `qor_pp_fcontext` here (anchored and raw both read 7), but prose `qor_pp_useswinapi` at `:23`, `:25`, `:43` and prose `Win32ErrorHandler` at `:49`; Windows marker at `:74`. `:120` uses the `qor_pp_useswinapiAW` spelling - the insert is independent of it.

## Insert handlers in caret.cpp

Atomic: one file, one scripted edit. `caret.cpp` - **8 sites** (anchors 23, 30, 37, 44, 51, 58, 65, 72); census `8 / 8 / 8 / 8` plus the include. Cleanest TU of the region so far: no prose hit on any census word, evenly spaced anchors (7 lines apart), Windows marker at `:9`.

## Insert handlers in devicecontext.cpp

Atomic: one file, one scripted edit. `devicecontext.cpp` - **8 sites** (anchors 59, 66, 73, 80, 87, 94, 101, 108); census `8 / 8 / 8 / 8` plus the include. Prose `qor_pp_useswinapi` at `:19`, `:20`; five `qor_pp_useswinapiAW` spellings at `:60`, `:67`, `:74`, `:81`, `:88`; Windows marker at `:9`. **Last of the batch and of the small region**: after it, re-census the region - it must read **124 sites over 33 TUs**, which with the probe's 5 and the large region's 468 gives the pass total **597 over 53 TUs**.
