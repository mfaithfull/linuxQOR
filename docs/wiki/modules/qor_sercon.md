---
Type: WikiPage
PageType: module
Title: "qor_sercon"
Target: qor_sercon
Status: current
LastVerified: 2026-11-17
Claims: 10
Summary: "A serial-console UI whose portable verbs are stubs - WriteLine and WriteChar have empty bodies - so its 475-line renderer writes into a no-op; nine of its thirteen sources are never compiled, and the class carries another module's export token."
Anchors:
  - { kind: in-tree, path: src/framework/ui/serial, symbol: qor_sercon, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/serial/CMakeLists.txt, symbol: qor_sercon, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/serial/serialconsole.h, symbol: SerialConsole, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/serial/renderer/renderer.h, symbol: Renderer, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/serial/compositor/compositor.h, symbol: Compositor, verified: 2026-11-17 }
  - { kind: in-tree, path: src/platform/os/windows/ui/serial/CMakeLists.txt, symbol: qor_sercon, verified: 2026-11-17 }
Judges:
  - cmake --build cmakebuild --target qor_sercon --parallel 8
---

# qor_sercon

"User interaction that can take place with nothing more than a bidirectional pipe for the code to
talk to." (`src/framework/ui/serial/purpose.md:1`), and "the concept is that this should operate
with the same backend models and ultimately have the same capability as a full graphical UI" (`:53`).
The folder is 28 files - 13 `.cpp` (1,422 lines), 13 headers (760 lines), a 15-line
`CMakeLists.txt` and that 32-line purpose - 2,229 lines, 69,369 bytes. It builds SHARED and is
configured (`src/framework/ui/CMakeLists.txt:3`). The index row compresses the purpose and ends with
a namespace, "ns qor::ui::con." (`docs/agent/purposes.md:59`); the purpose file has that fragment
alone on `:25`, and no file in the folder opens it.

- 2 of the 281 `CMakeLists.txt` under `src`, `test` and `examples` name the target, on 3 lines: its own
  (`src/framework/ui/serial/CMakeLists.txt:1`, `:3`) and the Windows sibling's link line
  (`src/platform/os/windows/ui/serial/CMakeLists.txt:13`). 0 test CMakeLists name it and 0
  directories under `test` are named `sercon*` or `serial*`. The link line -
  `qor_module qor_host qor_current qor_sync qor_injection` (`:15`) - is what
  `docs/agent/repo_map.md:72` prints, character for character; it does not name `qor_uicommon`,
  which owns the base of the biggest source in the target (see claim 4).
- Four of the folder's thirteen `.cpp` files are compiled; the other nine, 789 of the 1,422 `.cpp`
  lines, reach no target. `src/framework/ui/serial/CMakeLists.txt:3-5` names `qor_sercon.cpp`,
  `serialconsole.cpp`, `compositor/tokenbuffer.cpp` and `renderer/renderer.cpp` (633 lines between
  them). Not named: all eight `toolkit/*.cpp` (consoleview.cpp 244, consolemenuwidget.cpp 130,
  consolecolumnwidget.cpp 108, consoleviewcontroller.cpp 76, consoleviewmodel.cpp 68,
  consolelabelwidget.cpp 65, consolewidget.cpp 45, consoleui.cpp 23) and
  `compositor/compositor.cpp` (30). The judge log compiles exactly those four named sources and no
  others.
- The flagship class carries another module's token and the token is not a switch.
  `class qor_pp_module_interface(QOR_CONSOLE) SerialConsole : public app::IFeature`
  (`src/framework/ui/serial/serialconsole.h:18`) and its DI verb
  (`:13`) both write `QOR_CONSOLE`, a token this folder never defines and exactly one other target
  does - `src/components/ui/console/CMakeLists.txt:10`, target `qor_console`; the forward
  declaration of the same class at
  `src/framework/ui/serial/renderer/renderer.h:17` writes `QOR_SERIALCONSOLE`, which this folder
  does define, PRIVATE (`src/framework/ui/serial/CMakeLists.txt:10`). Neither choice changes the
  emitted linkage: `qor_pp_is_empty(_X)` tests emptiness, not definedness
  (`src/macros/facilities/is_empty.hpp:35-38`), so the non-empty arm of
  `qor_pp_if(qor_pp_is_empty(_X), qor_pp_import, qor_pp_export)`
  (`src/configuration/configuration.h:33`, `src/macros/control/if.hpp:24`) is always
  `qor_pp_export` - the dead switch the [[qor_platform]] page records, here across 4 marker sites
  (renderer.h 2, compositor.h 1, tokenbuffer.h 1) in a target whose name (`qor_sercon`) matches
  none of the tokens it uses.

