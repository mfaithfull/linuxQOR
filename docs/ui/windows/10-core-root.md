# 10 — Core: the window object, per-thread state, and the GL binding

**Status: detailed.** All 13 root files were read in full
(`gui.h/.cpp`, `window.h/.cpp`, `perthread.h/.cpp`, `wglcontext.h/.cpp`,
`openglsession.h/.cpp`, `windowstation.h/.cpp`, `winqor_gui.cpp`,
plus `purpose.md`, `CMakeLists.txt`).

## Files in this part

| File | Size | Role |
|---|---|---|
| `gui.h` / `gui.cpp` | 1.9 / 2.2 KB | module-level entry points (`Quit`, `InitCommonControlsEx`) |
| `window.h` / `window.cpp` | 4.1 / 3.9 KB | `Window` — the HWND wrapper |
| `perthread.h` / `perthread.cpp` | 4.4 / 7.8 KB | `PerThread` — thread singleton, handle registries, WGL current context |
| `wglcontext.h` / `wglcontext.cpp` | 2.9 / 2.3 KB | `WGLContext` — an OpenGL layer context on an HDC |
| `openglsession.h` / `openglsession.cpp` | 3.6 / 3.2 KB | `OpenGLSession` — the GL immediate-mode subset |
| `windowstation.h` / `windowstation.cpp` | 1.7 / 1.7 KB | `WindowStation` — empty placeholder |
| `winqor_gui.cpp` | 1.7 KB | module registration (`ThisModule()`) |

## The object model

```
PerThread (one per thread; thread singleton)
 ├── MessageHandler          owns the message loop; PerThread::Run() calls it
 ├── OpenGLSession           one GL state holder per thread
 ├── current WGLContext*     which WGLContext is bound to which HDC
 ├── map<void*, Window*>     HWND  -> Window
 ├── map<void*, Menu*>       HMENU -> Menu
 ├── map<void*, ref_of<DeviceContext>>  HDC -> DeviceContext
 └── map<void*, Monitor*>    HMON  -> Monitor

WindowController  owns  ref_of<Window>        (controller → window)
Window            holds a raw WindowController* back-pointer (window → controller)
Window            holds ref_of<AbstractWindowHandler>  (its handler tree root)
Window            holds ref_of<LayoutItem>             (its layout)
```

The owning direction is controller → window; the window keeps a **raw,
non-owning** back-pointer (`window.h:84`
`WindowController* m_controller{nullptr}`, set by
`Window::SetController(WindowController*)`).

`WindowController`'s constructor establishes both ends of that pair
(`windowcontroller.h:145`–`149`):

```cpp
WindowController(ref_of<Window>::type window) : m_window(window) {
    m_perThread = new_ref<PerThread>();   // current thread's instance
    m_window->SetController(this);        // back-pointer to the controller
}
```

## `gui.h` / `gui.cpp` — module entry points

Namespace `qor::platform::win`, guard `QOR_PP_H_WINDOWS_GUI`. The class is a
pair of **static** functions — no instance is ever made:

```cpp
class qor_pp_module_interface(QOR_WINGUI) GUI {
    static void Quit(int exitCode);
    static bool InitCommonControlsEx(struct InitCommonControlsEx& init);
};
```

- `GUI::Quit` → `User32::PostQuitMessage(exitCode)` (`gui.cpp:40`).
- `GUI::InitCommonControlsEx` →
  `ComCtl32::InitCommonControlsEx(reinterpret_cast<LPINITCOMMONCONTROLSEX>(&init)) ? true : false`
  (`gui.cpp:45`) — the QOR struct `InitCommonControlsEx` is reinterpreted as the
  SDK `LPINITCOMMONCONTROLSEX`, so the layout must match the SDK's; the bool
  result is normalised from the SDK `BOOL`.

`gui.h:28` includes `"window.h"` by **local** path (not source-root-relative),
and `gui.h:31` carries the standing interface rule comment "All types on this
interface must be portable" — repeated in every root header of this part.

## `window.h` / `window.cpp` — `Window`, the HWND wrapper

`Window` is the C++ wrapper for one `HWND`. It is the type every handler and
every GDI object in the library is threaded around.

