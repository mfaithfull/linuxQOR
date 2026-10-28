# 80 — Geometry and layout (plus the message vocabulary)

**Status: detailed — all 12 files** (`view/drawing` 4, `view/layout` 7, `view/messages.h` 1), read in full and verified against source 27 Oct 2026. Physical line counts: `drawing` 243 (`point.h` 39, `rect.h` 57, `rect.cpp` 108, `size.h` 39) · `layout` 544 (`layout.h` 96, `layoutitem.h` 57, `vbox.h` 47, `windowlayoutitem.h` 53, `layout.cpp` 209, `vbox.cpp` 37, `windowlayoutitem.cpp` 45) · `messages.h` 324 — **1,111 lines** over 12 files. All four part-80 translation units are in the library: `gui/CMakeLists.txt:56` `view/drawing/rect.cpp`, `:97` `view/layout/layout.cpp`, `:98` `view/layout/windowlayoutitem.cpp`, `:99` `view/layout/vbox.cpp`.

## Geometry — `view/drawing` (4 files, 243 lines)

### The three records

| Type | Declared | Members | Namespace | Exported |
|---|---|---|---|---|
| `struct Point` | `drawing/point.h:31` | `long m_x` `long m_y` — `:33` `:34` | `qor::platform::win` (`point.h:29`–`:37`) | **no** marker |
| `struct Rect` | `drawing/rect.h:33` | `m_left` `m_top` `m_right` `m_bottom` (`:35`–`:38`) + **13** member declarations `:40`–`:53` | `qor::platform::win` (`rect.h:31`–`:55`) | **yes** — `struct qor_pp_module_interface(QOR_WINGUI) Rect` |
| `struct Size` | `drawing/size.h:31` | `long m_x` `long m_y` — `:33` `:34` | `qor::platform::win` (`size.h:29`–`:37`) | **no** marker |

The summary's open question about export status is closed: `qor_pp_module_interface` matches **exactly once in the whole part** — `rect.h:33`. `point.h` and `size.h` carry no marker, and neither does anything in `layout/`, so `Rect` is the only declared export of `winqor_gui.dll` (`QOR_WINGUI` is the target's private compile definition, `gui/CMakeLists.txt:107`). The asymmetry is not cosmetic: `Rect::IsPointInRect(const Point&) const` is declared at `rect.h:46` and defined at `rect.cpp:67`–`71`, so an **exported** signature takes a parameter type that is **not** exported.

All three carry the same intent line — `//All types on this interface must be portable` at `point.h:28` `rect.h:30` `size.h:28` — and the `long` spelling plus member order is the entire basis for drop-in compatibility with the SDK `POINT` / `RECT` / `SIZE`. There is **no** `static_assert` on size or layout anywhere in the part; the bridge is `reinterpret_cast` alone (`rect.cpp:37` `:42` `:47` `:53` `:59` `:64` `:69` `:75` `:80` `:86` `:93`).

Include guards drop the `VIEW_DRAWING` segment — `QOR_PP_H_WINDOWS_GUI_POINT` `point.h:25`, `..._RECT` `rect.h:25`, `..._SIZE` `size.h:25` — and `rect.h:28` reaches the point record with a bare `#include "point.h"`. `rect.h` uses `qor_pp_module_interface` at `:33` but never includes `src/configuration/configuration.h`, which the conventions require for any header using `qor_pp_*`.

### `rect.cpp` — 11 forwards, 2 arithmetic

Header block `rect.cpp:25`–`:31`: `configuration.h`, `"rect.h"`, `common/stringconv.h` (source-root-relative, `:28` — nothing in the file uses a string conversion, so it is a leftover include), `api_layer/user/user32.h`, `using namespace qor::win::api;`.

| Member | `rect.cpp` | `User32::` call | SDK decl `api_layer/user/user32.h` |
|---|---|---|---|
| `CopyFrom(const Rect&)` | `:35`–`38` | `CopyRect` | `:447` |
| `Equal(const Rect&) const` | `:40`–`43` | `EqualRect` | `:448` |
| `Inflate(int dx, int dy)` | `:45`–`48` | `InflateRect` | `:449` |
| `FromIntersection` (static) | `:50`–`55` | `IntersectRect` | `:450` |
| `IsEmpty() const` | `:57`–`60` | `IsRectEmpty` | `:451` |
| `Offset(int dx, int dy)` | `:62`–`65` | `OffsetRect` | `:452` |
| `IsPointInRect(const Point&) const` | `:67`–`71` | `PtInRect` — `const POINT* p = reinterpret_cast<const POINT*>(&pt)` at `:69` | `:453` |
| `Set(int,int,int,int)` | `:73`–`76` | `SetRect` | `:454` |
| `SetEmpty()` | `:78`–`81` | `SetRectEmpty` | `:455` |
| `FromSubtraction` (static) | `:83`–`88` | `SubtractRect` | `:456` |
| `FromUnion` (static) | `:90`–`95` | `UnionRect` | `:457` |
| `Width()` | `:97`–`100` | `m_right - m_left` — **no API call** | — |
| `Height()` | `:102`–`105` | `m_bottom - m_top` — **no API call** | — |

The 11 forwards collapse every `BOOL` with `? true : false`, so the tri-state SDK result becomes a plain bool. The three static triple-call members each default-construct a local `Rect r;` (`:52` `:85` `:92`) — four uninitialised `long`s — hand its address to the API, then return it by value; the API filling them is the only thing that makes the result defined.