- The pipe the purpose describes is empty at the portable end, and the one real computation ends
  there. `SerialConsole::WriteLine` is `void SerialConsole::WriteLine(const string_t& line){ }`
  (`src/framework/ui/serial/serialconsole.cpp:32`), `WriteChar` is
  `void SerialConsole::WriteChar(char_t c){ }` (`:39`), `ReadLine` returns `string_t()` (`:27-30`),
  `ReadChar` returns `0` (`:34-37`) and `Setup`/`Shutdown` are empty (`:23-25`). The target's largest
  source, `renderer/renderer.cpp` (475 lines), drives exactly those verbs: `Renderer::Present()`
  (`:346-364`) walks `m_buffer->GetHeight()` rows, builds each row with `CharLookup` and hands it to
  `m_display->WriteLine(lineToWrite)` (`:362`) - the empty body at `serialconsole.cpp:32`. Its base is
  `class Renderer` at `src/framework/ui/common/renderer.h:16`, unmarked (0 export markers), 34 lines,
  whose only verb `Frame(Scene*)` (`:23-29`) is five comment lines; `qor_sercon` does not link the
  target that owns it. `Compositor` (`compositor/compositor.h`, 29 lines, 1 marker) declares two
  `Compose` verbs (`:23-24`) defined only in `compositor/compositor.cpp` (30 lines), which no target
  compiles; the qualified name `sercon::Compositor` appears in 0 of the 3,204 sources outside this
  folder.
- `ImplementsSerialConsole()` is declared with one token and defined with another, and never in this
  DLL. `bool qor_pp_module_interface(QOR_CONSOLE) ImplementsSerialConsole();`
  (`src/framework/ui/serial/serialconsole.h:13`) has no definition in the target's four sources, and
  none in the built artifact: `qor_sercon.lib.exp` (11,157 bytes, from the 17 Nov 2026 build) lists the
  `SerialConsole` ctor `??0SerialConsole@ui@qor@@QEAA@XZ`, a `sercon::Renderer` ctor and
  `TokenBuffer::operator=` but no `ImplementsSerialConsole` and no `ThisModule`; the
  only definition is `qor_pp_module_provide(QOR_WINSERCON, SerialConsole)`
  (`src/platform/os/windows/ui/serial/sercon.cpp:17`), which expands to
  `bool qor_pp_module_interface(QOR_WINSERCON) ImplementsSerialConsole(){ return true; }`
  (`src/configuration/configuration.h:37`). The consumer verb `qor_pp_module_requires(SerialConsole)`
  (`examples/intermediate/serialui/main.cpp:31`) expands to a call of that function
  (`configuration.h:35`), so the requirement is met only through the sibling, which the example links
  at `examples/intermediate/serialui/CMakeLists.txt:20-21` - a link line that never names `qor_sercon`
  itself. The DI surface is `qor_pp_declare_factory_of(ui::SerialConsole, ExternalFactory)`
  (`serialconsole.h:40`) and `qor_pp_declare_guid_of(ui::SerialConsole, SerialConsoleGUID)` (`:42`,
  literal `:41`) with no instancer declaration; the sibling registers the pair by hand -
  `static qor::TypeRegEntry< qor::ui::win::SerCon, qor::ui::SerialConsole > regTerminal;`
  (`src/platform/os/windows/ui/serial/winq_sercon.cpp:22`), a `sercon` registration named
  `regTerminal`.
- Half the folder is a line-for-line copy of a component that does get built. `toolkit/` holds 15
  files and `src/components/ui/console` holds 29; 15 filenames pair across them and 13 of the pairs
  have identical line counts (`consoleview.cpp` 244/244, `consolecolumnwidget.cpp` 108/108). The two
  that have drifted are `consolemenuwidget.cpp` (130 lines here, 141 there) and
  `consoleviewcontroller.cpp` (76 lines each, contents differ).
  15 of the 26 code files here open `namespace qor { namespace components {`, while the one header a
  consumer includes opens `qor::ui` (`serialconsole.h:16`); 7 files open a `sercon` namespace
  (`renderer/renderer.h:19`) and the neighbouring module's renderer closes the wrong one -
  `}}}//qor::ui::sercon` at `src/framework/ui/terminal/renderer/renderer.cpp:32`.

