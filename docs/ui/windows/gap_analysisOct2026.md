# winqor_gui — gap analysis: documented purpose vs. current state

**Date:** 27 Oct 2026
**Subject:** `winqor_gui` = `src/platform/os/windows/ui/gui` (212 `.h`/`.cpp` files, 7 areas)
**Basis:** the pages in `docs/ui/windows` (parts 10–90 all `detailed`; 00 by design a
summary), `src/platform/os/windows/ui/gui/purpose.md`, `src/platform/os/windows/ui/gui/controllers/purpose.md`,
and `memory-bank/{activeContext,progress}.md`. No source translation unit was read
to re-derive a claim; every claim is carried from a cited documentation page.

**Verified directly today (build files only, not TUs):**

- `gui/CMakeLists.txt` `add_library(${target} SHARED …)` lists **99** `.cpp` entries
  (`:3`–`102`); the disk holds **103** `.cpp` files. The four with no entry —
  `windowstation.cpp`, `controllers/i_m/mdiclientcontroller.cpp`,
  `view/handlers/mdiclient.cpp`, `windows/a_h/header.cpp` — match the list on
  [00-overview.md](00-overview.md) §Build facts.
- `examples/experimental/osspecific/windows/CMakeLists.txt` enables
  `add_subdirectory(mainwindow)` at `:4` only; `button` `dialogs` `editbox`
  `menu` `minimal` `console` `retropad` (`:1`–`:3`, `:5`–`:8`) are all commented out.
- `test/CMakeLists.txt` (`add_executable(QORTest …)`) names **no** GUI source, and
  `test/ui/` contains only `terminal/`. The library has no test target.

**How to read this:** every line below is either an *observation* recorded on a
part page with a file:line citation, or a `Windows API intent` / `code comment intent`
statement copied from that page. Where the two disagree the page's detail section wins.
Gaps are stated as **what is missing**, not as what the code "should look like".

---

## 1. The documented purpose (verbatim)

`src/platform/os/windows/ui/gui/purpose.md`:

| Line | Statement |
|---|---|
| `:1` `:3` | "winqor_gui provides a Windows native GUI class library … implements the portable GUI concepts of the QOR, specifically for the Windows Operating System" |
| `:5` | "It should provide factories, controllers and window classes for all built in Window types and Windows Common Controls. Including for OpenGL/EGL windows" |
| `:7` | "It should provide fully customizable message handling that covers all documented & undocumented Window messages. It should support, hooks, Owner Draw, Custom Draw, Themes and Off Screen Rendering with Composition to an Open GL target window …" |
| `:8` | "It should surface Windows Accessibility and IME features" |
| `:9` | "Have the capability to follow the users preference for dark/light theme" |
| `:11`–`:13` | one thread per `Window`; all messages and rendering on that thread with GDI objects bound to it; `PerThread` holds that state and owns a `MessageHandler` implementing the loop |
| `:15` | "Message handling is filtered through a customizable tree of objects derived from `BaseWindowPartHandler`. Each … either handles the message itself, handles it by passing it to its children, or gives up and its parent tries the next sibling." |

`controllers/purpose.md:1`–`:14` names the control controllers the area must own
(`WindowController` base + `Button` `ComboBox` `Edit` `ListBox` `MDIClient` `Menu`
`PopupMenu` `ScrollBar` `Static`), and `:15`–`:42` is a **TODO list of 21 control
types that have no controller**: `Animation` `ComboBoxEx` `DateTime` `Header`
`HotKey` `IPAddress` `ListView` `Menu` `MonthCalendar` `Pager` `ProgressBar`
`ReBar` `Ribbon` `RichEdit` `Slider` `SpinButton` `StatusBar` `Tab` `ToolBar`
`Tooltip` `TreeView` `WebBrowser`.

---

## 2. Scorecard — purpose clause vs. observed state

| Purpose clause | State today (cited) | Verdict |
|---|---|---|
| `:5` window classes for built-in types | 15 `windows/` pairs; 12 are `Window` subclasses whose only job is layout min/max defaults marked `//TEMP:` (part 40 `:40`–`:86`); `Dialog` `MessageBox` `Desktop` are not `Window` subclasses | **partial** |
| `:5` controllers for common controls | 13 compiled control controllers (`gui/CMakeLists.txt:22`–`33`, `:92`); **none is instantiated anywhere in the built app** (part 30 Level 4, `:829`) | **partial — unexercised** |
| `:5` factories | `WindowFactory` creates and registers classes; only `MainWindow` is pre-registered, and only per factory instance (`windowfactory.cpp:43`–`47`); **no `windowclass_of` specialisation exists anywhere** (part 20 answer 3; part 40 answer 2) | **partial** |
| `:5` OpenGL/EGL windows | `WGLContext` has 3 live calls (`wglcontext.cpp:37`–`53`) with the rest of the WGL surface commented (`wglcontext.h:39`–`57`); `OpenGLSession` is a fixed 14-call subset (`openglsession.h:59`–`72`) over `DynamicLibrary("opengl32")` (`openglsession.cpp:33`); **EGL does not exist** — `egl` and `gl` targets are commented out in `src/platform/os/windows/ui/CMakeLists.txt` (README `:73`) | **missing for EGL** |
| `:7` message coverage | ~256 id constants in `view/messages.h` (part 80 `:193`), no `.cpp`; 11 top-level ids answer `true` from empty hooks (`toplevel.cpp:260` `:269` `:278`); 13 of 40 rendered ids unreachable; `scArrange 0xF110` (`messages.h:313`) has no `OnSysCommand` case; `0x0017` has no named constant (`lifetime.cpp:82`) | **partial** |
| `:7` hooks | `Hook` is `class Hook {};` (`hook.h:31`–`33`), `hook.cpp:34`–`35` empty; `HookHandler` (`part.cpp:37`–`49`) has no caller; `m_nextHookHandler` (`part.h:44`) and `m_nextHookController` (`base.cpp:32`) have **no writer repo-wide** — the chain is walked four times per message (`base.cpp:43` `rendered.cpp:34` `interactive.cpp:34` `toplevel.cpp:64`) and is always empty | **missing** |
| `:7` Owner Draw | `PaintSession` (`paintsession.h:39`) is the one live `painting/` class and its only construction site is the example (`mainwindow.cpp:172`) | **partial — example-only** |
| `:7` Custom Draw | `Painter` (`windowpainter.h:42`) makes no Win32 call and its header claims three unimplemented features (`windowpainter.h:33`–`35`) | **missing** |
| `:7` Themes | `ThemeData` (`themedata.h:36`) is never constructed; 264 of its 354 header lines are commented-out `TMT_*` vocabulary and **no `TMT_*` name is defined anywhere**; the only theme call in the tree is `SetTheme(L" ", L" ")` (`mainwindow.cpp:368` → `windowcontroller.cpp:829`) | **partial** |
| `:7` Off-screen rendering composed to a GL target | the whole chain (DIB → WGL → GL → `BitBlt`) lives in the example (`mainwindow.cpp:251`–`315`, blit `:247`); **no library class implements it**; the metafile family carries no GDI call at all (part 60b) | **missing in the library** |
| `:8` Accessibility + IME | IME machinery is `IMEHandler` + `ClassStyle::SetIME` (`classstyle.h:86`) + `WindowController::SendIMEMessageEx` (`windowcontroller.h:282`) and is unreachable (part 50b); **no accessibility surface exists at all** | **missing** |
| `:9` dark/light preference | nothing reads the user's preference; `SetWindowThemeAttribute` (`uxtheme.h:112`) and `SetWindowThemeNonClientAttributes` (`:113`) are declared and never called | **missing** |
| `:11`–`:13` thread binding | `PerThread` is the thread singleton and owns `MessageHandler` (`perthread.h:102`, `perthread.cpp:38`–`41`); the DC registry it exists to serve is never populated (part 60a gotcha 1) | **partial** |
| `:15` customizable handler tree | the chain and the tri-state contract exist; the library installs exactly one part handler (`toplevel.cpp:41`–`44` `m_lifetime`); **0 of 10** `interactive.h:52`–`61` slots and **0 of 7** rendered slots have a library writer | **partial** |
| *(not in purpose.md)* test coverage | no unit tests; the `mainwindow` example is the de-facto test (README `:12`, part 90 `:8`–`:13`) | **missing** |

