# 40 — Window types: the C++ type for each built-in Windows window class

**Status: detailed.** All 30 files (15 header/implementation pairs) were read
in full, along with their consumer edges (`controllers/`, `factories/`,
`examples/`) and the `add_library` list in `gui/CMakeLists.txt`.

## Files in this part (30)

Line counts, so a session can be sized against them.

| Pair | Lines (h / cpp) | Shape |
|---|---|---|
| `animation` | 48 / 51 | `Window` subclass, min size only |
| `button` | 88 / 51 | `Window` subclass + `BS_*` style vocabulary + `N*` notifications |
| `combobox` | 48 / 53 | `Window` subclass, styles/notification placeholders commented out |
| `comboboxex` | 48 / 53 | same as `combobox`; no controller anywhere |
| `datetimepicker` | 48 / 53 | same; no controller anywhere |
| `desktop` | 37 / 35 | **not** a `Window` — empty `class Desktop {};`, includes nothing |
| `dialog` | 153 / 97 | **not** a `Window` — own `Handle`, own `DlgProc`, wire structs |
| `edit` | 71 / 53 | `Window` subclass + live `S*` styles + `N*` notifications |
| `flatscrollbar` | 43 / 53 | bare `Window` subclass, no vocabulary at all |
| `header` | 43 / 53 | bare `Window` subclass — `.cpp` **not in `add_library`** |
| `listbox` | 48 / 53 | `Window` subclass, vocabulary commented out |
| `mdiclient` | 50 / 53 | `Window` subclass, the only pair using a `VBox` layout |
| `messagebox` | 103 / 60 | **not** a `Window` — imperative call object, `MB_*` vocabulary, `MsgBoxParams` |
| `scrollbar` | 59 / 53 | `Window` subclass + enable/disable state constants |
| `static` | 88 / 53 | `Window` subclass + `S*` styles + "Control Mesages" constants; **no `virtual ~`** |

All live in `qor::platform::win` (unlike the handler classes, which are in
`qor::platform::win::gui::view`).

## What this part is for

It is the C++ type for each built-in Windows window class. Confirmed shape:
12 of the 15 pairs are `Window` subclasses whose only job is to **set the layout
default sizes** for that class; 3 pairs (`Dialog`, `MessageBox`, `Desktop`) are
not `Window` subclasses at all. All live in `qor::platform::win`
(`animation.h:31`–`33`, `dialog.h:34`–`36`, `messagebox.h:31`–`33`).

## Level 1 — the uniform `Window` subclass (12 pairs)

Every one of these declares the same triple and nothing else:

```cpp
class qor_pp_module_interface(QOR_WINGUI) ComboBox : public Window   // combobox.h:32
{
public:
    ComboBox();
    ComboBox(const PrimitiveHandle& h);
    virtual ~ComboBox() = default;
};
```

```cpp
ComboBox::ComboBox() : Window()                       // combobox.cpp:39-44
{
    SetLayout(new_ref<WindowLayoutItem>(this));
    m_layout->SetMinSize(Size{300,10});               //TEMP:
    m_layout->SetMaxSize(Size{500,500});
}
ComboBox::ComboBox(const PrimitiveHandle& h) : Window(h)   // combobox.cpp:46-51
{ /* identical body */ }
```

Facts about that body:

- `: Window()` / `: Window(h)` is the **base-class initialiser** — the subclass
  never adds a handle of its own; the handle semantics are `Window`'s
  (`window.h:55` `Window(const PrimitiveHandle& h)`, `window.h:59`
  `SetHandle(const PrimitiveHandle&)`). No class in this part calls
  `CreateWindowExT` — only the factory does.
- `SetLayout` and `m_layout` belong to `Window`, not to the subclass:
  `window.h:64` `void SetLayout(qor::ref_of<...::view::LayoutItem>::type)`,
  `window.h:65` `GetLayout()`, `window.h:81` `m_layout`. So each subclass
  constructor **overwrites the base `Window`'s layout** immediately after
  construction.
