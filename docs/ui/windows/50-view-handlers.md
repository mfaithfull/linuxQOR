# 50 — View handlers: the message loop and the handler tree

**Status: detailed for the `view/handlers` root and all three subtrees — `parts/`,
`interactive/`, and `rendered/` (74 of 74 files).** This pass read all 20 root
files, all 18 `parts/` files, all 22 `interactive/` files, and all 14 `rendered/`
files in full, plus consumer edges (`gui/` root, `factories/`, `controllers/`,
`examples/`) and the `add_library` list in `gui/CMakeLists.txt`.

## Files in this part (74)

| Subtree | Files | Members |
|---|---|---|
| `view/handlers` root | 20 | `abstractwindowhandler` `acceleratortable` `base` `dialoghandler` `hook` `interactive` `mdiclient` `message` `messagehandler` `rendered` `toplevel` (`.h` each; `.cpp` for all but `abstractwindowhandler` and `message`) |
| `view/handlers/interactive` | 22 | `clipboard` `draganddrop` `focus` `help` `ime` `keyboardinput` `ncpointer` `parent` `pointer` `scroll` `winctrlparent` |
| `view/handlers/parts` | 18 | `basetext` `devicechange` `lifetime` `menu` `owner` `part` `style` `timer` `user` |
| `view/handlers/rendered` | 14 | `cursor` `dwm` `font` `iconized` `nc` `ncrender` `rendering` |

Root line counts, so a session can be sized against them:

| Pair | Lines (h / cpp) | Shape |
|---|---|---|
| `abstractwindowhandler` | 50 / — | the contract: one pure virtual |
| `acceleratortable` | 72 / 86 | RAII wrapper over the Win32 accelerator table |
| `base` | 73 / 75 | chain base + 8 part-handler refs + the hook chain |
| `dialoghandler` | 58 / 47 | **no base class** — a two-line stub |
| `hook` | 37 / 35 | `class Hook {};` placeholder, empty `.cpp` |
| `interactive` | 66 / 55 | chain level + 10 interactive refs |
| `message` | 48 / — | `struct Message` — the Win32 `MSG` record |
| `messagehandler` | 120 / 254 | the loop: 26 declared API members + idle tracking |
| `rendered` | 61 / 51 | chain level + 7 rendered refs |
| `toplevel` | 106 / 393 | the only message decoder in the chain |
| `mdiclient` | 49 / 52 | orphan — 3 cases, all `return false` |

## What this part is for

From `src/platform/os/windows/ui/gui/purpose.md:15`:

> Message handling is a customisable tree of objects derived from
> `BaseWindowPartHandler`. Each handler either handles the message, passes it
> to its children, or gives up so the parent can try the next sibling.

That tri-state is one signature, repeated at every level of the chain:

```cpp
virtual bool ProcessMessage(Window&, long long& lResult, unsigned int msg,
                            unsigned long long wParam, long long lParam)
```

`abstractwindowhandler.h:44` (pure virtual), `base.h:52`, `rendered.h:46`,
`interactive.h:49`, `toplevel.h:60`. `true` means "handled" and the caller then
reads `lResult`; `false` means "give up".

## The entry point — how Windows reaches the tree

`gui/CMakeLists.txt` compiles `factories/windowfactory.cpp`; the message
function is installed there:

- `windowfactory.cpp:93` `windowClass->SetMessageFunction(WndProc);` — inside
  `WindowFactory::AddWindowClass` (`:78`–`97`).
- `windowclassregistration.cpp:40` `wndClassEx.lpfnWndProc = (WNDPROC)windowClass.WindowProc();`
  — the C `WNDCLASS` field Windows calls.
- `windowfactory.cpp:49` `long long qor_pp_compiler_stdcallconvention
  WindowFactory::WndProc(void* hwnd, unsigned int msg, unsigned long long wParam, long long lParam)`,
  declared `static` at `windowfactory.h:72`.

Its body, in order:

```cpp
Window* window = (Window*)(User32::GetPropT((HWND)hwnd, TEXT("QOR")));   // :51
if(window == nullptr)
{   // first message for a window being created
    auto perThread = new_ref<PerThread>();                               // :55
    ref_of<Window>::type windowRef = perThread->GetWindowCreationInProgress();
    window = windowRef.operator->();
    if(window) {
        User32::SetPropT((HWND)(hwnd), TEXT("QOR"), (HANDLE)(window));   // :60
        window->SetHandle(PrimitiveHandle(hwnd));                        // :61
    }
}
if(window != nullptr)
{
    auto handler = window->GetHandler();                                 // :67
    long long lResult = -1;                                              // :68
    if(handler.IsNotNull() && handler->ProcessMessage(*window, lResult, msg, wParam, lParam))
        return lResult == -1 ? DefWindowProc(...) : lResult;             // :71
}
return DefWindowProc((HWND)hwnd, msg, wParam, lParam);                   // :75
```

Three facts follow from it:

- **`lResult` starts at `-1`** (`windowfactory.cpp:68`). A handler that returns
  `true` but never writes `lResult` is routed back to the raw `DefWindowProc`
  (`:71`). That is why chain handlers write `lResult = 0` in their handled
  branches (`toplevel.cpp:76`, `:94`, `:102`, …).
- The window is found by the window property `"QOR"` (`:51`, `:60`) — the same
  property `Dialog` uses to identify itself (part 40, `dialog.h`
  `GetWindowProperty` route, mirrored at `windowfactory.cpp:51`–`60`).
- Two `DefWindowProc`s are in play: the raw API call here (`:71`, `:75`, via
  `using namespace qor::win::api;` at `windowfactory.cpp:35`) and
  `Window::DefWindowProcT` (`window.cpp:104`–`107`) that handlers call. The
  latter returns the API result, which is how a handler can hand `lResult`
  straight to Windows (`toplevel.cpp:154`).

## Level 1 — the chain of five base handlers

```
AbstractWindowHandler            abstractwindowhandler.h:37
└── BaseWindowHandler            base.h:44
    └── RenderedWindowHandler    rendered.h:39
        └── InteractiveWindowHandler  interactive.h:42
            └── TopLevelWindowHandler   toplevel.h:45
```

Declared bases: `base.h:44` `BaseWindowHandler : public AbstractWindowHandler`,
`rendered.h:39` `: public BaseWindowHandler`, `interactive.h:42` `: public
RenderedWindowHandler`, `toplevel.h:45` `: public InteractiveWindowHandler`.
Each header pulls the one above by local include — `base.h:31`
`abstractwindowhandler.h`, `rendered.h:28` `base.h`, `interactive.h:28`
`rendered.h`, `toplevel.h:28` `interactive.h` — so `toplevel.h` sees the whole
tree from one include.

### The `||` chains — order is the contract

`base.cpp:41`–`53`, `rendered.cpp:32`–`44` and `interactive.cpp:32`–`47` are the
same shape: a short-circuit `||` chain of `ProcessMessage` calls.

`BaseWindowHandler::ProcessMessage` (`base.cpp:43`–`52`):

```cpp
return ProcessHook(...)          // base.cpp:43
    || (m_lifetime      IsNotNull() && m_lifetime->ProcessMessage(...))      // :44
    || (m_timer         IsNotNull() && ...)                                  // :45
    || (m_owner         IsNotNull() && ...)                                  // :46
    || (m_style         IsNotNull() && ...)                                  // :47
    || (m_baseText      IsNotNull() && ...)                                  // :48
    || (m_user          IsNotNull() && ...)                                  // :49
    || (m_deviceChange  IsNotNull() && ...)                                  // :50
    || (m_menu          IsNotNull() && ...)                                  // :51
    || HandleMessage(...);                                                   // :52
```

Each part ref is guarded `m_x.IsNotNull() &&`, so an unset part is skipped
rather than dereferenced. `HandleMessage` (`base.h:53`) is the last resort.

| Chain level | Body | Part refs it owns (header line) |
|---|---|---|
| `BaseWindowHandler` | `base.cpp:43`–`52` | `m_lifetime` `m_style` `m_user` `m_baseText` `m_owner` `m_timer` `m_deviceChange` `m_menu` — `base.h:56`–`63` |
| `RenderedWindowHandler` | `rendered.cpp:34`–`43` | `m_nonClient` `m_ncRendering` `m_rendering` `m_iconized` `m_font` `m_cursor` `m_dwm` — `rendered.h:49`–`55` |
| `InteractiveWindowHandler` | `interactive.cpp:34`–`46` | `m_focus` `m_ncPointer` `m_pointer` `m_keyboardInput` `m_dragAndDrop` `m_IME` `m_help` `m_scroll` `m_clipboardOwner` `m_parent` — `interactive.h:52`–`61` |

Processing order against declaration order:

- `RenderedWindowHandler` (`rendered.cpp:34`–`43`): hook →
  `BaseWindowHandler::ProcessMessage` → nonClient → ncRendering → rendering →
  iconized → font → cursor → dwm → `HandleMessage`. **Same order as
  `rendered.h:49`–`55`.**
- `InteractiveWindowHandler` (`interactive.cpp:34`–`46`): hook →
  `RenderedWindowHandler::ProcessMessage` → ncPointer → pointer → **parent** →
  focus → keyboardInput → IME → scroll → help → **dragAndDrop** →
  clipboardOwner → `HandleMessage`. **Not the `interactive.h:52`–`61` order** —
  of the ten part refs, `m_parent` is declared last but is offered third, and
  `m_dragAndDrop` is declared fifth but offered ninth.
- `BaseWindowHandler`: processing `lifetime, timer, owner, style, baseText,
  user, deviceChange, menu` vs declaration `lifetime, style, user, baseText,
  owner, timer, deviceChange, menu` (`base.h:56`–`63`).

A message is offered down the whole chain in a fixed order and the first
`true` wins; nothing below the top level consults a message-id table.

### Every chain level re-enters the hook chain

`ProcessHook` is declared once, **non-virtual**, at `base.h:51`, and defined at
`base.cpp:36`–`39`:

```cpp
return m_nextHookController ? m_nextHookController->ProcessMessage(Window, lResult, msg, wParam, lParam) : false;
```

`base.cpp:43`, `rendered.cpp:34`, `interactive.cpp:34` and `toplevel.cpp:64` all
call it before their own work, so one unhandled top-level message walks the hook
chain **four times** — once per level. It is free while the chain is empty
(`m_nextHookController` is initialised `nullptr` at `base.cpp:32`), and
quadratic in chain length once populated.

`HookController` (`base.h:54`, defined `base.cpp:61`–`73`) appends to that
singly-linked list: it stores at the first empty slot (`:66`) else recurses
(`:70`), and `pResult` (`:63`) is only ever `nullptr`. **No code in `src/`,
`examples/` or `test/` calls `HookController`**, so in the built tree
`m_nextHookController` stays `nullptr` and `ProcessHook` always returns `false`.
The hook mechanism exists and is unexercised.

### `TopLevelWindowHandler` — the only decoder in the chain

`toplevel.cpp:62`–`167`. It does **not** chain first: it calls the hook
(`:64`–`67`), then switches on the message id, and only falls through when the
id is unmatched — `:166`
`return InteractiveWindowHandler::ProcessMessage(window, lResult, msg, wParam, lParam);`.

| Case | Line | What the code does |
|---|---|---|
| `wmEnterIdle` | `:71` | `Window w(PrimitiveHandle((void*)(lParam)))` (`:73`), then `OnEnterIdle(static_cast<unsigned short>(wParam), w)` (`:74`); `lResult = 0` only if the hook answers (`:76`) |
| `wmSysCommand` | `:80` | cracks `lParam`: `wXPos = LoWord(lParam)`, `wData = HiWord(lParam)` (`:83`–`84`); `bAcceleratorUsed` = `wData == (unsigned short)(-1)` (`:85`), `bMnemonic` = `wData == 0` (`:86`), `wYPos = wData` only when neither (`:87`–`90`); `code = wParam & 0XFFF0` (`:91`) → `OnSysCommand(window, code, wYPos, wXPos, bAcceleratorUsed, bMnemonic, wParam, lParam)` (`:92`) |
| `wmDropFiles` | `:98` | `void* hDropInfo = reinterpret_cast<void*>(wParam)` (`:100`), `OnDropFiles(hDropInfo)` (`:101`), then `lResult = 0` unconditionally (`:102`) |
| `wmQueryEndSession` | `:105` | `bLoggingOff = (lParam & EndSessionLogOff) > 0` (`:107`), `lResult = OnQueryEndSession(bLoggingOff) ? true : false` (`:108`) — the only case that reports the hook's answer as the result |
| `wmEndSession` | `:111` | `OnEndSession(wParam ? true : false, (lParam & EndSessionLogOff) > 0)`, `lResult = 0` |
| `wmQueryNewPalette` | `:119` | `lResult = OnQueryNewPalette()`, then `window.DefWindowProcT(msg, wParam, lParam)` (`:122`) |
| `wmPaletteChanged` | `:125` | `window.DefWindowProcT` **first** (`:127`), then `OnPaletteChanged(Window((void*)wParam))` |
| `wmCommand` | `:132` | `wNotify = HiWord(wParam)`, `wID = LoWord(wParam)` (`:134`–`135`) → `OnCommand` |
| `wmActivateApp` | `:142` | `OnActivateApp(wParam ? true : false, (unsigned long)lParam)` |
| `wmDeviceChange` | `:152` | **no hook at all** — `lResult = window.DefWindowProcT(msg, wParam, lParam)` |
| `wmGetTitleBarInfoEx` | `:157` | `OnGetTitleBarInfo(reinterpret_cast<TitleBarInfoEx*>(lParam))` |

Every one of the 11 ends in `return true`, so **those ids are claimed
unconditionally** — a hook that returns `false` still stops the message, and
the interactive/rendered/parts chain never sees it.

