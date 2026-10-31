# Agent State

Current task
Latest closed part: **80 — geometry + layout + message vocabulary (12/12 files, 1,111 lines) — the library walk is complete**; 00 remains high level by design. Prior session entry (part 60c) retained below as history.
- 27 Oct 2026 — **part 80 (geometry + layout + message vocabulary) walked in full and closed: 12/12 files, 1,111 verified lines** `docs/ui/windows/60-gdiobjects.md` now carries `## 60c — resource leaves: bitmap brush colourspace cursor font icon palette pen region (detailed)` with a new `### Gotchas — what the 60c walk adds` subsection and a `Next: [70 — painting and theming]` footer, and the page status line reads `detailed — 60a + 60b + 60c = all 32 files (16 pairs)`; README, `memory-bank/activeContext.md` and `memory-bank/progress.md` synced. The walk re-read all 18 files (verified sizes `bitmap` 108/76, `brush` 81/144, `colourspace` 88/81, `cursor` 83/177, `font` 97/71, `icon` 94/133, `palette` 76/106, `pen` 60/99, `region` 80/140 = 1,794 lines) and corrected six off-by-one citations on the page (the `//All types…` comment is at `colourspace.h:32` `cursor.h:34` `icon.h:36` `palette.h:33` `pen.h:33` `region.h:34`; `GetBrushOrgEx` at `brush.cpp:84`; `menu.h:34`; `caret.h:42` · `caret.cpp:37`; `windowclass.cpp:140`) and restated the call-map split: **31 of the 77 distinct symbols are `User32::`** (`cursor` 16, `icon` 14, `GetSysColorBrush` 1; the other 46 `GDI32::`), with `cursor` and `icon` the two pairs that never reach the drawing layer. Headline 60c findings: two parallel ownership models in one directory — `Cursor` (`cursor.h:46`) and `Icon` (`icon.h:61`) are not `GDIObject` subclasses and hand-roll `m_handle`/`m_owning` closed by `User32::DestroyCursor`/`DestroyIcon` (`cursor.cpp:80`, `icon.cpp:83`), the other seven derive from `GDIObject` tagging 12 of the 15 `GDIObjectType` enumerators (`gdiobject.h:35`–`49`, `OExtPen`/`OMetaFile`/`OUnknown` unused); `DIBSection` (`bitmap.cpp:62`–`66`) and `SysBrush` (`brush.cpp:132`–`137`) assign a raw handle to `m_handle`, re-arming `m_needsClose` (`handle.cpp:84`) so the temporary's `~Handle` runs `Close()` (`handle.cpp:199`–`207`) → `Kernel32::CloseHandle` (`:204`) on a GDI handle before use; `Palette` (`palette.h:71`) and `ColourSpace` (`colourspace.h:83`) shadow `GDIObject::m_handle` with an uninitialised member (`handle.cpp:80` → `Invalid_Handle_Value` at `constants.h:8`) so every operation passes `(HPALETTE)(-1)`/`(HCOLORSPACE)(-1)`, including `DeviceContext::SelectPalette`/`ColourCorrectPalette`; 6 of 9 struct mirrors are correct, `LogBrush` is 16 bytes where `LOGBRUSH` is 12, `LogPen` swaps the Win32 kinds, and `LogColourSpace`'s `#if qor_pp_unicde` is misspelled at `colourspace.h:60` so the `char[260]` branch always wins while `GetLogColorSpaceT` (`colourspace.cpp:70`) writes 520 bytes into it; **zero factory static in any leaf has a call site repo-wide** — the only live construction is the `(PrimitiveHandle,bool)` ctor via `buttoncontroller.cpp:46,52,77,82,87,92` and `staticcontroller.cpp:53,58,70,76,82,94`, `DIBSection`/`SysBrush` are never instantiated, and the only live `CreateDIBSection` is `ui/gl/windowimpl.cpp:152`, bypassing the wrapper. New gotchas: `pen.h:44` is the only leaf header with **no access label** (all of `Pen` private), `Cursor`'s copy ctor shares the handle and clears ownership (`cursor.cpp:40`) while `Icon`'s clones and owns (`icon.cpp:39`), `Cursor`'s six global operations never touch `m_handle`, and `windowfactory.cpp:91` `Brush(COLOR_WINDOW)` (inside `AddWindowClass` `:78`–`98`) routes a colour/stock selector through `Brush(int)` → `GDIObject(colour, OBrush, true)` (`brush.cpp:40`), storing it as `WNDCLASSEX::hbrBackground` (`windowclassregistration.cpp:46`) and `DeleteObject`-ing it the same statement. Next: 70 painting (`gui/painting/`, 8 files). Prior 60b close: `docs/ui/windows/60-gdiobjects.md` also carries `## 60b — the metafile family: metadc enhmetadc metafile enhmetafile (detailed)` with file-size, level-1, level-2, construction/close, consumers, question-2 answer, build/include and gotchas subsections; every claim cited. Headline 60b findings: the family wraps **no GDI call at all** — the api layer is complete and unused (`gdi32.h:265`–`291` declares 26 metafile entry points, `api_layer/gdi/metafile.cpp` wraps exactly 26, zero called from `gui/`), so the purpose statement's "Off Screen Rendering with Composition to an Open GL target window" (`gui/purpose.md:7`) is **not** built on metafiles and question 2 is answered "it does not"; three of the four pairs are unreachable (`metadc.h`/`enhmetadc.h`/`metafile.h` included by nothing outside their own `.cpp`), `MetaFile` cannot be constructed (`metafile.h:34`–`37` empty body, `GDIObject` has no default ctor), `metafile.cpp:33`–`36` defines nothing yet is linked at `gui/CMakeLists.txt:52`; `EnhancedMetaFile` is the family's only construction site, in `StaticController` (`staticcontroller.cpp:61`–`65`/`:85`–`89`, `new_ref<EnhancedMetaFile>(…, false)`), and `StaticController` itself has no construction site repo-wide → compiled, linked, never run; `enhmetafile.h:42` `PrimitiveHandle m_handle` is a dead shadow of `GDIObject::m_handle` (`gdiobject.h:64`); default-constructed DCs are destructor-poisoned (`m_handle(0)`, `m_owner=true`, `Invalid_Handle_Value` = `(void*)(size_t)-1` at `constants.h:8` → `GDI32::DeleteObject((HGDIOBJ)0)`); the base's single `DeleteObject` close step is wrong for all four kinds (pairing `CloseMetaFile`/`CloseEnhMetaFile`/`DeleteMetaFile`/`DeleteEnhMetaFile` at `gdi32.h:267`/`:266`/`:273`/`:272` never called); `gdi32.h:266`–`267` mistypes that close pair as `HDC`; the `qor_pp_module_interface` marker is absent from all four headers. The 60a pass (the `gui/gdiobjects/` core trio `gdiobject` `devicecontext` `memdevicecontext`, 6 of 32 files) remains detailed with its own file-size, level-1/level-2, surface-group, construction/close, consumers, build and gotchas subsections; every claim cited. Part 50 remains detailed at 74/74 files and its walk detail is kept below as history (full text in `docs/ui/windows/50-view-handlers.md`). Part-50 confirmed: the chain is `AbstractWindowHandler`→`BaseWindowHandler`→`RenderedWindowHandler`→`InteractiveWindowHandler`→`TopLevelWindowHandler`, each middle level a short-circuit `||` chain over its part refs (`base.cpp:43`–`52`, `rendered.cpp:34`–`43`, `interactive.cpp:34`–`46`) with chain order ≠ declaration order at the `base` and `interactive` levels (the rendered level's order matches); `TopLevelWindowHandler` is the only decoder (11 ids at `toplevel.cpp:62`–`167`, each returning `true` unconditionally, so they are claimed and never reach the sub-chain); `ProcessHook` is non-virtual (`base.h:51`) and re-entered at all four levels, and the hook chain is empty because nothing repo-wide calls `HookController`; `MessageHandler` lives in `qor::platform::win` and owns the Win32 shell loop (`messagehandler.cpp:81`–`122`) whose accelerator route (`:104`) is never taken (`MessageLoop()` is always argument-less: `perthread.cpp:40`, `mainwindow.cpp:385`); `DialogHandler` derives from nothing and is never constructed, `MDIClientHandler` returns `false` on all paths and its `.cpp` is absent from `add_library`, `Hook` is `class Hook {};`; `scArrange` is the one `sc*` constant with no `OnSysCommand` case; only `OnQueryEndSession`, `OnSysCommandClose` and `OnCommand` do work among the 28 virtual hooks (`toplevel.h:61`–`89`), and `TopLevelLifetimeHandler::OnDestroy` (`toplevel.cpp:35`–`39`) posts `PostQuitMessage(EXIT_SUCCESS)` installed by `TopLevelWindowHandler::TopLevelWindowHandler()` (`toplevel.cpp:41`–`44`). README index synced. Part 40 remains `detailed` from the 26 Oct increment (details in `memory-bank/activeContext.md` and `docs/ui/windows/40-window-types.md`).
- 27 Oct 2026 — **part 80 (geometry + layout + message vocabulary) walked in full and closed: 12/12 files, 1,111 verified lines** (`view/drawing/` `point.h` 39 `rect.h` 57 `rect.cpp` 108 `size.h` 39 = 243; `view/layout/` `layout.h` 96 `layoutitem.h` 57 `vbox.h` 47 `windowlayoutitem.h` 53 `layout.cpp` 209 `vbox.cpp` 37 `windowlayoutitem.cpp` 45 = 544; `view/messages.h` 324). `docs/ui/windows/80-geometry-layout.md` replaced the summary with cited detail: 11-of-13 `Rect` members forward to `User32::` rect maths (`user32.h:447`–`457`), the lattice (`Layout : public LayoutItem` owning `std::vector<LayoutItem*> m_items`, `layout.h:33` `:71`), the arrangement pass line by line, ownership, the 13-file `windows/` leaf installation table, the 39-file `messages.h` include count, and a 20-row issue register. Headline: `Rect` (`rect.h:33`) is the part's **only** `qor_pp_module_interface(QOR_WINGUI)` export and `Rect::IsPointInRect(const Point&)` (`rect.h:46`, body `rect.cpp:67`–`71`) crosses the DLL boundary with a non-exported parameter type; the arrangement pass has four defects — the `reversed` branch subtracts the **previous** item's extent (`layout.cpp:111` before the refresh at `:114`, same at `:200`), **`position` is never read** in either `Place` body (identifier occurs only in the signatures `:31` `:120`), the overflow arm is an empty block (`:51`–`54` `:132`–`135`) with the underflow arm comment-only (`:81`–`85` `:162`–`166`) so `enum Alignment` (`layout.h:37`–`42`) is dead, and `flexSize / item->TmpSize().m_x` (`:65` `:154`) divides by a min size `LayoutItem` zero-initialises (`layoutitem.h:50`); `AddItem` keeps **non-owning aliases** (`windowfactory.cpp:186` `:208` `:230` `:252` via `Ref<T>::operator T*()`, `ref.h:150`) with no removal API; only `WindowLayoutItem::Place` (`windowlayoutitem.cpp:37`–`43`) acts, and it acts by **moving child HWNDs** through `WindowController::SetPos` (`windowcontroller.h:211`) — layouts never partition a window into regions and no handler consults one, so the summary's contract question is closed; the engine is compiled and linked but **never executed in the built app** (three `factory.Create` calls at `mainwindow.cpp:347` `:367` `:381` → `windowfactory.cpp:145` → an empty `VertcialLayout`; `CreateButton`/`CreateEdit`/`CreateListBox`/`CreateComboBox` are the only `AddItem` sites and are never called), while `mainwindow.cpp:97`–`139` documents layout-driven hit testing but hard-codes `+31`/`+5` (`:108` `:110` `:114` `:129`); `messages.h` = 225 `static const unsigned short` + 19 `constexpr unsigned short` + 9 `static const int` + 3 `static const unsigned long`, 8 unprefixed cracking macros outside the namespace (`:29`–`:38`) duplicating the SDK's `LOWORD`/`HIWORD`/`MAKELONG` (41 vs 11 call sites), two `#ifndef _WIN32_WCE` blocks (`:58`–`62` `:121`–`123`), a 4-arm `_WIN32_WINNT` chain for `wmMouseLast` (`:200`–`208`) without including the header that defines those macros, `wmMouseHWheel` and `wmMouseLast` both `0x020E` (`:198` `:201`), `wm93`/`wm94` named by value (`:125` `:126`), and 6 of 7 `gwl*` offsets never read (only `gwlExStyle` at `style.cpp:41` `:48`). `VertcialLayout` is misspelled at 9 sites in 4 files. README index, `memory-bank/activeContext.md` and `memory-bank/progress.md` synced. **No `summary` parts remain except 00 (high level by design)**; open items are now cross-part questions (part 50: `StyleHandler::ProcessMessage` compares `wParam == gwlExStyle` — a field index as a message discriminator; part 30: `SetPos(insertAfter = nullptr)` as the layout sink; part 20: whether the child-window route is ever intended to run).

- 27 Oct 2026 — **part 60b `gui/gdiobjects/` metafile family walked in full** (8 of 32 files: `metadc.h/.cpp` 48/47, `enhmetadc.h/.cpp` 48/47, `metafile.h/.cpp` 41/36, `enhmetafile.h/.cpp` 48/39 = 354 lines) and appended to `docs/ui/windows/60-gdiobjects.md` as `## 60b — the metafile family: metadc enhmetadc metafile enhmetafile (detailed)`; README index and status line synced, and the page's duplicated footer slip removed. Confirmed: `metadc.h`/`enhmetadc.h` are the same 48 lines modulo the class name — three-member `GDIObject` shells (`:41` default ctor, `:42` handle ctor, `:43` virtual dtor) whose `.cpp` bodies are `GDIObject(OMetaDC)`/`GDIObject(OEnhMetaDC)` at `:35`, `GDIObject(h, …, takeOwnership)` at `:39`, empty dtor `:43`–`45`; `metafile.h:34`–`37` is an **empty class** and `metafile.cpp:33`–`36` an empty namespace block linked at `gui/CMakeLists.txt:52`; `enhmetafile.h:38` `explicit EnhancedMetaFile(const PrimitiveHandle&, bool own = true)` with the dead `PrimitiveHandle m_handle` at `:42`. Only `enhmetafile.h` is included outside its own `.cpp` (`staticcontroller.h:32`), and `StaticController::GetEnhMetaFile`/`SetEnhMetaFile` (`staticcontroller.cpp:61`–`65`/`:85`–`89`) are the family's only construction sites — `new_ref` (`newref.h:24`–`28`) → `instancer_of<T>::Instance<T>(1, …)`, ownership `false` — but `StaticController` has no construction site repo-wide, so the path is compiled, linked, never executed. The family calls **no GDI function**: `api_layer/gdi/gdi32.h:265`–`291` declares 26 metafile entry points and `api_layer/gdi/metafile.cpp` wraps exactly 26, none called from `gui/`; the replay pair `PlayEnhMetaFile`/`PlayMetaFile` (`:284`/`:286`) has no caller, so question 2 is closed as "off-screen never composes to a GL target" and the live route stays `DeviceContext::CreateForDIB()` + `DIBSection`. Next: 60c (the nine resource pairs).
- 27 Oct 2026 — **final coherence check of `50-view-handlers.md`** done: all 20 root files + `gui/CMakeLists.txt` re-verified; chain, three `||` chains, `toplevel.cpp:62`–`167` decoder, `messages.h` sc* table + `scArrange:313`, orphans and citations hold. Fixed three build-facts slips — root `.cpp` compile count **8 of 9** (was "19 of the 20"), `parts/` cpp block `:64`–`:71` (was `:64`–`:72`), compile clause now "11 headers (2 header-only) + 9 `.cpp` (8 compiled)". Line-count table confirmed correct via editor line numbers.

- 27 Oct 2026 — **part 50a `gui/view/handlers/parts/` (18 files) walked in full** and appended to `50-view-handlers.md` as `## 50a — parts/ subtree (detailed)`. Confirmed: `part.h`/`part.cpp` is `BaseWindowPartHandler` with its OWN hook link `m_nextHookHandler` (`part.h:44`, `HookHandler` `part.cpp:37`–`49`, `ProcessHook` `:56`–`59`, non-virtual `ProcessMessage` `:51`–`54`) and **no caller repo-wide**, so every part's `:34` `ProcessHook` is a no-op; `LifetimeHandler` (`lifetime.h:48`–`64`, `lifetime.cpp:32`–`93`) claims eight ids (`wmNull` `:38`, `wmCreate` `:41`, `wmDestroy` `:50`, `wmActivate` `:55`, `wmMouseActivate` `:60`, `wmEnable` `:65`, `wmClose` `:74`, `0x0017` `:82`) each `return true`; the `OnDestroy` override question is **resolved** — `TopLevelLifetimeHandler::OnDestroy` (`toplevel.h:42`, private-by-default but still an override of the virtual `lifetime.h:61`) runs `PostQuitMessage(EXIT_SUCCESS)` at `toplevel.cpp:37`, reached via `toplevel.cpp:43` installing it into `base.h:56`'s slot; the four other concrete parts are one-to-three-message leaves (`basetext` `devicechange` `style` `timer` `user` `owner`) whose defaults either `return false` or forward `Window::DefWindowProcT` (`window.cpp:104`–`106`); `MenuHandler` is the largest leaf (11 cases `menu.cpp:38`–`129`, wParam-vs-lParam handle side flips, `wmMenuCommand` keeps a commented-out Windows 98/ME crack at `:110`–`119`); **only `m_lifetime` is ever installed in the built tree** (part-slot assignments repo-wide: `toplevel.cpp:43` and the unbuilt `menu/main.cpp:56`), so `BaseWindowPartHandler::ProcessMessage` is dead; `wmDeviceChange` and `wmEnterIdle` are stolen by the top level (`toplevel.cpp:152`–`156`, `:71`–`79`) so those part cases are unreachable; `parent.cpp:82`/`:87` `case wmCreate/wmDestroy` are inner `OnParentNotify` event codes, NOT message-id claims; `0x0017` is the only id in the subtree with no named constant (`messages.h` jumps `wmEndSession 0x0016` → `wmShowWindow 0x0018`).
- 27 Oct 2026 — **part 50b `gui/view/handlers/interactive/` (22 files) walked in full** and appended to `50-view-handlers.md` as four new subsections (hook surface, build & include facts, installation path, gotchas). Confirmed: 11 pairs of `BaseWindowPartHandler` subclasses, **75 claimed ids across 77 declared hooks** (only `parent` 9 hooks/3 ids and `winctrlparent` 9 hooks/13 ids diverge — its seven `wmCtlColor*` ids collapse onto three `Brush` hooks `winctrlparent.h:55`–`57`); all 11 `.cpp` compiled contiguously at `gui/CMakeLists.txt:72`–`82`; `interactive.h:29`–`38` includes 10 of the 11 headers — `winctrlparent.h` only via `parent.h:31`; two handlers declare payload structs locally (`help.h:32`–`40` `HelpInfo` duplicating `structures.h:427` in a different namespace, `winctrlparent.h:33`–`44` `DrawItemStruct` with no `structures.h` counterpart); the ten interactive slots have exactly two writers repo-wide, both in the disabled `retropad` example (`controller.cpp:23` → `m_focus`, `mainwindowinstancecontroller.cpp:62` → `m_dragAndDrop`), and that example is dead three times over (`CMakeLists.txt:8` `#add_subdirectory(retropad)`, includes name a `view/controllers/` tree that no longer exists — `ui/gui/view/` holds only `drawing`/`handlers`/`layout`, and two example overrides return `void` against `bool` bases). Gotchas: `wmDropFiles` double-claimed (`toplevel.cpp:98`–`103` beats `draganddrop.cpp:41`), `WinCtrlParentHandler` doubly unreachable (not a level slot; `parent.cpp:39` delegation commented), `m_clipboardOwner` is the only mismatched slot name, IME defaults re-dispatch the same id through `DefWindowProcT` (8 of 11 hooks) while still swallowing, `ime.cpp:79`–`86` is a commented-out WM_IME_CHAR DBCS decode, only the three `long long` IME hooks can report a result, the hover/leave block `0x02A0`–`0x02A3` interleaves NC and client ids, and `keyboardinput.cpp:43`/`:51` invert polarity per Win32 contract.
- Next action: walk part 50c — `gui/view/handlers/rendered/` (14 files: `cursor` `dwm` `font` `iconized` `nc` `ncrender` `rendering`), entered at `rendered.cpp:36`–`43`. That completes part 50 (74 files); index's next `summary` part is then 60 — gdiobjects.
- Documentation lives in `docs/ui/windows/` (one page per area, claims cited to file and line); `docs/ui/windows/README.md` carries the part index and the session protocol. Detailed so far: 10, 20, 30, 40, 50 (**all 74 files**), 90, 60a+60b+60c (all 32 `gui/gdiobjects/` files), 70 (all 8 `gui/painting/` files), 80 (all 12 geometry/layout/message files). Still summary: 00 only (high level by design) — the library walk is complete.
- 27 Oct 2026 — **part 60a `gui/gdiobjects/` core trio walked in full** (`gdiobject.h/.cpp` 71/61, `devicecontext.h/.cpp` 438/899, `memdevicecontext.h/.cpp` 48/47) and appended to `60-gdiobjects.md`. Confirmed: `GDIObject` is the common base and `~GDIObject()` (`gdiobject.cpp:47`–`54`, `GDI32::DeleteObject` + `Drop()`) is the **only** close step — the handle ctor `gdiobject.cpp:41`–`45` calls `m_handle.DontClose()` (`:44`) which clears `m_needsClose` (`handle.cpp:178`–`181`), so `~Handle()` (`handle.cpp:89`–`96`) never reaches `Kernel32::CloseHandle`; `GDIObjectType` (`gdiobject.h:33`–`50`) has 15 enumerators, 13 naming a pair, `OExtPen`/`OUnknown` naming none, and **no enumerator for `Cursor`/`Icon`**; `devicecontext.h` = 16 hand-mirrored Win32 structs (`:40`–`229`) + 4 `__stdcall` proc typedefs (`:231`–`235`) + class `:237`–`434` with **164 declarations ↔ 164 definitions** in the `.cpp`, 147 `GDI32::` / 10 `User32::` / 5 `WGL::` forwards, the WGL five (`:874`–`897`) being the HDC→pixel-format bridge to `WGLContext`; `MemDeviceContext` adds no member and is **never constructed repo-wide**. Gotchas: `AddDeviceContext`/`RemoveDeviceContext` have zero call sites so `m_deviceContextHandleMap` (`perthread.h:97`) is always empty and `DeviceContextFromHandle` fabricates a fresh `ref_of<DeviceContext>` per call (`perthread.cpp:173`, owner=false); `DeviceContext::FromWindow` (`:63`–`69`, `:71`–`77`) has no callers so `m_hWnd` (`devicecontext.h:433`) is never written and every `Release()` (`:84`–`87`) passes a 0 HWND; owner+`MustRelease` DCs are closed twice (derived `Release()` then base `DeleteObject`) while `CreateForDIB`/`CreateCompatible` (owner=true, mustBeReleased=false) are closed by `DeleteObject` instead of the never-called `DeleteDC` wrapper (`:395`–`398`); `takeOwnership`'s default exists only on the definition (`gdiobject.cpp:41`) so it is unreachable; `LineDDA` (`:543`–`546`) forwards no HDC and `api_layer/gdi/gdi32.h:191` has no HDC slot; `UnrealizeObject` (`:222`–`226`) sits under the author's own `//TODO: Move this to GDIObject`. Only live consumer is `mainwindow.cpp` (`:270` `CreateForDIB`, `:313` `new_ref<WGLContext>(m_dcDIB)`, `:247` `BitBlt`).
- Next action at that time: part **60b — `gui/gdiobjects/{metadc,enhmetadc,metafile,enhmetafile}`** (8 files), then 60c (the nine resource pairs, 18 files) — **both since done; part 60 complete at 32/32 and the live next action is part 70 (`gui/painting/`)**.


## Previous task
TODO 16 winqor_api_shell COMPLETED: the whole modern Shell API of shell32.dll
(60 documented `shellapi.h` entry points) is wrapped as stateless `qor::win::api::Shell` functions across
`shell.h` + seven .cpp files; build green, audit 60/60/60 with zero missing and zero non-exported names, no
SHELL32.DLL import by design (GetProcAddress binding). Open surface decision left for review: the four live-but
out-of-scope entry points `SHEnumerateUnreadMailAccountsW`, `SHGetUnreadMailCountW`, `SHSetUnreadMailCountW`,
`SHGetDriveMedia` (DECISIONS.md D12, src/platform/os/windows/api_layer/shell/purpose.md).


#15 — qor_jsondata STRUCTURAL parse-performance refactoring — **DEFERRED 22 Oct 2026 until greater resources are
available**; no structural work started, the inventory below is kept as the parked plan. #14 (parse-perf
verification FINALIZATION, round 18) is **COMPLETED** 22 Oct 2026 — reproducible record in docs/agent/verification.md
top entry, verdict NEUTRAL (all three micro-edits inside the 13.5% same-source build spread); the user decision is
recorded: **all three micro-edits are KEPT** (committed 22 Oct 2026 as `dea24af2 "JSON parser performance review
concluded"` - 24 `src/` paths +105/-79 plus the harness +233/-192, so HEAD now equals variant D); TODO 14 marked [x];
`tmp/` pruned to the round-18 evidence.
(Prior task #13 — qor_mime: MIME recognition pipeline filter (plan steps 0-17 DONE 20-22 Oct 2026: outline + pass-through +
signature window + PNG/JPEG/GIF/PDF/ZIP/XML/HTML/JSON/JavaScript/plain text detection + the application/octet-stream
fallback + recognition of a signature split across pumped chunks + recognizer reuse across `Reset()` + chaining with `qor_base64encodefilter`; **ALL 10 of the 10 target types recognised, the
recognizer total — a stream pumped in full always ends with a type and a non-empty name — and it co-operates with an encoding filter behind it**) → new framework-layer module
`src/framework/data/mime` (lib `qor_mime`, class `qor::mime::MIMERecognizerFilter :
qor::pipeline::Filter`) + test target `test/framework/data/mime` (exe `QORTestMIME`, 10 popular MIME types +
application/octet-stream fallback). Test-first plan: `memory-bank/qor_mime_plan.md`. Next action: optional plan step
17 `mime_type_name_drives_a_sink` (application-level glue — the recognised type name driving a sink). Plan step 16
`mime_recogniser_chained_with_qor_base64encodefilter` is DONE and verified test-first — `StringSource -> MIMERecognizerFilter -> Base64EncodeFilter -> StringSink`;
the encode filter is an `InlineFilter<byte>` (a Buffer pair, not an Element) inserted as the buffer shared by the elements
either side of it, the recognizer still classifies the PRE-encoded stream (`image/png`) and the sink text is exactly the
base64 of the stream; it went RED (the sink got only the first 18 of the 24 encoded characters) and was made GREEN by
separating the consumed from the produced unit count in `mime.cpp::TransferOctets`. Plan step 15
`mime_recognizer_resets_between_streams` was likewise DONE and verified — ONE recognizer serving TWO streams
(PNG of step 3, `Reset()`, PDF of step 6 through fresh elements), GREEN on its FIRST run with NO implementation change.
Steps 0-16 of the plan are done and verified (`QORTestMIME` `[TOTAL](17/17) [OK](17/17)`
exit 0; step 4 verified after the fact, its RED phase never observed; steps 14 and 15 went GREEN on their FIRST runs with NO
implementation change — the window already accumulates across pumped chunks and `Reset()` already clears the recognizer state). (TODO.md item 13.)