Callbacks declared here are the `__stdcall` shapes Windows passes back into the
window's message procedure (`window.h:44`–`45`, `87`–`88`):

```cpp
typedef int (__stdcall* PropEnumProc)(void*, const TCHAR*, void*);
typedef int (__stdcall* PropEnumProcEx)(void*, TCHAR*, void*, unsigned long long);
typedef bool (* wndenumproc)(Window*, long long);
typedef bool (* propenumproc)(Window*, const TCHAR*, void*);
```

The `wndenumproc`/`propenumproc` typedefs take a **`Window*`**, i.e. the QOR
object, not a raw `HWND` — enumeration callbacks are handed library objects,
which is what makes the `PerThread` handle→object maps necessary.

### Construction and handles

Four constructors (`window.h:53`–`56`):

| Constructor | Effect |
|---|---|
| `Window()` | `m_handle = nullptr` (`window.cpp:61`–`64`) |
| `Window(className, windowName, style, exStyle, x, y, width, height, parent, menu, hInstance, param)` | calls `User32::CreateWindowExT` (`window.cpp:94`) |
| `Window(const PrimitiveHandle& h)` | adopts an existing handle: `m_handle(h.Use())` (`window.cpp:99`) |
| `virtual ~Window() = default` | no handle closing in the destructor |

The 12-argument constructor resolves the QOR objects to raw handles before the
call (`window.cpp:87`–`97`):

```cpp
HWND parentHWnd = 0;
if (parent) parentHWnd = reinterpret_cast<HWND>(parent->GetHandle().Use());
HMENU hMenu = reinterpret_cast<HMENU>(menu.GetHandle().Use());
m_handle = User32::CreateWindowExT(exStyle, className, windowName, style, x, y,
                                   width, height, parentHWnd, hMenu,
                                   reinterpret_cast<HINSTANCE>(hInstance.Use()), param);
m_handle.DontClose();
```

`WindowFactory::Create` supplies the defaults for that call
(`windowfactory.h:58`): `x`, `y`, `width`, `height` default to
`(int)0x80000000` and `style` defaults to
`0x00000000L | 0x00C00000L | 0x00080000L | 0x00040000L | 0x00020000L | 0x00010000L`.
Those sentinels are passed through to `CreateWindowExT` unchanged; their meaning
is Windows' own (position/size unspecified, style the composed window-style mask)
— **the bits are not named anywhere in this repository**, so treat the mapping
as Windows API intent rather than a fact established by this code.

Handle semantics come from `src/platform/os/windows/common/handles/handle.h`:

- `PrimitiveHandle` is a bare handle: `void* m_h`, `Use()` and
  `void* operator()()` — **no close semantics at all** (`handle.h:9`–`47`).
- `Handle` adds the close discipline: `bool m_needsClose{false}`,
  `DontClose()`, `NeedsClose()`, `virtual Close()`, `virtual Attach(void*)`,
  `virtual Detach()` (`handle.h:50`–`104`).
- `Window::SetHandle` (`window.cpp:50`–`54`) converts a `PrimitiveHandle` to the
  stored `Handle` with `h.Use()` and then calls `m_handle.DontClose()` — the
  wrapper deliberately never closes a window handle on the application's behalf.

Practical consequence: `HWND` lifetime is the application's, and the
`PerThread` handle maps keep `Window*` alive by raw pointer, so removal from the
registry (`PerThread::RemoveWindow`) is the point at which a `Window` stops
being reachable by handle lookup.

### Message procedure

```cpp
long long Window::DefWindowProcT(unsigned int msg, unsigned long long wparam, long long lparam) {
    return User32::DefWindowProcT((HWND)(m_handle.Use()), msg, wparam, lparam);
}
```

(`window.cpp:104`–`107`) — the default Windows message procedure, used as the
fallback when the handler tree gives up on a message (to confirm in the handler
detail pass).

`window.h:69`–`71` declares a member whose type is the type of a lambda:

```cpp
decltype([](unsigned int msg, unsigned long long wParam, long long lParam)->long long{
    return 0;
}) m_messageProc;
```

A lambda's type is unique and not assignable from another callable, so this
member cannot be re-targeted — as written it is a fixed no-op placeholder.
`window.cpp:34` also does `#undef TranslateAccelerator` before pulling in the
User32 API layer, guarding against an SDK macro of the same name.

