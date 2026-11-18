---
Type: WikiPage
PageType: module
Title: "qor_terminal"
Target: qor_terminal
Status: current
LastVerified: 2026-11-17
Claims: 10
Summary: "The framework TUI target: every one of its 11 sources compiles, but Setup, Shutdown, SwapBuffers and Present are empty bodies, the only definition of ImplementsTerminal sits in a Windows sibling, and the FTXUI-derived index row describes qor_tui, not this target."
Anchors:
  - { kind: in-tree, path: src/framework/ui/terminal, symbol: qor_terminal, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/terminal/CMakeLists.txt, symbol: qor_terminal, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/terminal/terminal.h, symbol: Terminal, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/terminal/renderer/renderer.h, symbol: Renderer, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/terminal/renderer/surface.h, symbol: Surface, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/terminal/toolkit/tlc.h, symbol: TopLevelContainer, verified: 2026-11-17 }
Judges:
  - cmake --build cmakebuild --target qor_terminal --parallel 8
---

# qor_terminal

"Intends to provide a full TUI. Probably initially derived from FTXUI but we'll need to deviate
significantly to maintain minimal UI and graphical UI compatability" (`src/framework/ui/terminal/purpose.md:1-2`),
with "The ncurses look and feel is still probably the cleanest" as the stated direction (`:3`).
The folder is 26 files - 11 `.cpp` (3,084 lines), 13 headers (865 lines), a 23-line `CMakeLists.txt`
and that 14-line purpose - 148,880 bytes. It builds SHARED and is configured
(`src/framework/ui/CMakeLists.txt:6`). **This page is about the framework target `qor_terminal`;
`qor_tui` at `src/components/ui/terminal/CMakeLists.txt:1` is a different target in a folder with the
same name** - claim 3 says which one the index row actually describes.

- Exactly 4 `CMakeLists.txt` name the target: its own (`src/framework/ui/terminal/CMakeLists.txt:2`),
  the Windows sibling's link line (`src/platform/os/windows/ui/terminal/CMakeLists.txt:13`) and the
  two example link lines (`examples/intermediate/terminal/CMakeLists.txt:12`,
  `examples/intermediate/serialui/CMakeLists.txt:12`). 0 test CMakeLists name it. The link line
  `qor_module qor_host qor_current qor_uilayout qor_uicommon qor_uiwidgets`
  (`src/framework/ui/terminal/CMakeLists.txt:23`) is what `docs/agent/repo_map.md:73` prints,
  character for character, and it covers both bases the folder needs - `qor_uicommon` owns
  `qor::ui::Renderer` (`renderer/renderer.h:19`) and `qor_uiwidgets` owns the widget bases
  (`toolkit/tlc.h:11`). The layer table lists the target at 9 (`docs/agent/repo_map.md:140`).
- The folder inventory and the source list agree: `CMakeLists.txt:4-13` names all 11 of the folder's
  `.cpp`, so unlike [[qor_sercon]] (4 of 13 compiled) nothing here hides from the judge. The
  centre of mass is one file: `renderer/string.cpp` is 1,705 lines of the 3,084 `.cpp` lines (55%)
  and 65,171 of the 147,688 code bytes, and `renderer/colourinfo.cpp` carries another 282 lines
  (21,623 bytes) behind the one-line declaration pair at `renderer/colourinfo.h:24-25`.
- The index row's provenance describes the sibling, not this target. `docs/agent/purposes.md:60`
  says "Full TUI (FTXUI-derived but deviates for UI/gfx compat)", repeating the folder's own class
  list (`purpose.md:5-11`); 0 code lines in this folder write the string `FTXUI` (only
  `purpose.md:2`), while all 55 src code lines that write it sit under
  `src/components/ui/terminal` - the `qor_tui` target. The one FTXUI artifact here is the
  `ComputeColourSupport` env probe (`src/framework/ui/terminal/terminal.cpp:54-78`) carrying the
  upstream comment about microsoft/terminal issue 1040 (`:68-71`). And the claimed
  class list itself is FTXUI's: of its five names, `Display` and `SurfaceBuffer` declare nothing in
  the folder (the only `Display` in code is the ctor parameter `Terminal* display` at
  `renderer/renderer.h:23`), and the folder declares `Surface` instead (`renderer/surface.h:16`).
- The folder runs two `tty` namespaces. 15 code files open `qor::ui::tty` (the renderer side) and 6
  open `qor::ui::widgets::tty` (`toolkit/label.h:9`, `pushbutton.h:9`, `tlc.h:9` and their three
  `.cpp`) - the built DLL mangles them apart (`?Present@Renderer@tty@ui@qor` vs
  `??0Label@tty@widgets@ui@qor`), and from inside plain `tty` the factory verb must reach across -
  `new_ref<ui::widgets::tty::TopLevelContainer>()` (`toolkit/toolkit.h:23`). The copy seams show:
  `renderer/renderer.cpp:32` closes `}}}//qor::ui::sercon` - the same line the [[qor_sercon]] page
  records from the other side - and `tlc.h:4` carries the "TOOKIT" misspelling in its header guard.
