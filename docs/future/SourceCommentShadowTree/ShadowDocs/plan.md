---
Type: Plan
Title: "Shadow docs"
Parent: "../plan.md"
Level: 1
Items:
  - Title: window cpp shadow doc
    Status: Atomic
    Content:
  - Title: large answer-shaped cpp shadow docs
    Status: Atomic
    Content:
  - Title: mid-size cpp shadow docs
    Status: Atomic
    Content:
  - Title: remaining cpp shadow docs
    Status: Atomic
    Content:
  - Title: user32 header shadow doc
    Status: Atomic
    Content:
---

# Shadow docs

Slug: `ShadowDocs` - from title "Shadow docs" (no drop-words, PascalCased, 10 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Have one shadow doc per source file in `src/platform/os/windows/api_layer/user` - **55 files** (54 `.cpp` + `user32.h`) - each carrying the per-function decision and evidence that the deleted comment runs held, so the sources can be swept with nothing lost.

## Resume

Cursor: item 1 **window cpp shadow doc** (`Atomic`) - this plan's first action. Terminal: all 5 items `Atomic`.

## Context

Inherited from the parent (`../plan.md`) and its locked decisions: the shadow doc is found by path convention (`docs/src/<source-path>/<basename-with-underscored-extension>.md`), carries no back-pointer from the source, keys entries by qualified name (`qor::win::api::<Class>::<Function>` - measured module opening `namespace qor { namespace win { namespace api {`), and holds documentation rather than a dated work log. No `tmp/` refs, no caller-audit counts, no line counts, no plan-tree refs inside a shadow doc.

Per-file run counts measured 12 Nov 2026 over `src/platform/os/windows/api_layer/user` (574 runs in the 54 `.cpp` files, 5 in `user32.h`): the 12 heaviest files hold 342 runs and the other 42 hold 232. Sibling titles in the parent: Pin the comment and shadow-doc rules, Extract the record before deleting, Delete the runs from the sources, Folder gate after the sweep, Records and commit.

## window cpp shadow doc

`docs/src/platform/os/windows/api_layer/user/window_cpp.md` from `window.cpp` - **51 runs**, the heaviest file and the one whose sweep the U2 pass closed at 55 BOOL + 17 HWND + 14 other rows. One doc, entries keyed by qualified name.

## large answer-shaped cpp shadow docs

`strings_cpp.md` (35), `keyboard_cpp.md` (34), `menu_cpp.md` (32), `pointer_cpp.md` (32) - **133 runs** over 4 docs. These are the L3 batch TUs of `User32ReturnCheckPass`.

## mid-size cpp shadow docs

`painting_cpp.md` (26), `message_cpp.md` (24), `misc_cpp.md` (23), `dialog_cpp.md` (23), `dpi_cpp.md` (22), `clipboard_cpp.md` (21), `desktop_cpp.md` (19) - **158 runs** over 7 docs, the L2 batch TUs.

## remaining cpp shadow docs

The other **42 `.cpp` files, 232 runs** (the small-TU batch: `accelerator`, `process*`, `rawinput`, `mouse`, `icon`, `touch`, `synchronization`, `errorhandling`, `systemshutdown`, ...). Doc names and per-file counts come from the sweep's fragment set, not from a hand list - the sweep is the source of truth for which files exist.

## user32 header shadow doc

`user32_h.md` from `src/platform/os/windows/api_layer/user/user32.h` - **5 runs**. The only header in the module, so the extension-underscore rule is exercised once here (`user32.h` -> `user32_h.md`) and the sweep must assert the 55 doc names are distinct.
