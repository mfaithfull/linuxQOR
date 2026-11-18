---
Type: WikiPage
PageType: module
Title: "qor_opengles"
Target: qor_opengles
Status: current
LastVerified: 2026-11-18
Claims: 16
Summary: "The portable OpenGL ES component: a 5-file shim that pairs an EGL display/context with a single OpenGLESFeature interface and a 622-macro Khronos enum header. The component itself carries no GLES calls - 67 of its 72 virtual entry points are empty inline stubs and the real feature factory is supplied per-OS under src/platform. Its own Setup resolves the EGL feature and then throws the handle away."
Anchors:
  - { kind: in-tree, path: src/components/ui/opengles, symbol: qor_opengles, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/opengles/CMakeLists.txt, symbol: qor_opengles, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/opengles/opengles.h, symbol: OpenGLESFeature, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/opengles/opengles.cpp, symbol: OpenGLESFeature::Setup, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/opengles/constants.h, symbol: GL_TRIANGLES, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/opengles/qor_opengles.cpp, symbol: ThisModule, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_opengles --parallel 8
---

# qor_opengles

Judge GREEN: `cmake --build cmakebuild --target qor_opengles --parallel 8` exits 0 and links
`cmakebuild/src/components/ui/opengles/Debug/qor_opengles.dll`, which imports
`cmakebuild/src/components/ui/egl/Debug/qor_egl.dll`; no warning or error line from this target.

- **Folder:** 5 files, 924 physical lines - `constants.h` 658, `opengles.h` 153, `opengles.cpp` 65,
  `qor_opengles.cpp` 35, `CMakeLists.txt` 13. The whole component is two headers and two sources.
- **`add_library` names 2 sources** (`CMakeLists.txt:3`): `qor_opengles.cpp` and `opengles.cpp` -
  the header-only bulk (`constants.h`, `opengles.h`) is never compiled into a TU of its own.
- **Links 5, and the index agrees:** `CMakeLists.txt:13` names `qor_module qor_role qor_application
  qor_error qor_egl`; `docs/agent/repo_map.md:124` lists the same 5, including `qor_egl`, the sibling
  render component this one wraps (`qor_egl.md`).
- **Purpose line is shared:** `docs/agent/purposes.md:101` carries one line for the pair - "qor_egl /
  qor_opengles EGL / OpenGLES render component" - so the wiki has no separate prose intent for this target.
- **The product is one class:** `class OpenGLESFeature : public app::IFeature` (`opengles.h:41`) in
  `qor::components`, registered as `SingletonInstancer` + `ExternalFactory` + a fixed GUID
  (`:145-148`), with members `m_display`/`m_context` of `ref_of<EGLDisplay>`/`ref_of<EGLContext>`
  (`:139-140`) - i.e. it is a holder of an EGL display and context behind a role feature interface.
- **The GLES surface is a stub:** 72 `virtual` members, 67 of them declared in the header with an
  empty inline body - `void GenBuffers(...){}`, `unsigned int CreateShader(...){ return 0; }` and so
  on (`opengles.h:69-135`). Only `Setup`/`Shutdown`/`GetString`/`GetError`/`~OpenGLESFeature` are not
  stubs, and of those the bodies in `opengles.cpp` return `nullptr`/`0`.
- **`Setup` finds EGL then discards it:** `opengles.cpp:40-48` walks `AppBuilder().TheApplication()
  ->GetRole()->GetFeature(&EGLFeatureGUID)`, calls `fatal("OpenGLES requires EGL")` when it is null,
  and otherwise does nothing with the handle - it never creates the `m_display`/`m_context` it stores.
- **Two declared methods have no definition anywhere:** `CreateWindow(...)` and `MakeCurrent(...)`
  (`opengles.h:56-57`) are declared but a tree-wide grep finds no `OpenGLESFeature::CreateWindow` or
  `::MakeCurrent`; they also return a `ref_of<OpenGLESWindow>` whose class is only forward-declared
  at `opengles.h:40` and never defined in this folder - calling either is a link error.
- **The real implementation lives in `src/platform`:** `ImplementsOpenGLESFeature()` is declared at
  `opengles.h:35` (guarded by `QOR_PP_IMPLEMENTS_OPENGLES`) and defined in the per-OS twins
  `src/platform/os/windows/ui/opengles/opengles.cpp` (`QOR_WINEGL`) and
  `src/platform/os/linux/ui/opengles/opengles.cpp` (`QOR_LINOPENGLES`) - the component is the
  portable interface, the platform layer is the binding.
- **`constants.h` is vendor-supplied, not written here:** `constants.h:1-28` is the Khronos Group
  copyright (2013-2017) and the "generated from the Khronos OpenGL / OpenGL ES XML API Registry"
  notice; `constants.h:35+` holds 622 `#define GL_*` macros and the file's own note "constants taken
  from GL3 header" (`:33`).
- **No GL entry point is declared in this folder:** the 67 stubs are named `GenBuffers`, `BindTexture`,
  `DrawElements` etc. (`opengles.h:69-135`); there is no `extern` GLES function, no `libGLESv2` import,
  and no `#include <GLES/...>` - the component compiles against the enum header and its own stubs alone.
- **Module bootstrap is the standard 5 lines:** `qor_opengles.cpp:31-34` `ThisModule()` names
  "Querysoft Open Runtime: OpenGL ES Module" and declares no `qor_pp_module_requires`, matching the
  tree's inert-module bootstrap shape.

## Failure modes

- **A green judge here certifies an interface, not a renderer:** every draw call this target exposes is
  an inline no-op (`opengles.h:69-135`); a caller that builds against `qor_opengles` and calls
  `Clear`/`DrawElements` links, runs, and draws nothing unless a platform subclass overrides them.
- **`Setup` cannot fail usefully:** the only check is the EGL feature being null (`opengles.cpp:43-46`);
  with EGL present it stores nothing and leaves `m_display`/`m_context` empty, so the later
  `Display()`/`Context()` accessors (`opengles.h:59-67`) hand back null refs with no signal.
- **`CreateWindow`/`MakeCurrent` are traps:** declared and unconditionally callable, but neither is
  defined in-tree (see the claim above), and `OpenGLESWindow` is an incomplete type - the hazard only
  bites at link time, and only for a consumer that actually calls them.
- **Stale guard region, same as its siblings:** `opengles.h:25-26` and `constants.h:30-31` use
  `QOR_PP_H_COMPONENTS_FRAMEWORK_UI_OPENGLES...` - the token says `FRAMEWORK` for a folder under
  `src/components/ui`, a naming fossil shared across the `ui/` components.

## See also

[[qor_egl]] [[qor_gl]] [[qor_module]] [[qor_role]] [[qor_application]] [[qor_error]]