---

## 3. Feature gaps — what the library does not have

### 3.1 Handler-tree customisation (`purpose.md:15`) — the largest structural gap

- **Nothing the library installs.** `BaseWindowHandler()` (`base.cpp:32`–`34`)
  initialises only `m_nextHookController`; the eight part slots (`base.h:56`–`63`)
  are default-constructed and guarded by `IsNotNull()` (`base.cpp:44`–`51`).
  Repo-wide there are exactly **two** writes to a part slot: `toplevel.cpp:43`
  (`m_lifetime`) and `menu/main.cpp:56` — the latter in an example that is
  commented out of the build (part 50a Reachability table).
- **The interactive level is never lit.** Its ten slots (`interactive.h:52`–`61`)
  have exactly two writers repo-wide, both inside the disabled `retropad` example
  (part 50b §"how a handler would be installed"): `retropad/controller.cpp:23`
  (`m_focus`) and `retropad/mainwindowinstancecontroller.cpp:62` (`m_dragAndDrop`).
  That example cannot build three times over: it is commented out
  (`osspecific/windows/CMakeLists.txt:8`), it includes a `view/controllers/` tree
  that no longer exists (`ui/gui/view/` holds only `drawing` `handlers` `layout`),
  and two overrides return `void` against `bool` bases
  (`mainwindowinstancecontroller.h:20` vs `focus.h:47`; `:46` vs `draganddrop.h:40`).
- **The rendered level is populated only by hand, in one example.** `mainwindow.cpp:319`–`332`
  (`CustomHandlerFactory`) writes `m_ncRendering` `m_nonClient` `m_rendering`;
  `m_iconized` `m_font` `m_cursor` `m_dwm` have **no holder anywhere**, so
  13 of the subtree's 40 ids never run (part 50c).
- **The hook half of the contract is absent.** `Hook` (`hook.h:31`–`33`) is an
  empty placeholder, `hook.cpp:34`–`35` is an empty namespace body, `HookHandler`
  (`part.cpp:37`–`49`) has no caller, and `Window::m_messageProc` (`window.h:69`–`71`)
  has exactly one occurrence in the repo — its own declaration.
- **Two handler classes are dead by construction**: `DialogHandler` derives from
  nothing (`dialoghandler.h:44`) and its `HandleMessage` returns `false`
  (`:42`–`45`); `MDIClientHandler` returns `false` on all three cases
  (`mdiclient.cpp:36`–`47`) and its TU is not compiled at all.
- **`BaseWindowPartHandler::ProcessMessage` (`part.cpp:51`–`54`) is dead** — no
  object of that exact type is ever constructed (part 50a gotcha 1 lead-in).

### 3.2 Common controls and window-class registration (`purpose.md:5`)

- 21 control types on the `controllers/purpose.md:15`–`:42` TODO list have no
  controller at all; 13 exist and are compiled but **no window ever installs one**
  (part 30 Level 4; part 60b records the same dead-controller pattern for
  `StaticController`, whose ctors are at `staticcontroller.cpp:40`/`:41` yet
  nothing constructs it).
- `CreateButton` `CreateEdit` `CreateListBox` `CreateComboBox`
  (`windowfactory.cpp:171` `:192` `:214` `:236`) are the **only** `AddItem` sites in
  the tree and the built example calls none of them (part 80 issue 18).
- Those four creators skip `SetHandler` entirely (part 20 gotcha 3), so every
  message for a common control falls through to `DefWindowProc`.
- `Animation` `ScrollBar` `Static` `MDIClient` `ComboBoxEx` `DateTimePicker`
  `Header` `FlatScrollBar` `Desktop` have a QOR type but **no construction path**
  (part 40 `:306`–`:310`).
- `DialogController` is declared and never defined (`dialog.h:36`);
  `Desktop` is `class Desktop {};` with no ctors and no includes (`desktop.h:31`).
- `windows/a_h/header.cpp` installs a `WindowLayoutItem` at `:41` `:48` but is
  absent from `add_library` — a layout leaf defined in a TU that never links
  (part 80 issue 17; part 40 gotcha 4).

### 3.3 Painting, theming and DWM (`purpose.md:7` `:9`)

- `Painter` (Custom Draw) and `BufferedPainting` are compiled, linked, never
  constructed (part 70 gotcha 1); `Painter` is an empty class whose header
  advertises three features (`windowpainter.h:33`–`35`).
- `ThemeData` is never constructed; the theme-property vocabulary is comment-only
  (264 of 354 header lines) and no `TMT_*` name is defined anywhere.
- `DTTOpts` / `DTTCallbackProc` sit **inside** the `ThemeData` class body
  (`themedata.h:306`–`333`), so the 7-parameter `DrawTextEx` (`:344`) is
  unnameable from application code; only the `nullptr`-options overload (`:343`)
  is reachable.
- The dark/light preference path does not exist; `SetWindowThemeAttribute`
  and `SetWindowThemeNonClientAttributes` are declared and never called.
