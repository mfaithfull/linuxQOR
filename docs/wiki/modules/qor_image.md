---
Type: WikiPage
PageType: module
Title: "qor_image"
Target: qor_image
Status: current
LastVerified: 2026-11-16
Claims: 13
Summary: "In-memory image object and its pipeline sink; it builds green, sizes nothing, and its one consumer is commented out of the build."
Anchors:
  - { kind: in-tree, path: src/framework/data/resources/image, symbol: qor_image, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/data/resources/image/CMakeLists.txt, symbol: qor_image, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/data/resources/image/image.h, symbol: Image, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/data/resources/image/image.cpp, symbol: Free, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/data/resources/image/imagesink.h, symbol: ImageSink, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/data/resources/image/purpose.md, symbol: Intends, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_image --parallel 8
---

# qor_image

"Standard in-memory, device-independent image (pixmap/bitmap, multiple pixel formats, many formats
R/W)." (`docs/agent/purposes.md`). Built SHARED from `qor_image.cpp imagesink.cpp image.cpp` at
version 0.0.1 (`src/framework/data/resources/image/CMakeLists.txt`).

- It sits inside another target's folder: `src/framework/data/resources/CMakeLists.txt:25` is the one
  `add_subdirectory(image)` in the tree, so [[qor_resources]] (version 0.0.2) is this target's parent
  twice over - as a directory and as the file that configures it.
- Link line names eight: `qor_module qor_thread qor_sync qor_platform qor_error qor_filesystem
  qor_pipeline qor_fileconnector` (`src/framework/data/resources/image/CMakeLists.txt`). The judge is
  green and no configured target links it: of the 281 `CMakeLists.txt` under `src`, `test` and
  `examples`, 2 name `qor_image` - its own, and `examples/experimental/images/CMakeLists.txt` (lines 16
  and 24), whose directory is commented out at `examples/experimental/CMakeLists.txt:9`
  (`#add_subdirectory(images)`).
- `QOR_IMAGE` has 1 hit in `src`: the marker on `ImageSink` (`src/framework/data/resources/image/imagesink.h`).
  `Image` - the class the target is named for, and the only thing `ImageSink` stores - carries no token
  (`src/framework/data/resources/image/image.h`), so the exported class exports a reference to an
  unexported one.
- Six files, 8.3 KB. `imagesink.cpp` is 11 lines: two `#include` lines, an empty
  `namespace qor{ namespace framework{ }}`, and the closing comment `//qor::components`
  (`src/framework/data/resources/image/imagesink.cpp`) - the namespace it closes is not the one it
  opens, and the TU compiles nothing that `imagesink.h` does not already define inline.
- The pixel formats have no users. `RGB888` has 21 case-sensitive hits in `src`, `test` and `examples`:
  1 is this folder's own `struct RGB888`, the other 20 are longer identifiers elsewhere
  (`src/components/ui/renderer/render.h:645` is `case ColorSpace::ARGB8888:`) in files that do not
  include `image.h`. `RGBA8888` has 2, both the declaration.
- `purpose.md` is three lines of aspiration - "Intends to provide a standard image object ...
  preferably readable/writable from numerous formats"
  (`src/framework/data/resources/image/purpose.md`) - and the `docs/agent/purposes.md` row upgrades
  that to "many formats R/W". Format code in the folder: 0 hits for
  `png|jpeg|jpg|bmp|gif|tiff|decode|encode|stb_`. The only route bytes have in is `memcpy`, and the
  decoder its one consumer wants is another library (`qor_jpegdecodefilter`,
  `examples/experimental/images/CMakeLists.txt:16`).
- `docs/agent/repo_map.md` lists this target with six dependencies (line 57) where its
  `CMakeLists.txt` names eight; the two it drops, `qor_pipeline` and `qor_fileconnector`, are what
  `ImageSink`'s base class rests on (`src/framework/data/pipeline/source.h` is reached from
  `src/framework/data/resources/image/imagesink.h`). It is also the only one of the index's 86
  dependency rows whose path column runs into the arrow - `src/framework/data/resources/image-> ` - so
  a column-splitting read of the index loses the dependency list for this target twice over.

