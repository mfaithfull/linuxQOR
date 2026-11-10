---
Type: Plan
Title: "Small TU batch S4"
Parent: "../plan.md"
Level: 2
Items:
  - Title: Insert handlers in systeminfo.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in text.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in help.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in ime.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in monitor.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in processthread.cpp
    Status: Atomic
    Content:
  - Title: Insert handlers in windowproperties.cpp
    Status: Atomic
    Content:
---

# Small TU batch S4

Slug: `SmallTUBatchS4` - from title "Small TU batch S4" (no drop-words, PascalCased, 14 chars, ASCII, inside the 24-char cap). Uniqueness is stored here, not recomputed.

## Goal

Give the fourth seven smallest wrapper TUs of `src/platform/os/windows/api_layer/user` their call-site handlers: **33 sites over 7 files**, plus one include line per file - so when this batch closes the small region's census reads **88 sites over 28 TUs** (S1 10/7 + S2 18/7 + S3 27/7 + S4 33/7).

## Context

Inherited from `../../BatchSmallTUs/plan.md` (which inherits the root `../plan.md`): the U1 pass installs `Win32ErrorHandler _;` at all **597 sites over the 53 wrapper TUs** (650 edits incl. 53 include lines). This batch is 7 TUs / **33 sites**; the counts below were re-measured, not inherited.

Baseline measured on disk at this resume (not inherited): folder anchored `^\s*qor_pp_fcontext;` = **597** against raw substring **598** (the anchor trap); TUs including `errorhandler.h` = **0**; code hits of `Win32ErrorHandler` = **0** (all 41 raw hits are banner prose); `^\s*CheckReturn` code lines = **4** (`errorhandling.cpp:81`, `processthread.cpp:85`, `:94`, `:110`). S1-S3 are `Refined` (planned), so this batch still starts from a clean baseline.

**Census-pattern warning, hardest case here.** The `Library::*Call<` column must be counted **comment-stripped and by substring**, never anchored: in `processthread.cpp` **3 of its 5** call sites are `CheckReturn`-prefixed lines where the call is not at line start (`:85`, `:94`, `:110`), so an anchored census reads **2/5** for that TU and looks like three missed inserts. Folder-wide the same trap reads 593 (`^\s*(return\s+)?Library::`) / 589 (`^\s*return Library::`) against the 597. Raw substring over-counts the other way: `Library::` reads 8 in `windowproperties.cpp` and 6 in `ime`/`monitor` because banners quote it.

Per-file procedure, identical for every item here:
1. Snapshot the file; do not write through any pipeline that can truncate - `if($null -eq $t) throw` and `if($out.Length -lt 1) throw` immediately before each write (the `record-editing.md` truncation trap).
2. Insert `Win32ErrorHandler _;` at every line whose **first non-whitespace token** on a **comment-stripped** line is `qor_pp_fcontext;` - never a substring match. Indentation **8 spaces**, statement **after** the anchor and **before** the `qor_pp_useswinapi*` line.
3. Add `#include "../kernel/errorhandler.h"` in the **generic** block right after `#include "src/qor/flyers/error/error.h"` and **before** the `//Windows specific headers must be last ...` marker - **only if absent**, so a re-run is a no-op.
4. Re-measure that TU the moment it is written: expect per-TU `qor_pp_fcontext; == qor_pp_useswinapi* == Library::*Call< == Win32ErrorHandler` at the TU's site count (last column comment-stripped substring), and tick `registers/u1.md`'s working record for that TU immediately (U1 stays OPEN).
5. Keep the file **pure ASCII**; never touch the TU banner (protocol 7).

Measured anchors for the seven TUs of this batch - line numbers are provenance only; the inserter anchors on content, never on line numbers:

| TU | sites | `qor_pp_fcontext;` code at | prose `qor_pp_fcontext` at | banner `Win32ErrorHandler` at | Windows marker |
|---|--:|---|---|---|--:|
| systeminfo.cpp | 4 | 92, 103, 114, 126 | none | 50 | 80 |
| text.cpp | 4 | 99, 106, 113, 120 | none | 48 | 9 |
| help.cpp | 5 | 110, 117, 124, 131, 138 | none | 64, 104 | 9 |
| ime.cpp | 5 | 62, 69, 78, 88, 97 | none | 43 | 9 |
| monitor.cpp | 5 | 128, 135, 142, 149, 156 | none | 109 | 9 |
| processthread.cpp | 5 | 83, 92, 101, 108, 117 | none | 54, 55, 59 | 9 |
| windowproperties.cpp | 5 | 80, 87, 94, 101, 108 | 22 | 49, 69 | 9 |

`#include "src/qor/flyers/error/error.h"` sits at **line 7** in all seven; the Windows-specific marker sits at **line 9** in all seven except `systeminfo.cpp`, where it sits at **line 80**.

Banner prose contradicts the disk in `windowproperties.cpp` and must not be trusted for line numbers (nor edited): its banner at `:22` quotes `qor_pp_fcontext` "(:81, :88, :95, :102, :109)" and `Library::Call` "(:83, :90, :97, :104, :111)", while the code on disk reads **80, 87, 94, 101, 108** and **82, 89, 96, 103, 110** - every quoted number is one line low. The comment-stripped first-token rule decides; a number-driven inserter would miss all five anchors here.