- The DWM attribute block in the controller is comment-only: `ExtendFrameIntoClientArea`
  `GetWindowAttribute` `InvalidateIconicBitmaps` `RegisterThumbnail`
  `SetIconicLivePreviewBitmap` `SetIconicThumbnail` `SetWindowAttribute`
  `TransitionOwnedWindow` (`windowcontroller.h:299`–`306`); only
  `DwmEnableBlurBehindWindow` is live (`:297` → `:818`–`825`).
- `FindWindowExT` (`windowcontroller.h:171`–`173`), `EnumPropsExT` (`:265`–`267`)
  and `PrintWindow` (`:291`) are commented out.

### 3.4 Off-screen composition to a GL target (`purpose.md:7`)

- The library has **no off-screen composition facility**. All four steps are
  members of the example's `CustomRenderingHandler : public RenderingHandler`
  (`mainwindow.cpp:143`–`317`): `CreateDIB` (`:251`–`288`), `CreateRenderingContext`
  (`:290`–`315`), `RenderScene` (`:223`–`240`) and `Blit` (`:243`–`249`).
- The metafile family was the candidate for this role and is not: it wraps **no
  GDI call at all**, and the replay side — `PlayEnhMetaFile` (`gdi32.h:284`),
  `PlayMetaFile` (`:286`), `PlayEnhMetaFileRecord`/`PlayMetaFileRecord`
  (`:285`/`:287`), `EnumEnhMetaFile`/`EnumMetaFile` (`:274`/`:275`) — has no
  caller anywhere (part 60b). `MetaFile` cannot even be constructed: it declares
  no ctor (`metafile.h:34`–`37`), and `GDIObject` has no default ctor
  (`gdiobject.h:56`–`58`).
- The live off-screen route is `DeviceContext::CreateForDIB()` + `DIBSection` +
  `WGLContext`, and `DIBSection`/`SysBrush` are **never instantiated** in the
  library (part 60c; the only live `CreateDIBSection` in the tree is
  `ui/gl/windowimpl.cpp:152`, which bypasses the wrapper).
- `wglcontext.h:39`–`57` comments out most of the WGL surface
  (`wglCopyContext` `wglShareLists` `wglUseFontBitmapsT`, layer palette calls);
  live: `CreateLayerContext` `DeleteContext` `GetHandle` (`wglcontext.cpp:37`–`53`).
- `OpenGLSession` is a fixed 14-function immediate-mode subset
  (`openglsession.h:59`–`72`); there is no VBO/shader path and no EGL at all.
- The WGL pixel-format bridge exists (`devicecontext.cpp:874`–`897`, the five
  `WGL::` forwards) but is reached only from the example.

### 3.5 Accessibility and IME (`purpose.md:8`)

- **Accessibility has no counterpart in the library at all** — the only
  IME-adjacent machinery is `IMEHandler` (`view/handlers/interactive/ime.h:32`),
  `ClassStyle::SetIME` (`classstyle.h:86`, bit `0x00010000`) and
  `WindowController::SendIMEMessageEx` (`windowcontroller.h:282`) (00-overview `:227`).
- `IMEHandler` is unreachable: no writer of `interactive.h`'s IME slot exists in
  the built tree, and even if installed it changes nothing observable — 8 of its
  11 hooks re-post the same id to `DefWindowProcT` (`ime.cpp:119` `:124` `:129`
  `:134` `:139` `:148` `:153` `:168`) while `ProcessMessage` returns `true`
  regardless, and two of them drop the payload (`OnStartComposition`/`OnEndComposition`
  forward `0, 0` at `:119` `:124`).
- `ime.cpp:79`–`86` is a commented-out `WM_IME_CHAR` decode — repeat count, scan
  code and the DBCS extended/context/previous/transition bits are all commented;
  the live code just calls `OnChar` (`:87`) then forces `lResult = 0` (`:88`).
- Only the three `long long` hooks (`OnSetContext` `OnNotify` `OnRequest`,
  `ime.cpp:58` `:63` `:109`) can report a result; the six `void` hooks cannot.
- Two IME-adjacent ids are claimed nowhere: `wmUniChar 0x0109` (`messages.h:150`)
  and `wmIMEControl 0x0283` (`messages.h:237`).

### 3.6 The layout engine (`view/layout/`) — present, compiled, never executed

- The engine is dead in the running app: the three `factory.Create` calls
  (`mainwindow.cpp:347` `:367` `:381`) all land at `windowfactory.cpp:145`, giving
  every window a `VertcialLayout` with zero items; the four `AddItem` sites and the
  four `Place` call sites (`layout.cpp:101` `:115` `:190` `:204`) are the engine's
  entire call surface and none executes (part 80 `:164`–`:166`).
- No handler anywhere consults a layout: a layout does **not** partition a window
  into regions; only `WindowLayoutItem::Place` acts, by moving child HWNDs through
  `WindowController::SetPos` (`windowcontroller.h:211`) (part 80 `:170`).
- The example documents layout-driven hit testing in a comment
  (`mainwindow.cpp:97`–`98`) and then hard-codes `+31`/`+5` offsets instead
  (`:102`–`139`) — the stated goal, not the behaviour.
- `VBox` is constructed exactly once in the tree (`windows/i_m/mdiclient.cpp:41`
  `:48`), and `VBox`'s only statement is `m_maxSize = Size{16384, 16384};`
  (`vbox.cpp:34`).

### 3.7 Verification and testing — the standing deficit

- **Zero unit tests.** `test/CMakeLists.txt`'s `add_executable(QORTest …)` names no
  GUI source; `test/ui/` holds only `terminal/` (`primary.cpp` `tui.cpp` `test_main.cpp`).
  README `:12`–`:16` and part 90 `:8`–`:13` state this explicitly: the example
  "doubles as the library's de-facto test".
- **No observed runtime behaviour.** Part 90's §Runtime observation log has an
  empty table (`:177`–`:179`, "no observations recorded") and five named checks
  never confirmed (`:181`–`:194`): the GL triangle + "Hello, Windows!", resize
  re-running `CreateDIB`/`CreateRenderingContext`, edge dragging via
  `OnNCHitTest`, the `ToolWindow` at 0,0 500×100, and the exit code from
  `MessageLoop()` (`mainwindow.cpp:385`).
- **Seven of eight sibling examples are disabled**, so whole areas are exercised by
  nobody: `button` `dialogs` `editbox` `menu` `minimal` `console` `retropad`
  (`osspecific/windows/CMakeLists.txt:1`–`3`, `:5`–`:8`).
- **Four TUs compile into nothing** (see the verified build facts above): any
  consumer of `WindowStation`, `MDIClientController`, `MDIClientHandler` or `Header`
  is a link error.
- **Documentation-side gap:** 00-overview is the only page still `summary`, and it
  contains a duplicated paragraph — "those three structural rules …" is stated
  twice (`00-overview.md:21`–`32` and again `:34`–`45`).

