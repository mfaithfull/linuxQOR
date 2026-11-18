---
Type: WikiPage
PageType: module
Title: "qor_desktopui"
Target: qor_desktopui
Status: current
LastVerified: 2026-11-17
Claims: 10
Summary: "The portable desktop feature: a class whose Run() returns EXIT_FAILURE, an export token shared with three folders that never define it, and one consumer that is commented out of the build and cites a folder that does not exist."
Anchors:
  - { kind: in-tree, path: src/framework/ui/desktop, symbol: qor_desktopui, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/desktop/CMakeLists.txt, symbol: qor_desktopui, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/desktop/desktopui.h, symbol: ImplementsDesktopUI, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/desktop/desktopui.cpp, symbol: DesktopUI, verified: 2026-11-17 }
  - { kind: in-tree, path: src/platform/os/windows/ui/desktopui/CMakeLists.txt, symbol: qor_desktopui, verified: 2026-11-17 }
  - { kind: in-tree, path: examples/experimental/visual/CMakeLists.txt, symbol: qor_desktopui, verified: 2026-11-17 }
Judges:
  - cmake --build cmakebuild --target qor_desktopui --parallel 8
---

# qor_desktopui

"For full desktp environments we'll want desktop integration with system menus, icons, launch
infrastructure, application registration, Run As, minimization to a shared bar etc."
(`src/framework/ui/desktop/purpose.md:1` - the misspelling is in the file), indexed as "Desktop
integration (system menus, icons, launch, registration, minimize-to-bar)."
(`docs/agent/purposes.md:55`). The folder is five files: a 15-line `CMakeLists.txt`, that one-line
purpose, the 14-line module stamp (`qor_desktopui.cpp:10-14`, `ThisModule()` and nothing else), a
53-line header (`desktopui.h`) and a 34-line implementation (`desktopui.cpp`). It builds SHARED at
project version 0.0.1 (`src/framework/ui/desktop/CMakeLists.txt:1`, `:4`) and it is configured -
`src/framework/ui/CMakeLists.txt:2` descends into it.

- 3 of the 281 `CMakeLists.txt` under `src`, `test` and `examples` name the target: its own, the
  Windows sibling's link line (`src/platform/os/windows/ui/desktopui/CMakeLists.txt:16`) and
  `examples/experimental/visual/CMakeLists.txt:14`. 0 test CMakeLists name it. The link line is
  five libraries - `qor_module qor_host qor_sync qor_thread qor_role`
  (`src/framework/ui/desktop/CMakeLists.txt:14-15`) - and `docs/agent/repo_map.md:68` prints four;
  the row drops `qor_role`, which is the folder holding the base class this class derives
  (`desktopui.h:15` includes `src/framework/app/role/ifeature.h`, and that base is
  `class IFeature` at `src/framework/app/role/ifeature.h:14` with 0 export markers on it).
