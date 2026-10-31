# 90 — The `mainwindow` example: how `winqor_gui` is exercised

**Status: detailed for the source** (all of `mainwindow.cpp` read);
**runtime not yet observed** — see the observation log at the foot.

## Why this example exists

`winqor_gui` has no unit tests. This example is the only executable that drives
the library, so it doubles as the library's de-facto test. It is deliberately
experimental: it asks the question "can a Windows window be made to render with
OpenGL through a DIB, with custom non-client-area handling?" — the purpose
statement's "Off Screen Rendering with Composition to an Open GL target window"
(`src/platform/os/windows/ui/gui/purpose.md:7`).

## Files

`examples/experimental/osspecific/windows/mainwindow/` holds exactly three
files: `mainwindow.cpp` (13,238 bytes), `CMakeLists.txt` (860 bytes),
`some_ideas.txt` (1,811 bytes). There is no `purpose.md` in this example folder.

`mainwindow/CMakeLists.txt` sets `set(target mainwindow)` and
`add_executable(${target} mainwindow.cpp)`, marks
`WIN32_EXECUTABLE true`, compiles with `WINDOWS UNICODE _UNICODE` plus the
`qor_pp_ver_*` version defines, and links
`winqor_exebootstrap qor_module qor_host qor_compiler qor_platform
winqor_api_kernel winqor_api_shell winqor_common winqor_gui user32 gdi32
comdlg32 comctl32 shell32 uxtheme`. The `POST_BUILD` step copies
`$<TARGET_RUNTIME_DLLS:${target}>` into `$<TARGET_FILE_DIR:${target}>`, which
is why the exe must be run from its build directory.

The parent `examples/experimental/osspecific/windows/CMakeLists.txt:4` is the
only enabled `add_subdirectory`; `button`, `dialogs`, `editbox`, `menu`,
`minimal`, `console`, `retropad` are commented out (lines 1–3, 5–8).

## The GL pipeline in the example

`CreateDIB(cx, cy)` (`mainwindow.cpp:251`–`288`):

```cpp
qor_pp_assert_that(cx > 0);  qor_pp_assert_that(cy > 0);
m_BIH.biWidth = cx; m_BIH.biHeight = cy; m_BIH.biPlanes = 1;
m_BIH.biBitCount = 24; m_BIH.biCompression = BIRGB;
m_dcDIB.Dispose();                                   // release the previous one
m_dcDIB = DeviceContext::CreateForDIB();             // a DIB-backed device context
m_bmpDIB = new_ref<DIBSection>(m_dcDIB, (BitmapInfo*)(&m_BIH), DIBRGBCOLORS,
                               &m_bmp, nullptr, 0);  // bits live in m_bmp
m_dcDIB->SelectObject(m_bmpDIB->GetHandle());        // select the DIB into the DC
```

So: **a device context whose bits are a DIB in memory** — the off-screen
surface. `DeviceContext::CreateForDIB()` is the factory for that; the
`DIBSection` constructor takes the DC, a `BitmapInfo*`, the
`DIBRGBCOLORS` flavour, an out-pointer for the pixel bits, and two more
arguments (`nullptr, 0`) whose meaning belongs to
[part 60](60-gdiobjects.md).

`CreateRenderingContext()` (`mainwindow.cpp:290`–`315`) then puts OpenGL on
that DIB:

```cpp
PixelFormatDescriptor pfd;
pfd.nVersion = 1;
pfd.dwFlags  = PFDSUPPORTOPENGL | PFDDRAWTOBITMAP;
pfd.iPixelType = PFDTYPERGBA;
pfd.cColorBits = 24; pfd.cDepthBits = 32; pfd.iLayerType = PFDMAINPLANE;
int PixelFormat = m_dcDIB->ChoosePixelFormat(&pfd);   // 0 => fail
bool bResult    = m_dcDIB->SetPixelFormat(PixelFormat, &pfd);
m_context = new_ref<WGLContext>(m_dcDIB);             // layer context on the DIB DC
return m_context.IsNotNull();
```

