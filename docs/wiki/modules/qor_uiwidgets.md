---
Type: WikiPage
PageType: module
Title: "qor_uiwidgets"
Target: qor_uiwidgets
Status: current
LastVerified: 2026-11-17
Claims: 10
Summary: "The framework's smallest target - 9 files, 217 lines - and its least-reachable one: one other CMakeLists links it, 21 of the dll's 78 exports are its own, and the desktop Window the Widget base was written for sits in a folder no target compiles."
Anchors:
  - { kind: in-tree, path: src/framework/ui/widgets, symbol: qor_uiwidgets, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/widgets/CMakeLists.txt, symbol: qor_uiwidgets, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/widgets/pushbutton.h, symbol: PushButton, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/widgets/label.h, symbol: Label, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/widgets/toolkit.h, symbol: Toolkit, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/widgets/widget.h, symbol: Widget, verified: 2026-11-17 }
Judges:
  - cmake --build cmakebuild --target qor_uiwidgets --parallel 8
---

# qor_uiwidgets

"Conceptual level widgets just have state and actions / If and how they are rendered is a separate
issue" (`src/framework/ui/widgets/purpose.md:1-2`). The folder is 9 files and 6,360 bytes - 2 `.cpp`
(14 and 19 lines), 6 headers (label.h 33, pushbutton.h 35, tlc.h 29, toolkit.h 41, widget.h 24,
purpose.md 7), a 15-line `CMakeLists.txt` - the smallest target folder in `src/framework`. It builds
SHARED and is configured (`src/framework/ui/CMakeLists.txt:7`).

- `CMakeLists.txt:4-5` compiles both of the folder's two `.cpp` - `qor_uiwidgets.cpp` (a 14-line
  `ThisModule`) and `pushbutton.cpp` (19) - so unlike [[qor_sercon]] the inventory and the source
  list cannot disagree; there is nothing here to hide from the judge. `QOR_UIWIDGETS` is defined
  PRIVATE by exactly this one CMakeLists (`:10-13`). The link line `qor_module qor_host qor_current
  qor_uicommon qor_uilayout` (`:15`) is what `docs/agent/repo_map.md:74` prints character for
  character, the layer table places the target at 9 (`repo_map.md:140`), and `docs/agent/purposes.md:61`
  restates the purpose file - "transient pushed state" - which claim 5 finds absent.
- Its reach is one link line and five `#include` lines. 3 of the 5 CMakeLists lines naming the
  target are its own (`CMakeLists.txt:2`, `:4`, `:10`); exactly one other target links it,
  [[qor_terminal]] (`src/framework/ui/terminal/CMakeLists.txt:23`), and 0 CMakeLists under `test`
  name it. Outside the folder its headers are read from 5 places: the four terminal toolkit headers
  (`terminal/toolkit/label.h:7`, `pushbutton.h:7`, `tlc.h:7`, `toolkit.h:7`, each by
  repo-root-absolute include) and `graphical/windows/window.h:7`. Tree-wide there are 0
  `new_ref<PushButton>`, `new_ref<Label>` or `new_ref<TopLevelContainer>` sites - the widgets that
  get built at runtime are [[qor_terminal]]'s `tty::` twins, never these.
- The folder carries one marker and three namespaces' worth of provenance drift. 1 of its 5 classes
  is marked: `class qor_pp_module_interface(QOR_UIWIDGETS) PushButton` (`pushbutton.h:15`); `Label`,
  `TopLevelContainer`, `Toolkit` and `Widget` are unmarked. Two classes sit outside the folder's
  namespace - `class Widget` in `qor::ui` (`widget.h:11-13`) and `class Toolkit` in `qor::ui`
  (`toolkit.h:13-15`) - and the header guards name other folders: `QOR_PP_H_FRAMEWORK_DESKTOPUI_*`
  (`pushbutton.h:4`, `widget.h:4`) for a folder that is `ui/widgets`, not `ui/desktop`, and
  `QOR_PP_H_FRAMEWORK_UI_COMMON_TOOLKIT` (`toolkit.h:4`) for a file that is not in `ui/common`. The
  base `Toolkit`'s only inheritor tree-wide is `class Toolkit : public ui::Toolkit`
  (`src/framework/ui/terminal/toolkit/toolkit.h:13`).
- The export table is 73% somebody else's. The built `qor_uiwidgets.dll` (1,036,800 bytes) exports
  78 symbols; 21 of them are the folder's own and all 21 are `PushButton` (`@widgets@ui@qor`: two
  ctors, dtor, assignment, 7 vftables covering its six interfaces, 8 vbtables, vector-delete,
  `OnSelected`). The other 57 are inline definitions from linked modules re-published by this dll:
  52 `@layout@ui@qor` symbols of the eight [[qor_uilayout]] classes (`AlignBottom`, `AlignLeft`,
  `AlignRight`, `AlignTop`, `Box`, `Center`, `HCenter`, `VCenter`) plus 5 `Contained@ui@qor` of
  [[qor_uicommon]] (`common/contained.h:19`) - the same always-export lease the [[qor_terminal]] page
  records from the other side of the link line. `Label`, `TopLevelContainer`, `Toolkit` and `Widget`
  contribute nothing to the table.
