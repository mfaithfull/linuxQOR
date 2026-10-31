# 70 — Painting and theming

**Status: detailed.** All 8 files (4 pairs, **747 lines**) read in full and
verified against source on 27 Oct 2026. Every claim names the file and the line
region. Where a claim is a design goal rather than an observation it is marked
`Windows API intent` or `code comment intent`.

## Files in this part

Verified line counts (h / cpp):

| Pair | Lines | Reach today |
|---|---|---|
| `paintsession` | 57 / 61 | **live** — the only `painting/` class with a construction site repo-wide (`mainwindow.cpp:172`) |
| `bufferedpainting` | 40 / 45 | compiled, linked, **never constructed** |
| `windowpainter` | 52 / 47 | compiled, linked, **never constructed**; the class it declares is named `Painter` |
| `themedata` | 354 / 91 | compiled, linked, **never constructed**; 264 of its 354 header lines sit in two comment blocks (`:40`–`81` `:83`–`304`) |

All four `.cpp` files are built into the library at `gui/CMakeLists.txt:93`–`96`
(source order there: `bufferedpainting.cpp` `themedata.cpp` `paintsession.cpp`
`windowpainter.cpp`), and the part's link dependency `winqor_api_uxtheme` is
declared at `gui/CMakeLists.txt:115`. The part is 747 lines — one session, no
split needed.

## What this part is for

`gui/purpose.md:7` states the goal this part serves: "It should support, hooks,
Owner Draw, Custom Draw, Themes and Off Screen Rendering with Composition to an
Open GL target window so that we can combine standard windows controls with GL
elements, transparency, non rectangular windows and animation." `purpose.md:9`
adds "Have the capability to follow the users preference for dark/light theme".

Four classes, four intended roles:

| Class | Declared at | Intended role | Win32 pairing actually made |
|---|---|---|---|
| `PaintSession` | `paintsession.h:39` | Owner Draw — a `WM_PAINT`-scoped paint session | `BeginPaint` / `EndPaint` |
| `BufferedPainting` | `bufferedpainting.h:31` | DirectWrite buffered paint, on/off | `BufferedPaintInit` / `BufferedPaintUnInit` |
| `Painter` | `windowpainter.h:42` | Custom Draw / frame painting | **none** — the class makes no Win32 call |
| `ThemeData` | `themedata.h:36` | one `HTHEME` per window: measure + draw themed parts | `Get(Window)Theme` / `OpenThemeData(Ex)` … `CloseThemeData` |

The part is a thin RAII shim layer: it owns handles, pairs their open/close calls,
and forwards the rest of its surface straight to `qor::win::api::UXTheme`. There
is no painting logic of its own — no brush selection, no path recording, no
compositing. "Off Screen Rendering with Composition to an Open GL target window"
is done by the **example**, not by this part (see §"Off-screen composition").

## PaintSession — the only live class in the part

`paintsession.h` (57 lines) forward-declares `Window` (`:36`) and `DeviceContext`
(`:37`) and declares the class at `:39`–`53`: ctor `PaintSession(Window*)` (`:42`),
virtual dtor (`:43`), `GetWindow()` (`:45`), `GetDC()` returning
`ref_of<DeviceContext>::type` (`:46`); private state `m_dc` (`:50`),
`m_window` (`:51`), `PaintStruct m_paint` (`:52`). Header includes are
`currentthread.h` `typeidentity.h` `newref.h` `structures.h` (`:28`–`31`); the
last supplies `PaintStruct`, the hand mirror of `PAINTSTRUCT` at
`src/platform/os/windows/common/structures.h:417`–`425` (`hdc` `fErase` `rcPaint`
`fRestore` `fIncUpdate` `rgbReserved[32]`). The `//All types on this interface
must be portable` comment is at `:33`; the closing comment `}}}//qor::platform::nswindow`
(`:55`) is a slip repeated in all four headers of this part. The include guard is
`QOR_PP_H_WINDOWS_GUI_PAINTING_SESSION` (`:25`–`26`) — the file is `paintsession`,
the guard says `SESSION`: the `paint` prefix is dropped.

`paintsession.cpp` (61 lines) includes `configuration.h` (`:25`), `paintsession.h`
(`:27`), `../window.h` `../gdiobjects/devicecontext.h` `../perthread.h`
(`:28`–`30`) and `api_layer/user/user32.h` (`:32`), then opens
`using namespace qor::win::api` (`:34`). The whole class is five functions:

```cpp
PaintSession::PaintSession(Window* window) : m_window(window)        // :38
{
    auto perThread = new_ref<PerThread>();                            // :40
    HDC hdc = BeginPaint((HWND)(m_window->GetHandle().Use()),         // :41
                         reinterpret_cast<LPPAINTSTRUCT>(&m_paint));
    m_dc = perThread->DeviceContextFromHandle(hdc);                   // :42
}
PaintSession::~PaintSession()
{
    EndPaint((HWND)(m_window->GetHandle().Use()),                     // :47
             reinterpret_cast<LPPAINTSTRUCT>(&m_paint));
}
```