`PFDSUPPORTOPENGL | PFDDRAWTOBITMAP` is the crux: OpenGL rendering into a
memory DIB rather than a window plane. `new_ref<WGLContext>(m_dcDIB)` uses the
one-argument form — `WGLContext(ref_of<DeviceContext>::type dc, int layer)`
(`wglcontext.h`) defaults the layer, and the implementation always calls
`WGL::CreateLayerContext` (`wglcontext.cpp:37`–`41`).

`InitScene` (`:197`–`210`), `ResizeScene` (`:212`–`221`) and `RenderScene`
(`:223`–`240`) are the GL immediate-mode calls, all reached through
`auto& gl = guiThread->OGLSession();` — the per-thread `OpenGLSession` from
[part 10](10-core-root.md). `RenderScene` clears
`COLOR_BUFFER_BIT | DEPTH_BUFFER_BIT`, then draws one RGB-triangle
(`Color3f` + `Vertex3f` inside `Begin(TRIANGLES)`/`End()`), then
`Flush()`. `Blit(DeviceContext& dest)` (`:243`–`249`) is
`dest.BitBlt(0, 0, m_width, m_height, m_dcDIB()(), 0, 0, SrcCopy)` —
`m_dcDIB()()` is `PrimitiveHandle::operator()()`, the raw handle of the DIB DC
(`handle.h:20`).

The whole chain — **GL → DIB → `BitBlt` into the window DC → GDI text on top**
— happens inside `OnPaint` (`mainwindow.cpp:167`–`183`). That is the concrete
evidence for "Composition to an Open GL target window".

## `main()` — the registration and window-creation sequence

`mainwindow.cpp:334`–`385`, in order:

```cpp
WindowFactory factory(GetInstance());   // :336  GetInstance() from winqorexeboot.h
MessageHandler messageHandler;          // :337  the loop, local to main

auto customClass = factory.AddWindowClass(L"CustomWindow");   // :339
ClassStyle cs;                                                // :340
cs.SetHRedraw(true); cs.SetVRedraw(true);                     // :341,:342
customClass->SetStyle(cs);                                    // :343
factory.RegisterClass(customClass, new_ref<CustomHandlerFactory>());  // :344

auto controller = factory.Create(L"MainWindow", L"Main Window Experiments"); // :347
controller->Show();  controller->Update();                                   // :348,:349

WindowStyle style;             // :351  Caption, SystemMenu, SizingBorder, Maximize, Minimize = true
ExtendedWindowStyle exStyle;   // :358  AcceptFiles, AppWindow, WindowEdge, ClientEdge,
                                //       Composited, ControlParent, Layered = true
auto second = factory.Create(customClass, L"Custom Top Level Window", style, exStyle); // :367
second->SetTheme(L" ", L" ");    // :368  blank theme strings
second->SetColourKey(0x0);      // :369
second->Show();  second->Update();

style.SetCaption(false); style.SetSystemMenu(false); style.SetSizingBorder(false); // :374-:376
exStyle.SetAppWindow(false); exStyle.SetToolWindow(true); exStyle.SetTopMost(true); // :377-:379
auto tool = factory.Create(L"MainWindow", L"ToolWindow", style, exStyle, 0, 0, 500, 100); // :381
tool->Show(); tool->Update();

return messageHandler.MessageLoop();   // :385
```

Three windows, three routes:

1. **`MainWindow` class, default style** (`:347`) — the built-in path, no
   custom class, so the library's default handler tree applies.
2. **`CustomWindow` class with the custom factory** (`:367`) — the class
   registered at `:339`–`344`, so the *custom* handler tree applies; this is
   where the GL pipeline runs. `SetTheme(L" ", L" ")` (`:368`) and
   `SetColourKey(0x0)` (`:369`) ask for themed, colour-keyed composition.
3. **A positioned `MainWindow` tool window** (`:381`, `x=0, y=0, cx=500, cy=100`)
   with `exStyle.SetToolWindow(true)` and `SetTopMost(true)` — the tool-window
   style, which Windows owns.

