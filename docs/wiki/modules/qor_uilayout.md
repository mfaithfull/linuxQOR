---
Type: WikiPage
PageType: module
Title: "qor_uilayout"
Target: qor_uilayout
Status: current
LastVerified: 2026-11-17
Claims: 10
Summary: "Three quarters of the folder's .cpp lines reach no target; the half that ships is unmarked, the marked half is a second model, and the flex decorators call a member that does not exist."
Anchors:
  - { kind: in-tree, path: src/framework/ui/layout/CMakeLists.txt, symbol: qor_uilayout, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/layout/arrangeable.h, symbol: IArrangeable, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/layout/flex.h, symbol: Flex, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/layout/sizeconstrainer.h, symbol: SizeConstrainer, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/layout/functional/layout.h, symbol: WidthOrHeight, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/layout/nodes/gridbox.h, symbol: GridBox, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/layout/measurable.h, symbol: IMeasurable, verified: 2026-11-17 }
Judges:
  - cmake --build cmakebuild --target qor_uilayout --parallel 8
---

# qor_uilayout

"base layout item/node / algorithms like flexbox for arranging nodes in a space / vbox / hbox / grid
/ flexbox / radial / textflow", then "Layout should be pure logic, not dependent at all on the
presentation except perhaps to get the extents of things. The same layout should be used across all
UIs, even minimal as it will determine the left, right, up down, navigation for that as well"
(`src/framework/ui/layout/purpose.md:1-12`), indexed as "Base layout node +
flexbox/vbox/hbox/grid/radial/textflow algorithms; pure logic (only needs extents); shared across
all UIs incl. nav." (`docs/agent/purposes.md:57`). No compiled source in the folder mentions
navigation, `radial` or `textflow`, and the folder's two competing node models are the actual
subject.

- 38 files, 2,978 lines: 14 `.cpp` (1,652 lines), 22 headers (1,294 lines), a 20-line
  `CMakeLists.txt` and the 12-line purpose. `CMakeLists.txt:4-10` names 7 sources, and the judge
  compiled exactly those 7 translation units - `qor_uilayout.cpp align.cpp box.cpp dbox.cpp hbox.cpp
  vbox.cpp boxhelper.cpp` - 440 of the folder's 1,652 `.cpp` lines. The other 7 files are
  `gridbox.cpp` (62), `node.cpp` (84), `functional/flexbox.cpp` (302),
  `functional/flexboxconfig.cpp` (53), `functional/flexboxhelper.cpp` (437),
  `functional/layout.cpp` (142) and `nodes/gridbox.cpp` (132): 1,212 lines, no target, and no
  `#if`/`#ifdef` anywhere to hide them - the CMakeLists simply never names them. 15 of the 22
  headers are not reached by any of the 7 compiled sources either.
- The export token is 10 occurrences wide and this folder is the only place it is defined. Scanning
  `src`, `test` and `examples`: `QOR_UILAYOUT` appears 10 times in 4 files - `align.h` 7
  (`:12, :20, :28, :36, :44, :52, :60`), `box.h:9`, `vbox.h:11`, and the one
  `target_compile_definitions(${target} PRIVATE QOR_UILAYOUT` (`CMakeLists.txt:15`); 9 of the 10
  are `qor_pp_module_interface( ... )` markers, and per `src/configuration/configuration.h:33` the
  marker is `qor_pp_export` only where the token is defined. Against that, this folder writes the
  [[qor_desktopui]] token 45 times over 6 files - `functional/layout.h` 35,
  `functional/layout.cpp` 6, `node.h:31`, `nodes/gridbox.h:31`, `functional/flexbox.cpp:66` and the
  commented `requirement.h:11` - and opens 8 of its headers with `QOR_PP_H_FRAMEWORK_DESKTOPUI*`
  guards (`node.h:4`, `requirement.h:4`, `functional/{layout.h:4, flexbox.h:4, flexboxconfig.h:4,
  flexboxhelper.h:4}`, `nodes/gridbox.h:4`, `nodes/detail/anyargs.h:4`).