Observations, in execution order:

1. `new_ref<PerThread>()` is `instancer_of<PerThread>::Instance<PerThread>(1)`
   (`newref.h:18`–`20`), the refcounted per-thread state of the calling thread —
   this is how the part binds to `purpose.md:13` rather than to the window.
2. `BeginPaint` runs on `m_window->GetHandle().Use()`; `Window::GetHandle()`
   returns `const Handle&` (`window.h:58`). Neither ctor nor dtor null-checks
   `m_window`.
3. `DeviceContextFromHandle` (`perthread.cpp:163`–`176`) looks up
   `m_deviceContextHandleMap`, which is always empty (no `AddDeviceContext` call
   site repo-wide, established in part 60), so the lookup misses and the
   `hDC != nullptr` branch fabricates `new_ref<DeviceContext>(PrimitiveHandle(hDC), false)`
   (`perthread.cpp:173`) — `takeOwnership = false`.
4. That DC is a `GDIObject` (`devicecontext.h:237`); its handle ctor
   (`devicecontext.cpp:41`–`45`) delegates to `GDIObject(h, ODeviceContext,
   takeOwnership)`, whose handle ctor calls `m_handle.DontClose()`
   (`gdiobject.cpp:44`) → `m_needsClose = false` (`handle.cpp:180`), so
   `~Handle()` (`handle.cpp:89`–`96`) closes nothing. **Net effect: the HDC that
   `BeginPaint` returned is wrapped but never released through the object; the
   only release step this class performs is `EndPaint` in the destructor**, using
   the same `m_paint` that `BeginPaint` filled.
5. `GetDC()` (`:55`–`58`) hands the fabricated ref back; `GetWindow()`
   (`:50`–`53`) hands back the raw `Window*` it was given.

The live call site is `mainwindow.cpp:172`–`173`, inside
`CustomRenderingHandler::OnPaint` (class declared `mainwindow.cpp:143`,
`OnPaint` spans `:167`–`183` and takes only `Window& window`):

```cpp
PaintSession ps(&window);                          // :172
ref_of<DeviceContext>::type dc = ps.GetDC();       // :173
Rect rc;                                           // :174
RenderScene(); // OpenGL -> DIB                    // :175
Blit(dc()());  // DIB -> hDC                       // :176
controller->GetClientRect(rc);                     // :177
Size size;                                         // :178
dc->GetTextExtentPoint32T(L"Hello, Windows!", 15, &size);                   // :179
dc->TextOutT((rc.Width()/2)-(size.m_x/2), (rc.Height()/2) - (size.m_y/2),
             L"Hello, Windows!", 15);                                       // :180
```

The session's DC is the destination of a `BitBlt` (`Blit` at
`mainwindow.cpp:243`–`249` → `dest.BitBlt(0, 0, m_width, m_height, m_dcDIB()(), 0, 0, SrcCopy)`)
and of two GDI text calls. `ps` lives for the whole handler body — exactly the
`WM_PAINT` scope `BeginPaint`/`EndPaint` require.

`WindowController` carries its own copy of the same idiom: `BeginPaint(PaintStruct&)`
(`windowcontroller.h:235`), implemented at `windowcontroller.cpp:517`–`523`
(`User32::BeginPaint(WINDOW_HANDLE, …)` at `:520` then
`m_perThread->DeviceContextFromHandle(hdc)` at `:521`, returning a **raw
`DeviceContext*`**), paired by `EndPaint(const PaintStruct&)`
(`windowcontroller.h:238`). The example chose the `PaintSession` form; **nothing in
the library calls the controller's `BeginPaint`/`EndPaint`**, so the two are parallel,
not layered.

## BufferedPainting — the smallest live-looking pair

`bufferedpainting.h` (40 lines) is the whole class surface: `BufferedPainting()`
(`:34`) and `virtual ~BufferedPainting()` (`:35`) — **no members, no other
declarations**, and it includes nothing at all (the header body is `:29`–`38`,
guard `QOR_PP_H_WINDOWS_GUI_PAINTING_BUFFEREDPAINTING` at `:25`–`26`).

`bufferedpainting.cpp` (45 lines) includes `src/configuration/configuration.h`
(`:25`), `bufferedpainting.h` (`:27`) and `api_layer/uxtheme/uxtheme.h` (`:29`),
opens `using namespace qor::win::api` (`:31`), then:

```cpp
BufferedPainting::BufferedPainting()   { UXTheme::BufferedPaintInit();   }   // :37
BufferedPainting::~BufferedPainting()  { UXTheme::BufferedPaintUnInit(); }   // :42
```

Both forwards are real, not stubs: `BufferedPaintInit` is `Library::Call<HRESULT>(pFunc)`
at `api_layer/uxtheme/uxtheme.cpp:68`–`73` (`BufferedPaintUnInit` at `:96`–`101`),
declared `uxtheme.h:42` and `:46`. **The `HRESULT` return of both calls is
discarded** — a failure to enter buffered-paint mode is silent.