## Failure modes

- The judge proves four files compile and says nothing about the other nine. The build linked
  `qor_sercon.vcxproj -> cmakebuild/src/framework/ui/serial/Debug/qor_sercon.dll` at exit 0 - 45 log
  lines, 13 warnings (C4100, C4244), 0 errors, 17 Nov 2026 - and the compile lines in that log are
  `qor_sercon.cpp`, `serialconsole.cpp`, `tokenbuffer.cpp`, `renderer.cpp`. Editing
  `toolkit/consoleview.cpp` (244 lines) or `compositor/compositor.cpp` (30) cannot break this judge;
  a rename in either is invisible until a target names the file.
- The portable class cannot display anything, and the shape that does the work is elsewhere. On this
  target alone, `Renderer::Present()` writes 25 rows into the empty `WriteLine`
  (`serialconsole.cpp:32`) and returns; the row reaches a screen only through the sibling's override,
  `void SerCon::WriteLine(const string_t& line){ std::wcout << line << std::endl; }`
  (`src/platform/os/windows/ui/serial/sercon.cpp:61-65`), whose console-API call is commented out one
  line below (`:64`). `winqor_sercon.dll` links `qor_sercon`
  (`src/platform/os/windows/ui/serial/CMakeLists.txt:13`) and registers the substitute
  (`winq_sercon.cpp:22`), so the only live path is Windows-only:
  `examples/intermediate/CMakeLists.txt:3-5` guards `add_subdirectory(serialui)` with
  `if(CMAKE_SYSTEM_NAME MATCHES Windows)` and the comment `#only on Windows at the moment` (`:4`). The
  Linux branch of that example (`examples/intermediate/serialui/CMakeLists.txt:15-16`) links
  `linqor_terminal linqor_thread linqor_filesystem` - no serial-console library at all, because there
  is no `src/platform/os/linux/ui/serial` and 0 `CMakeLists.txt` in the tree name `linqor_sercon`; the
  `qor_pp_module_requires(SerialConsole)` at `main.cpp:31` would have nothing to call.
- Two tokens in one sibling CMakeLists, and the one the sources use is defined nowhere.
  `src/platform/os/windows/ui/serial/CMakeLists.txt:8` defines `QOR_WINSERIALCONSOLE` and its sources
  write `QOR_WINSERCON` at `sercon.h:10`, `:14` and `sercon.cpp:17` - a token no `CMakeLists.txt` in
  the tree defines. Nothing fails, because the marker macro is not a definedness test
  (`src/macros/facilities/is_empty.hpp:35-38`), which also means the token cannot be used as a gate:
  the `#ifdef QOR_FILESYSTEM` shape the [[qor_filesystem]] page records at `filesystem.h:15-19` is
  unavailable here, and a guard written against `QOR_SERCON` or `QOR_WINSERIALCONSOLE` would key off
  a name no source reads.
- A framework class carries a leaf component's token. `QOR_CONSOLE` is defined by exactly one
  `CMakeLists.txt` in the tree - `src/components/ui/console/CMakeLists.txt:10`, target `qor_console`,
  the folder `docs/agent/repo_map.md:122` lists as a leaf component - and the layer table puts leaf
  components at 11 (`:142`) while `qor_sercon` sits at 8 (`:139`). The base
  `qor::ui::Renderer` (`src/framework/ui/common/renderer.h:16`) is unmarked and its owner
  `qor_uicommon` is absent from this link line, so every includer gets its own copy of the base and
  `Frame(Scene*)` (`:23-29`) stays a comment block. With 0 test CMakeLists naming the target, the
  only executable check of any of this is the Windows-only example.

## See also

[[qor_host]] [[qor_module]] [[qor_current]] [[qor_sync]] [[qor_injection]] [[qor_uicommon]]
[[qor_platform]] [[qor_desktopui]] [[qor_filesystem]] [[qor_uilayout]] [[qor_uiselector]]

Not yet paged: `src/components/ui/console` (target `qor_console` - the owner of the token this
target's flagship class carries, and the twin folder), `src/framework/ui/terminal`
(`qor_terminal`), and the OS sibling `src/platform/os/windows/ui/serial` (`winqor_sercon`).