- The model that ships is the unmarked one. `class Arranger : public Compound, public virtual
  IArrangeable` (`arranger.h:16`) and `class IArrangeable : public virtual Item`
  (`arrangeable.h:17`) carry 0 markers, and so do `HBox` (`hbox.h:11`), `DBox` (`dbox.h:11`),
  `GridBox` (`gridbox.h:11`), `Leaf` (`leaf.h:13`) and `Spacer` (`flex.h:14`); the only marked
  classes in the compiled half are `Box` (`box.h:9`), `VBox` (`vbox.h:11`) and the seven alignment
  wrappers (`align.h:12-60`). The interface consumers actually derive from is the unmarked one -
  `class Widget : public Compound, public virtual Contained, public virtual
  layout::IArrangeable, public virtual IRenderable` (`src/framework/ui/widgets/widget.h:13`) - while
  the `Contained` on the same base list is marked `QOR_UICOMMON`
  (`src/framework/ui/common/contained.h:19`). So each consumer compiles its own `IArrangeable`,
  `Arranger`, `HBox`, `DBox` and `Spacer`: the `qor_uicommon` shape one level down.
- Two node models share the folder and only one is compiled. The compiled one is the
  `Compound`/`IArrangeable` pair - `ComputeRequirement()` propagates through
  `compound->ApplyIf<IArrangeable>(...)` (`arrangeable.h:33-46`, `hbox.cpp:19`, `vbox.cpp:19`,
  `dbox.cpp:18`, `gridbox.cpp:19`) and `SetBox()` hands the same box down (`arranger.h:23-34`). The
  uncompiled one is `class qor_pp_module_interface(QOR_DESKTOPUI) Node` (`node.h:31`) with
  `Elements children_` (`:77`) typed by `using Element = ref_of<Node>::type`
  (`functional/layout.h:26`), its `ComputeRequirement()` walking a vector directly
  (`node.cpp:21-52`) - a second three-step protocol over the same `Box` (`box.h:9`) and the same
  `Requirement` (`requirement.h:16`), with `node.h:56-60` re-declaring the `Status` struct that
  `arrangeable.h:21-25` already declares. `requirement.h:11` still carries the forward declaration
  of the other model's `Node` as a comment. The navigation the purpose file promises
  (`purpose.md:11` - "it will determine the left, right, up down, navigation for that as well")
  exists only as commented code: the `Focused` struct is commented out inside `Requirement`
  (`requirement.h:28-56`, closing `Focused focused;*/` at `:56`), and every site that would propagate
  it is inside a block comment - `node.cpp:44-49`, `functional/flexbox.cpp:148-161`,
  `nodes/gridbox.cpp:63-76`. `radial` and `textflow` (`purpose.md:7-8`) appear in no source file in
  the folder.
- The only algorithm that ships is 105 lines of flex arithmetic. `detail::boxhelper::Compute`
  (`nodes/detail/boxhelper.cpp:70-102`) sums `min_size`, `flex_grow` and `flex_shrink` and picks one
  of `ComputeGrow` (`:22`), `ComputeShrinkEasy` (`:36`) or `ComputeShrinkHard` (`:51`), with
  `SafeRatio` guarding the divide (`:14-17`); `HBox::SetBox` calls it with
  `target_size = box.x_max - box.x_min + 1` (`hbox.cpp:45`) and `VBox::SetBox` with the `y` span
  (`vbox.cpp:47-48`). The 437-line `functional/flexboxhelper.cpp` and the 302-line
  `functional/flexbox.cpp` - the "flexbox" the purpose line and the index row both name - reach no
  target, and neither does `Box`'s own `Intersection`/`Union` peer set beyond the 7 methods that do
  compile (`box.cpp:12-66`).