---

## 4. Known errors and deficits (inventory by area)

These are defects already recorded on the pages; they are listed here so that the
gap list is not read as "features only".

### 4.1 Core / root (part 10)

| Item | Where | What |
|---|---|---|
| Handle never closed | `window.h` `SetHandle`, ctors call `DontClose()`; dtor defaulted | `Window` leaks no close step at all |
| Window→controller link is a raw pointer | `window.h:84` | `GetController()` can legitimately be null; the example guards at `:73` `:99` `:169` |
| `m_messageProc` not assignable | `window.h:69`–`71` | lambda-typed member, one occurrence repo-wide |
| `GetWindowCreationInProgress()` consumes | `perthread.cpp:51` `m_inProgress.Dispose()` | a second read returns null |
| WGL current-context tracking half-used | `wglcontext.h:39`–`57` commented | see 3.4 |

### 4.2 Factories (part 20)

| Item | Where | What |
|---|---|---|
| Unregistered class → null deref | `windowfactory.cpp:149` | `Create` with an unregistered name has no diagnostic |
| Registration leak on re-register | `windowfactory.cpp:110` | raw `new` into `windowClassData::registration` |
| Common controls get no handler | `CreateButton`/`CreateEdit`/`CreateListBox`/`CreateComboBox` | messages fall to `DefWindowProc` |
| `MenuBuilder::Build` leaks and under-terminates | `menubuilder.cpp:74`, `:85`–`90` | buffer leak + unterminated item text |
| `DialogBuilder::Build()` has no definition | part 20 gotcha 5 | half-implemented builder |
| `Brush(COLOR_WINDOW)` inside `AddWindowClass` | `windowfactory.cpp:91` (`:78`–`98`) | a colour/stock selector becomes the **handle**; it is stored as `WNDCLASSEX::hbrBackground` (`windowclassregistration.cpp:46`) and `DeleteObject`-ed in the same statement (`gdiobject.cpp:49`–`52`) |

### 4.3 Controllers (part 30)

| Item | Where | What |
|---|---|---|
| Every method derefs `m_window` | `WINDOW_HANDLE`; default ctor leaves it empty (`windowcontroller.h:140`–`143`) | a default-constructed controller is unsafe from the first call |
| `GetClassName` reserves but never sizes | `windowcontroller.cpp:730`–`736` | text Windows wrote is unreadable; contrast `GetAltTabInfo` `:173`–`177` |
| `GetText` capped at 1023 chars | `windowcontroller.cpp:260`–`266` | truncation detectable only via `GetTextLengthT` (`:268`–`271`) |
| Region refs unguarded | `GetRgn` `SetRgn` `InvalidateRgn` `ValidateRgn` `GetUpdateRgn` `GetADC` (`:562`–`634`) | only `Redraw` (`:589`) guards the null case |
| `Destroy()` leaves the controller live | `windowcontroller.cpp:844`–`849` | later calls re-deref a disposed ref |
| HRESULTs discarded | `windowcontroller.cpp:818`–`825` `:827`–`834` | both carry the author's `//TODO: Process the HRESULT into an error` |
| Thread spelling | 12 fresh `new_ref<PerThread>()` against 10 `m_perThread` uses | same singleton while messages stay on one thread |
| `GetId()` returns 0 in the base | part 30 gotcha 9 | a control is unidentified without an override |
| `MDIClientController` broken includes | `mdiclientcontroller.h:28` `"windowcontroller.h"`, `.cpp:28` `"../perthread.h"` | resolves to paths that do not exist; part 30 records "leave as is" |

### 4.4 Window types (part 40)

| Item | Where | What |
|---|---|---|
| `Dialog::GetHandle()` never returns the dialog's handle | `dialog.cpp:79` assigns only `m_result` | `m_handle` (`dialog.h:143`) left default |
| `m_result` has no accessor | `dialog.h:147` (private) | the `DialogBoxIndirectParamT` outcome cannot be read |
| `WriteString` hardcodes a 50-wide destination | `dialog.cpp:84` | overflow hazard |
| `button.h` `N*` constants have no out-of-line definitions | `button.h:69`–`78` | used by reference they fail to link |
| Vocabulary value collisions | `messagebox.h:60` `:64` (`MBTOPMOST` == `MBSERVICE_NOTIFICATION_NT3X`); `scrollbar.h:44`–`47` (`DisableLeft`==`DisableUp`, `DisableRight`==`DisableDown`) | |
| Commented placeholder vocabularies | `combobox.h:36` `:39`, `animation.h:38`, `mdiclient.h:40`; none at all in `flatscrollbar.h` `header.h` `desktop.h` | |
| `Static` omits `virtual ~Static()` | `static.h:81`–`82` | only `Window` subclass in the part without it |
| Typos in identifiers/comments | `scrollbar.h:49` `DiableRightDown`; `static.h:71` "Static Control Mesages" | |

### 4.5 Handlers (part 50)

| Item | Where | What |
|---|---|---|
| `DialogHandler` derives from nothing | `dialoghandler.h:44` | no base, no part refs; `HandleMessage` returns `false` (`:42`–`45`); nothing constructs it |
| `MDIClientHandler` all-false | `mdiclient.cpp:36`–`47` `:49` | every path identical; TU not in `add_library` |
| `Hook` empty placeholder | `hook.h:31`–`33` | see 3.1 |
| `m_messageProc` dead | `window.h:69`–`71` | see 4.1 |
| Wrong include-guard close comments | `interactive.h:66` `mdiclient.h:49` `abstractwindowhandler.h:33` | behaviour unaffected |
| `OnSysCommand` falls through `Default:` | `toplevel.cpp:257` | 17 of 18 codes reach a `return false` stub; `scArrange` has no case |
| Top-level hooks that answer `true` from empty bodies | `toplevel.cpp:260` `:269` `:278` | the message is claimed and nothing happens |
| `OnActivateDefer` double-answers | `toplevel.cpp:84`–`86` | `wmDeferredClipDraw` → `OnNCPaint` (`rendered.cpp:104`–`106`) → `DefWindowProcT(wmNCPaint)` |
| `OnThemeChanged` ignores the `THEMECHANGE` payload and re-posts `wmThemeChanged` | `toplevel.cpp:150` | possible loop |
| `OnSetConsoleInfo` re-posts its own id to `DefWindowProcT` | `toplevel.cpp:237` | `wmSetConsoleInfo = wmUser + 201` (`messages.h:284`) |
| `OnParentNotify` default arm re-posts `wmParentNotify` | `parent.cpp:121` | potential recursion |
| `OnKeyUpdate` posts `OnDefocus` but not `OnFocus` | `focus.cpp:64`–`66` | the focus hook has no live caller repo-wide |
| `OnSetFocus`/`OnClearFocus` claim before the payload null check | `focus.cpp:46` `:51` | |
| `OnTextLayerMax` answers with a `short` | `ncpointer.cpp:72` | `short lMax = 255` (`:70`) |
| `OnChildDefocus`/`OnChildRealize` re-post `wmParentNotify` | `parent.cpp:113` `:118` | |
| `m_winCtrlParent` never taken | `parent.h:56`–`57`; `winctrlparent.h:33`–`44` | the `DrawItemStruct` it feeds has no counterpart in `common/structures.h` |
| `OnUnimplementedMessage` names three ids and forwards a fourth | `nc.cpp:111`–`118` | |
| `OnDefocus` calls `DefWindowProc` not `DefWindowProcT` | `nc.cpp:75` | `lParam` passed as `long` not `long long` |
| `OnFontMessage` re-posts `wmCreateFontBitmap` into `DefWindowProcT` | `font.cpp:51` | |
| `OnDwmComposeSurface` re-posts `wmDwmComposition` | `dwm.cpp:52` | |
| Two handler classes declare local payload structs | `help.h:32`–`40` `winctrlparent.h:33`–`44` | two definitions to keep in step |