- Each of the 12 `.cpp` files pulls the layout type by local include at line 30
  (`../../view/layout/windowlayoutitem.h`) — except `mdiclient.cpp:30`
  (`../../view/layout/vbox.h`). All 12 also include
  `../../view/handlers/base.h` at line 29 but **use nothing from it**: no
  handler is attached here (compare part 20, where the factory attaches the
  handler at `windowfactory.cpp:149`). The two non-uniform pairs that compile,
  `desktop.cpp` and `messagebox.cpp`, include neither header.
- The `//TEMP:` marker on the min-size line is present in all 12 bodies —
  these defaults are provisional by the author's own note, not tuned values.

Default sizes as coded (min / max; blank = no `SetMaxSize` call):

| Class | Min | Max | Cite |
|---|---|---|---|
| `Animation` | 100×100 | — | `animation.cpp:42` |
| `Button` | 100×100 | — | `button.cpp:42` |
| `ComboBox` / `ComboBoxEx` / `DateTimePicker` / `Edit` / `FlatScrollBar` / `Header` / `ListBox` | 300×10 | 500×500 | e.g. `combobox.cpp:42`–`43` |
| `ScrollBar` | 10×10 | 500×500 | `scrollbar.cpp:42`–`43` |
| `Static` | 10×10 | 500×500 | `static.cpp:42`–`43` |
| `MDIClient` | 64×64 | 16384×16384 | `mdiclient.cpp:42`–`43` |

Two deviations from the uniform triple:

- `Desktop` has no constructors, no base, no destructor (`desktop.h:31`
  `class Desktop {};`) and `desktop.h` includes **no** header at all.
- `Static` declares the two constructors (`static.h:81`–`82`) but **no**
  `virtual ~Static()`. A repo-wide grep for `virtual ~` across this part hits
  11 headers only — `dialog.h`, `messagebox.h`, `desktop.h`, `static.h` have
  none. Behaviour is still correct (the inherited `Window` destructor is
  virtual), but the file breaks the pattern the other 11 follow.

`qor_pp_module_interface(QOR_WINGUI)` is not a decorator:
`src/configuration/configuration.h:33` defines it as
`qor_pp_if(qor_pp_is_empty(_X), qor_pp_import, qor_pp_export)`, so a non-empty
module token makes the declaration *exported* from the module interface.
`Desktop` is the one class in this part that carries no such marker.

## Level 2 — the style and notification vocabularies

Only six of the 15 headers carry **live** constants; the rest hold commented-out
placeholders that were never filled in.

| Header | Live constants | Commented-out placeholder |
|---|---|---|
| `button.h` | 28 `BS*` at `:35`–`62` (`constexpr unsigned long`, namespace scope); 10 `N*` at `:69`–`78` (`static const int`, class scope) | none |
| `edit.h` | 14 `S*` at `:36`–`49`; 12 `N*` at `:51`–`62` | none |
| `static.h` | 31 `S*` at `:38`–`69`; 8 "Control Mesages" at `:72`–`79` | none |
| `scrollbar.h` | 9 enable/disable state values at `:42`–`49` | none |
| `dialog.h` | 15 `DS*` at `:104`–`118` | none |
| `messagebox.h` | 37 `MB*` at `:33`–`69` | none |
| `combobox.h` / `comboboxex.h` / `datetimepicker.h` / `listbox.h` | none | `//static const unsigned long SLeft = 0x0000;` (`:36`) and `//static const unsigned short NSetFocus = 0x0100;` (`:39`) |
| `animation.h` / `mdiclient.h` | none | `//static const int NClicked = 0;` (`animation.h:38`, `mdiclient.h:40`) |
| `flatscrollbar.h` / `header.h` / `desktop.h` | none | not even a placeholder |

What the live vocabularies say, as the code states them:

- **`button.h` `BS*`** — a mirror of the Win32 button-style mask:
  `BSTYPEMASK 0x0000000F`, `BSLEFTTEXT 0x20`, `BSICON 0x40`, `BSBITMAP 0x80`,
  the placement group `BSLEFT/BSRIGHT/BSCENTER/BSTOP/BSBOTTOM/BSVCENTER`,
  `BSPUSHLIKE 0x1000`, `BSMULTILINE 0x2000`, `BSNOTIFY 0x4000`, `BSFLAT 0x8000`.
  Windows API intent: dialog `BUTTON` item styles. Aliases as coded:
  `BSRIGHTBUTTON = BSLEFTTEXT` (`button.h:62`), and `BSTEXT`/`BSPUSHBUTTON`
  both `0x00000000`.
- **`button.h` `N*`** — notification *indices*, not `WM_*` codes:
  `NClicked 0 … NKillFocus 7`, with `NPushed = NHilite` (`:75`) and
  `NUnpushed = NUnhilite` (`:76`). Nothing in this part consumes them, and
  `button.cpp` defines only the two constructors — so there is **no
  out-of-line definition** for these class-scope `static const int`s; ODR-use
  (binding to a reference, taking an address) would be a link error.
- **`edit.h`** — `SLeft…SNumber` (`:36`–`49`) mirror the Win32 edit styles
  (`SMultiline 0x4`, `SPassword 0x20`, `SAutoVScroll 0x40`, `SReadOnly 0x800`,
  `SWantReturn 0x1000`, `SNumber 0x2000`); `NSetFocus 0x0100 … NAfterPaste
  0x0801` (`:51`–`62`) are `WME_*`-shaped edit messages. Windows API intent.
