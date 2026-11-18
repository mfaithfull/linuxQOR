---
Type: WikiPage
PageType: module
Title: "qor_issue"
Target: qor_issue
Status: current
LastVerified: 2026-11-16
Claims: 4
Summary: "Issue base classes and infrastructure; the where/when/what record both other flyers depend on."
Anchors:
  - { kind: in-tree, path: src/qor/flyers/issue, symbol: qor_issue, verified: 2026-11-16 }
  - { kind: in-tree, path: src/qor/flyers/issue/CMakeLists.txt, symbol: qor_issue, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_issue --parallel 8
---

# qor_issue

Extensible base classes and infrastructure for issues: each issue records where and when, and the
`What` is derived-type determined (`docs/agent/purposes.md`).

- It is four translation units, and three of them are the record's three parts: `qor_issue.cpp`,
  `where.cpp`, `when.cpp`, `what.cpp` (`src/qor/flyers/issue/CMakeLists.txt`).
- Its QOR dependencies are only `qor_module`, `qor_host` and `qor_current`
  (`src/qor/flyers/issue/CMakeLists.txt`) - deliberately not `qor_error`, which links it
  (`src/qor/flyers/error/CMakeLists.txt`).
- Both other flyers depend on it: `qor_error` and `qor_log` each carry `qor_issue` on their link line
  (`src/qor/flyers/error/CMakeLists.txt`, `src/qor/flyers/log/CMakeLists.txt`).

## Failure modes

- It declares its own `project(QOR VERSION 0.5.0 ...)` and adds no test target, so its judge is a
  compile-and-link check on the base classes alone; which derived `What` types exist is a source fact
  this page's anchors cannot see.