The ids are `static const unsigned short` in `view/messages.h`:
`wmQueryEndSession 0x0011` (`:59`), `wmEndSession 0x0016` (`:61`),
`wmActivateApp 0x001C` (`:69`), `wmCommand 0x0111` (`:156`), `wmSysCommand
0x0112` (`:157`), `wmEnterIdle 0x0121` (`:167`), `wmDeviceChange 0x0219`
(`:218`), `wmDropFiles 0x0233` (`:232`), `wmQueryNewPalette 0x030F` (`:266`),
`wmPaletteChanged 0x0311` (`:268`), `wmGetTitleBarInfoEx 0x033F` (`:279`),
`EndSessionLogOff 0x80000000` (`:295`). The full table is part 80's.

## Level 2 — the loop, the record, the accelerator

### `MessageHandler` — the pump (`messagehandler.h:53`, `messagehandler.cpp`)

It is **not** part of the handler tree: it sits in `qor::platform::win`
(`messagehandler.h:35`), not `qor::platform::win::gui::view`. It owns the
outer loop.

`MessageLoop` (`messagehandler.h:109` virtual, `messagehandler.cpp:81`–`122`)
is the classic Win32 shell loop:

```cpp
do {
    while (bIdle && !Peek(&message, 0, 0, 0, MessageHandler::PMNoRemove))   // :91
        if (!OnIdle(lIdleCount++)) bIdle = false;                            // :94
    do {
        if (!(acceleratorTable.IsNull() ? RouteMessage() : RouteMessage(acceleratorTable)))  // :104
            return 0;                          // quit message
        if (IsIdleMessage(&message)) { bIdle = true; lIdleCount = 0; }      // :109
    } while (Peek(&message, 0, 0, 0, MessageHandler::PMNoRemove));          // :115
    bIdle = true; lIdleCount = 0;
} while (true);                                                             // :120
```

- `RouteMessage()` (`messagehandler.cpp:50`–`62`): `Get(&message, 0, 0, 0)`;
  `false` means "quit message received" (`:55`), then `Translate(&message)`
  (`:58`) and `Dispatch(&message)` (`:59`).
- `RouteMessage(ref_of<AcceleratorTable>)` (`:64`–`79`): same `Get`, then
  `Window window(PrimitiveHandle(message.hwnd))` (`:72`) and
  `acceleratorTable->Translate(window, message)` (`:73`); only on failure does
  it fall back to `Translate` + `Dispatch` (`:75`–`76`). **No null check**
  before the `->` — the guard lives at the call site, `:104`.
- `IsIdleMessage` (`messagehandler.h:97`–`100`) and `OnIdle` (`:102`–`105`) are
  virtual with inline default bodies returning `0`, and nothing overrides them
  anywhere, so the idle branch never re-arms from `IsIdleMessage`.
- `Quit(int)` (`messagehandler.h:111`–`114`) is the only inline override hook
  that does work — it calls `PostQuit`.

The other 26 declared members (`messagehandler.h:70`–`95`) are one-line
wrappers onto `User32::` — `Get` →
`GetMessageT` (`cpp:144`–`147`), `Peek` → `PeekMessageT` (`:179`–`182`),
`Post` → `PostMessageT` (`:184`–`187`), `Send` → `SendMessageT` (`:219`–`222`),
`Translate` → `TranslateMessage` (`:234`–`237`), `Dispatch` → `DispatchMessageT`
(`:134`–`137`), `Wait` → `WaitMessage` (`:249`–`252`). Only `Get`, `Peek`,
`Translate`, `Dispatch`, `PostQuit` are reached from `RouteMessage`/`MessageLoop`;
the broadcast pair (`:124`–`132`), `PostToThread` (`:194`–`197`),
`RegisterWindowMessageT` (`:199`–`202`), the `Send*` trio (`:209`–`227`) and
friends have **no caller in `src/`, `test/` or `examples/`**.

Supporting types beside it (`messagehandler.h:37`–`51`): `LUID`, `BSMINFO`
(the `BroadcastSystemMessageEx` record) and
`typedef void (* SendAsyncProc)(void*, unsigned int, unsigned long long, long long)`.
The three `PM*` constants (`:57`–`59` — `PMNoRemove 0x0000`, `PMRemove 0x0001`,
`PMNoYield 0x0002`) are `static const unsigned short`; the `PMQS*` group
(`:60`–`63`) is commented out. `PMNoRemove` is the value the loop passes to
`Peek` (`:91`, `:115`).

### `Message` — the Win32 `MSG` record (`message.h:33`–`44`)

In `qor::platform::win` (`message.h:31`), under the header's own note
`message.h:30` "All types on this interface must be portable":

```cpp
struct Message { void* hwnd; unsigned int message; unsigned long long wParam;
                 long long lParam; unsigned long time; Point pt;
#ifdef _MAC
                 unsigned long lPrivate;
#endif
};
```

`Point` comes from `../drawing/point.h` (`message.h:28`). Note `lPrivate` is
Mac-only — unlike the SDK `MSG`, which carries `lPrivate` on every platform.
This struct is what the API calls are `reinterpret_cast` onto
(`acceleratortable.cpp:83`, `messagehandler.cpp:136`/`146`/`181`) — the layout
match with `MSG` is asserted by field order alone, not by the compiler.

### `AcceleratorTable` — RAII over `HACCEL` (`acceleratortable.h:50`, `cpp:38`–`84`)

Also `qor::platform::win`. Four constructors, one destructor:

| ctor | Effect as coded | `m_temporary` |
|---|---|---|
| `()` (`cpp:43`–`47`) | `m_handle = 0` | `true` |
| `(const PrimitiveHandle& h)` (`cpp:49`–`53`) | `: m_handle(h.Use())`, then `m_handle.DontClose()` | `true` |
| `(std::vector<Accel>&)` (`cpp:55`–`59`) | `User32::CreateAcceleratorTableT(reinterpret_cast<LPACCEL>(accels.data()), (int)accels.size())` | `false` |
| `(void* hInstance, const std::string& tableName)` (`cpp:61`–`65`) | `User32::LoadAcceleratorsT((HINSTANCE)hInstance, to_tstring(tableName.c_str()).c_str())` | `false` |

`~AcceleratorTable()` (`cpp:67`–`74`) guards **both** clean-up calls with
`if(!m_temporary)` (`:69`): `User32::DestroyAcceleratorTable` (`:71`) and
`m_handle.Drop()` (`:72`). So for a temporary table the handle is neither
destroyed nor dropped — it leaks, and the `Handle` never releases it.

`Accel` (`acceleratortable.h:35`–`46`) mirrors `ACCEL`: `fVirt`/`key`/`cmd`,
widened to `unsigned short`/`unsigned long` under `_MAC`. `Copy` (`cpp:76`–`79`)
and `Translate` (`cpp:81`–`84`) map to `CopyAcceleratorTableT` and
`TranslateAcceleratorT`.

The only construction anywhere is in the **unbuilt** `retropad` example —
`retropad.cpp:962` and `viewmodel.cpp:493`, both
`AcceleratorTable accTable(LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RETROPAD)));`.
`MessageLoop()` is always called with no argument (`perthread.cpp:40`,
`mainwindow.cpp:385`, and the four other examples), so the default null
`ref_of<AcceleratorTable>` (`messagehandler.h:109`) is what runs.
`WindowController::TranslateAccelerator` (`windowcontroller.h:258`,
`windowcontroller.cpp:656`–`658`) is the second route and has no caller.

## Level 3 — the top level's own hooks

`OnSysCommand` (`toplevel.cpp:174`–`258`) is the second decoder: a switch on
`code` over the `sc*` constants (`messages.h:302`–`320`, `constexpr unsigned
short`, all multiples of `0x10`). 18 cases — `scClose` (`:178`),
`scContextHelp`, `scDefault`, `scHotKey`, `scHScroll`, `scVScroll`, `scKeyMenu`,
`scMaximize`, `scMinimize`, `scMonitorPower`, `scMouseMenu`, `scMove`,
`scNextWindow`, `scPrevWindow`, `scRestore`, `scScreenSave`, `scSize`,
`scTaskList` (`:178`–`255`). **`scArrange 0xF110` (`messages.h:313`) is the one
`sc*` constant with no case** — it falls to `:257 return false`.

The 18 `OnSysCommand*` bodies (`toplevel.cpp:297`–`386`) are stubs: all return
`false` except `OnSysCommandClose` (`:297`–`301`), which calls
`window.DefWindowProcT(wmSysCommand, wParam, lParam)` and returns `true`. The
other hook defaults are equally inert: `OnEnterIdle` (`:169`–`172`),
`OnDropFiles` (`:260`–`262` empty), `OnEndSession` (`:269`–`271` empty),
`OnQueryNewPalette` (`:273`–`276` `false`), `OnPaletteChanged` (`:278`–`280`
empty), `OnActivateApp` (`:292`–`295` `false`), `OnGetTitleBarInfo`
(`:388`–`391` `false`). Of the 28 virtual hooks declared at `toplevel.h:61`–`89`
(10 top-level + 18 `OnSysCommand*`), only `OnQueryEndSession`
(`:264`–`267`, returns `true`), `OnSysCommandClose` and `OnCommand` do work.

`OnCommand` (`:282`–`290`) is the real dispatch table:

```cpp
auto it = m_commandMap.find(MakeKey(id, notification));
if(it != m_commandMap.end()) return it->second.cb(window, it->second.ctrl);
return false;
```

`MakeKey` (`toplevel.h:95`–`99`, inline in a **private** section) packs
`(id << 16) | notification`; `SetCommandCallback` (`toplevel.h:91`, defined
`toplevel.cpp:46`–`60`) reads `ctrl->GetId()` (`:48`) as `id`, builds
`Command{ctrl, cb}` (`:49`) and inserts-or-replaces (`:54`/`:58`). `Command`
(`toplevel.h:51`–`55`) and the `cmd_callback` typedef (`toplevel.h:49`,
`bool(*)(Window&, WindowController*)`) live inside the class body;
`m_commandMap` is `std::map<unsigned long, Command>` (`toplevel.h:101`).
The only caller repo-wide is `editbox.cpp:88` — an example that is commented
out of the build.

`TopLevelLifetimeHandler` (`toplevel.h:40`–`43` `: public LifetimeHandler`) is
the one class here that does work: `OnDestroy` (`toplevel.cpp:35`–`39`) calls
`qor::win::api::User32::PostQuitMessage(EXIT_SUCCESS)` — the file's own
comment says "The app exits when this window is destroyed" — and returns `0`.
Note `OnDestroy` is declared **without `virtual`** at `toplevel.h:42`; 50a
resolves it — `LifetimeHandler::OnDestroy` is virtual at `lifetime.h:61`, so
this is an ordinary override (private access is irrelevant to dispatch), and the
quit-on-destroy handler does run.
`TopLevelWindowHandler::TopLevelWindowHandler()` (`toplevel.cpp:41`–`44`)
installs it: `m_lifetime = new_ref<TopLevelLifetimeHandler>();`, overwriting
the inherited `ref_of<LifetimeHandler>` slot (`base.h:56`). Every top-level
window therefore carries the quit-on-destroy lifetime handler.

`TitleBarInfoEx` (`toplevel.h:32`–`38`) is the record behind
`OnGetTitleBarInfo`: `cbSize{sizeof(TitleBarInfoEx)}` (`:34`), `Rect rcTitleBar`
(`:35`), and `rgstate[TitleBarChildren + 1]` / `rgrect[TitleBarChildren + 1]`
(`:36`–`37`) — `TitleBarChildren` is `static const int ... = 5` at
`common/constants.h:71`, mirrored in the controller layer as
`constexpr int countTitleBarChildren = 5` (`windowcontroller.h:60`, used by
`unsigned long rgstate[countTitleBarChildren + 1]` at `:67`). So the title-bar
record carries six state/rect slots, and the handler layer's copy of the
constant is the library's, not the controller's.

### The orphans and the placeholders

- **`DialogHandler`** (`dialoghandler.h:44`) derives from **nothing** —
  `class qor_pp_module_interface(QOR_WINGUI) DialogHandler {`, no base class,
  no part refs, despite including `abstractwindowhandler.h` and the eight
  `parts/` headers at `dialoghandler.h:31`–`40`. `ProcessMessage`
  (`dialoghandler.cpp:36`–`39`) is `return HandleMessage(...);` and
  `HandleMessage` (`:42`–`45`) returns `false`. It compiles
  (`gui/CMakeLists.txt:91`) and **nothing anywhere constructs it** — repo-wide
  grep hits only its own two files. The `Dialog` window class of part 40 has
  its own `DlgProc`, not this.
- **`MDIClientHandler`** (`mdiclient.h:36` `: public AbstractWindowHandler`)
  switches on `wmMDIActivate`, `wmMDIDestroy`, `wmMDIGetActive`
  (`mdiclient.cpp:36`–`47`) and returns `false` in all three, plus `false` at
  the fallthrough (`:49`) — every path identical. Its `.cpp` is **not** in
  `add_library`; the only `mdiclient.cpp` compiled is
  `windows/i_m/mdiclient.cpp` (`gui/CMakeLists.txt:21`), part 40's `MDIClient`
  — a different class in a different namespace.
- **`Hook`** (`hook.h:31`–`33`, `class Hook` with an empty body) is an empty
  placeholder in `qor::platform::win` (`hook.h:29`), and `hook.cpp`'s namespace
  body (`:34`–`35`) is empty while the file includes `stringconv.h` (`:28`) and
  `user32.h` (`:29`) and pulls `using namespace qor::win::api;` (`:31`). Both
  compile (`gui/CMakeLists.txt:59`). The object that ties a handler to a
  factory's `WndProc` does not exist — that tie is the `WndProc` free function
  of part 20 (`windowfactory.cpp:93`).