- Marker coverage is complete except for one class, and the export table leaks other modules' classes.
  15 `qor_pp_module_interface(QOR_TERMINAL)` sites sit in 10 files (`terminal.h:20`, `:30`, `:60`;
  `renderer/colour.h:12`; `renderer/colourinfo.cpp:272`, `:277`; `renderer/colourinfo.h:24`, `:25`;
  `renderer/image.h:15`; `renderer/renderer.h:15`, `:19`; `renderer/surface.h:16`;
  `toolkit/label.h:11`, `pushbutton.h:11`, `tlc.h:11`); `QOR_TERMINAL` is defined PRIVATE by exactly
  one CMakeLists, `CMakeLists.txt:18`. `class Toolkit : public ui::Toolkit` (`toolkit/toolkit.h:13`)
  is the one unmarked class and exports nothing. The built DLL exports 96 symbols; 10 belong to
  classes of the three linked UI modules - `Box` of [[qor_uilayout]] (`layout/box.h:9`), `Contained`
  of [[qor_uicommon]] (`common/contained.h:19`), the un-tty `PushButton` of `qor_uiwidgets` -
  the always-export marker switch the [[qor_platform]] page records, re-leased through the link
  line - and the table contains no `ImplementsTerminal` and no `ThisModule`, though
  `ThisModule` is defined unmarked at `qor_terminal.cpp:10`.
- The display half is empty verbs. `Terminal::Setup(){ }` (`terminal.cpp:16`) and `Shutdown(){ }`
  (`:18`) are the feature verbs; `Size()` returns the literal `Dimensions{80,24}` (`:20-23`) and
  `Full()` just calls it (`:30-33`); `GetRenderer()` news a fresh `tty::Renderer` on every call
  (`:80-83`). On the renderer, `SwapBuffers()` and `Present()` are empty bodies
  (`renderer/renderer.cpp:19-25`), and the ctor (`:15`) initializes `m_display` and `m_buffer` but
  not `m_currentSurface`, which `GetSurface()` returns as-is (`:27-30`). The 1,705-line string
  engine and the 531-line `Surface` have no live path out of the target.

## Failure modes

- The judge is green but proves nothing today. `cmake --build cmakebuild --target qor_terminal
  --parallel 8` (17 Nov 2026) exited 0 with an 11-line log, 0 compile lines, 0 warnings - msbuild
  judged the target up to date; `qor_terminal.dll` (1,318,912 bytes) is stamped 17 Nov 2026
  22:57:18 against a newest folder source of 15 Oct 2026 11:39:12. The Judges line re-checks
  stamps, not compiles; a source edit would make it mean something again.
- `ImplementsTerminal()` is declared (`terminal.h:20`) and defined by no source in this target; the
  only `qor_pp_module_provide` for it in the tree is `qor_pp_module_provide(QOR_WINTERMINAL,
  Terminal)` (`src/platform/os/windows/ui/terminal/termcon.cpp:17`), the Windows sibling. The
  requirement is live - `examples/intermediate/terminal/termapprole.cpp:19` writes
  `qor_pp_module_requires(Terminal)` - and satisfied on Windows only because `termapp` links
  `winqor_terminal` (`examples/intermediate/terminal/CMakeLists.txt:21`). Its Linux branch (`:16`)
  links `linqor_terminal`, which none of the 37 `set(target linqor_...)` declarations in the tree
  defines - `src/platform/os/linux/ui` has `termscreen`, not `terminal`. The dangling branch is
  unreachable only because both examples are gated by `if(CMAKE_SYSTEM_NAME MATCHES Windows)` with
  `#only on Windows at the moment` (`examples/intermediate/CMakeLists.txt:3-6`).
- The DI shape makes the emptiness invisible. `qor_pp_declare_factory_of(ui::Terminal,
  ExternalFactory)` (`terminal.h:78`) and the GUID literal (`:79-80`) mean `Role::AddFeature`
  accepts the class, and it is used: `IRole::AddFeature<qor::ui::Terminal>()`
  (`examples/intermediate/terminal/termapprole.cpp:54`) and
  `static qor::TypeRegEntry< qor::ui::win::Terminal, qor::ui::Terminal > regTerminal;`
  (`src/platform/os/windows/ui/terminal/winq_terminal.cpp:22`). A role that sets up this feature
  gets `Setup(){ }`, a `Size()` of 80x24 and a `Present()` that does nothing - the shape the
  [[qor_uiselector]] page records, but here with real consumers.
- The only test folder named terminal tests the console, not this target. `test/ui/terminal`
  builds `QORTermTests` (`test/ui/terminal/CMakeLists.txt:2`, `:11-13`) from 81+41+155 lines and
  links `qor_console` (`:21`), `linqor_console` (`:25`) and `winqor_console` (`:30`) - no
  `qor_terminal`, no `winqor_terminal`, no `qor_tui`. 0 CMakeLists under `test` name this target,
  so no automated run ever calls `Present()`.

## See also

[[qor_module]] [[qor_host]] [[qor_current]] [[qor_uilayout]] [[qor_uicommon]] [[qor_sercon]]
[[qor_uiselector]] [[qor_desktopui]] [[qor_platform]]

Not yet paged: `src/framework/ui/widgets` (target `qor_uiwidgets` - the owner of every widget base
this target subclasses), `src/components/ui/terminal` (target `qor_tui` - the FTXUI-derived sibling
with the same folder name), and the OS sibling `src/platform/os/windows/ui/terminal`
(`winqor_terminal`).