- The whole published verb set is `Setup()`, `Shutdown()` and `Run()`, and the implementation is
  empty plus one constant: `Setup()` (`desktopui.cpp:21-23`) and `Shutdown()` (`:25-27`) have empty
  bodies, the ctor and dtor are empty (`:13-19`), and `Run()` (`:29-32`) is
  `return EXIT_FAILURE;` - the one value this target's own code produces. All four are `virtual`
  (`desktopui.h:33-36`), the copy/move quartet is `= delete` (`:28-31`), and the specification of
  what `Run()` should do is six comment lines (`desktopui.h:37-42`: "provide toolkits and rendering
  backends", "provide singleton objects for the GUI, such as window class registration", "provide a
  universal system to connect UI interactions to application events").
- The export token is this folder's name and three other folders write it. Scanning the 3,564
  `.cpp/.h/.hpp` under `src`, `test` and `examples`: 51 bare `QOR_DESKTOPUI` occurrences over 11
  files, 50 of them inside `qor_pp_module_interface( QOR_DESKTOPUI )` - `desktopui.h` 2,
  `src/framework/ui/layout/functional/layout.h` 35 (`:24`, `:30-71`), `functional/layout.cpp` 6
  (`:29`, `:36`, `:43`, `:59`, `:69`, `:129`), `functional/flexbox.cpp:66`, `layout/node.h:31`,
  `layout/nodes/gridbox.h:31`, `layout/requirement.h:11` (a comment), and
  `src/framework/ui/graphical/windows/{window.h:14, mainwindow.h:12, imainwindowimpl.h:14}`. One
  CMakeLists in the tree defines the token, PRIVATE, and it is this one
  (`src/framework/ui/desktop/CMakeLists.txt:9`) - and that definition gates nothing:
  `qor_pp_is_empty(_X)` tests emptiness, not definedness (`src/macros/facilities/is_empty.hpp:35-38`),
  so a non-empty token always takes the `qor_pp_export` arm of
  `qor_pp_if(qor_pp_is_empty(_X), qor_pp_import, qor_pp_export)`
  (`src/configuration/configuration.h:33`, `src/macros/control/if.hpp:24`). All 50 marker sites in the
  other three folders therefore declare these classes `__declspec(dllexport)`
  (`src/platform/compiler/msvc/msvc.h`) in whoever includes them. The inverted client verb
  `qor_pp_module_external( _X )` (`configuration.h:34`) has 0 uses in the 3,283 sources under `src`,
  `test` and `examples` against 2,120 `qor_pp_module_interface(` sites.
- The same leak shows in the include guards: 12 headers outside this folder guard themselves with
  this folder's header name - `src/framework/ui/widgets/{widget.h:4, pushbutton.h:4}`,
  `src/framework/ui/layout/{node.h:4, requirement.h:4, functional/layout.h:4,
  functional/flexbox.h:4, nodes/gridbox.h:4, nodes/detail/anyargs.h:4}` and
  `src/framework/ui/graphical/windows/{window.h:4, mainwindow.h:4, imainwindowimpl.h:4,
  iwindowimpl.h:4}` all open `#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI...`, and their `#endif` comments
  carry the same prefix (`widget.h:34`, `pushbutton.h:34`, `node.h:84`). The `uiwidgets` classes are
  marked `QOR_UIWIDGETS`, so in those files the guard and the export token disagree.
- The one consumer that exists is a sibling target, and it overrides the only verb:
  `winqor_desktopui` derives `class qor_pp_module_interface(QOR_WINDOWSDESKTOPUI) DesktopUI :
  public qor::ui::DesktopUI` (`src/platform/os/windows/ui/desktopui/desktopui.h:36`), supplies its
  own `Run()` (`src/platform/os/windows/ui/desktopui/desktopui.cpp:60`), and registers the portable
  class as the DI key - `static qor::TypeRegEntry< qor::framework::win::DesktopUI,
  qor::ui::DesktopUI > regDesktopUI;` (`winqor_desktopui.cpp:43`). Nothing uses the three DI
  declarations this header makes: `qor_pp_declare_instancer_of(ui::DesktopUI, SingletonInstancer)`
  (`desktopui.h:46`, the `instancer_of` specialisation of
  `src/qor/memory/instance/instance.h:22`), `qor_pp_declare_factory_of(ui::DesktopUI,
  ExternalFactory)` (`:47`, `src/qor/memory/factory/factory.h:24`) and
  `qor_pp_declare_guid_of(ui::DesktopUI, DesktopUIGUID)` (`:49`, GUID literal `:48`,
  `src/qor/memory/injection/typeidentity.h:47`). The tree's only accessor is a hand-set file-static
  in the sibling: `DesktopUI* _theDesktopUI = nullptr;` (sibling `desktopui.cpp:37`), assigned in
  its ctor (`:47`), cleared in its dtor (`:52`), read by `DesktopUI& TheDesktopUI()` (`:40-42`)
  which `windows/mainwindow.cpp:36` and `:42` call for `GetWindowFactory()`.
- `ImplementsDesktopUI()` is declared with this target's token and defined with the sibling's:
  `namespace qor{ bool qor_pp_module_interface(QOR_DESKTOPUI) ImplementsDesktopUI();}`
  (`desktopui.h:17`) is its only occurrence in the 3,564 scanned sources, and neither of this
  target's two `.cpp` files defines it. The definition is
  `qor_pp_module_provide(QOR_WINDOWSDESKTOPUI, DesktopUI)` (sibling `desktopui.cpp:32`), which
  expands to `bool qor_pp_module_interface(QOR_WINDOWSDESKTOPUI) ImplementsDesktopUI(){ return
  true; }` (`src/configuration/configuration.h:37`). The consumer verb
  `qor_pp_module_requires(DesktopUI)` (`examples/experimental/visual/visual.cpp:54`) expands to
  `bool requiresDesktopUI = qor::ImplementsDesktopUI();` (`src/configuration/configuration.h:35`) -
  a call this DLL does not answer, so the requirement is only met by linking `winqor_desktopui`,
  which that example does at `examples/experimental/visual/CMakeLists.txt:22`. The same
  exported-`Implements*`-with-no-definition seam the [[qor_asyncioservice]] page records.

## Failure modes

- A green judge says nothing here, because the verb already returns failure. The build compiled both
  translation units and linked
  `qor_desktopui.vcxproj -> cmakebuild/src/framework/ui/desktop/Debug/qor_desktopui.dll` at exit 0
  on 17 Nov 2026, while `Run()` is `return EXIT_FAILURE;` (`desktopui.cpp:31`). A caller that reads
  the return as an event-loop exit code gets a failure on the first call, and nothing above the
  `IFeature` seam can tell it from a UI that ran and stopped.
- The only consumer in the tree is out of the build and its includes are dead paths.
  `examples/experimental/CMakeLists.txt:8` reads `#add_subdirectory(visual)`, so the `visual` target
  (`examples/experimental/visual/CMakeLists.txt:1`, `:3`) is never configured; and
  `examples/experimental/visual/visual.cpp:35-36` include
  `src/framework/ui/desktopui/desktopui.h` and `src/framework/ui/desktopui/windows/mainwindow.h`,
  neither of which exists - `Test-Path` is False for `src/framework/ui/desktopui` and for
  `src/framework/ui/desktopui/windows`, while the real spellings are
  `src/framework/ui/desktop/desktopui.h` and
  `src/platform/os/windows/ui/desktopui/windows/mainwindow.h`. Re-enabling that line 8 fails at the
  include, long before `role->AddFeature<ui::DesktopUI>()` at `visual.cpp:81` matters.
- A token that is not a switch cannot be fixed from CMake. `QOR_DESKTOPUI` is written at 50 marker
  sites in three other folders, and since `qor_pp_is_empty` never tests definedness
  (`src/macros/facilities/is_empty.hpp:35-38`), adding or removing the `QOR_DESKTOPUI` definition at
  `src/framework/ui/desktop/CMakeLists.txt:9` leaves every one of those sites on `qor_pp_export`: the
  classes in `layout/functional/layout.h`, `layout/node.h`, `layout/nodes/gridbox.h` and
  `src/framework/ui/graphical/windows/*.h` are declared `dllexport` by whichever target includes
  them. The folders holding most of those markers cannot be reached to check what they emit:
  `graphical` and `scenetemplate` have no `CMakeLists.txt` under `src/framework/ui`, and
  `src/framework/ui/layout/CMakeLists.txt:4-10` names seven sources, none of them under
  `functional/` - so `layout.cpp`'s six definitions reach no target today.
- The index row and the layer table disagree about this row's dependency. `docs/agent/repo_map.md:68`
  prints the link line as `qor_module qor_host qor_sync qor_thread` and omits `qor_role`, which the
  CMakeLists names at `:15`; and the same file's layering puts `qor_desktopui` at 9 (`:140`) while
  placing `qor_role` at 10 (`:141`) - the dependent is scheduled before the thing it derives from,
  the inversion the [[qor_sockets]] page records for its own row. The base is unmarked
  (`ifeature.h:14`), so every consumer compiles its own copy of `IFeature`.

## See also

[[qor_host]] [[qor_module]] [[qor_role]] [[qor_sync]] [[qor_thread]] [[qor_asyncioservice]]
[[qor_uicommon]] [[qor_sockets]]

Not yet paged: the OS sibling `src/platform/os/windows/ui/desktopui` (target `winqor_desktopui`, not
one of the 90 `qor_*` units), and the folders that write this target's export token -
`src/framework/ui/layout` (qor_uilayout), `src/framework/ui/widgets` (qor_uiwidgets) and
`src/framework/ui/graphical` (no target).

