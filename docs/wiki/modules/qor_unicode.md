---
Type: WikiPage
PageType: module
Title: "qor_unicode"
Target: qor_unicode
Status: current
LastVerified: 2026-11-16
Claims: 4
Summary: "Unicode and UTF handling inside the text subsystem; the one text subdirectory that is configured."
Anchors:
  - { kind: in-tree, path: src/qor/essentials/text/unicode, symbol: qor_unicode, verified: 2026-11-16 }
  - { kind: in-tree, path: src/qor/essentials/text/unicode/CMakeLists.txt, symbol: qor_unicode, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_unicode --parallel 8
---

# qor_unicode

Unicode and UTF handling within the text subsystem (`docs/agent/purposes.md`).

- Its QOR dependencies are `qor_module`, `qor_host`, `qor_reporting` and `qor_current`
  (`src/qor/essentials/text/unicode/CMakeLists.txt`).
- It is two translation units: the umbrella `qor_unicode.cpp` and `ucdfunctions.cpp`, the character
  data functions (`src/qor/essentials/text/unicode/CMakeLists.txt`).
- It is reached through the parent: `add_subdirectory(unicode)` is active where its sibling
  `shaping` is commented out (`src/qor/essentials/text/CMakeLists.txt`).

## Failure modes

- The file declares no `project()`, so `PROJECT_VERSION_*` come from `src/qor/essentials/text`;
  configuring this directory on its own expands the version defines to nothing
  (`src/qor/essentials/text/unicode/CMakeLists.txt`).