There is no construction site for `BufferedPainting` anywhere: the only occurrences
of the name repo-wide are its own header, its own `.cpp`, `gui/CMakeLists.txt:93`,
and this documentation. So the pair is compiled, linked, and never executed.

Windows API intent: `BufferedPaintInit`/`UnInit` are the process-wide enable/disable
pair for the Uxtheme buffered-paint (DirectWrite) path. The class shape says the
design wanted a scoped "buffered paint is on for this block" object. Note that
nothing in the library ever calls the related `BeginBufferedPaint` (`uxtheme.h:39`),
`BeginBufferedAnimation` (`uxtheme.h:38`), `BufferedPaintClear` (`:41`),
`BufferedPaintRenderAnimation` (`:43`), `BufferedPaintSetAlpha` (`:44`) or
`BufferedPaintStopAllAnimations` (`:45`) — the api_layer declares all of them and
`uxtheme.cpp` forwards them, but no `gui/` TU names them.

## Painter — a stub, and the file name lies

`windowpainter.h` (52 lines): guard `QOR_PP_H_WINDOWS_GUI_PAINTING_PAINTER`
(`:25`–`26`), includes `currentthread.h` `typeidentity.h` `newref.h`
`structures.h` (`:28`–`31`), then the design comment:

```
//Painter to paint a window, either for WM_PAINT handling or for Frame painting before UpdateLayeredWindow
//This will need to take into account style and maybe layout information
//May need to be retargetable to a bitmap or other surface.
```

at `:33`–`35`. It forward-declares `Window` (`:39`) and `DeviceContext` (`:40`)
and declares `Painter` at `:42`–`48` — default ctor and virtual dtor only. No
members, no paint surface, no retargeting hook, no `DeviceContext` parameter.
The class name is `Painter`; the file is `windowpainter`; the guard says
`PAINTER`. Three different names for one thing.

`windowpainter.cpp` (47 lines) includes `windowpainter.h` `../window.h`
`../gdiobjects/devicecontext.h` `../perthread.h` and `api_layer/user/user32.h`
(`:27`–`32`) — four includes it does not use — and defines:

```cpp
Painter::Painter()   { auto perThread = new_ref<PerThread>(); }   // :38–41
Painter::~Painter()  {                                          }   // :43–45
```

The ctor creates a per-thread ref, binds it to a local, and drops it; the dtor is
empty. **`Painter` makes no Win32 call at all.** No construction site exists
repo-wide (the name occurs only in these two files, `gui/CMakeLists.txt:96`, and
the docs). The "Custom Draw / frame painting before `UpdateLayeredWindow`" goal of
`purpose.md:7` is therefore unimplemented, not merely unused.

## ThemeData — the handle owner and the theme forwards

`themedata.h` (354 lines) includes `../window.h` (`:28`) and
`../gdiobjects/font.h` (`:29`) — the second one is there for `LogFont`, declared
`font.h:43` — and forward-declares `DeviceContext` (`:34`). The class is `:36`–`350`:
three ctors (`:335`–`337`), virtual dtor (`:338`), six operations (`:339`–`344`),
and **one protected member, `Handle m_handle`** (`:348`). There is no theme state
object: no colours, no metrics cache, no part/state table.

The bodies (`themedata.cpp`, 91 lines) make the handle discipline explicit:

```cpp
ThemeData::ThemeData(const Window& window) :
    m_handle(UXTheme::GetWindowTheme((HWND)(window.GetHandle().Use())))   // :39
{ m_handle.DontClose(); }                                                 // :41
ThemeData::ThemeData(const Window& window, const wchar_t* classList) :
    m_handle((UXTheme::OpenThemeData((HWND)(window.GetHandle().Use()), classList)))   // :45
{ m_handle.DontClose(); }
ThemeData::ThemeData(const Window& window, const wchar_t* classList, unsigned long flags) :
    m_handle((UXTheme::OpenThemeDataEx((HWND)(window.GetHandle().Use()), classList, flags)))  // :51
{ m_handle.DontClose(); }
ThemeData::~ThemeData() { UXTheme::CloseThemeData((HTHEME)(m_handle.Use())); }        // :58
```

`Handle(void*)` sets `m_needsClose = !GetProtectFromClose()` (`handle.cpp:84`), so
without the `DontClose()` calls the `HTHEME` would be destroyed through
`Handle::Close()` → `Kernel32::CloseHandle(m_h)` (`handle.cpp:199`–`207`, call at
`:204`) — the wrong API for a theme handle.
`DontClose()` (`handle.cpp:178`–`181`) clears the flag and the destructor's
`CloseThemeData` is the right pairing, so **this class is one of the few in the
library whose handle close-step matches the handle kind** (part 60 found the
opposite for the metafile family and for `Palette`/`ColourSpace`).

The six operations, each a single forward:

