---
Type: Plan
Title: "Framework pages group 4"
Parent: "../plan.md"
Level: 3
Items:
  - Title: Write the qor_uilayout page
    Status: Atomic
    Content:
  - Title: Write the qor_uiselector page
    Status: Atomic
    Content:
  - Title: Write the qor_sercon page
    Status: Atomic
    Content:
  - Title: Write the qor_terminal page
    Status: Atomic
    Content:
  - Title: Write the qor_uiwidgets page
    Status: Atomic
    Content:
---

# Framework pages group 4

## Goal

Five steps of **one page each** closing the `src/framework` region: `qor_uilayout`,
`qor_uiselector`, `qor_sercon`, `qor_terminal`, `qor_uiwidgets`.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- Group 4 **CLOSED 17 Nov 2026** - all 5 pages landed (`qor_uilayout`, `qor_uiselector`,
  `qor_sercon`, `qor_terminal`, `qor_uiwidgets`) and leaf 5 printed this plan's close line - and the
  framework region's close line - exactly: `coverage = 43 of 90`, `uncovered_targets = 47`,
  `dangling_module_pages = 0`, `illegal_fields = 0` (`pages = 43 of 49`, `strays = 0`), `dead = 0`
  (`anchors = 190 of 190`), each `VERDICT fail=0`; `wiki-lint provenance` unchanged at
  `violations = 2`, both on pages this group did not touch.
  **CLOSED 17 Nov 2026**: `docs/wiki/modules/qor_uiwidgets.md` (113 lines, `Claims: 10` = 6 claims +
  4 failure modes, 6 anchors, judge green - exit 0 but an msbuild no-op: 0 compile lines, the dll
  stamped 17 Nov 2026 22:57:05 against a newest folder source of 15 Oct 2026 11:39:12) printed
  `coverage = 43 of 90`, `uncovered_targets = 47`, `illegal_fields = 0`, `dead = 0`. What the page
  found: its reach is one link line (`src/framework/ui/terminal/CMakeLists.txt:23`) and five
  `#include`s (the four `terminal/toolkit/` headers + `graphical/windows/window.h:7`), 0 test
  CMakeLists and 0 `new_ref<PushButton|Label|TopLevelContainer>` sites tree-wide; the built dll
  exports 78 symbols of which 21 are the folder's own - all `PushButton`, the one marked class - and
  57 are re-published inline definitions of [[qor_uilayout]] (52) and `Contained` (5);
  `OnSelected() {}//TODO: Raise a signal` (`pushbutton.h:26`) with no pushed state although
  `purpose.md:5` promises one; `TopLevelContainer` re-derives `Compound` directly (`tlc.h:12-14`),
  not this folder's `Widget`; and the only `: public Widget` use in the tree
  (`src/framework/ui/graphical/windows/window.h:14`) sits in a folder named by 0 CMakeLists - 158
  code lines no target compiles.
  **CLOSED 17 Nov 2026**: `docs/wiki/modules/qor_terminal.md` (122 lines, `Claims: 10`, 6 anchors,
  judge green - exit 0, msbuild no-op, `qor_terminal.dll` stamped 17 Nov 2026 22:57:18 vs newest
  source 15 Oct 2026 11:39:12) printed `coverage = 42 of 90`, `uncovered_targets = 48`,
  `illegal_fields = 0` (`pages = 42 of 48`), `dead = 0` (`anchors = 184 of 184`). What the page
  found: `CMakeLists.txt:4-13` compiles all 11 `.cpp` - the inventory agrees - and 55% of the 3,084
  `.cpp` lines sit in `renderer/string.cpp` (1,705); the `docs/agent/purposes.md:60` row describes
  `qor_tui`, not this target (all 55 src lines writing `FTXUI` are under `src/components/ui/terminal`;
  the row's `Display`/`SurfaceBuffer` declare nothing here); the folder runs two `tty` namespaces;
  the dll exports 96 symbols, 10 foreign; `ImplementsTerminal` is declared (`terminal.h:20`) and
  defined only by the Windows sibling (`termcon.cpp:17`), and the Linux example branch links
  `linqor_terminal`, which no target declaration defines; `Setup`/`Shutdown`/`SwapBuffers`/`Present`
  are empty bodies behind a GUID'd DI shape; `test/ui/terminal` tests the console libs, not this one.
  Leaf 3 **CLOSED 17 Nov 2026**: `docs/wiki/modules/qor_sercon.md` (154 lines, `Claims: 10` = 6 claims +
  4 failure modes, 6 anchors, judge green - the 4 named translation units compiled,
  `qor_sercon.dll` + `.lib` + `.exp` linked, exit 0, 13 warnings (C4100, C4244), 0 errors) and its stop
  condition printed exactly: `coverage = 41 of 90`, `uncovered_targets = 49`,
  `dangling_module_pages = 0`, `illegal_fields = 0` (`pages = 41 of 47`, `strays = 0`), `dead = 0`
  (`anchors = 178 of 178`), each `VERDICT fail=0`. `wiki-lint provenance` is unchanged at
  `violations = 2`, both on pages this leaf did not touch (`qor_task.md:45` github URL,
  `qor_signals.md:52` `token:` false positive).
  What the page found: `CMakeLists.txt:3-5` compiles 4 of the folder's 13 `.cpp` - the other 789 of
  1,422 `.cpp` lines (all eight `toolkit/*.cpp` plus `compositor/compositor.cpp`) are named by no
  target; the flagship class carries another module's token,
  `qor_pp_module_interface(QOR_CONSOLE) SerialConsole` (`serialconsole.h:18`, DI verb `:13`), a token
  exactly one `CMakeLists.txt` defines - `src/components/ui/console/CMakeLists.txt:10`, the leaf
  component `qor_console` - while the 4 sites that write the folder's own `QOR_SERIALCONSOLE` sit on
  `Renderer`, `TokenBuffer` and `Compositor`; `WriteLine`/`WriteChar` are empty bodies
  (`serialconsole.cpp:32`, `:39`) and the 475-line `renderer/renderer.cpp` drives exactly those
  (`Present()` `:346-364` hands 25 rows of an 80x25 buffer to `:362`), and the sibling's override prints
  with `std::wcout` while its console-API call is commented out (`sercon.cpp:63-64`);
  `ImplementsSerialConsole()` is declared with `QOR_CONSOLE` and defined only as
  `qor_pp_module_provide(QOR_WINSERCON, SerialConsole)` (`sercon.cpp:17`) - a token no `CMakeLists.txt`
  defines, and absent from the built `.exp`, which does publish the `SerialConsole` ctor; the sibling
  instead defines `QOR_WINSERIALCONSOLE`, which 0 sources read; 15 of the 26 code files here open
  `qor::components`, and `toolkit/` is a 15-file twin of `src/components/ui/console` with 13 pairs at
  identical line counts (`consolemenuwidget.cpp` 130 vs 141); 0 test CMakeLists and 0 `test`
  directories cover it, and the only executable consumer is Windows-only
  (`examples/intermediate/CMakeLists.txt:3-5`, `#only on Windows at the moment`) and links
  `winqor_sercon` without ever naming `qor_sercon`.
  **CLOSED 17 Nov 2026**: `docs/wiki/modules/qor_uiselector.md` (126 lines, `Claims: 10` = 6 claims +
  4 failure modes, 4 anchors, judge green - both TUs compiled, `qor_uiselector.dll` + `.lib` + `.exp`
  linked, exit 0, 0 warning/`LNK` lines) and its stop condition printed exactly: `coverage = 40 of 90`,
  `uncovered_targets = 50`, `dangling_module_pages = 0`, `illegal_fields = 0` (`pages = 40 of 46`,
  `strays = 0`), `dead = 0` (`anchors = 172 of 172`), each `VERDICT fail=0`. `wiki-lint provenance` is
  unchanged at `violations = 2`, both on pages this leaf did not touch (`qor_task.md:45` github URL,
  `qor_signals.md:52` `token:` false positive).
  What the page found: fan-in **0** - all 3 lines naming the target are in its own `CMakeLists.txt`,
  one of only 8 of the 90 `qor_*` targets no other `CMakeLists.txt` names, and 0 `test` directories
  name it; the `.exp` publishes exactly 5 symbols (ctor, dtor, vtable, `Setup`, `Shutdown`) and
  `uiselector.cpp` is four empty bodies that restate the inherited `{}` of the unmarked `IFeature`
  (`ifeature.h:22-23`); `uiselector.h:36-41` is byte-identical to `desktopui.h`'s comment block
  including the "tookit" misspelling, so the specification describes a desktop UI, and the folder
  names no UI it could select (`DesktopUI`/`Terminal`/`SerialConsole` 0 hits);
  `ImplementsUISelector()` (`uiselector.h:17`) is 1 occurrence tree-wide with no
  `qor_pp_module_provide` anywhere, so the first `qor_pp_module_requires(UISelector)` cannot link;
  and the class carries a GUID (`uiselector.h:47-48`), so `Role::AddFeature` (`role.cpp:98`, GUID
  guard `:102`) accepts it and `TrySetup` (`role.cpp:29-44`) can never be reached by an empty
  `Setup()` - a registered, healthy-looking selector that selects nothing.
- Leaf 1 **CLOSED 17 Nov 2026**: `docs/wiki/modules/qor_uilayout.md` (153 lines, `Claims: 10` =
  6 claims + 4 failure modes, 7 anchors, judge green - 7 translation units compiled,
  `qor_uilayout.dll` + `.lib` + `.exp` linked, exit 0) and its stop condition printed exactly:
  `coverage = 39 of 90`, `uncovered_targets = 51`, `dangling_module_pages = 0`,
  `illegal_fields = 0` (`pages = 39 of 45`, `strays = 0`), `dead = 0` (`anchors = 168 of 168`),
  each `VERDICT fail=0`. `wiki-lint provenance` is unchanged at `violations = 2`, both on pages this
  leaf did not touch (`qor_task.md:45` github URL, `qor_signals.md:52` `token:` false positive).
  What the page found: `CMakeLists.txt:4-10` compiles 7 of the folder's 14 `.cpp` - 440 of 1,652
  lines, the other 1,212 lines named by no target; the compiled classes `Arranger`
  (`arranger.h:16`) and `IArrangeable` (`arrangeable.h:17`) carry **0** export markers while the
  folder's whole token is 10 occurrences over 4 files; `flex.h` calls `T::ComputeRequirment()` (no
  `e`) at its 10 template sites `:40, :58, :74, :90, :106, :121, :136, :152, :167, :182`, so none of
  its 10 decorators can be instantiated; `GridBox` is declared twice in `qor::ui::layout`
  (`gridbox.h:11` on `Arranger`, `nodes/gridbox.h:31` on `Node`) and the `WidthOrHeight`/`Constraint`
  enums twice (`sizeconstrainer.h:14-15`, `functional/layout.h:21-22`), so the folder's two halves
  cannot both be built; and `IArrangeable::ComputeRequirement` reads `compound->ChildAt(0)`
  (`arrangeable.h:41`) where `Compound::ChildAt` guards with `assert` only (`compound.h:25`).
