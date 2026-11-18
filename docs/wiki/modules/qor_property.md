---
Type: WikiPage
PageType: module
Title: "qor_property"
Target: qor_property
Status: current
LastVerified: 2026-11-16
Claims: 5
Summary: "Generic properties and collections, convertible to and from structs by reflection; experimental."
Anchors:
  - { kind: in-tree, path: src/qor/property, symbol: qor_property, verified: 2026-11-16 }
  - { kind: in-tree, path: src/qor/property/CMakeLists.txt, symbol: qor_property, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_property --parallel 8
---

# qor_property

Generic properties and collections, convertible to and from structs by reflection, variant-based and
JSON-convertible - and marked Experimental (`docs/agent/purposes.md`).

- One translation unit, `qor_property.cpp`, built SHARED (`src/qor/property/CMakeLists.txt`) - the
  whole variant-based property map, property vector and property value in a single object file.
- Its QOR dependencies are `qor_module`, `qor_host`, `qor_reporting` and `qor_current`
  (`src/qor/property/CMakeLists.txt`); the last two are `src/qor/essentials/`, so it is not a leaf
  even though almost nothing links it.
- `QOR_PROPERTY` gates 7 matches over `src` and `include` on 16 Nov 2026, so the library is compiled
  into more code than the build graph admits.

## Failure modes

- The test exists and does not run: `property/test_property.cpp` is commented out of `QORTest`'s
  source list, and `##qor_property` is commented out of **both** platform link lines
  (`test/CMakeLists.txt`). No binary in the tree links it as a QOR library, so the build is the only
  judge this page can carry - a green judge here proves it compiles, nothing more.
- What does link it is examples, and only experimental ones: `canvastest`, `egltest`, `microuitest`,
  `openglestest`, `tuitest` (`examples/experimental/canvastest/CMakeLists.txt`). The word
  "Experimental" in `docs/agent/purposes.md` is therefore enforced by the build graph, and a change
  that breaks it breaks no CI test - only those five example builds.
