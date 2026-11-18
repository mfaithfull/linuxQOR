---
Type: WikiPage
PageType: module
Title: "qor_application"
Target: qor_application
Status: current
LastVerified: 2026-11-16
Claims: 6
Summary: "Application object built from Roles, Features, Options and Workflows; the widest dependant in the app layer."
Anchors:
  - { kind: in-tree, path: src/framework/app/application, symbol: qor_application, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/app/application/CMakeLists.txt, symbol: qor_application, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/app/application/application.cpp, symbol: Application, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_application --parallel 8
---

# qor_application

The application object: "Application thing (Features/Roles/Options/Workflows)"
(`docs/agent/purposes.md`). Built SHARED from three translation units - `qor_application.cpp`,
`application.cpp`, `builder.cpp` (`src/framework/app/application/CMakeLists.txt`).

- Its five QOR dependencies are `qor_module qor_thread qor_sync qor_platform qor_role`
  (`src/framework/app/application/CMakeLists.txt`), which is exactly what its
  `docs/agent/repo_map.md` row lists - the index is true of this lib.
- Version 0.5.0 and the gate token `QOR_APPLICATION` are stamped by the same file; the token only has
  to be non-empty to select `__declspec(dllexport)` (`src/configuration/configuration.h`).
- It is the app layer's widest dependant: 65 `CMakeLists.txt` files outside its own directory name it,
  second in this slice only to `qor_host` (`src/framework/app/host/CMakeLists.txt`).
- `QORTest` both links it and compiles a test for it: `application/test_application.cpp` is in the
  source list and `qor_application` is in the link list (`test/CMakeLists.txt`).
- The class a caller derives against is `Application` in `application.h`; `application.cpp` carries 24
  mentions of the name (`src/framework/app/application/application.cpp#Application`).

## Failure modes

- The purpose line says "Aggregation header app.h: include before deriving/using qor::Application",
  and there is no `app.h`: 0 files of that name exist under `src` or `include`, and no source in the
  app layer includes it. The instruction cannot be followed as written
  (`src/framework/app/application/CMakeLists.txt`).

## See also

[[qor_role]] [[qor_platform]] [[qor_module]]
