# 20 — Factories: window classes, styles, handler factories

**Status: detailed.** All 15 files in `gui/factories` have been read in full
(`windowfactory.h/.cpp`, `windowstyle.h/.cpp`, `windowclass.h/.cpp`,
`windowclassregistration.h/.cpp`, `classstyle.h/.cpp`, `handlerfactory.h`,
`dialogbuilder.h/.cpp`, `menubuilder.h/.cpp`).

## Files in this part (15)

| File | Size | Role |
|---|---|---|
| `windowfactory.h` / `windowfactory.cpp` | 5.9 / 12.7 KB | creates windows, owns the message-procedure entry point |
| `windowstyle.h` / `windowstyle.cpp` | 16.3 / 12.3 KB | `WindowStyle` + `ExtendedWindowStyle` named bit masks |
| `windowclass.h` / `windowclass.cpp` | 3.9 / 4.1 KB | `WindowClass` — the `WNDCLASSEX` record as a QOR object |
| `windowclassregistration.h` / `.cpp` | 2.1 / 3.0 KB | `WindowClassRegistration` — `RegisterClassEx`/`UnregisterClass` RAII |
| `classstyle.h` / `classstyle.cpp` | 3.9 / 4.6 KB | `ClassStyle` — the `WNDCLASSEX.style` (`CS_*`) bit vocabulary |
| `handlerfactory.h` | 2.6 KB | `AbstractHandlerFactory` + `HandlerFactory<T>` template |
| `dialogbuilder.h` / `.cpp` | 4.9 / 3.8 KB | declarative dialog item list → `Dialog` window |
| `menubuilder.h` / `.cpp` | 3.1 / 4.0 KB | declarative menu item list → `Menu` via `LoadMenuIndirect` |

## What this area does

It answers "how does a `Window` get made?". `windowfactory.h:49`–`50` states the
contract: *make Window instances from a WindowClass and a WindowViewState;
each Window has a Controller and a Handler*. Three separate vocabularies meet
here.

### 1. The class of window (`WindowClass`)

`qor::platform::win::WindowClass` (`windowclass.h:42`) mirrors the Win32
`WNDCLASS`/`WCLASS` record: style, message function, class/window extra bytes,
instance handle, icon, cursor, background brush, menu name, name, small icon —
each with a getter and a `Set…` (`windowclass.h:48`–`70`), stored as protected
members (`windowclass.h:74`–`85`). The typedef

```cpp
typedef long long (__stdcall *messageFunc)(void*, unsigned int, unsigned long long, long long);
```

(`windowclass.h:40`) is the shape of the window's message procedure.

There is also the QOR traits hook:

```cpp
template<typename T> struct windowclass_of { typedef WindowClass type; };
#define qor_pp_declare_windowclass_of(_CLASS,_WINDOWCLASS) \
    template<> struct windowclass_of<_CLASS> { typedef _WINDOWCLASS type; };
```

(`windowclass.h:89`–`99`) — the intended route for "constructing
`new_ref<MainWindow>()` should find the factory and register the class", the
idea sketched in `examples/experimental/osspecific/windows/mainwindow/some_ideas.txt:6`–`10`.
Whether any specialisation is wired anywhere is a detail-pass question.

### 2. Style bit-vocabulary

Two style objects, both `final`, both convertible to `unsigned int` so they can
be passed where a Win32 mask is expected:

- **`ClassStyle`** (`classstyle.h:36`) — the `WNDCLASS` style bits, named in the
  class itself (`classstyle.h:75`–`88`):
  `vRedraw 0x0001`, `hRedraw 0x0002`, `dblClicks 0x0008`, `ownDC 0x0020`,
  `classDC 0x0040`, `parentDC 0x0080`, `noClose 0x0200`, `saveBits 0x0800`,
  `byteAlignClient 0x1000`, `byteAlignWindow 0x2000`, `globalClass 0x4000`,
  `ime 0x00010000`, `dropShadow 0x00020000`, with `Is…()` queries
  (`classstyle.h:45`–`57`) and `Set…(bool value = true)` setters
  (`classstyle.h:59`–`71`).
