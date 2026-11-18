---
Type: WikiPage
PageType: module
Title: "qor_interception"
Target: qor_interception
Status: current
LastVerified: 2026-11-16
Claims: 4
Summary: "Function-level interception framework for AOP; the leaf of the flyer set - three links in, one flyer out."
Anchors:
  - { kind: in-tree, path: src/qor/flyers/interception, symbol: qor_interception, verified: 2026-11-16 }
  - { kind: in-tree, path: src/qor/flyers/interception/CMakeLists.txt, symbol: qor_interception, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_interception --parallel 8
---

# qor_interception

General function-level interception framework for AOP; described as powerful and fundamental, with
current code only lightly using it (`docs/agent/purposes.md`).

- Its QOR dependencies are only `qor_module`, `qor_host` and `qor_current`
  (`src/qor/flyers/interception/CMakeLists.txt`), which makes it the leaf of the flyer group: within
  `src/qor/flyers` only `qor_error` links it (`src/qor/flyers/error/CMakeLists.txt`).
- It is eight translation units - `qor_interception.cpp`, `callcontext.cpp`, `functioncontext.cpp`,
  `flystrapbase.cpp`, `flystrap.cpp`, `callinterceptor.cpp`, `functioncontextlock.cpp` and
  `parameters/return.cpp` - the last being a subdirectory path, the only nested source list among the
  flyers (`src/qor/flyers/interception/CMakeLists.txt`).
- It declares its own `project(QOR VERSION 0.5.0 ...)`, so its version defines expand independently of
  the parent scope (`src/qor/flyers/interception/CMakeLists.txt`).

## Failure modes

- The framework is "fundamental" and the usage is light, and the file carries no test target, so a
  green judge here says the interception machinery compiles - nothing in this page's judges exercises
  a strap firing.
