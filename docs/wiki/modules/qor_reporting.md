---
Type: WikiPage
PageType: module
Title: "qor_reporting"
Target: qor_reporting
Status: current
LastVerified: 2026-11-16
Claims: 4
Summary: "Forwarding reporting proxy with a fallback, so low-level libs can report without depending on qor_error."
Anchors:
  - { kind: in-tree, path: src/qor/essentials/reporting, symbol: qor_reporting, verified: 2026-11-16 }
  - { kind: in-tree, path: src/qor/essentials/reporting/CMakeLists.txt, symbol: qor_reporting, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_reporting --parallel 8
---

# qor_reporting

A forwarding reporting proxy with a fallback, so a low-level library can report an error without
depending on `qor_error` (`docs/agent/purposes.md`).

- Its QOR dependencies are only `qor_module` and `qor_host`
  (`src/qor/essentials/reporting/CMakeLists.txt`).
- That short list is the page: the target exists so the pair above can stay sufficient. Linking
  `qor_error` here would hand every low-level caller the dependency this layer was built to avoid
  (`docs/agent/purposes.md`).
- The three forwarding implementations sit beside the umbrella file: `errorreporting.cpp`,
  `assertionfailurereporting.cpp`, `testfailurereporting.cpp`
  (`src/qor/essentials/reporting/CMakeLists.txt`).

## Failure modes

- The buildnumber rule, shared by every target that includes `cmake/updatebuildnumber.cmake`: a tree
  configured without `BUILD_NUMBER` in its cache is red before it is green.
