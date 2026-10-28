# 60 — GDI objects: device contexts, drawing surfaces, pens, fonts, regions

**Status: detailed — 60a + 60b + 60c = all 32 files (16 pairs).** The 60a pass
read `gdiobject.h`/`.cpp`, `devicecontext.h`/`.cpp` and `memdevicecontext.h`/`.cpp`
in full, plus consumer edges (`perthread`, `windowcontroller`, `wglcontext`,
`mainwindow`) and the `add_library` list. The 60b pass read the four metafile
pairs (`metadc` `enhmetadc` `metafile` `enhmetafile`) in full, their one consumer
edge (`staticcontroller`), the handle machinery they sit on (`handle.h`/`.cpp`,
`constants.h`, `newref.h`) and the `api_layer/gdi` metafile block they mirror.
The 60c pass read the nine resource-leaf pairs (`bitmap` `brush` `colourspace`
`cursor` `font` `icon` `palette` `pen` `region`) in full — 767 header lines +
1,027 body lines = 1,794 — plus the same consumer edges and the `Handle`/`GDIObject`
contract they sit on. A 27 Oct 2026 re-verification pass over all 18 files then
corrected six off-by-one citations on this page (see the corrected `//All types…`
comment lines, `GetBrushOrgEx` at `brush.cpp:84`, `menu.h:34`, `caret.h:42` ·
`caret.cpp:37`, `windowclass.cpp:140`) and restated the `User32`/`GDI32` split of
the 77-symbol call map. Everything under "What is known from call sites" is pre-walk
inference and is corrected by 60a/60b/60c below where they disagree — 60b in
particular corrects the "metafile/off-screen family" framing: the family wraps
no GDI call at all.

## Files in this part (32 — 16 pairs)

| Pair | Sizes (h / cpp) | |
|---|---|---|
| `devicecontext` | 19.9 / 35.3 KB | biggest pair in the library |
| `bitmap` | 4.1 / 2.9 KB | |
| `brush` | 3.1 / 4.6 KB | |
| `colourspace` | 3.1 / 3.1 KB | |
| `cursor` | 3.1 / 5.5 KB | |
| `enhmetadc` | 2.2 / 2.1 KB | |
| `enhmetafile` | 2.0 / 1.9 KB | |
| `font` | 3.8 / 3.3 KB | |
| `gdiobject` | 2.5 / 2.4 KB | likely the common base |
| `icon` | 4.1 / 5.6 KB | |
| `memdevicecontext` | 2.1 / 2.0 KB | |
| `metadc` | 2.1 / 2.0 KB | |
| `metafile` | 1.8 / 1.7 KB | |
| `palette` | 3.1 / 4.0 KB | |
| `pen` | 2.5 / 3.3 KB | |
| `region` | 3.5 / 5.3 KB | |

## What is known from call sites

**Device context.** `PerThread` keeps `std::map<void*, ref_of<DeviceContext>>`
(`perthread.h`) and fabricates wrappers for foreign handles with
`new_ref<DeviceContext>(PrimitiveHandle(hDC), false)`
(`perthread.cpp:171`–`176`) — the second argument is the "do not close" flag.
The controller surface is `GetDC()`, `GetADC()`,
`GetADC(ref_of<Region>, unsigned flags)`, `ReleaseDC(ref_of<DeviceContext>)`
(`windowcontroller.h:241`, `252`–`254`). The example uses
`controller->GetADC(updateRegion, DCXWindow|DCXIntersectRegion)`
(`mainwindow.cpp:80`) and `controller->GetDC()` (`mainwindow.cpp:84`).

**Region.** `ref_of<Region>::type updateRegion = new_ref<Region>(PrimitiveHandle((void*)(wParam)), false)`
(`mainwindow.cpp:79`) — a region is a handle wrapper, constructed from the
`wParam` of a `wmPaint`/`wmEraseBackground` message. Controller surface:
`GetUpdateRgn`, `GetRgn`, `SetRgn`, `InvalidateRgn`, `ValidateRgn`, `Redraw`
(`windowcontroller.h:240`, `242`–`247`, `248`, `244`–`246`).

**Brush / Icon / Cursor / Font are held by `WindowClass`**
(`windowclass.h:31`–`34` includes; accessors `BackgroundBrush()`, `Icon()`,
`Cursor()`, `SmallIcon()` at `windowclass.h:53`–`58` and their setters
`:66`–`70`). So the GDI part supplies the resources a window class carries.

**Paint session.** `WindowController::BeginPaint(PaintStruct&)` returns
`DeviceContext*` and `EndPaint(const PaintStruct&)` closes it
(`windowcontroller.h:235`, `238`) — the Owner Draw / Custom Draw pair.

`gdiobject.h:52` declares `class qor_pp_module_interface(QOR_WINGUI) GDIObject`
— and it **is** the common base: `Bitmap : public GDIObject` (`bitmap.h:81`),
`Brush : public GDIObject` (`brush.h:44`), `Region : public GDIObject`
(`region.h:52`), `Font : public GDIObject` (`font.h:82`),
`Palette : public GDIObject` (`palette.h:51`), `Pen : public GDIObject`
(`pen.h:43`). Confirmed pairs and their bases:

| Type | Base | Source |
|---|---|---|
| `Bitmap` | `GDIObject` | `bitmap.h:81` |
| `DIB` | `Bitmap` | `bitmap.h:90` |
| `DIBSection` | `Bitmap` | `bitmap.h:98` |
| `Brush` | `GDIObject` | `brush.h:44` |
| `SysBrush` | `Brush` | `brush.h:71` |
| `Region` | `GDIObject` | `region.h:52` |
| `Font` | `GDIObject` | `font.h:82` |
| `Palette` | `GDIObject` | `palette.h:51` |
| `Pen` | `GDIObject` | `pen.h:43` |

Supporting Win32 record structs seen in the same scan: `RGBQUAD`,
`BitmapInfoHeader`, `BitmapInfo` (`bitmap.h:37`/`45`/`60`); `LogBrush`
(`brush.h:37`); `RgnDataHeader`, `RgnData` (`region.h:37`/`46`); `LogFont`,
`EnumLogFontEx`, `DesignVector`, `EnumLogFontExDV` (`font.h:43`/`61`/`69`/`76`);
`PaletteEntry`, `LogPalette` (`palette.h:36`/`44`); `LogPen` (`pen.h:36`).
`bitmap.h:79` and `brush.h`/`region.h` forward-declare `DeviceContext`, so the
part is internally cyclic — draw order when reading is
`gdiobject.h` → `devicecontext.h` → the rest.

## Why this part matters for correctness

Per `purpose.md:11`–`13`, GDI objects are **bound to the thread** that owns the
window: a `DeviceContext` obtained on one thread must not be used on another.
That is the reason `PerThread` — not a global registry — owns the
handle→object maps. Any future multi-threaded UI code must not share these
objects.

## Questions for the detail pass

1. `DeviceContext`'s full surface (35 KB of `.cpp`) — which GDI calls it wraps,
   and how it maps onto `qor::win::api::Gdi32`.
