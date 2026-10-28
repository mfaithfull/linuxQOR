# 30 — Controllers: `WindowController` and the control subclasses

**Status: detailed** (levels 1–4 complete). `windowcontroller.h` (325 lines)
and `windowcontroller.cpp` (858 lines) read in full at Level 1; the control
controllers read at Levels 2–3; the example's live call sites recorded at
Level 4. See the per-level status lines below.

## Files in this part (28)

Subfolders are alphabetical ranges: `a_h`, `i_m`, `n_r`, `s_s`.

| Controller pair | Folder | Sizes (h / cpp) |
|---|---|---|
| `windowcontroller` | root | 13.9 / 30.9 KB |
| `animationcontroller` | `a_h` | 2.2 / 2.2 KB |
| `buttoncontroller` | `a_h` | 2.8 / 4.8 KB |
| `caret` | `a_h` | 2.3 / 2.8 KB |
| `comboboxcontroller` | `a_h` | 4.4 / 11.8 KB |
| `editcontroller` | `a_h` | 4.1 / 11.1 KB |
| `listboxcontroller` | `i_m` | 4.4 / 13.4 KB |
| `mdiclientcontroller` | `i_m` | 2.1 / 2.3 KB — **not in `add_library`** |
| `menu` | `i_m` | 5.8 / 7.8 KB |
| `monitor` | `i_m` | 3.9 / 8.9 KB |
| `mouse` | `i_m` | 2.8 / 3.5 KB |
| `popupmenu` | `n_r` | 2.3 / 2.4 KB |
| `scrollbarcontroller` | `s_s` | 2.8 / 3.7 KB |
| `staticcontroller` | `s_s` | 2.8 / 4.5 KB |

## Inheritance

```
WindowController                      windowcontroller.h:136
├── AnimationController               a_h/animationcontroller.h:36
├── ButtonController                  a_h/buttoncontroller.h:36
├── ComboBoxController                a_h/comboboxcontroller.h:45
├── EditController                    a_h/editcontroller.h:33
├── ListBoxController                 i_m/listboxcontroller.h:33
├── MDIClientController               i_m/mdiclientcontroller.h:33
├── ScrollBarController               s_s/scrollbarcontroller.h:33
├── StaticController                  s_s/staticcontroller.h:39

Menu                                  i_m/menu.h:68
└── PopupMenu                         n_r/popupmenu.h:39

Monitor  Mouse  Caret                 standalone (i_m/monitor.h:60, i_m/mouse.h:52, a_h/caret.h:38)
```

Every control controller derives from `WindowController`, so a control *is* a
window operation surface — this mirrors the Windows truth that every common
control is an HWND of a built-in class. `Menu`/`PopupMenu` form the parallel
HMENU surface.

## Level 1 — `WindowController` (h + cpp read in full)

**Status: detailed for `windowcontroller.h` (325 lines) and `windowcontroller.cpp`
(858 lines).** The control controllers below it are still level-1 inventory only.

### The shape of the header

Guard `QOR_PP_H_WINDOWS_GUI_WINDOWCONTROLLER` (`windowcontroller.h:25`–`26`).
Includes pull in the object model this controller sits on: `../window.h`,
`../perthread.h`, `../gdiobjects/devicecontext.h`, `../gdiobjects/region.h`,
`../view/drawing/size.h`, plus `common/structures.h` and `common/handles/handle.h`
(`windowcontroller.h:28`–`39`).

The header defuses SDK macros **before** the api_layer headers are pulled by its
`.cpp`: `#ifdef SendMessage → #undef SendMessage, GetProp, RemoveProp, SetProp,
GetClassName` (`windowcontroller.h:41`–`47`). Same collision treatment as
[part 10](10-core-root.md) and [part 20](20-factories.md).

`windowcontroller.h:49` states the interface rule verbatim:
*"All types on this interface must be portable"* — the parameter types are QOR
PODs (`Rect`, `Point`, `Size`, `BlendFunction`, `PaintStruct`, `Region`), never
SDK typedefs. The two callback typedefs it relies on are declared in
`window.h:87`–`88` (`wndenumproc`, `propenumproc` — QOR-portable callables taking
`Window*`), distinct from the SDK-side `PropEnumProc`/`PropEnumProcEx` at
`window.h:44`–`45`.

Between the includes and the class sit the **Win32 INFO records as QOR PODs**,
each `qor_pp_module_interface(QOR_WINGUI)` and each self-sizing through
`cbSize{sizeof(X)}`:

| POD | Lines | Fields that matter |
|---|---|---|
| `Dwm_BlurBehind` | `windowcontroller.h:52`–`58` | `dwFlags`, `fEnable`, `hRgnBlur`, `fTransitionOnMaximized` |
| `countTitleBarChildren = 5` | `windowcontroller.h:60` | sizes `TitleBarInfo::rgstate[6]` |
| `TitleBarInfo` | `windowcontroller.h:63`–`68` | `cbSize`, `Rect rcTitleBar`, `rgstate[5+1]` |
| `WindowInfo` | `windowcontroller.h:70`–`82` | `rcWindow`, `rcClient`, `dwStyle`, `dwExStyle`, `dwWindowStatus`, border sizes, `atomWindowType`, `wCreatorVersion` |
| `WindowPlacement` | `windowcontroller.h:84`–`95` | `length`, `flags`, `showCmd`, `ptMinPosition`, `ptMaxPosition`, `rcNormalPosition`; a `#ifdef _MAC rcDevice` block (`92`–`94`) that can never trigger on Windows |
| `UpdateLayeredWindowInfo` | `windowcontroller.h:97`–`109` | dst/src HDC + point + size, `crKey`, `const BlendFunction*`, `dwFlags`, `const Rect* prcDirty` |
| `AltTabInfo` | `windowcontroller.h:111`–`122` | items/columns/rows, focus cell, item size, `ptStart` |
| `MenuBarInfo` | `windowcontroller.h:125`–`134` | `rcBar`, `hMenu`, `hwndMenu`, bitfields `fBarFocused:1`, `fFocused:1`, `fUnused:30` |

These exist so a caller can fill one QOR struct and have the controller hand its
address to the Win32 call (see below) — the structs are the marshalling targets,
not a QOR abstraction over them.

### The class and its two constructors

```cpp
class qor_pp_module_interface(QOR_WINGUI) WindowController   // windowcontroller.h:136
{
public:
    WindowController()                       { m_perThread = new_ref<PerThread>(); }                 // 140–143
    WindowController(ref_of<Window>::type window) : m_window(window)
    { m_perThread = new_ref<PerThread>(); m_window->SetController(this); }                           // 145–149
    virtual ~WindowController() = default;                                                           // 151
protected:
    ref_of<Window>::type   m_window;        // windowcontroller.h:319
    ref_of<PerThread>::type m_perThread;   // windowcontroller.h:320
};
```