### 4.6 GDI objects (part 60)

| Item | Where | What |
|---|---|---|
| **The per-thread DC registry is dead** | `perthread.cpp:141`–`150` `AddDeviceContext`, `:152`–`161` `RemoveDeviceContext`, `:166`–`169` lookup, `:173` | no caller of either; the map is always empty and `DeviceContextFromHandle` fabricates a new wrapper per call — `purpose.md:11`–`13` is only half wired (60a gotcha 1) |
| `Release()` always passes a null window handle | `m_hWnd`'s only writers are the two `FromWindow` overloads, which have no callers | `User32::ReleaseDC(NULL, hdc)` on every release (60a gotcha 2) |
| Two owners of the close step | derived dtor `Release()`s, then the base dtor `GDI32::DeleteObject`s the same handle | `CreateForDIB`/`CreateCompatible` set owner=true / mustBeReleased=false, so the DC is closed by `DeleteObject` where the pairing is `DeleteDC` — wrapped at `devicecontext.cpp:395`–`398`, never called (60a gotcha 3) |
| `takeOwnership` default unreachable | `gdiobject.h:57` declares none; only `gdiobject.cpp:41` defaults it | 60a gotcha 4 |
| `LineDDA` forwards no device context | `devicecontext.cpp:543`–`546` six args; `api_layer/gdi/gdi32.h:191` has no HDC slot | the x coordinate lands where an HDC belongs (60a gotcha 5) |
| `UnrealizeObject` ignores the DC | `devicecontext.cpp:222`–`226` | author's own `//TODO: Move this to GDIObject` at `:222` (60a gotcha 6) |
| Naming slips carried into the surface | `GetworldTransform` (`devicecontext.h:296`, `.cpp:313`); `FontEumProc` (`devicecontext.h:235`); `memdevicecontext.h` four unused includes | 60a gotcha 7 |
| Two parallel ownership models in one directory | `Cursor` (`cursor.h:46`) and `Icon` (`icon.h:61`) are **not** `GDIObject` subclasses — hand-rolled `m_handle` + `m_owning`, destroyed through `User32::DestroyCursor`/`DestroyIcon` (`cursor.cpp:80`, `icon.cpp:83`) | `GDIObjectType` has no enumerator for either (60a gotcha 8, settled in 60c) |
| The metafile mirror is complete and unused | `gdi32.h:265`–`291` 26 entry points; `api_layer/gdi/metafile.cpp` 26 wrappers | not one is called from any GUI TU (60b) |
| `MetaFile` cannot be constructed | `metafile.h:34`–`37` empty class body | `OMetaFile` (`gdiobject.h:44`) is used zero times; `metafile.cpp:33`–`36` defines nothing yet is compiled at `gui/CMakeLists.txt:52` (60b) |
| `EnhancedMetaFile` carries a dead second handle field | `enhmetafile.h:42` `m_handle`, never initialised | the ctor (`enhmetafile.cpp:35`) delegates to `GDIObject(h,OEnhMetaFile,own)` (60b) |
| Default-constructed DC objects are destructor-poisoned | `MetaDeviceContext()`/`EnhancedMetaDeviceContext()` get `m_handle(0)` with `m_owner(true)` (`gdiobject.cpp:37`) | `IsInvalid()` tests `(void*)(size_t)-1` (`constants.h:8`), so `~GDIObject()` runs `GDI32::DeleteObject((HGDIOBJ)0)` (60b gotcha 4) |
| One close step for four handle kinds | `~GDIObject()` always calls `DeleteObject` | the family needs `CloseMetaFile`/`CloseEnhMetaFile`/`DeleteMetaFile`/`DeleteEnhMetaFile` (`gdi32.h:267`/`:266`/`:273`/`:272`), declared and wrapped, never called (60b gotcha 5) |
| Ownership defaults fight the call sites | `enhmetafile.h:38` defaults `own = true` | both `staticcontroller.cpp:61`–`65` and `:85`–`89` pass `false` (60b gotcha 6) |
| `#undef GetEnhMetaFile` is order-sensitive | `staticcontroller.cpp:35`, after `staticcontroller.h` at `:27` | protects the definitions at `:61`/`:85`; cannot affect the declaration at `staticcontroller.h:49` (60b gotcha 7) |
| The api-layer mirror mistypes the close pair | `gdi32.h:266` `CloseEnhMetaFile(HDC)`, `:267` `CloseMetaFile(HDC)` | take an `HDC` where the API wants `HENHMETAFILE`/`HMETAFILE`; the neighbours (`:268`–`269`, `:272`–`273`) do carry the metafile types (60b gotcha 8) |

### 4.7 Painting (part 70)