2. `memdevicecontext`, `metadc`, `enhmetadc`, `enhmetafile`, `metafile` —
   the metafile/off-screen family; how off-screen rendering composes to the GL
   target window (the purpose statement's "Off Screen Rendering with
   Composition to an Open GL target window").
3. `Bitmap` / `DIB` / `DIBSection` — expected members; confirm they exist
   (the inventory shows only `bitmap.h/.cpp`, so the DIB types are inside it).
4. Close semantics: which objects set `m_needsClose` and which call
   `DontClose()` — the handle discipline from
   [part 10](10-core-root.md#construction-and-handles).

Questions 1 and 4 above are answered by the 60a walk below; **question 2 is
answered by the 60b walk below** (the answer is "it does not" — see
[60b](#60b--the-metafile-family-metadc-enhmetadc-metafile-enhmetafile-detailed));
3 is 60c work.

## 60a — core object model: `gdiobject`, `devicecontext`, `memdevicecontext` (detailed)

**6 of 32 files read in full.** This is the split walk: the trio is the object
model everything else in the part sits on, and `devicecontext.cpp` at 35 KB is
the largest file in the whole library.

### File sizes (lines, h / cpp)

| File | Lines | |
|---|---|---|
| `gdiobject.h` | 71 | `gdiobject.cpp` 61 |
| `memdevicecontext.h` | 48 | `memdevicecontext.cpp` 47 |
| `devicecontext.h` | 438 | `devicecontext.cpp` **899** |

Line counts are the `Get-Content` counts for each file, so the citations below
are stable for this tree.

### Level 1 — the base class (`gdiobject.h` / `gdiobject.cpp`)

`gdiobject.h:25`–`26` guard `QOR_PP_H_WINDOWS_GUI_GDIOBJECT`; the only include
is `src/platform/os/windows/common/handles/handle.h` (`:28`). Comment at
`gdiobject.h:30`: "All types on this interface must be portable" — the same
comment heads every header in this part.

`enum GDIObjectType` at `gdiobject.h:33`–`50` has **15 enumerators**: `OBitmap`
`OBrush` `OColourSpace` `ODeviceContext` `OEnhMetaDC` `OEnhMetaFile` `OExtPen`
`OFont` `OMemDC` `OMetaFile` `OMetaDC` `OPalette` `OPen` `ORegion` `OUnknown`.
Thirteen name a pair that exists in this directory; `OExtPen` and `OUnknown`
name none. There is **no enumerator for `Cursor` or `Icon`** even though
`cursor.h`/`cursor.cpp` and `icon.h`/`icon.cpp` exist — a gap to settle when
60c reads those pairs.

The class at `gdiobject.h:52` is `class qor_pp_module_interface(QOR_WINGUI)
GDIObject` with:

- `GDIObject(GDIObjectType)` and `GDIObject(const PrimitiveHandle&, GDIObjectType, bool takeOwnership)` (`:56`, `:57`), `virtual ~GDIObject()` (`:58`), `const Handle& GetHandle() const` (`:60`);
- protected data `Handle m_handle` `GDIObjectType m_objType` `bool m_owner` (`:64`–`66`).

The `.cpp` (`gdiobject.cpp:37`–`59`) defines exactly those four:

- `:37` default ctor → `m_handle(0)`, `m_owner(true)` — an object that will own
  the handle it goes on to create;
- `:41`–`45` handle ctor → `m_handle(h.Use())`, `m_owner(takeOwnership)`,
  then `m_handle.DontClose();` at `:44`;
- `:47`–`54` dtor → `if(m_owner && !m_handle.IsInvalid()) { GDI32::DeleteObject(m_handle.Use()); m_handle.Drop(); }`;
- `:56`–`59` `GetHandle()` returns `m_handle`.

`gdiobject.cpp:25`–`30` includes `configuration.h`, `gdiobject.h`,
`common/stringconv.h`, `api_layer/user/user32.h`, `api_layer/gdi/gdi32.h`, with
`using namespace qor::win::api;` at `:32`. The only API used is `GDI32::DeleteObject`
— `stringconv.h` and `user32.h` are dead includes in this TU.

### Level 2 — the close discipline, grounded in `Handle`

The GDI part reuses the generic `Handle` wrapper from
`src/platform/os/windows/common/handles`. What the flags actually do:

| `Handle` member | Lines | Behaviour |
|---|---|---|
| `~Handle()` | `handle.cpp:89`–`96` | `Detach(); if (m_needsClose) Close();` |
| `DontClose()` | `handle.cpp:178`–`181` | `m_needsClose = false;` |
| `Close()` | `handle.cpp:199`–`207` | `Kernel32::CloseHandle(m_h)` when `m_needsClose` |
| `IsInvalid()` | `handle.cpp:214`–`217` | `m_h == Invalid_Handle_Value` |
| `Drop()` | `handle.cpp:219`–`223` | `Detach(); m_h = Invalid_Handle_Value;` |

So `GDIObject(const PrimitiveHandle&, …)` calls `DontClose()` (`gdiobject.cpp:44`)
and the `Handle` destructor never closes: `Close()` would send an HDC or `HGDIOBJ`
to `Kernel32::CloseHandle`, which is never reached for a GDI object. The Win32
close step is entirely the QOR object destructor's job — `~GDIObject()`
(`gdiobject.cpp:47`–`54`) is the only place a GDI handle is closed.

`gdiobject.h:57` declares the three-argument ctor with **no default**; the
default `bool takeOwnership = true` appears only on the **definition** at
`gdiobject.cpp:41`. A default written on a definition is not visible to callers
that include the header, so no caller in the repo can omit the flag — the
default is dead. Every construction site in the tree passes it explicitly
(`perthread.cpp:173`, `devicecontext.cpp:81`).

### `memdevicecontext` — a marker type with no surface

`memdevicecontext.h:38` `class MemDeviceContext : public GDIObject`, declaring
**only** `MemDeviceContext()`, `MemDeviceContext(const PrimitiveHandle&, bool)`
and `virtual ~MemDeviceContext()` (`:41`–`43`). It adds no method of any kind —
no `GetHandle()` override (it inherits the base's, which is non-virtual), no
factory, no data. The `.cpp` mirrors that: `:35` delegates to `GDIObject(OMemDC)`,
`:39` to `GDIObject(h, OMemDC, takeOwnership)`, and `:43`–`45` is an **empty**
destructor body, so `~GDIObject()` still runs after it and deletes the object if
`m_owner`.

Repo-wide, `MemDeviceContext` is **never constructed**: the only hits are its own
pair, `gui/CMakeLists.txt:50` (`add_library`), and this documentation. The TU is
compiled and linked and nothing in the library or the example can reach it. The
"Off Screen Rendering" of the purpose statement (`gui/purpose.md:7`) is therefore
not built on `MemDeviceContext` — the live path is `DeviceContext::CreateForDIB()`
(`devicecontext.cpp:79`–`82`) plus `DIBSection`, as part 90 records.

`memdevicecontext.h:28`–`33` includes `handle.h`, `../view/drawing/rect.h`,
`brush.h`, `../view/drawing/size.h`, `palette.h`, `gdiobject.h`. Only
`handle.h` and `gdiobject.h` are needed; `rect.h` `brush.h` `size.h` `palette.h`
are copy-paste from `devicecontext.h` (`:28`–`35`) and nothing in this class
names those types. Note the two include spellings in the same statement list:
absolute paths for shared headers, relative (`brush.h`, `../view/drawing/…`) for
part-local ones.

### `devicecontext.h` — the header shape (438 lines)

`devicecontext.h:25`–`26` guard; includes `:28`–`35`: `handle.h`,
`common/structures.h`, `../view/drawing/rect.h`, `brush.h`,
`../view/drawing/size.h`, `palette.h`, `font.h`, `gdiobject.h` — all live, the
class methods take `const Rect&` `const Brush&` `Palette&` `Size&`
(`:252`–`275`, `:399`–`404`).

Before the class, **16 hand-mirrored Win32 record structs** (`^    struct ` count
= 16) and 4 `typedef`s:

| Struct | Lines | Mirrors |
|---|---|---|
| `ColourAdjustment` | `:40`–`54` | `COLORADJUSTMENT` |
| `BlendFunction` | `:56`–`62` | `BLENDFUNCTION` |
| `Abc` / `AbcFloat` | `:64`–`69` / `:71`–`76` | `ABC`, `ABCFLOAT` |
| `GCPResults` | `:78`–`89` | `GCP_RESULTS` |
| `CharRange` / `GlyphSet` | `:91`–`95` / `:97`–`104` | `CHAR_RANGE`, `GLYPHSET` |
| `GlyphMetrics` | `:106`–`113` | `GLYPHMETRICS` |
| `Fixed` | `:115`–`124` | `FIXED16_16`, with a `#ifndef _MAC` field-order flip (`:117`–`123`) — the `_MAC` branch is dead on MSVC |
| `Mat2` | `:126`–`132` | `MAT2` |
| `KerningPair` | `:134`–`139` | `KERNINGPAIR` |
| `Panose` | `:141`–`153` | `PANOSE` |
| `TextMetric` | `:155`–`177` | `TEXTMETRIC` |
| `OutlineTextMetric` | `:179`–`213` | `OUTLINETEXTMETRIC` |
| `FontSignature` | `:215`–`219` | `FONTSIGNATURE` |
| `PolyText` | `:221`–`230` | `POLYTEXT` |

Then 4 `typedef`s of `__stdcall` callback procs at `:231`–`235`
(`IcmEnumProc` `GObjEnumProc` `LineDDAProc` `FontEumProc` — the last name is
misspelled, "Eum"). They exist so a wrapper method can take a callable argument
and `reinterpret_cast` it back to the SDK proc type at the call site
(`devicecontext.cpp:245` `:423` `:545` `:630` `:636` `:641`).

### The class declaration and the 164-method map

`devicecontext.h:237` `class qor_pp_module_interface(QOR_WINGUI) DeviceContext
: public GDIObject`; body braces `:238`–`434`; public `:239`–`428`; protected
`bool m_mustBeReleased;` (`:430`); private `Handle m_hWnd;` (`:433`).

**164 method declarations** in the body and **164 definitions** in the `.cpp` —
a 1:1 map, no declared-but-undefined name and no defined-but-undeclared one.
(Counting `;`-terminated lines in the body region reports 176; the extra 12 are
the commented-out block at `:406`–`420`.) That block is the part's own
"not wrapped" list, 12 GDI font/EUDC entry points: `GetRasterizerCaps`
`TranslateCharsetInfo` `GetFontResourceInfo` `RemoveFontMemResourceEx`
`RemoveFontResourceT` `RemoveFontResourceExT` `AddFontMemResourceEx`
`AddFontResourceT` `AddFontResourceExT` `AddFontResourceTracking`
`CreateScalableFontResourceT` `EnableEUDC`.

### `devicecontext.cpp` — the 899-line surface, by group

Every method is a one-line forward over `(HDC)(m_handle.Use())`. Prefix mentions
across the file: `GDI32::` on 147 lines, `User32::` on 10, `WGL::` on 5.

| Group | Definition lines | Forwards to | |
|---|---|---|---|
| identity, close, factories | `:37`–`87` | `User32::GetDC` `GetDCEx` `ReleaseDC` | ctors, dtor, `MustRelease`, `GetHandle`, `FromWindow` ×2, `CreateForDIB`, `Release` |
| blit, rects, patterns | `:89`–`112` | `GDI32::BitBlt` `PatBlt`; **`User32::FillRect` `FrameRect` `InvertRect`** at `:96` `:101` `:106` | |
| clip + meta regions | `:114`–`177` | `GDI32::` `ExcludeClipRect` `ExtSelectClipRgn` `GetClipBox` `GetClipRgn` `GetMetaRgn` `SetMetaRgn` `GetRandomRgn` `IntersectClipRect` `OffsetClipRgn` `PtVisible` `RectVisible` `SelectClipPath` `SelectClipRgn` | |
| colour, palette, ICM, gamma | `:179`–`281` | `GDI32::` `GetColorAdjustment` … `SetMagicColors` | `SelectPalette` `:204`–`210` returns a raw `Handle` with `h.DontClose()` |
| coordinate systems | `:283`–`381` | `GDI32::` `DPtoLP` `LPtoDP` `*Viewport*` `*Window*` `WorldTransform` | `GetworldTransform` at `:313` (header `:296`) — lowercase `w` |
| DC lifecycle, GDI meta API | `:383`–`516` | `GDI32::CreateCompatibleDC` `CreateDCT` `DeleteDC` `CancelDC` `CreateICT` `DeleteObject` `DrawEscape` `EnumObjects` `GetCurrentObject` `GetStockObject` `ResetDCT` `SaveDC` `RestoreDC` `SelectObject` … | `CreateCompatible` `:383`–`387` owner=true |
| drawing primitives | `:518`–`626` | `GDI32::AlphaBlend` `AngleArc` `Arc` `ArcTo` `LineDDA` `LineTo` `MoveToEx` `PolyBezier` … `RoundRect` | |
| text + font metrics | `:628`–`842` | `GDI32::Enum*` `ExtTextOutT` `Get*Width*` `GetGlyphOutlineT` `TextOutT` …; **`User32::DrawTextT` `DrawTextExT` `GetTabbedTextExtentT` `TabbedTextOutT`** at `:826` `:831` `:836` `:841` | |
| region fill/frame | `:844`–`872` | `GDI32::FillRgn` `FrameRgn` `GetPolyFillMode` `InvertRgn` `PaintRgn` `SetPolyFillMode` | |
| pixel format + GL | `:874`–`897` | **`WGL::` ×5** — `ChoosePixelFormat` `DescribePixelFormat` `GetPixelFormat` `SetPixelFormat` `SwapBuffers` | the only use of the `opengl.h` include at `devicecontext.cpp:31` |

The `User32::` and `WGL::` rows are the point of the table: the wrapper class is
named for GDI but 15 of its forwards bind to `user32.dll` and 5 to the WGL layer,
which is why `devicecontext.cpp:29` includes `user32.h` and `:31` includes
`opengl.h`. The WGL five are how an HDC gets a pixel format at all, and they are
the bridge to `WGLContext` in part 10 — part 90 shows the example calling
`m_dcDIB->ChoosePixelFormat(&pfd)` (`mainwindow.cpp:301`) then
`new_ref<WGLContext>(m_dcDIB)` (`mainwindow.cpp:313`).

### Construction and the close path

Five ways a `DeviceContext` comes into being, and their ownership flags:

| Factory | Lines | `takeOwnership` | `MustRelease()` |
|---|---|---|---|
| `FromWindow(hWnd)` | `devicecontext.cpp:63`–`69` | `false` | yes (`:67`), and sets `dc.m_hWnd = hWnd` (`:66`) |
| `FromWindow(hWnd, hRgnClip, flags)` | `:71`–`77` | `false` | yes (`:75`), sets `m_hWnd` (`:74`) |
| `CreateForDIB()` | `:79`–`82` | **`true`** | no |
| `CreateCompatible(dc)` | `:383`–`387` | **`true`** | no |
| `PerThread::DeviceContextFromHandle(hDC)` | `perthread.cpp:173` | `false` | set later by the caller |

`CreateForDIB()` is `new_ref<DeviceContext>(PrimitiveHandle(GDI32::CreateCompatibleDC(nullptr)), true)`
— `nullptr` display handle, so Win32 picks any display: a memory DC.
`Create` (`:389`–`393`, `GDI32::CreateDCT`) and `CreateICT` (`:405`–`409`) are the
printer/ICM counterparts and are never called anywhere. All six factories —
`FromWindow` ×2 and `CreateForDIB` at `devicecontext.h:244`–`246`,
`CreateCompatible` `Create` `CreateICT` at `:310`–`314` — are declared `static`,
which is why `mainwindow.cpp:270` calls `DeviceContext::CreateForDIB()` with no
instance.

The destructor chain is the whole close story. `~DeviceContext()`
(`devicecontext.cpp:45`–`51`) does `if(m_mustBeReleased) Release();` and `Release()`
(`:84`–`87`) is `User32::ReleaseDC((HWND)(m_hWnd.Use()), (HDC)(m_handle.Use()))`.
After that body, `~GDIObject()` (`gdiobject.cpp:47`–`54`) runs and, if
`m_owner && !m_handle.IsInvalid()`, calls `GDI32::DeleteObject` and `Drop()`s the
handle. Neither body clears the other's view of the handle, so a DC that is both
owner and `MustRelease` is closed twice, by two different entry points.

`m_mustBeReleased` has exactly one writer besides the ctors — `MustRelease()`
(`:53`–`56`) — and exactly one reader, the dtor at `:47`. Its call sites repo-wide
are `devicecontext.cpp:67` `:75` (inside the uncalled `FromWindow` overloads) and
`windowcontroller.cpp:557` `:619` `:631`. `m_hWnd` has exactly two writers,
`devicecontext.cpp:66` `:74`, both inside those same `FromWindow` overloads, and
one reader, `Release()` at `:86`.

### Consumers — who reaches this trio

- **`perthread.h:41`** includes `gdiobjects/devicecontext.h`; `:97` declares
  `std::map<void*, ref_of<DeviceContext>::type> m_deviceContextHandleMap;`.
  `perthread.cpp:141`–`150` `AddDeviceContext`, `:152`–`161` `RemoveDeviceContext`,
  `:163`–`176` `DeviceContextFromHandle`. **`AddDeviceContext` and
  `RemoveDeviceContext` have no call sites repo-wide**, so the map is always
  empty: the lookup at `perthread.cpp:166`–`169` never hits, and every call
  fabricates a fresh `ref_of<DeviceContext>` with `takeOwnership = false`
  (`perthread.cpp:173`). Two `GetDC()` calls on the same HDC produce two distinct
  objects.
- **`windowcontroller.cpp:550`–`560`** `GetDC()` → `User32::GetWindowDC` (`:553`)
  → `m_perThread->DeviceContextFromHandle(hdc)` (`:556`) → `dc->MustRelease()`
  (`:557`). Same shape at `:612`–`622` (`GetADC()`, `:619`) and `:624`–`634`
  (`GetADC(rgnClip, flags)`, `:631`). `WindowController::ReleaseDC`
  (`:636`–`639`) calls `User32::ReleaseDC(WINDOW_HANDLE, dc->GetHandle().Use())`
  — it bypasses `DeviceContext::Release()` and the `m_hWnd` it carries.
- **`mainwindow.cpp`** is the only live consumer: `:189` declares
  `ref_of<DeviceContext>::type m_dcDIB;`, `:270` `m_dcDIB = DeviceContext::CreateForDIB();`,
  `:265`–`268` `m_dcDIB.Dispose()` before re-creating, `:286`
  `m_dcDIB->SelectObject(m_bmpDIB->GetHandle())`, `:301`/`:307` the WGL pixel-format
  pair, `:313` `new_ref<WGLContext>(m_dcDIB)`, `:247`
  `dest.BitBlt(0, 0, m_width, m_height, m_dcDIB()(), 0, 0, SrcCopy)`,
  `:161` `guiThread->SetCurrentWGLContext(m_dcDIB, m_context)`.
  `wglcontext.h` takes `ref_of<DeviceContext>::type` — part 10's `WGLContext` is
  the consumer of this type.

### Build facts

All three `.cpp` are in `add_library` at `gui/CMakeLists.txt:44`
(`gdiobjects/devicecontext.cpp`), `:48` (`gdiobjects/gdiobject.cpp`),
`:50` (`gdiobjects/memdevicecontext.cpp`). The whole part is the block
`gui/CMakeLists.txt:40`–`55` — 16 `.cpp` for 16 pairs, none absent.

### 60a gotchas

1. **The per-thread registry is dead.** No caller of `AddDeviceContext`
   / `RemoveDeviceContext` exists, so `m_deviceContextHandleMap` is always empty
   and `DeviceContextFromHandle` fabricates a new wrapper per call. The
   thread-bound-objects design in `gui/purpose.md:11`–`13` is only half wired:
   the map exists, nothing populates it.
2. **`m_hWnd` is never set on any live path.** Its only writers are the two
   `FromWindow` overloads, which have no callers. So every `Release()` passes
   `m_hWnd` = 0, i.e. `User32::ReleaseDC(NULL, hdc)`. Windows API intent:
   `ReleaseDC` needs the window handle that owns the DC; the observation here is
   only that the argument is 0.
3. **Two owners of the close step.** For an owner-and-`MustRelease` DC the derived
   dtor `Release()`s and the base dtor then `GDI32::DeleteObject`s the same
   handle. Conversely `CreateForDIB`/`CreateCompatible` set `owner = true`,
   `mustBeReleased = false`: their DC is closed only by `DeleteObject`, where the
   Windows API pairing for a `CreateCompatibleDC` result is `DeleteDC`
   (wrapped at `devicecontext.cpp:395`–`398` but never called).
4. **`takeOwnership`'s default is unreachable** — declared without it
   (`gdiobject.h:57`), defaulted only on the definition (`gdiobject.cpp:41`).
5. **`LineDDA` forwards no device context.** `devicecontext.cpp:543`–`546` calls
   `GDI32::LineDDA(xStart, yStart, xEnd, yEnd, proc, data)` — six arguments, and
   `api_layer/gdi/gdi32.h:191` declares `LineDDA(int nXStart, int nYStart, int nXEnd,
   int nYEnd, ::LINEDDAPROC, LPARAM)`. The api_layer signature has no HDC slot at
   all, so the wrapper cannot target `m_handle` and the x coordinate lands where an
   HDC belongs. Windows API intent: `LineDDA(HDC, int, int, int, int, LINEDDAPROC, LPVOID)`.
6. **`UnrealizeObject` ignores the DC.** `devicecontext.cpp:222`–`226` forwards
   `GDI32::UnrealizeObject((HGDIOBJ)(hgdiobj.Use()))` — no HDC argument — under the
   author's own note `//TODO: Move this to GDIObject` at `:222`. It belongs on the
   base class and was left on `DeviceContext`.
7. **Naming slips carried into the surface:** `GetworldTransform`
   (`devicecontext.h:296`, `.cpp:313`), `FontEumProc` (`devicecontext.h:235`),
   `memdevicecontext.h`'s four unused includes.
8. **`GDIObjectType` has no `Cursor`/`Icon` enumerator** while both pairs exist —
   to be settled by 60c.

### Still to cover

- **60b — the metafile family** (8 files): `metadc` `enhmetadc` `metafile`
  `enhmetafile` — **done; the 60b section follows below.** Question 2 is
  answered there: the family carries no GDI call, so it is not the off-screen
  path; `DIBSection` (60c) stays the only live off-screen route.
- **60c — the resource leaves** (18 files): `bitmap` (`Bitmap` `DIB`
  `DIBSection` — question 3), `brush` `colourspace` `cursor` `font` `icon`
  `palette` `pen` `region` — **done; the 60c section follows below**, and it
  settles both the `GDIObjectType` each leaf passes and the `Cursor`/`Icon`
  question (they pass none — neither derives from `GDIObject`).

## 60b — the metafile family: `metadc` `enhmetadc` `metafile` `enhmetafile` (detailed)

**8 of 32 files read in full** — 354 lines across the four pairs, the smallest
quarter of this part (`devicecontext.cpp` alone is 899). The walk also read the
base these sit on (`gdiobject.h`/`.cpp`, covered by 60a), the handle machinery
behind it (`handle.h:9`–`47`, `handle.cpp`, `constants.h:8`), `newref.h:18`–`28`,
the one consumer (`staticcontroller.h`/`.cpp`) and the api layer this family
mirrors (`api_layer/gdi/gdi32.h:265`–`291`, `api_layer/gdi/metafile.cpp`).

### File sizes (lines, h / cpp)

| Pair | `.h` | `.cpp` |
|---|---|---|
| `metadc` | 48 | 47 |
| `enhmetadc` | 48 | 47 |
| `metafile` | 41 | 36 |
| `enhmetafile` | 48 | 39 |

The four pairs are byte-identical modulo two words: `metadc.h` and `enhmetadc.h`
are the same 48 lines (`:25`–`26` guard, `:28`–`33` include block, `:38`–`44`
class), as are `metadc.cpp` and `enhmetadc.cpp` (same 47 lines, `:25`–`29`
include block, `:35`/`:39`/`:43`–`45` definitions). Only `metafile` and
`enhmetafile` differ from that shape.

### Level 1 — what each pair names in Win32

Each pair is a `GDIObject` subclass tagged with one `GDIObjectType`
enumerator. The enumerator each passes, and the Win32 handle it is meant to
name (that column is **Windows API intent**, not observed behaviour):

| Class | Declared | Enumerator passed | Win32 handle (Windows API intent) |
|---|---|---|---|
| `MetaDeviceContext` | `metadc.h:38` | `OMetaDC` (`gdiobject.h:45`) | `HDC` from `CreateMetaFile` (`gdi32.h:271`) |
| `EnhancedMetaDeviceContext` | `enhmetadc.h:38` | `OEnhMetaDC` (`gdiobject.h:39`) | `HDC` from `CreateEnhMetaFile` (`gdi32.h:270`) |
| `MetaFile` | `metafile.h:34` | `OMetaFile` (`gdiobject.h:44`) | `HMETAFILE` from `GetMetaFile` (`gdi32.h:291`) |
| `EnhancedMetaFile` | `enhmetafile.h:34` | `OEnhMetaFile` (`gdiobject.h:40`) | `HENHMETAFILE` from `GetEnhMetaFile` (`gdi32.h:277`) |

The four enumerators sit in the enum's alphabetical run
(`gdiobject.h:39`–`40`, `:44`–`45`). Three of them are used exactly once in the
whole tree — each in its own pair's handle ctor (`metadc.cpp:39`,
`enhmetadc.cpp:39`, `enhmetafile.cpp:35`) — and `OMetaFile` is used **zero
times**, because `MetaFile` declares no ctor to pass it in.

The mirror this family was meant to fill is complete and unused. `gdi32.h:265`
carries the comment `//MetaFile and EnhMetaFile functions` and `:266`–`291`
declares 26 entry points (`CloseEnhMetaFile` `:266`, `CloseMetaFile` `:267`,
`CopyEnhMetaFile` `:268`, `CopyMetaFile` `:269`, `CreateEnhMetaFile` `:270`,
`CreateMetaFile` `:271`, `DeleteEnhMetaFile` `:272`, `DeleteMetaFile` `:273`,
`EnumEnhMetaFile` `:274`, `EnumMetaFile` `:275`, `GdiComment` `:276`,
`GetEnhMetaFile` `:277`, … `GetMetaFile` `:291`); `api_layer/gdi/metafile.cpp`
defines exactly 26 `GDI32::` wrappers for them. **Not one of the 26 is called
from any GUI TU**: a repo-wide search for those names hits only
`api_layer/gdi/*`, `api_layer/opengl/*`, the `#undef` at
`staticcontroller.cpp:35`, and this documentation.

### Level 2 — the shape of each pair

**`metadc.h` / `enhmetadc.h` — ctor-only shells, no surface at all.**
`metadc.h:38`–`44` declares `MetaDeviceContext` with exactly three members: the
default ctor (`:41`), `MetaDeviceContext(const PrimitiveHandle& h, bool
takeOwnership)` (`:42`) and `virtual ~MetaDeviceContext()` (`:43`). `enhmetadc.h`
is the same with `EnhancedMetaDeviceContext` (`:38`–`44`). Nothing else — no
factory, no accessor, no data. The `.cpp` mirrors that exactly: `:35`
`MetaDeviceContext::MetaDeviceContext() : GDIObject(OMetaDC)`, `:39`
`… (const PrimitiveHandle& h, bool takeOwnership) : GDIObject(h, OMetaDC,
takeOwnership)`, `:43`–`45` an **empty** destructor body — so every object's
whole behaviour is 60a's base: `GDIObject`'s two ctors (`gdiobject.cpp:37`/
`:41`–`45`) and `~GDIObject()` (`gdiobject.cpp:47`–`54`).

**`metafile.h` / `metafile.cpp` — an empty class and an empty TU.**
`metafile.h:34`–`37` declares `class MetaFile : public GDIObject` with an empty
body: no ctor, no dtor, no member. `metafile.cpp:33`–`36` is an empty namespace
block — the TU defines nothing at all, yet it is compiled and linked from
`add_library` at `gui/CMakeLists.txt:52`. `metafile.h:28`–`29` also carries the
minimal include pair (`handle.h`, `gdiobject.h`) — the shape the two DC headers
should have.

**`enhmetafile.h` / `enhmetafile.cpp` — one ctor, one stray member.**
`enhmetafile.h:34`–`44`: `explicit EnhancedMetaFile(const PrimitiveHandle& h,
bool own = true)` at `:38` (public section `:36`), then `protected: PrimitiveHandle
m_handle;` at `:42`. The single definition `enhmetafile.cpp:35`
`EnhancedMetaFile::EnhancedMetaFile(const PrimitiveHandle& h, bool own) :
GDIObject(h,OEnhMetaFile,own)` never initialises that `m_handle` and nothing
reads it — see the gotcha below. No default ctor, no declared destructor, so
`~GDIObject()` is the dtor.

`explicit` and the default argument both matter: `own = true` is written on the
**declaration**, so unlike 60a gotcha 4 (`gdiobject.h:57` declares no default,
`gdiobject.cpp:41` defaults it) this default *is* reachable —
`EnhancedMetaFile f(h);` compiles and takes ownership.

### Construction and close semantics (inherited from 60a's base)

- **Handle ctor.** `GDIObject(h, type, own)` (`gdiobject.cpp:41`–`45`) stores
  `m_handle(h.Use())` then calls `m_handle.DontClose()` (`:44`), clearing
  `m_needsClose` (`handle.cpp:178`–`181`), so `~Handle()` (`handle.cpp:89`–`96`)
  closes nothing. The Win32 close step is the object destructor's job alone.
- **Close step.** `~GDIObject()` (`gdiobject.cpp:47`–`54`) —
  `if (m_owner && !m_handle.IsInvalid()) GDI32::DeleteObject(m_handle.Use());`
  then `m_handle.Drop()` (`handle.cpp:219`–`223`). `DeleteObject` is declared at
  `gdi32.h:165` and wrapped at `api_layer/gdi/devicecontext.cpp:51`–`54`.
- **Wrong pairing for this whole family.** Windows API intent: a metafile
  `HDC` from `CreateMetaFile`/`CreateEnhMetaFile` is closed by
  `CloseMetaFile`/`CloseEnhMetaFile` (`gdi32.h:267`/`:266`), and
  `HMETAFILE`/`HENHMETAFILE` released by `DeleteMetaFile`/`DeleteEnhMetaFile`
  (`gdi32.h:273`/`:272`). The QOR close step is one call for all four kinds —
  `DeleteObject` — and none of those four entry points appears anywhere in the
  tree.
- **Default-constructed objects are destructor-poisoned.**
  `MetaDeviceContext()` (`metadc.cpp:35`) and
  `EnhancedMetaDeviceContext()` (`enhmetadc.cpp:35`) route to
  `GDIObject(GDIObjectType)` (`gdiobject.cpp:37`): `m_handle(0)`,
  `m_owner(true)`. `Handle(int h)` (`handle.cpp:87`) sets `m_h = (void*)0`;
  `IsInvalid()` (`handle.cpp:214`–`216`) compares with
  `Invalid_Handle_Value` = `((void* const)(size_t)(-1))` (`constants.h:8`), so a
  **null handle is not invalid** and the destructor runs
  `GDI32::DeleteObject((HGDIOBJ)0)` before `Drop()`. The same latent trap exists
  in 60a's `MemDeviceContext()` — 60a did not record it.

### Consumers — one include, and it is dead too

**The only include of any of the four headers outside its own `.cpp` is
`staticcontroller.h:32` `#include "../../gdiobjects/enhmetafile.h"`.** `metadc.h`
is included by `metadc.cpp:27` only, `enhmetadc.h` by `enhmetadc.cpp:27`,
`metafile.h` by `metafile.cpp:27` — the other three headers are invisible to the
rest of the library. A repo-wide search for `MetaDeviceContext`,
`EnhancedMetaDeviceContext` and `MetaFile` across `src` and `examples` returns
nothing outside their own pairs and this documentation.

**`EnhancedMetaFile` is the one class with a construction site**, in
`StaticController` (part [30](30-controllers.md)'s area):

| Site | Code |
|---|---|
| `staticcontroller.h:49` | `ref_of<EnhancedMetaFile>::type GetEnhMetaFile() const;` |
| `staticcontroller.h:53` | `ref_of<EnhancedMetaFile>::type SetEnhMetaFile(ref_of<EnhancedMetaFile>::type metaFile) const;` |
| `staticcontroller.cpp:61`–`65` | `return new_ref<EnhancedMetaFile>(reinterpret_cast<HENHMETAFILE>(User32::SendMessageT(WINDOW_HANDLE, STM_GETIMAGE, IMAGE_ENHMETAFILE, 0)), false);` |
| `staticcontroller.cpp:85`–`89` | `LPARAM lparam = reinterpret_cast<LPARAM>(metaFile.operator->());` then `new_ref<EnhancedMetaFile>(…, false)` on `STM_SETIMAGE` |

`STM_GETIMAGE`/`STM_SETIMAGE`/`IMAGE_ENHMETAFILE` are SDK constants —
`view/messages.h` defines none of them (grep: no match), and
`staticcontroller.cpp:31` includes `api_layer/user/user32.h` for
`SendMessageT`. `new_ref<T>(args…)` (`newref.h:24`–`28`) returns
`instancer_of<T>::type::template Instance<T>(1, args…)`, so the `explicit`
two-arg ctor at `enhmetafile.h:38` is the only thing that can build one.

Both sites pass ownership `false`, so even those objects would skip
`~GDIObject()`'s `DeleteObject`. And `StaticController` is itself unreachable:
its ctors are defined at `staticcontroller.cpp:40`/`:41` and the TU is compiled
from `gui/CMakeLists.txt:28`, but a repo-wide search for `StaticController` over
`src` and `examples` returns only its own pair — no window installs one. The
metafile path is **compiled, linked, and never executed**, the same dead-
controller pattern part 30 records.

### Question 2 answered: off-screen rendering never reaches a GL target

The family contains **no GDI call**: `metadc.cpp:35`–`45`,
`enhmetadc.cpp:35`–`45` and `enhmetafile.cpp:35`–`37` are ctor bodies,
`metafile.cpp:33`–`36` is empty, and none of the 26 metafile entry points is
called from any GUI TU. The replay side that would carry a recording into a
target device context — `PlayEnhMetaFile` (`gdi32.h:284`), `PlayMetaFile`
(`:286`), `PlayEnhMetaFileRecord`/`PlayMetaFileRecord` (`:285`/`:287`) and the
enumerators `EnumEnhMetaFile`/`EnumMetaFile` (`:274`/`:275`) — has no callers
anywhere. So the "Off Screen Rendering with Composition to an Open GL target
window" of `gui/purpose.md:7` is **not implemented by this family**; the live
off-screen route is 60a's `DeviceContext::CreateForDIB()` plus `DIBSection`
(part [90](90-example-mainwindow.md) records `mainwindow.cpp:270`), and `DIBSection`
itself is 60c work.

The only GL-adjacent metafile wrapper in the tree sits on the api-layer side and
is unreached from `gui/`: `api_layer/opengl/opengl.h:41` declares and
`api_layer/opengl/opengl.cpp:51`–`54` defines
`WGL::GetEnhMetaFilePixelFormat`, whose GDI twin is
`api_layer/gdi/pixelformat.cpp:55`–`59` (`gdi32.h:296`).

### Build and include facts

- All four `.cpp` are in `add_library`: `gui/CMakeLists.txt:45`
  (`gdiobjects/enhmetadc.cpp`), `:46` (`enhmetafile.cpp`), `:51` (`metadc.cpp`),
  `:52` (`metafile.cpp`) — the part's block is `:40`–`55`.
- Include shape: `metadc.h`/`enhmetadc.h` `:28`–`33` pull six headers —
  `handle.h`, `../view/drawing/rect.h`, `brush.h`, `../view/drawing/size.h`,
  `palette.h`, `gdiobject.h` — of which only `handle.h` and `gdiobject.h` are
  used; the block is `devicecontext.h:28`–`35` minus `structures.h` and `font.h`.
  `metafile.h`/`enhmetafile.h` `:28`–`29` pull only the two that are needed.
- All four `.cpp` open identically: `:25` `src/configuration/configuration.h`,
  `:27` own header, `:28` `common/stringconv.h`, `:29`
  `api_layer/user/user32.h`, then `using namespace qor::win::api;` (`:31`).
  **None includes `api_layer/gdi/gdi32.h`** — the header declaring the 26 metafile
  entry points. In this part only `gdiobject.cpp:30` includes it.
- The class marker `qor_pp_module_interface(QOR_WINGUI)` is **absent from all
  four headers** (`metadc.h:38`, `enhmetadc.h:38`, `metafile.h:34`,
  `enhmetafile.h:34`) — as it is from `memdevicecontext.h:38`. 11 of the part's
  16 headers carry it; the five that do not are these four plus `memdevicecontext.h`.

### 60b gotchas

1. **Three of the four pairs are unreachable.** `metadc.h`, `enhmetadc.h` and
   `metafile.h` are included by nothing outside their own `.cpp`, and
   `MetaDeviceContext`/`EnhancedMetaDeviceContext`/`MetaFile` have no
   construction site in `src` or `examples`. `MetaFile` cannot even be built:
   it declares **no ctor** (`metafile.h:34`–`37` empty body), base ctors are not
   inherited, and `GDIObject` has no default ctor (`gdiobject.h:56`–`58` declares
   only the two shown ctors and the dtor).
2. **`metafile.cpp` defines nothing yet is linked.** `metafile.cpp:33`–`36` is an
   empty namespace block — the only `.cpp` in this family that contributes no
   symbols, while sitting in `add_library` at `gui/CMakeLists.txt:52`.
3. **`EnhancedMetaFile::m_handle` is a shadow, not a handle.**
   `enhmetafile.h:42` declares `PrimitiveHandle m_handle;` over
   `GDIObject::m_handle`, which is a `Handle` (`gdiobject.h:64`). The ctor
   (`enhmetafile.cpp:35`) initialises only the base, nothing ever reads the
   derived member (default-constructed to `nullptr`, `handle.cpp:36`), and
   `GetHandle()` (`gdiobject.cpp:56`–`59`) returns the base's. So the class
   carries a second, dead handle field.
4. **Default-constructed DC objects are destructor-poisoned** —
   `MetaDeviceContext()`/`EnhancedMetaDeviceContext()` get `m_handle(0)` with
   `m_owner(true)` (`gdiobject.cpp:37`), and `IsInvalid()` tests against
   `(void*)(size_t)-1` (`constants.h:8`, `handle.cpp:214`–`216`), so
   `~GDIObject()` runs `GDI32::DeleteObject((HGDIOBJ)0)`. Windows API intent:
   `DeleteObject` takes a `HGDIOBJ` produced by `CreateBrushEx`/`SelectFont`-era
   calls, never a null one.
5. **One close step for four different handle kinds.** `~GDIObject()` calls
   `DeleteObject` for every `GDIObjectType`; the metafile family needs
   `CloseMetaFile`/`CloseEnhMetaFile` (DCs) and
   `DeleteMetaFile`/`DeleteEnhMetaFile` (files) — all four declared at
   `gdi32.h:267`/`:266`/`:273`/`:272` and wrapped in
   `api_layer/gdi/metafile.cpp`, never called.
6. **Ownership defaults fight the call sites.** `enhmetafile.h:38` defaults
   `own` to `true` — the one reachable default in this family — while both
   `staticcontroller.cpp:61`–`65` and `:85`–`89` pass `false`. Windows API
   intent: a `HENHMETAFILE` echoed back by `SendMessage` is not a fresh handle,
   so owning it would double-release it.
7. **`#undef GetEnhMetaFile` (`staticcontroller.cpp:35`) is order-sensitive.** It
   sits after `#include "staticcontroller.h"` (`:27`) and after
   `api_layer/user/user32.h` (`:31`), so it cannot affect the *declaration* at
   `staticcontroller.h:49` — that is parsed before `user32.h` is in play, and
   `windowcontroller.h:28`–`39` names no api_layer/user or gdi header. What the
   `#undef` protects is the member *definitions* at `:61` and `:85`. Repo
   convention: `user32.h:9`–`17` `#undef`s nine such SDK-macro names.
8. **The api-layer mirror mistypes the close pair.** `gdi32.h:266`
   `CloseEnhMetaFile(HDC hdc)` and `:267` `CloseMetaFile(HDC hdc)` take an
   `HDC`; Windows API intent is `HENHMETAFILE`/`HMETAFILE`. The neighbouring
   `CopyEnhMetaFile`/`CopyMetaFile`/`DeleteEnhMetaFile`/`DeleteMetaFile`
   declarations (`:268`–`269`, `:272`–`273`) do carry the metafile types, so
   these two are outliers in an otherwise consistent block.
9. **Naming slips:** the files are `metadc`/`enhmetadc` while the classes are the
   long `MetaDeviceContext`/`EnhancedMetaDeviceContext` and the guards the longer
   `QOR_PP_H_WINDOWS_GUI_METADEVICECONTEXT` (`metadc.h:25`–`26`) and
   `QOR_PP_H_WINDOWS_GUI_ENHMETADEVICECONTEXT` (`enhmetadc.h:25`–`26`); plus the
   four unused includes in `metadc.h`/`enhmetadc.h` and the four headers' missing
   `qor_pp_module_interface` marker.

### Still to cover

- **60c — the resource leaves** (18 files) — walked in full below. Question 3
  (`Bitmap` / `DIB` / `DIBSection`) is answered there, as is the question this
  part left open about which `GDIObjectType` each leaf passes and whether
  `Cursor`/`Icon` pass `OUnknown`.

## 60c — resource leaves: `bitmap`, `brush`, `colourspace`, `cursor`, `font`,
`icon`, `palette`, `pen`, `region` (detailed)

### File sizes (lines, h / cpp)

| Pair | `.h` | `.cpp` | |
|---|---|---|---|
| `bitmap` | 108 | 76 | three classes in one header |
| `brush` | 81 | 144 | biggest body after `cursor` |
| `colourspace` | 88 | 81 | |
| `cursor` | 83 | 177 | longest body in the family |
| `font` | 97 | 71 | |
| `icon` | 94 | 133 | |
| `palette` | 76 | 106 | |
| `pen` | 60 | 99 | shortest header |
| `region` | 80 | 140 | |

767 header lines + 1027 body lines = **1794 lines** across the 18 files. With
60a's trio (6 files) and 60b's four metafile pairs (8 files) this is the whole
16-pair inventory the part promised; `metafile.cpp` is the one file of that
count with no member definitions at all (36 lines, `metafile.cpp:36` closes an
empty namespace).

### Level 1 — what each pair names in Win32

Every leaf is a handle wrapper. The enumerator it tags and the base it uses:

| Leaf | Win32 handle | `GDIObjectType` | `GDIObject`-derived |
|---|---|---|---|
| `Bitmap` | `HBITMAP` | `OBitmap` (`bitmap.cpp:37`,`41`) | yes |
| `DIB` | `HBITMAP` | inherits `Bitmap` (`bitmap.h:90`) | through `Bitmap` |
| `DIBSection` | `HBITMAP` | inherits `Bitmap` (`bitmap.h:98`) | through `Bitmap` |
| `Brush` | `HBRUSH` | `OBrush` (`brush.cpp:36`,`40`,`44`) | yes |
| `SysBrush` | `HBRUSH` | inherits `Brush` (`brush.h:71`) | through `Brush` |
| `ColourSpace` | `HCOLORSPACE` | `OColourSpace` (`colourspace.cpp:35`,`39`) | yes |
| `Cursor` | `HCURSOR` | **none** | **no** (`cursor.h:46`) |
| `Font` | `HFONT` | `OFont` (`font.cpp:36`,`40`) | yes |
| `Icon` | `HICON` | **none** | **no** (`icon.h:61`) |
| `Palette` | `HPALETTE` | `OPalette` (`palette.cpp:35`,`39`) | yes |
| `Pen` | `HPEN` | `OPen` (`pen.cpp:35`,`40`) | yes |
| `Region` | `HRGN` | `ORegion` (`region.cpp:35`,`39`) | yes |

That settles 60a gotcha 8 and 60b's open question: `Cursor` and `Icon` do **not**
pass `OUnknown` — they are not `GDIObject` subclasses at all. They hold a
`PrimitiveHandle m_handle` plus a `bool m_owning` of their own (`cursor.h:74`,`77`;
`icon.h:86`,`89`) and destroy through `User32::DestroyCursor` / `DestroyIcon`
(`cursor.cpp:80`, `icon.cpp:83`). The directory therefore carries **two
parallel ownership models**: the `GDIObject` one (`m_owner`, closed by
`GDI32::DeleteObject`) and a hand-rolled one in the two handle types GDI has no
enumerator for.

Enumerator census: `gdiobject.h:33`–`50` declares 15 tags; 12 classes use 12 of
them; `OExtPen`, `OMetaFile` and `OUnknown` (`gdiobject.h:41`,`44`,`49`) occur
nowhere in the tree outside their declaration — `ExtCreatePen`'s product is
tagged `OPen` (`pen.cpp:35`), and `MetaFile` never tags anything (`metafile.cpp`
defines no members).

### Level 2 — the shape of each pair

**The struct mirrors.** Each leaf header re-declares the wingdi.h structures it
needs, inside `qor::platform::win`, under the same `//All types on this interface
must be portable` comment (`bitmap.h:34`, `brush.h:34`, `colourspace.h:32`,
`cursor.h:34`, `font.h:36`, `icon.h:36`, `palette.h:33`, `pen.h:33`, `region.h:34`):

| Header | Mirrors | Constants |
|---|---|---|
| `bitmap.h:37`–`64` | `RGBQUAD`, `BitmapInfoHeader` (14 fields, `biSize` defaulted to `sizeof(BitmapInfoHeader)`), `BitmapInfo` (header + `RGBQUAD bmiColors[1]`) | `BIRGB`…`BIPNG` (`67`–`72`), `DIBRGBCOLORS`/`DIBPALCOLORS` (`76`,`77`) |
| `brush.h:37`–`42` | `LogBrush {unsigned int m_style; unsigned long m_colour; unsigned long long m_hatch}` | — |
| `colourspace.h:35`–`65` | `CieXyz`, `CieXyzTriple`, `LogColourSpace` (9 scalars + `lcsFilename[MaxPath]`) | `MaxPath` from `constants.h:16` |
| `cursor.h:38`–`44` | `CursorInfo {cbSize, flags, void* hCursor, Point ptScreenPos}` | — |
| `font.h:39`–`80` | `LogFont`, `EnumLogFontEx`, `DesignVector`, `EnumLogFontExDV` | `LFFaceSize=32`, `LFFullFaceSize=64`, `MMMaxNumAxes=16` |
| `icon.h:39`–`59` | `IconInfo` (5 fields), `IconInfoEx` (adds `cbSize`, `wResID`, `szModName`/`szResName[MaxPath]`) | — |
| `palette.h:36`–`49` | `PaletteEntry` (4 bytes), `LogPalette` (`palPalEntry[1]`) | — |
| `pen.h:36`–`41` | `LogPen {lopnStyle, Point lopnWidth, lopnColor}` | — |
| `region.h:37`–`50` | `RgnDataHeader {dwSize, iType, nCount, nRgnSize, Rect rcBound}`, `RgnData {rdh, char Buffer[1]}` | — |

`Rect` and `Point` are the QOR drawing types (`view/drawing/rect.h:35`–`38` = four
`long`s; `point.h:33`,`34` = two `long`s), so `RgnDataHeader::rcBound` and
`LogPen::lopnWidth` embed the QOR layout, not the Win32 one — see the gotchas.

**Method census** (declarations, per class):

| Class | ctors | statics | instance ops |
|---|---|---|---|
| `Bitmap` | 2 | — | — (no operations at all) |
| `DIB` | 2 | — | — |
| `DIBSection` | 2 | — | — |
| `Brush` | 3 | 13 (5 creators + 8 stock) | 2 (`GetOrg`, `SetOrg`) |
| `SysBrush` | 1 | — | — |
| `ColourSpace` | 2 | 2 (`Create`, `Get`) | `Delete`, `GetLogicalColorSpace`, `Set` |
| `Cursor` | 6 | 2 (`GetCurrent`, `Show`) | `Clone`, `GetHandle`, `Clip`, `GetClip`, `GetInfo`, `GetPosition`, `GetPhysicalPosition`, `SetPosition`, `SetPhysicalPosition`, `SetAsCurrent`, `SetAsSystemCursor` |
| `Font` | 2 | 3 `Create` returning `ref_of<Font>::type` | `GetLogFont` |
| `Icon` | 8 | 3 (`LookupIdFromDirectory`, `LookupIdFromDirectoryEx`, `PrivateExtractT`) | `GetHandle`, `Clone`, `Draw`, `DrawEx`, `GetInfo`, `GetInfoExT` |
| `Palette` | 2 | 3 (`CreateHalftone`, `Create`, `Default`) | `GetHandle`, `Animate`, `GetNearestIndex`, `GetEntries`, `Resize`, `SetEntries` |
| `Pen` | 2 | 7 (3 `Create` + 4 stock) | none |
| `Region` | 2 | 9 (`CreateElliptic`×2, `CreatePolygon`, `CreatePolyPolygon`, `CreateRect`×2, `CreateRoundRect`, `Create`) | `operator==`, `Combine`, `GetData`, `GetBox`, `IsRectIn`, `SetRect`, `Offset`, `IsPointIn` |

Two factory styles coexist. Five classes return the object **by value**
(`Brush`, `Palette`, `Pen`, `Region`, `ColourSpace`), and `Font` alone returns a
**ref-counted `ref_of<Font>::type`** via `new_ref<Font>(…)` (`font.h:90`–`92`,
`font.cpp:56`–`69`) — which is why `font.h:29`–`31` and `bitmap.h:28`–`30` are
the only leaf headers pulling `newref.h`/`typeidentity.h`/`currentthread.h`.
`Bitmap` needs the same machinery because its consumers hold `ref_of<Bitmap>`
(`staticcontroller.h:30`, `buttoncontroller.h:30`), not because it produces any.

`Palette::Select` is the one operation that was written and then commented out —
the declaration at `palette.h:66` and the body at `palette.cpp:90`–`100` — leaving
`SelectPalette` reachable only through `DeviceContext::SelectPalette`
(`devicecontext.h:275`). `Cursor` also doubles as a namespace of **global** cursor
operations: `Clip`, `GetClip`, `GetPosition`, `GetPhysicalPosition`,
`SetPosition`, `SetPhysicalPosition` and the `static` `Show` (`cursor.h:61`–`69`)
act on the system cursor, not on `m_handle`, so they succeed or fail
independently of which cursor the object names.

### Construction and the close path

The `GDIObject` base gives the leaves two constructor shapes and one destroyer
(`gdiobject.cpp:37`–`54`):

- `GDIObject(GDIObjectType) : m_handle(0), m_objType(objType), m_owner(true)`
  (`:37`) — the default constructor every leaf delegates to; the handle is null
  and ownership is already `true`.
- `GDIObject(const PrimitiveHandle& h, GDIObjectType, bool takeOwnership = true)
  : m_handle(h.Use()), … { m_handle.DontClose(); }` (`:41`–`45`) — every handle-
  taking leaf constructor goes here, so the kernel-close flag is disarmed for
  any handle a caller supplied.
- `~GDIObject()`: `if(m_owner && !m_handle.IsInvalid()) { GDI32::DeleteObject(
  m_handle.Use()); m_handle.Drop(); }` (`:47`–`54`).

So the leaves' destroyer is GDI's generic `DeleteObject`, and question 4 of this
part's opening list is answered per leaf:

| Path | Who | Evidence |
|---|---|---|
| `GDI32::DeleteObject` when `m_owner` | `Bitmap` `Brush` `ColourSpace` `Font` `Palette` `Pen` `Region` (all `GDIObject` leaves) | `gdiobject.cpp:47`–`54` |
| never destroyed (`takeOwnership = false`) | 8 stock brushes (`brush.cpp:94`–`129`), 4 stock pens (`pen.cpp:71`–`96`), `Palette::Default` (`palette.cpp:69`–`74`), `Cursor::GetCurrent` (`cursor.cpp:112`–`117`), `ColourSpace::Get` (`colourspace.cpp:62`–`66`), and every non-owning ref a consumer builds (`windowfactory.cpp:89`–`92`, `buttoncontroller.cpp`, `staticcontroller.cpp`, `font.cpp:40`) | |
| `DontClose()` + `m_owner = false` from a derived ctor | `SysBrush(int)` — the only leaf that writes `DontClose()` itself (`brush.cpp:132`–`137`) | |
| `User32::DestroyCursor` / `DestroyIcon` when `m_owning` | `Cursor` (`cursor.cpp:76`–`83`), `Icon` (`icon.cpp:79`–`85`) | |
| a *third* path keyed on `NeedsClose()` | `ColourSpace::~ColourSpace` → `Delete()` (`colourspace.cpp:43`–`50`) | |

`DIBSection`'s constructor is the only constructor in the family that makes a GDI
call: `m_handle = GDI32::CreateDIBSection((HDC)(dc->GetHandle().Use()), … );
m_owner = true;` (`bitmap.cpp:62`–`66`), and `SysBrush`'s is the other
(`brush.cpp:134`). Both are **assignments from a raw handle**, and that is where
the `Handle` contract breaks: `Handle(void* h) : m_h(h) { m_needsClose =
!GetProtectFromClose(); }` (`handle.cpp:82`–`85`) asks the **kernel** for handle
information about a **GDI** handle, `GetHandleInformation` fails, `m_needsClose`
becomes `true`, the move-assignment (`handle.cpp:113`–`123`) carries that flag
into the member, and the temporary's own `~Handle` (`handle.cpp:89`–`96`) calls
`Kernel32::CloseHandle` on the GDI handle at the end of the statement — before
the object is ever used. `SysBrush` then calls `DontClose()` (`brush.cpp:135`),
which disarms the member but is too late for the temporary. The same cannot
happen via the `PrimitiveHandle` route, which is why the constructor
discipline in `gdiobject.cpp:44` matters.

### The two shadowed `m_handle` members

`Palette` (`palette.h:69`–`72`) and `ColourSpace` (`colourspace.h:81`–`84`)
redeclare `protected: Handle m_handle;` **on top of** the base member
(`gdiobject.h:64`). Name lookup in their own member functions therefore finds
the *derived* handle, which nothing ever initialises: `Handle()` sets
`m_h = Invalid_Handle_Value` (`handle.cpp:80`, `constants.h:8` =
`((void* const)(size_t)(-1))`) and `m_needsClose = false`. The constructors
delegate to `GDIObject(...)` and store the real handle in the **base** object
only (`palette.cpp:35`,`39`; `colourspace.cpp:35`,`39`).

Consequences, all of them observable:

- every `Palette` operation passes `(HPALETTE)(-1)` to GDI — `Animate`
  (`palette.cpp:54`), `GetNearestIndex` (`:78`), `GetEntries` (`:83`),
  `Resize` (`:88`), `SetEntries` (`:103`) — and `Palette::GetHandle` (`:49`)
  hands that invalid handle to callers, so `DeviceContext::SelectPalette`
  (`devicecontext.cpp`: `(HPALETTE)(palette.GetHandle().Use())`) and
  `DeviceContext::ColourCorrectPalette` (`(HPALETTE)(palette.GetHandle().Use())`)
  — the only live consumers of `Palette` in the library — always call GDI with a
  bogus handle;
- `ColourSpace::Delete` (`colourspace.cpp:59`), `GetLogicalColorSpace` (`:70`)
  and `Set` (`:75`) do the same, and because the shadow reports
  `NeedsClose() == false`, `~ColourSpace` never calls `Delete()` at all — the
  only cleanup that happens is the base destructor's `DeleteObject` on the real
  handle;
- `ColourSpace::Create` / `Get` return objects whose *base* handle is correct, so
  the pair is a working wrapper and a broken one at the same time, depending on
  which member you touch.

`Brush`, `Pen`, `Region`, `Font` and `Bitmap` declare no `GetHandle()` of their own,
so their callers get `GDIObject::GetHandle()` (`gdiobject.h:60`, `gdiobject.cpp:56`–`59`)
and the handle the constructor stored; `Cursor` and `Icon` return a
`const PrimitiveHandle&` (`cursor.h:59`, `icon.h:74`) with no `GDIObject` involved.

### Level 3 — the Win32 call map

The nine leaf bodies call **77 distinct** `GDI32::`/`User32::` symbols, and every
one of them has a definition in `api_layer` — a set-difference over the 106
`api_layer/*.cpp` files returned **zero unresolved symbols**, so this part links
cleanly (unlike 60b, whose `MetaFile` calls were unresolvable).

| Leaf | `.cpp` | Win32 symbols called | Defined in `api_layer` |
|---|---|---|---|
| `bitmap` | 76 | `CreateDIBSection` (`:64`) — the only live one; `CreateDIBitmap` appears only as the comment at `:51` | `gdi\gdi32.h:46`,`:45` |
| `brush` | 144 | `CreateBrushIndirect` `CreateDIBPatternBrush` `CreateHatchBrush` `CreatePatternBrush` `CreateSolidBrush` (`:54`–`79`), `GetBrushOrgEx` `SetBrushOrgEx` (`:84`,`:89`), `GetSysColorBrush` (`:134`) | `gdi.cpp`; `user\user32.cpp` |
| `colourspace` | 81 | `CreateColorSpaceT` `GetColorSpace` (`:53`,`:64`), `DeleteColorSpace` `GetLogColorSpaceT` `SetColorSpace` (`:59`,`:70`,`:75`) | `gdi\colour.cpp` |
| `cursor` | 177 | `CreateCursor` `LoadCursorT` `LoadCursorFromFileT` (`:51`,`:58`,`:66`,`:72`), `DestroyCursor` `CopyCursorT` (`:80`,`:87`), `ClipCursor` `GetClipCursor` (`:95`,`:101`), `GetCursor` `GetCursorInfo` `GetCursorPos` `GetPhysicalCursorPos` (`:114`,`:122`,`:129`,`:141`), `SetCursorPos` `SetPhysicalCursorPos` `ShowCursor` (`:152`,`:157`,`:162`), `SetCursor` `SetSystemCursor` (`:167`,`:173`) | `user\cursor.cpp` |
| `font` | 71 | `GetObjectT` (`:52`), `CreateFontT` `CreateFontIndirectT` `CreateFontIndirectExT` (`:58`,`:63`,`:68`) | `gdi\text.cpp`, `gdi\devicecontext.cpp` |
| `icon` | 133 | `CopyIcon` (`:39`,`:94`), `CreateIcon` `CreateIconFromResource` `CreateIconFromResourceEx` `CreateIconIndirect` (`:45`,`:51`,`:57`,`:63`), `LoadIconT` (`:69`,`:75`), `DestroyIcon` (`:83`), `DrawIcon` `DrawIconEx` (`:100`,`:105`), `GetIconInfo` `GetIconInfoExT` (`:110`,`:115`), `LookupIconIdFromDirectory` `LookupIconIdFromDirectoryEx` `PrivateExtractIconsT` (`:120`,`:125`,`:130`) | `user\icon.cpp` |
| `palette` | 106 | `AnimatePalette` `GetNearestPaletteIndex` `GetPaletteEntries` `ResizePalette` `SetPaletteEntries` (`:54`,`:78`,`:83`,`:88`,`:103`), `CreateHalftonePalette` `CreatePalette` `GetStockObject` (`:59`,`:65`,`:71`); `SelectPalette` only inside the comment `:90`–`100` | `gdi\colour.cpp` |
| `pen` | 99 | `CreatePen` `CreatePenIndirect` `ExtCreatePen` (`:52`,`:59`,`:66`), `GetStockObject` ×4 (`:73`,`:80`,`:87`,`:94`) | `gdi\pen.cpp` |
| `region` | 140 | `EqualRgn` `CombineRgn` `GetRegionData` `GetRgnBox` `RectInRegion` `SetRectRgn` `OffsetRgn` `PtInRegion` (`:46`–`81`), `CreateEllipticRgn` `CreateEllipticRgnIndirect` `CreatePolygonRgn` `CreatePolyPolygonRgn` `CreateRectRgn` `CreateRectRgnIndirect` `CreateRoundRectRgn` `ExtCreateRegion` (`:86`–`135`) | `gdi\region.cpp` |

Thirty-one of the 77 come from the `User32` window/message layer rather than GDI's
drawing layer: the `cursor` pair (16 symbols) and the `icon` pair (14) call nothing
else, and `GetSysColorBrush` is the single `User32` call in the other seven pairs;
the remaining 46 are `GDI32::`. Two of the nine **pairs** therefore never reach the
drawing layer at all. The include set matches the
call set exactly rather than the type set: `cursor.cpp:29` and `icon.cpp:29` pull
`api_layer/user/user32.h` and no `gdi32.h`; `colourspace.cpp:29`, `palette.cpp:29`,
`pen.cpp:29`, `region.cpp:29` pull `gdi32.h` and no `user32.h`; only `bitmap.cpp:30`/`31`,
`brush.cpp:29`/`30` and `font.cpp:29`/`30` pull both — and in those three the `user32.h`
half is what supplies `GetSysColorBrush` (`brush.cpp:134`) and the `SendMessageT`
declarations nothing here calls.

### Marshalling — three patterns, and the mirror verdicts

**Pattern A — cast a QOR struct straight into the Win32 pointer type.** Every
factory that takes structured input does it: `reinterpret_cast<const LOGBRUSH*>(&logBrush)`
(`brush.cpp:54`), `LPLOGCOLORSPACE` (`colourspace.cpp:53`), `LOGPALETTE`
(`palette.cpp:65`), `LOGPEN` (`pen.cpp:59`), `LPRGNDATA` (`region.cpp:56`),
`::LOGFONT` (`font.cpp:63`), `::ICONINFO` (`icon.cpp:63`), `PCURSORINFO`
(`cursor.cpp:123`), plus `RECT`/`POINT` for scalar geometry (`region.cpp:61`,`:66`,`:93`,`:100`,`:121`; `brush.cpp:84`,`:89`). The mirrors are compared field
for field against `wingdi.h`:

| Mirror (`qor::platform::win`) | Verdict |
|---|---|
| `BitmapInfoHeader` (`bitmap.h:45`–`58`) | **correct** — 11 fields, `biSize{sizeof(BitmapInfoHeader)}` = 40 = `BITMAPINFOHEADER`; `BitmapInfo` (`:60`–`64`) = header + `RGBQUAD bmiColors[1]` = `BITMAPINFO` |
| `RgnDataHeader` (`region.h:38`–`44`) | **correct** — `dwSize iType nCount nRgnSize Rect rcBound` = `RGNDATAHEADER`, and `Rect`'s four `long`s (`rect.h:35`–`38`) are `RECT`'s four `LONG`s under LLP64 |
| `PaletteEntry`/`LogPalette` (`palette.h:36`–`49`) | **correct** — 4 bytes; `palVersion palNumEntries palPalEntry[1]` |
| `LogFont` (`font.h:43`–`59`) | **correct** — five `long`s, eight `byte`s, `lfFaceName[LFFaceSize=32]` = `LF_FACESIZE` |
| `IconInfo` (`icon.h:39`–`46`) | **correct** on x64 — `int fIcon`, two `unsigned long`, two `void*` = `BOOL DWORD DWORD HBITMAP HBITMAP` with `HBITMAP` = `PVOID` |
| `CursorInfo` (`cursor.h:38`–`44`) | **correct** on x64 — 4+4+`void*`+`Point` = 24 = `CURSORINFO`; `cursor.cpp:121` sets `cbSize = sizeof(CursorInfo)` before the call |
| `LogBrush` (`brush.h:37`–`42`) | **wrong size** — `unsigned long long m_hatch` (8 bytes) where `LOGBRUSH::lbHatch` is `LONG` (4). The mirror is 16 bytes, GDI reads 12, and only a hatch pattern using bits above 31 would notice |
| `LogPen` (`pen.h:36`–`41`) | **wrong shape** — `unsigned int lopnStyle; Point lopnWidth; unsigned long lopnColor` vs `POINT lopnStyle; WORD lopnWidth; COLORREF lopnColor`. In Win32 the pen *style* is a point and the width is a word; here the kinds are swapped, so `Pen::Create(const LogPen*)` (`pen.cpp:59`) hands GDI a struct whose first 8 bytes are a style int plus padding |
| `LogColourSpace` (`colourspace.h:49`–`65`) | **wrong width of one field** — `lcsFilename[MaxPath]` is `wchar_t[260]` only under `#if qor_pp_unicde`, and the macro is misspelled at `colourspace.h:60` as `qor_pp_unicde` (undefined → 0), so the `char[260]` branch always wins. `GetLogColorSpaceT` (`colourspace.cpp:70`) then writes 260 `wchar_t` (520 bytes) into a 260-byte array — a 260-byte overwrite of `lcsGamma*` and the stack past it |

`XForm`, the one mirror `region.h` borrows rather than declares (`region.h:29`
→ `common/structures.h:235`–`241`), is six `float`s = 24 bytes = `XFORM` — correct,
and it is the only place in this part where a mirror is shared instead of
repeated.


**Pattern B — copy through a real Win32 local.** Only `Cursor` does it: `Clip`
builds `RECT r = {rect.m_left, rect.m_top, rect.m_right, rect.m_bottom}` and passes
`&r` (`cursor.cpp:94`–`95`); `GetClip` reads a `RECT r = {0}` and copies the four
fields back into the caller's `Rect` (`:100`–`107`); `GetPosition`/`GetPhysicalPosition`
use `::POINT p = {0}` and copy `p.x`,`p.y` into `Point::m_x`,`m_y` (`:128`–`135`,
`:141`–`147`). These four are the only marshalled calls in the family that are
layout-safe by construction — and `Region`, which has the most geometry of any
leaf, uses pattern A everywhere instead.

**Pattern C — cast the handle.** `(HBRUSH)(m_handle.Use())`, `(HRGN)(…)`,
`(HPALETTE)(…)`, `(HCOLORSPACE)(…)`, `(HPEN)(…)`, `(HGDIOBJ)(GetHandle().Use())`
(`font.cpp:52`), `(HBITMAP)(…)`. A device context is passed as a bare `Handle` —
`GetOrg`/`SetOrg` (`brush.h:67`,`:68`), `ColourSpace::Get`/`Set`
(`colourspace.h:77`,`:79`), `Palette::CreateHalftone` (`palette.h:60`) — so nothing
here reaches the `DeviceContext` type except `DIBSection`, which is the one leaf
taking the object: `ref_of<DeviceContext>::type dc` (`bitmap.h:101`) dereferenced as
`dc->GetHandle().Use()` (`bitmap.cpp:64`). `bitmap.h:79` forward declares that
class so the leaf can name it without the include cycle; `devicecontext.h:31`,`:33`,`:34`
pulls `brush.h`/`palette.h`/`font.h` in the other direction.

### Consumers — who reaches the nine leaves

| Consumer | Types | How |
|---|---|---|
| `controllers/a_h/buttoncontroller.h:30`,`:31` · `.cpp:44`–`92` | `Bitmap` `Cursor` `Icon` | `SendMessageT(WINDOW_HANDLE, BM_GETIMAGE/BM_SETIMAGE, IMAGE_BITMAP/IMAGE_CURSOR/IMAGE_ICON, …)`; marshals through `bitmap->GetHandle().Use()` (`:51`,`:82`,`:92`); `GetCursor` (`:77`) and `GetIcon` (`:87`) pass **one** argument, so `own` defaults to `true` (`cursor.h:50`, `icon.h:65`) and `~Cursor`/`~Icon` destroy a handle the window owns |
| `controllers/s_s/staticcontroller.h:30`–`33` · `.cpp:53`–`94` | `Bitmap` `Cursor` `Icon` (+`EnhancedMetaFile`) | the same message trio with `STM_GETIMAGE`/`STM_SETIMAGE`; marshals through `bitmap.operator->()` (`:75`,`:81`,`:93`) — `Ref<T>::operator->` yields the **C++ object address**, not the handle; every getter passes `false` (`:53`,`:58`,`:70`) so nothing owns |
| `controllers/i_m/menu.h:34` · `.cpp:206`,`:208` | `Bitmap` | `SetItemBitmaps(position, flags, Bitmap*, Bitmap*)` calls `SetMenuItemBitmaps(MENU_HANDLE, position, flags, (HBITMAP)(nullptr), (HBITMAP)(nullptr))` — both `Bitmap*` parameters are discarded |
| `controllers/a_h/caret.h:42` · `.cpp:37` | *not* `Bitmap` | `Create(Window&, Handle* bitmap, …)` → `CreateCaret((HBITMAP)(bitmap ? bitmap->Use() : nullptr), …)` — a raw `Handle*`, so no `Bitmap` type safety |
| `controllers/windowcontroller.h:66`–`93` · `.cpp:545`–`627` | `Region` | `(HRGN)(region->GetHandle().Use())` into `GetUpdateRgn` `GetWindowRgn` `InvalidateRgn` `RedrawWindow` `SetWindowRgn` `ValidateRgn` `GetDCEx` (`:547`,`:564`,`:579`,`:589`,`:594`,`:609`,`:627`) — the one place a leaf handle is consumed correctly |
| `factories/windowclass.h:31`–`33` · `.cpp:78`,`:138` | `Brush` `Icon` `Cursor` | `SetBackgroundBrush(const Brush&)` stores `brush.GetHandle()` (`windowclass.cpp:140`); `windowclassregistration.cpp:44`–`46`,`:49` write them to `WNDCLASSEX::hIcon`/`hCursor`/`hbrBackground`/`hIconSm`; `windowfactory.cpp:91` supplies `Brush(COLOR_WINDOW)` |
| `view/handlers/rendered/font.h:29` · `.cpp:40`,`:47` | `Font` | `case wmSetFont:` wraps `wParam` as `Font(PrimitiveHandle((void*)(wParam)), false)`; `wmGetFont` returns whatever `OnGetFont` yields, and the base returns `PrimitiveHandle(0)` (`font.cpp:63`) |
| `view/handlers/rendered/iconized.h:29` · `.cpp:54`,`:72` | `Icon` | only `PrimitiveHandle` crosses the hook; `OnQueryDragIcon` returns `PrimitiveHandle(0)` |
| `view/handlers/rendered/rendering.h:29`,`:30` · `painting/themedata.h:29` | `Icon` `Font` `DeviceContext` | includes only |
| `controllers/a_h/animationcontroller.h:30`,`:31` | `Cursor` `Icon` | includes only — `animationcontroller.cpp` names neither type, so the include pair is unused there |
| `ui/egl/window.h:37`,`:51`,`:73` | `Brush` `Icon` `Cursor` | `m_backgroundBrush(1)` then `SetBackgroundBrush`; **dead** — `ui/CMakeLists.txt:2` has `#add_subdirectory(egl)` |
| `examples/**` | — | no example in the tree includes a `gdiobjects` header; these types are library-internal, and `examples/experimental/osspecific/windows/CMakeLists.txt` builds only `mainwindow` (`:4`) with `button` `dialogs` `editbox` `menu` `minimal` `console` `retropad` all commented |

The three message controllers (`buttoncontroller`, `staticcontroller`, plus
`caret`) are where `Bitmap`/`Cursor`/`Icon` are actually constructed at runtime,
and both of them construct through `new_ref<T>(handle, …)` — the ref-counted
instance machinery — never through the leaves' own factory statics. **Not one
factory static in any of the nine leaves has a call site anywhere in the tree**:
`Brush::CreateSolid`, `Region::CreateRect`, `Pen::Create`, `Palette::Create`,
`Font::Create`, `Icon::CreateIconFromResource` … appear only in their own
declarations and definitions. The only live construction of these types is the
`(PrimitiveHandle, bool)` constructor reached from the controllers and the
message hooks. `DIBSection` and `SysBrush` — the two leaves that make a GDI call
in their constructor — are never instantiated at all: the only `CreateDIBSection`
outside `gdiobjects` is `ui\gl\windowimpl.cpp:152`, which calls the `api_layer`
function directly and bypasses the wrapper.

### Gotchas — what the 60c walk adds

1. **`pen.h` is the only leaf header with no access label.** `Pen : public GDIObject`
   (`pen.h:43`) opens `{` at `:44` and the first declaration `Pen();` follows at
   `:45` — every other leaf puts a `public:` label before its members
   (`bitmap.h:83`, `brush.h:46`, `colourspace.h:69`, `cursor.h:48`, `font.h:84`,
   `icon.h:63`, `palette.h:53`, `region.h:54`). So `Pen`'s two ctors, its virtual
   dtor and all seven factory statics are **private members**: the out-of-line
   definitions at `pen.cpp:35`–`96` compile, but no caller outside the class could
   reach them. That is consistent with the zero-call-site census — `Pen` is
   unreachable twice over, by access and by absence of callers.

2. **`Brush(COLOR_WINDOW)` is a colour-as-handle overload, and it is live.**
   `windowfactory.cpp:91` sits inside `WindowFactory::AddWindowClass`
   (`windowfactory.cpp:78`–`98`), so it runs on every window-class registration.
   It selects `Brush::Brush(int colour) : GDIObject(colour, OBrush, true)`
   (`brush.h:49`, `brush.cpp:40`) — the value becomes the **handle** (`m_handle =
   colour`), not the result of `CreateSolidBrush` (`brush.cpp:76`, no call site).
   `windowclass.cpp:138` then copies that number into `m_hbrBackground`
   (`:140`), `windowclassregistration.cpp:46` writes it into
   `WNDCLASSEX::hbrBackground`, and the temporary brush is destroyed at the end of
   the same statement: `m_owner` is `true` and the value is not
   `Invalid_Handle_Value` (`constants.h:8`), so `~GDIObject` runs
   `GDI32::DeleteObject((HGDIOBJ)COLOR_WINDOW)` (`gdiobject.cpp:49`–`52`) on the
   handle the class has just stored. `COLOR_WINDOW` is declared nowhere in this
   tree (repo-wide grep: the only other hit is the unrelated `microui` enum at
   `examples/experimental/microuitest/microui.h:49`), so it resolves through the
   SDK headers — either way it is a stock-object selector/colour being used as a
   brush handle, which `RegisterClass` hands to `RegisterClassEx`.

3. **`Cursor` and `Icon` have opposite copy semantics.** `Cursor`'s copy ctor shares
   the handle and clears ownership — `Cursor::Cursor(const Cursor& src) :
   m_handle(src.m_handle), m_owning(false)` (`cursor.cpp:40`) — so a copied cursor is
   inert and cannot be destroyed by value; `Icon`'s copy ctor clones and owns —
   `Icon::Icon(const Icon& src) : m_handle(User32::CopyIcon(...)), m_owning(true)`
   (`icon.cpp:39`). Both also expose the cloning path explicitly
   (`Cursor::Clone` `cursor.cpp:87`, `Icon::Clone` `icon.cpp:94`), so the copy ctor
   and `Clone()` disagree for one of the two types.

4. **`Cursor`'s "global" operations are not keyed to its handle at all.** `Clip`,
   `GetClip`, `GetPosition`, `GetPhysicalPosition`, `SetPosition`,
   `SetPhysicalPosition` and the static `Show` (`cursor.h:61`–`69`, calls at
   `cursor.cpp:95`,`:101`,`:129`,`:141`,`:152`,`:157`,`:162`) pass no `m_handle`,
   so they act on the system cursor; only `Clone` (`:87`), `SetAsCurrent`
   (`:167`) and `SetAsSystemCursor` (`:173`) use the handle the object names —
   `GetInfo` (`:122`) is the fourth member that reads it, and the two
   statics `GetCurrent`/`Show` have no handle to read. So of `Cursor`'s ten
   non-static operations (`cursor.h:58`–`71`, plus `GetHandle` at `:59`), **six
   ignore the handle entirely**, and a default-constructed `Cursor` still
   "works" for those six — the type is a half cursor, half namespace of globals.

### Still to cover

Nothing in this part — all 32 files (16 pairs) are walked. The part-level
questions are closed: q3 (`Bitmap`/`DIB`/`DIBSection`) in Level 2, q4 (close path)
in "Construction and the close path", the enumerator question in Level 1, and the
off-screen question in 60b.

**Next:** [70 — painting and theming](70-painting.md) ·
[back to index](README.md)
