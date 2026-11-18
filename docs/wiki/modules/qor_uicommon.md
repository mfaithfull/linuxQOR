---
Type: WikiPage
PageType: module
Title: "qor_uicommon"
Target: qor_uicommon
Status: current
LastVerified: 2026-11-17
Claims: 10
Summary: "The qor::ui base classes: twelve header-inline interfaces, one exported class, and two header guards shared with src/components/ui, so each consumer compiles its own copy of the behaviour and can silently drop this folder's twin."
Anchors:
  - { kind: in-tree, path: src/framework/ui/common, symbol: qor_uicommon, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/common/CMakeLists.txt, symbol: qor_uicommon, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/common/contained.h, symbol: Contained, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/common/compound.h, symbol: DetachAllChildren, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/common/selectable.h, symbol: Select, verified: 2026-11-17 }
  - { kind: in-tree, path: src/components/ui/compound.h, symbol: QOR_PP_H_UI_COMPOUND, verified: 2026-11-17 }
Judges:
  - cmake --build cmakebuild --target qor_uicommon --parallel 8
---

# qor_uicommon

"Common UI base (layout/nav shared across UIs)." (`docs/agent/purposes.md:54`). Built SHARED from
two translation units at project version 0.0.1 (`src/framework/ui/common/CMakeLists.txt:1`, `:4`):
`qor_uicommon.cpp` is the 14-line module stamp and `contained.cpp` the 47-line definition half of
`Contained` - the other 12 files are headers, 551 lines between them, and they compile into whoever
includes them.

- 4 of the 281 `CMakeLists.txt` under `src`, `test` and `examples` name the target: its own and
  `src/framework/ui/layout/CMakeLists.txt:20`, `src/framework/ui/terminal/CMakeLists.txt:23`,
  `src/framework/ui/widgets/CMakeLists.txt:15` - the link line `qor_module qor_host qor_current`
  (`src/framework/ui/common/CMakeLists.txt:15`) is `docs/agent/repo_map.md:67` token for token.
  0 test targets name it. Its headers arrive two ways: 11 `#include "../common/<name>.h"` lines
  across 7 files in `layout` (`align.h:8`, `arrangeable.h:12`, `leaf.h:9`) and `widgets`
  (`pushbutton.h:8-11`, `tlc.h:8`, `toolkit.h:9`, `widget.h:8-9`), and 3 files spelling it from the
  root - `src/framework/ui/serial/renderer/renderer.h:9`,
  `src/framework/ui/terminal/renderer/renderer.h:9` and
  `examples/experimental/osspecific/windows/console/expconsole.cpp:11-14`. The serial one is the
  outlier: `qor_sercon` links `qor_module qor_host qor_current qor_sync qor_injection`
  (`src/framework/ui/serial/CMakeLists.txt:15`) and never reaches this target, yet its
  `renderer.h:21` derives `Renderer : public qor::ui::Renderer` from it.
- Exactly one class in the 12 headers carries the export marker: `contained.h:19`
  `class qor_pp_module_interface(QOR_UICOMMON) Contained : public virtual Item`; the other 11
  headers hold 0. So `Item`, `Compound`, `Layer`, `Scene`, `Renderer` and the six `I*` interfaces
  are not in the import library, and `Activate()`, `Render()`, `Add()` and `ApplyIf()` are compiled
  separately into `qor_uilayout`, `qor_uiwidgets` and `qor_terminal` - each of which does mark its
  own classes (`src/framework/ui/widgets/pushbutton.h:15`). The DLL carries the module stamp
  (`qor_uicommon.cpp:10-14`) plus `Contained`'s 5 out-of-line members
  (`contained.cpp:12`, `:18`, `:23`, `:28`, `:37`).
