---
Type: WikiPage
PageType: module
Title: "qor_uiselector"
Target: qor_uiselector
Status: current
LastVerified: 2026-11-17
Claims: 10
Summary: "111 lines, five exported symbols, and no target in the tree links it; the two verbs restate an empty base default and the specification is the desktop feature's comment block copied byte for byte."
Anchors:
  - { kind: in-tree, path: src/framework/ui/selector/CMakeLists.txt, symbol: qor_uiselector, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/selector/uiselector.h, symbol: UISelector, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/selector/uiselector.cpp, symbol: UISelector::Setup, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/selector/qor_uiselector.cpp, symbol: ThisModule, verified: 2026-11-17 }
Judges:
  - cmake --build cmakebuild --target qor_uiselector --parallel 8
---

# qor_uiselector

"provides runtime selection from available UIs" (`src/framework/ui/selector/purpose.md:1`), indexed
as "Runtime selection among available UIs." (`docs/agent/purposes.md:58`). The folder is 5 files and
111 lines: a 15-line `CMakeLists.txt`, that one-line purpose, the 14-line module stamp
(`qor_uiselector.cpp:10-14`), a 52-line header (`uiselector.h`) and a 29-line implementation
(`uiselector.cpp`). It builds SHARED at project version 0.0.1 (`CMakeLists.txt:1`, `:4-5`) and it is
configured - `src/framework/ui/CMakeLists.txt:4` descends into it.

- Nothing in the build links it. Of the 281 `CMakeLists.txt` under `src`, `test` and `examples`, 3
  lines name the target and all 3 are its own file (`:2`, `:4`, `:10`) - fan-in 0. It is one of 8 of
  the 90 `qor_*` targets named by no other `CMakeLists.txt` (`qor_aggregatesource`, `qor_events`,
  `qor_jip`, `qor_optionparser`, `qor_pipeconnector`, `qor_textshaping`, `qor_uiselector`,
  `qor_unicode`); its own region neighbours all have edges - `qor_uicommon` 3, `qor_uilayout` 3,
  `qor_terminal` 3, `qor_desktopui` 2, `qor_gl` 2, `qor_sercon` 1, `qor_uiwidgets` 1. 0 directories
  under `test` name it. Its name appears on 24 lines in `src`, `test`, `examples` and `docs/agent`:
  21 inside the folder plus `docs/agent/purposes.md:58`, `docs/agent/repo_map.md:71` and
  `docs/agent/repo_map.md:140` - there is no `win*`/`lin*` sibling, no example, no test.
- The link line and the index row agree, and both omit the module that owns the base class:
  `target_link_libraries(${target} qor_module qor_host qor_current)` (`CMakeLists.txt:15`) is exactly
  what `docs/agent/repo_map.md:71` prints, while `uiselector.h:15` includes
  `src/framework/app/role/ifeature.h` and `:21` derives `public app::IFeature` from it. `qor_role` is
  not linked, and `IFeature` is an unmarked class (`class IFeature` at
  `src/framework/app/role/ifeature.h:14`, 0 export markers) whose two verbs are already default
  definitions - `virtual void Setup() {};` (`:22`) and `virtual void Shutdown() {};` (`:23`).
- The class has no verb of its own and no state. The whole body of `uiselector.h:22-42` is the ctor
  and dtor (`:25-26`), the copy/move quartet `= delete` (`:28-31`), `Setup()`/`Shutdown()` (`:33-34`)
  and six comment lines (`:36-41`): 0 data members, 0 parameters, 0 free functions in the header. The
  29-line `uiselector.cpp` is four empty bodies - ctor (`:13-15`), dtor (`:17-19`), `Setup()`
  (`:21-23`), `Shutdown()` (`:25-27`) - so the two overrides restate the inherited `{}` bodies
  (`ifeature.h:22-23`) and change nothing. The DLL's whole published surface is 5 symbols:
  `qor_uiselector.exp` (1,529 bytes) contains exactly `??0UISelector@ui@qor@@QEAA@XZ` (ctor),
  `??1UISelector@ui@qor@@UEAA@XZ` (dtor), `??_7UISelector@ui@qor@@6B@` (vtable),
  `?Setup@UISelector@ui@qor@@UEAAXXZ` and `?Shutdown@UISelector@ui@qor@@UEAAXXZ`.
- The specification is another module's comment block. The 8 comment-only lines of `uiselector.h`
  (`:1-2` license, `:36-41` specification) are byte-identical to the 8 comment-only lines of
  `src/framework/ui/desktop/desktopui.h` - `Compare-Object` on the two sets returns a diff count of 0
  - including the misspelling "tookit" at `uiselector.h:38` and lines that describe a desktop UI:
  "provide singleton objects for the GUI, such as window class registration" (`:39`), "provide access
  to the per thread GUI singletons for the toolkit and the renderingapi in use" (`:40`). The folder
  never names a UI it could select: case-sensitive scans of its three sources return `DesktopUI` 0,
  `Terminal` 0, `SerialConsole` 0 hits - while those are the three UI features the tree has
  (`src/framework/ui/desktop`, `src/framework/ui/terminal`, `src/framework/ui/serial`).