| Item | Where | What |
|---|---|---|
| `Painter` / `BufferedPainting` never constructed | part 70 gotcha 1 | the only live `painting/` class is `PaintSession`, constructed only in the example (`mainwindow.cpp:172`) |
| `ThemedWindowPainter` has no `.cpp` | 70 files table | no methods are defined; `WindowPainter::SetTheme` (`:793`–`801`) is the only theme route |
| `PaintSession::StartWindowPaintSession` is dead | `paintsession.h:41` | no definition anywhere |
| `StartWindowPaintSession` calls `Create` not `CreateT` | `paintsession.cpp:90`–`93` | `PAINTSTRUCT*` bound to `void*` |
| `EndPaint` passes `PaintSession::EndPaint` (the function) as the `PAINTSTRUCT*` | `paintsession.cpp:94`–`96` | not the struct |
| `EndWindowPaintSession` calls `End` not `EndT` | `paintsession.cpp:97`–`100` | same shape |
| `DTTCallbackProc` cast to `LPARAM` | `themedata.cpp:1463` | the callback never runs |
| `DTTOpts` passed by value | `themedata.cpp:1471` | `DTTGetColorBrush`/`DTTGetTextColor` cannot write back |
| `SetTheme` ignores its own `pszElement` parameter | `windowpainter.cpp:795` | uses the member (`:793`) |
| `SetColourKey` stores `0` unconditionally | `windowpainter.cpp:804` | the parameter is discarded |
| `WindowPainter` methods make no GDI calls | `:642`–`675` | no `Set*Brush*`, `SetDrawFrame`, `SetDrawText` |
| The three features `Painter` advertises are unimplemented | `windowpainter.h:33`–`35` | |
| `WindowPaintSession::OnPaint` re-posts `wmPaint` into `DefWindowProcT` | `bufferedpainting.cpp:515` | |
| `WindowPaintSession`'s default ctor leaves `m_windowPainter` empty | `:42`–`44` | `GetWindowPainter()` (`:46`) is then empty |
| Header comment says "17 methods" | `windowpainter.h:27` | the class body holds 31 declarations |
| Mixed constness for one flat vocabulary | `themedata.h` — 19 `static const`, 2 `constexpr`, rest bare `const` | |
| Include-guard close comments | `bufferedpainting.h:32`, `windowpainter.h:33` | |

### 4.8 Geometry & layout (part 80)

| Item | Where | What |
|---|---|---|
| **`position` parameter never used** | `layout.cpp:31` `:120` | in both arrangement bodies the identifier appears only in the signature, so children are always placed from `(0,0)` of the container's own space and nested layouts land on top of each other |
| `VertcialLayout` | `layout.h:55` | misspelled class name in a live API |
| `placeVertically` ignores its `alignment` parameter | `layout.cpp:45`–`55`, `:134`–`146` | both `Alignment` enum arms are unreachable |
| `placeHorizontally` mislabels the axis | `layout.cpp:149`–`150` | the `Direction` test is `vertical_` (`:149`); the `Alignment` test at `:157` is `vertical_` again |
| Division by zero | `layout.cpp:87` `:176` | `spaceCount` is `flexibleCount + fixedCount + 1` and the loop adds 1 per flexible item, so an all-fixed arrangement divides by 0 |
| Min/max clamping never applied | `layout.cpp:101` `:115` `:190` `:204` | `SetPos`/`SetSize` get the raw computed value |
| `IsFlexible` is true for every item | `layout.cpp:45` | the default min/max (`layoutitem.cpp:34`) make `max > min` always true |
| `GetTotal` is unclamped | `layout.cpp:61` `:150` | sums raw `GetSize()` |
| `Rect` has no `operator==` | `rect.h:69`–`77` | `!=` at `:78` is `!(r == r)` — always true |
| `FromIntersection`/`FromSubtraction`/`FromUnion` default-construct a local `Rect` and pass its address to the API | `rect.cpp:52` `:85` `:92` | four uninitialised `long`s; collapsing `BOOL` to `bool` makes a failed intersection indistinguishable from an empty one |
| SDK drop-in member layout is unenforced | `point.h:31` `size.h:31` | no `static_assert`; `mainwindow.cpp:53`–`56` reads the SDK payload `rgrc[0..2]` as `Rect*` with no cast |
| Unused includes | `vbox.cpp:28` `rect.cpp:28` | |
| Trailing comma in `enum Direction` | `layout.h:44` | while `enum Alignment` at `:37` has none |
| Four redundant overrides identical to `LayoutItem` | `windowlayoutitem.h:41`–`44` | only `Place` differs |
| Eight unprefixed macros duplicating SDK `LOWORD`/`HIWORD`/`MAKELONG` | `messages.h:29`–`38` | 41 call sites use the QOR spellings, 11 use the SDK spellings — both live in one tree; `ime.cpp:80`–`82` vs `parent.cpp:49`–`107` collide inside one directory |
| `_WIN32_WINNT`/`_WIN32_WCE` gates `wmMouseLast` | `messages.h:200`–`208` | the header never includes the header defining those macros, so the `#else` arm (`0x0209`) is chosen silently |
| `wmMouseHWheel` and `wmMouseLast` both `0x020E` | `messages.h:198` `:201` | duplicate-case collision |
| `wm93`/`wm94` named by value | `messages.h:125` `:126` | |
| Six of seven `gwl*` field offsets never read | `messages.h:287`–`293` | only `gwlExStyle` is used (`view/handlers/parts/style.cpp:41` `:48`) |
| Stray null statement after the loop | `layout.cpp:80` `:169` | `while(...){ ... };` in both copies |
| Three include-guard styles in one part | `QOR_PP_H_WINDOWS_GUI_POINT` `…_VBOX` `…_WINDOWLAYOUTITEM` vs full `QOR_PP_H_WINDOWS_GUI_VIEW_MESSAGES` | |

---

## 5. Small specific tasks (each independently testable, one TU or one build file)

Ordered by how much each closes a purpose clause. Sizes are "one change site" unless
noted. Nothing here requires reading a new TU — each cites a line already recorded.

### 5.1 Make the library compile what it claims (build-file fixes, do first)

| # | Task | Site | Why |
|---|---|---|---|
| T01 | Add `windowstation.cpp` to `add_library` | `gui/CMakeLists.txt` after `:7` | its only symbols are in a TU that never links |
| T02 | Add `windows/a_h/header.cpp` to `add_library` | after `:16` | it installs a `WindowLayoutItem` (`:41` `:48`) that no linked TU provides |
| T03 | Add `controllers/i_m/mdiclientcontroller.cpp` | after `:31` | `MDIClientController` is in the controller purpose list and cannot be linked |
| T04 | Add `view/handlers/mdiclient.cpp` | after `:62` | `MDIClientHandler` otherwise has no definition |
| T05 | Fix `mdiclientcontroller.h:28` / `.cpp:28` includes to source-root-relative | part 30 gotcha | currently unresolvable local paths |

### 5.2 Make the handler tree installable from the library (not only by examples)

