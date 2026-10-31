# winqor_gui — Windows GUI library documentation

Incremental documentation of the prototype Windows native GUI library
`winqor_gui`, source at `src/platform/os/windows/ui/gui`.

> **Built one part at a time.** Every page carries a **Status** line:
> `summary` = high-level pass done, detail walk pending ·
> `detailed` = walked file by file, claims cited to source.

## Why this documentation exists

`winqor_gui` has **no unit tests**. It is exercised only through the example
application `examples/experimental/osspecific/windows/mainwindow` (executable
`mainwindow`). The library is **212 `.h`/`.cpp` files** (215 files including the
two `purpose.md` files and `CMakeLists.txt`) across seven areas — too large to
read in one sitting. Documentation is therefore grown in passes: a high-level
summary per part first, then a detail walk of one part per session.

## Part index

| # | Part | Source area | Status |
|---|---|---|---|
| 00 | [Architecture overview](00-overview.md) | whole library | summary |
| 10 | [Core: window object, per-thread state, GL binding](10-core-root.md) | `gui/` root files | **detailed** |
| 20 | [Factories: window classes, styles, handler factories](20-factories.md) | `gui/factories/` | **detailed** |
| 30 | [Controllers: WindowController and control subclasses](30-controllers.md) | `gui/controllers/` | **detailed** |
| 40 | [Window types: built-in Windows window classes](40-window-types.md) | `gui/windows/` | **detailed** |
| 50 | [View handlers: message loop and handler tree](50-view-handlers.md) | `gui/view/handlers/` | **detailed (all 74 files)** |
| 60 | [GDI objects: device contexts, brushes, fonts, regions](60-gdiobjects.md) | `gui/gdiobjects/` | **60a+60b+60c detailed (32/32)** |
| 70 | [Painting and theming](70-painting.md) | `gui/painting/` | **detailed (8/8)** |
| 80 | [Geometry and layout](80-geometry-layout.md) | `gui/view/drawing/`, `gui/view/layout/`, `gui/view/messages.h` | **detailed (12/12)** |
| 90 | [The mainwindow example](90-example-mainwindow.md) | `examples/.../mainwindow` | detailed |

Detailed so far: 10, 20, 30, 40, 50 (**root + `parts/` + `interactive/` +
`rendered/` — all 74 files**), 90, **60a + 60b + 60c — all 32 `gui/gdiobjects/`
files** (60a the object model `gdiobject` `devicecontext` `memdevicecontext`, 60b
the 8-file metafile family, 60c the 9-pair resource-leaf family `bitmap` `brush`
`colourspace` `cursor` `font` `icon` `palette` `pen` `region`, verified against
source on 27 Oct 2026), **70 — all 8 `gui/painting/` files (747 lines)**, and
**80 — all 12 geometry/layout/message-vocabulary files (1,111 lines)**. Still
`summary`: 00 only (whole-library overview, deliberately kept high level). The
library is otherwise fully walked; per-part follow-ups are recorded on the
pages themselves.

## Ground rules for these pages

- **Cite, don't recall.** Every claim names the file, and the line region where
  it helps. A claim with no citation is a mistake to be corrected.
- **Separate intent from fact.** `Code does X` is an observation.
  `Windows API intent: Y` is the design goal the code is aiming at, not a
  measured behaviour.
- **No invented behaviour.** Code that has not been read is listed under
  "to cover" — it is never summarised from file names alone.
- **Observed runtime behaviour** is recorded only when the application was
  actually run and looked at. See the
  [runtime observation log](90-example-mainwindow.md#runtime-observation-log).

## Building and running the example

Regenerate the tree (required after any `CMakeLists.txt` change), then build
one target per command — two builds in one command race on shared `.pdb`s.

```powershell
cmake -B cmakebuild -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build cmakebuild --target mainwindow
```

`examples/experimental/osspecific/windows/CMakeLists.txt` currently enables
`mainwindow` only; the sibling examples (`button`, `dialogs`, `editbox`, `menu`,
`minimal`, `console`, `retropad`) are commented out. The library itself is
enabled in `src/platform/os/windows/ui/CMakeLists.txt` alongside
`gdi`, `opengles`, `renderapis`, `termscreen`, `toolkits`, `wincon`,
`serial`, `terminal`, `desktopui` (`egl` and `gl` are commented out).

Run the executable from its own build directory — the post-build step copies
runtime DLL dependencies next to it:

```powershell
.\cmakebuild\examples\experimental\osspecific\windows\mainwindow\Debug\mainwindow.exe
```

The current tree already contains a built `mainwindow.exe` and
`winqor_gui.dll` under `cmakebuild`.

## Session protocol — how to continue this work

1. Pick the next `summary` part in the index (lowest number first).
2. Read its files in full (a part is sized to fit one session; if not, split
   the walk and say so on the page).
3. Replace the summary sections with cited detail; keep the shape of the page.
4. Update the **Status** line and the "Next" line at the foot of the page.
5. Tick the part in the index table above and record the increment in
   `memory-bank/activeContext.md`, `memory-bank/progress.md` and
   `docs/agent/STATE.MD`.

Parts and rough file counts, for sizing a session (`.h`/`.cpp` only):

| Area | Files | Largest files |
|---|---|---|
| root | 13 | `perthread.cpp` 7.8 KB |
| `factories` | 15 | `windowstyle.h` 16.3 KB, `windowstyle.cpp` 12.3 KB, `windowfactory.cpp` 12.7 KB |
| `controllers` | 28 | `windowcontroller.cpp` 30.9 KB, `listboxcontroller.cpp` 13.4 KB |
| `windows` | 30 | `dialog.h` 5.4 KB — mostly small wrappers |
| `view` | 86 | `messages.h` 16.4 KB, `toplevel.cpp` 13.1 KB, `winctrlparent.cpp` 10.8 KB — subdivides into `drawing` 4, `handlers` 74, `layout` 7, `messages.h` 1 |
| `gdiobjects` | 32 | `devicecontext.cpp` 35.3 KB, `devicecontext.h` 19.9 KB |
| `painting` | 8 | `themedata.h` 12.3 KB |

`view/handlers` alone is 74 files (`handlers` root 20, `interactive` 22,
`parts` 18, `rendered` 14) and is the part most likely to need splitting
across sessions.

