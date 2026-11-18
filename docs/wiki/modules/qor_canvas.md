---
Type: WikiPage
PageType: module
Title: "qor_canvas"
Target: qor_canvas
Status: current
LastVerified: 2026-11-18
Claims: 27
Summary: "A thin GL canvas wrapper: hand a renderer canvas some Paint objects, ask it to draw. The window that would show it was commented out, so Show() and Refresh() dereference a null window, the only consumer is commented out of its band and includes a header path that no longer exists, and 48 of the 60 exports of the built dll belong to other libraries."
Anchors:
  - { kind: in-tree, path: src/components/ui/canvas, symbol: qor_canvas, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/canvas/CMakeLists.txt, symbol: qor_canvas, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/canvas/canvas.h, symbol: Canvas, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/canvas/canvas.cpp, symbol: Canvas::Resize, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/canvas/canvas.cpp, symbol: Canvas::Show, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/canvas/qor_canvas.cpp, symbol: ThisModule, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_canvas --parallel 8
---

# qor_canvas

Judge GREEN: `cmake --build cmakebuild --target qor_canvas
--parallel 8` links
`cmakebuild/src/components/ui/canvas/Debug/qor_canvas.dll`; no warning
or error line names `canvas.cpp`, `canvas.h` or `qor_canvas.cpp`.
`src/components/ui/CMakeLists.txt:1` adds the folder - it is the first
UI component configured.

- **Folder:** 3 sources + build file, 230 physical lines -
  `CMakeLists.txt` 14, `qor_canvas.cpp` 35, `canvas.h` 73,
  `canvas.cpp` 108.
- **What it is:** `class qor_pp_module_interface(QOR_CANVAS) Canvas`
  (`canvas.h:35`) in namespace `qor::components` (`:33`, closed `:70`),
  `typedef ref_of<Canvas>::type ref` (`:39`), defaulted virtual dtor
  (`:42`). The class name collides with its own member's type: it holds
  a `ui::renderer::Canvas*` (`:59`).
- **The API is 9 methods** (`canvas.h:44-53`): `Draw` `Prepare` `Show`
  `Resize` `Refresh`, then `Push` `Remove` `Update` `Viewport` returning
  `ui::renderer::Result`. The last four are pass-throughs - each is one
  `return m_canvas->...` (`canvas.cpp:88-106`).
- **Ownership is mixed:** `m_eglContext` and `m_window` are
  `qor::ref_of<...>::type` (`canvas.h:57-58`) but `m_canvas` is a bare
  pointer initialised `nullptr` (`:59`) - the surface it draws into is
  owned elsewhere and not counted here.
- **The ctor reaches through the running application**
  (`canvas.cpp:38-41`): `AppBuilder().TheApplication()->`
  `GetRole(qor_shared)->GetFeature(&OpenGLESFeatureGUID).`
  `AsRef<qor::components::OpenGLESFeature>()`, then `opengles->Context()`
  at `:43`. No hop is checked.
- `qor_shared` in that chain is not an identifier: `#define qor_shared )(`
  (`src/qor/memory/reference/sharedref.h:9`) - a paren-rewriting macro
  from the memory region, used as written at `canvas.cpp:39`.
- **The window was removed, the presentation calls were not:** the only
  line that ever assigns `m_window` is commented out -
  `//m_window = opengles->CreateWindow("Canvas", 1280, 800);`
  (`canvas.cpp:44-45`) - while `Show()` (`:74`) and `Refresh()` (`:85`)
  both call `m_window->Present()` unguarded.
- **The size is hardcoded, and the two sizes disagree:**
  `width = 1200; height = 800;` (`canvas.cpp:53-54`) are set nine lines
  after the commented `CreateWindow(..., 1280, 800)` at `:44` - and they
  differ by 80 px.
- **`Resize()` assumes the GL subclass:**
  `static_cast<ui::renderer::GlCanvas*>(m_canvas)->target(`
  `m_eglContext->Use(), 0, width, height, ui::renderer::ColorSpace::ABGR8888S)`
  (`canvas.cpp:80`) - the member is declared as the renderer base
  (`canvas.h:59`) but the factory used in the ctor is
  `ui::renderer::GlCanvas::gen(opengles)` (`:48`).
- **The unsupported-GLES path reports, then crashes on the same
  pointer:** `if (!m_canvas) { std::cout << "GlCanvas is not supported.
  Did you enable the GlEngine?" ... }` (`canvas.cpp:49-51`) is followed
  by `Resize()` at `:55`, which dereferences the null it just described.
  Only `Prepare()` checks (`:68`).
- **Four members are decoration:** `needResize`, `needDraw` and `print`
  appear exactly once in the whole folder each - their own declarations
  (`canvas.h:63,64,66`); `clearBuffer` (`:65`) is read at
  `canvas.cpp:61` and never assigned, so `Draw()` always passes `false`.
- **`Draw()` always returns `true`** (`canvas.cpp:63`) after
  `m_canvas->draw(clearBuffer)` and `m_canvas->sync()` (`:61-62`);
  `Prepare()` returns `false` only when `m_canvas` is null (`:68-69`).
- **Includes:** `canvas.h:28-31` pulls `currentthread.h`, `newref.h`,
  `egl/window.h`, `renderer/thor.h`; `currentthread` has exactly 1 hit in
  the folder - the include itself. `canvas.cpp:25-30` adds
  `configuration/configuration.h` (2 hits), `app/application/builder.h`,
  `opengles/opengles.h`. `std::cout` at `:50` with 0 `<iostream>`
  includes in the folder - the stream comes in transitively.
- **Link line vs sources:** `CMakeLists.txt:13-14` names 7 libraries -
  `qor_module` `qor_role` `qor_application` `qor_error` `qor_renderer`
  `qor_opengles` `qor_egl` - and 2 of them have no header from their own
  folder included here: `qor_role` (`src/framework/app/role`) and
  `qor_error` (`src/qor/flyers/error`), 0 hits for either in the folder.
  The reverse also holds: `src/configuration/` holds 2 files and no
  `CMakeLists.txt`, so `configuration.h` has no target to link.
- **Both indexes cut that link line at the same place:**
  `docs/agent/repo_map.md:120` prints
  `qor_canvas src/components/ui/canvas -> qor_module qor_role qor_application qor_error`
  and `docs/agent/deps.dot:140` prints
  `{ qor_module; qor_role; qor_application; qor_error; }` - 4 of the 7,
  exactly the first CMake line, dropping the three UI libs on
  `CMakeLists.txt:14`.
- **The dll exports mostly other libraries:** `dumpbin /exports` on the
  built dll prints 60 exports - 12 carry `qor::components::Canvas`
  (including all 9 public methods), 44 carry `OpenGLESFeature`,
  2 `EGLContext`, 2 `EGLDisplay`.
- **Why:** the classes it includes define their bodies in their headers -
  `opengles.h` (153 lines) carries 62 `{}` empty bodies, e.g.
  `virtual void BindTexture(unsigned int target, unsigned int texture){};`
  at `opengles.h:80` - so this target re-emits them into its own export
  table.
- **Every warning this target emits is somebody else's:** the judge build
  prints 173 `C4100` (unreferenced parameter) lines attributed to
  `qor_canvas.vcxproj`, all of them from
  `src/components/ui/opengles/opengles.h`; 0 from this folder.
- **Consumers: one build file outside the folder, and it is not
  configured:** `examples/experimental/canvastest/CMakeLists.txt:9,15`
  is the only place outside `src/components/ui/canvas/` that names
  `qor_canvas`, and `examples/experimental/CMakeLists.txt:3` reads
  `#add_subdirectory(canvastest)`. Nothing under `test/` links it.
- **That one consumer's include is stale:**
  `examples/experimental/canvastest/main.cpp:28` includes
  `src/components/framework/ui/canvas/canvas.h`, a path that does not
  exist (the `framework/` segment is gone), and it is the only file in
  the tree including this header - a grep for `ui/canvas/canvas.h`
  returns 1 hit.
- **The guard macro carries the same dead segment:**
  `QOR_PP_H_COMPONENTS_FRAMEWORK_UI_CANVAS` (`canvas.h:25,26,72`) -
  3 hits tree-wide, unique, saying `FRAMEWORK` for a folder at
  `src/components/ui/canvas`.
- **Index rows:** `docs/agent/purposes.md:99` - "qor_canvas Canvas
  drawing component."; `FEATURES.md:108` -
  "- qor_canvas (shared library) Experimental UI", with no description
  column and no Stable/Incomplete word, unlike `qor_loghandler` at
  `FEATURES.md:88`; `docs/agent/deps.dot:54` lists it in the UI band with
  `qor_console`, `qor_egl`, `qor_layout`, `qor_tui`, `qor_widgets`.
- **Bootstrap:** `qor_canvas.cpp:31-35` defines
  `qor::Module& ThisModule(void)` naming it "Querysoft Open Runtime:
  Canvas Module" - the same 5-line shape as `qor_loghandler.cpp`; it
  declares no `qor_pp_module_requires`, and its `#include <buildnumber.h>`
  at `:27` sits inside `#ifdef NDEBUG`, so the Debug build the judge
  runs never sees it.

## Failure modes

- **`Show()` and `Refresh()` are guaranteed null dereferences:** nothing
  assigns `m_window` outside the commented block (`canvas.cpp:44-45`),
  so `m_window->Present()` at `:74` and `:85` runs on a null
  `qor::ref_of<EGLWindow>::type`. The only consumer that exists calls
  exactly that (`examples/experimental/canvastest/main.cpp:159`).
- **Construction cannot fail safely:** every hop at `canvas.cpp:38-43`
  is unchecked, so a Canvas built before the application has an OpenGLES
  feature registered dereferences null inside the ctor; and when the
  renderer factory returns null the diagnostic at `:50` is followed by
  `Resize()` at `:55`, which uses that null at `:80`.
- **Only `Prepare()` knows the canvas is missing:** `Draw` (`:61`),
  `Resize` (`:80`), `Push` (`:90`), `Remove` (`:95`), `Update` (`:100`)
  and `Viewport` (`:105`) all use `m_canvas` unguarded, and `Draw()`
  returns `true` either way (`:63`), so a caller cannot tell a drawn
  frame from a crash avoided by luck.
- **A commented-out example is not a test:** the sole consumer is
  excluded from its band (`examples/experimental/CMakeLists.txt:3`) and
  its include no longer resolves, so the target is compiled on every
  judge run and never called. With 0 warnings from its own files, a
  green judge here proves only that the headers still parse.

## See also

[[qor_module]] [[qor_role]] [[qor_application]] [[qor_error]]
[[qor_loghandler]] [[qor_logaggregator]]
