---
Type: WikiPage
PageType: module
Title: "qor_widgets"
Target: qor_widgets
Status: current
LastVerified: 2026-11-18
Claims: 14
Summary: "The widgets component, and the smallest of the 'five densest' group: 7 files / 658 lines in src/components/ui/widgets, offering just an abstract WidgetBase and one concrete Panel over a qor_layout foundation. It links five libraries and its index row is honest, but nothing under src/ links it - its only consumers are an experimental example - and its public header forward-declares Focus and Params that no translation unit in the target defines."
Anchors:
  - { kind: in-tree, path: src/components/ui/widgets, symbol: qor_widgets, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/widgets/CMakeLists.txt, symbol: qor_widgets, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/widgets/widgetbase.h, symbol: WidgetBase, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/widgets/panel.h, symbol: Panel, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/widgets/event.h, symbol: Event, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/widgets/qor_widgets.cpp, symbol: ThisModule, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_widgets --parallel 8
---

# qor_widgets

Judge GREEN: `cmake --build cmakebuild --target qor_widgets --parallel 8` exits 0 and links
`cmakebuild/src/components/ui/widgets/Debug/qor_widgets.dll`; no warning or error line from this target.

- **The smallest member of the group, not the densest:** 7 files, 658 physical lines - 3 `.h`
  (`widgetbase.h` 119, `panel.h` 154, `event.h` 39) + 3 `.cpp` (`widgetbase.cpp` 268, `panel.cpp` 30,
  `qor_widgets.cpp` 35) + `CMakeLists.txt` 13, with no subfolders - against `qor_renderer`'s 27,693
  lines in the same `Group7`.
- **`add_library` names 3 sources** (`CMakeLists.txt:3`): `qor_widgets.cpp`, `widgetbase.cpp`,
  `panel.cpp` - the whole component is two implementation TUs plus the bootstrap.
- **Two concrete types only:** an abstract base `class ... WidgetBase` (`widgetbase.h:54`) and one
  widget, `class ... Panel : public WidgetBase, public PanelOptions` (`panel.h:91`) - a base plus a
  single panel, nothing else ships.
- **Links 5, and the index agrees:** `CMakeLists.txt:13` names `qor_module qor_role qor_application
  qor_error qor_layout`; `docs/agent/repo_map.md:127` lists the same five - the row is complete.
  `docs/agent/purposes.md:105` ("Widgets component.") names none of them.
- **The layout dependency is its sibling:** `qor_layout` (the leaf-1 `Group7` target,
  `src/components/ui/layout`) is the one non-qor infrastructure dependency, so `Panel` composes through
  that layout library rather than a private one.
- **Support types are option structs, not widgets:** `struct PanelOptions` (`panel.h:47`) and
  `struct PanelRenderState` (`panel.h:39`) parameterise `Panel`; `struct Event` (`event.h:30`) is a
  small value type (5 lines), so the component's vocabulary is one widget plus its render/option/event
  types.
- **Namespace matches the folder:** `namespace qor{ namespace components{ namespace ui`
  (`widgetbase.h:33`, and `panel.h:36`, `event.h:28`) - `qor::components::ui`, unlike `qor_renderer`
  (`::renderer`) and `qor_tui` (`::tui`).
- **Guard-macro fossil shared with its siblings:** all three headers guard
  `QOR_PP_H_COMPONENTS_FRAMEWORK_UI_WIDGETS_*` (`widgetbase.h`, `panel.h`, `event.h`) - the token says
  `FRAMEWORK` for a folder under `src/components/ui`, the same fossil as the other `ui/` components.
- **Bootstrap is the inert-standard shape:** `qor_widgets.cpp:31-34` `ThisModule()` names "Querysoft
  Open Runtime: Widgets Module" and declares no `qor_pp_module_requires`.
- **The name collides three ways in the tree:** this `qor_widgets` (`src/components/ui/widgets`), the
  12-`.cpp` `widgets/` subfolder compiled *into* `qor_tui` (namespace `qor::components::tui`), and
  `qor_uiwidgets` referenced as a `qor_terminal` dependency (`docs/agent/repo_map.md:72`) - an anchor
  must carry the full path.

## Failure modes

- **Nothing in the product uses it:** no target under `src/` links `qor_widgets`; the only linkers are
  `examples/experimental/canvastest/CMakeLists.txt:10,15`, so a green judge proves the library builds,
  not that any shipped component depends on it.
- **The "densest lib" heading measures the queue, not this target:** at 658 lines and two concrete
  types, `qor_widgets` is the group's floor; reading the `Group7` label as "complex" overstates this
  component, which is a base class and a panel.
- **The dependency line is empty:** `purposes.md:105` gives one bare label ("Widgets component.") with
  zero of the five linked libraries named; the link set (`qor_layout` included) is only in
  `repo_map.md:127` / `CMakeLists.txt:13`.
- **The public header advertises types the target never defines:** `WidgetBase` forward-declares
  `class Focus;` (`widgetbase.h:41`) and an exported `class ... Params;` (`widgetbase.h:46`), but a
  tree-wide search finds no `Focus`/`Params` definition in any `qor_widgets` TU - the only `class Focus`
  in the repo is `qor::components::tui::Focus` in `qor_tui` (`terminal/dom/frame.cpp:41`), a different
  namespace, so a consumer cannot size or construct these through `qor_widgets`.

## See also

[[qor_layout]] [[qor_tui]] [[qor_terminal]] [[qor_module]] [[qor_role]] [[qor_application]] [[qor_error]]