- Fan-in is two modules and no test. 6 lines across 4 `CMakeLists.txt` name the target: its own
  (`:2`, `:4`, `:15`), `src/framework/ui/widgets/CMakeLists.txt:15`,
  `src/framework/ui/terminal/CMakeLists.txt:23` and
  `examples/experimental/osspecific/windows/console/CMakeLists.txt:12` - and that last one is
  `#add_subdirectory(console)` (`examples/experimental/osspecific/windows/CMakeLists.txt:7`), so it
  is never configured. `test/ui/` holds one directory, `terminal`; there is no layout test project
  and no test CMakeLists names the target. Here the index row does match the link line exactly:
  `docs/agent/repo_map.md:70` prints `qor_module qor_host qor_current qor_uicommon` for
  `CMakeLists.txt:20` - but the same file's layer table puts `qor_uilayout qor_uiwidgets
  qor_terminal` all in layer 9 (`:140`), while widgets links layout (`:15`) and terminal links both
  (`:23`).

## Failure modes

- The flex decorators cannot be instantiated, and the build cannot tell you. `flex.h` is 193 lines
  with 11 classes: one non-template (`Spacer`, `:14`) and ten constrained templates (`Flex :31`,
  `XFlex :49`, `YFlex :65`, `Grow :81`, `XGrow :97`, `YGrow :112`, `Shrink :127`, `XShrink :142`,
  `YShrink :157`, `NotFlex :173`). Every one of the ten calls `T::ComputeRequirment()` - missing the
  `e` - at `:40, :58, :74, :90, :106, :121, :136, :152, :167, :182`, while the member
  `IArrangeable` actually declares is `ComputeRequirement()` (`arrangeable.h:30`). Nothing in the
  tree instantiates any of them, so the 10 bad lines are never name-checked; the first
  `new_ref<Flex<VBox>>()` is a compile error inside this header, and the same typo sitting in the
  comment at `arrangeable.h:32` (and at `test/datastructures/polymorphic.cpp:250`) means a search for
  the correct spelling does not surface it.
- The folder's two halves collide on names, so "just compile the rest" cannot work. `GridBox` is
  declared twice in namespace `qor::ui::layout` - `class GridBox : public Arranger` (`gridbox.h:11`,
  guard `QOR_PP_H_FRAMEWORK_UI_LAYOUT_GRIDBOX`) and `class qor_pp_module_interface(QOR_DESKTOPUI)
  GridBox : public Node` (`nodes/gridbox.h:31`, guard
  `QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_GRIDBOX`) - and both implementations define the same members
  (`gridbox.cpp:11, :14, :27` vs `nodes/gridbox.cpp:18, :36, :79`); a third `GridBox` lives in the
  component tree at `src/components/ui/layout/gridbox.h:53` marked `QOR_LAYOUT`. The constraint enums
  collide the same way - `enum WidthOrHeight { WIDTH, HEIGHT };` and
  `enum Constraint { LESS_THAN, EQUAL, GREATER_THAN };` at `sizeconstrainer.h:14-15` and again at
  `functional/layout.h:21-22` - so `sizeconstrainer.h` and `functional/layout.h` can never share a
  translation unit; and `nodes/gridbox.h:19` defines `int Integrate(std::vector<int>&)` with external
  linkage inside a header, a multiple-definition error the moment two sources include it.
- Two live defects sit in the arithmetic that does ship. `IArrangeable::ComputeRequirement` takes the
  first child unconditionally - `auto firstChild = compound->ChildAt(0)->As<IArrangeable>();`
  (`arrangeable.h:41`) - after only an `Item::Is<Compound>()` test (`:33`), with no `ChildCount()`
  check, and `Compound::ChildAt` guards with `assert(i < ChildCount()); return m_children[i];`
  (`compound.h:23-27`) - an `assert`, so the guard is compiled out under `NDEBUG` in every consumer
  that inlines this header, which is all of them, since `IArrangeable` carries no export marker. And
  `SizeConstrainer::SetBox` clamps with `box.x_max = std::min(box.x_min + m_value + 1, box.x_max);`
  (`sizeconstrainer.h:58`, same shape for height at `:70`) where the span convention is
  `x_max - x_min + 1` (`hbox.cpp:45`) - an `EQUAL` width of `N` yields a child `N + 1` wide.
- The dead headers are dead in two different ways. `measurable.h` declares `class IMeasurable` (`:14`)
  whose constructor, destructor and `Measure()` are all private by `class` default (`:16-22`), and
  includes `"../item.h"` (`:9`) - `src/framework/ui/item.h` does not exist; `Item` is reached through
  `#include "uiitem.h"` at `common/compound.h:14` and `common/contained.h:13`. Separately,
  `src/framework/ui/graphical/windows/window.h:8` includes `"../layout/nodes/vbox.h"`, and
  `Test-Path src/framework/ui/layout/nodes/vbox.h` is False - the compiled `VBox` is at
  `layout/vbox.h`. The three headers this folder exports to configured targets are `arrangeable.h`
  (`widgets/widget.h:7`), `align.h` (`widgets/pushbutton.cpp:7`) and `box.h`
  (`terminal/renderer/image.h:10`); the other 19 have no consumer anywhere.

## See also

[[qor_uicommon]] [[qor_desktopui]] [[qor_host]] [[qor_module]] [[qor_current]] [[qor_gl]]

Not yet paged: the two live consumers `qor_uiwidgets` (`src/framework/ui/widgets`, links this target at
`CMakeLists.txt:15`) and `qor_terminal` (`src/framework/ui/terminal`, `CMakeLists.txt:23`), and
`src/framework/ui/graphical` - headers with no `CMakeLists.txt` that include this folder's
`nodes/vbox.h` and write the [[qor_desktopui]] token.