### Where the geometry is actually used

The example is the part's densest consumer, and the uses are concrete rather than incidental:

| Site | What the code does |
|---|---|
| `mainwindow.cpp:35`–`38` | `void GetRectCenter(Rect* prc, Point *ppt)` — writes `ppt->m_x = prc->m_left + (prc->m_right - prc->m_left)/2` and likewise for `m_y`; SDK-shaped pointers to QOR records |
| `mainwindow.cpp:41`–`47` | `CustomClientAreaAdjustment(Rect* proposedClientArea)` adjusts `m_left`/`m_right`/`m_top`/`m_bottom` by `0`, with the intent comment `// Title bar height//themeData->GetSysSize(SMCYSIZE)` at `:45` — the theme-driven frame widths are commented out, so the adjustment is currently a no-op |
| `mainwindow.cpp:49`–`67` | `virtual long long OnNCCalcSize(Window&, bool, Rect* proposedWindowRect, NCCalcSizeParams* ncCalcSizeParams)` aliases `&ncCalcSizeParams->rgrc[0]` `:53`, `rgrc[1]` `:55`, `rgrc[2]` `:56` as `Rect*` with no cast, calls `CustomClientAreaAdjustment(prcClientNew)` `:54` and again on the window rect `:67`, then centres the valid destination on the client via `GetRectCenter(prcValidSrc,&ptOldCenter)` `:59` and `GetRectCenter(prcClientNew,&ptNewCenter)` `:60` |
| `mainwindow.cpp:102`–`129` | `Rect r;` `:102` → `controller->GetRect(r)` `:103` (`//get screen coordinate rectangle of window`) → `if(!r.IsPointInRect(Point(x,y)))` `:104`, then hit boxes at `y < r.m_top + 31` `:108`, `y < r.m_top + 5` `:110`, `x < r.m_left + 5` `:114` `:129` |
| `mainwindow.cpp:177`–`180` | `controller->GetClientRect(rc)` `:177` (`windowcontroller.h:176`) → `dc->GetTextExtentPoint32T(L"Hello, Windows!", 15, &size)` `:179` → `dc->TextOutT((rc.Width()/2)-(size.m_x/2), (rc.Height()/2) - (size.m_y/2), ...)` `:180` — both `Rect` arithmetic members plus `Size::m_x/m_y` |

`WindowController` consumes the records in its own SDK-shaped parameter structs, which is where the layout-compatibility requirement is hardest: `windowcontroller.h:66` `Rect rcTitleBar` (mirror `cbSize` at `:65`), `:73` `Rect rcWindow` `:74` `Rect rcClient` (`WindowInfo`, `cbSize` `:72`), `:91` `Rect rcNormalPosition` `:93` `Rect rcDevice`, `:108` `const Rect* prcDirty`, and `:102` `const Size* psize` inside the `UpdateLayeredWindowInfo` mirror (`cbSize` `:99`). `UpdateLayered` takes them by reference at `windowcontroller.h:222`: `bool UpdateLayered(ref_of<DeviceContext>::type dcDst, Point& ptDest, Size& size, ..., Point& ptSrc, ...)`.

Reach of the three headers across `gui/`:

- `point.h` — 5 headers: `gdiobjects/brush.h:31` `cursor.h:32` `pen.h:29`, `controllers/a_h/caret.h:32`, `view/handlers/message.h:28`; plus one `.cpp`, `gdiobjects/cursor.cpp:30`.
- `rect.h` — 11 headers: `window.h:38`, `gdiobjects/{cursor.h:31, devicecontext.h:30, enhmetadc.h:29, memdevicecontext.h:29, metadc.h:29, region.h:31}`, `controllers/a_h/caret.h:31`, `controllers/i_m/{menu.h:33, monitor.h:32, mouse.h:30}`. `menu.h:33` takes the source-root-relative spelling while its sibling `caret.h:31` uses the relative one — include style is inconsistent inside one directory.
- `size.h` — 6 headers: `controllers/windowcontroller.h:37`, `gdiobjects/{devicecontext.h:32, enhmetadc.h:31, memdevicecontext.h:31, metadc.h:31}`, `view/layout/layoutitem.h:28`.

`Point` is embedded in the message-cracking record: `view/handlers/message.h:33`–`44` declares `struct Message` in `qor::platform::win` (`message.h:31`–`:45`) with `Point pt;` at `:40`, next to `void* hwnd` `:35`, `unsigned int message` `:36`, `unsigned long long wParam` `:37`, `long long lParam` `:38`, `unsigned long time` `:39`, and `unsigned long lPrivate` guarded by `#ifdef _MAC` `:41`–`:43`. `window.h:35` includes it, so every window carries that shape.

The only consumer of the point geometry outside the example is `controllers/a_h/editcontroller.cpp:136`–`143`: `Point EditController::PosFromChar(unsigned int index) const` default-constructs `Point Pos;` (`:138`), calls `EM_POSFROMCHAR` (`:139`, local named `packedResutl` — a source typo) and fills `Pos.m_x`/`m_y` at `:140` `:141` with `GetXLParam`/`GetYLParam` — macros defined in **this part** at `messages.h:37` `:38`.

### Namespace split inside the part