## Completed work
- [x] qor_jsondata parse-perf verification FINALIZED 22 Oct 2026 (TODO 14, round 18): exact harness/fixture identity
      (`test/components/formats/json/main.cpp` `[perf]` lines 284-305, fixture `test/data/data_2mb.json` 2,219,028 bytes),
      21 RelWithDebInfo runs over 7 builds in `cmakebuild_rel` + 1 Debug run, all exit 0, logs `tmp/json_final_rel_*.log`
      (raw logs deleted 22 Oct 2026 - the values below are the recorded readings);
      verdict NEUTRAL — no micro-edit earns credit (same-source build spread 13.5%); round-16 record marked unreproducible
      (its fixture `big_json.txt`, `tmp/pristine_qorjson` tree and `PARSE_TIME_MS` mode are absent).
      **COMPLETED**: user decision recorded — all three micro-edits KEPT (review grounds, no perf credit); TODO 15
      DEFERRED until greater resources are available; `tmp/` pruned 22 Oct 2026 (45 files / 300,687 B deleted,
      34 entries / 146.5 KB remain - only the round-18 evidence, see docs/agent/verification.md cleanup bullet)
- [x] Read docs/structure.md, docs/tutorial.md
- [x] Read root + sub CMakeLists.txt (4 levels)
- [x] Full scan: all set(target qor_*) + target_link_libraries across src
"- [x] Wrote docs/agent/repo_map.md (174 lines; 60 QOR libs + deps + layering)
"- [x] Scanned src/qor dirs; resolved D6 (header-only rule: no CMakeLists.txt = header-only)
- [x] Resolved D5 (user: protocols/http_old NOT built, ignore)
- [x] Generated docs/agent/ctags/qor_module.tags "(hand-emitted ctags format; 26 tag lines)
- [x] Re-checked ctags: Universal Ctags 6.1.0 IS available (ctags.exe via WinGet) -> D7 updated; will ctags every lib
"- [x] Generated docs/agent/ctags/qor_host.tags (Universal Ctags 6.1.0; framework/app/host, 11 symbols)
- [x] Verified build: cmake configure (MSVC 19.50, VS2026, Windows) + cmake --build --target qor_module -> PASS (qor_module.lib produced, no errors/warnings)"
- [x] Wrote docs/agent/purposes.md — one-line purpose summary per QOR lib, all ~60 libs, derived from consolidated src/**/purpose.md (docs/agent/_tmp_purpose_all_out.md) + repo_map.md; header-only libs noted (D6)
"- [x] Wrote docs/agent/deps.dot — DOT/digraph of every QOR lib dependency (edge A->B = A target_links_libraries against B); clustered by subsystem; http_old excluded per D5; non-QOR platform adapters noted as leaves
- [x] Wrote docs/agent/parsers.md — skill file for creating a QOR parser module, learned by analysis of qor_jsondata (src/components/data/formats/json): two-layer design (general qor::data::parser framework + per-format qor_Xdata lib), ABNF->combinator table (Sequence/Optional/ZeroOrMore/OneOrMore/Specific), ParserState Prepare/Emit/Fail + node stack, tokens.h enum + nodes NodeAdapter + model/ types, BufferContext input feeding, build/test checklist"

