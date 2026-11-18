---
Type: WikiPage
PageType: module
Title: "qor_tui"
Target: qor_tui
Status: current
LastVerified: 2026-11-18
Claims: 15
Summary: "The terminal-UI component: 123 files / 23,813 lines under src/components/ui/terminal, built as a dom node tree (42 dom .cpp) over a screen surface and a header-less widgets folder. It is not the framework's qor_terminal (same folder name, FTXUI-derived, different deps); this component is the bare 6-dependency port, links honestly against its index row, and lives in namespace qor::components::tui despite living under a ui/terminal path."
Anchors:
  - { kind: in-tree, path: src/components/ui/terminal, symbol: qor_tui, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/terminal/CMakeLists.txt, symbol: qor_tui, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/terminal/dom/node.h, symbol: Node, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/terminal/screen/screen.h, symbol: Screen, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/terminal/widgets/button.cpp, symbol: ButtonBase, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/terminal/qor_tui.cpp, symbol: ThisModule, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_tui --parallel 8
---

# qor_tui

Judge GREEN: `cmake --build cmakebuild --target qor_tui --parallel 8` exits 0 and links
`cmakebuild/src/components/ui/terminal/Debug/qor_tui.dll`; no warning or error line from this target.

- **A heavy component, second only to `qor_renderer`:** 123 files, 23,813 physical lines - 41 `.h` +
  80 `.cpp` + `CMakeLists.txt` + `purpose.md`. By folder: `dom` 55 (9,068 lines), `ui` 31 (6,985),
  `screen` 19 (4,198), `widgets` 12 (3,058), `util` 3 (373).
- **`add_library` names 80 sources** (`CMakeLists.txt:3-82`): `qor_tui.cpp` plus `screen/` 9,
  `dom/` 42, `ui/` 16, `widgets/` 12. The `util/` folder (3 headers, 0 `.cpp`) is not compiled.
- **`widgets/` has 12 `.cpp` and zero headers:** every widget is defined wholly in its `.cpp`
  (`button.cpp` 60+ lines etc.), and the component exposes them only through forward-declared option
  structs in `ui/component.h:39-45` (`ButtonOption`, `MenuOption`, `RadioboxOption`, ...), so a
  header-only symbol search finds none of the twelve widgets.
- **`dom/` is the bulk and a node tree:** 55 files / 42 `.cpp` rooted at
  `class qor_pp_module_interface(QOR_TUI) Node` (`dom/node.h:47`), with decorator leaves
  (`bold.cpp`, `italic.cpp`, `blink.cpp`, `inverted.cpp`, `underlined.cpp`) and layout nodes
  (`box_helper.cpp`, `flexbox.cpp`, `gridbox.cpp`, `table.cpp`, `graph.cpp`, `gauge.cpp`,
  `spinner.cpp`) - a retained-mode terminal UI graph.
- **Namespace, folder and target disagree three ways:** the code opens
  `namespace qor{ namespace components{ namespace tui` (`dom/node.cpp:36`, also
  `screen.cpp`, `component.cpp`, `button.cpp`) - a 3-level `qor::components::tui` - while the folder
  path is `src/components/ui/terminal` and the target is `qor_tui`.
- **`Screen : public Image` is the output surface** (`screen/screen.h:45`); the widget base pattern is
  a pair - `class ... ButtonBase : public ComponentBase, public ButtonOption` (`button.cpp:60`) -
  a behaviour base plus an options struct.
- **Links 6, and the index is honest:** `CMakeLists.txt:93` names `qor_module qor_role qor_application
  qor_workflow qor_signals qor_error`; `docs/agent/repo_map.md:126` lists the same six, so this row
  carries no gap (unlike `qor_renderer`). `docs/agent/purposes.md:104` ("TUI component
  (role/app/workflow/signals/error)") names five, omitting `qor_module`.
- **Built wide-char on Windows:** `target_compile_definitions` (`CMakeLists.txt:88`) sets `UNICODE`
  and `QOR_TUI` alongside the version macros.
- **The sibling with the same folder name is a different target:** `qor_terminal`
  (`docs/agent/repo_map.md:72`, `src/framework/ui/terminal`) links a different set
  (`qor_host qor_current qor_uilayout qor_uicommon qor_uiwidgets`) in namespace `qor::ui::tty`, and
  `docs/agent/purposes.md:60` calls *that* one "Full TUI (FTXUI-derived but deviates for UI/gfx
  compat)" - the richer implementation is the framework neighbour, not this component.
- **The folder states its own floor:** `purpose.md` (3 lines) says it "Intends to provide a portable
  terminal interface ... Must provide at least enough functionality to support Qurses down the line
  ... Windows Console can manage it" - the stated contract is a Qurses-capable baseline, not a
  feature-complete TUI.
- **Bootstrap is the inert-standard shape:** `qor_tui.cpp:31-34` `ThisModule()` names "Querysoft Open
  Runtime: Terminal User Interface Module" and declares no `qor_pp_module_requires`.

## Failure modes

- **The folder name is ambiguous:** `.../ui/terminal` resolves to `qor_tui`, but `qor_terminal` lives
  at `.../ui/terminal` too (framework tree); an anchor that stops at the folder name cannot pick the
  target, so always give the full path or the `qor_tui`/`qor_terminal` symbol.
- **Header navigation misses all widgets:** with `widgets/` carrying no `.h`, a tool that indexes only
  headers lists zero widgets; the twelve (`button`, `checkbox`, `collapsible`, `dropdown`, `menu`,
  `modal`, `slider`, `toggle`, `window`, ...) exist only in `.cpp` and surface through `component.h`
  option structs, so they vanish from a header-driven map.
- **The one-line purpose under-counts the link set:** `purposes.md:104` names five of the six linked
  libraries (drops `qor_module`); a reader sizing this component from that line misses a dependency,
  and only `repo_map.md:126` is complete.
- **Do not read FTXUI lineage into this target:** the `dom/` node+decorator shape looks like FTXUI's
  API, but the tree attributes FTXUI derivation to `qor_terminal` (`purposes.md:60`), not `qor_tui`;
  portability and behaviour guarantees implied by that lineage do not transfer to this component.

## See also

[[qor_terminal]] [[qor_module]] [[qor_role]] [[qor_application]] [[qor_workflow]] [[qor_signals]] [[qor_error]]