Geometry is platform-level, layout and messages are gui-level. `drawing/` opens three namespaces — `point.h:29` `rect.h:31` `size.h:29`, closed `}}}//qor::platform::win` at `point.h:37` `rect.h:55` `size.h:37`. `layout/` opens five — `layout.h:31` `layoutitem.h:30` `vbox.h:31` `windowlayoutitem.h:31`, closed `}}}}}//qor::platform::win::gui::view` at `layout.h:94` `layoutitem.h:55` `vbox.h:45` `windowlayoutitem.h:51` — matched by `layout.cpp:29`–`209` `vbox.cpp:30`–`36` `windowlayoutitem.cpp:30`–`45`. `messages.h:40` opens the same five-deep namespace, closed at `:322`. `layoutitem.h:28` is the seam between the two levels: `#include "../drawing/size.h"`.

## Layout — `view/layout` (7 files, 544 lines)

### The lattice

| Class | Declared | Base | What it adds |
|---|---|---|---|
| `LayoutItem` | `layoutitem.h:32` | — | virtual `Place(Size,Size){}` `:39`; inline virtual `GetMinSize` `:40` `GetMaxSize` `:41` `SetMinSize` `:42` `SetMaxSize` `:43`; **non-virtual** `Size& TmpSize()` `:45`; protected `m_tmpSize` `m_minSize` `m_maxSize` zero-initialised `:49`–`:51` |
| `Layout` | `layout.h:33` | `LayoutItem` | `enum Alignment { beginning, middle, end }` `:37`–`42`; `enum Direction { forward, reversed, }` `:44`–`48` (trailing comma); virtual `Place(...) {}` **empty** `:53`; inline `AddItem(LayoutItem*)` `:54`–`57`; inline `GetDirection()` `:59`–`62` `SetDirection(const Direction)` `:64`–`67`; protected `std::vector<LayoutItem*> m_items` `:71`, `Direction m_direction{forward}` `:72` |
| `VertcialLayout` | `layout.h:76` | `Layout` | declares `virtual void Place(Size,Size)` `:82`; **defined** `layout.cpp:120`–`207` |
| `HorizontalLayout` | `layout.h:85` | `Layout` | declares `:91`; **defined** `layout.cpp:31`–`118` |
| `VBox` | `vbox.h:33` | `VertcialLayout` | ctor `VBox(qor::platform::win::Window*)` `:37` (body `vbox.cpp:32`–`35`), `m_window` `:42`; **no `Place` of its own** — inherits `VertcialLayout::Place` |
| `WindowLayoutItem` | `windowlayoutitem.h:33` | `LayoutItem` | ctor `:37` (body `windowlayoutitem.cpp:32`–`35`), `Place` `:40` (body `:37`–`43`), **re-declares** the four size accessors inline `:41`–`:44` with the same bodies as `LayoutItem`, `m_window` `:48` |

`Layout` is both an item and a container: it derives from `LayoutItem` (`layout.h:33`) and owns a vector of them (`:71`). Nothing in `layout/` carries `qor_pp_module_interface`, so the whole lattice is internal to `winqor_gui.dll` — yet `Window::SetLayout` / `GetLayout` (`window.h:64` `:65`) are exported `Window` members (`window.h:49` `class qor_pp_module_interface(QOR_WINGUI) Window`) whose signatures name `qor::ref_of<qor::platform::win::gui::view::LayoutItem>::type`, and `window.h:39` includes `view/layout/layout.h`.

The misspelling is load-bearing, not cosmetic: `VertcialLayout` appears at `layout.h:76` `:80` `:81` `:82`, `layout.cpp:120`, `vbox.h:33`, `vbox.cpp:32` and at the two construction sites `windowfactory.cpp:150` `:162` — 9 sites in 4 files, all spelled identically, which is why it compiles. Renaming is a maintainer decision; the grep is the answer to the summary's question 3.

`vbox.h:29` and `windowlayoutitem.h:29` both reach the window type with `#include "../../window.h"`; `windowlayoutitem.h:28` also includes `layoutitem.h` for its base. `vbox.cpp:28` and `windowlayoutitem.cpp:28` both include `../../controllers/windowcontroller.h` — in `windowlayoutitem.cpp` the include is needed (`:41` calls `SetPos`), in `vbox.cpp` it is **unused**: the file's only statement is `m_maxSize = Size{16384, 16384};` at `:34`.

### `Place` — the contract, and the one leaf that obeys it

Everywhere the signature is `void Place(Size position, Size extent)`: pass-by-value `Size` pairs, no return value, no error channel. Two of them are empty — `LayoutItem::Place` `layoutitem.h:39` and `Layout::Place` `layout.h:53` — so an un-overridden item or container places nothing.

The only leaf that does anything is `WindowLayoutItem::Place`, `windowlayoutitem.cpp:37`–`43`:

```cpp
if(m_window && m_window->GetController())
{
    m_window->GetController()->SetPos(nullptr, position.m_x, position.m_y, extent.m_x, extent.m_y, 0);
}
```

`Window::GetController()` returns a raw `WindowController*` (`window.h:73`), and `SetPos` is `windowcontroller.h:211` `bool SetPos(Window* insertAfter, int x, int y, int cx, int cy, unsigned int flags)`. So a layout placement is a **position-and-size request for the child HWND** — the engine moves real child windows, it does not draw them. Note the vocabulary mismatch: `extent.m_x/m_y` feeds `cx, cy` (a **size**), while `Rect` is right/bottom. The part's two geometry vocabularies do not line up, and nothing converts between them.