- The purpose contract is caption-with-no-action and no pushed state. `purpose.md:4-5` promises "a
  Caption and a Push function that triggers an Action or Enqueue's an event" and "a transient state
  indicating whether it is pushed"; the class has `m_text` (`pushbutton.h:29`) and
  `virtual void OnSelected() {}//TODO: Raise a signal` (`:26`), and no state member at all, while
  inheriting four interaction interfaces (`pushbutton.h:16-19`: `IActivatable`, `IFocusable`,
  `ISelectable`, `IHoverable`, all owned by `qor_uicommon`). `Label::ComputeRequirement` hard-codes
  `m_requirement.min_x = static_cast<int>(m_text.size()); m_requirement.min_y = 1` (`label.h:18-23`)
  under the comment "Defaults for fixed font, console rendering" (`:20`) - the renderer assumption
  `purpose.md:2` rules out of scope. And the folder has two incompatible roots: `TopLevelContainer`
  re-derives `Compound, virtual IActivatable, virtual IRenderable` (`tlc.h:12-14`), dropping the
  `Contained` and `layout::IArrangeable` that `Widget` gives `Label`.
- The desktop half the `Widget` base was written for is dead text. `Widget : public Compound, public
  virtual Contained, public virtual layout::IArrangeable, public virtual IRenderable`
  (`widget.h:13`); its only `: public Widget` use tree-wide is `Window`
  (`src/framework/ui/graphical/windows/window.h:14`), and the string `graphical` appears in 0
  CMakeLists in the tree, so that folder's 158 code lines (`window.cpp` 19, `mainwindow.cpp` 28, 4
  headers) are compiled by no target. The desktop UI that does compile is `winqor_desktopui`'s own
  `windows/mainwindow.cpp` (`src/platform/os/windows/ui/desktopui/CMakeLists.txt:4`), a different file
  that does not go through this base.

## Failure modes

- The judge is green but proves nothing today. `cmake --build cmakebuild --target qor_uiwidgets
  --parallel 8` (17 Nov 2026) exited 0 with 0 compile lines - msbuild found the target up to date;
  the dll is stamped 17 Nov 2026 22:57:05 against a newest folder source of 15 Oct 2026 11:39:12.
  Three older copies of the same 1,036,800-byte dll (stamped 19 Oct 2026) sit deployed in the
  `examples` Debug trees. The Judges line re-checks stamps, not compiles.
- The factory default is a null reference. `Toolkit::CreateTopLevelContainer` returns
  `ref_of<Item>::type()` (`toolkit.h:33-36`) - nothing - and the whole "pre-registered types ...
  Windows GUI Factory" specification is a 10-line comment block (`:22-32`) with no registration
  mechanism anywhere. A consumer holding this base `Toolkit` gets a null top-level container with no
  error; only [[qor_terminal]]'s override returns anything real.
- Four of the five classes cannot fail here, only in consumers. `Label`, `TopLevelContainer`,
  `Toolkit` and `Widget` have no translation unit in this target and 0 exports (`pushbutton.cpp` and
  `qor_uiwidgets.cpp` are the only compiled sources), so the judge stays green whatever those
  headers do; `Label`'s one-line console requirement and `TopLevelContainer`'s missing
  `IArrangeable` surface only as compile errors inside [[qor_terminal]] - or silently never surface.
- The guards pre-seed a collision. `widget.h:4`/`pushbutton.h:4` claim
  `QOR_PP_H_FRAMEWORK_DESKTOPUI_WIDGETS_*` and `toolkit.h:4` claims
  `QOR_PP_H_FRAMEWORK_UI_COMMON_TOOLKIT`; a future real `framework/ui/desktop/widgets` or
  `framework/ui/common/toolkit` header with the matching path would share the guard, and a TU
  including both silently keeps only the first. The risk is latent, not live: nothing that
  compiles today pairs these headers with a matching-name sibling.

## See also

[[qor_module]] [[qor_host]] [[qor_current]] [[qor_uicommon]] [[qor_uilayout]] [[qor_terminal]]
[[qor_desktopui]]

Not yet paged: the Windows sibling `src/platform/os/windows/ui/desktopui` (`winqor_desktopui`), the
component twin `src/components/ui/terminal` (`qor_tui`), and the unconfigured
`src/framework/ui/graphical` - no target compiles its 158 code lines.