## Open questions
- platform adapter libs referenced: winqor_*, linqor_* -- are these built in this tree or external?
- src/qor has no "top-level" dir; all libs live under essentials/flyers/function/memory/module/property.

## Decisions
- (D1) Library unit of work = one set(target qor_X) CMakeLists.txt.
- (D2) Symbol index granularity = per-QOR-lib ctags (per repo map 60 libs + adapters).
- (D3) Verification = compiler-as-judge; incremental rebuild per lib; record in verification.md.

## Task state
- qor_jsondata parse-perf verification (TODO 14): **COMPLETED** 22 Oct 2026 (round 18) — reproducible record + NEUTRAL
  verdict; logs tmp/json_final_rel_*.log (21 runs) and tmp/json_perf_debug_now.log (Debug) - raw logs deleted 22 Oct 2026;
  D9 rewritten; TODO 14 marked [x]; all three micro-edits KEPT by user decision; tmp/ emptied of JSON-perf logs (3 dirs left)
- qor_jsondata structural parse-perf refactoring (TODO 15): **DEFERRED** 22 Oct 2026 until greater resources are available
  — inventory parked, nothing started
- repo_map.md: DONE
- ctags/qor_module.tags: DONE (hand-emitted)
"- ctags/qor_host.tags: DONE (ctags 6.1.0)
"- ctags/qor_{current,injection,sync,thread}.tags: DONE (ctags 6.1.0)
- ctags/qor_*.tags for all 45 leaf libs (src/components): DONE (ctags 6.1.0; http_old skipped per D5)
"- verification.md: DONE (template written; enhanced existing file with D3/D5/D6 notes + per-lib entry template)
"- build verify qor_module: DONE (PASS — MSVC 19.50.35721.0, VS2026, Windows; qor_module.lib produced; logged in verification.md)
- add_module skill file docs/agent/add_module.md: DONE (qor_yaml experiment; facts to add future modules)
- qor_yaml experiment: DONE (src/components/protocols/yaml CMakeLists.txt + qor_yaml.cpp + yaml.cpp -> qor_yaml.dll; registered add_subdirectory(yaml) in src/components/protocols/CMakeLists.txt)
- QORTestYAML test: DONE (test/components/protocols/yaml CMakeLists.txt + test_main.cpp + test_yaml.cpp; added add_subdirectory(components/protocols/yaml) in test/CMakeLists.txt; QORTestYAML.exe builds, links qor_yaml.lib, test yaml_module_loads OK(1/1) exit=0)""
- purposes.md: DONE (one-line purpose summary per QOR lib, ~60 libs + subsystem context)
"- deps.dot: DONE (DOT/digraph; edges A->B = A target_links_libraries against B; clustered by subsystem; http_old excluded per D5)
- parsers.md skill file: DONE (docs/agent/parsers.md; learned from qor_jsondata analysis; ABNF->combinator mapping, module skeleton, production-class patterns, build/test checklist)"
- JSON generator increment (docs/archive/jsonGeneratorPlan.md — retired 20 Oct 2026, task COMPLETE): DONE — states OK(36/36) in QORTestJSONGenerator, roundtrip green in QORTestJSON (exit 0), and full QORTest regression run green at OK(678/678) (process exit 0)
- task 12 YAML parser: IN PROGRESS — resume point in 'Task 12 (YAML parser) — detailed progress' below (string.h/.cpp + CMakeLists add_library landed this session; remaining productions + link deps + CharacterNode + tests still pending)