- **`Window::m_messageProc`** (`window.h:69`–`71`, a lambda-typed member
  returning `0`) has one occurrence repo-wide: its own declaration. Dead.
- Include-guard close comments are copy-paste wrong in three headers:
  `interactive.h:66` closes with `..._HANDLER_TOPLEVEL`, `mdiclient.h:49`
  with `..._HANDLER_BASE`, `abstractwindowhandler.h:33` closes the `Window`
  forward-declaration block with `}}}qor::platform::nswindow`; the guards
  themselves (`interactive.h:25`–`26`, `mdiclient.h:25`–`26`) are distinct,
  so behaviour is unaffected.

## Consumers — who includes this part

Inside `gui/`, all includes are local relative paths:

| Consumer | Include | Why |
|---|---|---|
| `factories/windowfactory.cpp:42` | `../view/handlers/toplevel.h` | default factory `HandlerFactory<TopLevelWindowHandler>` at `:46` |
| `perthread.h:45` | `view/handlers/messagehandler.h` | `MessageHandler m_messageHandler;` member at `:102`; `PerThread::Run()` (`perthread.cpp:38`–`41`) returns `m_messageHandler.MessageLoop()` (`:40`) |
| `window.h:35`/`36`/`37` | `view/handlers/message.h` `acceleratortable.h` `abstractwindowhandler.h` | `SetHandler`/`GetHandler` typed on `ref_of<AbstractWindowHandler>` (`window.h:61`–`62`) |
| `factories/handlerfactory.h:35` | `../view/handlers/abstractwindowhandler.h` | `HandlerFactory<T>::Create()` (`:58`–`61`) returns `new_ref<T>()` |
| `controllers/windowcontroller.h:258` | `AcceleratorTable` in `TranslateAccelerator` | no caller |
| 12 `windows/*` `.cpp` files — all include it at their `:29`: `a_h/` `animation` `button` `combobox` `comboboxex` `datetimepicker` `edit` `flatscrollbar` `header`, `i_m/` `listbox` `mdiclient`, `s_s/` `scrollbar` `static | `../../view/handlers/base.h` | the widget layer's base handler |
| `gui/window.cpp:29` `factories/windowclass.h:34` | same header | 14 includes of `handlers/base.h` in total |

So the tree's consumers are: the widget layer (12 files), the factory,
`Window`, and `PerThread`. Note `a_h/header.cpp:29` is one of the 12 yet that
TU is absent from `add_library` (part 40's finding), so 11 of the 12 compile.

Example side: `mainwindow.cpp:9`/`10` include `messagehandler.h`/`toplevel.h`
and `:385` `return messageHandler.MessageLoop();`. `CustomHandlerFactory`
(`mainwindow.cpp:319`–`332`) is the only place a handler tree is assembled by
hand — it derives from `HandlerFactory<TopLevelWindowHandler>`, overrides
`Create()`, and hangs `CustomNonClientRenderingHandler` on `m_ncRendering`
(`:325`), `CustomNonClientHandler` on `m_nonClient` (`:327`),
`CustomRenderingHandler` on `m_rendering` (`:329`). Those three custom classes
override the `rendered/` subtree — walked in full in **50c** below, and in the
example itself in part 90.

`MessageLoop()` call sites repo-wide — `perthread.cpp:40` plus
`mainwindow.cpp:385`, `button.cpp:56`, `editbox.cpp:98`, `menu/main.cpp:82`,
`minimal/main.cpp:47`. Of the examples, only `mainwindow` builds
(`examples/experimental/osspecific/windows/CMakeLists.txt:4 add_subdirectory(mainwindow)`;
`button :1`, `editbox :3`, `menu :5`, `minimal :6`, `retropad :8` are commented
out), so `perthread.cpp:40` and `mainwindow.cpp:385` are the live pair.

## Build facts

`gui/CMakeLists.txt` compiles **8 of the 9** root `.cpp` files —
`view/handlers/acceleratortable.cpp:57` `base.cpp:58` `hook.cpp:59`
`messagehandler.cpp:60` `toplevel.cpp:61` `interactive.cpp:62`
`rendered.cpp:63` `dialoghandler.cpp:91`, with `parts/` at `:64`–`:71` and
`parts/lifetime.cpp` at `:90`, `interactive/` at `:72`–`:82`, `rendered/` at
`:83`–`:89`. `view/handlers/mdiclient.cpp` is **absent from the list** — the
only `mdiclient` compiled is `windows/i_m/mdiclient.cpp` (`gui/CMakeLists.txt:21`),
part 40's `MDIClient`. `abstractwindowhandler.h` and `message.h` have no
`.cpp`, so of the 20 root files, 11 are headers (2 header-only) and 9 are `.cpp` (8 compiled).

There are **no unit tests** for this part: grep for `HookController` under
`test/` returns nothing, and the exercise path is the `mainwindow` example.

## Gotchas worth carrying forward

1. **The handler contract is not "handled"** — `true` at the top level means
   "claimed", not "did work": 11 ids return `true` even when their hook body
   is empty (`toplevel.cpp:260`–`262`, `:269`–`:271`, `:278`–`280`). A
   sub-handler cannot get those ids.
2. **The hook chain is walked four times per message** (`base.cpp:43`,
   `rendered.cpp:34`, `interactive.cpp:34`, `toplevel.cpp:64`), and it is
   currently empty because nothing calls `HookController`.
3. **`m_parent` runs third, not last** in the interactive chain
   (`interactive.cpp:38` vs `interactive.h:61`) — and `m_dragAndDrop` ninth
   (`:44` vs `:56`). Chain order ≠ declaration order.
4. **`DialogHandler` and `MDIClientHandler` are unreachable**, and both are
   broken in the ways above — they are not alternative routes for the `Dialog`
   / `MDIClient` classes of part 40.
5. **`AcceleratorTable`'s destructor skips `DestroyAcceleratorTable` for
   temporaries** (`acceleratortable.cpp:69`–`72`), and the accelerator route
   through `MessageLoop` (`:104`) is never taken in the built tree.
6. **`scArrange` has no `OnSysCommand` case** (`messages.h:313`) — the one gap
   in an otherwise complete table.
7. **`Message` is layout-matched to `MSG` by field order only**, with `lPrivate`
   guarded `#ifdef _MAC` (`message.h:41`–`43`) — on Windows the struct has no
   `lPrivate`, unlike the SDK record.

## 50a — `parts/` subtree (detailed)

All 18 `view/handlers/parts/` files are leaf handlers owned by
`BaseWindowHandler`. `base.h:33`–`40` includes the eight concrete part headers
(`style` `basetext` `owner` `timer` `user` `devicechange` `menu` `lifetime`),
each of which pulls `parts/part.h` at its own `:28`; `base.h:56`–`63` keeps
public `ref_of<...>` slots for them, and `base.cpp:44`–`51` offers those slots in
the fixed chain order already documented for the root.

| Pair | Lines (h / cpp) | Shape |
|---|---|---|
| `part` | 50 / 62 | `BaseWindowPartHandler` — hook-chain base only |
| `basetext` | 46 / 75 | three text messages |
| `devicechange` | 44 / 55 | one device-change message |
| `lifetime` | 69 / 131 | eight lifetime/system-error cases; top-level override anchor |
| `menu` | 73 / 203 | eleven menu messages — largest pair here |
| `owner` | 47 / 78 | three menu-loop/idle messages |
| `style` | 53 / 69 | two style-change messages |
| `timer` | 46 / 55 | one timer message |
| `user` | 46 / 60 | one `wmUserChanged` message |

### The base part class — `part.h` / `part.cpp`

`part.h:32` declares `BaseWindowPartHandler`, not `BaseWindowHandler`. It owns a
separate hook link: protected raw pointer `m_nextHookHandler{nullptr}`
(`part.h:44`). It declares `HookHandler` virtual (`part.h:38`), `ProcessHook`
**non-virtual** (`part.h:39`), and `ProcessMessage` virtual (`part.h:40`).

`part.cpp:32`–`35` initialises `m_nextHookHandler` to `nullptr`;
`HookHandler` (`:37`–`49`) stores the first controller at the empty slot
(`:42`) or recurses (`:46`); `ProcessHook` (`:56`–`59`) forwards only when
`m_nextHookHandler` is non-null. `BaseWindowPartHandler::ProcessMessage`
(`:51`–`54`) is just `return ProcessHook(...)`, so an unoverridden part class
would do nothing but walk its own hook chain. Every concrete part handler listed
below overrides `ProcessMessage`.

Repo-wide grep finds no caller of `HookHandler`, so `m_nextHookHandler` is
effectively dead in the built tree — the same "present but unexercised" shape as
`BaseWindowHandler::HookController`.

### Anchor — `LifetimeHandler` and `TopLevelLifetimeHandler`

`parts/lifetime.h:32`–`46` declares `CreateStruct`, a hand mirror of the Win32
create record: `lpCreateParams`, `hInstance`, `hMenu`, `hwndParent`, `cy`, `cx`,
`y`, `x`, `style`, `lpszName`, `lpszClass`, and `dwExStyle` (`:34`–`45`).
`LifetimeHandler` itself starts at `lifetime.h:48` and declares
`ProcessMessage` (`:55`) plus seven lifetime hooks (`:57`–`63`):
`OnSystemError`, `OnCreate`, `OnEnable`, `OnClose`, `OnDestroy`,
`OnActivate`, and `OnMouseActivate`.

`parts/lifetime.cpp:32`–`93` implements `ProcessMessage`. It enters the part
hook at `:34`, then switches on these ids:

| Case | Lines | What the base handler does |
|---|---|---|
| `wmNull` | `:38`–`40` | deliberately ignores it and returns `true` (`lifetime.cpp:39` comment) |
| `wmCreate` | `:41`–`49` | casts `lParam` to `CreateStruct*` (`:43`), calls `OnCreate` (`:44`); if true sets `lResult = 0` (`:46`); returns `true` (`:48`) |
| `wmDestroy` | `:50`–`54` | `lResult = OnDestroy(...)` (`:52`), returns `true` (`:53`) |
| `wmActivate` | `:55`–`59` | `lResult = OnActivate(...)` (`:57`), returns `true` (`:58`) |
| `wmMouseActivate` | `:60`–`64` | `lResult = OnMouseActivate(...)` (`:62`), returns `true` (`:63`) |
| `wmEnable` | `:65`–`73` | `bEnable = wParam ? true : false` (`:67`), calls `OnEnable` (`:68`), zeroes `lResult` on true (`:70`), returns `true` (`:72`) |
| `wmClose` | `:74`–`81` | calls `OnClose` (`:76`), zeroes `lResult` on true (`:78`), returns `true` (`:80`) |
| `0x0017` / `WM_SYSTEMERROR` | `:82`–`87` | calls `OnSystemError`, then forces `lResult = 0` (`:85`) |

The id constants are `wmNull 0x0000` (`messages.h:42`), `wmCreate 0x0001`
(`:43`), `wmDestroy 0x0002` (`:44`), `wmActivate 0x0006` (`:47`), `wmEnable
0x000A` (`:50`), and `wmClose 0x0010` (`:56`); `0x0017` is hardcoded at
`lifetime.cpp:82`. The base defaults are inert: `OnSystemError` writes
`lResult = 0` (`lifetime.cpp:95`–`98`), `OnCreate`/`OnEnable`/`OnClose` return
`false` (`:100`–`103`, `:105`–`108`, `:110`–`113`), and `OnDestroy`/`OnActivate`/
`OnMouseActivate` return `0` (`:115`–`118`, `:120`–`123`, `:125`–`128`).

`TopLevelLifetimeHandler` is the answer to the root page's open question. It is
declared at `toplevel.h:40`–`43` as `: public LifetimeHandler`, and its
`OnDestroy` declaration (`toplevel.h:42`) has no `virtual` keyword and no
access specifier after the class, so it is **private by default**. That does not
hide the function: `LifetimeHandler::OnDestroy` is virtual at `lifetime.h:61`,
so `TopLevelLifetimeHandler::OnDestroy` overrides it. Its definition at
`toplevel.cpp:35`–`39` calls
`qor::win::api::User32::PostQuitMessage(EXIT_SUCCESS)` (`:37`) and returns `0`
(`:38`).

`TopLevelWindowHandler::TopLevelWindowHandler()` (`toplevel.cpp:41`–`44`)
installs it at `:43` with `m_lifetime = new_ref<TopLevelLifetimeHandler>();`.
That overwrites the `BaseWindowHandler` slot declared at `base.h:56`. Therefore
the live `mainwindow` path for `wmDestroy` is:
`toplevel.cpp:166` → `InteractiveWindowHandler::ProcessMessage` (`interactive.cpp:35`)
→ `RenderedWindowHandler::ProcessMessage` (`rendered.cpp:35`)
→ `BaseWindowHandler::ProcessMessage` (`base.cpp:44`, first term of the part chain)
→ `LifetimeHandler::ProcessMessage` inherited by `TopLevelLifetimeHandler`
(`lifetime.cpp:50`) → `TopLevelLifetimeHandler::OnDestroy` (`toplevel.cpp:35`)
→ `PostQuitMessage(EXIT_SUCCESS)`.

### Other leaf handlers — what each one claims

All seven non-lifetime concrete handlers follow the same outer shape:
`ProcessMessage` calls `ProcessHook` first, then switches only on its own message
ids, and returns `false` for anything unmatched (`basetext.cpp:54`,
`devicechange.cpp:44`, `menu.cpp:131`, `owner.cpp:57`, `style.cpp:53`,
`timer.cpp:44`, `user.cpp:48`). Their default virtual hooks mostly either return
`false`/`0` or forward to `Window::DefWindowProcT`, which is
`User32::DefWindowProcT` on the window's stored handle (`window.cpp:104`–`106`).

