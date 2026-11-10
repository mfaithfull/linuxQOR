---
Type: Plan
Title: "Batch the large TUs"
Parent: "../plan.md"
Level: 1
Items:
  - Title: Large TU batch L1
    Status: Refined
    Content: ./LargeTUBatchL1/plan.md
  - Title: Large TU batch L2
    Status: Refined
    Content: ./LargeTUBatchL2/plan.md
  - Title: Large TU batch L3
    Status: Refined
    Content: ./LargeTUBatchL3/plan.md
  - Title: Insert handlers in window.cpp
    Status: Atomic
    Content:
---

# Batch the large TUs

Slug: `BatchLargeTUs` - from title "Batch the large TUs" ("the" dropped, PascalCased, ASCII, 14 chars, inside the 24-char cap). No sibling collision: the sibling is `BatchSmallTUs`. Uniqueness is stored here, not recomputed.

## Goal

Give the **19 largest wrapper TUs** of `src/platform/os/windows/api_layer/user` their call-site `Win32ErrorHandler _;` - **468 sites** plus one include line per TU - so that with the probe TU (5) and the settled small region (124) the whole folder reads **597 handlers over 53 TUs**.

## Context

Inherited from `../plan.md` (the U1 pass; source narrative `docs/future/winqor_api_user_U1_plan.md`, superseded provenance) and from the settled sibling `../BatchSmallTUs/plan.md` (33 TUs / 124 sites). Every number below was **re-measured this pass** against the disk, not inherited.