## Task 12 (YAML parser) — detailed progress

### Done
- Model headers: model/valuemodel.h, model/value.h, model/string.h, model/sequence.h, model/pair.h, model/mapping.h, model/character.h, model/_true.h, model/_false.h, model/_null.h
- Parser headers: parser/tokens.h (yamlToken enum starting at eToken::Max+1), parser/helpers.h (GetTokenName, StandardPrepare, StandardFail, TrivialEmit, SimpleEmit — mirrors json/helpers.h)
- Node adapter headers: parser/nodes/value.h, string.h, sequence.h, mapping.h, pair.h, _true.h, _false.h, _null.h (each NodeAdapter<model::yaml::X> tagged with yamlToken::...)
- State headers (flow subset):
  - _4/: ws.h, unescaped.h, quotation_mark.h, value_separator.h, character.h (AnyOneOf_t<unescaped,unescaped> — escapes deferred per D8)
  - _1/: begin_sequence.h, begin_mapping.h, _true.h/_true.cpp, _false.h/_false.cpp, _null.h/_null.cpp
  - _2/: end_sequence.h, end_mapping.h
  - _3/: name_separator.h
- Bug fix: parser/nodes/_true.h was mangled with literal \n escapes; corrected via single_find_and_replace.

### Read for reference
- Framework: parser.h/.cpp, state.h, node.h, result.h, context.h, tokens.h, sequence.h, optional.h, zeroormore.h, specific.h, anyoneof.h, anyoneofset.h/.cpp, oneormore.h, oneofarange.h, deferred.h, rfc5234.h, podbuffer.h, buffercontext.h, parsersink.h, fastflow.h
- JSON model/states: value.h, valuemodel.h, array.h, object.h, member.h, string.h, _true.h, _false.h, _null.h; array.cpp, string.cpp/.h, ws.h, value.cpp/.h, member.cpp, _false.cpp, _true.h, begin_array.h, end_array.h, value_separator.h, name_separator.h, quotation_mark.h, unescaped.h, character.h/.cpp
- Grep confirmed UTF8CodePage lives at src/qor/essentials/text/codepage/codepages/utf8.h (module QOR_TEXT; needs qor_text link dep).
- AnyOneOf_t requires TWO alternatives; no single-arg form exists (character.h uses unescaped twice as stub).