`VBox::Place` is never defined, and a `VBox`'s `m_items` (inherited from `Layout`) is only ever filled by `AddItem`, which `VBox` construction sites never call — so placing a `VBox` places nothing.

### The arrangement algorithm — `layout.cpp:31`–`118` (horizontal), `:120`–`207` (vertical)

Two near-identical copies; the vertical one is the horizontal one with `m_x`→`m_y`. Walking the horizontal copy:

1. **Measure** `:39`–`50`. Accumulate `minSize` `:41` and `maxSize` `:42`; seed `item->TmpSize().m_x = item->GetMinSize().m_x` `:43`; accumulate `currentSize` `:44`; for each item with `max > min` add its **min** size to `flexSize` `:47` and increment `flexCount` `:48`.
2. **Overflow** `:51`–`54`: `if(minSize > extent.m_x){ //Overflow }` — an **empty block**. Nothing happens, items keep their min sizes, the container silently over-fills.
3. **Flex** `:55`–`85`, entered only when `minSize < extent.m_x`. `remaining = extent.m_x - minSize` `:57`; `while(remaining > 0 && flexCount > 0)` `:58`; per round `totalDistributed = 0` `:60`; per item `:61`–`77`:
   - guard `item->GetMaxSize().m_x > item->TmpSize().m_x` `:63`;
   - `offset = std::max(1L, std::min((max - Tmp), remaining / (flexSize / Tmp)))` `:65`;
   - `totalDistributed += offset` `:66`, `item->TmpSize().m_x += offset` `:67`;
   - `flexCount--` when the item reaches its max `:68`–`71`;
   - `if(remaining - totalDistributed == 0) break;` `:73`–`76`;
   - end of round: `remaining -= totalDistributed` `:78` and **`flexSize += totalDistributed`** `:79`.
4. **Underflow** `:81`–`85`: comment only — `//We could space out the items or align them beginning or end or middle`. That sentence is why `Alignment` exists (`layout.h:37`–`42`); the enum is never read anywhere in `gui/`.
5. **Placement** `:88`–`117`: `long placementY = 0` `:88` `long placementX = 0` `:89`, `Size itemPos{0}` `:90`, `Size itemExt{0}` `:91`.