| `ThemeData` member | Declared | Defined | Forwards to | `api_layer` verdict |
|---|---|---|---|---|
| `GetSysSize(int sizeId)` | `:339` | `:61`–`64` | `UXTheme::GetThemeSysSize` | real — `uxtheme.cpp:408`–`413` `Library::Call<int, HTHEME, int>` |
| `GetSysFont(int fontId, LogFont&)` | `:340` | `:66`–`69` | `UXTheme::GetThemeSysFont` | **stub** — `uxtheme.cpp:396`–`400` is `//TODO: return S_OK;` |
| `DrawBackground(DeviceContext&, int, int, const Rect&, Rect&)` | `:341` | `:71`–`74` | `UXTheme::DrawThemeBackground` | real — `uxtheme.cpp:110`–`115` |
| `DrawEdge(DeviceContext&, int, int, const Rect&, uint, uint, Rect&)` | `:342` | `:76`–`79` | `UXTheme::DrawThemeEdge` | real — `uxtheme.cpp:124`–`129` |
| `DrawTextEx(DeviceContext&, int, int, const std::wstring&, ulong, Rect&)` | `:343` | `:81`–`84` | `UXTheme::DrawThemeTextEx` with `nullptr` options | real — `uxtheme.cpp:159`–`164` |
| `DrawTextEx(…, const DTTOpts&)` | `:344` | `:86`–`89` | `UXTheme::DrawThemeTextEx` with `&options` | real — same forward |

`themedata.cpp` includes `src/configuration/configuration.h` (`:25`), `themedata.h`
(`:27`), `../gdiobjects/devicecontext.h` (`:28`) and `api_layer/uxtheme/uxtheme.h`
(`:30`), then `#undef DrawTextEx` at `:32` — the only `#undef` in this part, guarding
the member name against the same-named SDK macro that reaches the translation unit
through the api_layer chain (`api_layer/user/user32.h:569` declares `DrawTextExT`; the
SDK spelling comes from `<gdi32n.h>`). Marked as intent: the undef is the only reason
the name can be there, but no comment states it.

`GetSysFont`'s out-parameter crosses as `reinterpret_cast<LOGFONTW*>(&logFont)`
(`themedata.cpp:68`) against the api_layer signature `GetThemeSysFont(HTHEME, int,
LOGFONTW*)` (`uxtheme.h:92`) — the QOR mirror `LogFont` (`font.h:43`) is trusted to be
layout-identical to the SDK `LOGFONT`. The `DeviceContext` argument in every draw op
is unwrapped as `(HDC)(dc.GetHandle().Use())` (`DeviceContext::GetHandle()` at
`devicecontext.h:250`), and `Rect`/`DTTOpts` cross as `reinterpret_cast<LPCRECT>`/`LPRECT`/
`const DTTOPTS*` — layout compatibility is asserted by the mirror shape, never checked.
The text length is `static_cast<int>(text.size())` from a `std::wstring`, so a `wchar_t`
count is what the Win32 `cchText` slot gets.

`DTTOpts` (`themedata.h:316`–`333`) is the hand mirror of `DTTOPTS`, 15 fields:
`dwSize` `dwFlags` `crText` `crBorder` `crShadow` `iTextShadowType` `ptShadowOffset`
`iBorderSize` `iFontPropId` `iColorPropId` `iStateId` `fApplyOverlay` `iGlowSize`
`pfnDrawTextCallback` `lParam`. Unlike the other mirrors in the tree, **`dwSize`
is not defaulted** — nothing sets `sizeof(DTTOpts)`, so a caller that leaves the
struct to chance hands `DrawThemeTextEx` an uninitialised size. The callback slot
uses `DTTCallbackProc`, the `__stdcall` typedef at `:306`–`314`
(`qor_pp_compiler_stdcallconvention` = `__stdcall` in `src/platform/compiler/msvc/msvc.h:36`
and `clang/clang.h:27`); nothing in the repo ever supplies a callback.

## The theme-property vocabulary — what survived

This is why `themedata.h` is 354 lines while `themedata.cpp` is 91. Almost none of
the header is code: the class body opens `public:` at `:38` and then carries **two**
C block comments — `/*` at `:40` closing `*/` at `:81`, and `/*` at `:83` closing
`*/` at `:304`. Only one line of the whole `:40`–`304` span is live code:

```cpp
static const int CaptionFont     = 801;      // themedata.h:82
```

