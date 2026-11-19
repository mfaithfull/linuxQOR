---
Type: WikiPage
PageType: module
Title: "qor_application"
Target: qor_application
Status: current
LastVerified: 2026-11-19
Claims: 10
Summary: "Application object built from Roles, Features, Options and Workflows; the widest dependant in the app layer."
Sources:
  - hook-25c080074a-qor_application
Anchors:
  - { kind: in-tree, path: src/framework/app/application, symbol: qor_application, verified: 2026-11-19 }
  - { kind: in-tree, path: src/framework/app/application/CMakeLists.txt, symbol: qor_application, verified: 2026-11-19 }
  - { kind: in-tree, path: src/framework/app/application/application.cpp, symbol: Application, verified: 2026-11-16 }
  - { kind: in-tree, path: sdk/app.h, symbol: builder.h, verified: 2026-11-18 }
  - { kind: in-tree, path: src/framework/app/application/application.h, symbol: qor_pp_redirect_app_class, verified: 2026-11-18 }
  - { kind: in-tree, path: src/framework/app/application/builder.cpp, symbol: AppBuilder::TheApplication, verified: 2026-11-18 }
  - { kind: in-tree, path: src/framework/app/role/getfeature.h, symbol: GetFeature, verified: 2026-11-18 }
  - { kind: in-tree, path: src/framework/app/application/irunable.h, symbol: make_runable, verified: 2026-11-19 }
  - { kind: in-tree, path: src/framework/app/application/irunable.h, anchor: commit:25c080074a4fc59cb06100c76fb8be5255907953, verified: 2026-11-19 }
  - { kind: in-tree, path: src/framework/app/application/purpose.md, symbol: AppBuilder, verified: 2026-11-19 }
Judges:
  - cmake --build cmakebuild --target qor_application --parallel 8
---

# qor_application

The application object: "Application thing (Features/Roles/Options/Workflows)"
(`docs/agent/purposes.md`). The folder has stated it itself since commit `25c080074a`:
"Application is a natural singleton, one per process" and "always constructed via an AppBuilder"
(`src/framework/app/application/purpose.md`) - the same commit wrote the `irunable.h` contract as
comments ("Abstract interface intended to be included and implemented by main loop implementors",
the `make_runable` wrapper and its factory, `src/framework/app/application/irunable.h`). The
aggregation header for using it is `sdk/app.h` (`sdk/purpose.md`); it
pulls `builder.h`, which pulls `application.h` (`src/framework/app/application/builder.h`). Built
SHARED from three translation units - `qor_application.cpp`, `application.cpp`, `builder.cpp`
(`src/framework/app/application/CMakeLists.txt`).

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

- **The redirect to a derived Application is opt-in boilerplate, and its absence fails silently.**
  `new_ref<Application>()` routes through the `SingletonInstancer` holder
  (`src/qor/memory/reference/newref.h`, `src/qor/memory/instance/singleton.h`, `application.h`), so
  `AppBuilder().TheApplication()` (`src/framework/app/application/builder.cpp`) yields the empty base
  singleton - not the app the caller built - unless the derived class registered a redirect, either
  the macro pair `qor_pp_declare_app_class` + `qor_pp_redirect_app_class` (`application.h`) or the
  templated `Build` that calls `AutoRedirect` (`src/framework/app/application/builder.h`). An app
  built without either still runs its own `Run`/`RunWorkflow` correctly; the damage appears only in
  QOR-internal lookups, because the base's `GetRole()` returns the null `m_Role` with no guard
  (`src/framework/app/application/application.cpp`), and the framework's own convenience chains
  dereference it unchecked: `GetFeature` in `src/framework/app/role/getfeature.h` does
  `TheApplication(...)->GetRole(...)->GetFeature<...>()`, and `qor_netserver`'s session enters through
  exactly that chain (`src/components/io/network/server/netsession.h`). The examples state the
  contract as prose instead: "Required boiler plate for derived Application classes"
  (`examples/advanced/httpserver/httpserverapp.h`), with the reasoning spelled out at
  `examples/basic/options/optionsapp.cpp`.

## See also

[[qor_role]] [[qor_platform]] [[qor_module]]