### Still to do
1. Create state productions (headers + .cpp under src/components/protocols/yaml/parser/states/):
   - _4/string.h + .cpp (Sequence_t<quotation_mark, Sequence_t<ZeroOrMore_t<character>, quotation_mark>>; Prepare pushes StringNode; Emit collects chars via UTF8CodePage.Encode → SetValue; Fail pops)
   - _4/value.h + .cpp (AnyOneOf nested chain over _false/_null/_true/number/sequence/string + deferred<mapping>/deferred<sequence>; Prepare pushes ValueNode; Emit copies child ValueModel into Value; Fail pops)
   - _1/sequence.h + .cpp (Sequence(begin_sequence, Sequence(Optional(Sequence(value, ZeroOrMore(Sequence(value_separator,value)))), end_sequence)); Prepare pushes SequenceNode; Emit collects value nodes into m_values)
   - _3/mapping.h + .cpp (analogous with pair; Prepare pushes MappingNode; Emit collects pair nodes into m_pairs)
   - _3/pair.h + .cpp (Sequence_t<string, Sequence_t<name_separator, value>>; Prepare pushes PairNode; Emit sets key from string + value)
   - _1/document.h + .cpp (root = value)
   - parser/yamlparser.h umbrella include
   - _3/number.h + .cpp if scalars include numeric YAML (or defer per D8)
