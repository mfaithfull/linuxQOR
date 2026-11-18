---
Type: WikiPage
PageType: module
Title: "qor_renderer"
Target: qor_renderer
Status: current
LastVerified: 2026-11-18
Claims: 16
Summary: "The densest component in the tree: a vendored fork of the ThorVG vector engine (MIT, 2020-2025) wrapped in a Querysoft module, 101 files / 27,693 lines, with two rasterizers - SwRenderer and GlRenderer - both descending from RenderMethod. Its image-loader factory is compiled almost entirely out: config.h, which defines the THORVG_*_LOADER_SUPPORT macros, is included nowhere (its only reference is a commented line), so the PNG/JPG/SVG/... factories preprocess to nothing and only RawLoader is live."
Anchors:
  - { kind: in-tree, path: src/components/ui/renderer, symbol: qor_renderer, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/renderer/CMakeLists.txt, symbol: qor_renderer, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/renderer/render.h, symbol: RenderMethod, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/renderer/sw/renderer.h, symbol: SwRenderer, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/renderer/gl/renderer.h, symbol: GlRenderer, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/ui/renderer/loader.cpp, symbol: PngLoader, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_renderer --parallel 8
---

# qor_renderer

Judge GREEN: `cmake --build cmakebuild --target qor_renderer --parallel 8` exits 0 and links
`cmakebuild/src/components/ui/renderer/Debug/qor_renderer.dll`; its import closure brings in
`cmakebuild/src/components/ui/opengles/Debug/qor_opengles.dll` and
`cmakebuild/src/components/ui/egl/Debug/qor_egl.dll`; no warning or error line from this target.

- **The largest folder in the tree:** 101 files, 27,693 physical lines - 60 `.h` + 40 `.cpp` +
  `CMakeLists.txt`. By area: top level 33 files, `common` 12, `gl` 25, `sw` 16, `renderables` 15.
  The six biggest are `sw/raster.cpp` 1814, `gl/gl.h` 1502, `gl/renderer.cpp` 1422,
  `gl/shadersrc.cpp` 1169, `sw/renderer.cpp` 942, `render.cpp` 919.
- **The engine is vendored, not written here:** `render.h`, `loader.h` and `loadmodule.h` open with
  "Copyright (c) 2020 - 2025 the ThorVG project" under ThorVG's MIT notice, and `thor.h` is the
  ThorVG umbrella header (guard `_THORVG_H_`, `thor.h:1`) re-namespaced to
  `qor::components::ui::renderer` (`thor.h:73`). The 40 compiled `.cpp` are ThorVG's engine TUs; only
  `qor_renderer.cpp` carries the Querysoft copyright (2008 - 2025) and the module bootstrap.
- **`add_library` names 40 sources** (`CMakeLists.txt:3-42`): `qor_renderer.cpp` plus 14 top-level
  `.cpp`, 4 `common/`, 10 `sw/`, 12 `gl/`. The 15 `renderables/` headers are not among them.
- **Links 7, and the index says 6:** `CMakeLists.txt:56` names `qor_module qor_role qor_application
  qor_workflow qor_signals qor_error qor_opengles`; `docs/agent/repo_map.md:125` lists only the first
  six, omitting `qor_opengles`, and `docs/agent/purposes.md:103` ("Renderer (workflow+signals)") names
  two of the seven.
- **Two rasterizers, one base:** `struct RenderMethod` (`render.h:565`) is the abstract engine, with
  `struct SwRenderer : RenderMethod` (`sw/renderer.h:37`) and `struct GlRenderer : RenderMethod`
  (`gl/renderer.h:37`); `GlRenderer::gen` takes a `ref_of<OpenGLESFeature>` (`gl/renderer.h:168`), so
  the `qor_opengles` link is exactly the GL backend's input handle.
- **SIMD rasterizers ship, but the arch flag is GNU-only:** `sw/` holds `rasteravx.h`, `rasterneon.h`
  and `rasterc.h`, while `CMakeLists.txt:47-49` appends `-mavx` only when
  `CMAKE_CXX_COMPILER_ID` is GNU; the MSVC judge build sets no arch flag yet still links.
- **`config.h` is dead:** its 34 lines define 13 `THORVG_*_SUPPORT 1` macros,
  `THORVG_VERSION_STRING "1.0.0"` and `WIN32_LEAN_AND_MEAN`, but the file's only reference in the tree
  is the commented `//#include "config.h"` at `common/common.h:31` - no translation unit includes it,
  so none of those macros is ever defined.
- **The loader factory is almost entirely compiled out:** `_find` (`loader.cpp:49`) switches on
  `FileType` and constructs `PngLoader`/`JpgLoader`/`WebpLoader`/`SvgLoader`/`TtfLoader`/`LottieLoader`
  each inside `#ifdef THORVG_<X>_LOADER_SUPPORT` (`loader.cpp:52-87`); because `config.h` is never
  included, every one of those macros is undefined and the case bodies preprocess to a bare `break;`.
- **Only `RawLoader` is live:** it is the single unguarded case (`loader.cpp:89`) and the only loader
  class that exists (`rawloader.h`/`rawloader.cpp`, in `add_library`); a tree-wide search finds no
  `class`/`struct` for `PngLoader` et al. - the target links not because the loaders exist but because
  the references to them are preprocessed away.
- **`renderables/` is an orphan of 15 zero-byte headers** - `compositor.h`, `dirtyregion.h`,
  `effect.h`, `method.h`, `path.h`, `region.h`, `shape.h`, `stroke.h`, `surface.h`, `trimpath.h` plus
  5 under `effects/`, every one 0 bytes, none compiled, and no file in the folder includes anything
  from `renderables/` - a planned split of the engine that never landed.
- **Guard-macro fossil shared with its siblings:** 129 header-guard matches
  `QOR_PP_H_COMPONENTS_FRAMEWORK_UI_RENDERER_*` (e.g. `loader.h:23`) name `FRAMEWORK` for a folder
  under `src/components/ui`, the same naming fossil as `qor_opengles`.
- **Bootstrap is the inert-standard shape:** `qor_renderer.cpp:31-34` `ThisModule()` names "Querysoft
  Open Runtime: Renderer Module" and declares no `qor_pp_module_requires`.

## Failure modes

- **A green judge certifies a linkable ThorVG fork, not an image loader:** the PNG/JPG/WEBP/SVG/TTF/
  Lottie factories at `loader.cpp:52-87` compile to nothing (`config.h` is not on any include chain),
  so a caller that asks `qor_renderer` to load a PNG through this path gets an empty `break;` and no
  image, even though the judge links.
- **The dependency index misleads:** `repo_map.md:125` names 6 libs where `CMakeLists.txt:56` links 7,
  and the missing one is `qor_opengles` - the sole external input of the GL backend - so anyone
  planning a GL build from the map will not have the handle `GlRenderer::gen` requires.
- **`renderables/` is a navigation trap:** an index or ctags pass lists 15 filenames
  (`compositor.h`, `surface.h`, ...) whose symbols can never verify because every file is empty; cite
  the folder, never a `renderables/` header, as an anchor.
- **SIMD correctness is unproven by this judge:** `sw/` carries AVX and NEON rasterizers but the MSVC
  build compiles without `-mavx` (`CMakeLists.txt:47-49`), so the green `qor_renderer.dll` exercises
  only the generic path; only a GNU build sets the arch flag.

## See also

[[qor_opengles]] [[qor_egl]] [[qor_module]] [[qor_role]] [[qor_application]] [[qor_workflow]] [[qor_signals]] [[qor_error]]