- **`static.h` "Static Control Mesages"** (the header's own typo)
  `SetIcon 0x0170`, `GetIcon 0x0171`, `SetImage 0x0172`, `GetImage 0x0173`,
  then `Clicked 0`, `DblClk 1`, `Enable 2`, `Disable 3`. The 0x017x group is
  spelled as `WM_*`-shaped values, the 0…3 group as indices — **the two groups
  are not the same kind of number**, and nothing here uses either.
- **`scrollbar.h`** — `EnableBoth 0x0000`, `DisableBoth 0x0003`,
  `DisableLeft 0x0001`, `DisableRight 0x0002`, `DisableUp 0x0001`,
  `DisableDown 0x0002`, plus aliases `DisableLeftUp = DisableLeft` and
  `DiableRightDown = DisableRight` (`scrollbar.h:49` — typo in the identifier).
  Note `DisableLeft == DisableUp` and `DisableRight == DisableDown`: the
  horizontal and vertical pairs **share the same values**, so one mask cannot
  tell them apart.
- **`messagebox.h` `MB*`** — the `MessageBox` `uType` flags and their masks
  (`MBTYPEMASK 0xF`, `MBICONMASK 0xF0`, `MBDEFMASK 0xF00`, `MBMODEMASK 0x3000`,
  `MBMISCMASK 0xC000`), with aliases `MBICONWARNING = MBICONEXCLAMATION`,
  `MBICONERROR = MBICONHAND`, `MBICONINFORMATION = MBICONASTERISK`,
  `MBICONSTOP = MBICONHAND`. Overlaps to watch: `MBTOPMOST` and
  `MBSERVICE_NOTIFICATION_NT3X` are **the same bit** (`0x00040000`), and
  `MBRIGHT 0x00080000` sits inside `MBMISCMASK`.
- **`dialog.h` `DS*`** — `DSABSALIGN 0x01`, `DSSYSMODAL 0x02`, `DS3DLOOK 0x0004`,
  `DSFIXEDSYS 0x0008`, `DSNOFAILCREATE 0x0010`, `DSLOCALEDIT 0x20`, `DSSETFONT
  0x40`, `DSMODALFRAME 0x80`, `DSNOIDLEMSG 0x100`, `DSSETFOREGROUND 0x200`,
  `DSCONTROL 0x400`, `DSCENTER 0x800`, `DSCENTERMOUSE 0x1000`, `DSCONTEXTHELP
  0x2000`, and `DSSHELLFONT (DSSETFONT | DSFIXEDSYS)` (`dialog.h:118`).
  Two carry 16-bit-era comments (`DSLOCALEDIT` "meaningless" in 32-bit,
  `DSSETFOREGROUND` "not in win3.1").

## Level 3 — the three non-uniform pairs

### `Dialog` — the only real logic in this part

`Dialog` is **not** a `Window` (`dialog.h:120`); it owns a `Handle`
(`dialog.h:143`, the `Handle` type at `src/platform/os/windows/common/handles/
handle.h:50`) and carries its own message procedure.

Wire records, byte-packed by `#pragma pack(push,1)` (`dialog.h:38`, closed
`dialog.h:101`) so the layout matches the Win32 dialog template byte for byte:

| Struct | Lines | Fields as coded |
|---|---|---|
| `DlgTemplateEx` | `dialog.h:39`–`61` | `dlgVer, signature, helpID, exStyle, style, cDlgItems, x, y, cx, cy` — then a commented-out block (`:51`–`60`) for `menu[1]`, `windowClass[1]`, `title[1]`, `pointsize`, `weight`, `italic`, `charset`, `typeface[1]` the model deliberately omits |
| `DlgItemTemplateEx` | `dialog.h:63`–`78` | `helpID, exStyle, style, x, y, cx, cy, id` — commented-out `windowClass[1]`, `title[1]`, `extraCount` |
| `DlgTemplate` | `dialog.h:80`–`89` | `style, dwExtendedStyle, cdit, x, y, cx, cy` — the non-extended variant |
| `DlgItemTemplate` | `dialog.h:91`–`100` | `style, dwExtendedStyle, x, y, cx, cy, id` (`id` is `unsigned short` here, `unsigned long` in the Ex variant) |

Construction (`dialog.cpp:72`–`80`) makes no window of its own:

```cpp
HINSTANCE instance = Kernel32::GetModuleHandleW(nullptr);
m_result = User32::DialogBoxIndirectParamT(
    instance, reinterpret_cast<LPCDLGTEMPLATE>(t),
    (HWND)(parent.IsNotNull() ? parent->GetHandle().Use() : nullptr),
    (DLGPROC)(Dialog::DlgProc), reinterpret_cast<long long>(this));
```

The QOR object travels into the API as `dwInitParam`, and `Dialog::DlgProc`
recovers it — the dialog analogue of the `"QOR"` window property the factory
uses for ordinary windows (part 20, `windowfactory.cpp:51`/`60`).

`Dialog::DlgProc` (`dialog.cpp:38`–`70`, `qor_pp_compiler_stdcallconvention` →
`__stdcall`, `src/macros/.../msvc.h:36`):

| `msg` | Effect |
|---|---|
| `WM_INITDIALOG` (`:42`–`51`) | `lParam` → `Dialog*`; `User32::SetPropT(hDlg, TEXT("QOR"), (HANDLE)dlg)`; return `dlg->OnInit()` |
| `WM_COMMAND` (`:52`–`62`) | `LOWORD(wParam)`; only `IDOK`/`IDCANCEL` handled → `GetPropT` → `EndDialog(hDlg, dlg ? dlg->OnEnd(wId) : wId)`, return 1 |
| `WM_CLOSE` (`:63`–`67`) | `EndDialog(hDlg, dlg ? dlg->OnEnd(IDCANCEL) : IDCANCEL)`, return 1 |
| anything else | falls through to `return 0` (`:69`) — not handled |
Two code notes on that procedure: the `WM_CLOSE` case has **no braces**
(`dialog.cpp:63`–`67`), so its declarations sit directly in the `switch`
scope rather than in a case block; and `dialog.h:31` carries
`//TODO: include controls that can be in dialogs`, which is why `dialog.h:29`
includes `button.h` and `dialog.h:30` includes `../s_s/static.h` — the only
reason classes in this part reference each other at all.



`OnInit()` (`dialog.cpp:87`–`90`) returns 1; `OnEnd(int id)` (`:92`–`95`)
returns `id`. Both are `virtual` (`dialog.h:129`–`130`) — this is the
customisable dialog hook, and those two bodies are the whole implementation.

The rest of the declared surface is barely used:

- `GetHandle()`/`SetHandle()` (`dialog.h:126`–`127`) — the constructor never
  assigns `m_handle`, so `GetHandle()` on a `Dialog` returns a default-
  constructed handle.
- `WAlign(unsigned short*, int)` (`dialog.h:134`–`137`) — an inline static
  round-up helper with **no call site anywhere** (a grep for `WAlign` hits the
  declaration only).
- `WriteString(const char*, wchar_t*)` (`dialog.cpp:82`–`84`) —
  `Kernel32::MultiByteToWideChar(CP_ACP, 0, src, -1, dest, 50)`; the destination
  capacity is hardcoded at 50 wide chars and the caller passes no size, so a
  longer source overflows the caller's buffer. Also uncalled in this part.
- `DialogController` (`dialog.h:36`) is a **forward declaration only** — the
  sole occurrence repo-wide; no definition, no use.

### `MessageBox` — an imperative call object

Two constructors (`messagebox.h:91`–`92`), no default constructor, no handle,
no destructor declaration, one accessor:

- `MessageBox(window, text, caption, type, milliseconds = 0)`
  (`messagebox.cpp:37`–`48`): `milliseconds == 0` → `User32::MessageBoxT`
  (`:41`), otherwise `User32::MessageBoxTimeout` (`:46`) with
  `languageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL)` computed at `:45`.
  The window argument is optional-in-effect: `window.IsNotNull() ? … : nullptr`.
- `MessageBox(MsgBoxParams&)` (`messagebox.cpp:50`–`53`) →
  `MessageBoxIndirectT(reinterpret_cast<const LPMSGBOXPARAMS>(&params))`.
- `Result()` (`messagebox.h:94`, `messagebox.cpp:55`–`58`) returns `m_result`.

`MsgBoxParams` (`messagebox.h:73`–`85`) is the QOR mirror of `MSGBOXPARAMS`:
`cbSize{sizeof(MsgBoxParams)}` self-sized, `hwndOwner`/`hInstance` as `void*`
(the QOR handle convention of part 10), `dwContextHelpId` `unsigned long long`,
`lpfnMsgBoxCallback` the `MsgBoxCallback` typedef (`:71`). The `type`
parameter is `unsigned int` while the `MB*` constants are `unsigned long` — a
silent narrowing at the call, harmless at these values.

### `Desktop` — a placeholder

`desktop.h:31` `class Desktop {};` — no base, no members, no member functions,
no include of `window.h` (it is the only header in this part that includes
nothing), and no `qor_pp_module_interface` marker. `desktop.cpp`'s namespace
body (`:33`–`34`) is empty. Windows API intent: the `DESKTOP` window class.
Nothing implements that intent.

## Who consumes these types

Include edges, verified by grep over `src/`, `examples/`, `test/`:

| Header | Included by |
|---|---|
| `animation.h` | `controllers/a_h/animationcontroller.h:29` |
| `button.h` | `controllers/a_h/buttoncontroller.h:29`, `factories/windowfactory.h:36` |
| `combobox.h` | `controllers/a_h/comboboxcontroller.h:29` |
| `edit.h` | `controllers/a_h/editcontroller.h:29` |
| `listbox.h` | `controllers/i_m/listboxcontroller.h:29` |
| `scrollbar.h` | `controllers/s_s/scrollbarcontroller.h:29` |
| `static.h` | `controllers/s_s/staticcontroller.h:29`, `dialog.h:30` (`../s_s/static.h`) |
| `dialog.h` | `factories/dialogbuilder.h:33`, `examples/.../dialogs/dialogs.cpp:28` |
| `messagebox.h` | `examples/.../button/button.cpp:30`, `dialogs/dialogs.cpp:27`, `editbox/editbox.cpp:30` |
| `mdiclient.h` | `controllers/i_m/mdiclientcontroller.h:29` — written `"../windows/mdiclient.h"`, which resolves to `controllers/windows/mdiclient.h` and **does not exist**; that TU is not in `add_library` |
| `comboboxex.h`, `datetimepicker.h`, `desktop.h`, `flatscrollbar.h`, `header.h` | **nothing** — no consumer anywhere |

Instantiation, i.e. which of these the library actually constructs
(`factories/windowfactory.cpp`):

| Factory method | Constructs | Class name passed to `CreateWindowExT` |
|---|---|---|
| `CreateButton` (`:171`–`189`) | `ButtonController(new_ref<Button>(…))` at `:174`–`176` | `qor_pp_text("BUTTON")` (`:176`), `style \| WS_CHILD \| WS_VISIBLE` |
| `CreateEdit` (`:192`–`211`) | `EditController(new_ref<Edit>(…))` at `:195` | `qor_pp_text("EDIT")` (`:198`), `style \| WS_BORDER \| WS_CHILD \| WS_VISIBLE \| WS_VSCROLL` |
| `CreateListBox` (`:214`–`233`) | `ListBoxController(new_ref<ListBox>(…))` at `:217` | `qor_pp_text("LISTBOX")` (`:220`) |
| `CreateComboBox` (`:236`–`255`) | `ComboBoxController(new_ref<ComboBox>(…))` at `:239` | `qor_pp_text("COMBOBOX")` (`:242`) |

Each then does `layout->AddItem(x->GetWindow()->GetLayout())` (`:186`, `:208`,
`:230`, `:252`) — the controller's layout adopts the **window subclass's**
layout, i.e. the min/max sizes set in this part are what the parent layout sees.
The remaining classes are created only by a caller naming the class string
(`Create` at `windowfactory.cpp:145`, `CreateMain` at `:157`) — no QOR type is
involved, which is why `Animation`, `ScrollBar`, `Static`, `MDIClient`,
`ComboBoxEx`, `DateTimePicker`, `Header`, `FlatScrollBar`, `Desktop` have types
but no construction path.

## Build state (`gui/CMakeLists.txt`)

14 of the 15 `.cpp` files are in the `add_library(${target} SHARED …)` list —
`windows/a_h/animation.cpp` `:8`, `button.cpp` `:9`, `combobox.cpp` `:10`,
`comboboxex.cpp` `:11`, `datetimepicker.cpp` `:12`, `desktop.cpp` `:13`,
`dialog.cpp` `:14`, `edit.cpp` `:15`, `flatscrollbar.cpp` `:16`, then
`windows/s_s/static.cpp` `:17`, `windows/i_m/messagebox.cpp` `:18`,
`windows/i_m/listbox.cpp` `:19`, `windows/s_s/scrollbar.cpp` `:20`,
`windows/i_m/mdiclient.cpp` `:21`. **`windows/a_h/header.cpp` is absent** — the
`Header` class has no compiled translation unit, so anything that ever
constructs one is a link error. The ordering at `:17`–`21` breaks the
alphabetical grouping the rest of the list follows.

Note the split between the two `mdiclient` files: `windows/i_m/mdiclient.cpp`
**is** compiled (`:21`) while `controllers/i_m/mdiclientcontroller.cpp` is not
(part 30, "dead TU with broken includes") — and the controller's include of the
window header is written with the wrong relative path, so the pair is only half
alive.

## Answers to the questions that framed this part

1. **The common shape is not factory-mediated.** A `windows/` class is a plain
   `Window` subclass whose constructor sets layout min/max defaults; nothing
   registers it, and **no `windowclass_of` specialisation exists** for any of
   them (part 20 established the trait is declared at `windowclass.h:89`–`99`
   and never specialised). The factory reaches these types only for the four
   common controls it builds (`CreateButton`/`CreateEdit`/`CreateListBox`/
   `CreateComboBox`) and only as the inner object of a controller.
2. **No class here is pre-registered.** The only pre-registration in the
   library is `MainWindow`, per factory instance, in the factory constructor
   (`windowfactory.cpp:43`–`47`); `CreateButton` etc. pass a class-name string
   straight to `CreateWindowExT` without consulting the registration table
   (`windowfactory.cpp:176`, `198`, `220`, `242`). The `some_ideas.txt` intent
   that built-in classes be reportable as available is unimplemented.
3. **`header.cpp` not compiled is deliberate-looking but unexplained** — no
   comment, no other file references `Header`, and its header has no consumer.
   `mdiclient` is the half-alive pair described above.

Cross-part resolves recorded elsewhere:

- **`CompareItemStruct`** (part 30's follow-up) is defined at
  `src/platform/os/windows/common/structures.h:437`, visible here because every
  header in this part includes `../../window.h` and `window.h:32` includes
  `structures.h`. Used by `listboxcontroller.h:41` and `comboboxcontroller.h:53`.
- **`HelpInfo`** likewise: `structures.h:427`. `view/handlers/interactive/help.h:32`
  declares a second `HelpInfo` inside `qor::platform::win::gui::view` — two
  same-named structs in nested namespaces, which is why `help.cpp:43` and
  `messagebox.h:71` resolve to different ones.
- **`DialogBuilder`** (part 20) consumes `dialog.h:33` and its
  `DlgTemplateEx`/`DlgItemTemplateEx` records; `DialogHandler` (part 50) is a
  third, unrelated `Dialog` name — the dialog surface spans three parts and
  only this one has a working API call.

## Gotchas

1. **`Dialog::GetHandle()` never returns the dialog's handle** — the
   constructor assigns only `m_result` (`dialog.cpp:79`); `m_handle`
   (`dialog.h:143`) is left default. Separately, `m_result` (`dialog.h:147`,
   private) has **no accessor**, so the outcome of
   `DialogBoxIndirectParamT` cannot be read from outside the object.
2. **`Dialog::WriteString` hardcodes a 50-wide-char destination**
   (`dialog.cpp:84`) — overflow hazard for longer text.
3. **`DialogController` is declared and never defined** (`dialog.h:36`).
4. **`header.cpp` is not compiled** — the `Header` type is a declaration with
   no definition (`gui/CMakeLists.txt` has no `windows/a_h/header.cpp`).
5. **`button.h`'s `N*` constants have no out-of-line definitions** — class-scope
   `static const int` used by reference would fail to link.
6. **Two typos carried in identifiers/comments**: `DiableRightDown`
   (`scrollbar.h:49`) and "Static Control Mesages" (`static.h:71`).
7. **Value collisions in the vocabularies**: `MBTOPMOST` ==
   `MBSERVICE_NOTIFICATION_NT3X` (`messagebox.h:60`, `:64`), and
   `DisableLeft` == `DisableUp`, `DisableRight` == `DisableDown`
   (`scrollbar.h:44`–`47`).
8. **`#undef MessageBox` (`messagebox.cpp:33`) and `#undef MessageBox` /
   `#undef DialogBoxIndirectParam` (`dialog.cpp:33`–`34`)** before the
   User32-layer include — the same SDK-collision guard idiom as
   `windowfactory.cpp:38`–`39` (part 2 gotcha 6). No macro of those names
   exists in the vendored `src/`, `sdk/` or `3rdparty/` trees, so on this SDK
   they read as defensive no-ops.
9. **`Static` has no `virtual ~Static()`** (`static.h:81`–`82`) — the only
   `Window` subclass in this part that omits the destructor line.

## What the example actually exercises

Nothing. `examples/experimental/osspecific/windows/mainwindow/mainwindow.cpp`
contains no use of any type in this part — a grep for the class names hits only
`BitmapInfoHeader` (`:192`) and `style.SetSystemMenu` (`:353`, `:375`), neither
of which is a `windows/` type. The three files that do include these headers
(`button.cpp`, `dialogs.cpp`, `editbox.cpp`) belong to example directories
commented out of `examples/experimental/osspecific/windows/CMakeLists.txt`
(see the [index](README.md)), and the controllers that include them are
themselves never instantiated (part 30, Level 4). **Everything in this part is
read from source, not observed in a run.**

**Next:** [50 — view handlers](50-view-handlers.md) ·
[back to index](README.md)
