---
Type: WikiPage
PageType: module
Title: "qor_layout"
Target: qor_layout
Status: current
LastVerified: 2026-11-18
Claims: 19
Summary: "A terminal-UI layout engine in src/components/ui: a DOM of shared_ptr<Node> built with a | pipeline, laid out by a three-pass protocol (requirement up, box down, visit), with vbox/hbox/dbox/grid/flexbox containers. It replicates the CSS flexbox model per its own header. This is the components sibling of qor_uilayout - different target, and the Focused-focus machinery is live here where it is commented out over there."
Anchors:
  - { kind: in-tree, path: src/components/ui/layout, symbol: qor_layout, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/layout/CMakeLists.txt, symbol: qor_layout, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/layout/node.h, symbol: Node, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/layout/flexboxconfig.h, symbol: FlexboxConfig, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/layout/box.cpp, symbol: Box::Intersection, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/layout/qor_layout.cpp, symbol: ThisModule, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_layout --parallel 8
---

# qor_layout

Judge GREEN: `cmake --build cmakebuild --target qor_layout --parallel 8` exits 0 and links
`cmakebuild/src/components/ui/layout/Debug/qor_layout.dll`. One warning, no error: C4244
"conversion from `int64_t` to `int`, possible loss of data" at `boxhelper.cpp:37` - the
folder's only compiler diagnostic.

- **Folder:** 34 files - 18 headers, 15 `.cpp`, `CMakeLists.txt`; 3,363 physical lines. The
  three heaviest are `flexboxhelper.cpp` (463), `flexbox.cpp` (321), `flex.h` (212).
- **What it is:** a terminal-UI box model. `namespace qor::components::ui`; the base
  `class Node` (`node.h:58`), `using Element = std::shared_ptr<Node>` (`:46`), and
  `using Decorator = std::function<Element(Element)>` (`:48`) - the page's whole grammar.
- **Three-pass layout** (`node.h:70-78`): `ComputeRequirement` propagates child->parent,
  `SetBox` propagates parent->child, `Visit` walks the tree for focus/selection transients;
  `Check(Status*)` (`:89`) lets an element ask for more iterations, forwarded by every container.
- **`|` is the build language:** `Element operator|(Element, Decorator)`, `operator|=` and the
  `Decorator|Decorator` composition are declared at `node.h:50-53` and defined in
  `operators.cpp`, so a tree is written as `text(...) | border | flex` (the doc example at
  `flex.h:140-144`).
- **`Requirement`** (`requirement.h:37-77`) carries `min_x`/`min_y`, four flex ints
  (`flex_grow_x/y`, `flex_shrink_x/y`) and a nested `Focused` struct - and `Focused` is *live*
  here: `.Prefer(...)` is called in `node.cpp:65`, `vbox.cpp:47`, `hbox.cpp:46`, `gridbox.cpp:88`,
  `flexbox.cpp:171`, `dbox.h:49`. Compare `qor_uilayout`, where the same struct is `/* */`-commented.
- **`Box`** (`box.h:30-46`) is four ints plus `Intersection`/`Union`/`Shift`/`Contain`/`IsEmpty`
  (`box.cpp`); `flexbox.cpp:177-178` clips a focused box to its container with
  `Box::Intersection`, the one place focus is bounded.
- **Containers:** `vbox`/`hbox` (`vbox.cpp`/`hbox.cpp`) sum children and propagate focus;
  `dbox` (`dbox.h:41-68`) stacks children at the same box and takes the max extent;
  `gridbox` (`gridbox.cpp`) lays a 2-D `lines_[y][x]`; `flexbox` is the CSS engine.
- **CSS flexbox, by the file's own word:** `flexboxconfig.h:30-34` states "This replicate the
  CSS flexbox model" and names its guide; `FlexboxConfig` (`:46-142`) mirrors it with
  `Direction`, `Wrap`, `JustifyContent`, `AlignItems`, `AlignContent` enums and a chained
  `Set(...)`; `flexbox.cpp` `Normalize`/`SymmetryXY` (`flexboxhelper.cpp:42-56`) reduce the axes.
- **Default divergence:** `FlexboxConfig`'s `wrap` default is `Wrap::Wrap` (`flexboxconfig.h:70`),
  while the model it names defaults to no-wrap - a default-constructed `flexbox(...)` (`flexbox.cpp:304`,
  used by `hflow`) wraps its line.
- **flex is a decorator, not a flag:** `flex.h` defines a `Flex` node wrapping a
  `void(*)(Requirement&)` (`:37`); `filler()` (`:133`) is `Flex(function_flex)` with no child, and
  `flex`/`xflex`/`yflex`/`flex_grow`/`flex_shrink`/`notflex` (`:150-207`) are that node over a child.
- **Composite helpers:** `compositedecorator.h:40-58` builds `hcenter`/`vcenter`/`center`/`align_right`
  from `filler()` between the child and `hbox`/`vbox`; `reflect.h` adds a `Reflect` node whose
  `ComputeRequirement`/`SetBox` are `final` and which writes a caller's `Box&` (`reflected_box_`).
- **anyargs glue:** the `TAKE_ANY_ARGS` macro (`anyargs.h:52-95`) is applied to `vbox`, `hbox`,
  `dbox`, `hflow` (`:97-100`), generating one-arg, variadic, and container overloads; the
  container form is `template<>`-specialised for `std::stack`/`queue`/`deque`.
- **Link line matches the index exactly:** `CMakeLists.txt:14` links `qor_module qor_role
  qor_application qor_error`, and `docs/agent/repo_map.md:123` lists the same four for
  `qor_layout`; `add_library` (`:3-4`) names all 15 `.cpp`.
- **Purpose line is a subset:** `docs/agent/purposes.md:102` - "Layout component
  (role/app/error)" - names 3 of the 4 link libraries, omitting `qor_module`, the module
  bootstrap every qor_ lib needs (`qor_layout.cpp:31-35` `ThisModule`).

## Failure modes

- **Guard-macro drift, three schemes in one folder** (18 header guards): 16 use
  `QOR_PP_H_COMPONENTS_FRAMEWORK_UI_LAYOUT_*` - the token says `FRAMEWORK` for a folder at
  `src/components/ui/layout`; `anyargs.h` uses `..._FRAMEWORK_TU_LAYOUT_ANYARGS` (a `TU` typo for
  `UI`); only `compositedecorator.h` spells the real path, `QOR_PP_H_SRC_COMPONENTS_UI_LAYOUT_*`.
  None collide, but the macro a maintainer greps for depends on which header they opened.
- **The move-deletion idiom is inert:** `Node(const Node&&) = delete` and its `operator=` twin
  (`node.h:64,66`) delete a *const rvalue reference*, which is not the move constructor - the real
  `Node(Node&&)` is never declared, so it is the deleted copy ctor (`:63,65`) that suppresses the
  implicit move, not these lines.
- `Flex`'s member `FlexFunction f_` is public (`flex.h:127`) - the last declaration in a `public:`
  class with no later access specifier, so a caller can overwrite the function a decorator runs.
- **Silent truncation is compiled in:** the C4244 at `boxhelper.cpp:37` returns an `int64_t` from a
  function typed `int`; every coordinate in this engine is `int` (`box.h:34-37`, `requirement.h:40-41`),
  so a box larger than 2^31 cells wraps without a diagnostic beyond that one warning.
- The flexbox doc comments carry stray tokens that the model copy introduced: `reverse < order.` at
  `flexboxconfig.h:57,68` and "tems evenly" at `:124` - harmless to the compiler, but they sit in the
  /// docs a reader trusts for the enum semantics.

## See also

[[qor_uilayout]] [[qor_module]] [[qor_role]] [[qor_application]] [[qor_error]]