- The dependency-injection triple is declared and used by nobody. `uiselector.h:45` specialises
  `instancer_of` on `SingletonInstancer` (`src/qor/memory/instance/instance.h:22`), `:46` specialises
  `factory_of` on `ExternalFactory` (`src/qor/memory/factory/factory.h:24`), and `:47-48` bind
  `constexpr GUID UISelectorGUID = {0xB062DEE1, ...}` through `qor_pp_declare_guid_of`
  (`src/qor/memory/injection/typeidentity.h:47`). Across `src`, `test` and `examples` there are 0
  occurrences of `instancer_of<ui::UISelector>`, `factory_of<ui::UISelector>`,
  `guid_of<ui::UISelector>`, `AddFeature<ui::UISelector>`, `new_ref<ui::UISelector>` or
  `TheUISelector`, and `UISelectorGUID` appears on 2 lines (both in this header) while the literal's
  first field `0xB062DEE1` appears once - no registration table, `.def` or platform adapter names it.
- What the folder actually consumes is narrower than what it includes. Its three sources have 16
  `#include` lines; 6 of them name no symbol anywhere in the folder: `<atomic>` (`uiselector.h:7`,
  0 `atomic` tokens outside the include), `<coroutine>` (`:8`, 0 `co_await`),
  `src/qor/essentials/current/currentthread.h` (`:9`, 0 `CurrentThread`),
  `src/qor/memory/reference/newref.h` (`:14`, 0 `new_ref`), `<cassert>` (`uiselector.cpp:6`, 0
  `assert(`) and `src/framework/app/role/role.h` (`:9`, 0 `Role`). The stamp is the same shape as the
  other 86: `qor::Module& ThisModule(void)` (`qor_uiselector.cpp:10`) with no export marker - all 87
  module stamps under `src` are plain and 0 carry `qor_pp_export_non_msvc`, while 10 stamps under
  `test` do (`test/argparse/test_main.cpp:126`), and `qor_uiselector.exp` has no `ThisModule` entry.

## Failure modes

- A green judge proves only that the file still compiles. `cmake --build cmakebuild --target
  qor_uiselector --parallel 8` on 17 Nov 2026 compiled both translation units, created
  `qor_uiselector.lib`, `qor_uiselector.exp` and `qor_uiselector.dll` in
  `cmakebuild/src/framework/ui/selector/Debug/`, printed 0 warning or `LNK` lines over its 12 log
  lines and exited 0 - and every one of those facts would be unchanged if the class body were
  deleted, because the build graph has no edge that reads this DLL: 0 other `CMakeLists.txt` names
  the target, and 0 test directories do.
- `ImplementsUISelector()` is declared as an export and defined nowhere. `uiselector.h:17` is its
  only occurrence in the whole tree (1 hit across `src`, `test`, `examples`, `docs/agent`),
  `uiselector.cpp` contains no `qor_pp_module_provide` (`src/configuration/configuration.h:37` is the
  verb that would define it), and the `.exp` has no such entry. So the first consumer to write
  `qor_pp_module_requires(UISelector)` expands (`src/configuration/configuration.h:35`) to
  `bool requiresUISelector = qor::ImplementsUISelector();` against a symbol absent from
  `qor_uiselector.lib`, and nothing else in the tree can supply it - the seam the [[qor_desktopui]]
  page records has a platform sibling that defines the function; this module has no sibling at all.
- Deleting the folder breaks no link line, and the index hides that. Only
  `src/framework/ui/CMakeLists.txt:4` references it, while `docs/agent/repo_map.md:140` places
  `qor_uiselector` in topological layer 9 beside `qor_uilayout`, `qor_uiwidgets`, `qor_desktopui` and
  `qor_terminal` - layers that have consumers (`qor_uicommon` is named by 3 other `CMakeLists.txt`,
  `qor_uilayout` by 3). Its own link edges reach only layer 0, 1 and 2 (`qor_module qor_host
  qor_current`, `CMakeLists.txt:15`), so the layer-9 placement is justified by nothing and a
  build-order or impact tool reading that row over-estimates the blast radius of a change here to
  zero in one direction and nine layers in the other.
- The DI triple invites a wiring that cannot fail visibly. `Role::AddFeature`
  (`src/framework/app/role/role.cpp:98`) keys features by GUID and errors with "Attempting to add a
  feature without a GUID. Did you forget to qor_pp_declare_guid_of(`feature`,`guid`); ?"
  (`role.cpp:102`) when one is missing - and this class has one (`uiselector.h:47-48`), so registering
  it passes. `Role::Setup()` then calls `TrySetup` for each feature (`role.cpp:68-73`) and `TrySetup`
  calls `feature->Setup()` inside a `try` whose handlers escalate
  (`role.cpp:29-44`, `fatal("Feature setup failed with exception {0}.")` at `:39`). An empty
  `Setup()` (`uiselector.cpp:21-23`) can never reach those handlers, so an application that adds the
  "runtime UI selection" feature gets a registered, set-up, healthy-looking selector that selects
  nothing, with 0 data members to select from.

## See also

[[qor_host]] [[qor_module]] [[qor_current]] [[qor_uicommon]] [[qor_uilayout]] [[qor_desktopui]]
[[qor_gl]] [[qor_role]]

Not yet paged: the other UI features a selector would choose between - `qor_terminal`
(`src/framework/ui/terminal`) and `qor_uiwidgets` (`src/framework/ui/widgets`) - plus
`src/framework/app/role`, which owns the unmarked `IFeature` base.