- The shape is a virtual-base mixin set: `Item` (`uiitem.h:16`) is only two templated cast verbs,
  `Is<U>()` and `As<U>()` over `dynamic_cast` (`uiitem.h:21-31`), and every interface derives
  `public virtual Item` so one class can mix Activate/Focus/Hover/Select/Render/See. `Compound` owns
  the children - `std::vector< ref_of<Contained>::type > m_children` (`compound.h:94`) - and
  `Layer` (`layer.h:13`) and `Scene` (`scene.h:14`) are `Compound, public virtual IRenderable` with
  empty bodies. 26 `Is<>`/`As<>` sites exist in the tree; 3 are here (`compound.h:84`,
  `focusable.h:21`, `selectable.h:21`) and 11 are in `src/framework/ui/layout` - `arrangeable.h:33`,
  `:41`, `:64`, `arranger.h:28` and one per box (`dbox.cpp:31`, `hbox.cpp:34`, `vbox.cpp:36`,
  `gridbox.cpp:35`) - the target that actually drives this shape, while 6 of the remaining 12 are
  the private copy in `test/datastructures/polymorphic.cpp`.
- The version the module publishes is 0.0.1: `project(QOR VERSION 0.0.1 ...)` (`CMakeLists.txt:1`)
  feeds `qor_pp_ver_major/minor/patch` (`CMakeLists.txt:11-13`) into the string handed to
  `qor::Module` at `qor_uicommon.cpp:12`. Tree-wide, 45 of the 281 `CMakeLists.txt` declare
  `VERSION 0.5.0` and 8 declare `0.0.1`, and the siblings in this folder's own parent disagree:
  `src/framework/ui/gl/CMakeLists.txt:1` declares `0.0.0`, and
  `src/framework/ui/serial/CMakeLists.txt` has no `project(QOR VERSION` line at all - its line 1 is
  `set(target qor_sercon)`, so the `qor_pp_ver_*` in its `target_compile_definitions` (`:11-13`)
  resolve to whatever the parent left them.
- `IVisible` is declared twice in namespace `qor::ui` - `visible.h:12` and
  `src/components/ui/visible.h:10` - and the framework twin has no framework user. The only
  derivations in the tree are `src/components/ui/item.h:13`, which takes the components twin through
  its `#include "visible.h"` (`:9`), and the example at
  `examples/experimental/osspecific/windows/console/expconsole.cpp:209` and `:216`. Neither twin's
  verbs are called: `Show()` and `Hide()` sit at `visible.h:18` and `:26` with no call site, and of
  the 10 tree-wide `IsVisible(` hits only 2 are this class or its twin (`visible.h:34`,
  `src/components/ui/visible.h:31`); the other 8 are layout nodes
  (`src/framework/ui/layout/node.h:65`, `src/components/ui/layout/node.h:92`) and Windows window
  types (`src/platform/os/windows/ui/gui/controllers/windowcontroller.h:207`,
  `src/platform/os/windows/ui/gui/factories/windowstyle.h:63` and their two `.cpp` definitions).
  Both call sites hit those other types - `src/platform/os/windows/ui/gl/windowimpl.cpp:1797` on a
  `WindowController`, `examples/experimental/canvastest/main.cpp:101` on a layout `Node` inside the
  folder commented out at `examples/experimental/CMakeLists.txt:3`.

- The interaction chain has no entry point. `Select()` (`selectable.h:19`) reaches `Focus()`
  (`:23`), `Focus()` reaches `Activate()` (`focusable.h:23`), `Activate()` walks children
  (`activateable.h:27-30`) - and `Select()` has 2 hits in the tree, its own declaration and
  `Deselect()`'s (`selectable.h:31`). `IsActivated()`, `IsFocused()` and `IsSelected()`
  (`activateable.h:56`, `focusable.h:40`, `selectable.h:39`) have 3 hits in 3 files: the
  declarations, never a read. One hook override exists anywhere, `pushbutton.h:26`
  `virtual void OnSelected() {}//TODO: Raise a signal`, on the one class that mixes all four
  interfaces (`src/framework/ui/widgets/pushbutton.h:15-19`).

## Failure modes

