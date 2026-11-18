---
Type: WikiPage
PageType: module
Title: "qor_log"
Target: qor_log
Status: current
LastVerified: 2026-11-16
Claims: 5
Summary: "Log raising and default handlers; the top of the flyer stack, and it reaches a framework lib."
Anchors:
  - { kind: in-tree, path: src/qor/flyers/log, symbol: qor_log, verified: 2026-11-16 }
  - { kind: in-tree, path: src/qor/flyers/log/CMakeLists.txt, symbol: qor_log, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_log --parallel 8
---

# qor_log

Log raising plus the default log handlers; a log carries a level and the handlers filter by it
(`docs/agent/purposes.md`).

- The level ladder is one translation unit per level: `debug.cpp`, `important.cpp`, `impactful.cpp`,
  `informative.cpp`, `imperative.cpp`, beside `qor_log.cpp`, `log.cpp`, `defaultloghandler.cpp` and
  `levelwhat.cpp` - nine in all (`src/qor/flyers/log/CMakeLists.txt`).
- Its QOR dependencies are `qor_module`, `qor_host`, `qor_current`, `qor_interception`, `qor_issue`,
  `qor_error` and `qor_signals` (`src/qor/flyers/log/CMakeLists.txt`), so it sits above every other
  flyer.
- It declares its own `project(QOR VERSION 0.5.0 ...)`, so its version defines expand independently of
  the parent scope (`src/qor/flyers/log/CMakeLists.txt`).

## Failure modes

- One of its links leaves the core: `qor_signals` is declared at
  `src/framework/event/signals/CMakeLists.txt`, so a core flyer is downstream of a framework target,
  and building `qor_log` pulls the framework in.
- The edge to `qor_error` runs log -> error, not error -> log, contrary to the "Builds on issue/log"
  line in `docs/agent/purposes.md`; `qor_error` does not link `qor_log`
  (`src/qor/flyers/error/CMakeLists.txt`).
- `add_library` spells the target out as `qor_log` instead of using `${target}`, so a rename has to
  touch two lines (`src/qor/flyers/log/CMakeLists.txt`).