2. Update src/components/protocols/yaml/CMakeLists.txt: add all new .cpp sources to add_library; add link deps qor_pipeline qor_workflow qor_issue qor_error qor_log qor_text to target_link_libraries (current: qor_module qor_host qor_current qor_sync qor_injection qor_parser).
3. Build: cmake --build build --target qor_yaml; record PASS in docs/agent/verification.md (per D3).
4. Tests: replace placeholder test_yaml.cpp with real cases feeding YAML strings ([1, 2, 3], {a: 1, b: 2}, "hello", true, null) via PODBuffer<uint32_t>+CodePointContext+Parser::SetContext/SetInitialStep/Parse()/FinalParse()/PopNode, asserting model types/values; rebuild QORTestYAML.exe and run.
5. Update docs/agent/STATE.MD and docs/agent/TODO.md (mark item 12 [x] when verified).

### Reference skill
docs/agent/parsers.md (ABNF→combinator table, module skeleton, production-class patterns, build/test checklist).

## Task state — #13 qor_mime (20 Oct 2026)
- Step R research (how QOR pipeline filters are created + tested): DONE — findings in
  `memory-bank/qor_mime_plan.md` (filter.h/filter.cpp/base64encodefilter/linereader/QORTestYAML patterns).
- Module scaffold `src/framework/data/mime`: DONE — `CMakeLists.txt` (`qor_mime` SHARED, `QOR_MIME`),
  `qor_mime.cpp` (singleton), `mime.h` (`qor::mime::MIMERecognizerFilter` outline API + `MIMEType` enum:
  10 targets + OctetStream fallback + Unresolved), `mime.cpp` (`init_mime` export + placeholder `Inspect()`),
  `purpose.md`, `status.md`. Registered `add_subdirectory(mime)` in `src/framework/data/CMakeLists.txt`.
- Test target `test/framework/data/mime`: DONE — `QORTestMIME` exe (`test_main.cpp` + placeholder `test_mime.cpp`
  case `mime_module_loads`), registered `add_subdirectory(framework/data/mime)` in `test/CMakeLists.txt`.
- Verify #13 outline: DONE — `qor_mime.dll` + import lib built clean, `QORTestMIME.exe` `[OK](1/1)` exit 0
  (logged `docs/agent/verification.md`).
- Plan step 1 (`mime_filter_passes_octets_through`): DONE 20 Oct 2026 — test added to `test_mime.cpp` FIRST
  and seen RED (`[FAIL] #1`, exit 1, "value was : 0" — base `Filter` does not move octets by itself), then
  `mime.h`/`mime.cpp` gained `TransferOctets()` (buffer→buffer move like `CopyFilter::Copy`, called from
  `ReadFilter`/`WriteFilter`); rebuilt qor_mime + QORTestMIME → `[TOTAL](2/2) [OK](2/2)` exit 0
  (logged `docs/agent/verification.md`, run from `build\test\framework\data\mime\Debug`).
  Key plumbing finding: a filter pumps in Push mode under a `Pipeline` parent (Element::SetFlowMode only
  forwards to a parent); Pull mode hits `Filter::Read` → `ReadFilter(unitsRead, unitsRead)` (0 units at entry).