| # | Task | Site | Why |
|---|---|---|---|
| T06 | Give `BaseWindowHandler` a ctor that installs the seven part handlers it already declares | `base.cpp:32`–`34` | today only `m_lifetime` is ever installed (`toplevel.cpp:41`–`44`) |
| T07 | Add a library-side installer for the ten interactive slots, mirroring the existing `set_parts` | `interactive.h:52`–`61`, `interactive.cpp` | 0 of 10 slots writable from the library |
| T08 | Add a library-side installer for the seven rendered slots | `rendered.h:56`–`64`, `rendered.cpp` | 0 of 7; only `mainwindow.cpp:319`–`332` writes three of them |
| T09 | Implement `Hook` as the object that ties a handler to the factory's `WndProc` | `hook.h:31`–`33`, `hook.cpp:34` | the purpose's "hooks" clause has no type behind it |
| T10 | Make `Window::m_messageProc` (`window.h:69`–`71`) assignable and use it as the `WNDCLASSEX::lptrWndProc` source | `window.h` + `windowfactory.cpp:94` | the lambda-typed member has one occurrence repo-wide |
| T11 | Call `HookHandler` from the chain walk | `part.cpp:37`–`49` | dead; the chain walks `m_nextHookHandler`/`m_nextHookController` with no writer |
| T12 | Give `DialogHandler` a base and the eight part refs it already includes | `dialoghandler.h:44` | derives from nothing |

### 5.3 Close the message-coverage gap (`purpose.md:7`)

| # | Task | Site | Why |
|---|---|---|---|
| T13 | Add an `OnSysCommandArrange` case for `scArrange 0xF110` | `toplevel.cpp:178`–`255` | the one `sc*` constant with no case (`messages.h:313`) |
| T14 | Name `0x0017` as a `wm*` constant and claim it | `messages.h`; `lifetime.cpp:82` tests the raw value | the only message id in the tree with no constant |
| T15 | Route `wm93`/`wm94` to named ids | `messages.h:125` `:126` | the only identifiers named by value |
| T16 | Resolve `wmMouseHWheel`/`wmMouseLast` both `0x020E` | `messages.h:198` `:201` | duplicate-case collision in any `switch` |
| T17 | Make the `_WIN32_WINNT`/`_WIN32_WCE` arms in `messages.h:200`–`208` explicit | `messages.h` | include the defining header or drop the gate; today the `#else` arm is chosen silently |
| T18 | Replace the `true`-from-empty-hook arms | `toplevel.cpp:260` `:269` `:278` | 11 ids are claimed with no behaviour |
| T19 | Break the self-re-post loops | `toplevel.cpp:150` `:237`; `parent.cpp:121` `:113` `:118`; `ime.cpp:119` `:124` `:129` `:134` `:139` `:148` `:153` `:168` | handlers that post the id they handle |
| T20 | Give `wmUniChar 0x0109` (`messages.h:150`) and `wmIMEControl 0x0283` (`messages.h:237`) a claiming case | `ime.cpp:43`–`49` | IME ids claimed nowhere |
| T21 | Unify the message-id vocabularies to one spelling | `view/messages.h:29`–`38` vs SDK `LOWORD`/`HIWORD`/`MAKELONG` | 41 vs 11 call sites use the two spellings |

### 5.4 Make the layout engine actually run (part 80)

| # | Task | Site | Why |
|---|---|---|---|
| T22 | Use the `position` parameter in both arrangement bodies | `layout.cpp:31` `:120` | children always placed from `(0,0)` of the container |
| T23 | Fix the axis test in `placeHorizontally` | `layout.cpp:149` `:157` | both tests read `vertical_` |
| T24 | Guard the `spaceCount` division | `layout.cpp:87` `:176` | an all-fixed arrangement divides by 0 |
| T25 | Apply the min/max clamp at the four `SetPos`/`SetSize` sites | `layout.cpp:101` `:115` `:190` `:204` | the fields exist and are never consulted |
| T26 | Make `placeVertically`/`placeHorizontally` honour `alignment` | `layout.cpp:45`–`55`, `:134`–`146` | both `Alignment` enum arms are unreachable |
| T27 | Rename `VertcialLayout` → `VerticalLayout` | `layout.h:55` | live API name is misspelled |
| T28 | Remove the stray `;` after both loops | `layout.cpp:80` `:169` | |
| T29 | Make hit testing consult the layout instead of `+31`/`+5` | `mainwindow.cpp:102`–`139` | the comment at `:97`–`98` states the goal |
| T30 | Call at least one `Create*` control creator from an enabled example | `windowfactory.cpp:171` `:192` `:214` `:236` | `AddItem`/`Place` currently never execute |

### 5.5 Theme and dark/light preference (`purpose.md:9`)

| # | Task | Site | Why |
|---|---|---|---|
| T31 | Define the `TMT_*` theme-property constants that `themedata.h` comments out | 264 of 354 header lines | no `TMT_*` name exists anywhere |
| T32 | Move `DTTOpts`/`DTTCallbackProc` out of the `ThemeData` class body | `themedata.h:306`–`333` | the 7-param `DrawTextEx` (`:344`) is unnameable |
| T33 | Pass `DTTCallbackProc` as a callback, `DTTOpts` by reference | `themedata.cpp:1463` `:1471` | callback never runs; out-params cannot write back |
| T34 | Honour `pszElement` in `SetTheme` | `windowpainter.cpp:795` | uses the member (`:793`) |
| T35 | Store `nColourKey` in `SetColourKey` | `windowpainter.cpp:804` | always stores `0` |
| T36 | Call `SetWindowThemeAttribute` from `WindowPainter::SetTheme` | `uxtheme.h:112` | declared, never called |
| T37 | Add the user dark/light preference query path | none exists | `purpose.md:9` has no counterpart |

### 5.6 Off-screen composition as a library facility (`purpose.md:7`)

| # | Task | Site | Why |
|---|---|---|---|
| T38 | Move `CreateDIB`/`CreateRenderingContext`/`RenderScene`/`Blit` from the example into a library class | `mainwindow.cpp:251`–`315` | the whole GL pipeline is example code |
| T39 | Instantiate `DIBSection` through the wrapper instead of bypassing it | only live `CreateDIBSection` is `ui/gl/windowimpl.cpp:152` | `DIBSection`/`SysBrush` never instantiated |
| T40 | Uncomment the WGL surface | `wglcontext.h:39`–`57` | `wglCopyContext` `wglShareLists` `wglUseFontBitmapsT`, layer palette calls |
| T41 | Define the ~63 undeclared-in-definition `DeviceContext` methods | `devicecontext.h:118`–`219` commented | declarations with no definitions |
| T42 | Enable the `egl`/`gl` targets | `src/platform/os/windows/ui/CMakeLists.txt` (commented) | `purpose.md:5` names EGL |

### 5.7 Accessibility and IME (`purpose.md:8`)