`forward` `:93`–`103`: pos = running cursor `:97` `:98`, `itemExt.m_x = item->TmpSize().m_x` `:99`, `itemExt.m_y = extent.m_y` `:100` (the cross axis takes the parent's full extent), `item->Place(...)` `:101`, advance `placementX += itemExt.m_x` `:102`. Correct.

`reversed` `:105`–`117`: `placementX = extent.m_x` `:107`, then per item `placementX -= itemExt.m_x` `:111` **before** `itemExt.m_x` is refreshed at `:114`. Traced with widths `w1 w2` and extent `W`:

- item 1 — `itemExt.m_x` is still the `:91` zero-init `0`, so `placementX` stays `W` and `itemPos.m_x = W`; only then does `:114` set `itemExt.m_x = w1`.
- item 2 — `placementX = W - w1`, so `itemPos.m_x = W - w1`, which is item 1's intended slot; `itemExt.m_x` becomes `w2`.

So the reversed branch subtracts the **previous** item's width rather than its own and never advances the cursor by the current item: every item lands one slot late, the stack is shifted right by the last item's width, and it overhangs the container's right edge. `VertcialLayout` carries the identical lag at `:196`–`205` (`placementY = extent.m_y` `:196`, `placementY -= itemExt.m_y` `:200`).

Dead state in both copies: `maxSize` (`:42` / `:131`) and `currentSize` (`:44` / `:134`) are computed and never read; `GetDirection()` (`layout.h:59`) and `SetDirection()` (`layout.h:64`) have **no call sites** in `gui/` or the example, so `m_direction` is permanently `forward` (`layout.h:72`) and both `reversed` branches are unreachable as written.

Division hazard at `:65` (`:154` vertical): the divisor `flexSize / item->TmpSize().m_x` divides by the item's **min** size (`TmpSize` was seeded from it at `:43`), and `LayoutItem` zero-initialises `m_minSize` (`layoutitem.h:50`) — an item that never had `SetMinSize` called gives integer division by zero. Even with non-zero mins, `flexSize / Tmp` can round to 0 and then the outer `remaining / 0` traps. Neither divisor is guarded, and the `while` at `:58` has no iteration cap while its divisor is mutated at `:79`.

Two further gaps in the same function. **`position` is never read**: case-sensitive grep over `layout.cpp` finds the identifier only in the two signatures (`:31` `:120`), so both arrangements place from their own `(0,0)` and ignore the offset the caller supplied — a nested layout therefore places its children at the parent's coordinates rather than inside the parent. And the stray `;` at `:80` (`:169` vertical) puts a null statement after the `while` body. Separately, `Layout::Place` itself (`layout.h:53`) is `virtual void Place(Size position, Size extent){}//Do the placement calculation for contents` — the comment states the job, the body is empty, so a plain `Layout` root arranges nothing; only the `HorizontalLayout`/`VertcialLayout` overrides do.

### Ownership

`Layout::AddItem` (`layout.h:54`–`56`) takes a raw `LayoutItem*` and `emplace_back`s it into `std::vector<LayoutItem*> m_items` (`layout.h:71`). The call sites pass an owning ref: `layout->AddItem(button->GetWindow()->GetLayout())` `windowfactory.cpp:186`, where `Window::GetLayout()` returns `ref_of<LayoutItem>::type` (`window.h:65`) and `Ref<T>`'s implicit `operator T*()` (`src/qor/memory/reference/ref.h:150`) converts it. So `m_items` holds **non-owning aliases** into objects owned by each child's `Window::m_layout` (`window.h:81`). There is no `RemoveItem`, no `clear`, no way to drop a stale alias: if a child window dies before its parent's `m_items` is scanned, the scan dereferences freed memory, and nothing in the part prevents that.

The mirror-image cycle is in the leaf: `WindowLayoutItem`/`VBox` ctors take `Window*` (`windowlayoutitem.h:37` `vbox.h:37`) and store it raw in `m_window` (`windowlayoutitem.h:48` `vbox.h:42`), while the window owns the item through `m_layout`. The call sites pass `this` from inside the window's own ctor (`button.cpp:41`), so `m_window` points at an object whose derived members are not yet initialised; the ctors never dereference it, so this is latent rather than live.

### Installation sites

Layout **roots** — 2 sites, both the same class, both in `factories/windowfactory.cpp` (which includes `../view/layout/layout.h` at `:30`):

- `:150` `window->SetLayout(new_ref<VertcialLayout>());` inside `WindowFactory::Create(tstring className, ...)` `:145`;
- `:162` the same line inside `WindowFactory::CreateMain` `:157`.

The `WindowClass` overload at `:140` forwards to the string overload at `:142`, so both routes get a `VertcialLayout`. `Window::SetLayout` is `window.cpp:76`–`79` (`m_layout = layout;` at `:78`) and `GetLayout` `window.cpp:81`–`84` (`return m_layout;` at `:83`).

Layout **leaves** — 12 `windows/` `.cpp` files, 24 `SetLayout(new_ref<…>(this))` sites (two per file: default ctor and handle ctor), all `WindowLayoutItem` except one:

| File | `SetLayout` sites |
|---|---|
| `windows/a_h/{animation,button,edit,flatscrollbar}.cpp` | `:41` `:47` |
| `windows/a_h/{combobox,comboboxex,datetimepicker,header}.cpp` | `:41` `:48` |
| `windows/i_m/listbox.cpp` `windows/s_s/{scrollbar,static}.cpp` | `:41` `:48` |
| `windows/i_m/mdiclient.cpp` | `:41` `:48` — **`new_ref<VBox>(this)`**, the only `VBox` construction in the tree |

Each is followed immediately by `m_layout->SetMinSize(Size{100,100});//TEMP:` (e.g. `button.cpp:42`), and the item ctor itself sets `m_maxSize = Size{16384, 16384};` (`windowlayoutitem.cpp:34` `vbox.cpp:34`). So every built-in leaf is min 100×100 / max 16384×16384 — flexible by the algorithm's own test `max > min` (`layout.cpp:45`). `windows/a_h/header.cpp:41` `:48` is one of these, and `header.cpp` is **absent from `add_library`** (`gui/CMakeLists.txt` has no such entry) — the same TU part 40 flagged.

### What is live in the built example

The example calls `factory.Create` three times — `mainwindow.cpp:347` `:367` `:381` — and every route lands in `WindowFactory::Create` `:145`, so **each window in the running app gets a `VertcialLayout` with zero items**. The example calls none of `CreateButton`/`CreateEdit`/`CreateListBox`/`CreateComboBox` (grep over `mainwindow.cpp`: zero hits), so no `WindowLayoutItem` is ever constructed, `AddItem` never runs, `m_items` stays empty, and `Place` is never called. The four `AddItem` call sites (`windowfactory.cpp:186` `:208` `:230` `:252`) and the four `Place` call sites (`layout.cpp:101` `:115` `:190` `:204`) are the complete call surface of the engine, and none of them executes in the built example.

The hit-test comment in the example states the intent — `mainwindow.cpp:97`–`98`, "Interrogate layout to determine the Regions for the various elements / Each layout element should have a hit test value and we return the one of the first element hit" — and the code right below it (`:102`–`139`) does **not** consult the layout: it takes `controller->GetRect(r)` `:103`, tests `r.IsPointInRect(Point(x,y))` `:104`, then branches on hard-coded `+ 31` `:108` and `+ 5` `:110` `:114` offsets from `r.m_top`/`r.m_left`. Layout-driven hit testing is the stated goal, not the behaviour.

`LayoutItem`'s contract question from the summary is now answered: a layout does **not** partition a window into regions. `Place` hands a position/size to each item, and only `WindowLayoutItem` acts on it, by repositioning a real child HWND through `WindowController::SetPos`. No handler anywhere consults a layout — `NonClientRenderingHandler::OnNCCalcSize` is not a consumer; the example's override at `mainwindow.cpp:49` works on the SDK `NCCalcSizeParams` payload, not on `m_items`.

## The message vocabulary — `view/messages.h` (324 lines)

This file belongs to the part because it is the second place the **message parameter cracking** vocabulary lives, and it is the only header in `view/` that supplies the message ids the handlers switch on. It has no `.cpp` — every declaration is `static const` or `constexpr`, so each include site gets its own copy of the constants.

### Cracking macros (`:29`–`:38`, outside the namespace)

| Macro | `messages.h` | Expansion |
|---|---|---|
| `LoWord(l)` | `:29` | `(unsigned short)(((unsigned long long)(l)) & 0xffff)` |
| `HiWord(l)` | `:30` | `(unsigned short)((((unsigned long long)(l)) >> 16) & 0xffff)` |
| `LoByte(w)` | `:31` | `& 0xff` |
| `HiByte(w)` | `:32` | `>> 8 & 0xff` |
| `MakeWord(a,b)` | `:34` | low byte \| high byte `<< 8` |
| `MakeLong(a,b)` | `:35` | low word \| high word `<< 16` |
| `GetXLParam(lp)` | `:37` | `(int)(short)LoWord(lp)` |
| `GetYLParam(lp)` | `:38` | `(int)(short)HiWord(lp)` |

They are macro-style (unprefixed, parenthesised operands) and sit **before** the namespace opens at `:40`, so they leak into every TU that includes the header. Both spellings of the same operation are live in the tree: case-sensitive grep over `gui/**/*.cpp` finds **41** calls of `LoWord(`/`HiWord(` and **11** of `LOWORD(`/`HIWORD(`/`MAKELONG(`; no header under `src/` defines the all-caps forms, so those resolve to the Windows SDK headers. `view/handlers/interactive/ime.cpp:80`–`82` uses `LOWORD`/`HIWORD` while its sibling `parent.cpp:49`–`107` uses `LoWord`/`HiWord` — the same cracking operation, two vocabularies, one directory.

`GetXLParam`/`GetYLParam` are the signed pair for `EM_POSFROMCHAR`, and their only consumer in the tree is `editcontroller.cpp:140` `:141` — the macros are defined in this header and used 200 files away.

### Constants — 225 `static const unsigned short`, 19 `constexpr unsigned short`, 9 `static const int`, 3 `static const unsigned long`

All inside `qor::platform::win::gui::view` (`:40`–`:322`), tab-indented (unlike the four-space blocks of the other `view/` headers). Three groups:

**Message ids** (`wm*`, 225 declarations). Values match the Win32 message numbers, e.g. `wmCreate = 0x0001` `:43`, `wmDestroy = 0x0002` `:44`, `wmNCCalcSize = 0x0083` `:116`, `wmNCHitTest = 0x0084` `:117`, `wmNCPaint = 0x0085` `:118`, `wmParentNotify = 0x0210` `:210`, `wmDropFiles = 0x0233` `:232`, `wmThemeChanged = 0x031A` `:272`, `wmUser = 0x0400` `:281`, `wmApp = 0x8000` `:282`, `wmSetConsoleInfo = wmUser + 201` `:284`. `wmNull = 0x0000` `:42` is the sentinel handlers test for.

Two constants are named by number only — `wm93 = 0x0093` `:125`, `wm94 = 0x0094` `:126` — unnamed gaps in the SDK's message list, and `wmCommNotify = 0x0044` `:96` carries the trailing comment `// no longer suported` (source typo).

Aliases that share a value are deliberate first/last sentinels: `wmKeyFirst`/`wmKeyDown` both `0x0100` (`:141` `:142`), `wmMouseFirst`/`wmMouseMove` both `0x0200` (`:183` `:184`), `wmIMEComposition`/`wmIMEKeyLast` both `0x010F` (`:153` `:154`). But `wmMouseLast` collides with a real message by construction: `wmMouseHWheel = 0x020E` `:198` and `wmMouseLast = 0x020E` `:201` — same value, different meaning.

**Version guards** — two `#ifndef _WIN32_WCE` blocks, `:58`–`:62` (`wmQueryEndSession` `:59` `wmQueryOpen` `:60` `wmEndSession` `:61`) and `:121`–`:123` (`wmSyncPaint` `:122`); an `#if (_WIN32_WINNT >= 0x0600)` / `#elif >= 0x0500` / `#elif >= 0x0400 || _WIN32_WINDOWS > 0x0400` / `#else` chain at `:200`–`:208` giving four different values of `wmMouseLast` (0x020E / 0x020D / 0x020A / 0x0209). The `_WIN32_WCE` / `_WIN32_WINNT` macros are SDK-supplied and this header never includes the header that defines them — with neither defined, the WCE blocks compile **in** and the `#else` arm at `:207` is chosen, so the value of `wmMouseLast` silently depends on macro state this file does not control.

**Field offsets** `:287`–`293`, under the comment `//Window field offsets for GetWindowLong` `:286`: `gwlWndProc = -4` `gwlHInstance = -6` `gwlHwndParent = -8` `gwlIDwm = -12` `gwlStyle = -16` `gwlExStyle = -20` `gwlUserData = -21` — the negative indices you pass to `GetWindowLongW` to read a window's own fields back. Their only consumers in the tree are `view/handlers/parts/style.cpp:41` and `:48`, both `bool bExtended = ((wParam == gwlExStyle) ? true : false);` inside `StyleHandler::ProcessMessage` (`:32`) — the handler compares the incoming `wParam` against `-20` to decide whether a `WM_` style message is asking about the extended style. Six of the seven offsets are never read; `gwlWndProc` `gwlHInstance` `gwlHwndParent` `gwlIDwm` `gwlStyle` `gwlUserData` have no call site anywhere in `gui/`.

**`EndSession` flags** `:295`–`297` (`EndSessionLogOff = 0x80000000` `EndSessionCritical = 0x40000000` `EndSessionCloseApp = 0x00000001`) and **`nfr` format ids** `:299` `:300` (`nfrANSI = 1` `nfrUnicode = 2`).

**System-menu codes** `:302`–`:320`, the only `constexpr` block in the part: `scSize = 0xF000` `scMove = 0xF010` `scMinimize = 0xF020` `scMaximize = 0xF030` `scNextWindow = 0xF040` `scPrevWindow = 0xF050` `scClose = 0xF060` `scVScroll = 0xF070` `scHScroll = 0xF080` `scMouseMenu = 0xF090` `scKeyMenu = 0xF100` `scArrange = 0xF110` `scRestore = 0xF120` `scTaskList = 0xF130` `scScreenSave = 0xF140` `scHotKey = 0xF150` `scDefault = 0xF160` `scMonitorPower = 0xF170` `scContextHelp = 0xF180`. The `wm*` group is `static const` while this group is `constexpr` — two `const`-ness conventions in one file, and `static const` gives internal linkage per TU while `constexpr` at namespace scope is implicitly `const` (also internal). Functionally equivalent here, but the mixed spelling is the file's own inconsistency.

Guard: `QOR_PP_H_WINDOWS_GUI_VIEW_MESSAGES` `:25`/`:26`/`:324` — the **full** `VIEW_MESSAGES` spelling, unlike the `drawing/` and `layout/` guards, which drop segments (`QOR_PP_H_WINDOWS_GUI_POINT` `QOR_PP_H_WINDOWS_GUI_VBOX` `QOR_PP_H_WINDOWS_GUI_WINDOWLAYOUTITEM`). Three guard styles in one part.

### Reach of `messages.h`

**39 files** under `gui/` include it — 6 with the source-root-relative spelling `src/platform/os/windows/ui/gui/view/messages.h` at `:29` (the six `controllers/*controller.cpp` files: `combobox` `edit` `listbox` `mdiclient` `scrollbar` `static`), and 33 with relative spellings such as `../../messages.h` at `:28` (the `view/handlers/interactive/*` and `view/handlers/parts/*` files). No header in `gui/` includes it, which is why the message ids appear in handler/controller **implementations** only: `view/handlers/base.h` has zero `wm*` references, so the base handler dispatch is not driven by these constants.

Since `messages.h` has no `.cpp`, none of these names are exported; the constants are duplicated per TU — ~250 internal-linkage symbols compiled 39 times.

## Issues found (this part)

| # | Where | What |
|---|---|---|
| 1 | `layout.cpp:105`–`117` `:196`–`205` | **Off-by-one in the `reversed` branch**: `placementX -= itemExt.m_x` precedes the refresh of `itemExt.m_x`, so each item is offset by the previous item's width and the stack overhangs the right/bottom edge. Currently latent — `SetDirection` is never called — but the code is wrong as written. |
| 2 | `layout.cpp:65` `:154` | **Unguarded division**: `remaining / (flexSize / item->TmpSize().m_x)`; `TmpSize` is seeded from `GetMinSize()` and `LayoutItem` zero-initialises `m_minSize` (`layoutitem.h:50`), so an item with unset min size divides by zero. |
| 3 | `layout.cpp:51`–`54` `:132`–`135` | **Empty overflow block** `if(minSize > extent){ //Overflow }` — over-fill is silently accepted, and the `underflow` counterpart at `:81`–`85` `:162`–`166` is comment-only, so `Alignment` (`layout.h:37`–`42`) has no effect anywhere. |
| 4 | `layout.cpp:79` `:168` | `flexSize += totalDistributed` mutates the divisor inside the `while` loop at `:58` — termination depends on a value the loop also changes, and there is no iteration cap. |
| 5 | `layout.h:71` | `std::vector<LayoutItem*> m_items` holds **non-owning aliases** derived from owning refs at `windowfactory.cpp:186` `:208` `:230` `:252`; no removal API, so a dead child window leaves a dangling entry the next `Place` scan dereferences. |
| 6 | `windowlayoutitem.h:37` `vbox.h:37` | Both ctors take `Window*` and are called with `this` from inside the window's ctor (26 sites, e.g. `button.cpp:41`): the stored `m_window` points at a not-yet-constructed object. Latent (ctors don't dereference) but the reverse of the ownership direction the rest of the library uses (`ref_of`). |
| 7 | `rect.h:46` `rect.cpp:67`–`71` | **Exported signature with a non-exported parameter type**: `Rect` carries `qor_pp_module_interface(QOR_WINGUI)` (`rect.h:33`) but `Point` does not, and `IsPointInRect(const Point&)` crosses the DLL boundary in an exported class. |