| Handler | Messages claimed | Lines | Default behaviour |
|---|---|---|---|
| `BaseTextHandler` | `wmGetTextLength`, `wmGetText`, `wmSetText` | `basetext.h:39`–`41`, `basetext.cpp:38`–`52` | each sets `lResult` from an `On...` call and returns `true`; the defaults call `DefWindowProcT` (`:59`–`62`, `:64`–`67`, `:69`–`72`) |
| `DeviceChangeHandler` | `wmDeviceChange` | `devicechange.h:39`, `devicechange.cpp:38`–`42` | `lResult = OnDeviceChange(...)` (`:40`), default forwards `DefWindowProcT` (`:51`) |
| `StyleHandler` | `wmStyleChanging`, `wmStyleChanged` | `style.h:45`–`46`, `style.cpp:38`–`51` | casts `lParam` to `StyleStruct*` (`:40`/`:47`), sets `bExtended = (wParam == gwlExStyle)` (`:41`/`:48`), then `lResult = On... ? 0 : DefWindowProcT(...)` (`:42`/`:49`) |
| `MenuHandler` | 11 menu ids | `menu.h:56`–`66`, `menu.cpp:38`–`129` | see detailed table below |
| `OwnerHandler` | `wmEnterMenuLoop`, `wmExitMenuLoop`, `wmEnterIdle` | `owner.h:39`–`41`, `owner.cpp:38`–`55` | each `On...` default returns `false` (`:62`–`65`, `:67`–`70`, `:72`–`75`), so the ternary calls `DefWindowProcT` (`:41`, `:47`, `:53`) |
| `TimerHandler` | `wmTimer` | `timer.h:39`, `timer.cpp:38`–`42` | `lResult = OnTimer(...) ? 0 : DefWindowProcT(...)` (`:40`), default false (`:51`) |
| `UserHandler` | `wmUserChanged` | `user.h:39`, `user.cpp:42`–`46` | `lResult = OnUserChanged(...) ? 0 : DefWindowProcT(...)` (`:44`), default false (`:56`) |

Message ids cited from `view/messages.h`: `wmGetTextLength 0x000E` (`:54`),
`wmGetText 0x000D` (`:53`), `wmSetText 0x000C` (`:52`), `wmDeviceChange 0x0219`
(`:218`), `wmStyleChanging 0x007C` (`:109`), `wmStyleChanged 0x007D` (`:110`),
`gwlExStyle (-20)` (`:291`), `wmEnterMenuLoop 0x0211` (`:211`), `wmExitMenuLoop
0x0212` (`:212`), `wmEnterIdle 0x0121` (`:167`), `wmTimer 0x0113` (`:158`), and
`wmUserChanged 0x0054` (`:106`).

Two of those ids are **already stolen by the top level**, so the base chain does
not see them in the live top-level handler tree:

- `wmDeviceChange` is handled at `toplevel.cpp:152`–`156` before the
  fallthrough at `:166`, so `DeviceChangeHandler` is unreachable for that id in
  the top-level tree.
- `wmEnterIdle` is handled at `toplevel.cpp:71`–`79` before the fallthrough, so
  `OwnerHandler`'s `wmEnterIdle` case (`owner.cpp:50`–`55`) is unreachable for
  that id in the top-level tree.

`StyleHandler` also carries a nested `StyleStruct` inside the class body
(`style.h:36`–`40`: `styleOld` and `styleNew`). The handler does not inspect the
struct's fields; it only casts the pointer and decides `bExtended` from
`wParam == gwlExStyle`.

### `MenuHandler` — the largest part, 11 cases

`parts/menu.h:32`–`46` declares two records beside the handler: `MDINextMenu`
(`hmenuIn` `hmenuNext` `hwndNext`, `:33`–`37`) and `MenuGetObjectInfo`
(`dwFlags` `uPos` `hmenu` `riid` `pvObj`, `:39`–`46`). `MenuHandler` runs
`menu.h:48`–`68`: `ProcessMessage` at `:55`, eleven hooks at `:56`–`66`.

| Case | Lines | Parameter crack | Hook call |
|---|---|---|---|
| `wmContextMenu` | `:38`–`48` | `Window wnd(PrimitiveHandle((void*)wParam))` (`:40`), `wXPos = LoWord(lParam)` / `wYPos = HiWord(lParam)` (`:41`–`42`) | `if(!OnContextMenu(...)) Wnd.DefWindowProcT(...)` (`:43`, `:45`) — the `DefWindowProcT` result is **discarded** |
| `wmInitMenu` | `:49`–`54` | `Menu menu(PrimitiveHandle((void*)wParam))` (`:51`) | `lResult = OnInitMenu(...) ? 0 : DefWindowProcT(...)` (`:52`) |
| `wmInitMenuPopup` | `:55`–`62` | handle from `wParam` (`:57`), `wPos = LoWord(lParam)`, `bWindowMenu = HiWord(lParam)` (`:58`–`59`) | ternary (`:60`) |
| `wmUninitMenuPopup` | `:63`–`69` | handle from `wParam` (`:65`), `wDestroyedMenu = HiWord(lParam)` (`:66`) | `OnDestroyedMenuPopup` ternary (`:67`) |
| `wmMenuSelect` | `:70`–`77` | `wIndex`/`wFlags` from `wParam` (`:72`–`73`), handle from **`lParam`** (`:74`) | ternary (`:75`) |
| `wmMenuChar` | `:78`–`85` | `wCharCode`/`wMenuType` from `wParam` (`:80`–`81`), handle from `lParam` (`:82`) | `lResult = OnMenuChar(...)` (`:83`) — no fallback |
| `wmNextMenu` | `:86`–`91` | `MDINextMenu*` from `lParam` (`:88`) | `OnNextMenu(Wnd, wParam, pNextMenu)` (`:89`) — `void`, `lResult` untouched |
| `wmMenuGetObject` | `:92`–`97` | `MenuGetObjectInfo*` from `lParam` (`:94`) | `lResult = OnGetObject(...)` (`:95`) — no fallback |
| `wmMenuRButtonUp` | `:98`–`103` | handle from `lParam` (`:100`) | `OnRButtonUp(...)` (`:101`) — `void`, `lResult` untouched |
| `wmMenuCommand` | `:104`–`123` | handle from `lParam` (`:106`); `wItemID = (unsigned short)(-1)` (`:108`), `iIndex = 0` (`:109`) | `OnMenuCommand(...)` (`:121`) — `void`, `lResult` untouched |
| `wmMenuDrag` | `:124`–`129` | handle from `lParam` (`:126`), index = `wParam` (`:127`) | `lResult = OnMenuDrag(...)` (`:127`) — no fallback |