## `perthread.h` / `perthread.cpp` — `PerThread`

`class qor_pp_module_interface(QOR_WINGUI) PerThread final` (`perthread.h:49`);
`final` plus the thread-singleton instancer declaration
(`perthread.h:106`) make it a closed, per-thread singleton.

Plumbing at the foot of `perthread.cpp:245`–`260`:

```cpp
thread_local qor::detail::ThreadInstanceHolder<qor::platform::win::PerThread> ThreadInstanceHolderPerThread;
qor_pp_export qor::detail::ThreadInstanceHolder<...PerThread>* GetCurrentPerThread() { return &ThreadInstanceHolderPerThread; }
template<> ThreadInstanceHolder<...PerThread>* theThreadInstanceHolder<...PerThread>() { return GetCurrentPerThread(); }
```

So `thread_local` gives one holder per thread, and the specialisation of
`theThreadInstanceHolder<T>()` is what `new_ref<PerThread>()` resolves through.

### Handle registries

Uniform shape — add if absent, remove if present, look up by raw handle
(`perthread.cpp:55`–`209`). All four return `bool` for add/remove:

| Registry | Add | Remove | Lookup |
|---|---|---|---|
| `std::map<void*, Window*> m_windowHandleMap` | `AddWindow(Window*)` | `RemoveWindow(Window*)` | `FromHandle(void* hWnd)` |
| `std::map<void*, Menu*> m_menuHandleMap` | `AddMenu(Menu*)` | `RemoveMenu(Menu*)` | `MenuFromHandle(void* hMenu)` |
| `std::map<void*, ref_of<DeviceContext>::type> m_deviceContextHandleMap` | `AddDeviceContext(...)` | `RemoveDeviceContext(...)` | `DeviceContextFromHandle(void* hDC)` |
| `std::map<void*, Monitor*> m_monitorHandleMap` | `AddMonitor(Monitor*)` | `RemoveMonitor(Monitor*)` | `MonitorFromHandle(void* hMon)` |

`DeviceContextFromHandle` is the odd one out — it **fabricates** a wrapper for a
handle it has never seen (`perthread.cpp:171`–`176`):

```cpp
if (hDC != nullptr && dc.IsNull())
    dc = new_ref<DeviceContext>(PrimitiveHandle(hDC), false);
```

That is how a device context handed over by Windows (for example in a message
parameter) can be used by the library's GDI objects without the library having
created it. The `false` argument is the "do not close" flag seen elsewhere in
GDI object construction; confirm its exact parameter against
`gdiobjects/devicecontext.h` in that part's detail pass.

### WGL current-context tracking

Three members keep exactly one `WGLContext` current per thread
(`perthread.h:99`–`101`, `perthread.cpp:216`–`236`):

```cpp
bool PerThread::SetCurrentWGLContext(ref_of<DeviceContext>::type dc, ref_of<WGLContext>::type context) {
    m_currentWGLContext = context;
    return WGL::MakeCurrent((HDC)(dc->GetHandle().Use()), (HGLRC)(context->GetHandle().Use())) ? true : false;
}
bool PerThread::ClearCurrentWGLContext() {
    m_currentWGLContext = nullptr;
    return WGL::MakeCurrent((HDC)(nullptr), (HGLRC)(nullptr)) ? true : false;
}
bool PerThread::MakeThisWGLContextNotCurrent(WGLContext* context) {
    if (context == m_currentWGLContext) { ClearCurrentWGLContext(); return true; }
    return false;
}
```

The pairing matters: OpenGL calls made through `OpenGLSession` act on whatever
context Windows considers current for the thread, so binding a GL context to an
HDC and clearing it are **thread state transitions**, not local operations.
`WGLContext`'s destructor uses this to avoid leaving a dangling current context
(see below).

### Enumerators, creation-in-progress, resource ids

- `SetWindowEnumProc` / `GetWindowEnumProc` and
  `SetPropertyEnumProc` / `GetPropertyEnumProc` store the `wndenumproc` and
  `propenumproc` callbacks used when Windows enumerates windows and properties
  (`perthread.h:74`–`78`, `perthread.cpp:88`–`106`). The stored member is
  misspelled `m_properrtyEnumProc` (`perthread.h:95`) — cosmetic, but a grep
  hazard.
