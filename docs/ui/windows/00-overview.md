# 00 — Architecture overview

**Status: summary.** High-level pass over all seven areas. Each area has its
own page that will be walked in detail in a later session. Claims below are
cited to files that were read in full or to declaration inventories produced by
scanning every header in the area.

## What the library is for

From the folder's own `purpose.md`
(`src/platform/os/windows/ui/gui/purpose.md`):

Verbatim from `src/platform/os/windows/ui/gui/purpose.md`:

> (line 1) winqor_gui provides a Windows native GUI class library
> (line 3) This library implements the portable GUI concepts of the QOR, specifically for the Windows Operating System
> (line 5) It should provide factories, controllers and window classes for all built in Window types and Windows Common Controls. Including for OpenGL/EGL windows
> (line 7) It should provide fully customizable message handling that covers all documented & undocumented Window messages. It should support, hooks, Owner Draw, Custom Draw, Themes and Off Screen Rendering with Composition to an Open GL target window …
> (lines 8–9) It should surface Windows Accessibility and IME features / Have the capability to follow the users preference for dark/light theme

`purpose.md` states three structural rules that the code follows:

1. **Windows UI message loops are single-threaded.** "…even Window is bound to
   exactly one thread. All its messages must be handled on that thread and all
   its rendering takes place on that thread using GDI objects bound to that
   thread." (`purpose.md` lines 11–13)
2. **That per-thread state is a `PerThread` object**, which owns a
   `MessageHandler` implementing the message loop idiom (`purpose.md` line 13).
3. **Message handling is filtered through a customizable tree of objects
   derived from `BaseWindowPartHandler`.** "Each `BaseWindowPartHandler` either
   handles the message itself, handles it by passing it to its children, or
   gives up and its parent tries the next sibling." (`purpose.md` line 15)

`purpose.md` states three structural rules that the code follows:

1. **Windows UI message loops are single-threaded.** "Even Window is bound to
   exactly one thread. All its messages must be handled on that thread and all
   its rendering takes place on that thread using GDI objects bound to that
   thread." (`purpose.md` lines 11–13)
2. **That per-thread state is a `PerThread` object**, which owns a
   `MessageHandler` implementing the message-loop idiom.
3. **Message handling is a customisable tree** of objects derived from
   `BaseWindowPartHandler`. Each handler either handles the message, passes it
   to its children, or gives up so the parent can try the next sibling.
   (`purpose.md` line 15)

Those three rules are the spine of the library. Read them with
[the core part](10-core-root.md) and [the handler part](50-view-handlers.md).

## Subsystem map

Dependency direction inside the library, inferred from the `#include` graph of
the headers read so far (an area includes siblings of its own or of areas
*below* it — `factories` is the top of the public surface, `gdiobjects` the
bottom):

```
  application  ──►  factories ──► controllers ──► windows (window classes)
                        │             │                │
                        ▼             ▼                ▼
                    painting      handlers tree    gdiobjects
                        │             │                │
                        └────────► view/drawing + view/layout
```

| Area | Role | What it contains |
|---|---|---|
| `gui/` root | object model + thread binding | `GUI`, `Window`, `PerThread`, `WGLContext`, `OpenGLSession`, `WindowStation`, module registration |
| `factories/` | creating windows | `WindowFactory`, `WindowClass`, `WindowClassRegistration`, `WindowStyle`/`ExtendedWindowStyle`, `ClassStyle`, `HandlerFactory`, `DialogBuilder`, `MenuBuilder` |
| `controllers/` | one controller per control type, the Win32 message surface | `WindowController` base + `Button`/`ComboBox`/`Edit`/`ListBox`/`ScrollBar`/`Static`/`Animation` controllers; `Menu`, `PopupMenu`, `Monitor`, `Mouse`, `Caret` |
| `windows/` | the C++ type for each built-in Windows window class | `Button`, `ComboBox`, `ComboBoxEx`, `DateTimePicker`, `Desktop`, `Dialog`, `Edit`, `FlatScrollBar`, `Header`, `ListBox`, `MDIClient`, `MessageBox`, `ScrollBar`, `Static`, `Animation` |
| `view/handlers/` | the message-loop and handler tree | `MessageHandler`, `AbstractWindowHandler`, `BaseWindowHandler`, `RenderedWindowHandler`, `InteractiveWindowHandler`, `BaseWindowPartHandler`, `TopLevelWindowHandler`, and per-message-part handlers |
| `gdiobjects/` | GDI resource objects bound to a thread | `DeviceContext`, `Bitmap`/`DIB`/`DIBSection`, `Brush`, `Palette`, `Pen`, `Region`, `Font`, `Icon`, `Cursor`, `ColourSpace`, metafiles, `MemDeviceContext` |
| `painting/` | paint sessions, theming | `PaintSession`, `BufferedPainting`, `WindowPaintPainter`/`Painter`, `Themedata`/`ThemeData` |
| `view/drawing/` + `view/layout/` | geometry + arrangement | `Point`, `Rect`, `Size`; `LayoutItem`, `Layout`, `VertcialLayout`, `HorizontalLayout`, `VBox`, `WindowLayoutItem` |

