---
Type: Plan
Title: "Batch the small TUs"
Parent: "../plan.md"
Level: 1
Items:
  - Title: Small TU batch S1
    Status: Refined
    Content: ./SmallTUBatchS1/plan.md
  - Title: Small TU batch S2
    Status: Refined
    Content: ./SmallTUBatchS2/plan.md
  - Title: Small TU batch S3
    Status: Refined
    Content: ./SmallTUBatchS3/plan.md
  - Title: Small TU batch S4
    Status: Refined
    Content: ./SmallTUBatchS4/plan.md
  - Title: Small TU batch S5
    Status: Refined
    Content: ./SmallTUBatchS5/plan.md
---

# Batch the small TUs

Slug: `BatchSmallTUs` - from title "Batch the small TUs" ("the" dropped, PascalCased, 13 chars).

## Goal

Insert `Win32ErrorHandler _;` at every wrapper call site of the **33 smallest TUs** of `src/platform/os/windows/api_layer/user` (**124 sites**), smallest-first, so a cheap batch failure surfaces before the expensive TUs are touched.

## Context

Inherited from `../plan.md` (the U1 pass; source narrative `docs/future/winqor_api_user_U1_plan.md`, superseded provenance). Measured, not inherited.

- Folder: **54 sources**, 53 carry wrappers; the pass total is **597 sites / 53 TUs = 597 statement insertions + 53 include lines = 650 changes**. This region is 33 TUs / 124 sites; the probe TU (`accelerator.cpp`, 5) and the large region (19 TUs / 468 sites) are the sibling items of the parent.
- **Idiom, copied from `kernel`/`shell`, not invented:** include **`#include "../kernel/errorhandler.h"`** (the `shell/drag.cpp:11` form) in the **generic** block - immediately after `#include "src/qor/flyers/error/error.h"`, before the `//Windows specific headers must be last ...` comment - added **only if absent** so a re-run is a no-op. Statement `Win32ErrorHandler _;` **immediately after `qor_pp_fcontext;`, before the `qor_pp_useswinapi*` line**, at 8-space indentation.
- **Anchor trap:** raw grep for `qor_pp_fcontext;` over the folder reads **598**, one above the 597 code sites - exactly one TU banner spells the anchor with its semicolon in prose. Anchor on lines whose **first non-whitespace token** is `qor_pp_fcontext;` on a **comment-stripped** line; never a substring match.
- **Edit mechanics (per file):** scripted insertion; snapshot first, then `if($null -eq $t) throw` and `if($out.Length -lt 1) throw` immediately before any write (the `record-editing.md` truncation trap); re-measure after each file; sources **pure ASCII**; indentation exact.
- **Re-census invariant after every batch:** per-TU `qor_pp_fcontext; == qor_pp_useswinapi* == Library::*Call< == Win32ErrorHandler`; folder totals **597 / 597 / 597 / 597** with 53 TUs carrying the include. A batch that re-sums differently means the inserter missed or double-hit a site - stop and re-measure.
- **Tick the working record per TU the moment it is verified** (the U17 lesson: deferring ticks to the close forced a whole pass to be re-derived), while U1 stays OPEN.
- Gate discipline (parent's `## Decisions`): no gate is run per batch except the probe's standalone leg; the folder gate is a sibling item and must read **54/54 exit 0**.

Per-file site counts for this region (the exact counts the inserter must hit; the full alphabetical 53-row table is in the parent's `## Context`):

bitmap 1, brush 1, console 1, printing 1, devicemanagement 2, resource 2, security 2, synchronization 2, timer 2, windowprocedure 2, buttoncontrol 3, combobox 3, coordtransform 3, filledshapes 3, listbox 3, accessibility 4, errorhandling 4, hook 4, legacy 4, mdi 4, powermanagement 4, systeminfo 4, text 4, help 5, ime 5, monitor 5, processthread 5, windowproperties 5, systemshutdown 6, displayconfig 7, rawinput 7, caret 8, devicecontext 8 = **33 TUs / 124 sites**.

**Re-cut note:** the source narrative batched this region as B1 (15 TUs / 31 sites) and B2 (19 TUs / 98 sites); both exceed the 7-item breadth cap, so the region is re-cut into **S1-S5 of <= 7 TUs each**. B1 + B2 - `accelerator.cpp`'s 5 = 124 sites, which is what S1-S5 re-sum to. The re-sum is recorded, not silently inherited.

## Decisions

Inherited from `../plan.md`: **D50** is the closure record for the reversal of the settled 29 Oct 2026 "leave as-is" (handler half only); **U2 untouched** - no `CheckReturn`/`CheckNonZeroHandle` anywhere; **`wsprintfT` excluded** so `user32.h` is not edited and the 122-source gate is not in play; **U1 stays OPEN** until the whole pass and the dated folder gate are clean; **TU banners never edited** (protocol 7, ~35 banners assert the stale no-handler policy in this folder).

## Small TU batch S1

7 TUs / **10 sites**: `bitmap.cpp` 1, `brush.cpp` 1, `console.cpp` 1, `printing.cpp` 1, `devicemanagement.cpp` 2, `resource.cpp` 2, `security.cpp` 2. The cheapest batch - it is the first to run and the first to prove the inserter against a real TU. Refined into `./SmallTUBatchS1/plan.md`: one Atomic item per TU, each naming its site count.

## Small TU batch S2

7 TUs / **18 sites**: `synchronization.cpp` 2, `timer.cpp` 2, `windowprocedure.cpp` 2, `buttoncontrol.cpp` 3, `combobox.cpp` 3, `coordtransform.cpp` 3, `filledshapes.cpp` 3. Same mechanics and the same per-TU invariant; re-census the folder after the batch before S3 starts. Refined into `./SmallTUBatchS2/plan.md`: one Atomic item per TU, each naming its site count, and the child records the measured per-TU anchor lines so the inserter never anchors on the prose hits at `windowprocedure.cpp:22`/`:65` or `coordtransform.cpp:18`.

## Small TU batch S3

7 TUs / **27 sites**: `listbox.cpp` 3, `accessibility.cpp` 4, `errorhandling.cpp` 4, `hook.cpp` 4, `legacy.cpp` 4, `mdi.cpp` 4, `powermanagement.cpp` 4. `errorhandling.cpp` is the TU whose name suggests the handler class - read its banner before editing so the inserter's anchor is not confused with the banner prose. Refined into `./SmallTUBatchS3/plan.md`: one Atomic item per TU, each naming its site count and measured anchor lines. Two parent assumptions resolved against measurement: `errorhandling.cpp`'s banner never mentions the handler class (`findstr /I errorhandler` returns no match) - what it holds is one of the folder's 4 live `CheckReturn` sites at `:81` plus a bare-statement `Library::voidCall` at `:89`; and `legacy.cpp:65` banner prose claims the TU carries no `qor_pp_fcontext` while it has 4 code anchors. The child also carries the **census-pattern warning** the whole pass needs: `Library::*Call<` must be counted comment-stripped/substring, because anchored patterns read 593 (`^\s*(return\s+)?Library::`) or 589 (`^\s*return Library::`) against the 597 - the gap being the 4 `CheckReturn` sites.

## Small TU batch S4

7 TUs / **33 sites**: `systeminfo.cpp` 4, `text.cpp` 4, `help.cpp` 5, `ime.cpp` 5, `monitor.cpp` 5, `processthread.cpp` 5, `windowproperties.cpp` 5. `windowproperties.cpp`'s 5 sites count toward this batch, not toward the probe TU. Refined into `./SmallTUBatchS4/plan.md`: one Atomic item per TU, each naming its site count and measured anchor lines. Two measured findings the region carries forward: `processthread.cpp` holds **3 of the folder's 4 live `CheckReturn` sites** (`:85`, `:94`, `:110` - the 4th being `errorhandling.cpp:81`), which is where the comment-stripped census rule bites hardest (an anchored pattern reads 2/5 for that TU); and `windowproperties.cpp`'s banner quotes its own anchor line numbers **one line low** against the disk (banner `:81,:88,:95,:102,:109` vs code `80,87,94,101,108`), so no number-driven inserter is safe in this folder.

## Small TU batch S5

5 TUs / **36 sites**: `systemshutdown.cpp` 6, `displayconfig.cpp` 7, `rawinput.cpp` 7, `caret.cpp` 8, `devicecontext.cpp` 8. The last batch of the region: after it, the small region's census must read **124 sites over 33 TUs**, which with the probe's 5 and the large region's 468 gives the pass total 597.