| 8 | `rect.cpp:52` `:85` `:92` | `FromIntersection`/`FromSubtraction`/`FromUnion` default-construct a local `Rect r;` (four uninitialised `long`s) and pass its address to the API; only the API filling it makes the result defined, and collapsing `BOOL` to `bool` makes a failed intersection indistinguishable from an empty one. |
| 9 | `point.h:31` `size.h:31` | SDK drop-in compatibility rests on member order plus `long` widths with **no** `static_assert` and only the comment `//All types on this interface must be portable`; `mainwindow.cpp:53`–`56` reads the SDK payload `rgrc[0..2]` as `Rect*` with no cast, so a layout change in either record silently corrupts hit testing. |
| 10 | `vbox.cpp:28` `rect.cpp:28` | Unused includes — `vbox.cpp`'s only statement is `m_maxSize = Size{16384, 16384};` (`:34`), so it pulls `controllers/windowcontroller.h` for nothing; `rect.cpp` pulls `common/stringconv.h` for nothing. |
| 11 | `layout.h:44` | `enum Direction { forward, reversed, }` — trailing comma before `}`, while `enum Alignment` at `:37` has none. Legal, inconsistent. |
| 12 | `windowlayoutitem.h:41`–`44` | Four redundant overrides (`GetMinSize` `GetMaxSize` `SetMinSize` `SetMaxSize`) with bodies identical to `LayoutItem`'s (`layoutitem.h:40`–`43`) — only `Place` differs, so the re-declarations add nothing and hide the base contract. |
| 13 | `messages.h:29`–`38` | Eight unprefixed function-like macros defined outside any namespace, duplicating the SDK's `LOWORD`/`HIWORD`/`MAKELONG` in meaning; 41 `gui/` call sites use the QOR spellings and 11 use the SDK spellings, so both live in the same tree. `view/handlers/interactive/ime.cpp:80`–`82` vs `parent.cpp:49`–`107` is the collision inside one directory. |
| 14 | `messages.h:200`–`208` | `_WIN32_WINNT`/`_WIN32_WCE` gate `wmMouseLast` (four different values) and two other `wm*` declarations, but the header never includes the header that defines those macros — undefined macros select the `#else` arm at `:207` (`0x0209`) silently. |
| 15 | `messages.h:198` `:201` | `wmMouseHWheel` and `wmMouseLast` both `0x020E` — same value, different meaning; a `switch` across them is a duplicate-case collision. |
| 16 | `messages.h:125` `:126` | `wm93 = 0x0093` `wm94 = 0x0094` — the only two identifiers in the file named by value rather than meaning. |
| 17 | `gui/CMakeLists.txt` | `windows/a_h/header.cpp` installs a `WindowLayoutItem` (`:41` `:48`) but has no `add_library` entry — a layout leaf defined in a TU that never links. |
| 18 | `windowfactory.cpp:150` `:162` | The only layout roots are `VertcialLayout` with zero items; the four `AddItem` sites sit in `CreateButton`/`CreateEdit`/`CreateListBox`/`CreateComboBox`, which the built example never calls — the arrangement pass never runs in the running app. |
| 19 | `layout.cpp:31` `:120` | **`position` parameter is never used** in either arrangement body (grep finds the identifier only in the two signatures), so children are always placed from `(0,0)` of the container's own space — nested layouts land on top of each other. |
| 20 | `layout.cpp:80` `:169` | `while(...){ ... };` — a stray null statement after the loop body, in both copies. |

