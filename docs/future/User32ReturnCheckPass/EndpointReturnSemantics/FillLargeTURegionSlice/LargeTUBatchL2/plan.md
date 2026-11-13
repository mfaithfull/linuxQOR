---
Type: Plan
Title: "Large TU batch L2"
Parent: "../plan.md"
Level: 3
Items:
  - Title: desktop rows
    Status: Refined
    Content: ./DesktopRows/plan.md
  - Title: clipboard rows
    Status: Refined
    Content: ./ClipboardRows/plan.md
  - Title: dpi rows
    Status: Refined
    Content: ./DpiRows/plan.md
  - Title: dialog rows
    Status: Refined
    Content: ./DialogRows/plan.md
  - Title: misc rows
    Status: Refined
    Content: ./MiscRows/plan.md
  - Title: message rows
    Status: Refined
    Content: ./MessageRows/plan.md
  - Title: painting rows
    Status: Refined
    Content: ./PaintingRows/plan.md
---

# Large TU batch L2

Slug: `LargeTUBatchL2` - from title "Large TU batch L2" (no drop-words, PascalCased, 14 chars, ASCII); the sibling U1 tree's name. Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **153** table rows of these 7 large TUs, every item one fetch batch of at most 10 URLs.

## Resume

Cursor: **terminal** - all 7 items `Refined` with children on disk, all 7 closed 12 Nov 2026. Item 7 **painting rows** is **CLOSED 12 Nov 2026** - `./PaintingRows/plan.md` terminal at 3 closed `Atomic` items over 3 fetch batches of 10/10/5 URLs, all **25** rows of `painting.cpp` carrying the triple (`checked 15 / unchecked 10 / void 0`, `tmp/_tmp_u2_tu_state.ps1 painting.cpp` `rows=25 checked=15 unchecked=10 void=0 blank=0 shape_bad=0`, the `ValidateRect`/`ValidateRgn`/`InvalidateRect` trio measured **checked** against the leaf's prediction, `EndPaint`/`InvalidateRgn` unchecked on "always nonzero", `BeginPaint`/`GetWindowDC` checked `CheckNonZeroHandle< HDC >`). Item 6 **message rows** is **CLOSED 12 Nov 2026** - `./MessageRows/plan.md` terminal at 3 closed `Atomic` items over 3 fetch batches of 10/10/4 URLs, all **24** rows of `message.cpp` carrying the triple (`checked 10 / unchecked 13 / void 1`, `tmp/_tmp_u2_tu_state.ps1 message.cpp` `rows=24 checked=10 unchecked=13 void=1 blank=0 shape_bad=0`; 8 of 24 pages reached via the suffixed ANSI-twin slug, `PostQuitMessage` the `void` row, `SendMessageTimeout` checked / `SendMessage` unchecked on opposite page readings). Item 5 **misc rows** is **CLOSED 12 Nov 2026** - `./MiscRows/plan.md` is terminal at 3 closed `Atomic` items over 3 fetch batches of 10/10/3 URLs, and all **23** rows of `misc.cpp` carry the verdict triple (`checked 13 / unchecked 9 / void 1`, `tmp/_tmp_u2_tu_state.ps1 misc.cpp` `rows=23 checked=13 unchecked=9 void=1 blank=0 shape_bad=0`, the batch's only `void` site being `SetDebugErrorLevel`, and `ChangeMenu` / `GetClipboardMetadata` / `SetDebugErrorLevel` the three rows whose pages carry no Return value section; the folder single-pass audit `tmp/_tmp_u2_folder_audit.ps1` reads `rows=597 filled=318 checked=195 unchecked=118 void=5 class-shape violations=0`). Item 4 **dialog rows** is **CLOSED 12 Nov 2026** - `./DialogRows/plan.md` is terminal at 3 closed `Atomic` items over 3 fetch batches of 10/10/2 URLs, and all **22** rows of `dialog.cpp` carry the verdict triple (`checked 13 / unchecked 9 / void 0`, `tmp/_tmp_u2_tu_state.ps1 dialog.cpp` `rows=22 checked=13 unchecked=9 void=0 blank=0 shape_bad=0`, `MessageBoxTimeout` the only row here with no Learn page). Item 3 **dpi rows** is **CLOSED 12 Nov 2026** at all 21 of its rows (`checked 6 / unchecked 15 / void 0`, `tmp/_tmp_u2_tu_state.ps1 dpi.cpp` `rows=21 checked=6 unchecked=15 void=0 blank=0 shape_bad=0`, `tmp/_tmp_u2_filled.txt` `dpi.cpp rows=21 filled=21 full`; `./DpiRows/plan.md` terminal at 3 closed `Atomic` items, code-side gap=1 pending at the `DpiSites` leaf). Item 1 **desktop rows** is **CLOSED 12 Nov 2026** - `./DesktopRows/plan.md` is terminal at 2 closed `Atomic` items and all **18** rows of `desktop.cpp` carry the verdict triple (`checked 15 / unchecked 3 / void 0`, `tmp/_tmp_u2_filled.txt` `desktop.cpp rows=18 filled=18 full`). Item 2 **clipboard rows** is **CLOSED 12 Nov 2026** - `./ClipboardRows/plan.md` is terminal at 2 closed `Atomic` items over 2 fetch batches of 10 URLs, and all **20** rows of `clipboard.cpp` carry the verdict triple (`checked 10 / unchecked 10 / void 0`, `tmp/_tmp_u2_tu_state.ps1 clipboard.cpp` `rows=20 checked=10 unchecked=10 void=0 blank=0 shape_bad=0`, `tmp/_tmp_u2_filled.txt` `clipboard.cpp rows=20 filled=20 full`; the TU's code census `CheckReturn-code=10 table-checked=10 table-unchecked=10 returncheck-includes=1 ok` and the folder gate at `srcs=54 not-in-log=0 stale=0 real-diag-lines=0`). All 7 items closed; this plan is terminal. Terminal: all 7 items `Refined` with children on disk.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the grandparent's (`../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: Large TU batch L1, Large TU batch L3.

Measured by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt`: desktop 18 (2 slices), clipboard 20 (2), dpi 21 (3), dialog 22 (3), misc 23 (3), message 24 (3), painting 25 (3) = 153 rows / 19 fetch slices. Every TU here is over the 10-row cap, so each gets a child plan.

## desktop rows

18 rows: `OpenDesktop`..`EnumDesktopWindows` in table order, 2 slices of 10 and 8. Child plan: `./DesktopRows/plan.md` (2 `Atomic` items, terminal).

## clipboard rows

20 rows: `AddClipboardFormatListener`..`SetClipboardViewer` in table order, 2 slices of 10 and 10. Child plan: `./ClipboardRows/plan.md` (2 `Atomic` items, terminal). `GetClipboardData` returns NULL on failure where NULL is also a legal "no data" answer - that row needs the page's exact wording.

## dpi rows

21 rows in 3 slices (10/10/1). Child plan: `./DpiRows/plan.md` (3 `Atomic` items, terminal). Many rows here return a `DPI_AWARENESS_CONTEXT*` handle whose failure value is NULL - and `IsValidDpiAwarenessContext` answers BOOL legitimately.

## dialog rows

22 rows in 3 slices (10/10/2). Child plan: `./DialogRows/plan.md` (3 `Atomic` items, terminal). `GetDlgCtrlID` (8 in-tree callers, the heaviest nonzero row in the tree) lives here - its row must carry the caller count in `notes` before any code lands.

## misc rows

23 rows in 3 slices (10/10/3). Child plan: `./MiscRows/plan.md` (3 `Atomic` items, terminal).

## message rows

24 rows in 3 slices (10/10/4). Child plan: `./MessageRows/plan.md` (3 `Atomic` items, terminal). `PostQuitMessage`, `SendMessageTimeout` and the answer-`BOOL` rows (`InSendMessage`, `GetInputState`, `GetQueueStatus`) are the expected `unchecked` set.

## painting rows

25 rows in 3 slices (10/10/5). Child plan: `./PaintingRows/plan.md` (3 `Atomic` items, terminal).


## Cursors

desktop rows: {"done":true,"closed":"2026-11-12"}
clipboard rows: {"done":true,"closed":"2026-11-12"}
dpi rows: {"done":true,"closed":"2026-11-12"}
dialog rows: {"tu":"dialog.cpp","slice_start":22,"slice_len":0,"done":true,"closed":"2026-11-12"}
misc rows: {"tu":"misc.cpp","slice_start":23,"slice_len":0,"done":true,"closed":"2026-11-12"}
message rows: {"tu":"message.cpp","slice_start":0,"slice_len":10,"done":false}
painting rows: {"tu":"painting.cpp","slice_start":0,"slice_len":10,"done":false}