- **`WindowStyle`** (`windowstyle.h:36`) and **`ExtendedWindowStyle`**
  (`windowstyle.h:114`) — the per-instance `style`/`exStyle` masks used by
  `CreateWindowEx`. The example uses `SetCaption`, `SetSystemMenu`,
  `SetSizingBorder`, `SetMaximize`, `SetMinimize` and on the extended side
  `SetAcceptFiles`, `SetAppWindow`, `SetWindowEdge`, `SetClientEdge`,
  `SetComposited`, `SetControlParent`, `SetLayered`, `SetToolWindow`,
  `SetTopMost` (`mainwindow.cpp:348`–`380`). Bit definitions: detail pass.

### 3. Handler factories

`AbstractHandlerFactory` (`handlerfactory.h:39`) has exactly one pure virtual:
`ref_of<qor::platform::win::gui::view::AbstractWindowHandler>::type Create() = 0`.
`HandlerFactory<T>` (`handlerfactory.h:51`) implements it as
`return new_ref<T>();`. A caller subclasses it to build a whole tree instead of
one object — the example does that at `mainwindow.cpp:319`–`332`, returning a
`TopLevelWindowHandler` with `m_ncRendering`, `m_nonClient` and `m_rendering`
replaced by custom handlers.

`WindowFactory` keys these by class name
(`windowfactory.h:61`–`63`): `RegisterClass(ref_of<WindowClass>,
ref_of<AbstractHandlerFactory>)`, the name-taking overload, and
`SetHandlerForBuiltinClass(name, factory)`. They are stored in
`std::map<tstring, windowClassData>` (`windowfactory.h:82`) with
`windowClassData = { windowClass, registration, handlerFactory }`
(`windowfactory.h:75`–`80`).

### Creation surface

`WindowFactory(void* instance)` (`windowfactory.h:55`) takes the Win32
instance handle; the example passes `GetInstance()` (`mainwindow.cpp:336`).

| Entry point | Returns | Source |
|---|---|---|
| `Create(className, windowName, style, exStyle, x, y, w, h, parent, menu, param)` | `ref_of<WindowController>` | `windowfactory.h:58` |
| `Create(ref_of<WindowClass>, windowName, …)` | `ref_of<WindowController>` | `windowfactory.h:59` |
| `AddWindowClass(name)` | `ref_of<WindowClass>` | `windowfactory.h:60` |
| `CreateMain(windowName, …)` | `ref_of<WindowController>` | `windowfactory.h:64` |
| `CreateButton(parent, buttonText, …)` | `ref_of<ButtonController>` | `windowfactory.h:65` |
| `CreateEdit(parent, …)` | `ref_of<EditController>` | `windowfactory.h:66` |
| `CreateListBox(parent, …)` | `ref_of<ListBoxController>` | `windowfactory.h:67` |
| `CreateComboBox(parent, …)` | `ref_of<ComboBoxController>` | `windowfactory.h:68` |

`Create`'s default arguments (`windowfactory.h:58`): `style` defaults to
`0x00000000L | 0x00C00000L | 0x00080000L | 0x00040000L | 0x00020000L | 0x00010000L`,
`exStyle` to 0, and `x`, `y`, `width`, `height` to `(int)0x80000000`
(position/size left to Windows).

`WndProc` is a private static with the `__stdcall` convention marker
`qor_pp_compiler_stdcallconvention` (`windowfactory.h:72`) — one message-procedure
entry point per factory, through which the handler tree is reached.

## Creation flow — `WindowFactory` in detail

### Factory construction

```cpp
WindowFactory::WindowFactory(void* instance) : m_instance(instance) {
    auto windowClass = AddWindowClass(L"MainWindow");
    RegisterClass(windowClass, new_ref<HandlerFactory<TopLevelWindowHandler>>());
}
```

(`windowfactory.cpp:43`–`47`) — **exactly one window class is pre-registered,
per factory instance, at construction**: `MainWindow`, mapped to
`HandlerFactory<TopLevelWindowHandler>`. There is no library-load registry and
no static registration: the "preregister all the builtin types" idea noted in
`mainwindow/some_ideas.txt:25` is **not implemented**. The built-in common
controls (`BUTTON`, `EDIT`, `LISTBOX`, `COMBOBOX`) are created straight through
`CreateWindowExT` and never go through this registration at all.

### `WndProc` — the single message-procedure entry point

`windowfactory.h:72` declares it with `qor_pp_compiler_stdcallconvention`;
`windowfactory.cpp:49`–`76` is the whole path from Windows into the library:

```cpp
Window* window = (Window*)(User32::GetPropT((HWND)hwnd, TEXT("QOR")));

if (window == nullptr) {
    auto perThread = new_ref<PerThread>();
    ref_of<Window>::type windowRef = perThread->GetWindowCreationInProgress();
    window = windowRef.operator->();
    if (window) {
        User32::SetPropT((HWND)(hwnd), TEXT("QOR"), (HANDLE)(window));
        window->SetHandle(PrimitiveHandle(hwnd));
    }
}

if (window != nullptr) {
    auto handler = window->GetHandler();
    long long lResult = -1;
    if (handler.IsNotNull() && handler->ProcessMessage(*window, lResult, msg, wParam, lParam))
        return lResult == -1 ? DefWindowProc((HWND)hwnd, msg, wParam, lParam) : lResult;
}
return DefWindowProc((HWND)hwnd, msg, wParam, lParam);
```

Four facts this establishes:

1. **The C++ object pointer travels in a window property named `"QOR"`**
   (`GetPropT`/`SetPropT`, `windowfactory.cpp:51`, `60`). This is the primary
   handle→object bridge at message time; `PerThread::m_windowHandleMap` is a
   *separate* registry used by enumeration/lookup operations elsewhere, not by
   `WndProc`.
2. **`wmCreate` re-entrancy is resolved here, not in the constructor.** `Create`
   parks the half-built `Window` in
   `PerThread::SetWindowCreationInProgress` (`windowfactory.cpp:152`); the first
   `WndProc` Windows makes during `CreateWindowExT` retrieves it, attaches the
   handle (`window->SetHandle(PrimitiveHandle(hwnd))`,
   `windowfactory.cpp:61`) and stamps the `"QOR"` property so later messages
   skip the `PerThread` lookup. That is why
   `GetWindowCreationInProgress()` disposes its ref — single use by design.
3. **`lResult = -1` is the "no result yet" sentinel.** A handler that reports
   success but leaves `lResult` at `-1` still falls through to `DefWindowProc`
   (`windowfactory.cpp:71`).
4. **A window with no handler takes the `DefWindowProc` path**
   (`windowfactory.cpp:67`, `handler.IsNotNull()`) — the situation for every
   control made by `CreateButton`/`CreateEdit`/`CreateListBox`/`CreateComboBox`.

### `Create` — order of operations

`windowfactory.cpp:145`–`155` (the class-name form; the `ref_of<WindowClass>`
form at `:140`–`143` delegates to it):

```cpp
ref_of<Window>::type window = new_ref<Window>();
auto handlerFactory = GetHandlerFactoryForClass(className);
window->SetHandler(handlerFactory->Create());
window->SetLayout(new_ref<VertcialLayout>());
auto perThread = new_ref<PerThread>();
perThread->SetWindowCreationInProgress(window);
User32::CreateWindowExT(exStyle, className.c_str(), windowName.c_str(), style, x, y, width, height,
                        (HWND)(parent.IsNotNull() ? parent->GetHandle().Use() : nullptr),
                        (HMENU)(menu.IsNotNull() ? menu->GetHandle().Use() : nullptr),
                        (HINSTANCE)(m_instance), param);
return new_ref<WindowController>(window);
```

- The return value of `CreateWindowExT` is **discarded** — the handle is
  attached inside `WndProc` during the call. The `Window` exists before the
  `HWND` does; the `WindowController` exists after.
- Every created window gets its own **`VertcialLayout`** as its layout
  (`windowfactory.cpp:150`, `162`; `VertcialLayout : public Layout` is declared
  at `view/layout/layout.h:76`), which is what makes the layout bookkeeping in
  the child-control creators below work.
- **`GetHandlerFactoryForClass` returns a null ref for an unregistered class
  name** (`windowfactory.cpp:129`–`138`), and `Create` dereferences it
  unconditionally at `:149`. An unregistered class name is therefore a null
  dereference, not a graceful failure. `Create(L"MainWindow", ...)` is safe
  because the factory constructor registered it; the example's
  `L"CustomWindow"` is safe because it calls `RegisterClass`
  (`mainwindow.cpp:344`).
- `CreateMain` (`windowfactory.cpp:157`–`169`) is `Create` with the class name
  pinned to `MainWindow` plus `controller->ShowAsync()` before returning
  (`windowfactory.cpp:167`).

### `AddWindowClass` — the defaults a new class gets

`windowfactory.cpp:78`–`97` creates a `WindowClass` and sets:

```cpp
windowClass->SetName(windowClassName.c_str());
windowClass->SetInstance(PrimitiveHandle(m_instance));
windowClass->SetClassExtra(0);
windowClass->SetWindowExtra(0);
windowClass->SetIcon(Icon(IDI_APPLICATION));
windowClass->SetCursor(Cursor(IDC_ARROW));
windowClass->SetBackgroundBrush(Brush(COLOR_WINDOW));
windowClass->SetSmallIcon(Icon(nullptr));
windowClass->SetMessageFunction(WndProc);
m_windowClassMap.insert(std::make_pair(windowClassName, windowClassData{ windowClass, nullptr}));
```

Every class this library makes shares the **same message procedure** — the
factory's static `WndProc` — and the same default icon, cursor and background
brush, built from the `gdiobjects` `Icon`/`Cursor`/`Brush` classes
constructed from an SDK ID constant. The map value is initialised with two
members while `windowClassData` has three (`windowfactory.h:75`–`80`), so the
handler factory member defaults to null.

`WindowClass`'s own constructor (`windowclass.cpp:35`–`41`) zeroes only
`m_style`, `m_messageFunc`, `m_classExtra`, `m_windowExtra`; the handle members
are initialised in-class (`windowclass.h:74`–`85`). `MenuName()` and `Name()`
return `c_str()` of a `tstring` member (`windowclass.cpp:85`, `90`) — valid
only while the object is alive, and `SetMenuName`/`SetName` take a bare
`const TCHAR*` (`windowclass.cpp:143`, `148`).

### `RegisterClass` — what "register" means here

`windowfactory.cpp:99`–`115`: the object-taking overload delegates to the
name-taking one, which does

```cpp
it->second.registration = new WindowClassRegistration(it->second.windowClass()());
it->second.m_handlerFactory = handlerFactory;
```

`WindowClassRegistration`'s constructor
(`windowclassregistration.cpp:35`–`51`) fills a `WNDCLASSEX` from the
`WindowClass` accessors and calls
`m_Atom = User32::RegisterClassExT(&wndClassEx)`; its destructor
(`windowclassregistration.cpp:53`–`56`) calls
`User32::UnregisterClassT(m_windowClass.Name(), instance)`. So **a class
becomes usable at the moment it is given a handler factory** — registration and
handler are coupled in one call, which is why unregistered names crash rather
than fail.

Ownership as written: `registration` is a **raw `WindowClassRegistration*`**
held in the map (`windowfactory.h:77`), never `delete`d, and silently
overwritten by a second `RegisterClass` for the same name — the first
registration's destructor (and hence `UnregisterClassT`) never runs.

`SetHandlerForBuiltinClass` (`windowfactory.cpp:117`–`127`) replaces the
handler factory **without** registering, and requires the name to already be in
the map — so it is for re-targeting an already registered class.

### Child-control creators — a different path entirely

`CreateButton` (`windowfactory.cpp:171`–`190`), `CreateEdit` (`:192`–`212`),
`CreateListBox` (`:214`–`234`) and `CreateComboBox` (`:236`–`256`) share one
shape and bypass `WindowClass`, `RegisterClass`,
`GetHandlerFactoryForClass` and `SetHandler` completely:

```cpp
auto perThread = new_ref<PerThread>();
auto button = new_ref<ButtonController>(new_ref<Button>(
    PrimitiveHandle(
        User32::CreateWindowExT(exStyle, qor_pp_text("BUTTON"), buttonText.c_str(),
            style | WS_BORDER | WS_CHILD | WS_VISIBLE, x, y, width, height,
            (HWND)(parent.IsNotNull() ? parent->GetHandle().Use() : nullptr),
            (HMENU)(unsigned long long)(perThread->NextResourceId()),
            (HINSTANCE)GetWindowLongPtr((HWND)(parent...), GWLP_HINSTANCE),
            nullptr)));
auto layout = parent->GetLayout().AsRef<Layout>();
if (layout.IsNotNull()) layout->AddItem(button->GetWindow()->GetLayout());
```

- The style mask is built from **SDK** names (`WS_BORDER`, `WS_CHILD`,
  `WS_VISIBLE`) — not from `WindowStyle`. `EDIT`, `LISTBOX` and `COMBOBOX`
  additionally add `WS_VSCROLL` (`windowfactory.cpp:198`, `220`, `242`).