`menu.cpp:131` is the shared `return false`, `:133` the hook-hit `return true`.
All eleven default hooks (`:136`–`200`) are inert: `OnContextMenu`/`OnInitMenu`/
`OnInitMenuPopup`/`OnMenuSelect`/`OnDestroyedMenuPopup` return `false`,
`OnMenuChar`/`OnGetObject`/`OnMenuDrag` return `0`, and `OnNextMenu`/`OnRButtonUp`
/`OnMenuCommand` have empty bodies (`:167`–`170`, `:180`–`182`, `:184`–`187`).
`OnMenuChar` and `OnGetObject` carry the Win32 return-code legends as comments
(`:159`–`163`, `:174`–`176`), and `OnMenuSelect` carries a live `TODO`
(`menu.cpp:153`: "when we get here with a non zero index we need to issue an
actual command related to the menu item").

Two details worth carrying:

- The handle argument switches sides: `wParam` for `wmInitMenu`,
  `wmInitMenuPopup`, `wmUninitMenuPopup`; `lParam` for `wmMenuSelect`,
  `wmMenuChar`, `wmMenuDrag`, `wmMenuRButtonUp`, `wmMenuCommand`. That matches
  the SDK's per-message layout, but it is invisible at the call site.
- `wmMenuCommand` keeps a commented-out Windows 98/ME crack (`:110`–`119`): the
  live path is `iIndex = wParam` (`:118`) and `wItemID` stays `(unsigned short)(-1)`
  (`:108`). The `TheSystem().Info().As<SystemInfo>()->IsWindows98orME()` call it
  replaces is not compiled anywhere in this file.
- The `Menu` in these cases is the controller-layer
  `qor::platform::win::Menu` (`controllers/i_m/menu.h:68`), found by enclosing-namespace
  lookup — `parts/menu.h` includes only `part.h` (`:28`). Each case builds a
  temporary wrapper from the raw handle, and `Menu(const PrimitiveHandle&)`
  (`controllers/i_m/menu.cpp:50`–`53`) calls `m_handle.DontClose()`, so the
  wrapper never destroys the menu it borrowed.


### Reachability — which part slots are ever populated

`BaseWindowHandler()` (`base.cpp:32`–`34`) initialises only
`m_nextHookController`; the eight `ref_of` part slots (`base.h:56`–`63`) are
left default-constructed, and `base.cpp:44`–`51` guards each with
`IsNotNull()`. Repo-wide, exactly two assignments to a part slot exist:

| Site | Assignment | Built? |
|---|---|---|
| `toplevel.cpp:43` | `m_lifetime = new_ref<TopLevelLifetimeHandler>();` | yes |
| `examples/experimental/osspecific/windows/menu/main.cpp:56` | `topLevel->m_menu = menuHandler;` | **no** — `osspecific/windows/CMakeLists.txt:5 #add_subdirectory(menu)` |

So in the live `mainwindow` tree the **only** part handler that runs is the
lifetime one, and the ids it claims — `wmNull`, `wmCreate`, `wmDestroy`,
`wmActivate`, `wmMouseActivate`, `wmEnable`, `wmClose`, `0x0017` — are the only
ids the base level answers at all. No `new_ref<...>` of a concrete part type
appears anywhere else: grep over `view/handlers/**` and the built example finds
`BaseWindowPartHandler` mentioned only in `part.h`/`part.cpp` and in
`user.cpp:32` (`UserHandler::UserHandler() : BaseWindowPartHandler()` — the one
explicit part-constructor definition, all others being `= default`).

`BaseWindowPartHandler::ProcessMessage` (`part.cpp:51`–`54`) is therefore dead:
no object of that exact type is ever constructed.

### 50a gotchas

1. **The per-part hook deferral is a no-op.** Every part's `ProcessMessage`
   opens with `ProcessHook` at its own `:34` (`basetext.cpp`, `devicechange.cpp`,
   `lifetime.cpp`, `menu.cpp`, `owner.cpp`, `style.cpp`, `timer.cpp`, `user.cpp`),
   but `m_nextHookHandler` (`part.h:44`) has no writer: `HookHandler`
   (`part.cpp:37`–`49`) has no caller. Same shape as the unused
   `HookController` of the root.
2. **`LifetimeHandler` claims eight ids unconditionally** — every case ends in
   `return true` regardless of what the hook returns (`lifetime.cpp:48`, `:53`,
   `:58`, `:63`, `:72`, `:80`, `:86`), and `wmNull` returns `true` with a comment
   at `:39` ("Deliberately ignore WM_NULL as per spec"). Because the base level is
   consulted *first* in the dispatch order (`rendered.cpp:35`), these ids never
   reach the rendered or interactive parts.
3. **`parent.cpp:82` and `:87` look like `wmCreate`/`wmDestroy` claims but are not**
   — they are cases of the inner switch on `wEvent` inside
   `ParentHandler::OnParentNotify` (`parent.cpp:78`–`118`), i.e. the *event code*
   carried by `WM_PARENTNOTIFY`, reusing the same constants.
4. **Two part ids are already stolen by the top level**, so their part handlers
   are unreachable even if installed: `wmDeviceChange` (`toplevel.cpp:152`–`156`)
   and `wmEnterIdle` (`toplevel.cpp:71`–`79`).
5. **`0x0017` is the only message id in this subtree with no named constant** —
   `lifetime.cpp:82` writes `case 0x0017/*WM_SYSTEMERROR*/:`; `messages.h` has
   `wmEndSession 0x0016` (`:61`) then jumps to `wmShowWindow 0x0018` (`:66`).
6. **`StyleHandler::OnStyleChanging/OnStyleChanged` never read the style record.**
   They decide `bExtended` by comparing the message *parameter* against
   `gwlExStyle (-20)` (`messages.h:291`, a `GetWindowLong` field offset) — the
   nested `StyleStruct` (`style.h:36`–`40`) is cast but never inspected.
7. **`UserHandler::OnUserChanged` is documented as dead at Vista** — the comment
   at `user.cpp:53` says "Not supported as of Windows Vista", and `wmUserChanged`
   is `0x0054` (`messages.h:106`).

## 50b — `interactive/` subtree (detailed)

Walked in full this pass (27 Oct 2026): the 22 files under
`view/handlers/interactive/` — eleven `BaseWindowPartHandler`-derived classes.
Every claim below is cited; line numbers are editor 1-based.

| Pair | `.h` | `.cpp` | message ids its `ProcessMessage` claims |
|---|---|---|---|
| `focus` | 53 | 93 | 2 |
| `ncpointer` | 57 | 177 | 10 |
| `pointer` | 61 | 230 | 14 |
| `parent` | 63 | 175 | 3 |
| `keyboardinput` | 57 | 181 | 10 |
| `ime` | 56 | 172 | 11 |
| `scroll` | 47 | 73 | 2 |
| `help` | 56 | 62 | 1 |
| `draganddrop` | 45 | 61 | 1 |
| `clipboard` | 56 | 163 | 8 |
| `winctrlparent` | 70 | 272 | 13 |

75 claims in total. Headers include `"../parts/part.h"` for the part base
(`clipboard.h:28`, `ime.h:28`, `parent.h:30`) and several add
`src/platform/os/windows/common/structures.h` (`clipboard.h:29`) for
`PaintStruct` / `Rect` / `NotificationMessageHeader`. Each `.cpp` includes
`"../../messages.h"` — two levels up, because these files sit *below* the
`handlers` level where `interactive.cpp` needs only `"../messages.h"`
(`interactive.cpp:28`).

### Entry point and chain order

`TopLevelWindowHandler::ProcessMessage` ends by *explicitly* re-entering the
level below: `return InteractiveWindowHandler::ProcessMessage(...)`
(`toplevel.cpp:166`) — a qualified call, not a virtual dispatch.

That level is a 13-term short-circuit `||` chain (`interactive.cpp:34`–`46`):

1. `ProcessHook` (`:34`) — resolves to the non-virtual
   `BaseWindowHandler::ProcessHook` (decl `base.h:51`, def `base.cpp:36`–`39`),
   the fourth re-entry of the same empty hook chain per message
   (`toplevel.cpp:64`, `interactive.cpp:34`, `rendered.cpp:34`, `base.cpp:43`)
2. `RenderedWindowHandler::ProcessMessage` (`:35`) — qualified, so the whole
   rendered level runs *before* any interactive part
3. `m_ncPointer` `:36` · 4. `m_pointer` `:37` · 5. `m_parent` `:38` ·
   6. `m_focus` `:39` · 7. `m_keyboardInput` `:40` · 8. `m_IME` `:41` ·
   9. `m_scroll` `:42` · 10. `m_help` `:43` · 11. `m_dragAndDrop` `:44` ·
   12. `m_clipboardOwner` `:45`
13. `HandleMessage` (`:46`), defined always-`false` at `interactive.cpp:49`–`52`

Chain order is **not** declaration order: slots are declared
`m_focus, m_ncPointer, m_pointer, m_keyboardInput, m_dragAndDrop, m_IME,
m_help, m_scroll, m_clipboardOwner, m_parent` (`interactive.h:52`–`61`), so
`parent` — the tenth slot — is fifth in the chain, and `dragAndDrop` /
`clipboard` are pushed to the tail.

`InteractiveWindowHandler() = default;` (`interactive.h:46`) and `interactive.cpp`
defines no constructor; repo-wide there is **no writer** of any of those ten
slots (no `m_ncPointer =`, no `new_ref<PointerHandler>`-style construction of
any interactive handler outside the unbuilt `retropad` example). Contrast the
two part-slot assignments that do exist: `toplevel.cpp:43`
(`m_lifetime = new_ref<TopLevelLifetimeHandler>()`) and
`examples/.../menu/main.cpp:56` (that example is commented out of
`osspecific/windows/CMakeLists.txt:5`), plus the *rendered*-level slots the live
example fills at `mainwindow.cpp:325`/`327`/`329`
(`styledHandler->m_ncRendering`, `m_nonClient`, `m_rendering`). Since
`Ref<T>::IsNotNull()` is `m_p != nullptr && (!m_p->IsNull())`
(`src/qor/memory/reference/ref.h:284`–`286`), every guard in the chain is false.

**Net runtime effect:** the interactive level contributes only
`ProcessHook (false) || rendered level || false`. All 75 claims in this subtree
are dead code as shipped.

### Per handler

All eleven share the same opening: `if (ProcessHook(...)) return true;` at their
own `:34`, then `switch (msg)`; every `case` ends `return true` whether or not
the hook did work. The differences are the id sets and what the default hooks do.

**`focus`** — the only handler with a second base:
`FocusHandler : public BaseWindowPartHandler, public virtual SignalBase`
(`focus.h:33`), and the only one declaring signals:
`SetFocusSignal`/`KillFocusSignal` (`focus.h:40`, `:41`), defined with
`qor_pp_signal_func` + `qor_pp_signal` (`focus.cpp:32`–`35`, `:37`–`40`).
Claims `wmSetFocus 0x0007` (`messages.h:48`) at `:57` and
`wmKillFocus 0x0008` (`:49`) at `:66`; both build
`Window otherWindow(PrimitiveHandle((void*)(wParam)))` (`:59`, `:68`) and
`return true`. Default `OnSetFocus` (`:80`–`:84`) / `OnKillFocus` (`:86`–`:90`)
post the signal and return `true`, so `lResult = 0` (`:62`, `:71`).
`SetFocus()` (`:42`–`:46`) is a dead stub — body is `//TODO:` plus a commented
`window.SetFocus();`, and it has no caller.

**`pointer`** — 14 claims: `wmMouseMove 0x0200` … `wmMouseWheel 0x020A`
(`messages.h:184`–`:194`) at `:41`,`:49`,`:57`,`:65`,`:73`,`:81`,`:89`,`:97`,
`:105`,`:113`,`:121`; `wmCaptureChanged 0x0215` (`messages.h:215`) at `:129`;
`wmMouseHover 0x02A1`/`wmMouseLeave 0x02A3` (`messages.h:244`,`:245`) at
`:138`,`:146`. All fourteen hooks `return false` (`:159`–`:227`). Only
`wmCaptureChanged` is not `wParam`/`lParam`-shaped: it wraps `lParam` in a
`Window otherWindow` (`:131`). Not claimed: `wmXButtonDown 0x020B`,
`wmXButtonUp 0x020C` (`messages.h:195`,`:196`), nor the NC variants
`wmNCMouseHover 0x02A0`/`wmNCMouseLeave 0x02A2` (`:246`,`:247`).

**`ncpointer`** — 10 claims, the block `wmNCMouseMove 0x00A0`…`wmNCMButtonDblClk
0x00A9` (`messages.h:128`–`:137`) at `:41`,`:49`,`:57`,`:65`,`:73`,`:81`,`:89`,
`:97`,`:105`,`:113`. Every hook returns `false` (`:126`–`:174`). The NC X-button
ids `wmNCXButtonDown 0x00AB`–`wmNCXButtonDblClk 0x00AD` (`messages.h:138`–`:140`)
are absent, as are the NC hover/leave pair.

**`parent`** — claims only three ids: `wmParentNotify 0x0210`
(`messages.h:210`) at `:47`, `wmNotifyFormat 0x0055` (`messages.h:107`) at
`:61`, `wmNotify 0x004E` (`messages.h:101`) at `:67`. Its `ProcessMessage`
(`:32`–`:41`) is the smoking gun for `winctrlparent`:

```cpp
return /*TODO: WinCtrlParent().ProcessMessage(...) ||*/
    HandleMessage(window, lResult, msg, wParam, lParam);   // parent.cpp:39–:40
```

the delegation to the window-control level is **commented out**, so
`WinCtrlParentHandler` is unreachable even if a slot were filled —
and `m_winCtrlParent` (`parent.h:56`) has no writer either.
`OnParentNotify` (`:78`–`:118`) switches on the *event code* carried by
`WM_PARENTNOTIFY`: `wmCreate`, `wmDestroy`, `wmLButtonDown`, `wmMButtonDown`,
`wmRButtonDown`, `wmXButtonDown` → `OnChild*` hooks (all `false`, `:144`–`:172`).
Because the default hook returns `false`, `wmParentNotify` falls to
`lResult = window.DefWindowProcT(uMsg, wParam, lParam)` at `:57` — the one place
this handler re-dispatches to the window's `WndProc`.
`OnNotify` (`:120`–`:130`) returns `0` with its dispatch commented at `:127`;
`OnNotifyFormat` (`:132`–`:142`) returns `nfrUnicode` (2, `messages.h:300`)
under `qor_pp_unicode` else `nfrANSI` (1, `messages.h:299`).

**`winctrlparent`** — largest pair (70/272). 13 claims: the seven
`wmCtlColor*` ids `0x0132`–`0x0138` (`messages.h:176`–`:182`) at `:41`,`:48`,
`:55`,`:62`,`:69`,`:76`,`:83`, plus `wmDrawItem 0x002B` (`messages.h:83`) `:90`,
`wmCompareItem 0x0039` (`messages.h:93`) `:97`, `wmMeasureItem 0x002C` (`:84`)
`:102`, `wmDeleteItem 0x002D` (`:85`) `:109`, `wmVKeyToItem 0x002E` (`:86`)
`:116`, `wmCharToItem 0x002F` (`:87`) `:124`. Each colour case builds a
non-owning `DeviceContext dc(PrimitiveHandle((void*)(wParam)), false)` and a
`Window` from `lParam`, then `lResult = reinterpret_cast<long long>(OnCtl…
.GetHandle().Use())` (`:45`,`:52`,`:59`,`:66`,`:73`,`:80`,`:87`) — the three
colour hooks *do* work: each returns
`Brush(PrimitiveHandle(window.DefWindowProcT(<msg id>, dc, child)), false)`
(`:139`, `:144`, `:150`). Quirk: the `wmCtlColorScrollbar` case passes
`wmCtlColorStatic` (`:80`) and `wmCtlColorDlg` routes to the dedicated
`OnCtlColourDialog` (`:73`) while `wmCtlColorMsgBox` uses `OnCtlColourMsgBox`
(`:45`) — the other five share the generic `OnCtlColour`.
The item hooks are mostly commented-out legacy code: `OnDrawItem` (`:153`–`:175`)
builds `Window childWindow(PrimitiveHandle(itemStruct->phwndItem))` (`:158`), the
`dynamic_cast<CWinCtrlWindow*>` branch is commented (`:160`–`:168`) so
`bProcessed` stays `false` and it always re-dispatches
`DefWindowProcT(wmDrawItem, wParam, (long long)itemStruct)` at `:173`;
`OnMeasureItem` (`:197`–`:226`) body fully commented; `OnDeleteItem`
(`:228`–`:242`) builds the child then does nothing; `OnCompareItem`
(`:177`–`:195`), `OnVKeyToItem` (`:244`–`:255`), `OnCharToItem`
(`:257`–`:268`) return `0` (`:179` comment: "All items equal by default").
Header parameter names differ from the definitions (`DC`/`pItemStruct` in
`winctrlparent.h:55`–`:64`, `dc`/`itemStruct` in the `.cpp`) — legal, but noisy.

**`ime`** — 11 claims and the heaviest use of re-dispatch in this subtree:
`wmIMEStartComposition 0x010D` (`messages.h:151`) `:41`,
`wmIMEEndComposition 0x010E` (`:152`) `:46`, `wmIMEComposition 0x010F` (`:153`)
`:51`, `wmIMESetContext 0x0281` (`:235`) `:56`, `wmIMENotify 0x0282` (`:236`)
`:61`, `wmIMECompositionFull 0x0284` (`:238`) `:66`, `wmIMESelect 0x0285`
(`:239`) `:71`, `wmIMEChar 0x0286` (`:240`) `:76`, `wmIMEKeyDown 0x0290`
(`:242`) `:91`, `wmIMEKeyUp 0x0291` (`:243`) `:99`, `wmIMERequest 0x0288`
(`:241`) `:107`. Eight of the eleven hooks forward the *same* id back through
`window.DefWindowProcT(...)` (`:119`, `:124`, `:129`, `:134`, `:139`, `:148`,
`:153`, `:168`), and those three `lResult = On…(...)` cases (`:58`, `:63`,
`:109`) make the `WndProc` result the message result. Only
`OnCompositionFull` (`:142`–`:144`) is empty and only `OnKeyDown`/`OnKeyUp`
(`:156`, `:161`) return `false`. Unclaimed: `wmIMEControl 0x0283`
(`messages.h:237`); `wmIMEKeyLast 0x010F` (`:154`) is a duplicate of
`wmIMEComposition`, not a new id.

**`keyboardinput`** — 10 claims: `wmInputLangChangeRequest 0x0050`
(`messages.h:102`) `:41`, `wmInputLangChange 0x0051` (`:103`) `:49`, then the
key block `wmKeyDown 0x0100` … `wmSysDeadChar 0x0107` (`messages.h:142`–`:149`)
at `:57`,`:65`,`:73`,`:81`,`:89`,`:97`,`:105`,`:113` (`wmKeyDown` aliases
`wmKeyFirst 0x0100`, `messages.h:141`). All ten hooks `return false`
(`:126`–`:178`); the "accept by re-dispatch" alternative is left commented at
`:128`–`:131` and `:136`. Two cases invert the usual shape:
`wmInputLangChangeRequest` sets `lResult = 0` when the hook returns **false**
(`:43`–`:47`), and `wmInputLangChange` sets `lResult = 1` when it returns **true**
(`:51`–`:55`). `wmUniChar 0x0109` (`messages.h:150`) is not claimed.

**`scroll`** — 2 claims, and the switch order is inverted relative to the ids:
`wmVScroll 0x0115` (`messages.h:160`) at `:41` before
`wmHScroll 0x0114` (`:159`) at `:49`. Both hooks `false` (`:62`, `:67`).

**`help`** — 1 claim, `wmHelp 0x0053` (`messages.h:105`) at `:41`; casts
`HelpInfo*` from `lParam` (`:43`), the null guard at `:44` only skips the call,
and `lResult = 1` (`:48`) is set unconditionally — so a null `lParam` still
reports a consumed message with result 1. `OnHelp` returns `false` (`:56`–`:59`).

**`draganddrop`** — 1 claim, `wmDropFiles 0x0233` (`messages.h:232`) at `:41`,
`void* hDrop = reinterpret_cast<void*>(wParam)` (`:43`), hook `OnDropFiles`
returns `false` (`:55`–`:58`) so `lResult` is left untouched. Note the
signature differs from the top level's `OnDropFiles(void*)` (`toplevel.h:64`).

**`clipboard`** — 8 claims in the clipboard block:
`wmRenderFormat 0x0305` (`messages.h:256`) `:41`, `wmRenderAllFormats 0x0306`
(`:257`) `:49`, `wmDestroyClipboard 0x0307` (`:258`) `:57`,
`wmPaintClipboard 0x0309` (`:260`) `:65`, `wmVScrollClipboard 0x030A` (`:261`)
`:75`, `wmSizeClipboard 0x030B` (`:262`) `:87`, `wmAskCBFormatName 0x030C`
(`:263`) `:97`, `wmHScrollClipboard 0x030E` (`:265`) `:106`. Unclaimed in the
block: `wmDrawClipboard 0x0308` (`:259`), `wmChangeCBChain 0x030D` (`:264`).
The scroll-clipboard cases split `lParam` with `LoWord`/`HiWord` into
`(wEvent, wPos)` (`:78`–`:79`, `:109`–`:110`); `wmAskCBFormatName` passes a
`char*` buffer plus `wParam` as the count (`:99`), and `PaintStruct*`/`Rect*`
come from `common/structures.h` (`clipboard.h:29`). All eight hooks `false`
(`:122`–`:160`). Two hook names slip through: `wmDestroyClipboard` posts
`OnDestroyData` (`clipboard.h:44`) not `OnDestroy`, and the vertical scroll hook
is `OnVSCrollClipboard` — capital `S` — (`clipboard.h:48`).

### 50b — hook surface vs. claims

Declared `On*` hooks per header (each header's `virtual` keyword count minus its
destructor and `ProcessMessage`, minus `focus.h`'s `public virtual SignalBase`
at `:33`): **77 hooks against the 75 claimed ids**. Ten of the eleven pairs are
exactly one hook per claim; only `parent` and `winctrlparent` diverge.

| Handler | hooks | claims | divergence |
|---|---|---|---|
| `focus` | 2 | 2 | plus a non-virtual `SetFocus` (`focus.h:43`) with no caller |
| `ncpointer` | 10 | 10 | — |
| `pointer` | 14 | 14 | — |
| `parent` | 9 | 3 | the six `OnChild*` hooks (`parent.h:49`–`:54`) are reachable only from the `OnParentNotify` inner switch (`parent.cpp:78`–`:118`) |
| `keyboardinput` | 10 | 10 | — |
| `ime` | 11 | 11 | hook return types are heterogeneous — `void` ×6, `long long` ×3, `bool` ×2 (`ime.h:41`–`:51`) |
| `scroll` | 2 | 2 | — |
| `help` | 1 | 1 | — |
| `draganddrop` | 1 | 1 | — |
| `clipboard` | 8 | 8 | — |
| `winctrlparent` | 9 | 13 | the seven `wmCtlColor*` ids collapse onto three `Brush`-returning hooks (`winctrlparent.h:55`, `:56`, `:57`) |

Cross-check: the eleven headers carry 100 `virtual` keywords = 77 hooks + 11
`ProcessMessage` + 11 destructors + 1 `SignalBase` base. Return types are `bool`
everywhere except `ime.h` (`void` ×6, `long long` ×3), `winctrlparent.h:55`–
`:57` (`Brush` ×3) and `parent.h:47`/`:48` (`long` ×2).

### 50b — build & include facts

All eleven `.cpp` are compiled, contiguously, at `gui/CMakeLists.txt:72`–`82`
(`focus`, `clipboard`, `draganddrop`, `help`, `ime`, `keyboardinput`,
`ncpointer`, `parent`, `pointer`, `scroll`, `winctrlparent`) — the only subtree
block of part 50 with no file missing and no file listed out of block (contrast
`parts/lifetime.cpp` at `:90`).

`view/handlers/interactive.h:29`–`38` includes **ten** of the eleven headers;
`winctrlparent.h` is not among them. It reaches the level TU only through
`interactive/parent.h:31`, i.e. the window-control level is visible exactly where
its parent slot is declared (`parent.h:56`).

Every header takes `../parts/part.h` (which itself pulls only
`../../../window.h`), and only four add anything else:

- `clipboard.h:29` → `src/platform/os/windows/common/structures.h`
  (`PaintStruct :417`, `NotificationMessageHeader :254`)
- `focus.h:29` → `src/framework/event/signals/signal.h` — the only part in this
  subtree that is a `SignalBase`
- `parent.h:28`, `:29` → `src/qor/essentials/current/currentthread.h` and
  `src/qor/memory/reference/newref.h` — the only part header that imports the
  ref-construction machinery, for the never-taken `m_winCtrlParent`
- `winctrlparent.h:29` → `../../../gdiobjects/devicecontext.h`, which itself
  pulls `structures.h` at `devicecontext.h:29` and `rect.h` at `:30`

Every `.cpp` includes `"../../messages.h"` (two levels up) plus its own header
and `src/configuration/configuration.h` — uniform across all eleven.

Two handlers declare their message payload struct locally instead:
`help.h:32`–`:40` `HelpInfo` (inside `qor::platform::win::gui::view`,
duplicating the copy at `common/structures.h:427` in `qor::platform::win` —
different namespaces, so no clash, just two definitions to keep in step) and
`winctrlparent.h:33`–`:44` `DrawItemStruct`, which has **no** counterpart in
`structures.h` (that header has `DeleteItemStruct :261`, `ComparisonItem :270`,
`MeasureItemStruct :282`, `CompareItemStruct :437`, `PaintStruct :417`).

### 50b — how a handler would be installed (and why none is)

The ten slots at `interactive.h:52`–`61` have exactly two writers repo-wide, both
inside the disabled `retropad` example:

1. `examples/experimental/osspecific/windows/retropad/controller.cpp:23` —
   `m_focus = qor::new_ref<FocusHandler>();` from
   `RetroPadTopLevelWindowController::RetroPadTopLevelWindowController()`
   (`controller.h:34` derives `TopLevelWindowHandler`, so `m_focus` is the
   inherited `interactive.h:52` slot), immediately followed by
   `qor::connect(m_focus(qor_shared), &FocusHandler::SetFocusSignal, *this,
   &RetroPadTopLevelWindowController::OnSetFocus)` at `:24`, undone by the
   matching `disconnect` at `:29`. The listener body `controller.h:48`–`:51` is
   empty.
2. `retropad/mainwindowinstancecontroller.cpp:62` —
   `m_dragAndDrop = qor::new_ref<MainWindowDragAndDropController>(m_view)` writes
   the inherited `interactive.h:56` slot (`MainWindowInstanceController` derives
   `TopLevelWindowHandler` at `mainwindowinstancecontroller.h:66` and declares no
   member of that name). `MainWindowDragAndDropController` derives
   `DragAndDropHandler` (`mainwindowinstancecontroller.h:40`) and its
   `OnDropFiles` (`mainwindowinstancecontroller.cpp:45`–`:58`) is the only
   concrete drag-and-drop behaviour anywhere in the tree — `DragQueryFile`, then
   `PromptSaveChanges`/`LoadDocumentFromPath`, then `DragFinish`.

Both are dead three times over: `osspecific/windows/CMakeLists.txt:8` is
`#add_subdirectory(retropad)` (only `mainwindow` at `:4` is enabled); the example
includes a `view/controllers/` tree that no longer exists — `ui/gui/view/` holds
just `drawing`, `handlers`, `layout`, and `controller.h:14` /
`mainwindowinstancecontroller.h:7` ask for `view/controllers/toplevel.h`; and two
override signatures contradict their bases — `MainWindowFocusController::OnSetFocus`
returns `void` (`mainwindowinstancecontroller.h:20`) against `bool` at `focus.h:47`,
and `MainWindowDragAndDropController::OnDropFiles` returns `void` (`:46`) against
`bool` at `draganddrop.h:40`. So the one place that *would* light up the
interactive level cannot build today.

### 50b gotchas

1. **`wmDropFiles 0x0233` is claimed twice** — the top level answers it at
   `toplevel.cpp:98`–`103` *before* re-entering the interactive level at
   `:166`, and `draganddrop.cpp:41` claims it again. The top level always wins,
   so the live `OnDropFiles` work in the retropad example could never run even
   with the slot filled.
2. **`WinCtrlParentHandler` is doubly unreachable** — it is not a slot of
   `InteractiveWindowHandler` at all; the only holder is the `m_winCtrlParent`
   ref of `ParentHandler` (`parent.h:56`), and the delegation that would use it
   is commented at `parent.cpp:39`.
3. **`m_clipboardOwner` (`interactive.h:60`) is the only slot whose name does not
   match its class** (`ClipboardHandler`) — a leftover of an older, owner-flavoured
   design; nothing in this subtree is named "owner" except `parts/owner.h`.
4. **IME defaults re-dispatch and still swallow.** Eight of the eleven IME hooks
   post the *same* id to `DefWindowProcT` (`ime.cpp:119`, `:124`, `:129`, `:134`,
   `:139`, `:148`, `:153`, `:168`) while `ProcessMessage` `return true`s
   regardless — installing `IMEHandler` changes nothing observable except that the
   message stops being offered below. Two of those calls drop the payload:
   `OnStartComposition`/`OnEndComposition` forward `0, 0` (`ime.cpp:119`, `:124`).
5. **`ime.cpp:79`–`:86` is a commented-out WM_IME_CHAR decode** — the repeat
   count, scan code and the DBCS extended/context/previous/transition flag bits
   are all commented; the live code just calls `OnChar` (`:87`) then sets `lResult
   = 0` unconditionally at `:88`.

6. **Return types decide which IME cases can report a result.** Only the three
   `long long` hooks appear as `lResult = On…(...)` (`ime.cpp:58`, `:63`,
   `:109` — `OnSetContext`, `OnNotify`, `OnRequest`); the six `void` hooks
   cannot, so their cases leave `lResult` untouched, and `wmIMEChar` forces 0 at
   `:88`.
7. **The hover/leave block interleaves NC and client ids** — `wmNCMouseHover
   0x02A0`, `wmMouseHover 0x02A1`, `wmNCMouseLeave 0x02A2`, `wmMouseLeave
   0x02A3` (`messages.h:246`, `:244`, `:247`, `:245`), so numeric neighbours
   belong to different handlers; `pointer` claims the client pair, `ncpointer`
   claims neither NC one.
8. **The two language-change cases invert the usual polarity** —
   `keyboardinput.cpp:43` tests `!OnInputLangChangeRequest(...)` and `:51` sets
   `lResult = 1` on truth. That mirrors the Win32 contract (the first asks the app
   to choose a locale, the second reports which one) and is easy to misread as a
   bug.
9. **`HandleMessage` is not exclusive to the level classes** — `ParentHandler`
   declares its own non-virtual `HandleMessage` (`parent.h:44`) and
   `ParentHandler::ProcessMessage` (`parent.cpp:32`–`41`) is a one-line forward to
   it at `parent.cpp:40`; `HandleMessage` itself spans `:43`–`:76`. All three parent
   ids live in that second function, which is why the file's first screen has no
   `switch`.

10. **`pointer` and `ime` overlap conceptually but not by id** — `ime.cpp:91`/
    `:99` handle `wmIMEKeyDown 0x0290` and `wmIMEKeyUp 0x0291`, while
    `keyboardinput.cpp:57`/`:65` handle `wmKeyDown 0x0100` and `wmKeyUp
    0x0101`; the two ids that sit between the blocks — `0x0109 wmUniChar`
    (`messages.h:150`) and `0x0283 wmIMEControl` (`messages.h:237`) — are
    claimed nowhere in the subtree.

## 50c — `rendered/` subtree (detailed)

Seven `BaseWindowPartHandler` leaf pairs, all owned by the seven `RenderedWindowHandler`
slots. This is the subtree that answers the *drawing* half of the Win32 window
contract: the non-client (frame/caption) area, fonts, icons, the cursor, the
system palette, print requests, and the DWM (Windows Vista+ composition) messages.

### File sizes (lines, h / cpp)

| Pair | .h | .cpp | Notes |
|---|---:|---:|---|
| `nc` | 50 | 80 | non-client create/activate/destroy |
| `ncrender` | 55 | 123 | non-client calc-size / paint / hit-test |
| `rendering` | 89 | 343 | the big leaf — 21 ids, 21 hooks |
| `iconized` | 48 | 76 | 4 ids onto 2 hooks |
| `font` | 48 | 66 | set/get font |
| `cursor` | 45 | 62 | the smallest handler in `gui/` |
| `dwm` | 50 | 126 | 6 ids / 6 hooks, Vista+ only |

Subtree total: 14 files, 1211 lines. `rendering.cpp` alone is 28% of the subtree.

### Entry — the level handler at the parent (`rendered.h`, `rendered.cpp`)

The level itself is part 50's root, but its shape decides how these 14 files
run, so it is restated here:

- `rendered.h:28` includes `base.h`; `rendered.h:29`–`35` include the seven
  subtree headers in the order `nc` `ncrender` `rendering` `iconized` `font`
  `cursor` `dwm`.
- The seven slots are declared in **that same order** at `rendered.h:49`–`55`
  (`m_nonClient` `m_ncRendering` `m_rendering` `m_iconized` `m_font`
  `m_cursor` `m_dwm`).
- `rendered.cpp:34`–`43` is a ten-term short-circuit `||` chain:
  `ProcessHook` → **qualified** `BaseWindowHandler::ProcessMessage` (`:35`) →
  the seven slots (`:36`–`42`, each guarded `m_x.IsNotNull()`) →
  `HandleMessage` (`:43`).
- Unlike the `base` and `interactive` levels, **this level's chain order equals
  its declaration order** — the only rendered-level surprise is that the base
  level gets tried *before* any rendered leaf, because `:35` calls the base
  override qualified rather than dispatching through `ProcessMessage`.
- `RenderedWindowHandler::HandleMessage` (`rendered.cpp:46`–`49`) returns
  `false` unconditionally, so an id no rendered leaf claims falls through to
  whichever level above handles it.

### `nc` — `NonClientHandler` (the non-client area lifecycle)

`nc.h:28` includes `../parts/part.h` (the leaf base) and `nc.h:29` includes
`../base.h` — the *level* header, which is how `CreateStruct` reaches this file:
the struct lives in `parts/lifetime.h:32`–`46` and arrives via `base.h:40`.
Three hooks: `OnNCCreate` (`nc.h:42`), `OnNCActivate` (`:43`), `OnNCDestroy`
(`:44`).

| Id | Lines | Decode | Result |
|---|---|---|---|
| `wmNCCreate` | `:38`–`42` | `lParam` → `CreateStruct*` (`:40`) | `lResult = OnNCCreate ? 1 : 0` (`:41`) |
| `wmNCActivate` | `:44`–`51` | `active` from `wParam` (`:46`); `repaint = lParam != -1ll` (`:47`); `prevWindow = repaint ? (void*)lParam : nullptr` (`:48`) | see below |
| `wmNCDestroy` | `:53`–`56` | nothing | `lResult = OnNCDestroy ? 1 : 0` (`:55`) |

All three `return true` (`:42`, `:51`, `:56`), so all three ids are claimed.

`nc.cpp:49` is the one outright bug in this subtree:

```cpp
OnNCActivate(window, active, repaint, prevWindow) ? 1 : 0;   // value discarded
lResult = window.DefWindowProcT(msg, wParam, lParam);        // :50
```

The ternary is an unused expression — the `lResult =` prefix that the other two
cases have is missing, so `OnNCActivate`'s verdict never reaches the shell and
`lResult` is always whatever `DefWindowProcT` returns. The base defaults are
`OnNCCreate` → `true` (`:66`), `OnNCActivate` → `true` (`:72`), `OnNCDestroy` →
`false` (`:77`), so out of the box `wmNCCreate` reports `1` and `wmNCDestroy`
reports `0`.

### `ncrender` — `NonClientRenderingHandler` (frame sizing, painting, hit-testing)

`ncrender.h:33`–`37` declares the payload struct **locally**:
`NCCalcSizeParams { Rect rgrc[3]; WindowPos* lppos; }`. `WindowPos` is not a
Win32 SDK type and has no `structures.h` counterpart — it is defined at
`rendering.h:43`–`52`, which is why `ncrender.h:29` includes its sibling
`rendering.h`. (`MinMaxInfo` `rendering.h:34`–`41` is the other local mirror;
all three local structs in this subtree are single-definition, unlike the
`help.h`/`structures.h` duplication found in 50b.)

Three hooks (`ncrender.h:48`–`50`), three claimed ids:

| Id | Lines | Decode | Result |
|---|---|---|---|
| `wmNCCalcSize` | `:38`–`45` | `indicateValidClientArea` from `wParam` (`:40`); `lParam` becomes `NCCalcSizeParams*` **or** `Rect*` depending on that flag, the other pointer is `nullptr` (`:41`/`:42`) | `lResult = !indicateValidClientArea ? DefWindowProcT(...) : result` (`:44`) |
| `wmNCPaint` | `:47`–`51` | `wParam` is the update-region handle (comment `:49`) | `lResult = OnNCPaint ? 0 : DefWindowProcT(...)` (`:50`) |
| `wmNCHitTest` | `:53`–`59` | `x = GetXLParam(lParam)`, `y = GetYLParam(lParam)` (`:55`/`:56`, screen-relative per comment) | `lResult = OnNCHitTest(...)` (`:57`) |

Two design notes fall out of `:44`: when the window is asking for its
*valid client area* (`wParam` non-zero) the handler's own return code is used
and the Win32 default is skipped; when it is only asking for a proposed window
rect, the hook's return value is computed into a local `result` (`:43`) and
**thrown away** in favour of `DefWindowProcT`.

The base defaults are documentation-carriers rather than behaviour:

- `OnNCCalcSize` (`ncrender.cpp:66`–`79`) carries the whole Win32 `WVR_*`
  return-code table as a block comment (`:68`–`77`) and returns `0` —
  "preserve old client area and align with top left" (`:78` comment).
- `OnNCPaint` (`:81`–`88`) carries the commented-out `GetDCEx` / paint /
  `ReleaseDC` sketch (`:83`–`86`) and returns `false`.
- `OnNCHitTest` (`:90`–`120`) carries the full `HT*` code table (`:92`–`118`)
  and returns `window.DefWindowProcT(wmNCHitTest, wParam, lParam)` (`:119`) —
  the same message id re-dispatched through the Win32 shell, and the raw
  `wParam`/`lParam` are passed on again even though `x`/`y` were already cracked
  at the call site.

### `rendering` — `RenderingHandler` (the big leaf: 21 ids / 21 hooks)

`rendering.h:28` includes `../parts/part.h`; `:29`/`:30` include
`../../../gdiobjects/icon.h` and `../../../gdiobjects/devicecontext.h` for the
`Icon&` and `DeviceContext&` hook parameters. Two payload structs are declared
here: `MinMaxInfo` (`:34`–`41`, mirroring `MINMAXINFO`) and `WindowPos`
(`:43`–`52`, mirroring `WINDOWPOS` — not to be confused with the *other*
placement record, `WindowPlacement` in `controllers/windowcontroller.h:84`–`95`).
The class declares 21 hooks at `rendering.h:63`–`83`; `rendering.cpp`'s
`switch` has exactly 21 cases at `:40`–`215` (switch closes `:216`,
unclaimed ⇒ `return false` `:217`, claimed ⇒ `return true` `:219`).

| Id | case | Decode | Result style |
|---|---:|---|---|
| `wmPaint` | `:40` | — | hook true ⇒ `lResult = 0` (`:44`) |
| `wmSetRedraw` | `:48` | `bRedraw = (wParam == 1)` (`:50`) | hook true ⇒ `0` (`:53`) |
| `wmEraseBackground` | `:57` | `DeviceContext dc(wParam)` (`:59`) | hook true ⇒ `0` (`:62`) |
| `wmSysColorChange` | `:66` | — | hook true ⇒ `0` (`:70`) |
| `wmShowWindow` | `:74` | `bShow = wParam` (`:76`) | hook true ⇒ `0` (`:79`) |
| `wmGetMinMaxInfo` | `:83` | `lParam` → `MinMaxInfo*` (`:85`) | hook true ⇒ `0` (`:88`) |
| `wmWindowPosChanging` | `:92` | `lParam` → `WindowPos*` (`:94`) | hook true ⇒ `0` (`:97`) |
| `wmWindowPosChanged` | `:101` | `lParam` → `WindowPos*` (`:103`) | hook true ⇒ `0` (`:106`) |
| `wmDisplayChange` | `:110` | `wBitsPP` from `wParam` (`:112`), `hRes`/`vRes` = `LoWord`/`HiWord(lParam)` (`:113`/`:114`) | void hook (`:115`), `lResult` untouched |
| `wmGetIcon` | `:118` | `icon` from `wParam` (`:128`), `dpi = lParam` (`:129`); ICON_BIG/SMALL/SMALL2 table comment `:120`–`127` | `lResult = OnGetIcon(...)` (`:130`) |
| `wmSetIcon` | `:133` | `Icon icon(lParam)` (`:135`), `size = wParam` (`:136`) | `lResult = OnSetIcon(...)` (`:137`) |
| `wmSyncPaint` | `:140` | — | hook true ⇒ `0` (`:144`) |
| `wmSizing` | `:148` | `lParam` → `Rect*` (`:150`), `wParam` = edge | hook true ⇒ **`lResult = 1`** (`:153`) |
| `wmMoving` | `:157` | `lParam` → `Rect*` (`:159`) | hook true ⇒ **`lResult = 1`** (`:162`) |
| `wmQueryNewPalette` | `:166` | — | `lResult = hook ? 1 : 0` (`:168`) |
| `wmPaletteIsChanging` | `:171` | `Window win(wParam)` (`:173`) | hook true ⇒ `0` (`:176`) |
| `wmPaletteChanged` | `:180` | `Window win(wParam)` (`:182`) | void hook (`:183`), `lResult` untouched |
| `wmPrint` | `:186` | `DeviceContext dc(wParam)` (`:188`) | hook true ⇒ `0` (`:191`) |
| `wmPrintClient` | `:195` | `DeviceContext dc(wParam)` (`:197`) | hook true ⇒ `0` (`:200`) |
| `wmSize` | `:204` | — | void hook (`:206`), then `lResult = 0` (`:207`) |
| `wmMove` | `:210` | — | void hook (`:212`), then `lResult = 0` (`:213`) |

Four result styles, and the split matters for the Win32 return value:

1. **`if (hook) lResult = 0;`** — 12 ids (`wmPaint` `wmSetRedraw`
   `wmEraseBackground` `wmSysColorChange` `wmShowWindow` `wmGetMinMaxInfo`
   `wmWindowPosChanging` `wmWindowPosChanged` `wmSyncPaint`
   `wmPaletteIsChanging` `wmPrint` `wmPrintClient`). When the hook declines
   (the default), `lResult` keeps whatever the *caller* left in it — **no
   `DefWindowProcT` fallback**, so the message is swallowed and the shell's
   default frame painting never happens.
2. **`lResult = hook ? 1 : 0`** — `wmSizing` `wmMoving` `wmQueryNewPalette`.
3. **`lResult = hook(...)`** directly — `wmGetIcon` `wmSetIcon` (the previous
   icon handle is the Win32 contract, comment `rendering.cpp:284`).
4. **void hook, `lResult = 0`** — `wmSize` `wmMove`; and `wmDisplayChange` /
   `wmPaletteChanged`, which leave `lResult` alone entirely.

`RenderingHandler::OnSize` and `OnMove` are the only two defaults in this leaf
that call the Win32 shell at all — `rendering.cpp:224`
`window.DefWindowProcT(wmSize, wParam, lParam)` and `:229`
`window.DefWindowProcT(wmMove, 0, MakeLong(wXPos, wYPos))`, i.e. `WM_MOVE` is
re-dispatched with a synthesised packed-point `lParam` and `wParam` forced to
`0`.

The 21 hook bodies sit at `rendering.cpp:232`–`340`: 17 return `false`,
`OnGetIcon`/`OnSetIcon` return `0` (`:279`, `:284`), and `OnDisplayChange`
(`:273`–`275`) plus `OnPaletteChanged` (`:320`–`327`) are empty void bodies.
Doc comments carried in the defaults: `OnSysColourChange` has the only real
TODO here — `rendering.cpp:249` "Forward to all children and regen all
SysColour Brushes"; `OnQueryNewPalette` (`:304`–`309`) and
`OnPaletteChanging` (`:315`–`316`) carry the Win32 palette-message prose;
`OnPrintClient` `:336`–`338` the printer-DC prose.

### `iconized` — `IconizedHandler` (4 ids on 2 hooks)

`iconized.h:29` includes `../../../gdiobjects/icon.h`. Hooks: `OnQueryOpen`
(`:41`) and `OnQueryDragIcon` (`:42`).

| Id | case | Behaviour |
|---|---:|---|
| `wmQueryOpen` | `:38` | `lResult = OnQueryOpen ? 1 : 0` (`:40`) |
| `wmPaintIcon` | `:43` | `//Old win 3.x message` (`:44`) then `return true` (`:45`) — no hook, no `lResult` |
| `wmIconEraseBkgnd` | `:47` | same, comment at `:49`, `return true` (`:50`) |
| `wmQueryDragIcon` | `:52` | `OnQueryDragIcon` → `PrimitiveHandle`, then `lResult = reinterpret_cast<long long>(iconHandle.Use())` (`:55`) |

This is the subtree's only id/hook mismatch: **4 claimed ids, 2 hooks**. The
two Windows 3.x icon messages (`WM_PAINTICON` `0x26`, `WM_ICONERASEBKGND`
`0x27`) are swallowed with no hook at all, so they can never be overridden.
`OnQueryOpen`'s default returns **`true`** (`iconized.cpp:67`) with the prose
"No activation or focus changes allowed here" (`:66`) and "Can the iconized
window be restored?" (`:67`) — so the built-in answer to `WM_QUERYOPEN` is
`lResult = 1`. `OnQueryDragIcon` returns `PrimitiveHandle(0)` (`:72`).

### `font` — `FontHandler` (2 ids, 2 hooks)

`font.h:29` includes `../../../gdiobjects/font.h` for the by-value `Font`
parameter. `wmSetFont` (`font.cpp:38`–`44`): `Font font(PrimitiveHandle((void*)(wParam)), false)`
(`:40`), `bRedraw = LoWord(lParam) ? true : false` (`:41`), void hook (`:42`),
`lResult` never touched. `wmGetFont` (`:45`–`50`): the hook's `PrimitiveHandle`
is rebound (`:47`) and `.Use()` is `reinterpret_cast` to `long long` into
`lResult` (`:48`). Defaults: `OnSetFont` empty (`:57`–`59`), `OnGetFont`
returns `PrimitiveHandle(0)` (`:63`) — i.e. "no font", which the shell reads as
a null handle.

`font.h:41`–`42` are the two lines in the whole subtree that are indented with
tabs rather than spaces — cosmetic only, but it breaks the file's visual
alignment.

### `cursor` — `CursorHandler` (1 id, 1 hook — the smallest handler)

`cursor.h:28` includes only `../parts/part.h`; it deals in raw handles, not
QOR GDI objects. `wmSetCursor` (`cursor.cpp:42`–`49`): `Window wndOver(PrimitiveHandle((void*)(wParam)))`
(`:44`), `hitTestCode = LoWord(lParam)` (`:45`), `mouseMsgId = HiWord(lParam)`
(`:46`), `lResult = OnSetCursor(...) ? 1 : 0` (`:47`). Default `OnSetCursor`
returns `false` (`cursor.cpp:58`) — so by default **the cursor is never set**:
`WM_SETCURSOR` is claimed (`return true` `:48`) and `lResult = 0`, which is the
"success, nothing done" answer rather than a fall-through to `DefWindowProcT`.

`cursor` is also the only handler here whose constructor is user-declared
(`cursor.h:36`) and defined (`cursor.cpp:32`–`34`, an empty
`CursorHandler::CursorHandler() : BaseWindowPartHandler()`), and whose
destructor lacks the `noexcept` that the other six use.

### `dwm` — `DesktopWindowManagerHandler` (6 ids / 6 hooks)

The Vista+ composition messages. Constants live at `messages.h:273`–`278`
(`wmDWMCompositionChanged 0x031E` … `wmDWMSendIconicLivePreviewBitmap 0x0326`,
with `0x0322` and `0x0324`/`0x0325` absent). Hooks at `dwm.h:40`–`45`; cases at
`dwm.cpp:38` `:43` `:49` `:54` `:60` `:68`. All six use the **same** result
pattern, `lResult = hook ? 0 : window.DefWindowProcT(msg, wParam, lParam)` at
`:40` `:46` `:51` `:57` `:65` `:70`, and all six defaults return `false`
(`:81` `:86` `:91` `:96` `:101` `:123`) — so out of the box every DWM message is
forwarded to the Win32 shell. This is the only leaf in the subtree that
uniformly defers.

Decodes worth recording: `OnNonClientRenderingChanged` and
`OnWindowMaximizedChange` collapse `wParam` to a `bool` (`:45`, `:56`);
`OnSendIconicThumbnail` splits `lParam` with **`xMax = HiWord` (`:62`) and
`yMax = LoWord` (`:63`)** — the width is the high half, matching the Win32
`WM_DWMSENDICONICTHUMBNAIL` contract; `OnSendIconicLivePreviewBitmap` carries
two commented-out blocks at `dwm.cpp:108`–`115` (a `GetClientRect` measure) and
`:116`–`122` (`CreateDIB` → `DwmSetIconicLivePreviewBitmap` with
`DWM_SIT_DISPLAYFRAME` → `DeleteObject`) as the sketch of what a real
peek-preview would do.

### Subtree totals and hook/id divergence

| Handler | Hooks | Claimed ids |
|---|---:|---:|
| `NonClientHandler` | 3 | 3 |
| `NonClientRenderingHandler` | 3 | 3 |
| `RenderingHandler` | 21 | 21 |
| `IconizedHandler` | 2 | 4 |
| `FontHandler` | 2 | 2 |
| `CursorHandler` | 1 | 1 |
| `DesktopWindowManagerHandler` | 6 | 6 |
| **Total** | **38** | **40** |

## 50c — installation, build and include facts

### Which of these seven handlers ever run

`RenderedWindowHandler() = default` (`rendered.h:43`) and
`HandlerFactory<T>::Create()` returning `new_ref<T>()` (`handlerfactory.h:58`–`61`)
mean the default tree has **all seven rendered slots null** — the library's own
code installs only `m_lifetime` (`toplevel.cpp:41`–`44`). Repo-wide writers of
the seven slots:

| Slot | Writers | Built? |
|---|---|---|
| `m_nonClient` | `mainwindow.cpp:327` | yes |
| `m_ncRendering` | `mainwindow.cpp:325` | yes |
| `m_rendering` | `mainwindow.cpp:329`; `editbox.cpp:59` | mainwindow yes, editbox no |
| `m_iconized` | — | never |
| `m_font` | — | never |
| `m_cursor` | — | never |
| `m_dwm` | — | never |

So `IconizedHandler`, `FontHandler`, `CursorHandler` and `DesktopWindowManagerHandler`
have **no holder anywhere in the repo**: 13 of the 40 ids in this subtree are
unreachable code, and 4 of the 7 pairs never execute. Grep hits that look like
slot writes but are not: `retropad/controller.cpp:234`/`:235` assign
`m_rendering`/`m_ncRendering` on a `RetroPadTopLevelWindowController`
(a *controller* class, not a handler), and
`retropad/mainwindowinstancecontroller.cpp:64` writes `m_ncRendering` on
`MainWindowNonClientRenderController`; that example is disabled at
`osspecific/windows/CMakeLists.txt:8`, as is `editbox` at `:3`. Only
`mainwindow` is live (`:4`).

### The one real override set — `mainwindow.cpp`

`CustomHandlerFactory` (`mainwindow.cpp:319`–`332`) hangs all three
overrideable handlers on one `TopLevelWindowHandler`: `CustomNonClientRenderingHandler`
(`:31`–`141`, an `ncrender` subclass), `CustomNonClientHandler` (`:21`–`29`,
an `nc` subclass), `CustomRenderingHandler` (`:143`–`317`, a `rendering`
subclass). Part 90 covers that example in full; the relevant points for this
subtree are:

- `CustomNonClientHandler` overrides **only** `OnNCActivate`, returning `false`
  (`mainwindow.cpp:26`) — and per `nc.cpp:49` that verdict is discarded anyway.
- `CustomNonClientRenderingHandler::OnNCCalcSize` (`mainwindow.cpp:49`–`69`)
  returns `0x0400` = **`WVR_VALIDRECTS`** and fills `rgrc[0]` (new client),
  `rgrc[1]` (valid dst) and `rgrc[2]` (valid src) by centring — the one place in
  the repo that uses the `NCCalcSizeParams` payload the struct exists for.
- `OnNCHitTest` (`:94`–`140`) returns `HT*` codes against a hardcoded 31 px
  caption band and 5 px corner squares, via
  `controller->GetRect(r)` (`:103`).
- `CustomRenderingHandler` carries the app's whole renderer: a DIB-section
  (`CreateDIB` `:251`–`288`), a `WGLContext` (`CreateRenderingContext`
  `:290`–`315`), an OpenGL scene (`InitScene` `:197`–`210`, `ResizeScene`
  `:212`–`221`, `RenderScene` `:223`–`240`) and the `Blit` blit-to-hDC
  (`:243`–`249`); `OnSize` (`:152`–`165`) tears down and rebuilds that
  pipeline on every `wmSize`, and `OnPaint` (`:167`–`183`) renders OpenGL into
  the DIB then blits and writes "Hello, Windows!" with
  `GetTextExtentPoint32T`/`TextOutT`.

### Build facts

- Root `view/handlers/rendered.cpp` compiles at `gui/CMakeLists.txt:63`;
  the seven subtree `.cpp` files compile contiguously at **`:83`–`89`**, in the
  order `cursor` `iconized` `nc` `ncrender` `dwm` `rendering` `font` —
  different from both the header order (`rendered.h:29`–`35`) and the chain
  order (`rendered.cpp:36`–`42`).
- All 7 pairs compile: 7 headers (none header-only) + 7 `.cpp` (7 compiled).
  Unlike 50a and 50b, **no file in this subtree is absent from `add_library`**.
- Every `.cpp` includes its own header at `:27` and `../../messages.h` at `:28`
  (`cursor.cpp` `dwm.cpp` `font.cpp` `iconized.cpp` `nc.cpp` `ncrender.cpp`
  `rendering.cpp`).
- `rendering.cpp:29`/`:30` re-include `../../../gdiobjects/devicecontext.h` and
  `../../../gdiobjects/icon.h` although `rendering.h:29`/`:30` already included
  both — harmless duplication, but it means the `.cpp` does not trust its own
  header.
- Only `rendered.h` includes the seven headers (`:29`–`35`); no other file in
  `gui/` includes any of them. `mainwindow.cpp:4`–`13` includes
  `view/handlers/toplevel.h` only, so the three custom classes get these types
  transitively through `toplevel.h` → `interactive.h` → `rendered.h`.
- The widget layer (`gui/windows/*`) includes `handlers/base.h` at its `:29`
  and never reaches the rendered level, so no widget ever installs a rendered
  slot.
- GDI object dependencies point at part 60: `gdiobjects/font.h` (`font.h:29`),
  `gdiobjects/icon.h` (`iconized.h:29`, `rendering.h:29`),
  `gdiobjects/devicecontext.h` (`rendering.h:30`) — and
  `devicecontext.h:34` in turn includes `font.h`, which is why `Font` and `Icon`
  are available without extra includes in the handler layer.

## 50c — gotchas

1. **`nc.cpp:49` discards `OnNCActivate`'s verdict.** The statement is a bare
   ternary; only `DefWindowProcT`'s return reaches `lResult` (`:50`). Any
   override that tries to veto NC activation is silently ignored.
2. **Two of this leaf's ids are claimed higher up.** `TopLevelWindowHandler`
   decodes `wmQueryNewPalette` (`toplevel.cpp:119`–`124`) and `wmPaletteChanged`
   (`:125`–`131`) and returns `true` for both, so `rendering.cpp:166` and
   `:180` are dead in any tree rooted at a top-level handler. The top-level
   versions differ: `DefWindowProcT` runs *after* the hook and its return value
   is ignored (`:122`), and *before* it (`:127`), and `lResult` is set from a
   `bool` (`:121`).
3. **Four handlers have no holder** (`m_iconized` `m_font` `m_cursor` `m_dwm`
   are never assigned repo-wide), so `wmQueryOpen` `wmPaintIcon`
   `wmIconEraseBkgnd` `wmQueryDragIcon` `wmSetFont` `wmGetFont` `wmSetCursor`
   and the six DWM ids are unreachable in practice — 13 ids.
4. **`wmSetRedraw` tests `wParam == 1`** (`rendering.cpp:50`), not truthiness.
   A caller passing `-1` or a Win32 `TRUE` of any other shape gets `bRedraw == false`.
5. **Polarity is inconsistent inside `rendering.cpp`.** The paint-family
   cases treat `0` as "handled" (`lResult = 0`), while `wmSizing` (`:153`),
   `wmMoving` (`:162`) and `wmQueryNewPalette` (`:168`) treat `1` as "handled".
   Overriders must know which convention each id uses.
6. **Nineteen of the 21 `rendering` case bodies never call `DefWindowProcT`** —
   only the `wmSize` (`:206`) and `wmMove` (`:212`) hooks reach the shell, and
   then only through their default bodies at `rendering.cpp:224`/`:229`. An
   unhooked `wmPaint` `wmShowWindow` `wmEraseBackground` `wmSysColorChange`
   `wmSetRedraw` `wmGetMinMaxInfo` `wmWindowPosChanging` `wmWindowPosChanged`
   `wmSyncPaint` `wmPaletteIsChanging` `wmPrint` `wmPrintClient` therefore leave
   `lResult` at whatever the caller initialised it to — the value is not even
   zeroed. `dwm` (all six), `wmNCPaint`, the `!indicateValidClientArea` branch of
   `wmNCCalcSize` and `nc`'s `wmNCActivate` are the only other rendered cases
   that consult the Win32 shell.
7. **`wmQueryOpen` and `wmSyncPaint` come from the `#ifndef _WIN32_WCE` block**
   in `messages.h` (`:58`–`62` and `:121`–`123`), so `iconized.cpp:38` and
   `rendering.cpp:140` would not compile for WinCE even though the rest of the
   subtree would.
8. **`rendering.cpp:282` names a parameter after its type** —
   `Icon& Icon` in `OnSetIcon` — shadowing `gdiobjects/icon.h`'s class name
   inside that body.
9. **Three payload structs are hand mirrors with no SDK counterpart header**:
   `NCCalcSizeParams` (`ncrender.h:33`–`37`), `MinMaxInfo` (`rendering.h:34`–`41`)
   and `WindowPos` (`rendering.h:43`–`52`). Nothing in the library ever
   *constructs* a `WindowPos` or `MinMaxInfo` — every use is a
   `reinterpret_cast` of the shell's `lParam` (`rendering.cpp:85` `:94` `:103`
   `ncrender.h:36`), so the structs must stay layout-identical to the SDK
   records or the casts read garbage.