`style` and `exStyle` objects are **mutated between calls**, so the same
variables carry different masks to `Create` at `:367` and `:381`. Reading them
as one block is misleading — read them next to each `Create`.

The `ClassStyle cs` at `:340` sets only `hRedraw`/`vRedraw`
(bits `0x0002`/`0x0001`, `classstyle.h:75`–`76`), i.e. the custom class redraws
both directions — required for a GL-rendered window that repaints the whole
client area.

## Open ideas recorded next to the example

`some_ideas.txt` is the author's design notes; cite them as intent, not
behaviour. Relevant lines:

- `:2`–`:3` the Window Factory singleton should be owned by the DesktopGUI
  feature and "auto register the known types on construction".
- `:6` "a `new_ref` on a WindowClass derivative should go and find the
  factory, Add and Register the class once it's constructed."
- `:10`–`:20` the target end-user shape:
  `auto mainWindow = new_ref<MainWindow>("Title"); mainWindow->Add<CustomMenuBar>(…);`
  `mainWindow->Add<DockingSideFrame>(Layout::Start); mainWindow->Show();`
  `gui->RunPolling();` — with the commented-out `mainWindow->SetLayout<VBox>()`
  at `:12`.
- `:25` "Preregister all the builtin types so that the factory itself can
  report them as available."
- `:26`–`:31` the roadmap beyond WGL: EGL main window, EGL child window,
  OpenGL ES canvas window, Vulkan main window, OpenGL painting behind an MDI
  client.

These map onto [part 20](20-factories.md) (registration),
[part 80](80-geometry-layout.md) (`VBox`, `Layout::Start`) and
[part 40](40-window-types.md) (`mdiclient`).

## Runtime observation log

Nothing observed yet. To fill this in, run the exe and record what is actually
seen — mouse and keyboard control is not available to the agent, so the user
drives the interaction and reports results.

| Run | Date | Build | What was seen |
|---|---|---|---|
| — | — | — | no observations recorded |

Points worth checking on a run:

1. Does the **Custom Top Level Window** show the RGB triangle (the
   `RenderScene` output blitted through the DIB) with "Hello, Windows!" drawn
   over it (`mainwindow.cpp:179`–`180`)?
2. Does resizing that window change the picture size — i.e. is `OnSize`
   (`mainwindow.cpp:152`–`165`) actually re-running `CreateDIB`
   and `CreateRenderingContext`?
3. Does dragging work at the edges — does `OnNCHitTest`
   (`mainwindow.cpp:94`–`139`) return the code Windows then acts on?
4. Does the **ToolWindow** appear at 0,0 sized 500×100
   (`mainwindow.cpp:381`)?
5. Exit code / `MessageLoop()` return (`mainwindow.cpp:385`) after
   `wmQuit`/`wmClose`.

## Gotchas when using this example

- Two `cmake --build` invocations in one command race on shared `.pdb`s
  (`C1090`/`C1041 … use /FS`): one cmake build per command, strictly sequential.
- Build and run must be separate steps; the exe only finds its DLLs inside
  its own build directory.
- `GetInstance()` (`mainwindow.cpp:336`, from
  `src/platform/os/windows/app/exebootstrap/winqorexeboot.h` at `mainwindow.cpp:7`)
  is the process instance handle the factory needs.
- `qor_pp_assert_that` (`mainwindow.cpp:253`, `254`, `272`, `281`, `282`) comes
  from `src/qor/tdd/assert/assert.h` (`mainwindow.cpp:5`) — a failed assertion
  inside `CreateDIB` is a visible failure mode, not a silent one.
- The example's own `MessageHandler messageHandler;` (`mainwindow.cpp:337`) is a
  **local** object, distinct from the `PerThread`-owned one; the loop that ends
  the program is the local one at `mainwindow.cpp:385`.

**Next:** [back to index](README.md) — pick the next `summary` part and detail
it.