- The `hMenu` argument carries `perThread->NextResourceId()`
  (`windowfactory.cpp:178`, `200`, `222`, `244`) — for Windows common controls
  that slot is the control's creation ID, so `PerThread::NextResourceId()` is
  the control-ID source.
- The instance handle is read off the parent
  (`GetWindowLongPtr(parent, GWLP_HINSTANCE)`), so the child inherits the
  parent's registered class instance and therefore the parent factory's
  `WndProc`.
- `BUTTON` passes `buttonText.c_str()` as the **window title**
  (`windowfactory.cpp:176`); the other three pass `qor_pp_text("")`.
- Each control's `Window` is added to the parent's layout
  (`:181`–`188`, `203`–`210`, `225`–`232`, `247`–`254`) via
  `Layout::AddItem(window->GetLayout())`.

Because no handler is attached, messages to these children take the
`DefWindowProc` branch of `WndProc` — the QOR handler tree covers the factory
created windows (`MainWindow`, custom classes), not the common controls.

## `WindowStyle` / `ExtendedWindowStyle` in detail

Both are `final` classes with an `operator unsigned long()` (`windowstyle.h:40`,
`windowstyle.h:118`) so they convert implicitly to the Win32 mask, a private
`m_style`, and a private set of named bit constants each carrying the SDK's own
wording as a trailing comment.

`WindowStyle` bits (`windowstyle.h:88`–`107`):

| Name | Value | Name | Value |
|---|---|---|---|
| `Overlapped` | `0x00000000` | `Border` | `0x00800000` |
| `Caption` | `0x00C00000` | `Child` | `0x40000000` |
| `ClipChildren` | `0x02000000` | `ClipSiblings` | `0x04000000` |
| `Disabled` | `0x08000000` | `DialogFrame` | `0x00400000` |
| `Group` | `0x00200000` | `HScroll` | `0x00100000` |
| `Maximize` | `0x01000000` | `MaximizeBox` | `0x00010000` |
| `Minimize` | `0x20000000` | `MinimizeBox` | `0x00020000` |
| `Popup` | `0x80000000` | `SizingBorder` | `0x00040000` |
| `SystemMenu` | `0x00080000` | `TabStop` | `= MaximizeBox` |
| `Visible` | `0x10000000` | `VScroll` | `0x00200000` |

Default state (`windowstyle.h:109`–`111`):

```cpp
unsigned long m_topLevelStyle{Overlapped | Caption | SystemMenu | SizingBorder | MinimizeBox | MaximizeBox};
unsigned long m_popupStyle = {Popup | Border | SystemMenu};
unsigned long m_style{Overlapped | Caption | SystemMenu | SizingBorder | MinimizeBox | MaximizeBox};
```

`m_style`'s initialiser is **exactly the default `style` argument of
`WindowFactory::Create`** (`windowfactory.h:58`:
`0x00C00000 | 0x00080000 | 0x00040000 | 0x00020000 | 0x00010000`, with
`Overlapped` contributing 0) — a default-constructed `WindowStyle` is what the
factory asks for. That answers the style question: `MainWindow` is created with
caption + system menu + sizing border + min/max boxes.
`m_topLevelStyle` and `m_popupStyle` are **never referenced in
`windowstyle.cpp`** (grep found no use) — dead defaults, presumably intended for
`Create` and a future `CreatePopup`.

Mutual exclusions and implications are enforced in the setters, not the
getters (`windowstyle.cpp:131`–`244`):

- `SetChild(true)` → `SetPopup(false)` (`:141`–`148`);
  `SetPopup(true)` → `SetChild(false)` (`:208`–`215`).
- `SetSystemMenu(true)` → `SetCaption(true)` (`:222`–`229`).
- `SetMaximizeBox(true)` and `SetMinimizeBox(true)` → `SetSystemMenu(true)`
  (`:185`–`192`, `:199`–`206`).
- `IsTabStop()` is `IsChild() && (m_style & TabStop)` (`windowstyle.cpp:116`–`119`)
  — and since `TabStop` **is** `MaximizeBox` (`0x00010000`), a child window with
  a maximize box reports `IsTabStop()`.
- `IsOverlapped()` returns `true` unconditionally (`windowstyle.cpp:31`–`34`),
  matching `Overlapped = 0x00000000`.