## Namespaces

Two namespaces carry the library; the split is not uniform across areas and is
worth knowing before you grep:

- `qor::platform::win` — root files, `factories`, `gdiobjects`, `painting`,
  `controllers`, `windows`, and the geometry structs in `view/drawing`.
- `qor::platform::win::gui::view` — `view/handlers` classes, `view/layout`
  classes, and the message-id constants in `view/messages.h`
  (confirmed from `view/handlers/base.h:44`, `view/layout/layout.h:31`,
  `view/messages.h:40`).

Every exported type is marked `qor_pp_module_interface(QOR_WINGUI)`; the
compile definition is set in `gui/CMakeLists.txt:107`
(`target_compile_definitions(${target} PRIVATE QOR_WINGUI ...)`).

## The three objects every user of this library touches

The example at `examples/experimental/osspecific/windows/mainwindow/mainwindow.cpp`
shows the minimum working shape (lines 334–386):

```cpp
WindowFactory factory(GetInstance());
MessageHandler messageHandler;
...
auto controller = factory.Create(L"MainWindow", L"Main Window Experiments");
controller->Show();
controller->Update();
...
return messageHandler.MessageLoop();
```

1. **`WindowFactory`** (`factories/windowfactory.h:51`) — makes `Window`
   instances from a window class name and a name, and hands back a
   `WindowController`. Its header comment states the contract: "Make Window
   instances from a WindowClass and a WindowViewState. Each Window has a
   Controller and a Handler" (`windowfactory.h:49`–`50`).
2. **`WindowController`** (`controllers/windowcontroller.h:136`) — the
   operation surface for one window: `Show()`, `Update()`, `GetDC()`,
   `GetADC()`, `GetRect()`, `SetTheme()`, `SetColourKey()`, and ~150 more
   Win32 window operations.
3. **`MessageHandler`** (`view/handlers/messagehandler.h:53`) — runs the
   message loop; `PerThread::Run()` delegates to it
   (`perthread.cpp:38`–`41`: `return m_messageHandler.MessageLoop();`).

## Thread binding

`PerThread` (`perthread.h:49`) is declared `final` and registered as a thread
singleton: `qor_pp_declare_instancer_of(qor::platform::win::PerThread,
ThreadSingletonInstancer)` (`perthread.h:106`), with a
`thread_local qor::detail::ThreadInstanceHolder<...PerThread>` and an exported
`GetCurrentPerThread()` (`perthread.cpp:245`–`260`). Consequence:
`new_ref<PerThread>()` yields the instance for the **current** thread, which is
how the example obtains GL state (`mainwindow.cpp:199`).

Each `PerThread` owns (member list, `perthread.h:90`–`103`): four handle→object
maps (`Window`, `Menu`, `DeviceContext`, `Monitor`), the current `WGLContext*`,
an `OpenGLSession`, and a `MessageHandler`. `WindowController`'s constructor
takes the current thread's instance: `m_perThread = new_ref<PerThread>()`
(`windowcontroller.h:140`–`149`).

## Handler tree shape

Confirmed from the header declarations:

```
AbstractWindowHandler                    view/handlers/abstractwindowhandler.h:37
└── BaseWindowHandler                    view/handlers/base.h:44
    └── RenderedWindowHandler            view/handlers/rendered.h:39
        └── InteractiveWindowHandler     view/handlers/interactive.h:42
            └── TopLevelWindowHandler    view/handlers/toplevel.h:45

BaseWindowPartHandler                    view/handlers/parts/part.h:32   (sibling root)
├── parts:     BaseText, DeviceChange, Lifetime, Menu, Owner, Style, Timer, User
├── rendered:  Cursor, DesktopWindowManager(DWM), Font, Iconized,
│              NonClient(nc.h:33), NonClientRendering(ncrender.h:39), Rendering(rendering.h:54)
└── interactive: Clipboard, DragAndDrop, Focus(+SignalBase), Help, IME,
                Keyboard, NCPointer, Parent, Pointer, Scroll, WinCtrlParent
```