## Failure modes

- Nothing ever sizes an image. `m_width` and `m_height` have **0 assignments** in the whole folder
  (instrument count over `src/framework/data/resources/image`), although four places read them.
  `Image(size_t width, size_t height)` allocates `new TPixel[width * height]` and stores only
  `m_pixelSize` (`src/framework/data/resources/image/image.h`); `Image::SetSize` frees and re-allocates
  `new byte[m_pixelSize * m_width * m_height]` - the old, still-zero values - and never stores its own
  arguments either (`src/framework/data/resources/image/image.cpp`). So `At()`, `operator[]`,
  `VisitPixels` and `VisitLines` all loop `for (y = 0; y < m_height; ...)`, which is never, and
  `SetSize`'s guard `if((width != m_width) || (height != m_height))` is true on every call because the
  members it compares against are pinned at 0, so calling it repeatedly frees and re-allocates a
  zero-byte buffer forever.
- `RGBPixel`'s default template argument names a type that does not exist:
  `template< typename TPixelFormat = rgb8 >` (`src/framework/data/resources/image/image.h`), and `rgb8`
  has 0 occurrences in `src`, `test` and `examples` - the folder's own formats are `RGB888` and
  `RGBA8888`. Every default in the class resolves through it: `Image()`, `Image(width, height)`,
  `VisitPixels<>`, `VisitLines<>`. Only an explicit pixel type compiles, which is exactly what excludes
  the tree's only consumer - `Image output;` at `examples/experimental/images/images.cpp:114` is the
  default constructor - and that example is commented out of the build
  (`examples/experimental/CMakeLists.txt:9`). The target builds green because no TU instantiates them.
- `ImageSink::WriteBytes` counts in one unit and advances in another:
  `m_index += result;` where `result` is a byte count (`src/framework/data/resources/image/imagesink.h`),
  but `Image::operator[]` scales its index by `m_pixelSize`
  (`src/framework/data/resources/image/image.cpp`). A 64-byte write on a 4-byte pixel format leaves the
  cursor 64 pixel-slots - 256 bytes - ahead of where it wrote, so every subsequent write lands
  `m_pixelSize` times too far into the buffer, and nothing bounds `m_index` against the image at all.
  `operator[]` returns `nullptr` when `m_bytes` is null, and the very next use is
  `memcpy(m_image[m_index], data, bytesToWrite)` - a null destination.
- `ImageSink::Write` hands its out-parameter to `Push` as the count:
  `return (unitsToWrite == 0 || Pull(unitsWritten, unitsToWrite)) ? Push(unitsWritten, unitsWritten) : false;`
  (`src/framework/data/resources/image/imagesink.h`). The caller's `unitsToWrite` is dropped after the
  `Pull`, and when it is 0 the `||` short-circuits `Pull`, so `Push` is driven by whatever the caller
  left in `unitsWritten`.
- `Image` owns `delete [] m_bytes` in its destructor (`src/framework/data/resources/image/image.cpp`)
  and declares no copy constructor or assignment operator, only a copy that nobody wrote
  (`src/framework/data/resources/image/image.h`). The implicit copy is a shallow pointer copy and both
  destructors free it.
- The alpha format is declared backwards. `RGB888` is `r, g, b`, and `RGBA8888` directly below it is
  `r, b, g, a` - blue second (`src/framework/data/resources/image/image.h`). The `RGBPixel` accessors
  are name-based (`return base::g;`) so reads stay self-consistent, but the memory layout is BGRA and
  the only writer, `ImageSink::WriteBytes`, copies bytes rather than pixels.

## See also

[[qor_resources]] [[qor_pipeline]] [[qor_optionparser]] [[qor_module]]

