---
Type: WikiPage
PageType: module
Title: "qor_error"
Target: qor_error
Status: current
LastVerified: 2026-11-16
Claims: 5
Summary: "Raise errors of varying severity and handle them; the widest link line of the four flyers."
Anchors:
  - { kind: in-tree, path: src/qor/flyers/error, symbol: qor_error, verified: 2026-11-16 }
  - { kind: in-tree, path: src/qor/flyers/error/CMakeLists.txt, symbol: qor_error, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_error --parallel 8
---

# qor_error

Raise errors of varying severity and handle them (`docs/agent/purposes.md`).

- Its QOR dependencies are `qor_module`, `qor_host`, `qor_issue`, `qor_text`, `qor_current`,
  `qor_interception` and `qor_reporting` (`src/qor/flyers/error/CMakeLists.txt`) - seven, the widest
  link line among the flyers.
- It is eleven translation units, and the severity ladder is visible in their names: `fatal.cpp`,
  `serious.cpp`, `continuable.cpp`, `warning.cpp`, `note.cpp`, beside `errorbase.cpp`, `clibresult.cpp`,
  `commonerrormessages.cpp`, `defaulterrorhandler.cpp` and `reportingimpl.cpp`
  (`src/qor/flyers/error/CMakeLists.txt`).
- It declares its own `project(QOR VERSION 0.5.0 ...)`, so `PROJECT_VERSION_*` expand here whether or
  not the file is reached through `src/qor/CMakeLists.txt` (`src/qor/flyers/error/CMakeLists.txt`).

## Failure modes

- `docs/agent/purposes.md` says it "Builds on issue/log", but the link line names `qor_issue` and not
  `qor_log`: the dependency runs the other way, `qor_log` links `qor_error`
  (`src/qor/flyers/log/CMakeLists.txt`). Reading purposes.md as a build graph inverts the edge.
- `add_library` spells the target out as `qor_error` where its siblings use `${target}`, so a rename
  has to touch both the `set(target ...)` line and the `add_library` line
  (`src/qor/flyers/error/CMakeLists.txt`).
- The file adds no test and no executable: the build is the only judge this page can carry, so a green
  judge means it compiles and links, not that a handler was exercised.