| Region | Content | Verdict |
|---|---|---|
| `:40`–`81` | comment block 1: 27 `TMT_*` property-**type** defines (`TMT_RESERVEDLOW 0` `:44`, `TMT_RESERVEDHIGH 7999` `:45`, `TMT_DIBDATA 2` `:47`, `TMT_ENUM 200` `:49` … `TMT_COLORSCHEMES 401` `:67` `TMT_SIZES 402` `:68` `TMT_CHARSET 403` `:69`, name ids `TMT_NAME 600`–`TMT_DESCRIPTION 608` `:70`–`78`, RC-string bounds `:79`–`80`) | commented |
| `:82` | `static const int CaptionFont = 801;` | **live — the one theme-property name the part exports** |
| `:83`–`304` | comment block 2 (~220 defines): fonts 802–809 (`:84`–`91`) with `TMT_FIRSTFONT`/`TMT_LASTFONT` `:92`–`93`, bools 1001 (`:94`, bounds `:95`–`96`), sizes 1201–1210 (`:97`–`106`, bounds `:107`–`108`), ints 1301 (`:109`), strings 1401–1404 (`:112`–`115`), colours 1601–1631 (`:118`–`148`, bounds `:149`–`150`), hue pairs 1801–1810 (`:151`–`160`), gradient colours 2001–2010, behaviour flags 2201–2220, numerics 2401–2434 (ends `:224`), glyph font 2601 (`:225`), image files 3001–3010 (`:226`–`234`), text/classic 3201–3202 (`:235`–`236`), offsets/min sizes 3401–3411 (`:237`–`247`), margins 3601–3603 (`:248`–`250`), element colours 3801–3827 (`:251`–`277`), enum types 4001–4015 (`:278`–`292`), picture/pane/animation 5001–5006 (`:293`–`298`), transitions 6000 (`:299`), scaled background 7001 (`:300`), atlas 8000–8002 (`:301`–`303`) | commented |
| `:306`–`314` | `DTTCallbackProc` typedef | live |
| `:316`–`333` | `struct DTTOpts` | live |
| `:335`–`344` | ctor/dtor/operation declarations | live |
| `:346` `:348` | `protected:` and `Handle m_handle` | live |

`CaptionFont` is declared *inside* the class under `public:`, so the one name that
is reachable from application code is `qor::platform::win::gui::ThemeData::CaptionFont`
= 801 — the SDK `TMT_CAPTIONFONT` id, spelled as a literal because the name itself
is nowhere defined (the commented `TMT_FIRSTFONT` at `:92` refers to
`TMT_CAPTIONFONT`, which the header never defines).

Consequence for the API: `GetSysSize(int)` and `GetSysFont(int, …)` take a bare `int`,
and **no name in this part can supply a size id** — the ids the theme expects
(`TMT_SIZINGBORDERWIDTH 1201` `:97` … `TMT_PADDEDBORDERWIDTH 1210` `:106`, bounded by
`TMT_FIRSTSIZE`/`TMT_LASTSIZE` at `:107`–`108`) exist only as text. A caller must write
a magic number. The example's commented-out call shows the resulting confusion:

```
proposedClientArea->m_top += 0;    // Title bar height//themeData->GetSysSize(SMCYSIZE)
```

at `mainwindow.cpp:45`. `SMCYSIZE` **does** exist in the tree — as
`constexpr unsigned int SMCYSIZE = 31;` at `src/platform/os/windows/common/constants.h:181`,
inside the legacy `SM*` size-id block (`constants.h:150`–`193`). That is a **different
vocabulary** from the theme ids: 31 is a legacy `SM*` id, the theme equivalent is
`TMT_SMCAPTIONBARHEIGHT 1207` (`themedata.h:103`, inside comment block 2). The commented-out
call mixes the two, so even as intent it would return the wrong metric. The
surrounding comments at `mainwindow.cpp:43`–`46` ("Left frame width" "Right frame width"
"Bottom frame width") all add/subtract `0` — the theme-metric maths was stubbed to
zero and left there.

## The UXTheme call map for the whole library

Within `gui/`, the entire `UXTheme::` call surface is 13 sites in three files:

| File | Sites | Functions |
|---|---|---|
| `painting/themedata.cpp` | 10 (`:39` `:45` `:51` `:58` `:63` `:68` `:73` `:78` `:83` `:88`) | `GetWindowTheme` `OpenThemeData` `OpenThemeDataEx` `CloseThemeData` `GetThemeSysSize` `GetThemeSysFont` `DrawThemeBackground` `DrawThemeEdge` `DrawThemeTextEx` ×2 |
| `painting/bufferedpainting.cpp` | 2 (`:37` `:42`) | `BufferedPaintInit` `BufferedPaintUnInit` |
| `controllers/windowcontroller.cpp` | 1 (`:829`) | `SetWindowTheme` — the body of `WindowController::SetTheme(subAppName, subIdList)` (`windowcontroller.h:308`) |