## Cross-part questions raised

- **For part 50 (view handlers):** the `wm*` ids and the `LoWord`/`HiWord` pair are this header's, yet `view/handlers/base.h` contains no `wm*` reference at all — where does the base handler's `switch` get its message values? Also: `StyleHandler::ProcessMessage` (`style.cpp:32`) compares `wParam == gwlExStyle` (`:41` `:48`) — a *field index* (`-20`) used as a *message discriminator*, which reads as a placeholder rather than the intended `WM_` id.
- **For part 30 (controllers):** `WindowController::SetPos(nullptr, x, y, cx, cy, 0)` (`windowcontroller.h:211`) is the sole sink of a layout placement. Does any controller call `SetPos` for other reasons, and is `insertAfter = nullptr` correct there?
- **For part 20 (factories):** `CreateButton`/`CreateEdit`/`CreateListBox`/`CreateComboBox` are the only `AddItem` call sites and no caller uses them — is the child-window route ever intended to be taken, or is the layout tree permanently vestigial in the current build?
- **For part 40 (window types):** `header.cpp`'s absence from `add_library` and its `SetLayout` sites should be reconciled there.
- **For part 90 (example):** the hit-test override at `mainwindow.cpp:97`–`139` documents layout-driven region hit testing but hard-codes `+31`/`+5` offsets instead — the example is where the layout engine was meant to be used.