10. **`cursor.h` is the one header that cannot see QOR's cursor type** — it
    includes only `parts/part.h`, so `WM_SETCURSOR` deals with a raw
    `Window wndOver` handle and the hit-test/message-id pair cracked from
    `lParam`; anything wanting a real cursor object has to come from elsewhere.
11. **`wmNCHitTest`'s default re-dispatches `wmNCHitTest` through
    `DefWindowProcT`** (`ncrender.cpp:119`) and its result is then assigned
    straight to `lResult` (`:57`) — so the built-in behaviour is
    "ask the shell, report its HT code", the only sane default in this subtree.
12. **`OnGetFont`/`OnQueryDragIcon`/`OnGetIcon` return `PrimitiveHandle`/`long`
    but the cases widen into `long long& lResult`** (`font.cpp:48`,
    `iconized.cpp:55`, `rendering.cpp:130`) via `reinterpret_cast`; the
    handles' low bits are the only meaningful part of the contract.

## Cross-references and next

- Part 50 root: the level chain and the `ProcessHook` walk; part 50a `parts/`
  for `CreateStruct` (`lifetime.h:32`–`46`) and `LifetimeHandler`; part 50b
  `interactive/` for the pointer/keyboard half of the same messages.
- Part 20 for `HandlerFactory` / `WndProc`; part 30 for `WindowController`
  (`GetRect` `GetClientRect` `GetADC` used by 50c's decodes and by
  `mainwindow`); part 60 for `Font` `Icon` `DeviceContext` `Bitmap`
  `DIBSection` `WGLContext`; part 70 for `PaintSession` (`mainwindow.cpp:172`);
  part 80 for `view/messages.h` constants (`sc*`, `wm*`, DWM at `:273`–`278`).
- Part 90 for the `mainwindow` example, which is the only built consumer of this
  subtree.

**Part 50 is now fully detailed: root (20) + `parts/` (18) + `interactive/` (22)
+ `rendered/` (14) = 74 files.** The index's next `summary` part is
**60 — gdiobjects** (`gui/gdiobjects/`), the GDI object wrappers these
handlers' parameters are typed on.