So `winqor_api_uxtheme` (`gui/CMakeLists.txt:115`) exists to serve this part plus
one controller method, and `uxtheme.h` declares **77** entry points (`static`
declarations) of which **12 distinct ones are reachable from the library** — all
declared inside `uxtheme.h:42`–`111`: `BufferedPaintInit` `:42` `BufferedPaintUnInit`
`:46` `CloseThemeData` `:47` `DrawThemeBackground` `:48` `DrawThemeEdge` `:50`
`DrawThemeTextEx` `:55` `GetThemeSysFont` `:92` `GetThemeSysSize` `:94`
`GetWindowTheme` `:100` `OpenThemeData` `:108` `OpenThemeDataEx` `:109`
`SetWindowTheme` `:111`. `uxtheme.cpp` defines **all 77** of them and **29 of those
bodies are `//TODO:` stubs** — one of which is `GetThemeSysFont`, the only stub the
part depends on, so 48 of the 77 make a real UXTHEME call. The 65 never named from
`gui/` include the buffered-paint entry points the part might have used:
`BeginBufferedAnimation` `:38`, `BeginBufferedPaint` `:39`, `BufferedPaintClear`
`:41`, `BufferedPaintRenderAnimation` `:43`, `BufferedPaintSetAlpha` `:44`,
`BufferedPaintStopAllAnimations` `:45`, plus `SetWindowThemeAttribute` `:112` and
`SetWindowThemeNonClientAttributes` `:113`.
`SetWindowTheme` at `windowcontroller.cpp:829` is the *only* theme call the running
example makes (`mainwindow.cpp:368` `second->SetTheme(L" ", L" ")`).

## How this part connects to the rest of the library

| Edge | Evidence |
|---|---|
| `PaintSession` → `PerThread` | ctor `new_ref<PerThread>()` (`paintsession.cpp:40`) → `DeviceContextFromHandle` (`perthread.cpp:163`) — part 10's per-thread state |
| `PaintSession` → `Window` | raw `Window*` ctor arg (`paintsession.h:42`), unwrapped `GetHandle().Use()` (`paintsession.cpp:41` `:47`) — part 10's `Window` |
| `PaintSession` → `DeviceContext` | `GetDC()` returns `ref_of<DeviceContext>::type` (`paintsession.h:46`) — part 60's GDI object |
| `ThemeData` → `Window` | takes `const Window&` in all three ctors (`themedata.h:335`–`337`) and unwraps `GetHandle().Use()` |
| `ThemeData` → `DeviceContext` | every draw op takes `DeviceContext&` (`themedata.h:341`–`344`) |
| `ThemeData` → `Font`'s mirror | `LogFont` (`font.h:43`) reached via the header's `#include "../gdiobjects/font.h"` (`:29`) — part 60c |
| `BufferedPainting` | **no edges at all** — the header includes nothing |
| `Painter` | includes four headers, uses none of them — no edges |
| `PaintSession` ↔ `WindowController::BeginPaint/EndPaint` | parallel copies (`windowcontroller.h:235` `:238`), neither calls the other |
| `WindowController::SetTheme` → `UXTheme::SetWindowTheme` | `windowcontroller.cpp:829`, declared `windowcontroller.h:308` — the only theme entry point used by the example |
| consumers that *include* this part | `mainwindow.cpp:11` (`themedata.h`) `mainwindow.cpp:12` (`paintsession.h`); nothing in `gui/` |
| built into the library | `gui/CMakeLists.txt:93`–`96` (all four `.cpp`) |

**No header in `gui/` includes any header from `painting/`.** The part is
consumer-facing only: reachable from application code (the example) and from
nothing inside the library. That is why three of its four classes are never
constructed — the library never asks for them.

## Off-screen composition — where this part does *not* do it

`purpose.md:7`'s "Off Screen Rendering with Composition to an Open GL target
window" is carried out by the example, not by this part (`mainwindow.cpp`). All
four steps are members of `CustomRenderingHandler : public RenderingHandler`
(`mainwindow.cpp:143`–`317`):

- `CreateDIB` (`mainwindow.cpp:251`–`288`) → `DeviceContext::CreateForDIB()` (`:270`) + `new_ref<DIBSection>(m_dcDIB, (BitmapInfo*)(&m_BIH), DIBRGBCOLORS, &m_bmp, nullptr, 0)` (`:279`).
- `CreateRenderingContext` (`:290`–`315`) → `m_context = new_ref<WGLContext>(m_dcDIB)` (`:313`), after `ChoosePixelFormat`/`SetPixelFormat` on the DIB DC.
- `RenderScene` (`:223`–`240`) drives `OpenGLSession` (`guiThread->OGLSession()`), and `Blit(dc()())` at `OnPaint` `:176` copies the DIB into the paint DC via `dest.BitBlt(0, 0, m_width, m_height, m_dcDIB()(), 0, 0, SrcCopy)` (`mainwindow.cpp:247`).
- `OnPaint` (`:167`–`183`) is the only place in the tree where a `painting/` object exists: `PaintSession ps(&window)` supplies the destination DC for that blit.