- Where two headers share one guard, the second include is dropped without a word.
  `QOR_PP_H_UI_COMPOUND` is the `#ifndef` of both `compound.h:5` and
  `src/components/ui/compound.h:5`, and `QOR_PP_H_COMPONENTS_UI_SELECTABLE` is the `#ifndef` of
  `selectable.h:5` and of `src/components/ui/visible.h:5` - a guard naming a class the file does not
  declare. The two `Compound` classes are not the same design either: this `ApplyIf` takes a
  forwarding lambda (`compound.h:80`) over `ref_of<Contained>` children (`compound.h:94`), theirs
  takes a function pointer (`src/components/ui/compound.h:86`) over `ref_of<Compound>` children
  (`:100`). The copy-paste is visible in the stale `#endif//QOR_PP_H_UI_COMPOUND` at `contained.h:40`
  and at `uiitem.h:36`, whose own guard is `QOR_PP_H_UI_ITEM` (`:5`). Today the pair is latent rather
  than live: 0 files include the components twins, because `src/components/ui/CMakeLists.txt:1-8`
  only descends into subfolders and the loose headers beside it reach no target at all.
- `DetachAllChildren()` has an exit condition it does not own. It loops
  `while (!m_children.empty()) { m_children[0]->Detach(); }` (`compound.h:71-77`) and trusts each
  child to erase itself; `Contained::Detach()` (`contained.cpp:37-45`) returns immediately when
  `m_parent == nullptr` and otherwise erases through `m_parent->Detach(this)`. Two public paths
  break that trust: `SetParent(Compound*)` (`contained.h:29`, body `contained.cpp:23-26`) moves the
  link with no vector surgery, and `Compound::Detach(Contained*)` (`compound.h:58-69`) erases the
  entry without clearing the child's `m_parent`. A child whose `m_parent` is not this compound is
  never erased from `m_children`, so the loop never terminates; and after the second path the child
  still names a compound that no longer holds it, so `Index()` answers -1
  (`contained.cpp:28-35` through `compound.h:34-46`). Only `Add()` (`compound.h:48-56`) keeps both
  sides in step, and it is the only caller of `SetParent` in the folder.
- Descent only reaches children that already have the interface. `Activate()`
  (`activateable.h:18-33`), `Deactivate()` (`:37-52`) and `IRenderable::Render()`
  (`renderable.h:20-30`) each route through `Compound::ApplyIf<R>`, which casts each **direct** child
  and skips it when the cast fails (`compound.h:79-90`). A `Layer` child -
  `class Layer : public Compound, public virtual IRenderable` (`layer.h:13`), never `IActivatable` -
  takes its whole subtree out of `Activate()`, and an `Arranger` child -
  `class Arranger : public Compound, public virtual IArrangeable`
  (`src/framework/ui/layout/arranger.h:16`), never `IRenderable` - takes its subtree out of
  `Render()`. The folder names the missing half itself: `//Render if it's visible otherwise skip`
  (`layer.h:12`) sits above a class with an empty body (`layer.h:14-20`), and nothing here reads
  `IVisible`.
- The render verb is a stub, and the judge proves only that it parses. `renderer.h` is 34 lines:
  `#include "scene.h"` is commented out (`:9`), `class Scene;` is forward-declared (`:14`) and
  `Frame(Scene*)` (`:23-29`) is four comment lines and no statement - the tree's 15 `Frame(` hits
  include no call of it on a `qor::ui::Renderer`. `IRenderable::Render(Renderer*)`
  (`renderable.h:20`) has exactly one override anywhere,
  `src/framework/ui/terminal/toolkit/label.h:19`, while `src/framework/ui/serial/toolkit/` declares
  7 parameterless `Render()` (`consoleview.h:33`, `consolewidget.h:29` and `:55` among them) that
  cannot override it - in `qor_sercon`, the one target that includes
  `src/framework/ui/serial/renderer/renderer.h:21`'s `Renderer : public qor::ui::Renderer` and does
  not link this target. And `test/CMakeLists.txt:26` builds
  `test/datastructures/polymorphic.cpp`, which re-declares the whole hierarchy privately instead of
  including these headers (`Item` `:37`, `Contained` `:59`, `Compound` `:78`, `Renderable` `:175`,
  `Scene` `:194`, `Widget` `:301`), so no test target in the tree exercises a shipped class from this
  folder.

## See also

[[qor_host]] [[qor_module]] [[qor_current]] [[qor_injection]]

Not yet paged, and the three real consumers: `src/framework/ui/layout` (qor_uilayout),
`src/framework/ui/widgets` (qor_uiwidgets), `src/framework/ui/terminal` (qor_terminal).
