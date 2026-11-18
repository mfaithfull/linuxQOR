---
Type: WikiPage
PageType: module
Title: "qor_console"
Target: qor_console
Status: current
LastVerified: 2026-11-18
Claims: 34
Summary: "A component console UI that composes per-widget token buffers into WriteLine output; the component Console defaults to the interface no-op unless a platform console is linked, the visible ConsoleUI holder has no usable surface, and TokenBuffer frees its array storage with scalar delete on every reset."
Anchors:
  - { kind: in-tree, path: src/components/ui/console, symbol: qor_console, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/console/CMakeLists.txt, symbol: qor_console, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/console/qor_console.cpp, symbol: ThisModule, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/console/iconsole.h, symbol: IConsole, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/console/tokenbuffer.cpp, symbol: Reset, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/console/consoleview.cpp, symbol: ConsoleView, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_console --parallel 8
---

# qor_console

Judge GREEN: `cmake --build cmakebuild --target qor_console
--parallel 8` exits 0 after touching `consolemenuwidget.cpp`,
`consolerenderer.cpp` and `consoleview.cpp`, and links
`cmakebuild/src/components/ui/console/Debug/qor_console.dll`. The
console-folder warning is `consolemenuwidget.cpp:78`; another C4244
comes from `utility(274,54)` while instantiating a `char_t` from `int`.
`src/components/ui/CMakeLists.txt:2` adds the folder.

- **Folder:** 27 code files, 2033 physical lines, `CMakeLists.txt` 15,
  `qor_console.cpp` 14. The CMake target adds 13 sources
  (`CMakeLists.txt:3-5`) and links `qor_module qor_thread qor_sync
  qor_injection qor_role` (`:15`).
- **Purpose:** `purpose.md:1` says this is “just the std io console
  that could work over a serial link if required. No TTY specialization
  here.” The code is a token-buffer UI layer on top of that console.
- **License and guards:** all 27 code files carry BSL-1.0. Fourteen
  headers use three guard generations: two stale `FRAMEWORK` guards
  (`console.h:4`, `iconsole.h:4`), eleven `COMPONENTS_CONSOLE_*`
  guards, and one path-shaped guard at `tokenbuffer.h:5`.
- **Interface:** `IConsole` (`iconsole.h:15`) is not abstract:
  `Setup` and `Shutdown` are empty (`:22-23`), `ReadLine` returns an
  empty string (`:25-28`), `WriteLine` has an unused parameter (`:30`),
  `ReadChar` returns `0` (`:32-35`), and `WriteChar` does nothing
  (`:37`). It is registered as singleton/external (`:43-44`) with
  `IConsoleGUID` (`:45-46`).
- **Console:** `Console` inherits `app::IFeature` (`console.h:17`) and
  has its own GUID (`:39-40`). The constructor calls
  `new_ref<IConsole>()` (`console.cpp:20`); `ReadLine`, `WriteLine`,
  `ReadChar`, and `WriteChar` forward directly to that pimpl
  (`:23-40`).
- **Platform hook:** `console.h:12` declares `ImplementsIConsole()` as
  the linker-visible marker. Linux and Windows provide it at
  `src/platform/os/linux/ui/console/console.cpp:36` and
  `src/platform/os/windows/ui/wincon/console.cpp:17`; tests call it at
  `test/ui/terminal/primary.cpp:39` and `test/ui/terminal/tui.cpp:45`.
- **ConsoleUI:** `ConsoleUI` (`consoleui.h:14`) only has ctor/dtor and
  two private refs. Its ctor creates a `Console` and
  `ConsoleViewController` (`consoleui.cpp:15-19`), but the component
  type is referenced only by its own files; the similarly named
  `ConsoleUI` in `src/framework/ui/serial/toolkit/` is a different
  folder.
- **Controller:** `ConsoleViewController` constructs `Console`,
  `ConsoleView`, `ConsoleViewModel`, and `ConsoleCompositor`
  (`consoleviewcontroller.cpp:16-19`). Its `Render()` gets view size,
  lays out the model, renders widgets, composes, then renders the view
  (`:29-38`).
- **Input:** `GetUserInput()` reads a line, intercepts a backslash
  command at `consoleviewcontroller.cpp:46`, then only sets
  `handled = true` while a comment at `:48` admits the view-menu push
  is missing. Unhandled input goes to widgets (`:53-56`), and the
  application fallback at `:59-62` is empty.
- **Compositor:** `Compose(model, view)` iterates the model vector with
  the odd call form `model()()` at `consolecompositor.cpp:19`, then
  copies each widget buffer into the main buffer at `:27`.
- **Token buffer:** `TokenBuffer` starts at 80x25 (`tokenbuffer.cpp:12`)
  and calls `Reset` immediately (`:14`). `ConsoleView` owns two of them
  (`consoleview.h:41-42`), so one default view allocates 4000 ints.
- **Reset:** `Reset` stores the requested dimensions, then does
  `delete m_tokens` (`tokenbuffer.cpp:44`) and `new int[...]` (`:45`).
  The dtor also uses `delete m_tokens` (`:21`) on the same array
  allocation. No guard rejects zero or negative dimensions.
- **Access:** `GetToken` and `SetToken` range-check line and column
  (`tokenbuffer.cpp:59-74`), index as `(line * m_width) + column`, and
  silently return or drop for out-of-range coordinates. `Clear` uses
  `memset` with zero (`:76-79`), and `CopyFrom` clips by calling those
  accessors (`:81-90`).
- **View:** `ConsoleView` owns a double-buffer set and a renderer
  (`consoleview.h:41-43`); its ctor creates `ConsoleRenderer`
  (`consoleview.cpp:95`). `ConsoleRenderer` owns another `Console`
  (`consolerenderer.cpp:243`).
- **Size casts:** `SetWidth` and `SetHeight` pass unsigned arguments
  into `TokenBuffer` setters as `static_cast<int>`
  (`consoleview.cpp:112-119`). `GetWidth` and `GetHeight` return
  `abs`-cast values (`:102,107`), hiding a negative internal width or
  height from callers.
- **Enum order:** the enum in `tokenbuffer.h` runs from `BLANK = 0` at
  line `19` through `CHAR_255` near line `242`. The first letter
  enumerator, `CHAR_A`, is line `35` and therefore has value 16, not
  the ASCII code 65.
- **Lookup tables:** `ConsoleView::s_LookupToken` at
  `consoleview.cpp:14-80` contains 64 entries. `ConsoleRenderer`
  `s_charLookup` at `consolerenderer.cpp:13-239` contains 224 entries.
  The tables are not inverses: the renderer has an entry for almost
  every enumerator, while the view knows only a small character subset.
- **Renderer:** `CharLookup` returns `'?'` when an enumerator is not in
  the renderer table (`consolerenderer.cpp:254-264`). `Render()` emits
  a leading CRLF (`:273`), then walks the buffer and writes each row
  through `Console::WriteLine` (`:274-287`).
- **Draw helpers:** `LookupTokenForChar` returns `0` on a miss
  (`consoleview.cpp:82-90`), so `DrawCharacter` turns unknown input into
  blank rather than `QUESTION_MARK` (`:127-129`). `DrawLabel` also
  appends `COLON` at the end (`:192`).
- **Wrap rule:** `DrawString` draws a character first and only then
  tests `xpos > GetWidth()` (`consoleview.cpp:195-215`). A glyph at the
  last writable column is bounds-rejected before the position wraps.
- **Window geometry:** `DrawWindow` places corners at `x + width` and
  `y + height` (`consoleview.cpp:132-148`), and `ClearWindow`/
  `FillWindow` loop with `<=` (`:150-169`), so the helper uses an
  inclusive extent rather than the usual width/height count.
- **Widget base:** `ConsoleWidget` creates a full `ConsoleView` in its
  constructor (`consolewidget.cpp:19`), and its `Render()` is empty
  (`:38`). Its virtual layout/size methods return `{0,0}` and
  `HandleInput` returns false (`consolewidget.h:25-33`).
- **Model:** `ConsoleViewModel::Layout` forwards the same total width
  and height to every widget (`consoleviewmodel.cpp:15-21`) despite the
  comment at `consoleviewmodel.h:23` saying it positions and sizes
  items. The header still has `//RemoveWidget` (`:22`) and comments
  naming missing container widgets (`:27`).
- **Column:** `ConsoleColumnWidget::Layout` divides the incoming height
  by the child count (`consolecolumnwidget.cpp:22-35`), and `Render`
  sets its own view size, renders children, and composes each child
  buffer downward (`:89-104`).
- **Menu:** `ConsoleMenuWidget::GetSize` computes height as
  item count + 4 (`consolemenuwidget.cpp:47-59`). Its Windows unicode
  helper at `:77-81` returns an empty wide string for any input; the
  compiler selected that branch and warned about the ignored `str`
  parameter.
- **Exports:** the Debug dll has 145 exports. It exports interface and
  console classes, widget methods, and both static lookup maps.
- **Consumers:** 12 CMake lines name the exact target, 9 outside the
  folder. `examples/intermediate/client` and `test/ui/terminal` are
  configured by `examples/intermediate/CMakeLists.txt:1` and
  `test/ui/CMakeLists.txt:1`. No `add_subdirectory(curlike)` or
  `add_subdirectory(echoclient)` was found in the configured chain,
  and the windows experimental console example is commented at
  `examples/experimental/osspecific/windows/CMakeLists.txt:7`.
- **Index rows:** `docs/agent/repo_map.md:121` and
  `docs/agent/deps.dot:141` list the same five link libraries as
  `CMakeLists.txt:15`. The purposes row at `docs/agent/purposes.md:100`
  describes this folder plus a framework console UI.

## Failure modes
- A plain `Console` reads empty lines, returns `0`, and writes nothing
  unless a platform library supplies the real `IConsole` through the
  linker-visible `ImplementsIConsole()` hook.
- `TokenBuffer` calls scalar `delete` on memory allocated with `new
  int[...]` at `tokenbuffer.cpp:44-45`; this is reached by construction,
  resize, and destruction.
- A too-large unsigned width becomes a negative `int` width.
  `TokenBuffer::Reset` may allocate badly, and later `SetToken` range
  checks reject writes because `column < m_width` is false.
- `ConsoleUI` constructs a controller but exposes no way to run or
  inspect it; the only folder-local controller input command is also a
  no-op.
- The Windows menu helper discards formatted menu text and returns an
  empty string, so menu prefixes disappear even though the label drawing
  code runs.
- `ConsoleViewModel` does not position widgets; nested widgets can all
  draw into their own private views while the model only forwards the
  same viewport constraints.