`MDIClientHandler` derives directly from `AbstractWindowHandler`
(`view/handlers/mdiclient.h:36`), and `DialogHandler`
(`view/handlers/dialoghandler.h:44`) and `Hook` (`view/handlers/hook.h:31`)
have no listed base class in the header — the base, if any, is set in the
`.cpp` or by construction; confirm in the detail pass.

## Registration flow (as the example uses it)

`mainwindow.cpp:338`–`344` adds a custom window class and registers a handler
factory for it:

```cpp
auto customClass = factory.AddWindowClass(L"CustomWindow");
ClassStyle cs;  cs.SetHRedraw(true); cs.SetVRedraw(true);
customClass->SetStyle(cs);
factory.RegisterClass(customClass, new_ref<CustomHandlerFactory>());
```

`WindowFactory` keeps `std::map<tstring, windowClassData>` where
`windowClassData` is `{ windowClass, registration, handlerFactory }`
(`windowfactory.h:75`–`84`). A `HandlerFactory<T>` is a template over an
`AbstractHandlerFactory` whose `Create()` returns `new_ref<T>()`
(`handlerfactory.h:51`–`63`) — that is the extension point for per-class
handler trees, and the example overrides it to wire `m_ncRendering`,
`m_nonClient` and `m_rendering` members on a `TopLevelWindowHandler`
(`mainwindow.cpp:319`–`332`).

## Build facts

- Library target `winqor_gui` is a **SHARED** library
  (`gui/CMakeLists.txt:3`); it links `qor_module qor_host qor_error qor_issue
  qor_current qor_interception qor_signals` plus the Windows API wrappers
  `winqor_api_kernel user gdi shell shcore comctl32 dwm uxtheme opengl`
  and `winqor_dynamiclibrary winqor_common winqor_sharedbootstrap`
  (`gui/CMakeLists.txt:112`–`117`).
- The module registers itself in `winqor_gui.cpp:31`–`35`: `ThisModule()`
  returns a static `qor::Module` named "Querysoft Open Runtime: Windows GUI
  Module".
- `gui/CMakeLists.txt` lists **99 `.cpp` files** in `add_library`; there are
  **103 `.cpp` files on disk**. These four are compiled into nothing and are
  dead as far as the built `winqor_gui.dll` is concerned:
  `windowstation.cpp`, `controllers/i_m/mdiclientcontroller.cpp`,
  `view/handlers/mdiclient.cpp`, `windows/a_h/header.cpp`.

## Known gaps and open questions

Recorded from source, not inferred:

- `controllers/purpose.md` lines 15–42 are a **TODO list** of control types
  without a controller: ComboBoxEx, DateTime, Header, HotKey, IPAddress,
  ListView, Menu, MonthCalendar, Pager, ProgressBar, ReBar, Ribbon, RichEdit,
  Slider, SpinButton, StatusBar, Tab, ToolBar, Tooltip, TreeView, WebBrowser.
- `windowstation.h` declares an **empty** `WindowStation` class and
  `windowstation.cpp` has an empty namespace body — a placeholder with no
  relationships.
- `wglcontext.h:39`–`57` comments out most of the WGL surface
  (`wglCopyContext`, `wglShareLists`, `wglUseFontBitmapsT`, layer palette calls);
  only `CreateLayerContext`, `DeleteContext` and `GetHandle` are live
  (`wglcontext.cpp:37`–`53`).
- `OpenGLSession` exposes a fixed 14-function subset of GL immediate mode
  (`openglsession.h:59`–`72`) backed by
  `qor::framework::win::DynamicLibrary m_opengldll("opengl32")`
  (`openglsession.cpp:33`).
- `WindowController` declares DWM attribute calls as comments only
  (`windowcontroller.h:299`–`306`), and `PrintWindow`, `FindWindowExT` and
  `EnumPropsExT` are commented out (`windowcontroller.h:171`, `265`, `291`).
- Accessibility/IME support exists as `IMEHandler`
  (`view/handlers/interactive/ime.h:32`), `ClassStyle::SetIME`
  (`classstyle.h:86`, bit `0x00010000`) and
  `WindowController::SendIMEMessageEx` (`windowcontroller.h:282`) — narrower
  than the purpose statement claims.
