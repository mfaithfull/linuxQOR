---
Type: WikiPage
PageType: module
Title: "qor_gl"
Target: qor_gl
Status: current
LastVerified: 2026-11-17
Claims: 11
Summary: "The largest unit in src/framework - 7,867 lines and 29 headers - with a judge that cannot run, because no CMakeLists.txt in the tree descends into the folder."
Anchors:
  - { kind: in-tree, path: src/framework/ui/gl/CMakeLists.txt, symbol: qor_gl, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/CMakeLists.txt, symbol: add_subdirectory, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/gl/session.h, symbol: TheSession, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/gl/session.cpp, symbol: TheSession, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/gl/iplatform.h, symbol: IPlatform, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/ui/gl/window/windowpimpl.cpp, symbol: WindowPimpl::Create, verified: 2026-11-17 }
  - { kind: in-tree, path: src/platform/os/windows/ui/gl/CMakeLists.txt, symbol: winqor_gl, verified: 2026-11-17 }
Judges:
  - cmake --build cmakebuild --target qor_gl --parallel 8
---

# qor_gl

"GL (OpenGL) support base for graphical UI." (`docs/agent/purposes.md:56`). There is no `purpose.md`
in the folder - 28 exist under `src/framework` and this one has none - so the index line is one of
the ones that file's own header admits inferring: `"—" = no dedicated purpose.md; intent inferred
from subsystem purpose.md.` (`docs/agent/purposes.md:4`). It is the biggest target in this plan's
slice: 15 `.cpp` (5,007 lines) and 29 headers (2,860 lines), 7,867 lines across 44 files.
`src/framework/ui/gl/CMakeLists.txt` is 28 lines: it names all 15 `.cpp` at `:4-18`, defines `QOR_GL`
PRIVATE at `:23`, and links four libraries at `:28` - the four `docs/agent/repo_map.md:69` prints
(`qor_module qor_host qor_thread qor_error`), one of the few rows where the index and the link line
agree. Nothing configures it.

- No `CMakeLists.txt` in the tree descends into the folder. `src/framework/ui/CMakeLists.txt` is 7
  lines - `common`, `desktop`, `serial`, `selector`, `layout`, `terminal`, `widgets` (`:1-7`) - and
  `gl` is not among them, not even commented. The only `add_subdirectory(gl)` in the tree is
  `src/platform/os/windows/ui/CMakeLists.txt:4` and it reads `#add_subdirectory(gl)`. What that
  leaves on disk: `cmakebuild` holds 306 `.vcxproj` and 0 of them is `qor_gl.vcxproj`; its
  `CMakeCache.txt` has 0 mentions of `framework/ui/gl`; and `Test-Path cmakebuild/src/framework/ui/gl`
  is False. `wiki-census` counts the target anyway, because it enumerates `set(target qor_`
  declarations rather than configured ones (`memory-bank/tools/wiki-census/tool.md:81` - "208
  CMakeLists.txt, 91 `set(target qor_` declarations, 90 distinct"), so one of the 90 targets the wiki
  is bounded by has never been compiled.
- The two largest files in the target are two of the three largest `.cpp` under `src/framework`:
  `input/input.cpp` (1,606 lines, 46,724 bytes) and `window/window.cpp` (1,174 lines, 35,712 bytes),
  behind only `ui/terminal/renderer/string.cpp` (65,171 bytes). Both run the same seam -
  `window.cpp:151` `if (!TheSession().GetPlatform().CreateWindow(window, &wndconfig, &ctxconfig,
  &fbconfig))` and `input.cpp:120` `TheSession().GetPlatform().GetCursorPos(window,
  &window->virtualCursorPosX, &window->virtualCursorPosY);` - so the code that carries half the
  target's bulk is a dispatcher over a `GetPlatform()` that no compiled library implements.
- The interface is a GLFW port that stopped mid-port. `iplatform.h` (164 lines) forward-declares 11
  marked types (`:37-47`) and opens `class qor_pp_module_interface(QOR_GL) IPlatform` (`:49`) with
  ~60 pure virtuals in GLFW's shape - `GetCursorPos` (`:58`), `CreateStandardCursor` (`:64`),
  `GetVideoModes` (`:82`), `SetWindowMousePassthrough` (`:120`), `WaitEventsTimeout` (`:123`),
  `PostEmptyEvent` (`:124`) - with the EGL block commented at `:126-128` and the Vulkan half at
  `:130-132`. `session.cpp:53` is the port's own verdict,
  `//TODO: Implement anything from GLFWAPI int glfwInit(void) that we actually need`, and the ctor's
  one statement is `DefaultWindowHints();` (`:54`). `constants.h` (289 lines, 237 `constexpr`) is
  GLFW's values with the `GLFW_` prefix removed (`ANY_PLATFORM = 0x00060000` at `:30`), 16 of them
  inside a comment at `:58-73`. 17 `GLFW` mentions over 4 files. `iplatform.h` is entered from
  exactly one place in the tree: `platformimpl.h:34`.
- The session is one exported global with two definitions. `session.h:310` declares
  `extern qor_pp_module_interface(QOR_GL) Session& TheSession(void);` and the lines that mention it
  number 276 (`TheSession` as a substring, `Select-String` count):
  178 inside this folder (`input.cpp:44`, `context.cpp:49`, `window.cpp:43`...), 96 in the Windows
  sibling and 2 in `examples/experimental/glgears/glgears.cpp:278` and `:325`. Two files define it:
  `session.cpp:241-244` (`static Session _theSession; return _theSession;`) and
  `src/platform/os/windows/ui/gl/session.cpp:201-204`, which repeats the same function-local static
  under the same name. Neither definition currently reaches a target - the portable one because this
  target is unconfigured, the sibling's because `src/platform/os/windows/ui/gl/CMakeLists.txt:3-7`
  compiles 4 of that folder's 10 `.cpp` - `winqor_gl.cpp`, `cursorimpl.cpp`, `windowimpl.cpp`,
  `wgl.cpp` (`:3-7`) - and `session.cpp` is not one of them, nor are `platform.cpp`,
  `monitorimpl.cpp`, `joystickimpl.cpp`, `helperwindow.cpp` or `wglcontextimpl.cpp`.
  Per-thread state is no better: `tls.h:30-36` is a `TLS` class with two defaulted functions and no
  data, `session.h:195` holds one as `TLS contextSlot;`, and `context.cpp:49` reads the window back
  out of it through `PlatformGetTls(&TheSession().contextSlot)`.
- 12 of the 29 headers are named by none of the 15 `.cpp` the target compiles: `constants.h`,
  `iplatform.h`, `tls.h`, `input/mappings.h`, `monitor/vidmode.h`, `window/wndconfig.h` and the six
  `iplatform*.h` interfaces (`window/iplatformwindow.h`, `context/iplatformcontext.h`,
  `cursor/iplatformcursor.h`, `input/iplatforminput.h`, `input/iplatformjoystick.h`,
  `monitor/iplatformmonitor.h`). They are reached only header-to-header - `session.h:32-33` pulls
  `tls.h` and `constants.h`, `input/input.h:32` pulls `mappings.h`,
  `window/iplatformwindow.h` is pulled by `iplatform.h:33`. Two of them are stubs with no consumer:
  `input/mappings.h` is 10 lines whose only content is one SDL joystick string ending in a comma
  (`:5-9`), and `monitor/vidmode.h` is named by no file outside the folder
  (`src/platform/os/linux/ui/x/xf86vidmode/xf86vidmode.cpp:11` names its own `xf86vidmode.h`).
- The token is this folder's name and 62 of its 64 tree-wide occurrences are the export marker:
  `iplatform.h` 12, `window/iplatformwindow.h` 7, `input/iplatforminput.h` 5, `session.h` 4,
  `monitor/iplatformmonitor.h` 4 over 27 headers, plus the `PRIVATE` definition at
  `CMakeLists.txt:23`. The 64th is not C++ at all - the Windows sibling sets
  `wc.lpszClassName = L"QOR_GL";` (`src/platform/os/windows/ui/gl/windowimpl.cpp`), a Win32 window
  class name wearing the token, so any token sweep across the tree reads 64 and means 63.
- The portable behaviour is a not-implemented report. `window/windowpimpl.cpp` and
  `monitor/monitorpimpl.cpp` hold 45 copies of `continuable("Override not implemented for the current
  platform")` (38 and 7): `WindowPimpl::Create` (`windowpimpl.cpp:43-48`) reports it and returns
  false, `Destroy` (`:50-54`) and `SetTitle` (`:56-60`) report it and fall through. The real
  implementations are registered by the sibling over the pimpl seam -
  `src/platform/os/windows/ui/gl/winqor_gl.cpp:46` `TypeRegEntryWithParams< qor::framework::win::gl::
  WindowImpl, qor::gl::WindowPimpl, qor::gl::Window*, const qor::gl::Wndconfig&,
  const qor::gl::Ctxconfig&, const qor::gl::FBconfig& > regWindowPimpl;` - and that sibling links
  this target at `src/platform/os/windows/ui/gl/CMakeLists.txt:17`, so both halves of the seam sit
  behind two `#add_subdirectory` comments.

## Failure modes

- The judge does not run. `cmake --build cmakebuild --target qor_gl --parallel 8` printed
  `MSBUILD : error MSB1009: Project file does not exist. Switch: qor_gl.vcxproj` at exit 1 on
  17 Nov 2026 - the page's `Judges:` row is a command whose only output is that error, and this is
  the only leaf in the group whose target has never been compiled. Nothing is in the way of the fix:
  `src/framework/ui/CMakeLists.txt:1-7` has no commented `gl` row to uncomment, the row has to be
  written. There is also no instrument - no `CMakeLists.txt` under `test` names `qor_gl`, and
  `test/ui` holds one directory (`terminal`), so 7,867 lines are covered by neither a build nor a test.
- Un-commenting the sibling first fails at its own includes. 11 `#include "src/framework/gl/..."`
  lines across 10 files in `src/platform/os/windows/ui/gl/` - `session.h:28`, `platform.h:28`,
  `platform.cpp:26`, `monitorimpl.h:28` and `:29`, `contextimpl.h:28`, `cursorimpl.h:28`,
  `cursorimpl.cpp:26`, `joystickimpl.h:28`, `joystickimpl.cpp:26`, `monitorimpl.cpp:26` - name
  `src/framework/gl`, and `Test-Path src/framework/gl` is False; the live prefix is
  `src/framework/ui/gl`. Two files in the same folder already spell it correctly
  (`windowimpl.cpp:32-33`, `windowimpl.h:28-29`), so both spellings sit side by side - and the dead
  spelling is in code the sibling would compile: `cursorimpl.cpp:26` pulls
  `src/framework/gl/session.h` and `cursorimpl.h:28` pulls `src/framework/gl/cursor/cursorpimpl.h`,
  and `cursorimpl.cpp` is one of the four sources at
  `src/platform/os/windows/ui/gl/CMakeLists.txt:4`.
- The sibling assumes portable state that was never added. Sibling `session.cpp:62` and `:76` call
  `TheSession().ntdll.RtlVerifyVersionInfo_(&osvi, mask, cond)`; `ntdll` occurs 3 times in the
  sibling folder and 0 times in this one, and `qor::gl::Session` (`session.h:119`) declares no such
  member. Fix the include paths and that call is still unresolved, so the sibling cannot be enabled
  without first widening the portable struct.
- Two `TheSession()` definitions are waiting for both targets to build: this target's
  `session.cpp:241-244` and the sibling's `session.cpp:201-204`, each with its own function-local
  `static Session _theSession;`. The sibling's definition carries none of the
  `qor_pp_module_interface(QOR_GL)` marker from `session.h:310`, and the sibling links `qor_gl`
  (`src/platform/os/windows/ui/gl/CMakeLists.txt:17`) - so once both build there are two `Session`
  objects holding per-object state (`session.h:169 initialized`, `session.h:195 contextSlot`), with
  the 178 in-folder gates such as `input.cpp:44`
  (`if (!TheSession().initialized) { Session::InputError(0x00010001, ""); return; }`) reading the
  `qor_gl` copy and the sibling's 96 call sites reading its own.

## See also

[[qor_uicommon]] [[qor_host]] [[qor_module]] [[qor_thread]] [[qor_error]] [[qor_desktopui]]

Not yet paged: the Windows sibling `src/platform/os/windows/ui/gl` (target `winqor_gl`, 21 headers
and sources plus its own `CMakeLists.txt`, not one of the 90 `qor_*` units), the only consumer
`examples/experimental/glgears` (commented out at `examples/experimental/CMakeLists.txt:7`, links
`qor_gl` at `examples/experimental/glgears/CMakeLists.txt:14`), and the `src/components/ui/egl`
folder, which carries its own `constants.h` rather than this one.