- Plan step 2 (`mime_captures_leading_octets`): DONE 20 Oct 2026 — test written FIRST and seen RED
  (`[FAIL] #2`, `[OK](2/3)`, exit 1), then `Inspect()` in `mime.cpp` captures the leading octets via the
  buffer three-step read protocol — `ReadRequest` → `memcpy` into `m_signature` → **`ReadReject`** (gives
  the reservation back, source NOT consumed; `buffer.cpp` `ReadReject` restores `m_readEnd`) — capped by
  the 16-byte `SIGNATURE_WINDOW_BYTES` and `ReadCapacity`; `ReadFilter`/`WriteFilter` call `Inspect()`
  BEFORE `TransferOctets()`; `mime.h` needed no change. Rebuilt → `QORTestMIME` `[TOTAL](3/3) [OK](3/3)`
  exit 0 (logged `docs/agent/verification.md`). NOTE: a run issued in the same command batch as the build
  used the stale previous exe — build and run must be separate sequential commands; and `log::debug`
  output is not visible in `QORTestMIME` (log flyer not wired there), so it is not a usable probe.
- Plan step 3 (`mime_recognises_png_signature`): DONE 20 Oct 2026 — test written FIRST and seen RED
  (`[FAIL] #3`, `[OK](3/4)`, exit 1), then a fixed-order guard in `Inspect()` (`m_detectedType ==
  Unresolved && m_signatureCount >= 8`, `memcmp` the window vs `89 50 4E 47 0D 0A 1A 0A` → `PNG`)
  plus the complete registered-name switch in `GetTypeName()`; `mime.h` unchanged →
  `QORTestMIME` `[TOTAL](4/4) [OK](4/4)` exit 0.
- Plan step 4 (`mime_recognises_jpeg_signature`): **VERIFIED DONE 20 Oct 2026, NOT test-driven** — the
  test and the jpeg guard (`m_signatureCount >= 3`, `memcmp` vs `FF D8 FF`) were already in the working
  tree as uncommitted edits to `mime.cpp`/`test_mime.cpp` with no build, no run and no record (no
  step-4 commit in `git log`). Verified after the fact: built `qor_mime` + `QORTestMIME`, ran the exe
  from `build\test\framework\data\mime\Debug` → `[TOTAL](5/5) [OK](5/5)` exit 0. The RED phase was
  never observed — recorded honestly, not assumed. LESSON: unverified working-tree code is not done.
- Plan step 5 (`mime_recognises_gif_signature`): DONE 20 Oct 2026 test-first — test written FIRST
  (two streams in the case, fresh elements for the second, `GIF89a` and `GIF87a` six-byte headers,
  pass-through + `== GIF` + `"image/gif"` per stream) and seen RED (`[FAIL] #5`, `[TOTAL](6/6)
  [OK](5/6) [FAIL](1/6)`, exit 1), then a third `Inspect()` guard (`m_signatureCount >= 6`,
  `memcmp` vs GIF89a OR GIF87a headers) → `[TOTAL](6/6) [OK](6/6)` exit 0; `mime.h` unchanged.
- Plan step 6 (`mime_recognises_pdf_signature`): DONE 20 Oct 2026 test-first — test written FIRST
  (one stream, `%PDF-1.4\n` four-byte magic + version marker + short body; asserts pass-through,
  `== PDF`, `"application/pdf"`) and seen RED (`[FAIL] #6`, `[TOTAL](7/7) [OK](6/7) [FAIL](1/7)`,
  exit 1), then a fourth `Inspect()` guard (`m_signatureCount >= 4`, `memcmp` vs `{0x25,0x50,0x44,0x46}`
  = `%PDF` → `MIMEType::PDF`) → `[TOTAL](7/7) [OK](7/7)` exit 0; `mime.h` unchanged. No new gotchas.
- NEW build gotcha: two `cmake --build` invocations issued in the same command batch run concurrently
  over one build tree and race on the shared `.pdb` — `error C1090: PDB API call failed` (unrelated
  dependency) and `error C1041: cannot open program database '…vc145.pdb'; if multiple CL.EXE write to
  the same .PDB file, please use /FS`. Transient but real; exactly one cmake build per command.
- Plan step 7 (`mime_recognises_zip_signature`): DONE 20 Oct 2026 test-first — test written FIRST
  (one stream, raw `PK 03 04` magic `"PK\x03\x04"` + body) and seen RED (`[FAIL] #7`, `[TOTAL](8/8)
  [OK](7/8) [FAIL](1/8)`, exit 1), then a fifth `Inspect()` guard (`m_detectedType == Unresolved &&
  m_signatureCount >= 4`, `memcmp` vs `{0x50,0x4B,0x03,0x04}` = `PK 03 04` → `MIMEType::ZIP`) →
  `[TOTAL](8/8) [OK](8/8)` exit 0; `mime.h` unchanged. No new gotchas.
- Plan steps 8-14: DONE 20-22 Oct 2026 test-first (XML, HTML, JSON, JavaScript, plain text, the octet-stream
  fallback, the signature split across pumped chunks) — details in `src/framework/data/mime/status.md` and
  `docs/agent/verification.md`; all 10 target types recognised and the recognizer made total.
- Plan step 15 (`mime_recognizer_resets_between_streams`): DONE 22 Oct 2026 test-first — ONE `MIMERecognizerFilter`
  instance serving TWO streams (the PNG stream of step 3, then `Reset()`, then the PDF stream of step 6 through
  FRESH elements re-linked with `Filter::SetSource`/`Filter::SetSink`); asserts the cleared state after `Reset()`
  (`GetDetectedType() == MIMEType::Unresolved`, `GetTypeName() == ""`, `GetSignatureByteCount() == 0`) and that
  stream 2 recognises `== MIMEType::PDF` / `"application/pdf"` with pass-through and a 16-octet window. GREEN on
  the FIRST run `[TOTAL](16/16) [OK](16/16)` exit 0 — NO implementation change needed (`Reset()` already zeroes
  `m_signatureCount` and restores `m_detectedType`); `mime.h`/`mime.cpp` untouched, only `test_mime.cpp` recompiled
  (logs `tmp/qortestmime_step15_build_exe.log`, `tmp/qortestmime_step15_green.log`). LESSON: a filter is reused by
  re-wiring the FILTER — `Element::SetSink`/`SetSource` return false on an element that already has a connection.