## Next

Part 80 closes the last `summary` part of the library: everything except **00 — architecture overview**, which is kept high level by design (`view/handlers/` 74 files are part 50's, so there is no part 81). The open work is the cross-part questions above, in priority order:

1. **Part 50 follow-up** — `StyleHandler::ProcessMessage` (`style.cpp:32`) compares `wParam == gwlExStyle` (`:41` `:48`), i.e. a `GetWindowLong` **field index** used as a message discriminator. Confirm against `50-view-handlers.md` whether the intended id was `wmStyleChanging` (`messages.h:109`) / `wmStyleChanged` (`:110`).
2. **Part 30 follow-up** — `WindowController::SetPos(nullptr, x, y, cx, cy, 0)` (`windowcontroller.h:211`) is the layout engine's only sink; check the sibling `SetPosTop`/`SetPosBottom`/`SetPosTopmost`/`SetPosNoTopmost` (`:212`–`:215`) and whether `insertAfter = nullptr` is the right Z-order argument.
3. **Part 20 follow-up** — whether `CreateButton`/`CreateEdit`/`CreateListBox`/`CreateComboBox` (`windowfactory.cpp:171` `:192` `:214` `:236`) were ever meant to be called; they are the only `AddItem` sites.
4. **Part 40 follow-up** — `windows/a_h/header.cpp` absent from `add_library` alongside its `SetLayout` sites (`:41` `:48`).





