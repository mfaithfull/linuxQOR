---
Type: WikiPage
PageType: module
Title: "qor_egl"
Target: qor_egl
Status: current
LastVerified: 2026-11-18
Claims: 30
Summary: "The base component EGL feature and wrapper layer; its display, context, window, session, and surface classes provide inert defaults, while real EGL behavior lives in separate winqor_egl and linqor_egl platform targets, so the qor_egl judge is green mainly because the inert API compiles and links."
Anchors:
  - { kind: in-tree, path: src/components/ui/egl, symbol: qor_egl, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/egl/CMakeLists.txt, symbol: qor_egl, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/egl/qor_egl.cpp, symbol: ThisModule, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/egl/egl.h, symbol: EGLFeature, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/egl/display.cpp, symbol: EGLDisplay::Initialize, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/egl/session.h, symbol: EGLSession, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_egl --parallel 8
---

# qor_egl

Judge GREEN: `cmake --build cmakebuild --target qor_egl --parallel 8`
exits 0 after touching all seven component `.cpp` files and links
`cmakebuild/src/components/ui/egl/Debug/qor_egl.dll`. The build has no
errors, but it emits 81 `C4100` warnings from the base EGL classes.

## Layout

- **Folder:** 14 code files, 1244 physical lines, `CMakeLists.txt` 13,
  `constants.h` 193, `egl.h` 88, `display.cpp` 243, `qor_egl.cpp` 35.
  The target adds 7 sources (`CMakeLists.txt:3`) and links `qor_module
  qor_role qor_application qor_error` (`:13`).
- **Configuration:** `src/components/ui/CMakeLists.txt:3` adds the `egl`
  subdirectory, making `qor_egl` one of the configured component UI
  libraries alongside `qor_canvas` and `qor_console`.
- **Index rows:** `FEATURES.md:110` lists `qor_egl` as "Experimental UI",
  `docs/agent/repo_map.md:122` and `docs/agent/deps.dot:142` list exactly
  the four link libraries from `CMakeLists.txt:13`, and
  `docs/agent/purposes.md:101` describes the EGL family together.
- **Purpose:** `src/components/ui/egl/purpose.md` is absent. The nearest
  prose is the grouped `qor_egl / qor_opengles` purpose row.
- **Constants:** `constants.h` contains no EGL function prototypes. It is a
  fallback macro block (`:31-191`) under `#ifndef EGL_VERSION_1_0`, after an
  empty `qor::components` namespace block at `:28-29`.
- **License and guards:** all 14 code files carry the Querysoft license
  block, and the 7 headers use `QOR_PP_H_COMPONENTS_FRAMEWORK_UI_EGL*`
  guards.
- **Public types:** `egl.h:39-49` forward declares `EGLSession`,
  `EGLDisplay`, `EGLContext`, and `EGLWindow`, then defines an
  `EGLPlatformHint` enum with `Wayland`, `X`, and `Win32`.
- **Feature marker:** `egl.h:33-35` declares `ImplementsEGLFeature()` only
  when `QOR_PP_IMPLEMENTS_EGL` is absent. The component target does not
  define that marker; `src/platform/os/linux/ui/egl/egl.cpp:45` and
  `src/platform/os/windows/ui/egl/egl.cpp:35` do.
- **Factory declarations:** `egl.h:80-83` declares an instancer, external
  factory, and fixed GUID for `components::EGLFeature`.
- **Module file:** `qor_egl.cpp` is 35 lines and only defines
  `ThisModule()` (`:31-34`), not the EGLFeature platform marker.
- **Exports:** `cmakebuild/src/components/ui/egl/Debug/qor_egl.dll` has
  92 ordinal exports, mostly constructors, destructors, vftables, and
  methods for the six `qor::components::EGL*` classes.

## Consumers

- **Configured consumers:** `src/components/ui/canvas/CMakeLists.txt:13-14`
  links `qor_egl` from `qor_canvas`, and
  `src/components/ui/opengles/CMakeLists.txt:13` links it from
  `qor_opengles`.
- **Header consumers:** a bounded grep found 36 include lines across 10
  `.cpp` files referencing `src/components/ui/egl`, including
  `src/components/ui/canvas/canvas.cpp:29` and
  `src/components/ui/opengles/opengles.cpp:29-32`.
- **Disabled examples:** `egltest`, `openglestest`, and `canvastest` link
  `qor_egl` in their CMake files, but
  `examples/experimental/CMakeLists.txt:1-3` comments them out.
- **Platform variants:** `src/platform/os/linux/ui/egl/CMakeLists.txt:1`
  defines `linqor_egl`, `src/platform/os/windows/ui/egl/CMakeLists.txt:1`
  defines `winqor_egl`, and both link `qor_egl`
  (`linqor:14`, `winqor:16`).
- **Tests:** no direct test files reference `qor_egl` or include
  `src/components/ui/egl`.

## API shape

- **Inline no-ops:** `EGLFeature::Setup()` and `EGLFeature::Shutdown()` are
  empty inline functions at `egl.h:60-61`.
- **Base feature methods:** `egl.cpp:37-85` returns `false`, `0`, or
  `nullptr` for `ReleaseThread`, `WaitClient`, `GetCurrentContext`,
  `BindAPI`, `QueryAPI`, `WaitGL`, `WaitNative`, `GetCurrentDisplay`,
  `GetCurrentSurface`, and `GetError`.
- **Session creation:** `EGLFeature::CreateSession()` ignores its platform
  hint and returns an empty `EGLSession` ref at `egl.cpp:87-90`.
- **Display creation:** `EGLFeature::CreateDisplay()` returns
  `new_ref<EGLDisplay>()` at `egl.cpp:92-95`, and the native-display
  overload constructs the same inert base display at `:97-100`.
- **Window creation:** `EGLFeature::CreateWindow()` returns
  `new_ref<EGLWindow>()` at `egl.cpp:102-105`, ignoring the session,
  context attribute vector, coordinates, and size.

## Failure modes

- **Uninitialized display:** `EGLDisplay` stores `m_major`, `m_minor`,
  `m_display`, and `m_config` at `display.h:80-83`, but the component
  constructors at `display.cpp:34-41` leave them unset and ignore
  `nativeDisplay`.
- **Base initialization fails closed:** `EGLDisplay::Initialize()` fetches
  the application `EGLFeature` at `display.cpp:45`, calls `BindAPI` at
  `:47`, calls the base `ChooseConfig` at `:67`, then returns
  `m_config != nullptr` at `:70`; base `ChooseConfig` returns `false` at
  `:91-93`, so base display initialization is expected to fail.
- **Inert display API:** `GetNativeDisplay()` returns `nullptr` with a
  comment that platform technology should override it at `display.cpp:78-80`,
  and the later base virtuals return `false`, `nullptr`, or `0` at
  `:91-240`.
- **Context path is forwarded:** `EGLContext::MakeCurrent`, `Query`, and
  `CreateImage` call `EGLDisplay` internal methods at `context.cpp:55-72`;
  because the base internal methods return `false` or `nullptr`
  (`display.cpp:108-155`), base contexts cannot usefully query or bind.
- **Window surface is not usable:** `EGLWindow::Initialize()` creates a
  context with `nullptr` share and then calls `CreateWindowSurface`, which
  returns `nullptr` in the base display (`window.h:68-74`,
  `display.cpp:123-125`); `Present()` ignores the false result from
  `SwapBuffers` (`window.h:51-54`).
- **Platform hint is ignored:** `EGLSession` initializes its display in the
  constructor at `session.h:41-44`, but `GetPlatform()` always returns
  `Wayland` at `:55-57`, despite `EGLPlatformHint` also naming `X` and
  `Win32`.
- **Surface destructor forwards unset storage:** `EGLSurface` declares
  `m_surface` at `surface.h:48`, never sets it in the base
  constructor at `surface.cpp:32-34`, and its destructor forwards
  `m_surface` to `InternalDestroySurface` at `surface.cpp:36-39`.
- **Template wrappers are where platform code attaches:** `EGLSessionWrapper`
  combines a native session with a platform display at `session.h:65-84`,
  and `EGLWindowWrapper` combines a native window with the EGL window at
  `window.h:82-103`; the component library itself contains only the base
  side.
- **Warning profile:** the forced rebuild log `tmp/_egl_build2.txt` records
  81 `C4100` warnings from `display.cpp` and `egl.cpp`, all unreferenced
  parameters in the inert API, with exit 0 and the DLL link at log line 107.