`processthread.cpp` is the pass's densest interaction TU: its banner at `:59` states "the 4 CheckReturn sites in the library (the 4th is errorhandling.cpp MessageBeep); G6 keeps all three live", and those three live sites are `:85` (`CheckReturn< BOOL, BoolCheck >`), `:94` (`CheckReturn< DWORD, CheckNonZero< DWORD> >`), `:110` (`CheckReturn< BOOL, BoolCheck >`) - **3 of the folder's 4**. Their paired anchors are `:83`, `:92`, `:108`; the other two wrappers pair `:101` -> `Library::Call<BOOL>` at `:103` and `:117` at `:119`. Insert only the handler line; do not reformat, retype or reorder any `CheckReturn` line.

`systeminfo.cpp` and `windowproperties.cpp` also carry prose that reads as a policy claim ("0 Win32ErrorHandler and 0 CheckReturn") next to real code anchors - skip-only prose, never an absence signal.

## Decisions

Inherited from `../../BatchSmallTUs/plan.md` and the root `../plan.md`: **D50** is the closure record for reversing the settled 29 Oct 2026 "leave as-is" for the **handler half only**; **U2 stays settled** - no `CheckReturn`/`CheckNonZeroHandle` is introduced anywhere, and the 4 existing sites keep their semantics, **3 of which live in this batch's `processthread.cpp`** (`:85`, `:94`, `:110`, kept live by G6): the pass only adds the handler beside them, so those wrappers move from "escalate" to "emit + escalate" per the root's measured delta row; **`user32.h` and `wsprintfT` untouched**, so the 122-source all-libraries gate is not in play and the folder gate is the audit; **U1 stays OPEN** until all 53 TUs / 597 wrappers carry the handler and the dated folder gate reads 54/54 exit 0; **banners never edited** (protocol 7 - this batch has 10 such prose lines, all listed above); **no gate runs per TU** in this batch - the only gate legs are the probe TU's standalone `/W4` run (`accelerator.cpp`, a sibling of the parent plan) and the root's folder gate.

## Insert handlers in systeminfo.cpp

Atomic: one file, one scripted edit. `systeminfo.cpp` - **4 sites** (anchors 92, 103, 114, 126); after the edit its census reads `4 / 4 / 4 / 4` and it carries the include. Its Windows marker sits at `:80`, so the include goes between `:7` and `:80`; banner policy prose at `:50` and `:52` is skip-only.

## Insert handlers in text.cpp

Atomic: one file, one scripted edit. `text.cpp` - **4 sites** (anchors 99, 106, 113, 120); census `4 / 4 / 4 / 4` plus the include. Banner policy prose at `:48`; one prose `Library::` mention sits in its banner, so the call column must be comment-stripped.

## Insert handlers in help.cpp

Atomic: one file, one scripted edit. `help.cpp` - **5 sites** (anchors 110, 117, 124, 131, 138); census `5 / 5 / 5 / 5` plus the include. Two policy-prose lines at `:64` and `:104`; `:104` sits directly above the first anchor, so a snapshot-and-splice edit must not treat it as a site.

## Insert handlers in ime.cpp

Atomic: one file, one scripted edit. `ime.cpp` - **5 sites** (anchors 62, 69, 78, 88, 97); census `5 / 5 / 5 / 5` plus the include. Note the uneven spacing - anchors are 7, 9, 10, 9 lines apart, so nothing in this TU is evenly spaced and a stride-driven edit would drift. Banner prose at `:43`, `:75`, `:84`.

## Insert handlers in monitor.cpp

Atomic: one file, one scripted edit. `monitor.cpp` - **5 sites** (anchors 128, 135, 142, 149, 156); census `5 / 5 / 5 / 5` plus the include. Banner prose at `:109` states "no CheckReturn and no Win32ErrorHandler" for a TU that is about to carry 5 handlers - skip-only.

## Insert handlers in processthread.cpp

Atomic: one file, one scripted edit - and the TU where the census trap bites. `processthread.cpp` - **5 sites** (anchors 83, 92, 101, 108, 117); after the edit its census reads `5 / 5 / 5 / 5`, with the call column counted comment-stripped because **3 of its 5** call lines are `CheckReturn`-prefixed at `:85`, `:94`, `:110`. Insert the handler at the paired anchors only; never reformat, retype or reorder those three lines. Banner prose at `:54`, `:55`, `:59`.

## Insert handlers in windowproperties.cpp

Atomic: one file, one scripted edit. `windowproperties.cpp` - **5 sites** (anchors 80, 87, 94, 101, 108); census `5 / 5 / 5 / 5` plus the include. Prose `qor_pp_fcontext` at `:22` quotes anchor numbers one line low (:81, :88, :95, :102, :109) against the disk's 80, 87, 94, 101, 108 - trust the content rule, not the banner. Last of the batch: after it, re-census the region - the small region must read **88 sites over 28 TUs** (S1-S4), which is the gate for S5 starting.

