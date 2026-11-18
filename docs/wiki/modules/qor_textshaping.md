---
Type: WikiPage
PageType: module
Title: "qor_textshaping"
Target: qor_textshaping
Status: current
LastVerified: 2026-11-16
Claims: 4
Summary: "Text layout and shaping for display; declared but not configured, so its judge cannot go green."
Anchors:
  - { kind: in-tree, path: src/qor/essentials/text/shaping, symbol: qor_textshaping, verified: 2026-11-16 }
  - { kind: in-tree, path: src/qor/essentials/text/shaping/CMakeLists.txt, symbol: qor_textshaping, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_textshaping --parallel 8
---

# qor_textshaping

Text layout and shaping for display, part of the text subsystem: UTF to UCS4 code points for glyph
lookup (`docs/agent/purposes.md`).

- The target is declared (`src/qor/essentials/text/shaping/CMakeLists.txt`) and its sources are on
  disk, so the declaration in `docs/agent/repo_map.md` is true of the source tree.
- It is **not configured**: `add_subdirectory(shaping)` is commented out in the parent, and a build
  of it fails with MSB1009, "Project file does not exist", because no project file was ever
  generated for it (`src/qor/essentials/text/CMakeLists.txt`).
- Its version defines come from the parent scope - the file declares no `project()` of its own - so
  it expands correctly only as a subdirectory of `src/qor/essentials/text`
  (`src/qor/essentials/text/shaping/CMakeLists.txt`).

## Failure modes

- The judge on this page cannot go green while the subdirectory stays commented out. `wiki-reconcile
  verify -RunJudges` prints it RED, and that red is the tree's fact, not the page's.