- **Read the enclosing region before calling anything a compile landmine.** `focused` appeared on 12
  lines in three uncompiled files and looked like the reason they are uncompiled; all 12
  are inside `/* */` (`node.cpp:44-49`, `functional/flexbox.cpp:148-161`,
  `nodes/gridbox.cpp:63-76`) - the feature (`requirement.h:28-56`) is commented out everywhere.
- **Folder inventory against the `add_library` source list is the cheapest high-yield probe**: it
  found the 1,212 uncompiled lines here in one pass, and it is what makes "the marked half is the
  uncompiled half" checkable.
- **Two `qor_*terminal` targets exist**: `qor_terminal` here
  (`src/framework/ui/terminal`) and `qor_tui` at `src/components/ui/terminal`. A leaf that reads the
  folder name picks the wrong one; the leaf names its target and its CMakeLists and nothing else.

## Context

Inherited from the parent (`../plan.md`, Level 2) and grandparent (`../../plan.md`, Level 1): one
page per build target in `docs/wiki/modules/`; `Judges:` is that target's own build command; the
target list was verified once by `Reconcile the target list`. **`../plan.md` §Decisions holds the
page contract - shape, read budget, judge rule, stop pair - and binds every leaf here.** Sibling
titles: `FrameworkPagesGroup1`, `FrameworkPagesGroup2`, `FrameworkPagesGroup3`.