- `SetWindowCreationInProgress(ref_of<Window>::type window)` stores the window
  being constructed; `GetWindowCreationInProgress()` **disposes the stored ref
  before returning it** (`perthread.cpp:48`–`53`). It is a take-once accessor:
  a second call yields null. This exists because `CreateWindowExT` re-enters
  the message loop with a `wmCreate` message before the constructor has
  returned, so the object Windows will hand back has to be reachable by handle
  before it is reachable by name.
- `NextResourceId()` returns `++m_nextResourceId` with the member initialised
  to 100 (`perthread.h:99`, `perthread.cpp:211`–`214`) — so ids handed out per
  thread start at 101. Used for hot-key ids and event ids
  (`WindowController::RegisterHotKey(int id, ...)`,
  `SetTimer(unsigned long long idEvent, ...)`, `KillTimer(...)`).
- `int Run()` is the loop entry point: `return m_messageHandler.MessageLoop();`
  (`perthread.cpp:38`–`41`).
- `OpenGLSession& OGLSession()` returns the owned GL state holder by reference
  (`perthread.cpp:238`–`241`).

`perthread.cpp:32` does `#undef AddMonitor` before including the GDI API layer —
the SDK defines an `AddMonitor` macro that collides with the member name.

## `wglcontext.h` / `wglcontext.cpp` — `WGLContext`

```cpp
WGLContext::WGLContext(ref_of<DeviceContext>::type dc, int layer) {
    // m_h = GDI32::wglCreateContext((HDC)(dc->GetHandle().Use()));
    m_h = WGL::CreateLayerContext((HDC)(dc->GetHandle().Use()), layer);
}
WGLContext::~WGLContext() {
    auto guiThread = new_ref<PerThread>();
    guiThread->MakeThisWGLContextNotCurrent(this);
    WGL::DeleteContext((HGLRC)(m_h.Use()));
}
```

(`wglcontext.cpp:37`–`48`) Two deliberate choices recorded in the code:

1. `wglCreateContext` is commented out; **layer contexts are always used**,
   even for `layer = 0`. That is what makes compositing a GL render into a
   normal GDI window plane possible — the stated purpose of the library
   ("Composition to an Open GL target window").
2. The destructor clears thread state **before** deleting the GL context, so
   `PerThread::m_currentWGLContext` cannot end up pointing at a deleted object.

The rest of the WGL surface is present only as commented-out declarations in
`wglcontext.h:39`–`57` (`wglCopyContext`, `wglDeleteContext`,
`wglGetProcAddress`, `wglShareLists`, `wglUseFontBitmapsT`,
`wglUseFontOutlinesT`, layer palette and swap-buffer calls). Only
`GetHandle()` returns the `PrimitiveHandle m_h` (`wglcontext.h:61`).

## `openglsession.h` / `openglsession.cpp` — `OpenGLSession`

`class qor_pp_module_interface(QOR_WINGUI) OpenGLSession final`
(`openglsession.h:33`). Fourteen fixed GL immediate-mode entry points, each a
one-line delegation to the `GL::` API layer
(`openglsession.cpp:43`–`111`): `Begin`, `BlendFunc`, `Clear`, `ClearColor`,
`Color3f`, `Enable`, `End`, `Flush`, `LoadIdentity`, `MatrixMode`, `PopMatrix`,
`PushMatrix`, `Vertex3f`, `Viewport`.

The class also carries the GL enum values it needs as
`static const unsigned int` members (`openglsession.h:37`–`55`), which is why
call sites read `OpenGLSession::ALPHA_TEST` rather than `glAlphaTestT`:

| Name | Value | Name | Value |
|---|---|---|---|
| `TRIANGLES` | `0x0004` | `ALPHA_TEST` | `0x0BC0` |
| `DEPTH_TEST` | `0x0B71` | `COLOR_MATERIAL` | `0x0B57` |
| `LIGHTING` | `0x0B50` | `LIGHT0` | `0x4000` |
| `BLEND` | `0x0BE2` | `SRC_ALPHA` | `0x0302` |
| `ONE_MINUS_SRC_ALPHA` | `0x0303` | `PROJECTION` | `0x1701` |
| `MODELVIEW` | `0x1700` | `COLOR_BUFFER_BIT` | `0x00004000` |
| `DEPTH_BUFFER_BIT` | `0x00000100` | | |