`ExtendedWindowStyle` bits (`windowstyle.h:171`–`194`) with their values:
`DialogModalFrame 0x1`, `NoParentNotify 0x4`, `TopMost 0x8`,
`ToolWindow 0x80`, `WindowEdge 0x100`, `ClientEdge 0x200`,
`ContextHelp 0x400`, `MDIChild 0x40`, `AcceptFiles 0x10`,
`LeftScrollBar 0x4000`, `RightAligned 0x1000`, `RTLReading 0x2000`,
`ControlParent 0x10000`, `Layered 0x80000`, `StaticEdge 0x20000`,
`AppWindow 0x40000`, `NoInheritLayout 0x100000`,
`NoRedirectionBitmap 0x200000`, `Composited 0x2000000`,
`LayoutRTL 0x400000`, `NoActivate 0x8000000`, plus the composites
`ExOverlapped {WindowEdge | ClientEdge}` and
`PaletteWindow {WindowEdge | ToolWindow | TopMost}`
(`windowstyle.h:193`–`194`). Its `m_style{0}` (`windowstyle.h:197`) —
extended styles default to none, and `ExtendedWindowStyle::SetLayered(true)`
has no cross-linking, unlike the `WindowStyle` setters.

`ClassStyle` (`classstyle.h:75`–`88`) is the third mask vocabulary, for
`WNDCLASSEX.style` (`CS_*`): `vRedraw 0x1`, `hRedraw 0x2`, `dblClicks 0x8`,
`ownDC 0x20`, `classDC 0x40`, `parentDC 0x80`, `noClose 0x200`,
`saveBits 0x800`, `byteAlignClient 0x1000`, `byteAlignWindow 0x2000`,
`globalClass 0x4000`, `ime 0x10000`, `dropShadow 0x20000`. Setters are plain
set/clear (`classstyle.cpp:96`–`159`) with no cross-linking, and `operator
unsigned int()` (`classstyle.h:40`) feeds `WindowClass::SetStyle`
(`windowclass.cpp:98`–`101`) → `wndClassEx.style`
(`windowclassregistration.cpp:39`). The example uses this at
`mainwindow.cpp:340`–`343` (`SetHRedraw`, `SetVRedraw`).

## Builders

### `MenuBuilder` — declarative menus

`menubuilder.h:38`–`64` declares the two template records — the plain
`MenuItemTemplateHeader {version, offset}` / `MenuItemTemplate {mtOption, mtID,
mtString[1]}` and the extended `MenuExTemplateHeader {version{1}, offset{4},
helpId}` / `MenuExTemplateItem {dwType, dwState, uId, wFlags}` — matching the
`MENU_TEMPLATE` / `MENU_ITEM` / `MENUEX_TEMPLATE` / `MENUEX_ITEM` layouts
`LoadMenu`/`LoadMenuIndirect` expect. Only the Ex forms are used in
`menubuilder.cpp`.

`AddItem` (`menubuilder.cpp:42`–`53`) appends `{item, type, state, flags}` and
assigns `uId = perThread->NextResourceId()`, returning it — so menu item ids
share the same per-thread allocator as control ids and hot-key ids.

`Build()` (`menubuilder.cpp:70`–`99`) serialises the vector into a raw buffer
and calls `User32::LoadMenuIndirectT`:

```cpp
menu = new_ref<Menu>(PrimitiveHandle(User32::LoadMenuIndirectT(
        reinterpret_cast<const MENUTEMPLATE*>(buffer))));
```

`CalcBufferNeeded` (`menubuilder.cpp:55`–`68`) sizes it as
`sizeof(MenuExTemplateHeader)` + per item
`sizeof(MenuExTemplateItem) + text octets`, each item padded up to an
`unsigned long` boundary. Observations:

- `new byte[bufferByteCount]` (`menubuilder.cpp:74`) is **never
  `delete`d** — the buffer leaks on every `Build()`; only
  `LoadMenuIndirectT` consuming it is guaranteed.
- The item-name loop writes `for (TCHAR c : item.second)`
  (`menubuilder.cpp:85`–`90`) with **no terminating NUL**, so the
  `MENUEX_ITEM` text Windows reads is not terminated as the SDK requires. The
  padding loop that follows (`:91`–`95`) zero-fills to the dword boundary, which
  supplies a NUL only when the text does not end flush with the boundary.
- `menubuilder.h:74` `AddItem(...)` takes `type`/`state` as `unsigned long` and
  `flags` as `unsigned short` matching `dwType`/`dwState`/`wFlags`.