So the off-screen path is **`DeviceContext::CreateForDIB()` + `DIBSection` + `WGLContext`**
(matches part 60b's conclusion that the metafile family is not the off-screen path),
and `painting/` contributes only the `BeginPaint`/`EndPaint` bookkeeping at the end
of that chain. `BufferedPainting` (the DirectWrite alternative) and `Painter`
(the frame painter) are both absent from the chain.

## Installation and build facts

- All four `.cpp` files are in `add_library` (`gui/CMakeLists.txt:93`–`96`);
  unlike parts 30/40/50 this subtree has **no file absent from the build**.
- `winqor_api_uxtheme` (`gui/CMakeLists.txt:115`) is the part's reason for existing;
  `paintsession.cpp` and `windowpainter.cpp` additionally pull
  `api_layer/user/user32.h` whose `BeginPaint` (`user32.h:263`) / `EndPaint`
  (`:270`) are the wrapped pair.
- `painting/` is not a CMake target of its own — its sources are listed flat in the
  `gui` library's source list, and there is no `painting/CMakeLists.txt`.
- `gui/CMakeLists.txt:107`–`110` puts `QOR_WINGUI` and the version triple on every
  TU, which is what the `qor_pp_module_interface(QOR_WINGUI)` decorations on the
  four classes (`themedata.h:36` `paintsession.h:39` `bufferedpainting.h:31`
  `windowpainter.h:42`) and the forward declarations beside them consume.
- `mainwindow` is the only consumer, and it is an *example* target, not a test —
  the library has no unit tests, so verification of this part is read-only plus the
  example's build.

## Gotchas and open questions

1. **`Painter` and `BufferedPainting` are dead weight.** Both are compiled, linked,
   and never constructed. `Painter` is an empty class whose header claims three
   unimplemented features (`windowpainter.h:33`–`35`). The part's honest name is
   "one class that works, one class that forwards, two classes that exist".
2. **`GetSysFont` always lies.** `ThemeData::GetSysFont` returns `S_OK`
   unconditionally because `uxtheme.cpp:396`–`400` is `//TODO: return S_OK;` — the
   caller's `LogFont&` is left untouched. Any code that trusts the verdict gets a
   success with an unset out-parameter. (29 of `uxtheme.cpp`'s 77 bodies are such
   stubs; this is the one the part depends on.)
3. **`themedata.h`'s bulk is commented vocabulary** — 264 of its 354 lines sit in two
   C block comments (`/*` `:40`→`*/` `:81` = 42 lines, `/*` `:83`→`*/` `:304` = 222), and
   only `:82` is live between them. They are a record of the theme property ids the
   author intended to use; **no `TMT_*` name is defined anywhere in the tree**, so
   `GetSysSize`/`GetSysFont` ids must be hard-coded by callers. The one live name,
   `CaptionFont` = 801 (`:82`), is a *static data member of `ThemeData`* — reachable
   only as `ThemeData::CaptionFont`, and referenced nowhere. Deleting the blocks
   changes no behaviour.
4. **`DTTOpts::dwSize` is uninitialised** (`themedata.h:316`), unlike the sibling
   mirrors in `common/structures.h` that default `cbSize`/`cbSize`. Passing
   `DrawTextEx(…, options)` with a caller-built `DTTOpts` sends a garbage size to
   `DrawThemeTextEx`.
5. **The `SMCYSIZE` id in `mainwindow.cpp:45` is the wrong vocabulary.** `SMCYSIZE`
   is `31` (`common/constants.h:181`, legacy `SM*` block `:150`–`193`); the theme
   size ids are the 1201–1210 block in the commented text (`TMT_SIZINGBORDERWIDTH`
   … `TMT_PADDEDBORDERWIDTH`, bounded by `TMT_FIRSTSIZE`/`TMT_LASTSIZE`). Anyone
   reviving that line must pick the theme namespace first — and the client-area
   maths beside it (`mainwindow.cpp:43`–`46`) adds/subtracts literal `0`.
6. **`PaintSession`'s `Window*` is unchecked** and the ctor takes a raw pointer where
   the rest of the tree passes `Window&` or `ref_of<…>`; the class is constructible
   from a null pointer and both ctor (`:41`) and dtor (`:47`) would dereference it.

7. **`PaintSession` never releases the HDC it obtains**; it only calls `EndPaint`
   against the `PaintStruct` `BeginPaint` filled, and `DeviceContextFromHandle`'s
   fabrication (`perthread.cpp:173`) means the per-thread DC registry is bypassed.
8. **Two independent `BeginPaint`/`EndPaint` sites** (`paintsession.cpp:41` `:47`) and
   `WindowController::BeginPaint` (`windowcontroller.h:235`, defined `windowcontroller.cpp:517`–`523`)
   / `WindowController::EndPaint` (`windowcontroller.h:238`, defined `windowcontroller.cpp:535`–`538`) — with no
   shared owner. Nesting them would produce two `BeginPaint`s for one `WM_PAINT`
   message; nothing prevents that today because the controller pair has no caller.
9. **`DTTOpts` and `DTTCallbackProc` are declared inside the `ThemeData` class body**
   (`themedata.h:306`–`333`, i.e. inside `:36`–`350`), so they are member names with
   class scope: code outside `ThemeData` cannot name the type, which makes the
   7-parameter `DrawTextEx` overload (`themedata.h:344`) effectively unusable from
   application code. Only the 6-parameter, `nullptr`-options overload (`:343`) is
   reachable, and that is the whole reason `DTTOpts` exists in a header nobody can
   use.
10. **The `purpose.md:9` dark/light-theme goal is only half wired**: the example sets
   a theme with `SetTheme(L" ", L" ")` (`mainwindow.cpp:368` →
   `UXTheme::SetWindowTheme` `windowcontroller.cpp:829`) — a single space, i.e. an
   empty-string placeholder — and nothing anywhere reads the user's dark/light
   preference. `SetWindowThemeAttribute` (`uxtheme.h:112`) and
   `SetWindowThemeNonClientAttributes` (`:113`) are declared and never called.
11. **Naming drift, four ways**: `windowpainter.h/.cpp` ↔ class `Painter` ↔ guard
    `..._PAINTING_PAINTER`; `paintsession.h/.cpp` ↔ class `PaintSession` ↔ guard
    `..._PAINTING_SESSION`; `themedata.h/.cpp` ↔ class `ThemeData` ↔ guard
    `..._PAINTING_THEMEDATA` ↔ doc comment `Themedata`. `bufferedpainting.h` is the
    only pair whose guard matches its file name. All four headers also close with
    `}}}//qor::platform::nswindow` (`paintsession.h:55` `bufferedpainting.h:38`
    `windowpainter.h:50` `themedata.h:352`), and all four `.cpp` files close with the
    same slip (`paintsession.cpp:61` `bufferedpainting.cpp:45` `windowpainter.cpp:47`
    `themedata.cpp:91`).
12. **Open decision worth recording**: whether `BufferedPainting` should be given a
    `HPAINTBUFFER`-owning surface (the api_layer already wraps `BeginBufferedPaint`
    at `uxtheme.h:39` and `BeginBufferedAnimation` at `:38`) or deleted, and whether
    `Painter` should be implemented against `WindowController::BeginPaint`/`EndPaint`
    or deleted. Both are currently link-time no-ops.

## Verification of this part

- Read in full: `paintsession.h` 57, `paintsession.cpp` 61, `bufferedpainting.h` 40,
  `bufferedpainting.cpp` 45, `windowpainter.h` 52, `windowpainter.cpp` 47,
  `themedata.h` 354, `themedata.cpp` 91 — **747 lines, 8/8 files**.
- Re-checked after writing: line counts via `Get-Content` (all 8 above);
  `themedata.h` two block comments `/*` `:40` → `*/` `:81` and `/*` `:83` → `*/`
  `:304`, live `CaptionFont` `:82`, `DTTCallbackProc` `:306`, `DTTOpts` `:316`,
  `pfnDrawTextCallback` `:331`, `Handle m_handle` `:348`, `#include "../gdiobjects/font.h"` `:29`;
  `uxtheme.h` 77 `static` declarations and `uxtheme.cpp` 77 definitions with 29 `//TODO:` stubs, with the
  12 reachable names located at `uxtheme.h:42`–`111` and zero hits for the other
  buffered-paint/attribute names anywhere under `gui/`; `Handle::DontClose` at
  `handle.cpp:178`–`181` and `m_needsClose = !GetProtectFromClose()` at `handle.cpp:84`;
  `PaintStruct` at `common/structures.h:417`–`425`; `SMCYSIZE = 31` at
  `common/constants.h:181` (legacy `SM*` block `:150`–`193`);
  `qor_pp_compiler_stdcallconvention` → `__stdcall` at `msvc.h:36` (`clang.h:27`);
  `DeviceContext` ctors `devicecontext.cpp:37`/`:41` and `GetHandle()` `devicecontext.h:250`;
  `LogFont` `font.h:43`; `new_ref` `newref.h:18`–`20`; `gui/CMakeLists.txt:93`–`96` `:107`–`110` `:115`;
  `user32.h:263` `:270` `:569`; `mainwindow.cpp` lines 11/12, 43–46, 143–317, 167–183, 172–180, 223–240, 243–249,
  251–288, 290–315, 313, 368; `purpose.md:7` `:9` `:13`; `windowcontroller.h:235` `:238` `:308`;
  `windowcontroller.cpp:517`–`523` `:829`; `perthread.cpp:163`–`176`;
  `gdiobject.cpp:41`–`44` `gdiobject.h:64`; `handle.cpp:84` `:89`–`96` `:178`–`181`.
- Zero-construction claims (`BufferedPainting` `Painter` `ThemeData`) each verified
  by a repo-wide search for the class name and for `new_ref<` of it; the only hits
  are the files themselves, `gui/CMakeLists.txt`, and this documentation.
- Second re-verification pass (same session) corrected: the `themedata` row of the
  files table to **264** commented lines (42 + 222, not "~270"); the colours range to
  `:118`–`148` with bounds `:149`–`150` and hue pairs to `:151`–`160` (the 1601–1631
  block runs 31 lines, not 21); gotcha 3 to name `CaptionFont` as a *static data member*
  rather than a free constant; and gotcha 8 to cite the `WindowController` pair's
  **definitions** `windowcontroller.cpp:517`–`523` / `:535`–`538` rather than only the
  header declarations. `WindowController::BeginPaint` returns `DeviceContext*`
  (`:517`) and `EndPaint` returns `bool` (`:535`) — both confirmed against body text.