- Folder: **54 sources**, 53 carry wrappers, `winqor_api_user.cpp` is the module marker (**0** sites). Pass total **597 sites / 53 TUs = 597 statement insertions + 53 include lines = 650 changes**. Split: probe `accelerator.cpp` 5 + small region 33 TUs / 124 + **this region 19 TUs / 468** = 597.
- **Starting state measured this session:** over the folder the code-site count of `Win32ErrorHandler _;` is **0** and the count of TUs including `errorhandler.h` is **0** - nothing in this region is half-edited, so the region starts from zero.
- **Per-file census measured this session** (comment-stripped code anchors, each equal to that TU's `qor_pp_useswinapi*` and `Library::*Call<` count): mouse 10, rectangle 11, scrollbar 12, icon 14, touch 14, windowclass 14, cursor 15 | desktop 18, clipboard 20, dpi 21, dialog 22, misc 23, message 24, painting 25 | pointer 32, keyboard 34, strings 34, menu 39 | window 86 = **19 TUs / 468 sites**. It agrees exactly with the parent's 53-row table and with `user32.h`'s standing 597-binding census.
- **Idiom, copied from `kernel`/`shell`, not invented:** include **`#include "../kernel/errorhandler.h"`** in the **generic** block - immediately after `#include "src/qor/flyers/error/error.h"` (measured at `window.cpp:7`) and **before** the `//Windows specific headers must be last ...` marker (measured at `window.cpp:9`), because `errorhandler.h` is a generic QOR header that pulls `src/qor/flyers/error/defaulterrorhandler.h` at its line 28. Add it **only if absent**, so a re-run is a no-op. Statement **`Win32ErrorHandler _;` immediately after `qor_pp_fcontext;`, before the `qor_pp_useswinapi*` line**, at 8-space indentation (target shape `accelerator.cpp:22-27`).
- **The anchor trap lives in this region.** Raw substring hits for `qor_pp_fcontext;` over the folder read **598** against **597** code anchors, and the single prose hit that carries the semicolon is **`painting.cpp:74`** ("`//Call-site idiom: 25/25 carry qor_pp_fcontext;` ..."). Four more comment lines name the token **without** the semicolon: `dpi.cpp:54`, `pointer.cpp:37`, `rectangle.cpp:90`, `touch.cpp:75`. Anchor only on lines whose **first non-whitespace token** on a **comment-stripped** line is `qor_pp_fcontext;`; never a substring match.
- **Census patterns measured for this region:** all 468 sites match `Library::\w*Call<`, and **this region holds 0 `CheckReturn` code sites** - the folder's 4 live ones all sit in the small region (`errorhandling.cpp:81`, and `processthread.cpp:85`/`:94`/`:110` per the settled S4 child; a raw grep for `CheckReturn<` over the folder reads **9** lines, the other 5 - `buttoncontrol.cpp:57`, `devicecontext.cpp:28`, `processthread.cpp:54`/`:55`, `synchronization.cpp:73` - being banner prose). What does bite here is `Library::voidCall` as a bare statement: **6 sites, one each in `keyboard.cpp`, `message.cpp`, `misc.cpp`, `mouse.cpp`, `pointer.cpp`, `window.cpp`**, so an anchored `^\s*return Library::` pattern under-reads this region by 6 (the S3 child's census-pattern warning, confirmed here).
- **Banner numbers are stale; never drive an inserter from them and never edit them (protocol 7):** `window.cpp:16` claims "**85 wrappers**" and `window.cpp:25` claims "75 `qor_pp_useswinapi` + 10 `qor_pp_useswinapiAW`" = 85 bindings, while the disk census reads **86** anchors / **86** bindings. Banners that happen to agree (`dpi.cpp:10` 21, `painting.cpp:14` twenty-five, `pointer.cpp:5` 32, `rectangle.cpp:15` eleven, `menu.cpp:14` thirty-nine, `keyboard.cpp:15` 34, `touch.cpp:10` 14, `clipboard.cpp:15` twenty) are coincidence, not licence.
- **Per-file mechanics (identical to the small region):** snapshot the file first, then `if($null -eq $t) throw` and `if($out.Length -lt 1) throw` immediately before any write (the `record-editing.md` truncation trap); re-measure after each file; keep sources **pure ASCII**; indentation exact.
- **Re-census invariant after every batch:** per-TU `qor_pp_fcontext; == qor_pp_useswinapi* == Library::*Call< == Win32ErrorHandler` at that TU's count, and running folder totals **597 / 597 / 597 / 597** with 53 TUs carrying the include. A batch that re-sums differently means the inserter missed or double-hit a site - **stop and re-measure, do not proceed**.
- **Tick `registers/u1.md`'s working record for a TU the moment it verifies**, while U1 stays OPEN (the U17 lesson: deferring ticks to the close forced a whole pass to be re-derived).
- **No gate runs per TU or per batch here.** The only standalone-gate leg is the probe TU (`accelerator.cpp`, a sibling of the parent plan); the folder gate (**54/54 exit 0**, `/Fo` **attached** to the switch, dated by `cmd /c dir /T W`, never pwsh) is a sibling item of the root.

Sibling items of the parent plan: `Probe the accelerator TU` (settled, 5 sites), `Batch the small TUs` (settled, 33 TUs / 124 sites), `Gate the folder`, `Write the records`, `Commit the pass`, `All-libraries gate` (Deferred).

**Instruments for the numbers above** (`tmp/` is scratch and uncommitted - the names are provenance, not a promise the files survive): `tmp/_tmp_u1_census.ps1` -> `tmp/_tmp_u1_census.txt` (per-TU comment-stripped census: anchors / handlers / bindings / `Library::*Call<` / includes), `tmp/_tmp_u1_anchor_probe.ps1` -> `tmp/_tmp_u1_anchor_probe.txt` (raw vs code anchor hits and the semicolon-bearing prose lines), `tmp/_tmp_u1_census.ps1` -> `tmp/_tmp_u1_large_traps.txt` (per-TU prose mentions, `CheckReturn`, `voidCall`, line counts), `tmp/_tmp_u1_prose.ps1` -> `tmp/_tmp_u1_prose_lines.txt` (banner prose incl. the stale wrapper tallies), `tmp/_tmp_u1_tree_verify.ps1` -> `tmp/_tmp_u1_tree_verify.txt` (tree invariants: Content paths, Parent resolution, Level = parent + 1, <=7 items, <=1 Drafted).
## Decisions