`m_perThread` is the current thread's singleton (`new_ref<PerThread>()` binds to
whatever thread constructs the controller — see
[part 10](10-core-root.md#perthread--the-thread-singleton)). The window-taking
constructor is the one that matters: it owns the `Window` ref and publishes the
raw back-pointer into the window. The default constructor leaves `m_window` empty,
and every method below dereferences it (see *Gotchas*).

### The wrapper contract in the `.cpp`

The whole file is a catalogue of the User32 window message surface. Four
mechanisms repeat, and once they are known the file reads fast:

```cpp
#define WINDOW_HANDLE (HWND)(m_window->GetHandle().Use())   // windowcontroller.cpp:47
...
#undef WINDOW_HANDLE                                       // windowcontroller.cpp:858
```

`WINDOW_HANDLE` is used on **132 lines** — it is the single place the HWND is
extracted from the owned window ref. Alongside it, `using namespace qor::win::api;`
(`windowcontroller.cpp:36`) and a further `#undef` block (`37`–`45`) defuse
`GetModuleFileName`, `GetAltTabInfo`, `TranslateAccelerator`, `EnumProps`,
`SendMessage`, `GetProp`, `SetProp`, `RemoveProp`, `GetClassName` before the
`api_layer/user/user32.h`, `dwm/dwm.h`, `shell/shell.h`, `uxtheme/uxtheme.h`
includes (`31`–`34`).

Measured counts over the file: **129** `User32::` call sites, **3** calls into
`Shell::`/`DWM::`/`UXTheme::`, **42** `reinterpret_cast` occurrences, **76** lines
carrying `TRUE`/`FALSE`. Every wrapper therefore does: QOR POD → SDK POD cast,
handle extraction, call, then `? true : false` to collapse the SDK `BOOL` into a
C++ `bool`.

### The surface, in header order

The declarations at `windowcontroller.h:153`–`316` and the definitions in
`windowcontroller.cpp` follow the same order, so the groups below pair declaration
and implementation:

| Group | Declarations (h) | Implementations (cpp) | API reached |
|---|---|---|---|
| Geometry & style | `AdjustRect`/`AdjustRectForDpi` `153`–`154`, `GetStyle` `314`, `GetDPI` `283` | `60`–`68`, `55`–`58`, `778`–`781` | `AdjustWindowRectEx(ForDpi)`, `GetWindowLongPtrT(GWL_STYLE)`, `GetDpiForWindow` |
| Enable/disable, hotkeys | `155`–`158` | `70`–`88` | `EnableWindow`, `IsWindowEnabled`, `RegisterHotKey`, `UnregisterHotKey` |
| Animation & iconic layout | `159`–`162` | `90`–`108` | `AnimateWindow`, `ArrangeIconicWindows`, `BringWindowToTop`, `CascadeChildWindows` |
| Window tree | `163`–`169` | `110`–`171` | `CascadeWindows`, `ChildWindowFromPoint(Ex)`, `CloseWindow`, `DeregisterShellHookWindow`, `EndTask`, `EnumChildWindows` |
| Queries (`Get*`/`Is*`) | `174`–`202` | `173`–`311` | `GetAltTabInfoT`, `GetAncestor`, `GetClientRect`, `GetLastActivePopup`, `GetLayeredWindowAttributes`, `GetTitleBarInfo`, `GetTopWindow`, `GetWindow`, `GetWindowInfo`, `GetWindowModuleFileNameT`, `GetWindowPlacement`, `GetWindowRect`, `GetWindowText(T|LengthT)`, `GetWindowThreadProcessId`, `IsChild/HungAppWindow/Iconic/Window/WindowUnicode/Visible/Zoomed`, `LogicalToPhysicalPoint`, `MoveWindow`, `OpenIcon`, `PhysicalToLogicalPoint` |
| Placement & z-order | `202`–`219` | `333`–`428` | `RealChildWindowFromPoint`, `RealGetWindowClassT`, `RegisterShellHookWindow`, `SetForegroundWindow`, `SetLayeredWindowAttributes`, `SetParent`, `SetWindowPlacement`, `SetWindowPos` (five z-order variants), `SetWindowTextT`, `ShowOwnedPopups`, `ShowWindow(Async)` |
| Composition (layered window) | `221`–`223` | `435`–`455` | `TileWindows`, `UpdateLayeredWindow(Indirect)` |
| Menu surface | `225`–`233` | `466`–`515` | `DrawMenuBar`, `GetMenu`, `GetMenuBarInfo`, `GetMenuItemRect`, `GetSystemMenu`, `MenuItemFromPoint`, `HiliteMenuItem`, `SetMenu` |
| Painting & device contexts | `235`–`250` | `517`–`610` | `BeginPaint`/`EndPaint`, `DrawAnimatedRects`, `DrawCaption`, `GetUpdateRect/Rgn`, `GetWindowDC`, `GetWindowRgn(Box)`, `InvalidateRect/Rgn`, `LockWindowUpdate`, `RedrawWindow`, `SetWindowRgn`, `UpdateWindow`, `ValidateRect/Rgn` |
| DC access & coordinate mapping | `252`–`258` | `612`–`659` | `GetDC(Ex)`, `ReleaseDC`, `ClientToScreen`, `ScreenToClient`, `MapWindowPoints`, `TranslateAcceleratorT` |
| Caret & drag | `259`–`262` | `661`–`682` | `HideCaret`, `ShowCaret`, `DragDetect`, `MonitorFromWindow` |
| Properties | `264`–`270` | `684`–`713` | `EnumPropsT` (via trampoline), `GetPropT`, `RemovePropT`, `SetPropT` |
| Class/word/long accessors | `271`–`281` | `715`–`771` | `FlashWindow`, `Get/SetClassLongPtrT`, `Get/SetClassWord`, `Get/SetWindowLongT`, `Get/SetWindowLongPtrT` |
| IME, help, events, timers | `282`–`290` | `773`–`811` | `SendIMEMessageEx`, `Get/SetWindowContextHelpId`, `WinHelp`, `NotifyWinEvent`, `Set/KillTimer` |
| Shell interaction | `294` | `813`–`816` | `Shell::DragAcceptFiles` |
| DWM | `297` | `818`–`825` | `DWM::EnableBlurBehindWindow` |
| Theme | `308` | `827`–`834` | `UXTheme::SetWindowTheme` |
| Focus & destruction | `310`–`311` | `836`–`849` | `SetFocus`, `DestroyWindow` |
| Identity | `315` (`virtual unsigned short GetId() const`) | `851`–`854` | none — returns `0` |

`GetId()` is the **only virtual** in the class (`windowcontroller.h:315`), and the
base returns `0` (`windowcontroller.cpp:851`–`854`). It is the polymorphic hook the
control controllers use to identify the control they drive; everything else is
resolved by the static type.

### Reverse mapping: handle → QOR object

Any Win32 call that returns an `HWND` is converted back through the thread's
handle map before it is returned, so callers only ever see QOR `Window*`:

| Method | Lines | Lookup used |
|---|---|---|
| `ChildWindowFromPoint` (both overloads) | `windowcontroller.cpp:122`–`138` | `perThread->FromHandle` |
| `GetAncestor` | `179`–`186` | `perThread->FromHandle` |
| `GetLastActivePopup` | `193`–`200` | `perThread->FromHandle` |
| `GetTopChild` (→ `GetTopWindow`) | `219`–`226` | `perThread->FromHandle` |
| `GetRelatedWindow` (→ `GetWindow`) | `228`–`235` | `perThread->FromHandle` |
| `SetParent` | `371`–`378` | `perThread->FromHandle` |
| `RealChildWindowFromPoint` | `333`–`340` | `perThread->FromHandle` |
| `GetParent` | `457`–`464` | `m_perThread->FromHandle` |
| `SetFocus` | `836`–`842` | `m_perThread->FromHandle` |
| `GetMenu`, `GetSystemMenu` | `471`–`477`, `489`–`495` | `m_perThread->MenuFromHandle` |
| `BeginPaint`, `GetDC`, `GetADC` (both) | `517`–`523`, `550`–`560`, `612`–`634` | `m_perThread->DeviceContextFromHandle` |
| `ToMonitor` | `676`–`682` | `m_perThread->MonitorFromHandle` |

Two consequences read straight off the code: a handle Windows returns for a window
this thread never registered yields **`nullptr`**, and `DeviceContextFromHandle` is
the one lookup that *fabricates* a wrapper for an unseen HDC
(`perthread.cpp:171`–`176`, recorded in [part 10](10-core-root.md)).

### The enum-callback trampoline idiom

`EnumChildWindows` and `EnumProps` take **portable** QOR callbacks
(`wndenumproc`/`propenumproc`, `window.h:87`–`88`), but Win32 wants a `__stdcall`
C function with SDK parameter types. The file bridges that with a fixed C shim
plus thread-held state:

```cpp
BOOL __stdcall ChildWindowEnumProc(HWND hwnd, LPARAM param)   // windowcontroller.cpp:155–162
{
    auto perThread = new_ref<PerThread>();
    wndenumproc enumProc = perThread->GetWindowEnumProc();
    Window* window = perThread->FromHandle(hwnd);
    return (*enumProc)(window, param) ? TRUE : FALSE;
}

bool WindowController::EnumChildWindows(wndenumproc enumProc, long long param)   // 164–171
{
    auto perThread = new_ref<PerThread>();
    perThread->SetWindowEnumProc(enumProc);
    bool result = User32::EnumChildWindows(WINDOW_HANDLE, &ChildWindowEnumProc, param) ? true : false;
    perThread->SetWindowEnumProc(nullptr);
    return result;
}
```

`PropertiesEnumProc` (`windowcontroller.cpp:684`–`690`) and `EnumProps`
(`692`–`698`) repeat the pattern through
`SetPropertyEnumProc`/`GetPropertyEnumProc` (`perthread.h:77`–`78`,
`perthread.cpp:98`–`106`). The trampoline re-derives the `Window*` for each visited
HWND, so the callback sees QOR objects only.

Windows API intent: the only way to hand a C enumerador a QOR-aware callback. The
side effect is that the slot is set-then-cleared, so **one enumeration per thread at
a time** — nested or concurrent enumerations would overwrite each other's callback.

### Layered-window attributes are a matched pair

The colour-key and alpha accessors split the single Win32 call by flag:

```cpp
SetColourKey(crKey)  → SetLayeredWindowAttributes(WINDOW_HANDLE, crKey, 0, 0x00000001)   // cpp:361–364
SetAlpha(alpha)      → SetLayeredWindowAttributes(WINDOW_HANDLE, 0, alpha, 0x00000002)   // cpp:366–369
GetColourKey(k)      → GetLayeredWindowAttributes(WINDOW_HANDLE, &colourKey, nullptr, &flags)  // flags = 0x00000001, cpp:202–206
GetAlpha(a)          → GetLayeredWindowAttributes(WINDOW_HANDLE, nullptr, &alpha, &flags)      // flags = 0x00000002, cpp:208–212
```

`GetColourKey`/`GetAlpha` pass `nullptr` for the unused out-parameter and a
**non-null** `flags` as the in/out selector — the QOR signature hides that
`flags` is both an argument and a result (`windowcontroller.h:178`–`179`).

### Device-context release discipline

Three DC-returning methods mark the wrapper as needing release; one deliberately
does not:

| Method | Lines | Release handling |
|---|---|---|
| `GetDC` | `windowcontroller.cpp:550`–`560` | `dc->MustRelease()` when the HDC is non-null |
| `GetADC()` / `GetADC(rgnClip, flags)` | `612`–`622` / `624`–`634` | `dc->MustRelease()` when non-null |
| `BeginPaint` | `517`–`523` | **no** `MustRelease()` — the DC belongs to the paint cycle |
| `ReleaseDC` | `636`–`639` | the explicit release path |
| `EndPaint` | `535`–`538` | closes the paint cycle (`PaintStruct` passed back in) |

`MustRelease()` is the flag that tells the eventual releaser
(`gdiobjects/devicecontext.h`, part 60) that this HDC came from a
`Get…DC`-style call. `BeginPaint`'s DC is paired with `EndPaint` instead, which is
why it skips the flag — `paintsession.cpp:41`–`42` uses the same
`perThread->DeviceContextFromHandle(hdc)` lookup without any release marker.

### Not implemented / deferred, as the file states it

- Commented-out prototypes, i.e. deliberately unwrapped: `FindWindowExT`
  (`windowcontroller.h:171`–`173`), `EnumPropsExT` (`265`–`267`), `PrintWindow`
  (`291`).
- The whole DWM attribute block is comment-only: `ExtendFrameIntoClientArea`,
  `GetWindowAttribute`, `InvalidateIconicBitmaps`, `RegisterThumbnail`,
  `SetIconicLivePreviewBitmap`, `SetIconicThumbnail`, `SetWindowAttribute`,
  `TransitionOwnedWindow` (`windowcontroller.h:299`–`306`). Only
  `DwmEnableBlurBehindWindow` is live (`297` → `818`–`825`).
- HRESULT results are discarded with a to-do comment in both theme/DWM wrappers:
  `DwmEnableBlurBehindWindow` (`windowcontroller.cpp:818`–`825`, `//TODO: Proces
  the HRESULT into an error`) and `SetTheme` (`827`–`834`, `//TODO:: Process the
  HRESULT into an error`). Neither failure is observable to the caller.
- `windowcontroller.h:323` closes the namespaces with the comment
  `}}}//qor::platform::nswindow` — a typo in the comment only; the namespace is
  `qor::platform::win` (`windowcontroller.h:50`).

### Gotchas in the base controller

1. **Every method dereferences `m_window` through `WINDOW_HANDLE`.** The default
   constructor (`windowcontroller.h:140`–`143`) leaves `m_window` empty, so a
   default-constructed controller is only safe until the first call.
2. **`GetClassName` reserves but never sizes its out-string**
   (`windowcontroller.cpp:730`–`736`): `className.reserve(maxCount)`, then a write
   through `className.data()`, then `shrink_to_fit()`. `reserve` does not change
   `size()`, so the text Windows wrote is not readable through the string — the
   caller sees an empty string and only the return count. Contrast
   `GetAltTabInfo` (`windowcontroller.cpp:173`–`177`), which does
   `itemText.resize(512)` first and passes `size() - 1` as the capacity.
3. **`GetText` is capped at 1023 characters** (`windowcontroller.cpp:260`–`266`,
   `TCHAR buffer[1024]`, request `1023`); `GetTextLengthT` (`268`–`271`) reports the
   true length, so truncation is detectable only by comparing the two.
   `GetModuleFileNameW` (`242`–`248`) is capped at `MAX_PATH` the same way.
4. **`Redraw` is the only region-taking method that guards a null ref**
   (`windowcontroller.cpp:589`: `rgnUpdate.IsNull() ? (HRGN)nullptr : …`).
   `GetRgn` (`562`–`565`), `SetRgn` (`592`–`595`), `InvalidateRgn` (`577`–`580`),
   `ValidateRgn` (`607`–`610`), `GetUpdateRgn` (`545`–`548`) and `GetADC`
   (`624`–`634`) dereference the region ref unconditionally.
5. **`Destroy()` disposes the owned ref and leaves the controller live**
   (`windowcontroller.cpp:844`–`849`: `User32::DestroyWindow` then
   `m_window.Dispose()`); any later call re-dereferences a disposed ref.
6. **Thread spelling differs inside the file.** `m_perThread` is bound at
   construction; the trampolines and the handle-returning getters build a fresh
   `new_ref<PerThread>()` for the *calling* thread (12 occurrences in the `.cpp`
   against 10 uses of `m_perThread`). Same singleton while a window's messages stay
   on one thread — and that is exactly the rule `purpose.md` lines 11–13 insist on.
7. **Vector marshalling is manual.** `CascadeWindows` (`windowcontroller.cpp:110`–`120`)
   and `TileChildren` (`435`–`444`) build a `std::vector<HWND>` from the QOR refs and
   pass `.size()` explicitly — the QOR refs never cross the API boundary.
8. **`swNormal` default.** `Show(int how = swNormal)` (`windowcontroller.h:218`)
   defaults to `constants.h:59` (`constexpr int swNormal = 1`).
9. **`GetId()` returns 0 in the base** — an override is required to identify a
   control; nothing in the base sets it.

## Level 2 — the control controllers (`a_h`, `i_m`, `s_s`, `n_r`)

**Status: detailed for `animationcontroller.h/.cpp`, `caret.h/.cpp`,
`mouse.h/.cpp`, `popupmenu.h/.cpp`, `mdiclientcontroller.h/.cpp`, `buttoncontroller.h/.cpp`,
`staticcontroller.h/.cpp`, `scrollbarcontroller.h/.cpp`, `menu.h/.cpp`, and the
headers of `editcontroller.h`, `listboxcontroller.h`, `monitor.h`,
`comboboxcontroller.h`.** Still unread: `editcontroller.cpp`, `listboxcontroller.cpp`,
`monitor.cpp`, and most of `comboboxcontroller.cpp`.

### `controllers/` is not one kind of type

Two structurally different things live in the same directory:

- **Controllers** — `class X : public WindowController` with the same two
  constructors and a `GetId()` override: `Animation`, `Button`, `ComboBox`,
  `Edit`, `ListBox`, `MDIClient`, `ScrollBar`, `Static`.
- **Standalone message objects** — no `WindowController` base, holding a raw
  `Handle m_handle` instead of a window ref: `Menu`, `PopupMenu` (derives from
  `Menu`), `Monitor`, `Mouse`, `Caret`.

That distinction decides how each file reaches the API. Controllers use the
`WINDOW_HANDLE` macro over the owned window ref; the standalone objects define
their own macro over their own handle (`menu.cpp:36` and `popupmenu.cpp:36`:
`#define MENU_HANDLE ((HMENU)(m_handle.Use()))`).

### The uniform controller constructor

Identical in every controller `.cpp` — the concrete object ref is narrowed to a
`Window` ref and the back-pointer is published:

```cpp
ButtonController::ButtonController(ref_of<Button>::type button) : WindowController(button.AsRef<Window>())
{ button->SetController(this); }                    // buttoncontroller.cpp:39–42
```

`AsRef<Window>()` (`ref_of<X>::type` → `ref_of<Window>::type`) is what lets a
`Button` (a window type from `windows/`, part 40) be handed to the base
`WindowController` ctor. The default ctor in each file is `: WindowController(){}`
(`buttoncontroller.cpp:38`, `staticcontroller.cpp:40`, `scrollbarcontroller.cpp:39`,
`animationcontroller.h:40` + `.cpp` equivalent) — again leaving `m_window` empty.

Each `.cpp` re-`#define`s `WINDOW_HANDLE` and `#undef`s it at the end
(`buttoncontroller.cpp:35`/`112`, `scrollbarcontroller.cpp:35`/`92`,
`staticcontroller.cpp:36`/`99`) — the macro is per-TU, not shared state.

### The thinnest controllers are `GetId()`-only

`AnimationController` (`animationcontroller.h:36`–`44`, `.cpp:38`–`49`) and
`MDIClientController` (`mdiclientcontroller.h:33`–`42`, `.cpp:39`–`48`) declare
nothing beyond the ctor pair, dtor and `GetId()`. Both `GetId()` bodies are the same
one call:

```cpp
return static_cast<unsigned short>(User32::GetDlgCtrlID(WINDOW_HANDLE));
```

`GetDlgCtrlID` is the *only* Win32 call those types make, and it is the same line in
`ComboBoxController::GetId` (`comboboxcontroller.cpp:45`–`48`),
`ScrollBarController::GetId` (`scrollbarcontroller.cpp:45`–`48`),
`StaticController::GetId` (`staticcontroller.cpp:46`–`49`) and
`ButtonController::GetId` (`buttoncontroller.cpp:105`–`108`). The control's
message surface is therefore whatever its window type supports; the controller only
adds an id.

### Button and Static drive controls through `SendMessage`

`ButtonController` (`buttoncontroller.cpp:44`–`108`) is the Win32 *message* surface
for `BM_*` messages, all through `User32::SendMessageT`:

| Method | Message | Lines |
|---|---|---|
| `GetCheck` / `SetCheck` | `BM_GETCHECK` / `BM_SETCHECK` | `65`–`68` / `70`–`73` |
| `GetState` / `SetState` | `BM_GETSTATE` / `BM_SETSTATE` | `95`–`98` / `100`–`103` |
| `GetBitmap`/`SetBitmap`, `GetCursor`/`SetCursor`, `GetIcon`/`SetIcon` | `BM_GETIMAGE`/`BM_SETIMAGE` with `IMAGE_BITMAP`/`IMAGE_CURSOR`/`IMAGE_ICON` | `44`–`53`, `75`–`83`, `85`–`93` |
| `GetButtonStyle` | **not a message** — `GetStyle() & 0xFFFF` (inherited base accessor) | `55`–`58` |
| `SetButtonStyle` | `BM_SETSTYLE`, `redraw` passed as `LPARAM` | `60`–`63` |

`StaticController` (`staticcontroller.cpp:51`–`95`) is the same shape with `STM_*`
messages, adding `IMAGE_ENHMETAFILE` (`61`–`65`, `85`–`89`). It needs one extra
defuse: `#undef GetEnhMetaFile` (`staticcontroller.cpp:35`) because the SDK defines
that macro and the class declares a method of the same name.

The image setters return the *handle Windows echoes back* as a fresh non-owning
ref: `new_ref<Bitmap>(..., false)` — `false` meaning "not owned by the ref".
`GetBitmap`/`GetCursor`/`GetIcon`/`GetEnhMetaFile` do the same on the returned
handle.

### ScrollBar hardcodes one bar

`ScrollBarController` (`scrollbarcontroller.h:53`–`60`, `scrollbarcontroller.cpp:50`–`88`)
wraps seven messages, and **every call passes `SB_CTL`** — the vertical bar:

```cpp
EnableScrollBar(WINDOW_HANDLE, SB_CTL, arrowFlags)          // cpp:50–53
GetScrollInfo / GetScrollPos / GetScrollRange               // cpp:55–68
SetScrollInfo / SetScrollPos / SetScrollRange / ShowScrollBar  // cpp:70–88
```

There is no `SB_HDR` path anywhere, so a horizontal bar is not addressable through
this controller. The header's default argument `EnableScrollBar(unsigned int
arrowFlags = ScrollBar::EnableBoth)` (`scrollbarcontroller.h:53`) comes from the
window type `ScrollBar` (`windows/s_s/scrollbar.h`, part 40), not from the
controller.

`ScrollInfo` (`scrollbarcontroller.h:33`–`42`) is the Win32 `SCROLLINFO` record as a
QOR POD with `cbSize{sizeof(ScrollInfo)}` and zero-initialised `fMask`, `nMin`,
`nMax`, `nPage` — `nPos` and `nTrackPos` are deliberately uninitialised because
`GetScrollInfo` (`scrollbarcontroller.cpp:55`–`58`) fills them.

### `Menu` — an object, not a controller

`Menu` (`menu.h:68`–`114`, `menu.cpp:40`–`209`) sits in `controllers/` but has no
`WindowController` base: it owns a `Handle m_handle` (`menu.h:113`) and every
constructor immediately calls `m_handle.DontClose()` (`menu.cpp:40`–`58`) so the
`HMENU` is never closed by ref destruction.

| Constructor | Lines | Meaning |
|---|---|---|
| `Menu()` | `menu.cpp:40`–`43` | `m_handle(User32::CreateMenu())` |
| `Menu(int id)` | `45`–`48` | a menu **id**, not a handle |
| `Menu(const PrimitiveHandle& h)` | `50`–`53` | `m_handle(h.Use())` — copies the raw handle value |
| `Menu(const Menu& m)` | `55`–`58` | copy ctor taking another menu's handle |

`PopupMenu` (`popupmenu.h:39`–`49`, `popupmenu.cpp:40`–`59`) derives from `Menu` and
adds exactly one operation, `Track` → `TrackPopupMenuEx` (`popupmenu.cpp:56`–`59`),
with `PopupMenu()` defaulting to `Menu(User32::CreatePopupMenu())` (`40`–`42`) —
a popup and a normal menu are different `HMENU` kinds created by different calls.
`popupmenu.h:33`–`37` adds `TrackMenuParams` (`cbSize{sizeof(TrackMenuParams)}` plus
`Rect rcExclude`) and `Track` takes a raw `Window*` that it null-guards inline
(`popupmenu.cpp:58`).

Menu message surface (all in `menu.cpp`): `CheckMenuItem`, `EnableMenuItem`,
`AppendMenuT` (three ways: `Append` `75`–`78`, `AppendTextItem` `80`–`89`,
`AppendSubMenu` `91`–`94` with `MF_POPUP`), `CheckMenuRadioItem`, `DeleteMenu`,
`DestroyMenu`, `EndMenu`, `GetMenuDefaultItem`, `GetMenuInfo`, `GetMenuItemCount`,
`GetMenuItemID`, `GetMenuItemInfoT`, `GetMenuItemRect`, `GetMenuState`,
`GetMenuStringT`, `GetSubMenu`, `HiliteMenuItem`, `InsertMenuT`, `InsertMenuItemT`,
`ModifyMenuT`, `RemoveMenu`, `SetMenuDefaultItem`, `SetMenuInfo`,
`SetMenuItemInfoT`, `SetMenuItemBitmaps`. Deliberately unwrapped and left as
comments (`menu.h:106`–`110`): `IsMenu`, `LoadMenuT`, `LoadMenuIndirectT`.

### Cross-referencing gotchas in `Menu`

- **`SetItemBitmaps` throws away its arguments** — `menu.cpp:206`–`209` calls
  `SetMenuItemBitmaps(MENU_HANDLE, position, flags, (HBITMAP)(nullptr), (HBITMAP)(nullptr))`
  despite receiving `Bitmap* bitmapUnchecked, Bitmap* bitmapChecked`. The
  parameter names `bitmapUnchecked, bitmapChecked` are also **swapped** against the
  Win32 signature order (`hbmpChecked, hbmpUnchecked`) — so even if the arguments
  were passed, they would land in the wrong slots. Two bugs in one line.
- **`GetString` repeats the `reserve`-without-`resize` mistake** found in
  `WindowController::GetClassName`: `item.reserve(1024)` then `item.data()` and
  `item.capacity()` (`menu.cpp:151`–`155`). Capacity is not size, so the text
  Windows wrote is not visible to the caller.
- **`AppendTextItem` is the only place a menu id is minted**: it asks the thread
  singleton for `NextResourceId()` (`menu.cpp:80`–`89`, tying `Menu` to
  `PerThread`'s resource-id counter in [part 10](10-core-root.md)), returns that id
  on success, and returns **`-1` from an `unsigned int`** on failure — a caller
  comparing against a valid id sees `4294967295`.
- `GetSubMenu` (`menu.cpp:157`–`164`) returns a raw `Menu*` looked up through
  `MenuFromHandle`, so an unregistered submenu yields `nullptr` (and `subMenu` is
  uninitialised in the declaration at `159` before being assigned at `162` — safe
  only because the assignment is unconditional).
- `GetItemRect` and `HiliteItem` (`141`–`144`, `166`–`169`) take a raw `Window*`
  and guard it: `window ? (HWND)(window->GetHandle().Use()) : nullptr`.
- `SetItemInfo` (`201`–`204`) casts away constness of its `const MenuItemInfo&`
  argument for `SetMenuItemInfoT`, while `InsertItem` (`176`–`179`) passes the
  const ref straight through to `InsertMenuItemT` — the two directions of the same
  record are marshalled differently.
- `menu.h:66` forward-declares `class Window;` (as
  `qor_pp_module_interface(QOR_WINGUI) Window`) *after* including
  `view/drawing/rect.h` and `gdiobjects/bitmap.h` but **not** `window.h` — the
  declaration exists so the two `Window*` parameters (`92`, `96`) compile without
  pulling the window header in.

`Caret` (`caret.h:38`–`50`, `caret.cpp:35`–`73`) is a stateless all-static class:
`Create`, `Destroy`, `GetBlinkTime`, `SetBlinkTime`, `GetPosition`, `SetPosition`,
`Show`, `Hide`. It takes a `Window&` and a raw `Handle*` for the bitmap (`caret.h:42`),
and the null-guard for that bitmap is inline in the call:
`(HBITMAP)(bitmap ? bitmap->Use() : nullptr)` (`caret.cpp:37`).
Note `caret.h` does **not** include `windowcontroller.h` — it pulls
`../../window.h`, `view/drawing/rect.h` and `view/drawing/point.h` instead.
`SetBlinkTime` returns `User32::SetCaretBlinkTime(umseconds)` un-narrowed
(`caret.cpp:50`–`53`), unlike its siblings which use `? true : false`; the
parameter name is misspelled `umseconds` (`caret.h:45`).

`Mouse` (`mouse.h:36`–`64`, `mouse.cpp:37`–`75`) is likewise all-static, wrapping
`GetCapture` (returns a **by-value `Handle`**, `mouse.cpp:37`–`40`),
`GetDoubleClickTime`, `GetMovePointsEx`, `ReleaseCapture`,
`SetDoubleClickTime`, `SwapMouseButton`, `TrackMouseEvent`, plus `_event`
(`mouse.h:59`, `mouse.cpp:52`–`55` → `User32::mouse_event`) — the leading
underscore is how the file avoids colliding with the Win32 `mouse_event` macro.
`mouse.h:36`–`50` declares the Win32 records as PODs (`MouseMovePoint`,
`TrackMouseEvent` with `void* hwndTrack`), and `TrackMouseEvent` is both the name
of the POD and of the method (`mouse.h:63`), which is why the method signature has
to spell `struct TrackMouseEvent*` (`mouse.h:63`, `mouse.cpp:72`–`75`).

`Mouse::GetCapture` and `ReleaseCapture` are the pair used to take over a window
during a message handler, and both act on the *current* window — there is no
`HWND` argument, mirroring Win32's implicit-window behaviour.

`mouse.cpp:77`–`85` re-declares the Win32 prototypes locally
(`static HWND GetCapture(void);`, `static UINT GetDoubleClickTime(void);`, …) —
redundant with `user32.h`, and the file's only reason to exist alongside it.

`Monitor` (`monitor.h:38`–`92`) is the odd one of the three: it is
handle-owning like `Menu` (`Handle m_handle` at `monitor.h:87`) **and** it carries
a private `__stdcall` static — `CreateCallback` (`monitor.h:90`) — because
`Enumerate` (`monitor.h:78`) takes a portable `MonitorEnumProc`
(`monitor.h:38`: `int (__stdcall*)(void*, void*, Rect*, long long)`). Same
trampoline shape as `WindowController::EnumChildWindows`, but the callback type is
SDK-shaped rather than QOR-shaped, and `monitor.h:40`–`58` keeps the Win32 records
as PODs (`DisplayAdapter` is the QOR-friendly one: `tstring m_name`,
`tstring m_description`, `bool m_primary`).

`monitor.h:83`–`84` declare `CreateMonitors`, `EumerateAdapters` and
`EumerateMonitors` — the last two are **misspelled** ("Eumerate") in the header.

### `MDIClientController` — a dead TU with broken includes

`mdiclientcontroller.cpp` is one of the four source files that
[part 00](00-overview.md) lists as compiled into nothing. Its includes are the
reason it can never compile as written:

```cpp
#include "windowcontroller.h"     // mdiclientcontroller.h:28 — no ../ prefix
#include "../perthread.h"         // mdiclientcontroller.cpp:28 — one level short
```

From `controllers/i_m/` those resolve to `controllers/i_m/windowcontroller.h` and
`controllers/perthread.h`, neither of which exists (the real paths are
`controllers/../windowcontroller.h` and `gui/perthread.h`). Other controllers spell
both correctly (`buttoncontroller.h:28` `../windowcontroller.h`,
`buttoncontroller.cpp:28` `../../perthread.h`). The file also includes
`view/messages.h` (`mdiclientcontroller.cpp:29`) — the only include in it that
suggests a planned-but-unimplemented message surface.

**Action: leave as is.** The file is not in `gui/CMakeLists.txt` (the compiled
controller list is `animationcontroller`, `caret`, `menu`, `monitor`, `mouse`,
`windowcontroller`, `staticcontroller`, `buttoncontroller`, `editcontroller`,
`listboxcontroller`, `comboboxcontroller`, `scrollbarcontroller`, `popupmenu` —
`CMakeLists.txt:22`–`33` and `:92`). Fixing it would change the library, not the
docs.

### How a control controller reaches a message handler

`SetController(this)` in the controller ctor (`buttoncontroller.cpp:41`,
`staticcontroller.cpp:43`, `scrollbarcontroller.cpp:42`,
`popupmenu.cpp`/`menu.cpp` do **not** call it) stores the raw controller pointer in
the window object; the handler side is covered in [part 50](50-view-part-1.md) and
`view/handlers/` in [part 20](20-factories.md#the-handler-tree-as-it-relates-to-factories).
The pair is asymmetric by design: the controller owns the ref (`m_window`), the
window keeps the controller as a raw back-pointer.

## Level 3 — the message-driven controllers (`listbox`, `combobox`, `edit`)

**Status: read in full** — `listboxcontroller.cpp` (332 lines),
`comboboxcontroller.cpp` (312 lines), `editcontroller.cpp` (290 lines).

These three are the *entire* Win32 message surface for list/edit controls: every
method is `User32::SendMessageT(WINDOW_HANDLE, <message>, wparam, lparam)`. Nothing
else is called except `GetDlgCtrlID` for `GetId()`. So the controller *is* the
message table — the useful thing to record is which message, which packing, and
where the packing is asymmetric.

### `ListBoxController` — the LB_* table

Header `listboxcontroller.h:33`–`91` groups the declarations as
`Get*/Set*`, then *Single-Selection Operations*, *Multiple-Selection Operations*,
*String Operations* (`listboxcontroller.h:67`–`89` carry those comments). The
`.cpp` follows the same grouping.

| Group | Methods → message | Lines (cpp) |
|---|---|---|
| Counts & items | `GetCount`→`LB_GETCOUNT`, `GetHorizontalExtent`→`LB_GETHORIZONTALEXTENT`, `GetItemData`/`GetItemDataPtr`→`LB_GETITEMDATA`, `GetItemHeight`, `GetItemRect` (two), `GetLocale`→`LB_GETLOCALE`, `GetSel`, `GetText`/`GetTextLen`, `GetTopIndex` | `62`–`133` |
| Item geometry | `ItemFromPoint` → `LB_ITEMFROMPOINT` with `MAKELPARAM(x,y)` | `135`–`140` |
| Setters | `SetColumnWidth`, `SetHorizontalExtent`, `SetItemData`/`SetItemDataPtr`→`LB_SETITEMDATA`, `SetItemHeight`, `SetLocale`, `SetTabStops` (three), `SetTopIndex` | `142`–`204` |
| Single selection | `GetCurSel`/`SetCurSel` | `207`–`216` |
| Multiple selection | `GetAnchorIndex`, `GetCaretIndex`, `GetSelCount`, `GetSelItems`, `SelItemRange`, `SetAnchorIndex`, `SetCaretIndex`, `SetSel` | `219`–`275` |
| Strings | `AddString`→`LB_ADDSTRING`, `DeleteString`, `Dir`, `FindString`, `FindStringExact`, `InsertString`, `ResetContent`, `SelectString` | `278`–`328` |

`listboxcontroller.h:41` declares `virtual int CompareItem(CompareItemStruct*)` and
`listboxcontroller.cpp:50`–`60` implements it as a stub returning `0` (the
`-1/0/1` ordering table is quoted in the comment above it). That is the application
hook Win32 calls when `LB_*` sorting happens; the library leaves it to the caller.

Notable in this file:

- `GetItemData` (`72`–`76`) and `GetItemDataPtr` (`78`–`82`) send **the same message**
  and differ only in the return cast (`DWORD` vs `void*`); likewise
  `SetItemData` (`154`–`159`) vs `SetItemDataPtr` (`161`–`166`). Win32 has one message
  for both, so QOR splits the *type*, not the call.
- `GetText` (`117`–`122`) passes `buffer.c_str()` as the out-parameter with **no
  capacity word and no resize** — `LB_GETTEXT` writes into a buffer the caller has
  not sized. `GetTextLen` (`124`–`128`) is the way to size it first. Same bug family
  as `WindowController::GetClassName` and `Menu::GetString`.
- `ItemFromPoint` (`135`–`140`) is the only place a packed result is unpacked:
  `isOutside = !!HIWORD(result)` and `return LOWORD(result)`; `isOutside` is an
  `int&` out-parameter on an otherwise `unsigned int` return.
- `SetItemHeight` (`168`–`173`) packs with `MAKELONG(cyItemHeight, 0)` while
  `GetItemHeight` (`84`–`88`) reads a plain `int` — the setter packs, the getter
  does not.
- `SelItemRange(bool isSelected, int firstItem, int lastItem)` (`241`–`255`) is the
  only conditional message packing in the file: when `isSelected` is false it sends
  `wparam = lastItem, lparam = firstItem` (reversed) for the *same*
  `LB_SELITEMRANGEEX`. Windows API intent: `LB_SELITEMRANGEEX` with `wParam > lParam`
  **clears** the range, which is what `isSelected == false` wants.
- `SetSel(int index, bool isSelected)` (`270`–`275`) sends `wparam = isSelected`,
  `lparam = index` — the arguments are **reordered** against the QOR signature,
  matching Win32's `LB_SETSEL(wParam = TRUE/FALSE, lParam = item)`.
- `SetCaretIndex(int index, bool partialScroll)` (`263`–`268`) packs the bool with
  `MAKELONG(partialScroll, 0)`; `SetTabStops(int cxEachStop)` (`193`–`198`) sends
  `WPARAM(1)` plus the address of the local parameter. Both borrow the
  pointer-to-local idiom (valid only for the duration of the call).
- The three `SetTabStops` overloads (`181`–`198`) are duplicated verbatim from
  `EditController` (`editcontroller.cpp:248`–`261`).

### `ComboBoxController` — the CB_* table

`comboboxcontroller.h:34`–`43` declares `ComboBoxInfo` (the Win32 `COMBOBOX` INFO
record with `void* hwndCombo`/`hwndItem`/`hwndList`), and `:45`–`95` the class. The
header includes `../i_m/listboxcontroller.h` (`comboboxcontroller.h:30`) — **a
combobox depends on the listbox header**, because both need `CompareItemStruct`.

| Methods → message | Lines (cpp) |
|---|---|
| `DeleteString`→`CB_DELETESTRING`, `Dir`→`CB_DIR`, `FindString`, `FindStringExact` | `83`–`108` |
| `GetComboBoxInfo` — **a direct API call**, not a message: `cbi = {0}; cbi.cbSize = sizeof(ComboBoxInfo);` then `GetComboBoxInfo` | `110`–`115` |
| `GetCount`, `GetCurSel`, `GetDroppedControlRect` (→`CB_GETDROPPEDCONTROLRECT`), `GetDroppedState`, `GetDroppedWidth`, `GetEditCtrl`, `GetEditSel`, `GetExtendedUI`, `GetHorizontalExtent`, `GetItemData`, `GetItemHeight`, `GetLBCtrl`, `GetLBText`, `GetLBTextLen`, `GetLocale`, `GetTopIndex` | `117`–`209` |
| `InitStorage`→`CB_INITSTORAGE`, `InsertString`, `LimitText`, `Paste`, `ResetContent`, `SelectString`, `SetCurSel`, `SetDroppedWidth`, `SetEditSel`, `SetExtendedUI`, `SetHorizontalExtent`, `SetItemData`, `SetItemHeight`, `SetLocale`, `SetTopIndex`, `ShowDropDown` | `211`–`308` |
| Clipboard shared with Edit | `WM_CLEAR`/`WM_COPY`/`WM_CUT`/`WM_PASTE` | `68`–`81`, `231`–`234` |

Notable:

- `GetEditCtrl` (`145`–`151`) and `GetLBCtrl` (`180`–`186`) are **not messages**: each
  calls `GetComboBoxInfo` into a local `COMBOBOXINFO` and returns `cbi.hwndItem` /
  `cbi.hwndList` as a raw `void*`. They hand back an unmanaged HWND with no
  `PerThread::FromHandle` conversion — unlike every `WindowController` getter.
  `GetEditSel`/`SetEditSel` are the packed pair (`MAKELONG(startChar, endChar)`,
  `260`–`264`).
- `cbi.cbSize = sizeof(cbi)` (`148`, `183`) takes `sizeof` of the **variable**
  rather than the type. It happens to be right because `cbi` is the struct itself,
  but it is the one line in the file that would silently break if `cbi` became a
  pointer.
- `GetLBText` (`188`–`193`) has the same unsized-buffer problem as
  `ListBoxController::GetText`; `GetLBTextLen` (`195`–`199`) is the size probe.
- `CompareItem` returns `0` (`50`–`61`) and `AddString` returns `0` (`63`–`66`) —
  the only two stubs. The second is not laziness: Win32 has no `CB_ADDSTRING`
  message, so a combobox cannot add a string to its list window through this
  object.
- Style note: this file consistently builds `WPARAM wparam = …; LPARAM lparam = …;`
  locals before the call, where `editcontroller.cpp` mostly casts inline.

### `EditController` — EM_* plus the shared WM_* clipboard

`editcontroller.h:33`–`85` (56 declarations) and `editcontroller.cpp:50`–`286`.

| Methods → message | Lines (cpp) |
|---|---|
| `AppendText` — a **three-message composition**: `WM_GETTEXTLENGTH` → `EM_SETSEL(pos,pos)` → `EM_REPLACESEL` | `50`–`55` |
| `CanUndo`, `CharFromPos` (`MAKELPARAM(x,y)`), `GetFirstVisibleLine`, `GetBufferHandle`, `GetLimitText`, `GetLine`, `GetLineCount`, `GetMargins`, `GetModify`, `GetPasswordChar`, `GetRect` (two), `GetSel` (two), `GetTextLength` (`WM_GETTEXTLENGTH`) | `57`–`134` |
| `PosFromChar`, `SetBufferHandle`, `SetLimitText`, `SetMargins`, `SetModify`, `EmptyUndoBuffer`, `FmtLines`, `LimitText`, `LineFromChar`, `LineIndex`, `LineLength`, `LineScroll`, `ReplaceSel`, `SetPasswordChar`, `SetReadOnly`, `SetRect`, `SetRectNP`, `SetSel` (two), `SetTabStops` (three) | `136`–`261` |
| Clipboard: `WM_CLEAR`, `WM_COPY`, `WM_CUT`, `WM_PASTE`, then `EM_UNDO` | `264`–`286` |

Notable in this file:

- **`GetLine` is the only correct Win32 out-buffer idiom in the whole gui tree**
  (`editcontroller.cpp:82`–`86`): `EM_GETLINE` requires the first `WORD` of the
  buffer to carry the maximum length, and the file writes it
  — `*reinterpret_cast<LPWORD>(buffer.data()) = static_cast<WORD>(buffer.capacity())`
  — before passing `buffer.data()`. It still relies on the caller having reserved
  capacity (`capacity()` on a default `tstring` is 0), so it is *shaped* right and
  *sized* wrong — the one place to look when comparing against `ListBoxController::GetText`.
- `GetSel(int& startChar, int& endChar)` (`121`–`124`) passes the **addresses** of the
  two ints as `WPARAM`/`LPARAM`, matching Win32's `EM_GETSEL(WPARAM FAR* lpStart,
  LPARAM FAR* lpEnd)`.
- `PosFromChar` (`136`–`143`) unpacks the packed point with the helpers
  `GetXLParam` / `GetYLParam` (spelled `packedResutl` for the local).
- `SetMargins` (`155`–`158`) always sends `(EC_LEFTMARGIN | EC_RIGHTMARGIN)` with
  `MAKELONG(left, right)`; `GetMargins` (`93`–`96`) returns the packed value with no
  matching unpack helper — the pair is asymmetric like `SetItemHeight`/`GetItemHeight`.
- `SetSel(unsigned long selection, bool isScrolled)` (`230`–`237`) unpacks with
  `LOWWORD`/`HIWORD`; **both** `SetSel` overloads then send an extra `EM_SCROLLCARET`
  when `isScrolled` is true. That flag is a QOR convenience — Win32's `EM_SETSEL`
  does not scroll the caret.
- `LineScroll(int lines, int chars)` (`200`–`203`) sends `wparam = chars`,
  `lparam = lines` — **swapped against the QOR parameter order** and correct against
  Win32 (`EM_LINESCROLL` = horizontal chars, vertical lines). A readability trap, not
  a bug.
- `SetRect(Rect rc)` (`220`–`223`) and `SetRectNP` (`225`–`228`) send the address of a
  **by-value** parameter; `GetRect(Rect& rc)` (`108`–`112`) returns void and
  `GetRect()` (`114`–`119`) returns a `Rect rc{0}` — the pair is duplicated rather than
  one calling the other.
- `SetTabStops(int cxEachStop)` (`258`–`261`) sends `WPARAM(1)` with `LPARAM(&cxEachStop)`
  — the same "one stop value" encoding as `ListBoxController::SetTabStops`.
- Several `bool`-returning wrappers use the redundant
  `static_cast<int>(SendMessageT(...)) ? true : false` (`98`–`101`, `175`–`178`,
  `215`–`218`, `248`–`251`) while `CanUndo` (`57`–`60`) uses the plain
  `... ? true : false`. Same result, two spellings.

### `Monitor` — three API layers and the LPARAM-as-object trick

`monitor.cpp` (252 lines) is the only controller-directory file that reaches beyond
`user32.h`: it includes `api_layer/shcore/shcore.h` and `api_layer/gdi/gdi32.h`
(`monitor.cpp:30`–`31`) alongside `user32.h` (`29`).

`Monitor` owns a `Handle m_handle` (`monitor.h:87`) but — unlike `Menu` — **none of
its constructors call `DontClose()`**: `Monitor() : m_handle(0)` (`monitor.cpp:37`–`39`),
`Monitor(const PrimitiveHandle& h) : m_handle(h.Use())` (`41`–`43`),
`Monitor(const Monitor& m) : m_handle(m.m_handle)` (`45`–`47`, reaching the
*private* member from inside the class, which is legal but couples the copy ctor to
the storage).

Instance methods (`monitor.cpp:54`–`154`):

| Method | Lines | API |
|---|---|---|
| `GetInfo(MonitorInfo&)` | `54`–`57` | `GetMonitorInfoT` — note the cast is spelled `reinterpret_cast<::LPMONITORINFO>` with a leading `::` |
| `GetInfo(MonitorInfoEx&)` | `59`–`63` | same call, but sets `info.cbSize = sizeof(MonitorInfoEx)` first — the derived struct's `cbSize` inherits the **base** size from `MonitorInfo : MonitorInfo` (`monitor.h:55`–`58`), so only this overload corrects it |
| `GetWorkarea` | `65`–`76` | composes `GetInfo`, then derives x/y/w/h from `info.rcWork` |
| `GetDeviceName` | `78`–`87` | `GetInfo(MonitorInfoEx&)` → `info.szDevice` |
| `GetDeviceMode` | `89`–`95` | `memset(&dm,0,sizeof(DeviceMode))`, `dm.dmSize = sizeof(DeviceMode)`, then `EnumDisplaySettingsExT(deviceName, ENUM_CURRENT_SETTINGS, &dm, EDS_ROTATEDMODE)` |
| `GetPosition` | `97`–`107` | from `dm._display_fields.dmPosition` |
| `GetBitsPerPixel` | `109`–`134` | arithmetic on `dm.dmBitsPerPel` |
| `GetContentScale` | `137`–`154` | `ShCore::GetDpiForMonitor(hmon, MDT_EFFECTIVE_DPI, …)`; on non-`S_OK` falls back to `User32::GetDC(NULL)` + `GDI32::GetDeviceCaps(dc, LOGPIXELSX/SY)` + `User32::ReleaseDC(NULL, dc)` |

Two real defects sit in this file:

- `GetDeviceMode` passes **`EDS_ROTATEDMODE` as the `hMonitor` argument**
  (`monitor.cpp:94`). Win32's signature is
  `EnumDisplaySettingsEx(lpszDeviceName, dwModeNumber, lpModifiedDevMode, HMONITOR)`,
  and `EDS_ROTATEDMODE` is a `DWORD` DevCaps flag — the last argument is the wrong
  kind for the parameter. `ui/gl/monitorimpl.cpp:63` makes the same call with
  `EnumDisplaySettingsExW`, so the mistake is consistent across both trees rather
  than a typo in one.
- `GetBitsPerPixel` **derives** channel widths from bits-per-pixel
  (`bpp == 32 → 24`, `bpp/3` then distributing the remainder to green then red,
  `monitor.cpp:109`–`134`). It never reads real per-channel masks, so it is an
  approximation — fine for a 24-bit display, wrong for a packed 16-bit mode.
- `GetContentScale`'s fallback does not check `GetDC(NULL)` for `NULL` before
  passing it to `GetDeviceCaps` (`monitor.cpp:146`–`149`).

The static enumeration path is where the LPARAM trick lives:

```cpp
int qor_pp_compiler_stdcallconvention Monitor::CreateCallback(void* handle, void* dc, Rect* rect, long long data)   // monitor.cpp:157
{
    MONITORINFOEXW mi;
    ZeroMemory(&mi, sizeof(mi));
    mi.cbSize = sizeof(mi);
    if (GetMonitorInfoW((HMONITOR)handle, (MONITORINFO*)&mi))
    {
        std::vector<Monitor>* vecMonitors = reinterpret_cast<std::vector<Monitor>*>(data);
        PrimitiveHandle h(handle);
        Monitor m(h);
        vecMonitors->emplace_back(m);
    }
    return TRUE;                                                  // monitor.cpp:157–172
}
```

`CreateMonitors` (`monitor.cpp:174`–`188`) seeds it with a `Rect` built from a
`DEVMODEW` (`dm.dmPosition` + `dmPelsWidth`/`dmPelsHeight`) and passes
`(LPARAM)&vecMonitors` as `data` — a **C++ vector pointer smuggled through a Win32
`LPARAM`**. `Enumerate` (`226`–`232`) is the general form of the same call and null-guards
`dc`. `EumerateMonitors` (`190`–`205`) and `EumerateAdapters` (`207`–`224`) drive it
with an `EnumDisplayDevicesT` loop filtered on `DISPLAY_DEVICE_ACTIVE`, resetting the
`DISPLAY_DEVICE` record with `ZeroMemory` + `displayDevice.cb = sizeof(DisplayDevice)`
each iteration (`201`–`203`, `220`–`222`). `FromPoint`/`FromRect`/`FromWindow`
(`234`–`250`) construct a `Monitor` from the raw handle and return it by value.

### One cross-cutting note for this part

`editcontroller.cpp:29` and the other message-driven `.cpp` files
(`listboxcontroller.cpp:29`, `comboboxcontroller.cpp:29`) include
`view/messages.h` for the **message-parameter macros** — `LoWord`, `HiWord`,
`MakeLong`, `GetXLParam`, `GetYLParam` (`view/messages.h:29`–`38`). So the
controllers and the handler tree share the same parameter-cracking vocabulary; the
controllers *pack* with the Win32 `MAKELONG`/`MAKELPARAM`/`LOWORD`/`HIWORD` SDK
macros and the handlers *unpack* with the QOR ones. That asymmetry is why
`PosFromChar` (`editcontroller.cpp:136`–`143`) uses the QOR spellings while
`SetSel` (`230`–`237`) uses the SDK ones — see
[part 50](50-view-handlers.md) for the macro set.

## Level 4 — what the example actually exercises

The library has no unit tests; the only live caller is
`examples/experimental/osspecific/windows/mainwindow/mainwindow.cpp` (386 lines,
documented in [part 90](90-example-mainwindow.md)). Grepping that file for this
part's types gives a hard boundary:

| Site | Lines | Uses |
|---|---|---|
| `window.GetController()` then a null check, then `GetADC(updateRegion, DCXWindow\|DCXIntersectRegion)` / `GetDC()` | `mainwindow.cpp:73`–`84` | the DC accessors + the `IsNULL`-guarded region path |
| `controller->GetRect(r)` — "get screen coordinate rectangle of window" | `99`–`103` | `GetRect` |
| `ps.GetDC()` then `controller->GetClientRect(rc)` | `169`–`177` | paint-session DC + `GetClientRect` |
| `factory.Create(L"MainWindow", L"Main Window Experiments")` then `controller->Show()` / `controller->Update()` | `347`–`349` | the `WindowFactory` return type **is** a `WindowController` (part 20) |
| two more `factory.Create(...)` → `Show()`/`Update()` | `367`–`371`, `381`–`383` | same, with explicit `style`/`exStyle` and a geometry argument |

Consequences worth stating:

- **Only the base `WindowController` is exercised.** No control controller
  (`Button`/`ComboBox`/`Edit`/`ListBox`/`ScrollBar`/`Static`/`Animation`/`MDIClient`)
  is instantiated anywhere in the example, and neither are `Menu`, `PopupMenu`,
  `Monitor`, `Mouse` or `Caret`. Everything in Levels 2–3 above is read from source,
  not observed in a run.
- `Show()` is called with no argument, so `swNormal` (`windowcontroller.h:218`) is the
  exercised default; `Update()` (`windowcontroller.cpp:597`–`600` → `UpdateWindow`) is
  the only repaint trigger the example uses.
- `GetADC` with `DCXWindow|DCXIntersectRegion` (`mainwindow.cpp:80`) is the one place a
  `Region` ref is passed non-null, so the unguarded `GetADC` region dereference is
  exercised **with** a valid region; the null case is not.
- `style.SetSystemMenu(true)` (`mainwindow.cpp:353`) / `false` (`375`) is the style-side
  counterpart of `WindowController::GetSystemMenu` — the menu surface is only reached
  through `WindowStyle`, never through a controller.
- The back-pointer pair is used in both directions: `GetController()`
  (`window.h:73`–`74`, `WindowController* GetController()` /
  `void SetController(WindowController*)`) is how a message handler gets the
  controller the ctor published.

### What remains in this part

Nothing in `controllers/` is unread: all 14 compiled `.cpp` files and their headers
have been through a read pass. Two follow-ups are worth recording but belong to other
parts:

- `CompareItemStruct` (used by `listboxcontroller.h:41` and
  `comboboxcontroller.h:53`) is defined outside `controllers/` — **resolved in
  part 40**: `src/platform/os/windows/common/structures.h:437`, visible through
  `window.h:32`.
- `DeviceContext`'s `MustRelease()`/release pairing (`devicecontext.h`) is the
  counterpart of the DC discipline recorded in Level 1 — covered in part 60.

**Next:** [40 — window types](40-window-types.md) ·
[back to index](README.md)