- `MenuItemTemplateHeader` / `MenuItemTemplate` (the non-Ex forms) are declared
  but unused anywhere in this file — a non-Ex `Build()` does not exist.

### `DialogBuilder` — half-implemented

`dialogbuilder.h:37`–`111` declares the description types and the builder, but
`dialogbuilder.cpp` implements **only** the three view-model constructors
(`:37`–`50`) and `AddStatic` (`:52`–`62`) and `AddButton` (`:64`–`73`).
`AddEdit`, `AddList`, `AddCombo`, `AddScroll`, `Build()` and
`CalcBufferNeeded()` are **declared and never defined**, so any call to them is
a link error; nothing in `examples/` calls them (grep over
`examples` for `DialogBuilder` found no uses — the only builder use in the tree
is `menubuilder.h` included by `examples/experimental/osspecific/windows/menu/main.cpp:28`,
an example whose directory is commented out of
`examples/experimental/osspecific/windows/CMakeLists.txt:5`).

`AddStatic`/`AddButton` mirror `MenuBuilder::AddItem`: a zero `id` argument is
replaced with `perThread->NextResourceId()` (`dialogbuilder.cpp:54`–`58`,
`66`–`70`) and the id is returned for the caller to use as a control id.
The item view models carry the Win32 dialog-record fields directly:
`StaticDialogItemViewModel::m_classId{0x0082}` and
`ButtonDialogItemViewModel::m_classId{0x0080}`
(`dialogbuilder.h:60`, `72`) — the `STATIC_TEXT_16`/`BUTTON` control-class
identifiers — with `m_classHeader{0xFFFF}` the
`CREATESTRUCT`/item-header marker, plus `m_message` and `m_creationData`.
`dialogbuilder.h:33` includes `"../windows/a_h/dialog.h"`, so the built
`Dialog` type is the one documented in [part 40](40-window-types.md).

## Answers to the questions that framed this part

1. **`Create` does not use the `PerThread` window map.** The handle→object link
   is the `"QOR"` window property, established in `WndProc`
   (`windowfactory.cpp:51`, `60`); `PerThread` only carries the
   creation-in-progress slot (`windowfactory.cpp:152`) during
   `CreateWindowExT`. The handler is attached by the **factory**
   (`windowfactory.cpp:149`), never by the controller; the controller only sets
   the window's back-pointer (`windowcontroller.h:148`).
2. **Only `MainWindow` is pre-registered**, and only per factory instance, in
   the factory constructor (`windowfactory.cpp:43`–`47`). No library-load
   registration exists; `WindowClassRegistration` is constructed on demand by
   `RegisterClass` (`windowfactory.cpp:110`).
3. **No `windowclass_of` specialisation exists anywhere** — `grep` over
   `src/` returns only the definition and the macro
   (`windowclass.h:89`–`99`). The "construct the QOR class, the factory
   registers it" idea in `mainwindow/some_ideas.txt:6`–`10` is unimplemented
   machinery, not a wired feature.
4. **The example's default style is a default `WindowStyle`** (see the
   `m_style` initialiser match above); its extended bits are set one by one via
   `ExtendedWindowStyle` setters (`mainwindow.cpp:348`–`380`).

## Gotchas

1. **`Create` with an unregistered class name dereferences a null ref**
   (`windowfactory.cpp:149`) — there is no diagnostic.
2. **`RegisterClass` leaks the previous registration** for a name
   (`windowfactory.cpp:110`, raw `new` into `windowClassData::registration`).
3. **Common controls get no QOR handler** — `CreateButton`/`CreateEdit`/`CreateListBox`/`CreateComboBox`
   skip `SetHandler` entirely, so their messages fall to `DefWindowProc`.
4. **`MenuBuilder::Build` leaks its buffer and writes unterminated item text**
   (`menubuilder.cpp:74`, `85`–`90`).
5. **`DialogBuilder` is a partial implementation** — `Build()` has no definition.
6. `windowfactory.cpp:38`–`39` do `#undef RegisterClass` and `#undef GetClassName`
   before the User32 layer include, the same SDK-macro-collision treatment noted
   in [part 10](10-core-root.md).
7. `windowfactory.cpp:36` pulls `using namespace qor::platform::win::gui::view;`
   so `TopLevelWindowHandler` and `VertcialLayout` resolve unqualified.

**Next:** [30 — controllers](30-controllers.md) ·
[back to index](README.md)