Inherited from `../plan.md`, linked not restated: **D50** is the closure record for reversing the settled 29 Oct 2026 "leave as-is" for the **handler half only** (`docs/agent/DECISIONS.md` index line + `docs/agent/decisions/D50.md`; that index currently ends at **D49**, so D50 is the next free id - re-measure before writing); **U2 stays settled and untouched** - no `CheckReturn`/`CheckNonZeroHandle` is introduced anywhere, and the folder's 4 existing `CheckReturn` sites (all in the small region) keep their semantics; **`wsprintfT` is excluded**, so `user32.h` and every shared header stay untouched and the 122-source all-libraries gate is **not** in play; **U1 stays OPEN** until all 53 TUs / 597 sites carry the handler and the dated folder gate is clean; **TU banners are never edited** (protocol 7); **nothing is appended to frozen or closed records** (`tooling.md` is frozen at session 34); **no project-wide `UNICODE`/`WINVER`/`_WIN32_WINNT` pin and no `NO_STRICT` change** (the D21/D46 residuals stay).

**Re-cut for the 7-item breadth cap** (measured and re-summed here, not silently inherited from the source narrative's B3-B6): L1 7 TUs / 90 sites, L2 7 TUs / 153, L3 4 TUs / 139, `window.cpp` alone 1 TU / 86 = **19 TUs / 468 sites**. Ascending order, so a cheap failure surfaces before the expensive TUs are touched.

## Large TU batch L1

7 TUs / **90 sites** - the 10-15 site band: `mouse.cpp` 10, `rectangle.cpp` 11, `scrollbar.cpp` 12, `icon.cpp` 14, `touch.cpp` 14, `windowclass.cpp` 14, `cursor.cpp` 15. Cheapest batch of the region; it is the first to prove the inserter against a multi-site TU of this scale. `rectangle.cpp:90` and `touch.cpp:75` carry comment mentions of `qor_pp_fcontext` (without the semicolon) - anchor rule applies. Refined into `./LargeTUBatchL1/plan.md`: one Atomic item per TU, each naming its measured site count.

## Large TU batch L2

7 TUs / **153 sites** - the 18-25 site band: `desktop.cpp` 18, `clipboard.cpp` 20, `dpi.cpp` 21, `dialog.cpp` 22, `misc.cpp` 23, `message.cpp` 24, `painting.cpp` 25. This batch holds **the folder's only semicolon-bearing prose anchor, `painting.cpp:74`** - the inserter must be run there with the comment-stripped first-token rule and re-measured to **25**, not 26. `dpi.cpp:54` is a second comment mention. `misc.cpp` and `message.cpp` each hold one `Library::voidCall` bare statement. Refined into `./LargeTUBatchL2/plan.md`: one Atomic item per TU, each naming its measured site count.

## Large TU batch L3

4 TUs / **139 sites** - the 32-39 site band: `pointer.cpp` 32, `keyboard.cpp` 34, `strings.cpp` 34, `menu.cpp` 39. `pointer.cpp:37` is a comment mention of the anchor token; `pointer.cpp` and `keyboard.cpp` each hold one `Library::voidCall`. `menu.cpp`'s banner (`:53`, `:83`) quotes call-site and BOOL-return tallies that are not anchors - read it, never edit it. Refined into `./LargeTUBatchL3/plan.md`: one Atomic item per TU, each naming its measured site count.

## Insert handlers in window.cpp

Atomic: one file, one scripted pass, one re-measure. `window.cpp` - **86 sites** (measured; the largest TU in the folder at 695 lines), plus its include line. After the pass its census must read `86 / 86 / 86 / 86` and it must carry `#include "../kernel/errorhandler.h"` once. It holds one `Library::voidCall` bare statement, and its frozen banner (`:16` "85 wrappers", `:25` "75 + 10" bindings) is **one below** the disk census - so the count driving the inserter is the measured 86, never the banner's 85. No gate runs here; the folder gate is a sibling item of the root.