| # | Task | Site | Why |
|---|---|---|---|
| T43 | Add the accessibility surface the purpose names — none exists | — | the clause has no counterpart in any area |
| T44 | Un-comment and implement the `WM_IME_CHAR` decode | `ime.cpp:79`–`86` | repeat count, scan code and the DBCS extended/context/previous/transition bits are all commented; live code calls `OnChar` (`:87`) then forces `lResult = 0` (`:88`) |
| T45 | Give the six `void` IME hooks a reportable return | `ime.h:41`–`51` | heterogeneous return types (`void` ×6, `long long` ×3, `bool` ×2) |
| T46 | Stop `OnStartComposition`/`OnEndComposition` forwarding `0, 0` | `ime.cpp:119` `:124` | payload dropped |
| T47 | Make `IMEHandler` reachable from the library | `interactive.h:52`–`61` IME slot | no writer exists in the built tree |

### 5.8 Controllers and common controls (`purpose.md:5`)

| # | Task | Site | Why |
|---|---|---|---|
| T48 | Attach a handler in the four common-control creators | `windowfactory.cpp:171` `:192` `:214` `:236` | they skip `SetHandler`, so every message falls to `DefWindowProc` |
| T49 | Add `windowclass_of` specialisations for the 12 `Window` subclasses | part 40 answer 2; part 20 answer 3 | the factory's class-name mapping is empty for every class |
| T50 | Define `DialogController` (declared, never defined) | `dialog.h:36` | |
| T51 | Give `Desktop` ctors and a base | `desktop.h:31` | `class Desktop {};` with no includes at all |
| T52 | Add the 21 controllers on the `controllers/purpose.md:15`–`:42` TODO list | `controllers/purpose.md` | one small TU each; start with `Animation` `ComboBoxEx` `DateTimePicker` `Header`, which already have a `windows/` type |
| T53 | Add `virtual ~Static()` | `static.h:81`–`82` | the only `Window` subclass without it |
| T54 | Give `button.h`'s class-scope `N*` constants out-of-line definitions | `button.h:69`–`78` | used by reference they fail to link |
| T55 | Resolve the vocabulary value collisions | `messagebox.h:60` `:64`; `scrollbar.h:44`–`47` | `MBTOPMOST` == `MBSERVICE_NOTIFICATION_NT3X`; `DisableLeft` == `DisableUp` |
| T56 | Replace the twelve `//TEMP:` min/max defaults with one documented default per class | the 12 `windows/*.cpp:42` bodies | the author marks all 12 provisional |
| T57 | Fix `Dialog::GetHandle()` to return the dialog's handle and expose `m_result` | `dialog.cpp:79`; `dialog.h:147` | `m_handle` left default; the outcome unreadable |

### 5.9 Verification — close the test-coverage deficit

| # | Task | Site | Why |
|---|---|---|---|
| T58 | Add a `test/ui/windows` target to `test/CMakeLists.txt` plus `test_main.cpp` | `test/CMakeLists.txt:5`; `test/ui/` holds only `terminal/` | the library has no test target at all |
| T59 | Controller round-trip tests, headless: `Show`/`Hide`, `Update`/`Redraw`/`Validate`, `SetPos`/`GetPos`, `GetClientId` | `windowcontroller.cpp:730`–`736` (never sizes), `:260`–`266` (1023 cap), `:844`–`849` (`Destroy`) | the two clearest defects are testable without an HWND |
| T60 | Layout tests, headless: non-zero `position`, an all-fixed arrangement, both `Alignment` arms | `layout.cpp:31` `:120` `:87` `:176` | three defects are pure arithmetic |
| T61 | `Rect` arithmetic tests | `rect.cpp:52` `:85` `:92`; `rect.h:78` | `operator!=` is `!(r == r)` — always true |
| T62 | `GDIObject` ownership tests: `Release` ordering, `operator->` null path | `gdiobject.cpp:42`–`57` `:91`–`96` | |
| T63 | Re-enable the seven disabled examples one at a time and log each build result | `osspecific/windows/CMakeLists.txt:1`–`3`, `:5`–`:8` | whole areas exercised by nobody |
| T64 | Record the five part-90 runtime checks in the observation log | `90-example-mainwindow.md:171`–`194` | the table reads "no observations recorded" |
| T65 | Add the `PerThread::GetWindow()` accessor the `#if` arm needs | `devicecontext.cpp:97`–`99`, `:124`–`132`; `perthread.h:130`–`132` (`m_dcMap`) | the thread-binding clause of `purpose.md:11`–`13` is otherwise unimplementable |

---

## 6. If only a few things are done, do these first

1. **T01–T05** — four unlinked TUs plus two broken includes. Cheapest, and they
   turn four documented classes into unusable link errors today.
2. **T06–T08** — the library installs one handler part in total. Everything the
   purpose says about "fully customizable message handling" is otherwise only
   reachable by hand-wiring in an example.
3. **T22–T26** — five one-line fixes in `layout.cpp` that currently make the
   arrangement engine place every child at `(0,0)`, ignore alignment, and divide
   by zero for all-fixed input.
4. **T31 + T37** — theming has no vocabulary (`TMT_*` is comment-only) and the
   dark/light clause of `purpose.md:9` has no code at all.
5. **T58–T60** — a test target for GUI does not exist; the library's only
   "test" is one example whose runtime behaviour has never been observed.

## 7. Limits of this analysis

- **Derived from documentation, not from re-reading the TUs.** Every claim carries
  the file:line citation recorded on a part page; only build files and directory
  listings were re-verified today. A claim is only as current as the page that
  records it, and the pages carry a "confirmed by reading" date — defects fixed
  since a page was written would not appear here.
- **Counts quoted from pages** (e.g. "63 of ~120 `DeviceContext` declarations",
  "264 of 354 `themedata.h` lines", "77 hooks against 75 claimed ids") are the
  pages' own counts.
- **Out of scope:** the sibling libraries the GUI links against
  (`winqor_api_*`, `winqor_common`, `qor_*`), the `ui/gl` and `ui/egl` trees
  (referenced only where they expose the same gap, e.g. `ui/gl/windowimpl.cpp:152`),
  and the portability layer above this library.
- **Re-verified today:** 109 `.h` + 103 `.cpp` = 212 files under `ui/gui`;
  99 `.cpp` entries in `add_library`; the four missing entries;
  `test/ui/` = `terminal/` only (`primary.cpp` `tui.cpp` `test_main.cpp`);
  `test/CMakeLists.txt` names no GUI source; only `mainwindow` enabled among the
  eight examples; `00-overview.md` repeats the three structural rules at
  `:21`–`32` and again at `:34`–`45`; `90-example-mainwindow.md` observation log
  at `:171`–`194` with the single row "no observations recorded" (`:179`).

## 8. Suggested next step

Pick from §6 in order. The first block (T01–T05) touches only `gui/CMakeLists.txt`
and two include lines, is verifiable by a single rebuild, and removes four link
errors that the documentation already names. The test-target block (T58–T60) is the
one that changes the shape of the whole project: it converts "the example is the
test" into something runnable headless, which is what makes the rest of this list
verifiable at all.