State: one member, `qor::framework::win::DynamicLibrary m_opengldll`
constructed with `"opengl32"` (`openglsession.cpp:33`). So the DLL is resolved
by the dynamic-library facility rather than linked at import time. There is no
error handling and no state tracking in the wrapper — every call goes straight
through.

## `windowstation.h` / `windowstation.cpp` — `WindowStation`

A placeholder. `windowstation.h:31`–`33` declares
`class WindowStation { };` in `qor::platform::win` with **no**
`qor_pp_module_interface` marking, and `windowstation.cpp:33`–`35` is an empty
namespace body that nonetheless includes `"src/platform/os/windows/api_layer/user/user32.h"`
and pulls `using namespace qor::win::api`.

`windowstation.cpp` is **not in the `add_library` list** in
`gui/CMakeLists.txt` — the translation unit is never compiled, so nothing about
it is present in `winqor_gui.dll`. `windowstation.h` is also not included by
any other header in this part, so the class is currently unreachable.

Intended meaning, from the name and the purpose statement: a station that hosts
windows for a thread — the natural home for the `PerThread`/`MessageHandler`
pairing once it is implemented. Do not document it as working behaviour.

## `winqor_gui.cpp` — module registration

```cpp
qor::Module& ThisModule(void) {
    static qor::Module QORModule("Querysoft Open Runtime: Windows GUI Module", qor_pp_module_ver_string);
    return QORModule;
}
```

(`winqor_gui.cpp:31`–`35`) Required of every QOR lib/module: the file-static
`ThisModule()` that the `Library` singletons register with. Note the
`#ifdef NDEBUG #include <buildnumber.h> #endif` at line 26–28 — the generated
build-number header is only pulled in for non-debug builds, and
`gui/CMakeLists.txt:104` includes `cmake/updatebuildnumber.cmake` to produce it.

## Gotchas in this part

1. **`new_ref<PerThread>()` is a lookup, not a creation.** It resolves the
   current thread's singleton. Code that expects a fresh object per call
   (for example `auto guiThread = new_ref<PerThread>();` twice in a row) gets
   the same instance — that is the intended idiom, seen in the example at
   `mainwindow.cpp:156`, `199`, `214`, `225`.
2. **`GetWindowCreationInProgress()` consumes.** Calling it for a second time
   returns null (`perthread.cpp:51` `m_inProgress.Dispose();`).
3. **`Window` never closes its handle.** `SetHandle` and the constructors call
   `DontClose()`; the destructor is defaulted.
4. **The window → controller link is a raw pointer**
   (`window.h:84`), so `window.GetController()` can legitimately be null —
   every handler in the example guards on it
   (`mainwindow.cpp:73`, `99`, `169`).
5. **`m_messageProc` is not a `std::function`** and cannot be assigned
   (`window.h:69`–`71`).
6. **Header include style in this part is local, not source-root-relative**
   (`gui.h:28` `#include "window.h"`, `perthread.h:39` `#include "window.h"`,
   `window.h:34` `#include "controllers/i_m/menu.h"`). This deviates from the
   project's root-relative convention and is a real, consistent choice inside
   this library folder — see `docs/skills/QORconventions.md` for the general
   rule.
7. **SDK macro collisions are handled by `#undef`** before the API-layer
   include: `window.cpp:34` `TranslateAccelerator`,
   `perthread.cpp:32` `AddMonitor`,
   `windowcontroller.h:41`–`47` `SendMessage`/`GetProp`/`RemoveProp`/`SetProp`/`GetClassName`.
   Any new member named like an SDK macro needs the same treatment.

## To cover later

- The exact `DeviceContext(ref_of<DeviceContext>::type dc, ...)` construction
  signature and the meaning of the `false` "do not close" argument — deferred to
  [60 — GDI objects](60-gdiobjects.md).
- Whether `winqor_gui.dll` should import `opengl32.dll` (the
  `DynamicLibrary("opengl32")` route implies not) — an audit item, checked with
  the export audit used for the api_layer libraries.

**Next part to detail:** [20 — factories](20-factories.md).