## Decisions

- **This is the tail of the region, so it carries the region's close line** - `coverage = 43 of 90`
  is the same number the pre-refinement slice 3 claimed. The re-cut moved boundaries, not totals;
  `tmp/_tmp_llmwiki_page_cut.ps1` -> `.txt` proves all 26 framework targets are still owned once.

## Write the qor_uilayout page

Input verified: `FrameworkPagesGroup3` closed on `coverage = 38 of 90`.

One page: `docs/wiki/modules/qor_uilayout.md` for target `qor_uilayout`
(`src/framework/ui/layout/CMakeLists.txt` - folder `layout`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `wiki-census coverage` re-reads `coverage = 39 of 90` with
`uncovered_targets = 51`, `wiki-lint schema` prints `illegal_fields = 0`,
`wiki-ref-check anchors` prints `dead = 0`.

## Write the qor_uiselector page

Input verified: `coverage = 39 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_uiselector.md` for target `qor_uiselector`
(`src/framework/ui/selector/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 40 of 90`, `uncovered_targets = 50`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_sercon page

Input verified: `coverage = 40 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_sercon.md` for target `qor_sercon`
(`src/framework/ui/serial/CMakeLists.txt` - folder `serial`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 41 of 90`, `uncovered_targets = 49`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_terminal page

Input verified: `coverage = 41 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_terminal.md` for target `qor_terminal`
(`src/framework/ui/terminal/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md). `qor_tui` (components) is a different target with the same folder name
- this page is about the framework one, and says so once.

Stop condition: `coverage = 42 of 90`, `uncovered_targets = 48`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_uiwidgets page

Input verified: `coverage = 42 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_uiwidgets.md` for target `qor_uiwidgets`
(`src/framework/ui/widgets/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 43 of 90`, `uncovered_targets = 47`, `illegal_fields = 0`, `dead = 0` -
the framework region's close line, and the number `ComponentsPagesGroup1` opens on.