- Plan step 16 (`mime_recogniser_chained_with_qor_base64encodefilter`): DONE 22 Oct 2026 test-first — TWO pipeline
  filters in series `StringSource -> MIMERecognizerFilter -> Base64EncodeFilter -> StringSink`. Anatomy FIRST: the
  encode filter is an `InlineFilter<byte>` — a Buffer pair, not an Element — so the framework inserts it as the buffer
  shared by the elements either side of it (`Pipeline::InsertInlineFilter`, BeforeSink position); here it is the SINK's
  buffer, built with an explicit capacity (`Base64EncodeFilter encodeFilter(128)`) because its default ctor holds zero
  items. Stream: the 8-byte PNG magic as explicit octets + a 10-byte payload = 18 octets (a multiple of 3, so no base64
  padding), pumped for the 24 ENCODED units; asserts the sink text is exactly `"iVBORw0KGgpJSERSIHFvcjAx"` and that the
  recognizer still reports `MIMEType::PNG` / `"image/png"` with a 16-octet window. Seen RED FIRST (`[FAIL] #16`,
  `[OK](16/17)`, exit 1 — the sink got only the first 18 of the 24 encoded characters), then fixed in
  `mime.cpp::TransferOctets`: the consumed count (`unitsConsumed`, drives the memcpy and the source `ReadAcknowledge`)
  must be kept apart from the produced count (`unitsToTransfer`, rewritten by the sink's `InlineFilter::WriteAcknowledge`
  and returned in `unitsTransferred`) — sharing one variable let `ReadAcknowledge` clamp the produced count back to the
  raw one. `mime.h` contract note extended; `test/framework/data/mime/CMakeLists.txt` gained `qor_base64encodefilter`.
  Rebuilt `qor_mime` then `QORTestMIME` (separate commands) → `[TOTAL](17/17) [OK](17/17)` exit 0 (logs
  `tmp/qor_mime_step16_dll.log`, `tmp/qortestmime_step16_exe.log`, `tmp/qortestmime_step16_green_run.log`). LESSON: a
  recognising filter inspects the stream BEFORE any downstream transform, so it classifies the PRE-encoded octets and is
  valid in front of an encoding filter; a pass-through filter must propagate the count its sink buffer PRODUCED, not the
  count it CONSUMED.
- NOT done: the optional `mime_type_name_drives_a_sink` scope, the full `QORTest` regression run over the changed
  `mime.cpp`, and consumers (HTTP Content-Type TODO, UI renderer loader, `Resource::Type()`).
- Note: TODO.md/STATE.MD on disk do not contain the earlier `qor_workflow` pipeline-integration item that
  STATE.MD's "Current task" previously referenced; the queue files appear trimmed. Item numbers on disk now
  run 1..13 with 13 = this task (item 14 = this JSON parse-performance task, added 22 Oct 2026). Not invented — just recorded so the numbering is not read as a gap.

## Task state — qor_jsondata parse-performance verification (FINAL round 18, 22 Oct 2026 — COMPLETED, TODO 14 [x])
- Harness identity verified on disk: fixture `test/data/data_2mb.json` = **2,219,028 bytes**; the timer is the
  `[perf] 2MB JSON parse (cold|warm): N ms` pair printed by `test/components/formats/json/main.cpp` lines 284-305.
  There is NO `PARSE_TIME_MS` env mode and no `[JSON parse time]` line in that file (grep: no match), and the
  handler is `log::Level::Informative` (main.cpp line 82), so run logs are 7 non-blank lines / 1332-1336 bytes in Debug and
  RelWithDebInfo — no log flood. This exe prints no `[OK](n/n)` summary; judge it on exit `0` + no
  `Test failed`/`Failure:` text.
- FINAL measurement (reproducible, 21 runs over 7 builds in `cmakebuild_rel`, logs
  `tmp/json_final_rel_{A,A2,E,F,D,D2,E2}_run{1,2,3}.log` (raw logs deleted 22 Oct 2026), all exit `0`): git-HEAD sources **2110 ms mean**
  (per-build means 1959.6/2182.5/2188.6 -> **11.8% spread for identical sources**), current tree (all three edits)
  **1879 ms mean** (builds 1875.4/1881.9; a third build of the same sources, `json_perf_rel_now.log` since deleted,
  measured 2128.5 ms mean -> **13.5% spread**), mask-only 2259 ms, mask-less 2255 ms; Debug current tree 54.6-54.8 s
  (`json_perf_debug_now.log`, since deleted). Verdict: **performance-neutral within the noise floor** — the -11.0% A->D delta
  is smaller than the same-source build spread, so no micro-edit earns credit.
- The round-16 record (2,102,067-byte `big_json.txt`, `tmp/pristine_qorjson` tree, `PARSE_TIME_MS` mode,
  1.93 s / 2.14 s / 53.2 s / 55.1 s, ~6.7M `log::debug` lines, `[OK](42/42)`) is UNREPRODUCIBLE — those artifacts
  are absent from this tree; it is kept in `docs/agent/verification.md` only as marked history. The "2MB ~56 s ->
  ~18 s" claim does not exist in either config.
- Working-tree state: all three micro-edits remain applied (variant D) and are now **committed at HEAD** (`dea24af2`)
  — `helpers.h`/`state.cpp` `#ifndef NDEBUG`
  guards, `ParserState::Workflow()`/`Specific::Enter` RTTI removal, `src/framework/data/pipeline/podbuffer.h`
  power-of-2 masking. RESOLVED: keep the mask (bounds every index read, `assert` removal stays) — kept for review,
  not for performance. **User decision 22 Oct 2026: all three micro-edits are KEPT** — the task is closed with the
  edits in place and zero performance credit claimed.
- Cleanup 22 Oct 2026 (two passes): pass 1 deleted 45 files / 300,687 B of superseded round-16/17 artifacts and scratch
  scripts (`tmp/dbg_*`, `tmp/qortestjson_*`, `tmp/rel_{B,C,pre,post,pre2,config,fixed,instr,json,pristine,full}_*`,
  `tmp/rel_D_run_{1,2,3}.txt`, `tmp/json_perf.patch`, `tmp/_*.js` / `tmp/_*.txt`); pass 2 deleted the 31 round-18 logs
  (146.5 KB — the 28 `tmp/json_final_rel_*_{build,run1,run2,run3}.log`, `tmp/json_perf_rel_now.log`,
  `tmp/json_perf_debug_now.log`, `tmp/rel_D_build.log`). **`tmp/` now holds only the 3 dirs `tmp/dcast`,
  `tmp/edited`, `tmp/jsonread`** — no JSON-perf log survives anywhere, so the `docs/agent/verification.md` FINAL entry's
  table is the sole durable record and every `tmp/...` name in it (and in the SUPERSEDED entries) is provenance only.
- DEFERRED (TODO 15, structural refactoring) 22 Oct 2026 — parked until greater resources are available, nothing
  started; when resumed, measure each with the same `[perf]` timer and believe only deltas over ~14%: (1) bulk scanning
  of string/number runs in `parser/states/_2/character.cpp` (+`_4/string.cpp`,
  `_3/integer.cpp`, `_3/fraction.cpp`); (2) devirtualized/table-driven machine for the `ParserState` hierarchy
  (`src/components/data/parser/{state.cpp,states/*}`, 48 JSON state files / 17 .cpp under
  `src/components/data/formats/json/parser/states/_1.._4`); (3) model-node pooling after removing the
  `Parser::FinalParse()` -> `AnyObject::Cast<Fastflow>` hop (`typedany.h:158` `CastToBase_` live `dynamic_cast`).

