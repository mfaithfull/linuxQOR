# winqor_api_user - per-TU session narratives (archived verbatim from status.md on 31 Oct 2026)

Source: status.md lines 1063-2151 as of the session-15 commit (`9ecbf581`). These are the full session write-ups; the findings, decisions and registers remain in `status.md`. Session 16 (`help.cpp`) has no narrative here - its record is the session-16 row in the status.md session-log table, the U26 entry and the T4 task.

## accessibility.cpp + timer.cpp - session 2 (29 Oct 2026)

Reviewed together because the two files were transposed - `accessibility.cpp` held the Timer wrappers and
`timer.cpp` held the accessibility wrapper. The bodies were moved so each file matches the `user32.h` section it
is named after (U12, now fixed). Three wrappers, three bindings; each pairs exactly with the SDK prototype and
with a real export, and all three are live.

| Wrapper | Now defined in | SDK prototype (`winuser.h` 10.0.26100) | Resolved binding (`cl /EP`) | Export |
|---|---|---|---|---|
| `NotifyWinEvent(DWORD, HWND, LONG, LONG) -> void` | `accessibility.cpp` | `:13728 VOID WINAPI NotifyWinEvent(DWORD, HWND, LONG, LONG)` | `"NotifyWinEvent"` | yes (ordinal 295) |
| `KillTimer(HWND, UINT_PTR) -> BOOL` | `timer.cpp` | `:7124 BOOL WINAPI KillTimer(HWND, UINT_PTR)` | `"KillTimer"` | yes (ordinal 251) |
| `SetTimer(HWND, UINT_PTR, UINT, TIMERPROC) -> UINT_PTR` | `timer.cpp` | `:7079 UINT_PTR WINAPI SetTimer(HWND, UINT_PTR, UINT, TIMERPROC)` | `"SetTimer"` | yes (ordinal 372) |

- **All three correct as written - no P1/P2 conversion needed.** `winuser.h` declares each entry point directly;
  there is no A/W family and no `stem -> stemW` macro for any of them, and `user32.dll` exports only the bare
  stems (the export table has `KillTimer`/`SetTimer`/`NotifyWinEvent` with no `*A`/`*W` - the only other
  `*Timer` names are `SetCoalescableTimer` and `RIMOnTimerNotification`). Under D13 the bare
  `qor_pp_useswinapi` is therefore the right spelling, and the `/EP` expansion confirms the literal name reaches
  `GetProcAddress`. Same case as session 1's `DestroyAcceleratorTable`; source comments now record it.
- No `T` suffix on any of the three wrappers - correct, since none has an A/W family.
- Signature match is exact against the SDK for declaration and definition alike (`user32.h:665` `NotifyWinEvent`,
  `:668-669` the Timer pair - all three `static`); `Library::Call`/`voidCall` template arguments match the
  parameter types one-for-one, so the `reinterpret_cast` in the call helper is at least type-correct (the
  calling-convention caveat is U6).
- Live callers: `ui/gui/controllers/windowcontroller.cpp:800` (`NotifyWinEvent`), `:805` (`KillTimer`), `:810`
  (`SetTimer`). The caller's `reinterpret_cast<TIMERPROC>(timerFunc)` is sound on x64 -
  `ui/../common/structures.h:415` defines `TimerProc` as `void (stdcall*)(void*, unsigned int, unsigned long
  long, unsigned long)`, same convention and same argument widths as `TIMERPROC` (`winuser.h:109`).
- Fixed (style only): `( pFunc … )` -> `(pFunc …)` at both deviating call sites. The folder norm is now 415 of
  416 sites; the single remaining outlier is `resource.cpp:27`, for that TU's session.
- `return Library::voidCall<…>(…);` inside a `void` wrapper is legal C++ (returning a void expression) and kept
  as found. Note the sibling inconsistency for whoever settles U1/U2: `api_layer/comctl32` uses the same
  `return voidCall` form, every `api_layer/kernel` TU calls `voidCall` as a bare statement.
- Not touched (already logged): no `Win32ErrorHandler _;` (U1), no `CheckReturn<BOOL, BoolCheck>` on `KillTimer`
  or `CheckNonZero<UINT_PTR>` on `SetTimer` (U2 - both signal failure as plain 0, so a failure today is silent);
  `NotifyWinEvent` returns `void`, so a swallowed SEH fault (U7) is completely unobservable; binding cached for
  process lifetime (U5).
- Coverage note: the "//Active Accessibility functions" section (`user32.h:661-665`) has four members and one is
  implemented (`NotifyWinEvent`); `SetWinEventHook`/`UnhookWinEvent`/`IsWinEventHookInstalled` are still
  declared-and-undefined (U4) and are the obvious content for this TU if that surface is ever wanted.
- Verified: `/EP` after the move shows `accessibility.cpp` -> `"NotifyWinEvent"`, `timer.cpp` -> `"KillTimer"`
  and `"SetTimer"`; both TUs compile standalone at `/W4` exit 0; the whole library compiles 38/38 at `/W4`,
  exit 0, no diagnostics.

## accelerator.cpp - the 29 Oct 2026 finding in full

Declarations `user32.h:419-424`, definitions `accelerator.cpp:23-58`: five wrappers, five bindings, each
pairing exactly with the SDK prototype and with a real export.

| Wrapper | SDK entry | Resolved binding (after fix) | Export |
|---|---|---|---|
| `CopyAcceleratorTableT(HACCEL, LPACCEL, int) -> int` | `CopyAcceleratorTableW` | `CopyAcceleratorTableW` | yes (hint 58) |
| `CreateAcceleratorTableT(LPACCEL, int) -> HACCEL` | `CreateAcceleratorTableW` | `CreateAcceleratorTableW` | yes (5E) |
| `DestroyAcceleratorTable(HACCEL) -> BOOL` | `DestroyAcceleratorTable` | `DestroyAcceleratorTable` | yes (AD) - single export, no A/W |
| `LoadAcceleratorsT(HINSTANCE, LPCTSTR) -> HACCEL` | `LoadAcceleratorsW` | `LoadAcceleratorsW` | yes (253) |
| `TranslateAcceleratorT(HWND, HACCEL, LPMSG) -> int` | `TranslateAcceleratorW` | `TranslateAcceleratorW` | yes (3B8) |

- Fixed (fragility, zero behaviour change): `CreateAcceleratorTable`, `LoadAccelerators` and
  `TranslateAccelerator` were bound with the bare stem, which only works because of G1. A `#undef` of any of
  those stems - the header already carries nine of them - would turn the wrapper into a guaranteed run-time
  throw. They now use `qor_pp_useswinapiAW`, which spells in the source the name `GetProcAddress` is given.
  Verified by re-preprocessing the edited TU (the five names above) and by a `cl /W4` compile at exit 0.
- Live callers exist: `ui/gui/view/handlers/acceleratortable.cpp:57` (`CreateAcceleratorTableT`), `:63`
  (`LoadAcceleratorsT`), `:83` (`TranslateAcceleratorT`) - not dead wrappers.
- Not fixed, deferred to U1/U2: no `Win32ErrorHandler _;`, and no `CheckReturn` on the `BOOL` result of
  `DestroyAcceleratorTable` or on the two `HACCEL` results (`CheckNonZeroHandle<HACCEL>` would be the
  `shell`-policy spelling). `CopyAcceleratorTableT`'s `int` is a count and would stay raw.
- Already consistent: license header, include order (`configuration.h` first, Windows headers last),
  `qor_pp_fcontext` in all five, 4-space indent matching the folder, `}}}//qor::win::api` namespace comment,
  and a `T` suffix exactly where an A/W family exists and absent where it does not.



## brush.cpp - session 3 (29 Oct 2026)

The smallest TU in the folder: one wrapper, one binding, and the whole "Brush functions" section
(`user32.h:654-655`). Nothing in it was wrong.

| Wrapper | SDK entry (`winuser.h` 10.0.26100) | Resolved binding (`cl /EP`) | Export |
|---|---|---|---|
| `GetSysColorBrush(int nIndex) -> HBRUSH` | `:9694-9698` `WINUSERAPI HBRUSH WINAPI GetSysColorBrush(_In_ int nIndex)`, guarded `WINVER >= 0x0400` | `"GetSysColorBrush"` | yes (index 1958, ordinal 1C6) |

- **Correct as written - no P1/P2 conversion needed.** `winuser.h` mentions the stem exactly once (the
  declaration), i.e. there is no `GetSysColorBrush -> GetSysColorBrushW` macro, and `user32.dll` exports only the
  bare stem - the export table contains no `*A`/`*W` form of it. Under D13 the bare `qor_pp_useswinapi` is
  therefore the right spelling, and `/EP` proves the literal name reaches `GetProcAddress` (the same
  no-A/W-family case as session 1's `DestroyAcceleratorTable` and session 2's `KillTimer`/`SetTimer`/
  `NotifyWinEvent`). No `T` suffix on the wrapper - correct. The stem is not among the nine `#undef`s at
  `user32.h:9-17`, so G2 does not bite here.
- Declaration/definition/SDK pairing exact: `static HBRUSH GetSysColorBrush(int nIndex)` at `user32.h:655`,
  `HBRUSH User32::GetSysColorBrush(int nIndex)` at `brush.cpp:22`, parameter names match the SDK (`nIndex`).
  The member is `static` (unlike the 97 U4 members) and is qualified correctly (unlike U11).
- **Fixed (convention):** `Library::Call< HBRUSH >(pFunc, nIndex)` passed only the return type, letting
  `MethodArgs...` deduce. Now `Library::Call<HBRUSH, int>(pFunc, nIndex)`, which states the SDK signature and
  matches the dominant idiom (382 of 428 sites in this folder, 344 of 387 in `shell`+`kernel`). Type-correctness
  was already fine - deduction yields the wrapper's own `int` parameter, which is the SDK type - so this is a
  clarity/convention fix with zero behaviour change. It also tightened the inner spacing on that one line; see
  U14(a) for why the rest of the folder's 178 spaced sites were deliberately left alone.
- **Fixed (documentation):** the TU had no comment. It now records (i) why the bare stem is the correct binding
  for this entry point, and (ii) the ownership contract the wrapper hands to its caller - `GetSysColorBrush`
  returns a **system-cached brush owned by USER32**, which must never be `DeleteObject`'d, so returning the raw
  `HBRUSH` (rather than a checked/owned handle) is deliberate. That contract is load-bearing for the one live
  caller, `ui/gui/gdiobjects/brush.cpp:111-116` (`SysBrush::SysBrush`), which does `m_handle = User32::
  GetSysColorBrush(nIndex); m_handle.DontClose(); m_owner = false;` - the `DontClose()` is exactly what the API
  requires. Cross-reference, not re-verified here: `docs/agent/STATE.MD` part-60c walk reports that the raw-handle
  assignment to `m_handle` re-arms `m_needsClose` (`handle.cpp:84`) so the conversion temporary's `~Handle` can run
  `Close()` -> `Kernel32::CloseHandle` on a GDI system-brush handle. If that is right it is a `ui/gui` defect at
  the call site, not in this wrapper, and it belongs to the `winqor_gui` documentation track.
- Section coverage is **complete**: `user32.dll` exports exactly one `*Brush*` entry point (measured against
  `tmp/_tmp_user32_exports.txt`), so there is no un-wrapped brush surface in this library; the other brush calls
  (`CreateBrushIndirect`, `GetBrushOrgEx`, ...) are `gdi32.dll` and live in `api_layer/gdi`.
- Not touched, and now settled policy rather than an open question (see "Convention decision"): no
  `Win32ErrorHandler _;` (U1); no return check although `GetSysColorBrush`
  returns NULL on failure, so a failure today is a silent NULL handle (U2 - the `shell` policy spelling would be
  `CheckNonZeroHandle<HBRUSH>`; G6 means adopting it would change behaviour). Recorded as this TU's instance count:
  1 call site, 0 handlers, 0 return checks. Also left alone as logged: the binding is cached in a function-local
  `static const` for process lifetime (U5); `Call`'s `fPtr` is default `__cdecl` against a `WINAPI` export, so
  this call site is one more instance of the x86 UB in U6; a swallowed SEH fault returns a default `HBRUSH` (U7);
  `returncheck.h` is included and unused here, as in nearly every TU.
- Verified: standalone `/W4` compile of the edited TU exit 0; `/EP` after the edit still resolves
  `Kernel32::GetModuleHandleW(L"user32")` + `"GetSysColorBrush"` and now shows `Library::Call<HBRUSH, int>`;
  whole library 38/38 at `/W4`, exit 0, no diagnostics.


## caret.cpp - session 4 (29 Oct 2026)

The "//Caret functions" section (`user32.h:459-467`): eight wrappers, eight bindings. The TU matches its name and
its header section exactly - the eight caret names are defined in no other file in the folder, so there is no
U12-style transposition here - and it is the first TU reviewed with **nothing wrong in it**.

| Wrapper | SDK prototype (`winuser.h` 10.0.26100) | Resolved binding (`cl /EP`) | Export |
|---|---|---|---|
| `CreateCaret(HWND, HBITMAP, int, int) -> BOOL` | `:9456-9463` `BOOL WINAPI CreateCaret(_In_ HWND, _In_opt_ HBITMAP, _In_ int, _In_ int)` | `"CreateCaret"` | yes (ordinal 5F) |
| `DestroyCaret(void) -> BOOL` | `:9477-9481` `BOOL WINAPI DestroyCaret(VOID)` | `"DestroyCaret"` | yes (AE) |
| `GetCaretBlinkTime(void) -> UINT` | `:9465-9469` `UINT WINAPI GetCaretBlinkTime(VOID)` | `"GetCaretBlinkTime"` | yes (126) |
| `GetCaretPos(LPPOINT) -> BOOL` | `:9502-9506` `BOOL WINAPI GetCaretPos(_Out_ LPPOINT)` | `"GetCaretPos"` | yes (127) |
| `HideCaret(HWND) -> BOOL` | `:9483-9487` `BOOL WINAPI HideCaret(_In_opt_ HWND)` | `"HideCaret"` | yes (200) |
| `SetCaretBlinkTime(UINT) -> BOOL` | `:9471-9475` `BOOL WINAPI SetCaretBlinkTime(_In_ UINT)` | `"SetCaretBlinkTime"` | yes (321) |
| `SetCaretPos(int, int) -> BOOL` | `:9495-9500` `BOOL WINAPI SetCaretPos(_In_ int, _In_ int)` | `"SetCaretPos"` | yes (322) |
| `ShowCaret(HWND) -> BOOL` | `:9489-9493` `BOOL WINAPI ShowCaret(_In_opt_ HWND)` | `"ShowCaret"` | yes (394) |

- **All eight correct as written; neither pre-scan P1 nor P2 touches this TU.** `winuser.h` mentions each stem
  exactly once (the declaration), i.e. no A/W family and no `stem -> stemW` macro, and the export table holds no
  `*A`/`*W` form of any caret name - so under D13 the bare `qor_pp_useswinapi` is the right spelling and `/EP`
  proves the literal name reaches `GetProcAddress` (as in `DestroyAcceleratorTable`, `KillTimer`/`SetTimer`,
  `NotifyWinEvent`, `GetSysColorBrush`). None of the eight stems is among the nine `#undef`s at `user32.h:9-17`,
  so G2 cannot bite here either. No `T` suffix on any wrapper - correct, since none has an A/W family.
- Declaration/definition pairing exact for all eight (`user32.h:460-467` against `caret.cpp:22,29,36,43,50,57,64,71`);
  return types, parameter types **and parameter names** match the SDK verbatim (`hWnd`, `hBitmap`, `nWidth`,
  `nHeight`, `uMSeconds`, `X`, `Y`, `lpPoint`). All eight are declared `static`, so the section is not part of the
  97 U4 non-static members and none of them is the U13 `ConsoleControl` shape.
- **Coverage complete in both directions**: 8 header declarations -> 8 definitions, and `user32.dll` exports exactly
  8 `*Caret*` names -> all 8 wrapped. The only other caret-named things in the header (`ConsoleSetCaretInfo`
  `user32.h:24`, `CONSOLE_CARET_INFO` `:58-62`) are console-internal types, not `user32` exports - see U10.
- **Fixed (documentation only - no line of code changed):** the TU had no comment. It now records why the bare stem
  is the correct binding for all eight entry points (so no later session "converts" them to `qor_pp_useswinapiAW`,
  which would ask `GetProcAddress` for the non-existent `CreateCaretW`), and the `CreateCaret` bitmap contract:
  `hBitmap` is `_In_opt_` (`NULL` = solid system caret), the bitmap is **not** consumed by the call, stays owned by
  the caller and must outlive the caret.

- Everything else already conformed: license header, the folder's include block identical to `brush.cpp`/
  `accelerator.cpp`, `qor_pp_fcontext;` in all eight, 4-space indent with no tabs and no trailing whitespace,
  `}}}//qor::win::api`, and `../returncheck.h` included but unused (folder-wide, logged under U2).
- **U14 contributes nothing from this TU**: 8/8 sites state the full SDK argument list and 8/8 use the spaced
  `Call< BOOL, HWND >` form, so neither half of U14 applies and nothing was retouched. The old U14(b) claim of
  "caret 2" bare sites was an artefact of the previous counting rule (it read `Call< BOOL >(pFunc)` - a complete
  zero-argument call - as if it deduced); U14(b) is corrected and re-measured with `tmp/_tmp_u14_census.ps1`.
- U1/U2 instance count for this TU: **8 call sites, 0 `Win32ErrorHandler`, 0 return checks** (settled policy).
  Under the `shell`/`kernel` policy the seven `BOOL` results would take `CheckReturn<BOOL, BoolCheck>`.
  **`GetCaretBlinkTime` must stay raw** - `0` is a legitimate blink interval (0 = the caret does not blink), not a
  failure - and equally no `CheckNonZero` arm may be added on `SetCaretBlinkTime`, because `SetCaretBlinkTime(0)` is
  the documented way to switch blinking off. So this TU would have 7 checkable sites and 1 deliberately unchecked.
  U5 (binding cached for process lifetime), U6 (8 more `__cdecl` `fPtr` call sites), U7 (a swallowed SEH fault
  returns `FALSE`/`0`, indistinguishable from a real failure) and U8 all apply unchanged.
- **Liveness - the whole caret surface is exported but never executed in-tree: the 8 wrappers are U15 instances.**
  The eight wrappers are reachable
  only through `platform::win::Caret` (`ui/gui/controllers/a_h/caret.cpp:35-73`, built at `gui/CMakeLists.txt:23`)
  and `WindowController::HideCaret`/`ShowCaret` (`windowcontroller.cpp:661-669`, declared `windowcontroller.h:259-260`)
  - and neither the `Caret` class nor those two methods has a single call site in `src`/`examples`/`test`. Linked,
  exported through `QOR_WINAPIUSER`, zero in-tree consumers: a published surface rather than dead code, but nothing
  in the built app exercises it. That is a `winqor_gui` observation, not a defect in this library; the `Caret`
  controller is already documented on the gui track (`docs/ui/windows/30-controllers.md:503-512`, which separately
  flags its un-narrowed `SetBlinkTime` return and the `umseconds` misspelling).
- Cross-boundary cast checked and **sound**: `Caret::GetPosition` hands `reinterpret_cast<LPPOINT>(&pt)`
  (`a_h/caret.cpp:57`) to `GetCaretPos`, where `pt` is `qor::platform::win::Point` = standard-layout
  `struct { long m_x; long m_y; }` (`ui/gui/view/drawing/point.h:31-36`) - layout-compatible with
  `POINT {LONG x; LONG y;}`, so the write-back is well-defined on the current definition (same class of check as
  session 2's `TIMERPROC` note). Any future member, base or virtual added to `Point` would break it silently.
- Verified: `/EP` after the edit still resolves exactly the eight literal names in the table above; `caret.cpp`
  compiles standalone at `/W4` exit 0; whole library 38/38 at `/W4`, exit 0, no diagnostics
  (`tmp/_tmp_cl_all.log`).

## clipboard.cpp - session 5 (29 Oct 2026)

The "//Clipboard functions" section (`user32.h:106-126`): twenty wrappers, twenty bindings, in exactly the header's
order. Second TU reviewed with nothing wrong in the wrappers (`caret.cpp` was the first) and the first that was already
**fully D13-conformant before the session started** - both A/W families bound with `qor_pp_useswinapiAW` and carrying
the `T` suffix, the other eighteen genuine single exports.

| Wrapper (definition line in the edited file) | SDK prototype (`winuser.h` 10.0.26100) | Resolved binding (`cl /EP`) | Export |
|---|---|---|---|
| `AddClipboardFormatListener(HWND hwnd) -> BOOL` (`:38`) | `:5568-5572` `_In_ HWND` [WINVER>=0x0600] | `"AddClipboardFormatListener"` | yes (1) |
| `ChangeClipboardChain(HWND hWndRemove, HWND hWndNewNext) -> BOOL` (`:45`) | `:5459-5464` `_In_` both | `"ChangeClipboardChain"` | yes (25) |
| `CloseClipboard(void) -> BOOL` (`:52`) | `:5424-5428` `CloseClipboard(VOID)` | `"CloseClipboard"` | yes (4F) |
| `CountClipboardFormats(void) -> int` (`:59`) | `:5510-5514` `(VOID)` | `"CountClipboardFormats"` | yes (5C) |
| `EmptyClipboard(void) -> BOOL` (`:66`) | `:5542-5546` `(VOID)` | `"EmptyClipboard"` | yes (E8) |
| `EnumClipboardFormats(UINT format) -> UINT` (`:73`) | `:5516-5520` `_In_ UINT` | `"EnumClipboardFormats"` | yes (F8) |
| `GetClipboardData(UINT uFormat) -> HANDLE` (`:80`) | `:5473-5477` `_In_ UINT` | `"GetClipboardData"` | yes (136) |
| `GetClipboardFormatNameT(UINT format, LPTSTR lpszFormatName, int cchMaxCount) -> int` (`:87`) | `:5522-5540` A/W pair, `_Out_writes_(cchMaxCount)`, macro `:5537/:5539` | `"GetClipboardFormatNameW"` | yes (138) |
| `GetClipboardOwner(void) -> HWND` (`:94`) | `:5441-5445` `(VOID)` | `"GetClipboardOwner"` | yes (13A) |
| `GetClipboardSequenceNumber(void) -> DWORD` (`:101`) | `:5433-5437` `(VOID)` [WINVER>=0x0500] | `"GetClipboardSequenceNumber"` | yes (13B) |
| `GetClipboardViewer(void) -> HWND` (`:108`) | `:5453-5457` `(VOID)` | `"GetClipboardViewer"` | yes (13C) |
| `GetOpenClipboardWindow(void) -> HWND` (`:115`) | `:5561-5565` `(VOID)` | `"GetOpenClipboardWindow"` | yes (191) |
| `GetPriorityClipboardFormat(UINT* paFormatPriorityList, int cFormats) -> int` (`:122`) | `:5554-5559` `_In_reads_(cFormats)` | `"GetPriorityClipboardFormat"` | yes (1AC) |
| `GetUpdatedClipboardFormats(PUINT lpuiFormats, UINT cFormats, PUINT pcFormatsOut) -> BOOL` (`:129`) | `:5580-5586` `_Out_writes_(cFormats)` [WINVER>=0x0600] | `"GetUpdatedClipboardFormats"` | yes (1D8) |
| `IsClipboardFormatAvailable(UINT format) -> BOOL` (`:136`) | `:5548-5552` `_In_ UINT` | `"IsClipboardFormatAvailable"` | yes (230) |
| `OpenClipboard(HWND hWndNewOwner) -> BOOL` (`:143`) | `:5418-5422` `_In_opt_ HWND` | `"OpenClipboard"` | yes (29C) |
| `RegisterClipboardFormatT(LPCTSTR lpszFormat) -> UINT` (`:150`) | `:5494-5508` A/W pair (`LPCSTR`/`LPCWSTR`), macro `:5505/:5507` | `"RegisterClipboardFormatW"` | yes (2E2) |
| `RemoveClipboardFormatListener(HWND hwnd) -> BOOL` (`:157`) | `:5574-5578` `_In_ HWND` [WINVER>=0x0600] | `"RemoveClipboardFormatListener"` | yes (301) |
| `SetClipboardData(UINT uFormat, HANDLE hMem) -> HANDLE` (`:164`) | `:5466-5471` `_In_ UINT`, `_In_opt_ HANDLE` | `"SetClipboardData"` | yes (328) |
| `SetClipboardViewer(HWND hWndNewViewer) -> HWND` (`:171`) | `:5447-5451` `_In_ HWND` | `"SetClipboardViewer"` | yes (329) |


- **All twenty correct as written; neither pre-scan P1 nor P2 touches this TU.** No clipboard stem is among the nine
  `#undef`s at `user32.h:9-17`, so G2 cannot bite here, and `winuser.h` defines a `stem -> stemW` macro for exactly two
  clipboard names (`:5505/:5507` and `:5537/:5539`) - both of which are already bound explicitly, so G1 is not
  load-bearing anywhere in this file. `/EP` after the edits resolves 18 literal stems plus `GetClipboardFormatNameW`
  and `RegisterClipboardFormatW`; every one is in the `user32.dll` export table with the ordinal shown above.
- **`T` suffix correct by construction**: present on exactly the two A/W families, absent on the other eighteen - the
  inverse of the P1 state in `keyboard.cpp`/`dialog.cpp`, where `T`-suffixed wrappers are bound bare.
- **A/W consistency argument (new, worth keeping):** `qor_pp_unicode` is 1 (`src/configuration/configuration.h:13`),
  so `kernel32.h:7-10` defines `UNICODE`/`_UNICODE`; `qor_pp_useswinapiAW` (`kernel32.h:52`) then picks the `W` entry
  point **and** `TCHAR`/`LPTSTR`/`LPCTSTR` are wide. One switch drives both halves, so a `*T` wrapper cannot end up
  wide-named with narrow parameters. `LPCTSTR` in `RegisterClipboardFormatT` is const (`LPCWSTR` in this build) exactly
  as the SDK's `_In_ LPCWSTR` wants, and `LPTSTR` in `GetClipboardFormatNameT` is non-const as its `_Out_writes_`
  parameter requires.
- Signature pairing exact against the SDK for all twenty - return type, parameter types **and parameter names**
  (`hwnd`, `hWndRemove`, `hWndNewNext`, `uFormat`, `hMem`, `format`, `lpszFormatName`, `cchMaxCount`,
  `paFormatPriorityList`, `cFormats`, `lpuiFormats`, `pcFormatsOut`, `lpszFormat`, `hWndNewOwner`). All twenty are
  declared `static`, so the section is not part of U4's 97 and none has U13's shape; `tmp/_tmp_static_census.ps1`
  re-run after the edits still reports exactly one defined-but-non-static member folder-wide (`ConsoleControl`).
- **Fixed (convention, U14(c)):** `()` -> `(void)` at `clipboard.cpp:52,59,115` and the matching declarations
  `user32.h:109,110,118` - the SDK spells these `(VOID)` and 33 of the folder's 38 zero-argument definitions already
  did. Zero behaviour change.
- **Fixed (documentation):** the TU had no comment. It now records the bare-stem rationale for the eighteen, the
  one-switch A/W argument, the affinity/ownership contracts (`OpenClipboard` takes the clipboard for the calling
  thread's task and must be matched by `CloseClipboard` on that thread; `EmptyClipboard` needs ownership;
  `SetClipboardData` transfers `hMem` to the system only on success; `GetClipboardData` is a borrowed system-owned
  handle) and the coverage/deprecation note.
- **Section complete in both directions:** 20 header declarations -> 20 definitions, none of them defined in another TU
  (no U12 transposition, no U4 gap, no U11 unqualified definition); and 20 of the 21 clipboard entry points
  `winuser.h:5418-5587` declares are wrapped. The one unwrapped SDK entry point is `GetClipboardMetadata`
  (`:5487-5492` + `GETCLIPBMETADATA`). `user32.dll` exports two further clipboard names,
  `CheckProcessForClipboardAccess` and `GetClipboardAccessToken`, that have no public SDK declaration and are therefore
  out of reach of this class by design.
- **U16 opened from this TU:** 4 of the 20 SDK declarations (`GetClipboardSequenceNumber`,
  `AddClipboardFormatListener`, `RemoveClipboardFormatListener`, `GetUpdatedClipboardFormats`) sit behind `WINVER`
  guards, and the build pins no `WINVER`/`_WIN32_WINNT`/`NTDDI_VERSION` anywhere - the prototypes are visible only
  through the SDK default (`sdkddkver.h:227-229`, `:246-253` = `0x0A00`).

- **U1/U2 instance count: 20 call sites, 0 `Win32ErrorHandler`, 0 return checks** (settled policy). Under the
  `shell`/`kernel` policy 11 sites would take a mechanical check (8 `BOOL` -> `CheckReturn<BOOL, BoolCheck>`,
  2 `HANDLE` -> `CheckNonZeroHandle<HANDLE>`, `RegisterClipboardFormatT` -> `CheckNonZero<UINT>`) and 9 must stay raw
  because zero/NULL is a legitimate answer (`CountClipboardFormats`, `EnumClipboardFormats` where 0 ends the
  enumeration, `GetClipboardOwner`, `GetClipboardViewer`, `GetOpenClipboardWindow`, `SetClipboardViewer`,
  `GetClipboardSequenceNumber` where 0 also means "not associated"), while `GetClipboardFormatNameT` is ambiguous by
  contract (0 for a standard format *and* for failure) and `GetPriorityClipboardFormat` is **inverted** - 0 = failure,
  -1 = "none of the listed formats" - so the folder's non-zero idiom would fire on its success path, and
  `IsClipboardFormatAvailable` would raise an issue on every ordinary "not available" answer. This TU is the strongest
  instance of the U2 decision: a mechanical rule would be wrong on a third of the surface.
- **U14 contributes nothing to change:** 20/20 sites state the full SDK argument list (0 deducing) and 20/20 use the
  spaced `Call< BOOL, HWND >` form, so neither half of U14(a)/(b) applies; nothing was retouched on those grounds.
- **U15: all 20 wrappers are instances** (analysis in the U15 list). The notable part is that the tree does do clipboard
  work - through a **parallel direct-SDK path** in `ui/gl` (`platform.cpp:143-158`, `:170-202` - 9 live sites), complete
  with its own `while (!OpenClipboard(...))` retry-on-busy loop, which never reaches `GetProcAddress`. gui's
  `ClipboardHandler` handles the clipboard messages but calls no wrapper, and its slot `m_clipboardOwner` has no writer,
  so it is never installed. *(Corrected 30 Oct 2026, session 8: this entry also cited `windowimpl.cpp:2395-2410` as part
  of that path. It is not - the whole region sits inside a block comment that opens at `windowimpl.cpp:2221` and closes at
  `:2498`, verified with `tmp/_tmp_comment_state.ps1`. The 9 `platform.cpp` sites were re-checked the same way and are all
  CODE. Counted as open item U19's first instance.)*
- U5 (binding cached for the process lifetime), U6 (20 more default-`__cdecl` `fPtr` call sites), U7 (a swallowed SEH
  fault returns `FALSE`/`NULL`/`0`, indistinguishable from a real clipboard failure - and clipboard code is exactly
  where a use-after-`CloseClipboard` fault is plausible) and U8 all apply unchanged. No cross-boundary type work in
  this TU: no QOR struct is reinterpreted as a clipboard SDK struct and `GetUpdatedClipboardFormats`'s buffer is
  caller-owned, so no layout-compatibility check of the session-2/session-4 kind was needed.
- Verified: `/EP` after the edits resolves exactly the twenty names in the table; `clipboard.cpp` compiles standalone
  at `/W4` exit 0; whole library **38/38 at `/W4`, exit 0**, no diagnostics (`tmp/_tmp_cl_all.log`).

## combobox.cpp - session 6 (29 Oct 2026)

The "//Combo Box functions" section (`user32.h:550-553`): three wrappers, three bindings, in header order, and the
first TU in this review that contains **bindings that could never work** - two of the three asked `GetProcAddress` for
a name `user32.dll` does not export, so those two wrappers could only ever throw.

| Wrapper (spelling after this session) | SDK entry (`winuser.h` 10.0.26100) | Binding before -> after (`cl /EP`) | Export |
|---|---|---|---|
| `DlgDirListComboBoxT(HWND, LPTSTR, int, int, UINT) -> int` | `:11643-11665` `DlgDirListComboBoxA/W`, `#define DlgDirListComboBox DlgDirListComboBoxW` at `:11662` (`#undef`'d at `user32.h:15`) | `"DlgDirListComboBox"` MISSING -> `"DlgDirListComboBoxW"` | yes (C3/C2, probe) |
| `DlgDirSelectComboBoxExT(HWND, LPTSTR, int, int) -> BOOL` | `:11667-11687` `DlgDirSelectComboBoxExA/W`, macro at `:11684` (`#undef`'d at `user32.h:16`) | `"DlgDirSelectComboBoxEx"` MISSING -> `"DlgDirSelectComboBoxExW"` | yes (C6/C5, probe) |
| `GetComboBoxInfo(HWND, PCOMBOBOXINFO) -> BOOL` | `:14771-14787` `COMBOBOXINFO` + `WINUSERAPI BOOL WINAPI GetComboBoxInfo(_In_ HWND, _Inout_ PCOMBOBOXINFO)` | `"GetComboBoxInfo"` (unchanged) | yes (13D) |

- **Fixed (correctness).** Both `DlgDir*` bindings are now `qor_pp_useswinapiAW(user32, <stem>)`. The reason the bare
  spelling is fatal *for these two specifically* is that `user32.h:15,16` `#undef` the stems, so G2's macro rescue
  cannot fire and `_ATXT(stem)` stringizes the literal, un-exported name. `qor_pp_useswinapiAW` token-pastes `W`
  before expansion, so the `#undef` is irrelevant to it. Live probe after the fix: all three names RESOLVED.
- **Fixed (naming, user decision).** `user32.h:551-552` and the two definitions now read `DlgDirListComboBoxT` /
  `DlgDirSelectComboBoxExT`, restoring the folder's `T`-suffix norm for A/W wrappers. Zero call sites existed for
  either name anywhere in `src`/`examples`/`test`, so no call-site edits were needed and the rename cannot change
  behaviour in this tree. `GetComboBoxInfo` keeps the bare SDK name (no A/W family; D13 names it as the example).
  The remaining 11 such members are U17, and the mirror case (`T` with no A/W family) is pre-scan P3.
- **Fixed (consistency).** All three bindings' module token `User32` -> `user32` (the `_TXT()` argument of
  `GetModuleHandle`; the run-time lookup is case-insensitive, so this is spelling only - G4). Folder-wide census over
  all 428 bindings: 416 now `user32`, 12 `User32`, all in `scrollbar.cpp`, which keeps them for its own session.
- **Fixed (documentation).** New TU comment: why the third binding is bare, why the pair must be AW, the `_Inout_`
  `lpPathSpec` / `_Out_writes_(cchOut)` `lpString` buffer contracts, the `DDL_*` filter set, the
  `cbSize`-pre-set-and-child-handles-are-control-owned contract of `GetComboBoxInfo`, the `T`-suffix rationale and the
  section-coverage claim.
- **Verified correct as written:** all three return types, parameter types, parameter count and order against the SDK;
  `PCOMBOBOXINFO` is used as the SDK type rather than a QOR stand-in; `qor_pp_fcontext` in all three; 4-space indent,
  file-local includes last, `}}}//qor::win::api`; the whole `//Combo Box functions` section is `static` (unlike the
  adjacent List Box and DDE blocks - U4) and all three members are defined, so this TU is not part of U4's gap.
- **Left alone deliberately:** the tight `Call<...>` spacing, 3/3 explicit template arguments (U14 - no churn); no
  `Win32ErrorHandler _;` and no `CheckReturn` at any of the 3 sites (settled policy, U1/U2 - this TU's tally is 3 call
  sites, 0 handlers, 0 checks), so all three failures are silent today; `returncheck.h` included and unused; the
  binding still caches in a function-local `static const` for process lifetime (U5); `Call`'s `fPtr` is still the
  default-`__cdecl` typedef against `WINAPI` exports - 3 more U6 sites; a swallowed SEH fault returns a default
  `int`/`BOOL` (U7); U8 (no test, no caller) applies, and 2 of the 3 wrappers became U15 instances.
- **Return-contract note for a future return-check decision:** `DlgDirListComboBoxT`'s documented result is a list-box
  index with `-1` on failure, so neither a `CheckReturn` non-zero test nor `!= 0` is valid for it (index 0 is
  legitimate); `DlgDirSelectComboBoxExT` and `GetComboBoxInfo` are plain `BOOL`s where `CheckNonZeroHandle`-style
  checking would be the `shell`-library spelling.
- **Section coverage: complete.** `user32.dll` exports exactly three `*Combo*` entry points (`GetComboBoxInfo` plus the
  two `DlgDir*ComboBox*` families), all wrapped; the combo-box *messages* (`CB_ADDSTRING`, `CB_DIR`, ...) are not
  entry points and are reached through `SendMessageT` from `ui/gui`.
- **Found while cross-checking, not fixed (not this TU): `window.cpp:124`** binds
  `qor_pp_useswinapiAW(user32, CreateWindow)` inside `User32::CreateWindowT` - `CreateWindowW` is MISSING, `CreateWindow`
  being the SDK's macro for `CreateWindowEx`. Now the largest remaining P1 item; see the pre-scan entry.
- Verified: `cl /EP` post-edit resolves `"DlgDirListComboBoxW"`, `"DlgDirSelectComboBoxExW"`, `"GetComboBoxInfo"`
  against `L"user32"`; live probe RESOLVED for all three; `combobox.cpp` compiles standalone at `/W4` exit 0; whole
  library **38/38 at `/W4`, exit 0, 0 diagnostics** (`tmp/_tmp_cl_all.log`) - the header rename broke no other TU.


## console.cpp - session 7 (30 Oct 2026)

The "Console functions" section (`user32.h:689-690`): one wrapper, one binding - and the only TU in the review whose
wrapper has **no SDK declaration at all** to pair against, so method step 2 (SDK prototype comparison) had no target and
the export table became the sole authority for the name.

| Wrapper | Declaration / definition | Resolved binding (`cl /EP`) | Export |
|---|---|---|---|
| `ConsoleControl(CONSOLECONTROL, PVOID, DWORD) -> NTSTATUS` | `user32.h:690` (was non-static - **U13**, now `static`) / `console.cpp:18+` | `"ConsoleControl"` against `L"user32"` | yes - ordinal 1594, hint 55, RVA 00028FD0, real code (not one of the four table forwarders) |

- **Binding correct as written; no P1/P2 conversion needed.** `user32.dll` exports exactly one `*Console*` name (fresh
  `dumpbin /exports`: the `Console` grep returns that single line), so there is no A/W family and no `stem -> stemW`
  macro - live probe: `ConsoleControl` RESOLVED, `ConsoleControlA` MISSING, `ConsoleControlW` MISSING. Under D13 the bare
  `qor_pp_useswinapi` is therefore the right spelling and `/EP` proves the literal name reaches `GetProcAddress`. No `T`
  suffix on the wrapper - correct. The stem is not one of the nine `#undef`s at `user32.h:9-17`, so G2 does not apply,
  and the U17 census classifies it with the 404 correct single-export members.
- **The signature cannot be verified against Microsoft - the headline risk of this TU, not a defect in it.**
  `ConsoleControl`, `CONSOLECONTROL` and the five structures at `user32.h:19-62` appear in **no** header of
  SDK 10.0.26100.0 or 10.0.10240.0 (whole `Include\` tree grepped; `wincon.h` only pulls in
  `consoleapi.h`/`consoleapi2.h`/`consoleapi3.h`, whose sole hit for the pattern set is `SetConsoleMode` at
  `consoleapi.h:138`). Consequences recorded: no collision risk (U10's SDK-side check is now closed), the enum numbering
  (0-7 with `Reserved1/2/3` holes) and the structure layouts rest on the de-facto internal definition rather than on
  anything installed here, and the parameter names (`Command`, `ConsoleInformation`, `ConsoleInformationLength`) are the
  project's own with no authority to check them against. `NTSTATUS` is the one SDK-supplied type in the signature
  (`typedef LONG NTSTATUS`, `/EP` line 320947), so the return really is a 32-bit signed status. A newer console may
  expose control codes past `ConsoleEndTask` (7); nothing installed here can tell us, so the enum is recorded as
  *possibly incomplete* - a documentation gap, not a defect.
- **Fixed (U13, the promised one-token fix).** `static` added at `user32.h:690`. Measured both ways with
  `tmp/_tmp_console_static_probe.cpp` (compile-only caller probe, `ConsoleSetWindowOwner` + `CONSOLEWINDOWOWNER`): before
  = **1 x C2352** "a call of a non-static member function requires an object", exit 2; after = `/W4` exit 0. Census
  re-run after the fix: 525 member-declaration-shaped lines, 428 `static` (was 427), 426 distinct defined names, zero
  defined-but-non-static. `desktop.cpp` no longer needs the U10 collision check - the grep was whole-tree.
- **Fixed (nits).** Missing space `,DWORD` -> `, DWORD` at the declaration; section comment `//Console` ->
  `//Console functions` (every other section comment ends in "functions"); dead `#undef WinHelp` at `console.cpp:14`
  removed - the TU references `WinHelp` nowhere and `user32.h:10` already undefs the stem, so the line was inert in
  every state of this file (`help.cpp:14` carries the same line and keeps it for that TU's session).
- **Fixed (documentation).** New TU comment inside the namespace (the `brush.cpp`/`combobox.cpp` pattern): the
  no-SDK-authority finding, the section-completeness claim, the bare-stem/D13 rationale with the live-probe numbers, the
  caller-owned in/out buffer contract (`ConsoleInformation` must be the structure belonging to the chosen control code,
  `ConsoleInformationLength` its size, nothing allocated or freed by the call), the NTSTATUS success semantics, and the
  console-ownership / access-denied behaviour of the sensitive control codes.
- **Verified correct as written:** return and parameter types against the export's real contract; `Library::Call<NTSTATUS,
  CONSOLECONTROL, PVOID, DWORD>` explicit 1/1 with tight spacing (no U14 churn); `qor_pp_fcontext` present; the include
  block identical to the folder norm (4 QOR includes, then `user32.h`, `../returncheck.h`, `../library.h` last);
  4-space indent; `}}}//qor::win::api`.
- **Left alone deliberately:** no `Win32ErrorHandler _;` and no `CheckReturn` at the single call site (settled policy,
  U1/U2 - this TU's tally is 1 call site, 0 handlers, 0 checks); `../returncheck.h` included and unused; the binding is
  cached in a function-local `static const` for process lifetime (U5); `Call`'s default-`__cdecl` `fPtr` against a
  `WINAPI` export (U6, 1 more site) - and here the callee really is user32's own stub, since `win32u.dll` exports
  `NtUserConsoleControl` and not `ConsoleControl` (probe: MISSING for the latter), so no forwarding is involved.
- **Return-contract note for any future return-check decision (the worst U7 instance so far).** NTSTATUS success is
  `>= 0` (0 = `STATUS_SUCCESS`, positive = informational), so `== 0` is not a valid failure test and `returncheck.h` has
  no NTSTATUS-aware check - only `SuccessCheck`/`TSuccessCheck`, which test `== 0`. Worse, `Library::Call` turns a
  swallowed SEH fault into `NTSTATUS _{}` = **0 = STATUS_SUCCESS**: a fault in this wrapper is not merely silent, it is
  reported as success. If U2 is ever reopened, this is the wrapper that needs a real check class (test `< 0`) and a
  non-zero default.
- **Surface-scope note (feeds U9's missing scope note).** The console surface a caller would actually want is in
  `kernel32.dll` - 135 `*Console*` exports (`AllocConsole`, `GetConsoleWindow`, `ReadConsoleW`, `WriteConsoleW`, ...) -
  and `api_layer/kernel/kconsole.cpp` already wraps it, with the kernel error-handler idiom intact; `user32.dll` has
  exactly one, which is why this section is one wrapper long. Nothing in `src`/`examples`/`test` calls this wrapper
  (U15 instance), and there is no console controller in `ui/gui` that could.
- Verified: `/EP` after the edits still shows `qor_pp_useswinapi` -> `GetModuleHandleW(L"user32")` + `"ConsoleControl"`;
  `console.cpp` compiles standalone at `/W4` exit 0; the caller probe compiles `/W4` exit 0; the whole library compiles
  **38/38 at `/W4`, exit 0, no diagnostics** (`tmp/_tmp_cl_all.log`) - the `static` addition broke no other TU.

---

## coordtransform.cpp - session 8 (30 Oct 2026)

Smallest-but-one TU (3 wrappers, 3 bindings) and the most consequential find of the review so far: **every wrapper in
the file called the wrong entry point.**

| Wrapper (`user32.h`) | Binding as found | Binding after fix | Verdict |
|---|---|---|---|
| `:377 static BOOL ClientToScreen(HWND, ::LPPOINT)` | `qor_pp_useswinapi(user32, GetDCEx)` | `qor_pp_useswinapi(user32, ClientToScreen)` | Fixed - wrong export |
| `:378 static BOOL ScreenToClient(HWND, ::LPPOINT)` | `qor_pp_useswinapi(user32, GetDCEx)` | `qor_pp_useswinapi(user32, ScreenToClient)` | Fixed - wrong export |
| `:379 static int MapWindowPoints(HWND, HWND, ::LPPOINT, UINT)` | `qor_pp_useswinapi(user32, GetDCEx)` | `qor_pp_useswinapi(user32, MapWindowPoints)` | Fixed - wrong export |

- **What the defect actually did.** One binding line, copied into three bodies. `GetDCEx` is
  `HWND GetDCEx(HWND hWnd, HRGN hrgnClip, DWORD flags)` (`winuser.h:8575`), so each call passed the wrapper's `lpPoint`
  as `hrgnClip`, left `flags` as whatever the third argument register held, and returned an `HDC` that `Library::Call`
  truncated to `BOOL`/`int`. Consequences, in order of severity: (a) no coordinate was ever transformed - the caller's
  POINT is untouched; (b) the return value is not the API's, so the `? true : false` collapse in
  `windowcontroller.cpp:643,:648` reported success whenever the HDC's low 32 bits were non-zero; (c) passing a `POINT*`
  where USER expects a region handle makes USER dereference caller memory it does not own, and a successful call acquires
  a device context that nobody ever releases; (d) `ClientToScreen`'s FALSE-on-bad-hWnd channel is gone entirely.
- **Why nothing caught it, and the new instrument.** `GetDCEx` is a genuine export (table index 1830, ordinal 144), the
  module token was already `user32`, `cl /EP` emitted a valid literal, the TU compiles clean at `/W4`, and the call site
  type-checks because `Library::Call` receives a cast function pointer and never sees a prototype. Every check in the
  method up to step 3 therefore passed. The missing check is *binding-vs-member agreement*, now
  `tmp/_tmp_wrong_stem_scan.ps1` (pre-scan P5): 428 bindings over 38 TUs, 7 flags at the first run, 4 after this fix, of
  which one is a live defect (`mouse.cpp:82`, added to P1), one is the known P3 case (`cursor.cpp:26` binds the single
  `CopyIcon` export) and two are the script correctly failing to find an enclosing `User32::` member in `resource.cpp`
  (i.e. it reproduces U11 mechanically).
- **It was not dead code.** Unlike the previous seven TUs, all three wrappers are called in-tree -
  `windowcontroller.cpp:643` (`WindowController::ClientToScreen`), `:648` (`ScreenToClient`), `:653`
  (`WindowController::MapPoints` -> `MapWindowPoints`) - and reachable one level up from `WindowImpl::GetPos`
  (`windowimpl.cpp:1657`) and `WindowImpl::SetCursorPosWin32` (`:2218`). So `GetPos` was returning the untranslated
  client origin and `SetCursorPosWin32` was placing the cursor from unconverted coordinates while leaking a DC per call.
  First non-U15 TU of the review; recorded in the U15 list. (A naive grep suggested `windowimpl.cpp` called the SDK forms
  directly at eight sites - **all eight are inside `/* ... */` blocks** (`:257-268`, `:325-408`, `:530-1301`, `:2042-2210`)
  and are not compiled; C comments do not nest, so the block opened at `:2042` runs to the first `*/` at `:2210`. The two
  live calls at `:1657`/`:2218` go to the private base `WindowController` (`windowimpl.h:38`) and from there through the
  layer, so this family has **0** bypass sites. Measured with `tmp/_tmp_bypass_scan.ps1`; the phenomenon is open item
  **U19** and per-session counting of it is now review-method step 5b.)

- **Binding verification after the fix.** `cl /EP` on the edited TU resolves all three to the literal names with module
  token `L"user32"`: `"ClientToScreen"`, `"ScreenToClient"`, `"MapWindowPoints"`. Live probe against the loaded
  `user32.dll`: all three RESOLVED; `ClientToScreenA/W`, `ScreenToClientA/W`, `MapWindowPointsA/W` all MISSING, so these
  are single exports and the bare `qor_pp_useswinapi` is the D13 spelling (no `AW`, no `T` suffix - the header spellings
  at `:377-379` are correct as written and were not touched). Export table: `ClientToScreen` 1586/4D, `ScreenToClient`
  2282/30C, `MapWindowPoints` 2146/27F; `MapVisualRelativePoints` (2145/27E) sits beside it in the table but is
  undocumented and declared nowhere in this library.
- **SDK pairing.** `winuser.h:9508-9513`, `:9515-9520`, `:9556-9563`; the wrappers' parameter lists match the SDK's
  `_In_ HWND` / `_Inout_ LPPOINT` / `_In_opt_ HWND` x2 / `_Inout_updates_(cPoints) LPPOINT` + `_In_ UINT` exactly, and the
  return types match (`BOOL`, `BOOL`, `int`). **U16: 0 version-guarded declarations** - the cleanest tally so far, only
  `#ifndef NOUSER` plus the Desktop-family partition (measured with `tmp/_tmp_sdk_guard_scan.ps1`). The reverse half of
  U16 (library declarations of SDK names the SDK would withhold) is contributed by the neighbouring `user32.h:188,:191`
  pair, not by this TU.
- **Section coverage.** The header's "Coordinate Transform functions" section is these three. The SDK's wider
  coordinate-conversion neighbourhood (`winuser.h:9522-9554`, `:9565-9576`) lives in other header sections
  (`Window functions`: `WindowFromPoint` `:212`, `WindowFromPhysicalPoint` `:211`, the `Logical/PhysicalTo*` pair
  `:188,:191`; `Cursor functions`: `GetPhysicalCursorPos` `:390`). The two `*Point` converters that *are* defined return
  `void` where the SDK returns `BOOL` - **new open item U18**, for `window.cpp`'s session. The two
  `*ForPerMonitorDPI` variants are not declared anywhere in this library (noted under U18 rather than opened separately).
- **Open items this session contributes to (recorded, not acted on):**
  - U1/U2 - 3 more call sites, `0` with `Win32ErrorHandler _;`, `0` wrapping the result in
    `CheckReturn`/`CheckNonZero`. This TU is the strongest argument yet for reopening them: all three functions have a
    real failure channel and the only two callers already collapse the result with `? true : false`
    (`windowcontroller.cpp:643,:648`), so even with the bindings fixed a failure stays silent - but the value now at least
    means what the SDK says. `MapWindowPoints` is the deliberate counter-example: non-zero is a coordinate delta, so a
    `CheckNonZero` there would be wrong by construction.
  - U3 - none. U4 - none (all three declared, all three defined). U6 - 3 more sites hand a `Library::DefProc`
    (`__stdcall`) to `Library::Call`, whose `fPtr` is default-`__cdecl` (13 known sites now).
  - U7 - applies to all three: a fault inside the callee surfaces as `FALSE`/`0`, indistinguishable from a legitimate
    zero (for `MapWindowPoints` that is exactly the common success case).
  - U8 - untested, but unlike the previous TUs it *is* called in-tree, so a test could observe real behaviour through
    `WindowController::ClientToScreen` rather than only checking that a symbol links.
  - U10, U11 - none. U13 - **none, and this is the first reviewed section whose defined members were all already
    `static`** (`user32.h:377-379`), so no `C2352` caller risk existed here at all.
  - U14 - contributes nothing to change: 3/3 sites state the full SDK argument list explicitly and 3/3 use the spaced
    `Call< BOOL, HWND, ::LPPOINT >` form, which is file-consistent (3/3), so no churn per the no-clobber rule.
  - U17 - nothing to rename: no A/W family, so no `T` suffix is due and none is present.
- **Conventions checked and clean:** licence header, include order, Windows headers last, `qor_pp_fcontext` at all 3
  sites, lowercase `user32` module token, section comment `//Coordinate Transform functions` already in the "ends in
  functions" house style (U14(d) - unchanged), no local `#undef`, 4-space indent, no tabs, definitions in header order.
  Added: the TU comment block (D13 binding rationale, the `_Inout_` caller-owned POINT contract, the "non-zero
  `MapWindowPoints` return is a delta" warning, section coverage, and the U18 pointer).
- **Verified at exit:** `cl /EP` -> the three literals above; `coordtransform.cpp` compiles standalone at `/W4` exit 0;
  the whole library compiles **38/38 at `/W4`, exit 0, no diagnostics** (`tmp/_tmp_cl_all.log`; the single
  `warning|error` grep hit is the filename `errorhandling.cpp`). No header change and no rename, so no published surface
  moved and no caller needed recompiling - the fix changes run-time behaviour only.


## cursor.cpp - session 9 (30 Oct 2026)

Sixteen wrappers, sixteen bindings - the largest TU reviewed and **0 defects of any class** (binding, signature, UB,
convention). It is therefore the TU where the method had to earn its keep: nothing to fix, so everything the earlier
sessions had been asserting by inspection was measured instead. Two instruments changed as a result, one open item (U20)
was opened from the caller side, and the pre-flagged P3 case was settled as correct.

| Wrapper (`user32.h:381-397`, all 16 `static`) | Definition | Binding (`cl /EP` literal) | Export (ordinal, RVA) | SDK prototype (`winuser.h` 10.0.26100) | Verdict |
|---|---|---|---|---|---|
| `ClipCursor(const RECT*) -> BOOL` | `:53` | `user32, ClipCursor` -> `"ClipCursor"` | 4E, 0002EA90 | `:9594 BOOL WINAPI ClipCursor(_In_opt_ CONST RECT*)` | correct |
| `CopyCursorT(HCURSOR) -> HCURSOR` | `:60` | `user32, CopyIcon` -> `"CopyIcon"` | 59, 00027DF0 | `:10976 HICON WINAPI CopyIcon(_In_ HICON)`; `CopyCursor` is the `:10681` macro | correct (see below) |
| `CreateCursor(HINSTANCE,int,int,int,int,const VOID*,const VOID*) -> HCURSOR` | `:67` | `"CreateCursor"` | 60, 0004CF40 | `:10659` | correct |
| `DestroyCursor(HCURSOR) -> BOOL` | `:74` | `"DestroyCursor"` | AF, 00029250 | `:10671` | correct |
| `GetClipCursor(LPRECT) -> BOOL` | `:81` | `"GetClipCursor"` | 134, 0002EE40 | `:9447` | correct |
| `GetCursor(void) -> HCURSOR` | `:88` | `"GetCursor"` | 13F, 0002EE90 | `:9453 HCURSOR WINAPI GetCursor(VOID)` | correct |
| `GetCursorInfo(PCURSORINFO) -> BOOL` | `:95` | `"GetCursorInfo"` | 141, 0002EEA0 | `:14661` | correct |
| `GetCursorPos(LPPOINT) -> BOOL` | `:102` | `"GetCursorPos"` | 142, 0002B230 | `:9426` | correct |

| `GetPhysicalCursorPos(LPPOINT) -> BOOL` | `:109` | `"GetPhysicalCursorPos"` | 193, **0002B230** | `:9439` | correct |
| `LoadCursorT(HINSTANCE,LPCTSTR) -> HCURSOR` | `:116` | `useswinapiAW` -> `"LoadCursorW"` | 259, 00015C30 | `:10619` (macro `:10623`) | correct |
| `LoadCursorFromFileT(LPCTSTR) -> HCURSOR` | `:123` | `useswinapiAW` -> `"LoadCursorFromFileW"` | 258, 00048300 | `:10642` (macro `:10645`) | correct |
| `SetCursorPos(int,int) -> BOOL` | `:130` | `"SetCursorPos"` | 32E, 0002FA50 | `:9398` | correct |
| `SetPhysicalCursorPos(int,int) -> BOOL` | `:137` | `"SetPhysicalCursorPos"` | 354, **0002FA50** | `:9406` | correct |
| `ShowCursor(BOOL) -> int` | `:144` | `"ShowCursor"` | 395, 0002FD90 | `:9386 int WINAPI ShowCursor(_In_ BOOL)` | correct |
| `SetCursor(HCURSOR) -> HCURSOR` | `:151` | `"SetCursor"` | 32C, 0002A790 | `:9420 HCURSOR WINAPI SetCursor(_In_opt_ HCURSOR)` | correct |
| `SetSystemCursor(HCURSOR,DWORD) -> BOOL` | `:158` | `"SetSystemCursor"` | 36A, 0007EB90 | `:10728` | correct |

- **All sixteen resolve, and by two independent routes.** `cl /EP` of the TU prints the sixteen `GetProcAddress` literals
  (`tmp/_tmp_cursor_pp.txt:453241-453346`), and a live probe that resolves them exactly the way `qor_pp_useswinapi` does
  (`GetModuleHandleW(L"user32")` then `GetProcAddress`) resolves **16 of 16 RESOLVED**, plus the A/W alternatives, plus the
  names that should fail (`tmp/_tmp_cursor_probe.cpp`: `CopyCursor` MISSING, `SetCursorContents`/`ShowSystemCursor`/
  `GetCursorFrameInfo` RESOLVED). Note the probe's first line: `GetModuleHandleW(L"user32")` returns NULL in a process that
  has not mapped the DLL - the library never `LoadLibrary`s, so `pFunc` would be cached NULL forever. That is G5/U5
  demonstrated rather than asserted; recorded, not fixed.
- **None of the sixteen stems is in the `user32.h:9-17` `#undef` list** (measured against the header: 9 names -
  `RegisterDeviceNotification`, `WinHelp`, `SystemParametersInfo`, `GetRawInputDeviceInfo`, `DlgDirList`,
  `DlgDirSelectEx`, `DlgDirListComboBox`, `DlgDirSelectComboBoxEx`, `LoadBitmap`). So G2 - the SDK `stem -> stemW` macro
  making a bare-stem binding a live P1 failure - cannot bite in this family, which is why fourteen bare
  `qor_pp_useswinapi` spellings are correct here rather than a defect class.
- **The pre-scan P3 flag (`CopyCursorT` -> `CopyIcon`) is a correct binding, and now for a measured reason.** `CopyCursor`
  is not an export at all: `winuser.h:10681` is `#define CopyCursor(pcur) ((HCURSOR)CopyIcon((HICON)(pcur)))` (inside
  `#ifndef _MAC`), and the live probe says `CopyCursor` MISSING / `CopyIcon` RESOLVED. It is also the mirror case the U17
  census reports for this section (`T` suffix with no A/W family). Left as-is: renaming a member exported through
  `QOR_WINAPIUSER` is a published-surface change and belongs with the folder-wide U17/P3 decision.
- **The type story behind that wrapper, measured rather than assumed:** `HCURSOR` *is* `HICON` in this build.
  `windef.h:88` is `DECLARE_HANDLE(HICON)` and `:131` is `typedef HICON HCURSOR;`; the separate
  `DECLARE_HANDLE(HCURSOR)` at `:133` is the `#else` arm of the same `#ifndef _MAC`, so it is not what this TU gets.
  `/EP` of `cursor.cpp` contains exactly one HCURSOR spelling - `typedef HICON HCURSOR;`
  (`tmp/_tmp_cursor_pp.txt:174186`). So `Library::Call< HCURSOR, HCURSOR >` against the `HICON WINAPI(HICON)` export is
  type-identical and needs no cast; the only remaining hazard in that path is U6's `__cdecl` `fPtr`, shared by every
  wrapper in the folder.
- **Session 8's defect class is closed for this family by construction, and that was tested.** A wrapper in this class
  cannot call the SDK entry point of the same name even by accident: inside `User32`, an unqualified `SetCursorPos(x, y)`
  binds to the member itself. Probe `tmp/_tmp_recursion_probe.cpp` compiles that shape and MSVC gives
  `warning C4717: 'User32::SetCursorPos' : recursive on all control paths, function will cause runtime stack overflow` -
  a warning, not an error, so it would still build and then recurse. All 16 bodies here go through `pFunc`, and
  `tmp/_tmp_wrong_stem_scan.ps1` over the whole folder now flags `cursor.cpp:63` only as the known `CopyIcon` alias: 4
  flags folder-wide after session 8's fix, this one, `mouse.cpp:82` (`TrackMouseEvent` binding
  `winapiAW(user32, LPTRACKMOUSEEVENT)` - a live P1-class find for mouse.cpp's session), and the two `resource.cpp`
  "binding outside a `User32::` definition" lines that reproduce U11 mechanically.
- **Two run-time facts the SDK declarations do not show** (export table, this `user32.dll`): the logical/physical pairs are
  *one implementation exported twice* - `GetCursorPos` 142 and `GetPhysicalCursorPos` 193 both at RVA **0002B230**,
  `SetCursorPos` 32E and `SetPhysicalCursorPos` 354 both at RVA **0002FA50**. Whatever DPI behaviour the
  `WINVER >= 0x0600` declarations draw, it is not observable through this table on this build.

- **Ownership is now written where it belongs.** `CreateCursor`, `LoadCursorFromFileT` and `CopyCursorT` return handles the
  caller must `DestroyCursor`; `LoadCursorT` and `GetCursor` return shared system handles that must not be destroyed;
  `SetSystemCursor` takes no ownership (its SDK declaration carries no ownership SAL). `ClipCursor` is `_In_opt_` (NULL
  clears the clip rect) and `GetCursorInfo` requires `pci->cbSize` from the caller first. `ShowCursor` returns the signed
  display *count*, so 0 or negative is a normal hide - there is no failure channel for a return check to read. All of that
  is now in the TU comment; none of it is stated in the callers, which is **open item U20** (four measured caller-side
  findings: default-`true` ownership in the `Cursor`/`Icon` ctors, `ButtonController` omitting the flag its sibling
  `StaticController` passes, a spurious `PrimitiveHandle` leak warning on every destruction because `~Cursor` never clears
  `m_handle`, and `Cursor::Show` collapsing the count to `bool`). Nothing outside `cursor.cpp` was touched.
- **U19 - the cleanest family of the review: 0 bypass sites in built code.** 17 **LAYER** sites (one per wrapper,
  `LoadCursorT` twice, all in `ui/gui/gdiobjects/cursor.cpp`, built at `gui/CMakeLists.txt:43`), 13 raw **BYPASS** hits, of
  which 9 are not SDK calls at all once the enclosing declaration is read (`BlobBuffer::SetCursor`, `Input::DestroyCursor`,
  the two-argument `Platform::SetCursor`, two member declarations). That is why `_tmp_bypass_scan.ps1` gained a fourth
  class, **MEMBER**. The remaining 4 are live SDK calls, all in one file and all in a target that is **not in the build**:
  `ui/gl/windowimpl.cpp:247` (`SetCursor` + `LoadCursorW`), `:254`, `:272` (`::ClipCursor(nullptr)`), `:2219`
  (`::SetCursorPos`) - `winqor_gl` is commented out at `ui/CMakeLists.txt:4` and has no project anywhere in
  `cmakebuild/QOR.sln` (149 projects scanned). Those four lines are CODE, not comment (`tmp/_tmp_comment_state.ps1`; the
  file is 1503 comment-only lines of 2637).
- **U15 - the first TU with a call site for every wrapper, and the sharpest illustration of the item.** At wrapper level
  16/16 are called; one level up, the twelve `Cursor` methods that make those calls (`Show`, `Clip`, `GetClip`,
  `GetCurrent`, `GetInfo`, `GetPosition`, `GetPhysicalPosition`, `SetPosition`, `SetPhysicalPosition`, `SetAsCurrent`,
  `SetAsSystemCursor`, `Clone`) have no call site anywhere in `src`/`examples`/`test`, so fifteen wrappers are linked and
  never executed. Only `LoadCursorT` is live end to end: `examples/experimental/osspecific/windows/mainwindow/mainwindow.cpp:339`
  (a project that *is* in `QOR.sln`) -> `ui/gui/factories/windowfactory.cpp:90`
  (`windowClass->SetCursor(Cursor(IDC_ARROW))`) -> `Cursor::Cursor(const wchar_t* id)` (`gdiobjects/cursor.cpp:49-53`,
  `m_owning = false` - correct for a shared system cursor) -> `User32::LoadCursorT` -> `LoadCursorW`. That path is also
  what makes the U20 findings observable rather than theoretical.
- **Open items this session contributes to (recorded, not acted on):**
  - U1 - 16 more call sites, 0 with `Win32ErrorHandler _;` (0 of 38 TUs unchanged).
  - U2 - 0 `CheckReturn`, and this TU shows why the item is a behaviour decision rather than a typo: `CheckNonZero` on
    `ShowCursor` would be wrong by construction (a count), `CheckPointer` on the three allocating wrappers would turn a
    legitimate NULL-on-failure into an issue, and `SetCursor` returns NULL legitimately to mean "no previous cursor".
  - U3, U10, U11, U12, U13 - none. U4 - none: all sixteen members are declared `static` (`user32.h:382-397`) and defined.
  - U5 - 16 more process-lifetime-cached bindings; the probe above demonstrates the unmapped-module case.
  - U6 - 16 more `Library::Call` sites whose `fPtr` is default `__cdecl` against `__stdcall` exports.
  - U7 - applies to all sixteen, worst for the four handle-returning wrappers, where the swallowed default return
    (`nullptr`) is the same value the API legitimately returns for "no cursor"/"previous cursor was NULL".
  - U8 - untested; only the `LoadCursorT` path could be exercised without inventing a new caller (through
    `WindowFactory::AddWindowClass`).
  - U14 - (b) 16/16 sites state the full SDK argument list, so nothing to fix; (a) this is the first file with an
    *internal* spacing split - 15 `Call< BOOL, LPRECT >` against 1 `Call<BOOL, const RECT*>` (`cursor.cpp:57`) - left
    alone under the no-churn rule and recorded as the folder-wide pass's per-file deviation; (c) `GetCursor(void)` already
    follows the `(void)` norm.
  - U16 - **3 of 16** pair with guarded SDK declarations: `GetPhysicalCursorPos` (`:9439`) and `SetPhysicalCursorPos`
    (`:9406`) inside `#if(WINVER >= 0x0600)` (`:9435`, `:9402`); `GetCursorInfo` (`:14661`) and the
    `CURSORINFO`/`PCURSORINFO` types in its signature (`:14644-14650`) inside `#if(WINVER >= 0x0500)` (`:13594`) with
    `#ifndef NOWINABLE` (`:13716`) between. The other thirteen sit under nothing but `#ifndef NOUSER` plus a
    `WINAPI_FAMILY_PARTITION` (Desktop; Desktop|Games; Desktop|PC-App for `ClipCursor`). Guard stacks printed by
    `tmp/_tmp_sdk_guard_scan.ps1` are kept in `tmp/_tmp_cursor_sdk.txt`. Reverse half: nothing - the seven cursor exports
    not wrapped here are declared nowhere in `user32.h`, so no consumer-visible declaration outranks the SDK.
  - U17 - the mirror case `CopyCursorT` (`:383`) confirmed as *binding-correct, name-indirect*; census unchanged.
  - U18 - none: no return-type contradiction in this section (`ShowCursor`'s `int` is right; the loss is in the caller,
    which is U20).
  - U19, U20 - see the two entries above; U20 opened by this session.

- **Section coverage.** "Cursor functions" (`user32.h:381-397`) is exactly these sixteen. Not wrapped and not declared
  anywhere in `user32.h`: `GetCursorFrameInfo` (140), `SetCursorContents` (32D), `ShowSystemCursor` (399) - all three
  RESOLVED by the live probe - plus `SetThreadCursorCreationScaling` (36D), `GetPointerCursorId` (194),
  `GetPointerDeviceCursors` (196), `MITGetCursorUpdateHandle` (274). `SetSystemCursor`/`ShowSystemCursor` remain supported
  exports although Microsoft documents both as unsupported from Windows 8 up (a documentation claim, not re-measured).
- **Conventions checked and clean:** licence header, include order with the Windows headers last, `qor_pp_fcontext` at all
  16 sites, lowercase `user32` module token, section comment `//Cursor functions` in house style, no local `#undef`,
  4-space indent, no tabs, definitions in header order, all sixteen declaration/definition pairs present in one TU.
- **Fixed this session:** the TU comment block only (D13 binding rationale with ordinals and RVAs, the
  `CopyCursor`-is-a-macro note, the `HCURSOR`-is-`HICON` note, the shared-RVA finding, per-wrapper handle ownership, the
  `ShowCursor` count-not-failure contract, the guard exposure and the list of unwrapped cursor exports). No header change,
  no rename, no binding change.
- **Verified at exit:** `cursor.cpp` compiles standalone at `/W4` exit 0; the whole library compiles **38/38 at `/W4`,
  exit 0, no diagnostics** (`tmp/_tmp_cl_all.log`, re-run after the edit). No published surface moved, so no caller was
  affected.
- **Instruments this session leaves behind:** `tmp/_tmp_cursor_probe.cpp` + `tmp/_tmp_run_cursor_probe.bat` (the
  `GetModuleHandleW`/`GetProcAddress` probe for this family, including the negative controls),
  `tmp/_tmp_cursor_sdk_scan.ps1` -> `tmp/_tmp_cursor_sdk.txt` (SDK signature + guard stack per cursor declaration),
  `tmp/_tmp_recursion_probe.cpp` + `tmp/_tmp_recursion_run.bat` (proves a wrapper cannot call its own SDK name in this
  class - C4717), `tmp/_tmp_cursor_bypass_s9.txt` (the U19 split) and the **MEMBER** class added to
  `tmp/_tmp_bypass_scan.ps1`, which every later session's U19 count now benefits from.

## desktop.cpp - session 10 (30 Oct 2026)

- **Scope:** 18 wrappers, the "Window Station and Desktop functions" section (`user32.h:399-417`). 18 declarations
  <-> 18 definitions, header order preserved; declaration/definition agreement (return type, parameter types, parameter
  **names**) checked mechanically with `tmp/_tmp_desktop_sigcmp.ps1`, which before the fixes reported exactly the 2
  divergences in items 1-2 below and after them reports **18/18 ok** (its one `HEADER LINE NOT PARSED 398` line is the
  section comment, not a defect). No hidden signature drift beyond those two.
- **Bindings (method steps 3 and 3b) - all 18 correct as written, 0 defects:** 9 SDK A/W families bound
  `qor_pp_useswinapiAW` (CreateDesktop, CreateDesktopEx, OpenDesktop, EnumDesktops, CreateWindowStation,
  OpenWindowStation, EnumWindowStations, GetUserObjectInformation, SetUserObjectInformation) and 9 single exports bound
  bare `qor_pp_useswinapi` (CloseDesktop, CloseWindowStation, GetProcessWindowStation, GetThreadDesktop, OpenInputDesktop,
  SetProcessWindowStation, SetThreadDesktop, SwitchDesktop, EnumDesktopWindows) - already D13-conformant before the
  session. `cl /EP` (`tmp/_tmp_pp_desktop.bat` -> `tmp/_tmp_desktop_pp.txt`) resolves the 18 binding literals; all 18 names are in
  `tmp/_tmp_user32_exports.txt`; the live probe `tmp/_tmp_desktop_probe.cpp` + `tmp/_tmp_run_desktop_probe.bat` resolves
  all 18 through the library's own `GetModuleHandleW`/`GetProcAddress` path and gets non-null handles back from
  `GetProcessWindowStation` and `GetThreadDesktop`, which also proves the section is usable rather than merely loadable.
  `_tmp_wrong_stem_scan.ps1` re-run after the edits: 38 files, 426 `User32::` definitions, 428 bindings, **4 folder-wide
  flags, none in this TU** (unchanged set: `mouse.cpp:82` live P1, `cursor.cpp` `CopyCursorT`->`CopyIcon` = P3,
  2 x `resource.cpp` = U11).
- **Fixed (4 consistency defects, no behaviour change):**
  1. `OpenWindowStationT` parameter type `LPTSTR` -> `LPCTSTR`, in the declaration (`user32.h:412`) and the definition
     in the same pass. The SDK takes `LPCSTR`/`LPCWSTR` and every other `T`-suffix wrapper in this section spells the
     parameter `LPCTSTR`, so the divergence was in this wrapper, not in the convention. The member has no in-tree caller
     (task T2), so no caller can observe the change.
  2. `CreateDesktopT`'s definition parameter `pDevMode` -> `pDevmode`, matching its declaration at `user32.h:402`.
     Cosmetic, but parameter-name divergence is exactly what the `sigcmp` script exists to catch.
  3. U14(b): `GetThreadDesktop` `Call< HDESK >(pFunc, dwThreadId )` -> `Call< HDESK, DWORD >(pFunc, dwThreadId)`
     (stray space before `)` removed in the same edit).
  4. U14(b): `OpenWindowStationT` `Call< HWINSTA >(pFunc, lpszWinSta, fInherit, dwDesiredAccess)` ->
     `Call< HWINSTA, LPCTSTR, BOOL, ACCESS_MASK >(pFunc, ...)`. The folder's worst deducing site - three argument types
     were being inferred from the wrapper's own signature.
  5. Documentation: a 28-line TU comment block (`desktop.cpp:16-45`) in the house style - per-binding D13 rationale with
     the `winuser.h` line ranges, the `HDESK`/`HWINSTA` release contract (released by `CloseDesktop`/`CloseWindowStation`,
     **not** `Kernel32::CloseHandle`, and returned raw, so dropping one leaks the object), the thread/process rules
     (`SetThreadDesktop` fails once the thread has a window or a hook on that desktop; `SwitchDesktop` is per-process), the
     always-`W` consequence for the `DESKTOPENUMPROC`/`WINSTAENUMPROC` callback name, section coverage, and U1's 0/18
     `Win32ErrorHandler` note.
- **Checked and deliberately *not* changed:** `GetUserObjectInformationT` / `SetUserObjectInformationT` keep the folder's
  `T` suffix (correct for A/W wrappers; the U17 list stays a folder-wide decision); `EnumDesktopWindows` has **no** `T`
  suffix because it is a genuine single export with no A/W family - the mirror image of the U17 pattern, correct as it
  stands; `CreateDesktopExT`'s `ulHeapSize`/`pvoid` parameters match the SDK's `CreateDesktopExW`; the section's only
  zero-argument member (`GetProcessWindowStation`) already spelled `(void)` in both declaration and definition, so U14(c)
  contributes nothing here.
- **Open items this session feeds:** U1 - 18 sites, 0 `Win32ErrorHandler` (settled policy, unchanged); U2 - 0 return
  checks, and this section shows the shape of that decision: 4 handle-returning wrappers would take `CheckNonZeroHandle`,
  8 `BOOL` wrappers `CheckReturn<BOOL>`, and `GetUserObjectInformationT` reports "buffer too small" *only* through its
  `lpnLengthNeeded` out-parameter, which the wrapper neither checks nor forwards; U5 applies to all 18 bindings; U15
  **+18**, the largest dead-surface block of the review (task T2); U19 - 0 LAYER and 0 BYPASS sites; U16 - every SDK
  declaration this section pairs with sits inside `WINAPI_PARTITION_DESKTOP` + `!NODESKTOP` / `!NOWINDOWSTATION`
  (`winuser.h:1391-1661`), so a non-DESKTOP `WINAPI_FAMILY` build loses the whole section while `user32.h:399-417` still
  declares it - recorded, not acted on, per the session-5 deferral of the version-pin decision.
- **Verified at exit:** `desktop.cpp` compiles standalone at `/W4`, exit 0; the whole library compiles **38/38 at `/W4`,
  exit 0** (`tmp/_tmp_compile_all.ps1`, re-run after the edits). U14 census re-run: folder **423 explicit / 6 deducing**
  (was 420/8), spacing 250 tight / 179 spaced; `desktop.cpp` 18 explicit / 0 deducing, all 18 spaced.
- **Instruments this session leaves behind:** `tmp/_tmp_desktop_sigcmp.ps1` (declaration <-> definition comparison
  including parameter names - reusable in every later session), `tmp/_tmp_pp_desktop.bat` (`cl /EP` of one TU),
  `tmp/_tmp_desktop_bindings.ps1` (binding-vs-member listing for one TU), `tmp/_tmp_desktop_probe.cpp` +
  `tmp/_tmp_run_desktop_probe.bat` (live `GetProcAddress` probe with non-null-handle positive controls),
  `tmp/_tmp_desktop_sdk_26100.txt` (the `winuser.h` declarations with their guard stacks) - but note
  `tmp/_tmp_desktop_sdk_10240.txt` came out **0 bytes**, so this session could not compare the desktop declarations
  against the older 10.0.10240 SDK: U16's claim for this section rests on the 26100 headers alone, and the older-SDK
  comparison is still open for whichever session takes up the U16 pin decision.

## devicecontext.cpp - session 11 (30 Oct 2026)

- **Scope:** 8 wrappers, the `//DC functions` section (`user32.h:325-333`). 8 declarations <-> 8 definitions, header order
  preserved. Declaration/definition agreement (return type, parameter types, parameter **names**) plus the `Call<...>`
  arity/type check run mechanically with `tmp/_tmp_sigcmp.ps1` - generalised this session from the session-10 one-off
  script, now `-Tu <file> -First <line> -Last <line>`; it reports **8/8 `decl==def ok` and 8/8 `Call ... ok`**, with no
  name-only divergence either. No signature drift at all in this section, which is why the session's code edit count is 1.
- **Bindings (method steps 3 and 3b) - all 8 correct as written, 0 defects:** 5 SDK A/W families bound
  `qor_pp_useswinapiAW` (ChangeDisplaySettings, ChangeDisplaySettingsEx, EnumDisplayDevices, EnumDisplaySettings,
  EnumDisplaySettingsEx) and 3 single exports bound bare (`GetDC`, `GetDCEx`, `ReleaseDC`) - already D13-conformant.
  `cl /EP` (`tmp/_tmp_pp_dc.bat` -> `tmp/_tmp_dc_pp.txt`) resolves the 8 binding literals at lines 453278-453327 to
  `ChangeDisplaySettingsW`, `ChangeDisplaySettingsExW`, `EnumDisplayDevicesW`, `EnumDisplaySettingsW`,
  `EnumDisplaySettingsExW`, `GetDC`, `GetDCEx`, `ReleaseDC`. Export table (`tmp/_tmp_user32_exports.txt` lines 1542-1545,
  1759-1765, 1829-1830, 2270): the bound forms are ordinals 29, 28, FD, 102, 101, 143, 144, 2FF; the A halves are
  26, 27, FC, FF, 100 and are exported but unreachable through this surface. `_tmp_wrong_stem_scan.ps1` re-run after the
  edits: 38 files, 426 `User32::` definitions, 428 bindings, **4 folder-wide flags, none in this TU** (unchanged set).
  None of this TU's stems is in the `user32.h:9-17` `#undef` list, so the G2/P1 hazard does not apply here.
- **Fixed (1 consistency nit + documentation):**
  1. U14 spacing: `GetDC`'s `Call< HDC, HWND >(pFunc, hWnd )` -> `(pFunc, hWnd)`. This TU's only such site; 8/8 sites
     state the full SDK argument list (0 deducing).
  2. A 36-line TU comment block (`devicecontext.cpp:15-50`) in the house style: per-binding D13 rationale with the
     `winuser.h` line ranges and ordinals, the **two opposite zero-return conventions inside one section**, the
     `_Reserved_`/`_In_opt_`/`_Inout_` parameter contracts (incl. "the caller must set `dmSize`/`dmDriverExtra` first"),
     the `GetDC`/`GetDCEx` -> `ReleaseDC` pairing (never `Kernel32::CloseHandle`) and the `hWnd`/`hDC` match requirement,
     section coverage with the `GetWindowDC`/`WindowFromDC` cross-reference, and the U16 guard picture.
- **Checked and deliberately *not* changed:** all five A/W wrappers already carry the `T` suffix, so **U17 contributes
  nothing from this TU** (unlike the next one); `GetDC`/`GetDCEx`/`ReleaseDC` correctly have no suffix (single exports);
  `ChangeDisplaySettingsExT`'s `hwnd` stays a free `HWND` even though the SDK marks it `_Reserved_`
  (`winuser.h:13101`, `:13110`) - tightening it to a forced `NULL` would change an exported signature; `lParam` is
  `LPVOID` matching the SDK. No return checks added anywhere (U2 policy) - and this TU is the strongest argument yet that
  that is a behaviour decision, not an oversight (see U2 below).
- **Open items this session feeds:**
  - U1 - 8 call sites, 0 `Win32ErrorHandler` (settled policy).
  - U2 - 0 return checks, plus the sharpest case study of the review: `ChangeDisplaySettingsT`/`ChangeDisplaySettingsExT`
    return `DISP_CHANGE_*` codes where **zero is success** (`DISP_CHANGE_SUCCESSFUL == 0`, positive = reboot needed,
    negative = failure), while the three `EnumDisplay*` wrappers and `ReleaseDC` use the convention where **zero is
    failure**. One `CheckReturn<>` policy cannot cover this one section, and a `CheckNonZero` over the
    ChangeDisplaySettings pair would flag every success as a failure.
  - U5 (binding cached for the process lifetime) applies to all 8. U7 is worse here than anywhere else in the folder:
    the SEH default `ret _{}` is `0`, which for these two wrappers **is `DISP_CHANGE_SUCCESSFUL`** - a fault inside a
    display-mode change would read as a successful one. Recorded, not fixed.
  - U15 - **+1 dead wrapper**: `ChangeDisplaySettingsT` has **no call site anywhere** in `src`/`examples`/`test`.
    `ChangeDisplaySettingsExT`'s only wrapper-level caller is `DisplayDevices::ChangeSettings`
    (`ui/gdi/displaydevices.cpp:70`), and `DisplayDevices` has no user outside its own header. The other five are called
    from built `ui/gui`/`ui/gdi` code (see U19).
  - U19 - **16 LAYER / 12 BYPASS, the largest split of the review** (full entry under U19). Instrument change: the scan
    now matches `::stem(` anywhere in a line, not only at line start - the first version missed
    `if (::ChangeDisplaySettingsExW(...` (`ui/gl/monitorimpl.cpp:138`) and `const HDC dc = ::GetDC(NULL);` (`:263`),
    giving 10 where the fixed instrument gives 12. Bare-stem runs under-count far worse (8 / 4): see step 5b.
  - U16 - 4 of the 13 SDK declarations this section pairs with are version-guarded (`WINVER >= 0x0500`), and the TU adds
    a **new U16 sub-case**: the whole display block sits inside `_WINGDI_` / `!NOGDI` / `!NOSYSPARAMSINFO` and five
    wrapper signatures name `wingdi.h` types, so a `NOGDI` consumer does not merely lose prototypes, it gets an
    ill-formed `user32.h:326-330`. None of those macros is defined anywhere in `cmake/` or in any `CMakeLists.txt`
    (measured this session).
  - U21 (new) - the section name over-promises, and the SDK's DC neighbourhood is split across `devicecontext.cpp` and
    `painting.cpp`.
  - U22 (new) - the `DeviceContext` release path this layer feeds calls `ReleaseDC` with a NULL `HWND`, so the DC is not
    released. Unlike U20 this one **is on a built, reachable path** (`mainwindow` is a project in
    `cmakebuild/QOR.sln:902`, verified this session).
- **Verified at exit:** `devicecontext.cpp` compiles standalone at `/W4`, exit 0; the whole library compiles **38/38 at
  `/W4`, exit 0**, no diagnostics (`tmp/_tmp_compile_all.ps1` -> `tmp/_tmp_cl_all.log`: 38 TU echo lines, the single
  `error|warning` grep hit being the filename `errorhandling.cpp`; run summary in `tmp/_tmp_cl_all_s11b.log`). U14 census
  re-run: folder **423 explicit / 6 deducing**, spacing 179 spaced / 250 tight - unchanged, since this TU's one fix is a
  spacing nit inside the already-explicit 423.
- **Instruments this session leaves behind:** `tmp/_tmp_sigcmp.ps1` - now the reusable declaration<->definition +
  `Call<>` checker for any section (`-Tu`, `-First`, `-Last`). Two instrument hazards were found and fixed here, and both
  produced false findings before they were caught: (a) the parameter **must not** be named `$Src`, because PowerShell
  names are case-insensitive so `$Src` aliases the `$src` file-content array - the first run reported "NO DEFINITION" for
  all 8 wrappers; (b) `cl /EP` output must go to a per-TU file, since the shared `_tmp_pp_one.bat` writes
  `_tmp_pp_out.txt` and reading it before `cl` finished returned a stale, unrelated TU's preprocessed text. Also
  `tmp/_tmp_dc_bypass.txt` (the under-counted bare-stem run, kept as the worked example) and
  `tmp/_tmp_dc_bypass2.txt` (the corrected suffix-variant run).


## devicemanagement.cpp - session 12 (30 Oct 2026)

- **Scope:** the "Device Management functions" section, `user32.h:681-683` - 2 declarations (`RegisterDeviceNotification`
  `:682`, `UnregisterDeviceNotification` `:683`), 2 definitions, both in the header's order. Second-smallest TU of the
  review (30 lines before this session), and the first one whose whole SDK block (`winuser.h:3939-3977`) is covered 2 of 2
  while the block **after** it is not covered at all (T3).
- **0 binding defects - and the first TU where both binding spellings were already right for measurable reasons.**
  `qor_pp_useswinapiAW(user32, RegisterDeviceNotification)` -> `/EP` gives `"RegisterDeviceNotificationW"`; the bare
  `qor_pp_useswinapi(user32, UnregisterDeviceNotification)` is correct because `UnregisterDeviceNotificationA`/`W` are both
  **MISSING** in the live probe. The export table alone cannot show that - it lists only what exists, and the absence of the
  `A`/`W` spellings is what makes bare right, which is why the probe (not the table) is the authority here.
- **G2 live for this stem, harmless in practice.** `RegisterDeviceNotification` is `#undef`'d at `user32.h:9` and bare
  `"RegisterDeviceNotification"` is MISSING (probe), so had the binding been bare it would have thrown
  `"Missing library function."` on every call; it is not bare, and the AW macro token-pastes `W` before the SDK macro could
  ever be consulted, so the `#undef` is inert for this TU. Recorded as the first G2 case where the hazard is confirmed to be
  genuinely live rather than theoretical.
- **2 defects fixed, both parameter naming.** `notificationFilter` -> `NotificationFilter` and `handle` -> `Handle` in the
  definitions (`:73`/`:77`, `:80`/`:84` of the file as it now stands, comment block included), matching both the declarations and the SDK's names.
  **Convention for line references in this file:** `user32.h` line numbers are stable (the header was not touched this session), but
  in-TU line numbers move whenever a session's comment block grows - so cross-references from the U-items to a TU cite the
  number as of that session, and `user32.h` numbers in preference to TU numbers wherever both identify the same thing.
  These were the **only two such
  divergences in the entire folder**: `tmp/_tmp_name_divergence_scan.ps1` compares parameter names case-sensitively across
  every `User32::` definition and measured 2 in 426 definitions before the fix, 0 after.
- **Instrument blind spot corrected.** `_tmp_sigcmp.ps1` had used PowerShell `-ne` for names, which is case-insensitive, so
  it reported `decl==def ok` for exactly these two sites. Switched to `-cne` and re-ran the previously reviewed TUs: no
  earlier verdict changes (`devicecontext.cpp` 8/8, `desktop.cpp` 18/18 including its session-10 `pDevmode` fix,
  `combobox.cpp` 3/3).
- **Convention counts:** U13 0 (both `static`); U14(b) 0 (2/2 explicit `Call<>`); U17 1 instance of 2 (`:682` on the
  11-member list, **no rename** per the settled policy) - sharpened here because the A/W choice is invisible in the
  signature: the filter is a bare `LPVOID` and the bound half decides how the payload's trailing `TCHAR` name field is read;
  U15 2/2 (no caller anywhere); U19 0 LAYER / 1 BYPASS (`ui/gl/helperwindow.cpp:85`, live code, correct struct setup, but
  the file is in no target and `gl` is commented out of `ui/CMakeLists.txt:4`); U16 2/2 with the hardest dependency yet -
  `HDEVNOTIFY` is typedef'd inside the same `WINVER >= 0x0500` + Desktop-partition guards as the prototypes, so a consumer
  pinning a lower `WINVER` gets `user32.h:682-683` **ill-formed**, not merely unprototyped; U1/U2 unchanged (no handler, no
  `CheckReturn`).
- **New deferred task T3** (see the Deferred tasks section): the Power Management pair at `user32.h:686-687` is **not**
  `static`, is declared, and is exported for real (2F1/3CB, both RESOLVED in the probe) - yet it is defined in no TU, in a
  folder that has no file for it. U4's cleanest case.
- **Verified at exit:** `devicemanagement.cpp` compiles standalone at `/W4` exit 0; whole library **38/38** exit 0
  (`tmp/_tmp_cl_all_s12.log`, 38 `.cpp` echoed, 38 `.obj` refreshed); `/EP` re-run post-edit confirms both binding literals
  (`tmp/_tmp_devman_pp.txt:453294`, `:453301`); the name-divergence scan reports 0 for this TU and 0 folder-wide.
- **Instruments this session leaves behind:** `tmp/_tmp_name_divergence_scan.ps1` (folder-wide case-sensitive parameter-name
  comparison - run it with no `-Tu` for the folder sweep, expect 0), the corrected `_tmp_sigcmp.ps1` (`-cne`), the extended
  `tmp/_tmp_name_resolve_probe.cpp` (device-notification + power-setting spellings) and `tmp/_tmp_pp_devman.bat`.
- **Bookkeeping corrected in this file:** the "Remaining queue" line listed 25 names under a "26 TUs" count because
  **`text.cpp` had been omitted** from the queue; it is now listed, and the counts reconcile (13 done + 25 remaining = the
  38 sources in `add_library`). The stray `| 2b |` row that had been stranded below the Deferred-tasks block is back inside
  the session log.

## dialog.cpp - session 13 (30 Oct 2026)

- **Scope:** the "Dialog functions" section, `user32.h:469-493` - **22 declarations** (`:470-492`, with the `//NOTE:` at
  `:488`), 22 definitions in the header's order, all `static` (U13: 0 instances). The largest section reviewed so far. The
  TU was 171 lines before this session and 192 after (the 20-line section comment now at `:16-35` records the binding
  evidence and the contracts the wrappers pass through).
- **1 real run-time failure fixed - P1's third instance.** The binding that was `qor_pp_useswinapi(user32, GetDlgItemTextT)`
  (`:89` pre-fix; now `:110`) stringized the **member** name, which is not an export. `/EP` pre-fix literal
  `"GetDlgItemTextT"` -> live probe **MISSING**; post-fix `qor_pp_useswinapiAW(user32, GetDlgItemText)` literal
  `"GetDlgItemTextW"` -> **RESOLVED** (ordinal 151; the `A` half 150 is exported too but is not what a `LPTSTR` signature
  wants here). Same class as the two `combobox.cpp` sites. **Latent rather than live:** `GetDlgItemTextT` has no call site
  anywhere in `src`/`examples`/`test` (U15), so unlike `coordtransform.cpp` (session 8) no shipped path was broken.
- **1 convention conversion (P2 7 -> 6).** `MessageBox` -> `qor_pp_useswinapiAW(user32, MessageBox)` (`:145`).
  **No behaviour change**: the pre-fix `/EP` literal was already `"MessageBoxW"`, because the SDK's
  `#define MessageBox MessageBoxW` (`winuser.h:9244`) rewrites the stem before `_ATXT` stringizes it (G1). Converted
  because P2 tracks bindings that work *only* through that macro; G3's false-positive list named `MessageBox` for exactly
  this reason.
- **13 `AW` + 9 bare, and the 9 bare ones are right for a reason the export table alone cannot show.** AW bindings:
  `:40`, `:47`, `:54`, `:61`, `:68`, `:110`, `:131`, `:145`, `:152`, `:159`, `:167`, `:174`, `:188`. Bare bindings:
  `EndDialog` `:75`, `GetDialogBaseUnits` `:82`, `GetDlgCtrlID` `:89`, `GetDlgItem` `:96`, `GetDlgItemInt` `:103`,
  `GetNextDlgGroupItem` `:117`, `GetNextDlgTabItem` `:124`, `MapDialogRect` `:138`, `SetDlgItemInt` `:181` - all nine have a
  **single** SDK spelling (`winuser.h:5147`, `:5293`, `:5287`, `:5154`, `:5170`, `:5271`, `:5279`, `:11573`, `:5161`), no
  `A`/`W` variant in the export table, and all nine probe **RESOLVED** exactly as written. `DefDlgProcW` (A0) and
  `DefDlgProcA` (9F) are exported as **forwarders to `NTDLL.NtdllDialogWndProc_W`/`_A`**, so the binding resolves through
  `user32.dll` while the code lives in `ntdll` - the first forwarders seen in this review apart from the `win32u` route in
  session 7. The trap in this section is `IsDialogMessage`: its bare stem **is** exported (231) but at the same RVA as
  `IsDialogMessageA`, so binding the stem would hand the ANSI half a `LPTSTR`; the `AW` macro at `:131` is what makes the
  binding match the signature.
- **`MessageBoxTimeout` is the review's second SDK-invisible entry point** (after `ConsoleControl`, session 7): zero hits for
  the name across the installed `Windows Kits\10\Include\10.0.26100.0` tree, yet `MessageBoxTimeoutW` (28A) and
  `MessageBoxTimeoutA` (289) are exported and probe **RESOLVED** while the bare stem is **MISSING**. So the `AW` binding at
  `:167` is the only spelling that can bind, and `user32.h:489`'s six-parameter list has **no SDK prototype to check it
  against** - the export table plus the live probe are the entire authority (U16's mirror case: no version guard to worry
  about because there is no declaration at all).
- **Section coverage (U21's shape again - the header mirrors only part of the SDK's dialog block).** The SDK block is
  `winuser.h:4912-5385` (`#ifndef NOCTLMGR`) plus the message-loop half at `:11233-12072`. This section mirrors 22 of it and
  leaves out: `CheckDlgButton` (`:5223`), `CheckRadioButton` (`:5231`), `IsDlgButtonChecked` (`:5240`) - which `user32.h:372-374`
  declares as **non-static members that no TU defines**, three of U4's 97 - and the DPI-behaviour quartet
  `SetDialogControlDpiChangeBehavior` (`:5340`), `GetDialogControlDpiChangeBehavior` (`:5347`), `SetDialogDpiChangeBehavior`
  (`:5363`), `GetDialogDpiChangeBehavior` (`:5370`), all four exported and probe **RESOLVED** but declared nowhere in this
  library. `DlgDirList`/`DlgDirSelectEx` (`:11580` and following) belong to the same SDK family but are declared in the
  header's list-box section (`:546-547`, non-static, defined by no TU - U4 again); their combo-box counterparts are the
  session-6 pair.
- **Correctness / UB pass: nothing found.** 22/22 declarations match their definitions and 22/22 `Library::Call<>`
  template-argument lists match the definition's parameter types (`_tmp_sigcmp.ps1`), 0 parameter-name divergences for this TU
  and 0 folder-wide (`_tmp_name_divergence_scan.ps1`). The three wrappers that write through caller pointers match the SDK's
  SAL exactly - `GetDlgItemTextT` forwards `LPTSTR`/`int` under `_Out_writes_(cchMax)` + `_Ret_range_(0, cchMax)` with no
  buffer logic of its own, `GetDlgItemInt` the `_Out_opt_ BOOL*`, `MapDialogRect` the `_Inout_ LPRECT` - and the four
  callback-taking wrappers (`CreateDialog*`, `DialogBox*`) hand the caller's `::DLGPROC` straight to `user32` and never
  synthesize one, so the CALLBACK-convention hazard of the enum-callback wrappers does not arise in this TU.
- **3 sites fixed for spelling consistency.** `Call<..., DLGPROC, ...>` -> `Call<..., ::DLGPROC, ...>` at `:41`, `:48`, `:69`;
  the declarations (`user32.h:470`, `:471`, `:474`) and the definitions already spelled it `::DLGPROC`, and `:62`
  (`DialogBoxIndirectParamT`) already did too, so this TU is now internally uniform - no type change, no surface change.
  **Instrument note for every later session:** before this fix those three were exactly the rows `_tmp_sigcmp.ps1` printed as
  `CALL TYPE MISMATCH ... sdk '::DLGPROC' vs call 'DLGPROC'`. That is a string comparison on a spelling, not a type error -
  the review's only sigcmp false-positive class, so a MISMATCH row must be read before it is acted on. Re-run after the fix:
  22/22 clean.
- **Convention counts:** U13 0 of 22 (all `static`, all defined `User32::`-qualified); U14(b) 0 deducing (22/22 explicit,
  spaced style, 0 spacing nits); U17 **1 of this section's 13 A/W families** (`MessageBoxTimeout` `user32.h:489`, no rename per
  the settled policy); U15 **16 of 22** instances; U19 14 LAYER / 24 BYPASS (see the U19 entry); U16 1 version-guarded pair +
  3 type-level opt-out dependencies (see the U16 entry); U1/U2 22 sites, 0 `Win32ErrorHandler`, 0 return checks (settled
  policy) - and worth recording that **17 of the 22 could not use the `CheckReturn` pattern anyway**: only `EndDialog`,
  `IsDialogMessageT`, `MapDialogRect`, `SetDlgItemInt`, `SetDlgItemTextT` return `BOOL`; the rest return handles, `INT_PTR`
  dialog results, `LRESULT`, `UINT` text lengths, a packed `LONG` or a button id where 0 is both a result and a failure.
- **Verified at exit:** `dialog.cpp` compiles standalone at `/W4` **exit 0** (re-run after every edit, last run 30/10/2026
  23:59+), and the whole library compiled **38/38 exit 0** earlier in the session (`tmp/_tmp_cl_all_s13.log`:
  `exit=0 loglines=43`, 38 `.cpp` echoed, 44 objects in `tmp/obj`); post-edit `/EP` re-run yields the 22 binding literals in
  definition order (13 `*W` + 9 bare) and all 22 are **RESOLVED** in the extended probe; `git diff --numstat` for this session is
  `dialog.cpp` **+26/-5** (20 comment lines + 1 blank added; 5 lines changed - 3 `::DLGPROC` spellings + 2 binding macros) with
  **`user32.h` untouched**. (`devicemanagement.cpp` is still in
  `git status` from session 12 - uncommitted at the time of writing, so a working-tree diff of this folder shows both files.)
- **Instruments this session leaves behind:** `tmp/_tmp_pp_dialog.bat`; `tmp/_tmp_binding_names.ps1` (reads the binding
  literals out of a `/EP` dump in definition order - the only practical way to audit a 453k-line preprocessed file);
  `tmp/_tmp_decl_no_def_scan.ps1` / `tmp/_tmp_decl_no_def_repo_scan.ps1` (the declared-but-never-defined census, 64 names into
  `tmp/_tmp_decl_no_def_s13.txt`, reconciled with U4's 97 figure in the U4 entry); `tmp/_tmp_guard_stack_scan.ps1` (the
  generalised U16 guard-stack scan, `-Pattern <regex>` over `winuser.h`, replacing the hard-coded
  `_tmp_sdk_guard_scan.ps1`); and `tmp/_tmp_name_resolve_probe.cpp` extended with 24 dialog spellings plus the 7 exported
  dialog entry points this header does not declare.

## errorhandling.cpp - session 14 (31 Oct 2026)

- **Scope:** the "Error Handling functions" section, `user32.h:613-617` - **4 declarations** (`:614-617`), 4 definitions in
  the header's order, all `static`, all defined `User32::`-qualified. No U4 member (the whole section lives in this TU),
  no U11, no U13. The TU was 46 lines before this session and 92 after (the 46-line section comment now at `:16-61`);
  `user32.h` untouched. In `add_library` at `user/CMakeLists.txt:6`.
- **0 binding / correctness / UB defects, and the bare spelling is provably right for all four.** `cl /EP`
  (`tmp/_tmp_pp_errh.bat` -> `tmp/_tmp_errh_pp.txt`) yields the four literals `"FlashWindow"`, `"FlashWindowEx"`,
  `"MessageBeep"`, `"SetLastErrorEx"` (`:453288`, `:453295`, `:453302`, `:453310` of the 31 Oct run, in definition order);
  the export table gives 115, 116, 283, 342; and the extended live probe (`tmp/_tmp_name_resolve_probe.cpp`, 16 spellings
  added this session) reports **RESOLVED** for all four stems and **MISSING** for every `*A`/`*W` candidate -
  `FlashWindowA/W`, `FlashWindowExA/W`, `MessageBeepA/W`, `SetLastErrorExA/W` all absent. So this is the D13 case in its
  purest form: `qor_pp_useswinapi` is correct and `qor_pp_useswinapiAW` would break all four bindings. None of the four
  stems is among the nine `user32.h:9-17` `#undef`s, so G1 has nothing to expand and the source spelling is literally the
  `GetProcAddress` argument. `tmp/_tmp_wrong_stem_scan.ps1` scanned 428 bindings across 38 files, flagged 4
  (`cursor.cpp:63`, `mouse.cpp:82`, `resource.cpp:19`, `resource.cpp:26` - all known) and **nothing in this file**.
- **The signature's one non-trivial type was checked rather than assumed:** `FlashWindowEx(::PFLASHWINFO)` at
  `user32.h:615` names the SDK's own typedef (`winuser.h:4694`), and the wrapper is a pure pass-through, so the caller
  hands `user32` a real `FLASHWINFO` and `cbSize` must be `sizeof(FLASHWINFO)`. Unlike U23's `MsgBoxParams` there is no
  QOR-side mirror struct in play in this TU - and none exists at all: this library publishes neither the `FLASHW_*` flags
  (`winuser.h:4702-4707`) nor the `SLE_*` severities (`:13551-13553`).
- **2 nits fixed (both trailing spaces at call sites):** the two-line `Library::Call< BOOL, UINT >(` + `pFunc, uType );`
  pair at `MessageBeep` became one line ending `(pFunc, uType);`, and
  `Library::voidCall< DWORD, DWORD >(pFunc, dwErrCode, dwType )` became `(pFunc, dwErrCode, dwType)`.
  Inner template-argument spacing is left alone: this file is 2 tight / 2 spaced against the folder's 250 tight / 179
  spaced, which is not a norm either way (U14(a)).
- **U18 - no new instance, settled from the SDK.** `SetLastErrorEx` is `VOID` in the SDK
  (`winuser.h:13558-13563`, `WINUSERAPI VOID WINAPI SetLastErrorEx(_In_ DWORD dwErrCode, _In_ DWORD dwType)`), so
  `Library::voidCall< DWORD, DWORD >` matches it exactly and the `void` return in the header is correct. U18 keeps its one
  pair (`window.cpp`'s `void`-returning point converters) and gains nothing.
- **Return-check policy instance (U1/U2, settled):** 4 call sites, 1 `CheckReturn` (`MessageBeep`, `:77-83`), 0
  `Win32ErrorHandler`. That site is **1 of only 4 `CheckReturn` uses in `winqor_api_user`** - the other 3 are
  `processthread.cpp:20`, `:29`, `:45` (measured with a `Get-ChildItem | Select-String` pass over `api_layer`, where
  `kernel`/`shell`/`winsock2` use the pattern heavily). G6's "always live" was re-verified on this TU's `/EP` dump: only the
  checked branch of `returncheck.h:19-23` survives (`typedef CheckReturn< TReturn, TCheck > TType;` at pp line 452867;
  `typedef TReturn TType;` appears nowhere in the 453k-line dump), so every `FALSE` from `MessageBeep` raises a continuable
  issue at run time. `FlashWindow` deliberately has no check - U24 explains why adding one would be a behaviour bug - and
  `FlashWindowEx` (FALSE on failure) is unchecked, which is a U2 policy gap rather than a TU defect.
- **Citation correction applied to G6 in place:** G6 cited the `#if` as `returncheck.h:40`; measured now it is `:19`
  (checked typedef `:20`, `#else` `:21`, bare typedef `:22`, `#endif` `:23`). The finding itself stands unchanged.
- **Instrument results after the edits:** `_tmp_sigcmp.ps1 -Tu errorhandling.cpp -First 614 -Last 617` parses 4
  declarations and prints `decl==def ok` + `ok` for `FlashWindow`, `FlashWindowEx` and `MessageBeep`; the `SetLastErrorEx`
  row prints `(no Library::Call - voidCall or inlineCall?)` by design, so its call site was read out of the `/EP` dump
  instead: `Library::voidCall< DWORD, DWORD >(pFunc, dwErrCode, dwType)` - two `DWORD` template arguments for two `DWORD`
  arguments, matching the SDK. `_tmp_name_divergence_scan.ps1` -> 0 of 4 definitions; the folder-wide parameter-name
  divergence count stays **0 of 426**, a third consecutive TU confirming it.
- **Convention counts:** U13 0 of 4; U14(b) 0 deducing (4/4 explicit); U17 0 (no A/W family in the section, so no `T`
  suffix is due on any of the four); U4 0; U11 0; U15 **3 of 4**; U19 **1 LAYER / 0 INDIRECT / 0 BYPASS / 0 MEMBER**
  (`tmp/_tmp_errh_bypass.txt`); U16 **1 version-guarded of 4**, and it is the type-level sub-case again - `PFLASHWINFO` is
  typedef'd inside the same `#if(WINVER >= 0x0500)` block as the prototype, so `WINVER < 0x0500` makes `user32.h:615`
  ill-formed rather than merely prototype-less (full guard stack in the U16 entry).
- **Liveness:** only `FlashWindow` has a call site at all - `WindowController::Flash`
  (`ui/gui/controllers/windowcontroller.cpp:717`, built at `gui/CMakeLists.txt:27`) - and its only in-tree caller,
  `WindowImpl::RequestAttention()` (`ui/gl/windowimpl.cpp:1833-1836`), belongs to `winqor_gl`, which `ui/CMakeLists.txt:4`
  comments out of the build. That one call site is also **U24**, opened this session: the method turns a
  previous-active-state `BOOL` into a `bool` "did it work" result.
- **Section coverage:** `user32.dll` exports two more error-adjacent entry points that `user32.h` declares nowhere -
  `RegisterErrorReportingDialog` (2E6) and `SetDebugErrorLevel` (32F), both **RESOLVED** in the probe - while
  `GetLastError`/`SetLastError` are **MISSING from `user32.dll`** (probe) and correctly belong to `kernel32`.
  `DdeGetLastError` (8E) is part of the undeclared DDE section (U4).
- **Verified at exit:** `errorhandling.cpp` compiles standalone at `/W4` **exit 0** (re-run after every edit,
  `_tmp_compile_one.ps1`); the whole library compiled **38/38 exit 0** with the edits in place
  (`tmp/_tmp_compile_all.ps1` -> `exit=0 loglines=43`, `errorhandling.cpp` echoed, no diagnostics); the post-edit `/EP`
  re-run still yields the four bare literals in definition order; `user32.h` untouched.
- **Instrument note for every later session (cost one wasted scan): array parameters and `pwsh -File`.** Passing
  `-Stems A,B -Roots 'x','y'` to a script through `pwsh -NoProfile -File script.ps1` does **not** deliver arrays - each
  comma list arrives as one literal string, quotes and all. `_tmp_bypass_scan.ps1` then reports `stems: 1`, and the bogus
  `-Roots` string makes it die at `:12` with the misleading `Get-ChildItem: A parameter cannot be found that matches
  parameter name 'File'` (a path that resolves to a provider with no `-File` dynamic parameter) **while still printing its
  totals line with 0 BYPASS from an empty file list**. Correct forms: rely on the script's default `-Roots`
  (`src`, `examples`, `test`) and pass `-Stems` as a single element, or invoke
  `pwsh -NoProfile -Command "& script.ps1 -Stems @('A','B')"`. A totals line printed alongside a `:12` error is not a clean
  scan - the session's real numbers came from the `-Command` form.
- **Instruments this session leaves behind:** `tmp/_tmp_pp_errh.bat` (+ `tmp/_tmp_errh_pp.txt`,
  `tmp/_tmp_errh_pp_err.txt`), `tmp/_tmp_errh_bypass.txt`, and `tmp/_tmp_name_resolve_probe.cpp` extended with 16
  error-family spellings (4 stems x 3 spellings plus `RegisterErrorReportingDialog`, `SetDebugErrorLevel`, `GetLastError`,
  `SetLastError`).

## filledshapes.cpp - session 15 (31 Oct 2026)

- **Scope:** the "Filled Shapes functions" section, `user32.h:608-611` - **3 declarations** (`:609-611`), 3 definitions in
  the header's order, all `static`, all defined `User32::`-qualified. No U4 member (the section is fully covered here), no
  U11, no U13. The TU was 38 lines before this session and 99 after (the 61-line section comment now at `:16-76`);
  `user32.h` untouched; the only code-side edit is removing one stray blank line between the comment and the first
  definition. In `add_library` at `user/CMakeLists.txt:31`.
- **0 binding / correctness / UB defects, and the bare spelling is provably right for all three.** `cl /EP`
  (`tmp/_tmp_pp_fs.bat` -> `tmp/_tmp_fs_pp.txt`) yields the literals `"FillRect"`, `"FrameRect"`, `"InvertRect"`
  (`:453304`, `:453311`, `:453318` of the post-edit run, in definition order); the export table gives 110, 117, 226
  (`tmp/_tmp_user32_exports.txt:1779`, `:1786`, `:2057`); and the extended live probe reports all three **RESOLVED** with
  `FillRectA/W`, `FrameRectA/W`, `InvertRectA/W` **MISSING**. So `qor_pp_useswinapiAW` would have broken all three
  bindings - this is the D13 case again, and it is worth stating because the three signatures look exactly like the A/W
  families elsewhere in the folder (`Rect`+`HBRUSH` arguments, no `T` suffix, and the SDK does spell the pointer
  `CONST RECT *`). None of the three stems is among the nine `user32.h:9-17` `#undef`s and none has a `stem -> stemW`
  macro in `winuser.h` (`tmp/_tmp_winuser_aw_macros.txt` has no match for any of them), so neither half of G1 can fire and
  the source spelling is literally the `GetProcAddress` argument. `tmp/_tmp_wrong_stem_scan.ps1` flagged **nothing** in
  this file.
- **New U25 from this TU - the `CONST::RECT*` spelling.** `CONST` is the SDK's object-like macro for `const`
  (`shared/minwindef.h:153`), so the header's `CONST::RECT*` and the call sites' `CONST ::RECT*` are one type written two
  ways; the `/EP` dump proves it (`tmp/_tmp_fs_pp.txt:453301`: `int User32::FillRect(HDC hDC, const::RECT* lprc, HBRUSH
  hbr)`). Recorded, not fixed: 17 header lines use `CONST::`, 7 use `const ::`, and the same family reaches
  `rectangle.cpp`, `painting.cpp` and `text.cpp`, so it joins the folder-wide style pass with U14(a). The only real smell
  found inside that family (`user32.h:571`'s `CONST::LPINT`) is U23-shaped and is recorded in the U25 entry for
  `text.cpp`'s session.
- **U18 - no new instance.** The SDK is `int FillRect(...)`, `int FrameRect(...)`, `BOOL InvertRect(...)`
  (`winuser.h:9726-9747`), which is exactly what `user32.h:609-611` publishes, so the `int`/`int`/`BOOL` returns need no
  change and the failure values (0 / 0 / FALSE) are at least representable. U1/U2 instance: 3 call sites, 0
  `Win32ErrorHandler`, 0 `CheckReturn` - per the settled policy, so a failed paint is silent (and per U7 an SEH fault
  inside the callee would look identical to a legitimate 0). The only in-tree code that reads any of the three results is
  `ui/gui/gdiobjects/devicecontext.cpp:85`, which folds `InvertRect`'s `BOOL` into `bool`; the two `int` results are never
  inspected.
- **U16 - this TU adds 0 of 3, and it is the first TU whose guard stack has no version macro at all.** Measured with
  `tmp/_tmp_sdk_guard_scan.ps1` on `winuser.h` 10.0.26100: the prototypes at `:9729`, `:9737`, `:9745` are open only under
  `#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)` (`:9717`) inside `#ifndef NOUSER` (`:64`). There is also no
  type-level sub-case, and the direction is the **opposite** of the `HDEVNOTIFY`/`FLASHWINFO` shape U16 usually records:
  `RECT` is declared at `shared/windef.h:154-160` under the *Application/Games/System* partition (`:152`), i.e. in a
  broader partition than the prototypes that use it, so a non-Desktop partition would delete the SDK prototypes while
  `user32.h:609-611` keeps compiling - which is harmless here because the wrapper binds by name, never through the
  prototype.


- **Section coverage (U21's shape again - the header's section is a slice of the SDK's block, and the SDK's block is
  spread over three of this library's TUs).** The SDK's own region is the contiguous Desktop-Family block
  `winuser.h:9716-9852`: `DrawFocusRect`, `FillRect`, `FrameRect`, `InvertRect`, `SetRect`, `SetRectEmpty`, `CopyRect`,
  `InflateRect`, `IntersectRect`, `UnionRect`, `SubtractRect`, `OffsetRect`, `IsRectEmpty`, `EqualRect`, `PtInRect` - 15
  entry points - plus `GetWindowWord`/`SetWindowWord` inside `#ifndef NOWINOFFSETS` (`:9832-9847`). This library declares
  **15 of the 15** but files them in three places: the 3 here, the 11 rect helpers at `user32.h:447-457`
  (`rectangle.cpp` - `CopyRect`, `EqualRect`, `InflateRect`, `IntersectRect`, `IsRectEmpty`, `OffsetRect`, `PtInRect`,
  `SetRect`, `SetRectEmpty`, `SubtractRect`, `UnionRect`, 11 definitions), and `DrawFocusRect` at `user32.h:267`
  (`painting.cpp:44`). So this session's section is complete on its own terms and the folder's coverage of the SDK block
  is complete too - what is missing is two exported names the header never declares at all: **`AlignRects`** (ordinal 6)
  and **`EvaluateProximityToRect`** (ordinal 10D), both **RESOLVED** in the probe, both Windows 8-era additions that
  Microsoft documents as internal ("do not use") - which is probably why they were skipped, and both are candidates for
  the same "declared nowhere" list U21 keeps.
- **Convention counts:** U13 0 of 3; U14(b) 0 deducing (3/3 explicit - all three spell the full SDK argument list);
  U17 0 (no A/W family, so no `T` suffix is due); U4 0; U11 0; U15 **3 of 3**; U19 **3 LAYER / 3 INDIRECT / 0 BYPASS /
  0 MEMBER** (`tmp/_tmp_fs_bypass.txt`); U16 **0 of 3**; U25 **3 declaration lines + 3 call-site template-argument
  spellings** (the largest single-TU contribution to the folder-wide `CONST::` family so far).
- **Liveness (U15):** each wrapper has exactly **one** call site, all three in `DeviceContext::FillRect`/`FrameRect`/
  `InvertRect` (`ui/gui/gdiobjects/devicecontext.cpp:75`, `:80`, `:85`, built at `gui/CMakeLists.txt:44`). Those three
  methods are non-virtual (`devicecontext.h:232-234`) and have **no call site anywhere** in `src`/`examples`/`test`, so
  all three wrappers are compiled, exported and never executed. The distinguishing feature against the other dead chains:
  the device context they would run on *is* live and *is* used by built code - `examples/.../mainwindow/mainwindow.cpp:84`
  (`WindowController::GetDC`) and `:173` (`PaintSession::GetDC`) both produce a `DeviceContext`, and `OnPaint` calls
  `GetTextExtentPoint32T`/`TextOutT` on it at `:179`, `:180`, with a `Rect` in hand at `:174` - a real painted window with
  a real DC and a real rect that simply never goes through `FillRect`. That makes this the most "reachable" dead chain in
  the folder so far, and the reason the caller-side cast below had to be checked rather than waved away.
- **Caller-side cast checked rather than assumed:** the three sites pass `reinterpret_cast<const RECT*>(&rc)` where `rc`
  is `qor::platform::win::Rect` (`ui/gui/view/drawing/rect.h:33-38`: `long m_left, m_top, m_right, m_bottom`) against
  `RECT` (`LONG left, top, right, bottom` at `shared/windef.h:154-160`). Field-for-field the same order and widths on this
  toolchain, so the alias is sound in practice - the same hand-mirror pattern as `COMBOBOXINFO` (U15) and
  `MSGBOXPARAMS` (U23), and now confirmed on a **by-value-painted** path rather than a struct-with-`cbSize` path. No UB
  observed, but it is still a `reinterpret_cast` between unrelated class types, i.e. technically an invalid alias: the
  gui side should convert (or derive), which is a `winqor_gui` change and the third instance of the same underlying
  decision. Recorded under the U23/U15 mirror-struct notes rather than as a new item.
- **Instrument note for later sessions (second `sigcmp` false-positive class):** `_tmp_sigcmp.ps1` compares the
  declaration's parameter spelling with the `Library::Call<>` template argument spelling as **strings**, so any site where
  the header writes `CONST::X*` and the call site writes `CONST ::X*` prints `CALL TYPE MISMATCH` - here 3 of 3 sites,
  all false (U25). Read the pair before believing the row: after preprocessing both are `const ::RECT*`. Added to the
  known-bad rows from session 14 (the `voidCall` row prints `(no Library::Call - voidCall or inlineCall?)` by design).
  `_tmp_name_divergence_scan.ps1` on this TU: 0 of 3 definitions, 0 parameter-name divergences - the folder's zero
  (established by session 12, re-confirmed by 13 and 14) holds for a fourth consecutive TU.
- **Verified at exit:** `filledshapes.cpp` compiles standalone at `/W4` **exit 0** after every edit
  (`tmp/_tmp_compile_one.ps1`); the whole library compiles **38/38 exit 0** with the edits in place
  (`cmd /c tmp/_tmp_cl_all.bat` -> `cl_exit=0`, `tmp/_tmp_cl_all.log` 42 lines, `filledshapes.cpp` echoed, and the only
  line matching `error` is the file name `errorhandling.cpp`); the post-edit `/EP` re-run (`pp_exit=0`) still yields the
  three bare literals in definition order; `user32.h` untouched (`git diff --stat` for the folder: 1 file changed,
  `filledshapes.cpp` +62/-1).
- **Instruments this session leaves behind:** `tmp/_tmp_pp_fs.bat` (+ `tmp/_tmp_fs_pp.txt`, `tmp/_tmp_fs_pp_err.txt`),
  `tmp/_tmp_fs_bypass.txt`, and `tmp/_tmp_name_resolve_probe.cpp` extended with 12 filled-shape spellings - the 3 stems x
  3 spellings plus `DrawFocusRect`, `AlignRects`, `EvaluateProximityToRect`, the last two being the coverage probes for
  the names this header omits - plus 5 help-family names pre-staged for session 16 (`WinHelpA`,
  `Set`/`GetWindowContextHelpId`, `Set`/`GetMenuContextHelpId`; all five **RESOLVED** in the post-edit run, and `WinHelp`
  itself **MISSING** with `WinHelpA`/`WinHelpW` **RESOLVED**, re-confirmed from the `:51-52` entries an earlier session
  added). `tmp/_tmp_cl_all.ps1`'s `pwsh -File` wrapper returned exit 1 this session while the generated
  `tmp/_tmp_cl_all.bat` it writes returned `cl_exit=0` with a clean 42-line log - run the `.bat` and read the log directly
  rather than trusting that wrapper's exit code.
  **Encoding trap checked rather than "fixed" (session 15):** `status.md` and the memory-bank files contain en dashes,
  em dashes and ellipses, and they are committed convention - counting them by piping `git show HEAD:<path>` through
  PowerShell reports **0** of each because PowerShell re-decodes the blob (each 3-byte dash arrives as three cp1252
  characters of mojibake), which invites a pointless "normalize the encoding" edit. Measure with
  `[System.IO.File]::ReadAllText` on the working file and compare code-point groups instead:
  `memory-bank/progress.md` U+2013 x56 / U+2014 x39 / U+2192 x20, and the working tree matches HEAD for those files.
  Source `.cpp` files in this folder stay pure ASCII - `filledshapes.cpp` has **0** non-ASCII bytes after this session's
  comment block.



---

## Consumed pre-scan notes for `help.cpp` (session 16) - moved here verbatim from status.md on 31 Oct 2026
superseded in full by the session-16 row of the status.md session-log table, open item U26 and task T4.

Measured pre-flags for `help.cpp` (session 16) - from this session's scans, so they are facts, not guesses:
- **Expect 0 P1 and 0 D13 conversions, and one binding that is already AW and must stay AW.** The export table has
  **`WinHelpA` 3EC / `WinHelpW` 3ED only - there is no bare `WinHelp`** (live probe re-run 31 Oct: `WinHelp` **MISSING**,
  `WinHelpA`/`WinHelpW` **RESOLVED**), so `help.cpp:35`'s `qor_pp_useswinapiAW(user32, WinHelp)` is mandatory, and
  `#define WinHelp WinHelpW` at `winuser.h:12386` is exactly the macro `user32.h:10` exists to kill. The two
  context-help-id wrappers at `:19`/`:28` are bare, and the four help-id names are genuine single exports
  (`SetWindowContextHelpId` 37A, `GetWindowContextHelpId` 1E1, `SetMenuContextHelpId` 34A, `GetMenuContextHelpId` 17C -
  no `A`/`W` spelling of any of them appears in `tmp/_tmp_user32_exports.txt`), all four **RESOLVED** by the probe run
  this session. All five names are already in `tmp/_tmp_name_resolve_probe.cpp` (`WinHelp` and `WinHelpW` from an earlier
  session at `:51-52`, the rest added by session 15), so no probe edit is needed to start. Run `cl /EP`, the probe and
  `_tmp_wrong_stem_scan.ps1` as usual.
- **The section is `//Shell functions` (`user32.h:641`), not "Help functions"** - the file name does not match the section
  it implements, and the section holds menu-help entries too: a U12/U21-shaped placement note.
- **2 U4 + likely 1 U13**: `user32.h:642-643` `SetMenuContextHelpId`/`GetMenuContextHelpId` are **non-static and defined
  nowhere in the folder**, while `:644-646` are `static` and defined. Both undeclared-off members are real exports, so the
  pair is a published-surface decision (add or delete), not a TU fix - collect it under U4/U13 like the rest.
- **U16 gains up to 4 of 5**: the context-help-id prototypes are `winuser.h:9125`, `:9132`, `:9138`, `:9145` under
  `#if(WINVER >= 0x0400)` (`:9105`) + Desktop (`:9110`) inside `#ifndef NOUSER` (`:64`); `WinHelpA`/`WinHelpW` are
  `:12372`/`:12380` under `#ifndef NOHELP` (`:12270`) + Desktop (`:12268`) with **no version guard**, so `WinHelp` adds 0
  and the other four add 1 each. Measured with `tmp/_tmp_sdk_guard_scan.ps1` this session (run over
  `WinHelpA,SetWindowContextHelpId,GetWindowContextHelpId,SetMenuContextHelpId`).
- **A dead `#undef WinHelp` at `help.cpp:14`** - `user32.h:10` already undefs the stem before this TU's own
  `#include "user32.h"`, and nothing after it can re-process `winuser.h` past its `_WINUSER_` guard, so the line is inert.
  Session 7 removed the identical line from `console.cpp:14` for the same reason (status.md `:1363-1364`) - same fix here.
- **Expect 3 of 3 U15, but re-measure with the instrument before writing it.** The three **LAYER** call sites are
  `ui/gui/controllers/windowcontroller.cpp:785`, `:790`, `:795` inside `WindowController::SetContextHelpId`
  (`:783-786`, declared `windowcontroller.h:284`), `GetContextHelpId` (`:788-791`, `:285`) and `Help` (`:793-796`, `:286`)
  - note the third method is **`Help`, not `WinHelp`**, and it folds the `BOOL` into `bool` with `? true : false`. All
  three methods are built (`gui/CMakeLists.txt:27`) and have **no call site anywhere** in `src`/`examples`/`test`
  (measured 31 Oct by scanning those roots for the three method names - only the declarations and definitions match), so
  the chain is compiled, exported and never executed one level up, exactly like most U15 instances. Run
  `_tmp_bypass_scan.ps1` over the three stems + `*A`/`*W` variants in the session to get the U19 split. The two menu-help
  exports have no wrapper, so no call site can exist for them.
- **U14 clean on inspection**: `user32.h:642-646` all five declarations spell explicit parameter types with `, ` spacing;
  `help.cpp:22`, `:29`, `:36` pass `HWND`/`DWORD`/`LPCTSTR`/`UINT`/`ULONG_PTR` explicitly. Note `WinHelp`'s `LPCTSTR`
  parameter is `_In_opt_` in the SDK (`winuser.h:12374`, `:12382`), and `hWndMain` is `_In_opt_` too - worth a contracts
  sentence, and `windowcontroller.cpp:795` passes `help.c_str()` so it never exercises the optional case. Two caller-side
  `BOOL` conversions to check there: `:785` returns the wrapper's `BOOL` from a `bool` method **implicitly**, while `:795`
  does it explicitly with `? true : false` - a consistency nit on the gui side, not in this layer.
- The name-divergence scan should report **0** again: session 12 took the folder to zero (2 of 426 definitions, both in
  `devicemanagement.cpp`) and sessions 13, 14 and 15 re-confirmed 0.
Take the pre-scan P1/P2/P3/P5 entries into account only when their owning TU comes up; do not fix other TUs outside
their own session.

### Session 16's confirmation/correction pass over those pre-flags (as recorded in status.md before consolidation)

Measured pre-flags for `help.cpp` (session 16) - kept as the record of what was measured; every bullet below was
confirmed or corrected by session 16 (its record is now the session-log table row 16 in `status.md`, item **U26** and
task **T4**):

- **The five-declaration asymmetry and the `AW` mandate are confirmed by this session.** `user32.h:642-643`
  `SetMenuContextHelpId`/`GetMenuContextHelpId` are **non-static and defined nowhere in the folder** (a U4 pair; both real
  exports, `17C`/`34A`, both already in `tmp/_tmp_name_resolve_probe.cpp` from session 15). `WinHelp` is the folder's
  second-confirmed **AW** binding (`qor_pp_useswinapiAW(user32, WinHelp)`; `WinHelp` MISSING / `WinHelpW` RESOLVED per the
  session-1 probe note at line 306) and the only stem here that is in the `user32.h:9-17` `#undef` list, so the `#undef` at
  `help.cpp:14` is dead by construction: `user32.h` includes only `kernel32.h` at `:7` and does the nine `#undef`s at
  `:9-17`, and no later include can re-process `winuser.h` past its own guard. Callers traced:
  `windowcontroller.cpp:785`, `:790`, `:795`. In `add_library` (`user/CMakeLists.txt:11`).
- **Pre-measured exports and guards, all confirmed with one citation correction.** All five names are real exports, and
  the export table has **`WinHelpA`/`WinHelpW` only** - no bare `WinHelp`, which is why the `AW` form is mandatory. The
  four context-help-id prototypes are at `winuser.h:9122-9127`, `:9129-9133`, `:9135-9140`, `:9142-9146` (the earlier
  `:9125`/`:9132`/`:9138`/`:9145` citations are the name lines of the same four blocks) under `#if(WINVER >= 0x0400)`
  (`:9105`) + Desktop (`:9110`) inside the `#ifndef NOUSER` region; `WinHelpA`/`WinHelpW` are at `:12369-12376` /
  `:12377-12384` under `#ifndef NOHELP` (`:12270`) + Desktop (`:12268`) with **no version guard**, and the
  `#ifdef UNICODE` / `#define WinHelp WinHelpW` at `:12385-12386` (`#else` `WinHelpA` at `:12388`) is the macro
  `user32.h:10` exists to kill. So **U16 turned out 0 of 3, not 4 of 5**: the version guard is real but benign here,
  because none of the types these three wrappers name is version-gated (session-16 measurement).

### The pre-flags as they stood in the session-15 commit (`status.md` at HEAD `9ecbf581`, lines 117-135)

**Next session: `help.cpp`** (alphabetical from here; 16 TUs done across sessions 1-15: `accelerator`,
`accessibility`, `brush`, `caret`, `clipboard`, `combobox`, `console`, `timer`, `coordtransform`, `cursor`, `desktop`,
`devicecontext`, `devicemanagement`, `dialog`, `errorhandling`, `filledshapes`). **3 wrappers over 5 declarations**, a
39-line TU with no comment block (`:18 SetWindowContextHelpId`, `:25 GetWindowContextHelpId`, `:32 WinHelp`) plus a
redundant `#undef WinHelp` at `:14`. The section is `//Shell functions` (`user32.h:641`), **not** "Help functions" - the
file name does not match the section it implements, worth a U12/U21-style note. `:642-643`
`SetMenuContextHelpId`/`GetMenuContextHelpId` are **non-static and defined nowhere in the folder** (a U4 + U13 pair; both
are real exports, `17C`/`34A`, and both are already in `tmp/_tmp_name_resolve_probe.cpp` from session 15). `WinHelp` is the folder's
second-confirmed **AW** binding (`qor_pp_useswinapiAW(user32, WinHelp)`; `WinHelp` MISSING / `WinHelpW` RESOLVED per the
session-1 probe note at line 306) and the only stem here that is in the `user32.h:9-17` `#undef` list, so the `#undef` at
`help.cpp:14` is dead by construction: `user32.h` includes only `kernel32.h` at `:8` and does the nine `#undef`s at
`:9-17`, and no later include can re-process `winuser.h` past its own guard. Callers to trace:
`windowcontroller.cpp:785`, `:790`, `:795`. In `add_library` (`user/CMakeLists.txt:11`).
Pre-measured so the session can start at the analysis: all five names are real exports (`17C`, `34A`, `3EC`, `3ED` - and
note the export table has **`WinHelpA`/`WinHelpW` only**, no bare `WinHelp`, which is why the `AW` form is mandatory here);
the four context-help-id prototypes are at `winuser.h:9125`, `:9132`, `:9138`, `:9145` under
`#if(WINVER >= 0x0400)` (`:9105`) + Desktop (`:9110`) - so **4 of 5 are U16 instances** - while `WinHelpA`/`WinHelpW` are
at `:12372`/`:12380` under `#ifndef NOHELP` (`:12270`) + Desktop (`:12268`) with **no version guard**, and the
`#define WinHelp WinHelpW` at `:12386` is the macro `user32.h:10` exists to kill.

(The `:9125`/`:9132`/`:9138`/`:9145`, `:12372`/`:12380` and "`kernel32.h` at `:8`" citations in that last block were
superseded by the corrected measurements in the block above; `:8` vs `:7` is the `user32.h` include line as read in the
two sessions - session 16's `:7` is the measured value.)

### Superseded bookkeeping lines kept for the record (session-15 commit `status.md`, lines 115, 229-234)

```
| 16..38 | not started (TU index; 22 left, see "Remaining queue") | | | |

Remaining queue (22 TUs, alphabetical; `accelerator.cpp`, `accessibility.cpp`, `brush.cpp`, `caret.cpp`,
`clipboard.cpp`, `combobox.cpp`, `console.cpp`, `timer.cpp`, `coordtransform.cpp`, `cursor.cpp`, `desktop.cpp`,
`devicecontext.cpp`, `devicemanagement.cpp`, `dialog.cpp`, `errorhandling.cpp`, `filledshapes.cpp` done):
help, hook, icon, ime, keyboard, mdi, menu, message, monitor, mouse, painting,
processthread, rectangle, resource, scrollbar, systemshutdown, text, window, windowclass, windowprocedure,
windowproperties, winqor_api_user (module glue). `hook.cpp` is now in the build (see U3).

This file carries the session log and the open items. `memory-bank/activeContext.md` carries a pointer here.
```

Superseded by session 16 (`help.cpp` reviewed): the queue is now 21 TUs and the table's last row is "17..38".
The last line of the block above is `status.md`'s old one-sentence header pointer, replaced on consolidation by the
three-line header that also names this archive file.




## Moved out of `status.md` on 1 Nov 2026 (session log, session-17 block, icon pre-flags, notes, residue, consolidation history)

## Session log

| # | TU | Wrappers | Verdict | Action taken |
|---|---|---|---|---|
| 1 | `accelerator.cpp` | 5 | 0 correctness bugs, 3 fragility/consistency defects | Fixed: bare-stem bindings on the three A/W families made explicit `qor_pp_useswinapiAW` (29 Oct 2026); conforms to D13 |
| 2 | `accessibility.cpp` + `timer.cpp` | 1 + 2 | 0 correctness/UB defects; 1 file-organisation defect (the two files held each other's bodies), 2 style nits | Fixed: swapped the bodies so each file matches its `user32.h` section (was U12), `( pFunc … )` -> `(pFunc …)` at both call sites, added single-export comments. All three bindings confirmed correct-as-written by `cl /EP` + export table; no A/W conversion needed |
| 2b | `accessibility.cpp` (same day, after user commit `89da3452` added the three missing WinEvent wrappers) | +3 | 0 binding defects; **1 call-site defect found: the three declarations lacked `static`, so every call site was `error C2352`** | Fixed: `user32.h:662-664` made `static` (verified by a compile-only caller probe, before = 3 errors, after = exit 0); TU comments generalised to the four WinEvent entry points. Census re-run: `user32.h` now 524 declarations, 427 static, 97 non-static (was 100) - see U4, and new U13 for the one remaining defined-but-non-static member |
| 3 | `brush.cpp` | 1 | 0 correctness/UB/binding defects; 1 convention defect (implicit template args), 1 documentation gap | Fixed: `Call< HBRUSH >` -> `Call<HBRUSH, int>` (explicit SDK arg types - folder norm 382/428, reference `shell`+`kernel` 344/387) + a single-export and handle-ownership comment added. Binding correct as written: no A/W family, `cl /EP` resolves to `"GetSysColorBrush"`, which is the export (table index 1958, ordinal 1C6) |
| 4 | `caret.cpp` | 8 | 0 correctness/UB/binding/convention defects - the cleanest TU so far | Fixed: TU comment only (single-export binding rationale under D13 + the `CreateCaret` bitmap contract). All 8 bindings correct as written; `cl /EP` resolves all 8 to the literal stems, all 8 are exports, and they are the only 8 `*Caret*` entries in `user32.dll`, so the section is complete. U14(b)'s "caret 2" was a miscount - caret.cpp has 0 deducing call sites (re-measured, see U14) |
| 5 | `clipboard.cpp` | 20 | 0 correctness/UB/binding/convention defects in the wrappers - second clean TU after `caret.cpp`, and the first already D13-conformant before the session started | Fixed: `()` -> `(void)` at the three zero-argument definitions (`CloseClipboard`, `CountClipboardFormats`, `GetOpenClipboardWindow`) plus their three `user32.h` declarations (the SDK spells `VOID`; folder norm now 36 defs against 2, 37 decls against 2 - see U14(c)) + a TU comment (bare-stem rationale for the 18 single exports, the one-switch A/W argument, the clipboard affinity/ownership contracts, section coverage and deprecation). All 20 bindings verified correct as written by `cl /EP` + export table |
| 6 | `combobox.cpp` | 3 | **2 real run-time failures** - pre-scan P1: both `DlgDir*` bindings asked `GetProcAddress` for stems `user32.dll` does not export, so those wrappers could only ever throw; plus 2 consistency defects (no `T` suffix on an A/W wrapper, `User32` module token) and 1 documentation gap | Fixed: both bindings now `qor_pp_useswinapiAW(user32, ...)` (`cl /EP` resolves `"DlgDirListComboBoxW"`/`"DlgDirSelectComboBoxExW"`, both RESOLVED by live probe); members renamed `DlgDirListComboBoxT`/`DlgDirSelectComboBoxExT` at `user32.h:551-552` + definitions (user decision; new **U17** tracks the 11 remaining A/W wrappers without the suffix); module token `User32` -> `user32`; TU comment added. `GetComboBoxInfo` correct as written (single export, ordinal 13D, no A/W family). Also produced a **new pre-scan P1 finding** in `window.cpp:124` (`CreateWindowW` MISSING) |
| 7 | `console.cpp` | 1 | 0 correctness/binding/UB defects - and the first wrapper in the review with **no SDK prototype to pair against at all**: neither `ConsoleControl` nor `CONSOLECONTROL` nor the five console-internal structures are declared anywhere in SDK 10.0.26100.0 or 10.0.10240.0. 1 declaration defect (**U13** - the only defined member declared non-static) + 3 nits (dead `#undef WinHelp`, `//Console` section comment, missing comma space) | Fixed: `static` added at `user32.h:690` (U13 closed; caller probe `tmp/_tmp_console_static_probe.cpp` - before = 1 x C2352 exit 2, after = exit 0), `,DWORD` -> `, DWORD`, `//Console` -> `//Console functions`, dead `#undef WinHelp` removed, TU comment added. Binding correct as written: `cl /EP` -> `"ConsoleControl"`, which is the only `*Console*` name in the export table (ordinal 1594, hint 55, RVA 00028FD0, not a forwarder); live probe RESOLVED with `ConsoleControlA`/`ConsoleControlW` MISSING, so the bare stem is the D13 spelling and the missing `T` suffix is correct |
| 8 | `coordtransform.cpp` | 3 | **3 real run-time defects** - all three wrappers bound `GetDCEx`, a real export, so every existing check (export table, `cl /EP`, compile) passed while no wrapper called its own entry point; 0 convention defects (`static` already correct at `user32.h:377-379`) | Fixed: bindings `GetDCEx` -> `ClientToScreen`/`ScreenToClient`/`MapWindowPoints` (`cl /EP` resolves the three literals; live probe RESOLVED with `*A`/`*W` all MISSING, so the bare stem is the D13 spelling) + TU comment (binding rationale, `_Inout_` POINT ownership, the "non-zero `MapWindowPoints` return is a delta, not a failure" contract, section coverage). No header change, no rename. New pre-scan **P5** covers the defect class; new **U18** records `window.cpp`'s `void`-returning point converters |
| 9 | `cursor.cpp` | 16 | **0 binding / correctness / UB / convention defects** - largest clean TU of the review; all 16 signatures pair field-for-field with the `winuser.h` prototypes and all 16 bindings resolve (14 bare single exports + 2 A/W switches). The two claims a reader would take on trust were measured instead: `CopyCursorT` -> `CopyIcon` is the only correct binding of a name the SDK spells as a macro, and `HCURSOR` *is* `HICON` in this build (`/EP` gives exactly one `typedef HICON HCURSOR;`) | Fixed: TU comment block only (binding rationale + ordinals/RVAs, the `CopyCursor`-is-a-macro note, per-wrapper handle ownership, the `ShowCursor` count-not-failure contract, the unwrapped cursor exports). No header change, no rename (P3/U17 decision: deferred to the folder-wide pass). New **U20** opened from the caller side |
| 10 | `desktop.cpp` | 18 | 0 binding / correctness / UB defects - all 18 bindings resolve (9 A/W families bound `qor_pp_useswinapiAW`, 9 single exports bound bare; `cl /EP` + export table + live probe), 0 P1/P5 flags; 4 consistency defects (2 U14(b) deducing sites, 2 parameter decl/def divergences), 1 documentation gap | Fixed: `OpenWindowStationT`'s parameter `LPTSTR` -> `LPCTSTR` in **both** the declaration `user32.h:412` and the definition (the only decl/def parameter-type divergence in the section), `CreateDesktopT`'s definition parameter `pDevMode` -> `pDevmode` to match its declaration, both U14(b) sites made explicit (`GetThreadDesktop`, `OpenWindowStationT`) + a 28-line TU comment (binding rationale, `HDESK`/`HWINSTA` ownership, thread/desktop contracts, callback widening, section coverage). **18/18 wrappers have no in-tree caller** - the largest U15 block of the review. Two items deferred as tasks **T1** and **T2** (see below) |
| 11 | `devicecontext.cpp` | 8 | **0 binding / correctness / UB / convention defects** - all 8 signatures pair field-for-field with the `winuser.h` prototypes and all 8 bindings resolve (5 A/W families bound `qor_pp_useswinapiAW` -> the `*W` export, 3 single exports bound bare: `GetDC` 143, `GetDCEx` 144, `ReleaseDC` 2FF); 0 P1/P5 flags, 0 U13/U14(b)/U17/U18/U20 instances; 1 style nit | Fixed: `(pFunc, hWnd )` -> `(pFunc, hWnd)` + a 36-line TU comment (binding rationale and ordinals, the **two opposite zero-return conventions** in this one section, the `_Reserved_`/`_In_opt_`/`_Inout_` parameter contracts, `GetDC`/`GetDCEx`/`ReleaseDC` pairing, section coverage, U16 guards including the hard `NOGDI`/`wingdi.h` type dependency). New **U21**: the SDK's DC neighbourhood (`winuser.h:8560-8614`) is split across this TU and `painting.cpp` (`GetWindowDC`, `WindowFromDC`), while 5 of this TU's 8 members are display-configuration entry points with no `HDC` at all. **Largest U19 split of the review - 16 LAYER / 12 BYPASS** - only 4 of the 16 LAYER sites are reachable at run time, and all 12 BYPASS sites are in the disabled `ui/gl` tree. Also found two **scan-instrument blind spots** that had been under-counting families in earlier sessions (bare-stem matching and line-start-only `::` matching) - see step 5b and the U19 entry |
| 12 | `devicemanagement.cpp` | 2 | DONE 30 Oct (session 12) - 0 binding / correctness / UB / convention defects; **2 naming defects, and they are the only 2 in the whole folder** | "Device Management functions" (`user32.h:681-683`): 2 declarations, 2 definitions, header order, both `static`, both `User32::`-qualified (U13 clean), both `Call<>` sites fully explicit (U14: 2/2 explicit, spaced style, no spacing nit). **Both bindings were already D13-correct**: `qor_pp_useswinapiAW(user32, RegisterDeviceNotification)` -> `/EP` resolves `"RegisterDeviceNotificationW"` (2E5, RESOLVED), and the bare `qor_pp_useswinapi(user32, UnregisterDeviceNotification)` is the right spelling because the probe shows `UnregisterDeviceNotificationA`/`W` both **MISSING** - a genuine single export (3C6). This stem is one of the nine `user32.h:9-17` `#undef`'s **and** bare `"RegisterDeviceNotification"` is MISSING (G2 confirmed live for this stem), but the AW token-paste means the `#undef` never reaches the binding, so the pre-flagged question was naming-only. **Fixed (2 sites, both naming):** definition parameters renamed to their declaration/SDK names - `notificationFilter` -> `NotificationFilter` (`:73` signature + `:77` call arg), `handle` -> `Handle` (`:80` + `:84`; line numbers of the file as it now stands, with the TU comment block in place). Measured outlier: the new instrument `tmp/_tmp_name_divergence_scan.ps1` reports **2 divergent parameter names across 426 definitions folder-wide - both in this TU**. TU comment block added. **U17 1/2** (no rename, per the session-11 policy) with a sharpened argument: the A/W choice here is invisible in the signature, since the filter is a bare `LPVOID` whose trailing `TCHAR` name field is read per the bound half. **U15 2/2** - the first section where every wrapper is dead *and* the family is still used in the tree, through a bypass. **U16 2/2** - both prototypes sit under `#if(WINVER >= 0x0500)` (`winuser.h:3942`) + `WINAPI_PARTITION_DESKTOP` (`:3940`), and `HDEVNOTIFY` is itself typedef'd at `:3943` under the same guards, so a lower `WINVER` makes `user32.h:682-683` **ill-formed** rather than merely absent (the hardest U16 dependency so far after `NOGDI`). **U19: 0 LAYER / 1 BYPASS** - `ui/gl/helperwindow.cpp:85` `::RegisterDeviceNotificationW` is live code (file 0% commented) that fills `dbcc_size`/`dbcc_devicetype`/`dbcc_classguid` first (`:79-83`), but the file is in no source list (`winqor_gl` = `winqor_gl.cpp cursorimpl.cpp windowimpl.cpp wgl.cpp`) and `ui/CMakeLists.txt:4` comments `add_subdirectory(gl)` out -> source debt, not run-time behaviour. **New deferred task T3**: the adjacent "Power Management functions" pair (`user32.h:686-687`; exports 2F1/3CB, both RESOLVED in the probe) is defined in no TU and has **no owning file in the folder at all**. 22 open items unchanged (U1-U22) |
| 13 | `dialog.cpp` | 22 | **1 real run-time failure + 1 convention defect**; 1 published-signature defect and 1 cosmetic inconsistency recorded, not fixed | Fixed: `:89` `qor_pp_useswinapi(user32, GetDlgItemTextT)` -> `qor_pp_useswinapiAW(user32, GetDlgItemText)` (`/EP` post-fix `"GetDlgItemTextW"`, probe RESOLVED - pre-fix asked for a name the DLL does not export) and `:124` `MessageBox` -> explicit AW (no behaviour change, G1). All 22 bindings then confirmed by `/EP` + export table + live probe; the 9 bare spellings are correct because the SDK declares those nine entry points with a single non-A/W spelling. decl==def 22/22; 0 parameter-name divergences (folder now 0 of 426). New **U23** from `MessageBoxIndirectT`'s `const ::LPMSGBOXPARAMS` |
| 14 | `errorhandling.cpp` | 4 | **0 binding / correctness / UB / convention defects** - the "Error Handling functions" section is fully covered by this TU (4 declarations, 4 definitions: 0 U4, 0 U11, 0 U13); all four bindings bare and all four correct as written (`cl /EP` -> `"FlashWindow"`, `"FlashWindowEx"`, `"MessageBeep"`, `"SetLastErrorEx"`, all four **RESOLVED** by live probe with every `*A`/`*W` spelling **MISSING**, so the bare stem is the D13 spelling and `AW` would break all four); `SetLastErrorEx`'s `void` wrapper matches the SDK's `VOID` (`winuser.h:13558-13563`), so **no new U18 instance**; 2 trailing-space nits at the call sites | Fixed: TU comment block (46 lines, `:16-61`) + `(pFunc, uType )` -> `(pFunc, uType)` at the `CheckReturn` site and `(pFunc, dwErrCode, dwType )` -> `(pFunc, dwErrCode, dwType)` at the `voidCall` site. New **U24** from the one in-tree caller, which reads `FlashWindow`'s previous-active-state `BOOL` as a success flag |
| 15 | `filledshapes.cpp` | 3 | **0 binding / correctness / UB / convention defects** - the "Filled Shapes functions" section is fully covered by this TU (3 declarations, 3 definitions: 0 U4, 0 U11, 0 U13); all three bindings bare and all three correct as written (`cl /EP` -> `"FillRect"`, `"FrameRect"`, `"InvertRect"`, all three **RESOLVED** by live probe with every `*A`/`*W` spelling **MISSING**, so the bare stem is the D13 spelling and `AW` would break all three); return types match the SDK exactly (`int`/`int`/`BOOL` at `winuser.h:9726-9747`), so **no new U18 instance**; U16 gets **0 of 3** - the three prototypes sit under nothing but the Desktop partition + `NOUSER`, no version guard | Fixed: TU comment block (61 lines, `:16-76`) + one blank line removed between the comment and the first definition. New **U25** from this TU's `CONST::RECT*` spelling (17 header lines use `CONST::`, 7 use `const ::`); `sigcmp` prints a **false** `CALL TYPE MISMATCH` on all three sites here - instrument note below |
| 16 | `help.cpp` | 3 (over 5 declarations) | **0 binding / correctness / UB / convention defects** - the `//Shell functions` block (`user32.h:641-646`) is **5 declarations, 3 definitions**: the 3 here are in header order, `static`, `User32`-qualified, `Call<>` explicit 3/3, and all 3 bindings were already D13-correct (2 bare single exports - `SetWindowContextHelpId` 37A, `GetWindowContextHelpId` 1E1, no `A`/`W` spelling of either exists - plus the folder's second confirmed **AW**: the table has **no bare `WinHelp`**, only `WinHelpA` 3EC / `WinHelpW` 3ED, probe `WinHelp` **MISSING** / `WinHelpW` **RESOLVED**); 0 U11, 0 U13, 0 U14(b), 0 U17, 0 U18, 0 U25; **U16 0 of 3** (the types are version-independent, unlike `FLASHWINFO`/`HDEVNOTIFY`); 0 parameter-name divergences (folder zero holds for a fifth TU); the four SDK help-id prototypes have **unnamed parameters**, so parameter names cannot be SDK-compared here at all | Fixed: the inert `#undef WinHelp` at `:14` removed - measured as the **only** `#undef` directive in any of the 38 `.cpp` files, so the folder is now clean of them - + a 70-line TU comment (`:16-85`) + 2 stray blank lines; file 39 -> 108 lines, pure ASCII; `user32.h` untouched. **2 U4 members** (`:642-643` `SetMenuContextHelpId`/`GetMenuContextHelpId` - declared non-static, defined in no TU, and **absent from the built import library** per `dumpbin /linkermember:1`, which lists exactly the 3 help-family members) -> task **T4**. New **U26** (TCHAR width is an include-order property, not a build setting) + a measured correction to **U2** (`../returncheck.h` is included by 37 of 38 TUs and used by 2). **U15 3/3**, **U19 3 LAYER / 0 INDIRECT / 0 BYPASS / 0 MEMBER** - cleanest split of the review |
| 17 | `hook.cpp` | 4 | **0 binding / correctness / UB / convention defects** - the `//Hook functions` block (`user32.h:315-319`) is 4 declarations, 4 definitions, in header order, all `static`, all `User32::`-qualified, `Call<>` explicit 4/4 (spaced style); all 4 bindings were already D13-correct and are now verified three ways (`cl /EP` literal + export table + live probe): `CallMsgFilterT` -> `"CallMsgFilterW"`, `CallNextHookEx` -> `"CallNextHookEx"`, `SetWindowsHookExT` -> `"SetWindowsHookExW"`, `UnhookWindowsHookEx` -> `"UnhookWindowsHookEx"` (`tmp/_tmp_hook_pp.txt`); the two `AW` bindings are **load-bearing** (live probe: bare `SetWindowsHookEx` **MISSING**, `SetWindowsHookExW`/`A`/**`AW`** all RESOLVED) and the two bare ones are the D13 spelling (no `CallNextHookExA/W` or `UnhookWindowsHookExA/W` in the table); signatures and returns match the SDK exactly (`winuser.h:5401-5403`, `:10384-10388`, `:10399-10400`, `:10405-10409`), so **0 U18**; 0 U11, 0 U13, **0 U17** - the pre-flag "2 more U17 instances" was **wrong**: `CallMsgFilterT`/`SetWindowsHookExT` do carry the `T` suffix and sit in the census' 108-member norm bucket (census re-run unchanged: 525 declarations, 108 A/W+`T`, 404 bare single exports, 11 deviations, 2 mirror cases); **U16 0 of 4** - no version guard anywhere, only `#ifndef NOWH` (`:10322`) / `#ifndef NOMSG` (`:5390`) + the Desktop partition + `NOUSER`, measured with `tmp/_tmp_hook_guards.ps1` (8 hits over 16475 lines; the two extra `CallNextHookEx` rows are the `DefHookProc` macro at `:10416`/`:10419`, not declarations) | Fixed: **1 parameter-name divergence** - `user32.h:318` `hMod` -> `hmod` to match `winuser.h:10379`, with the definition (`hook.cpp:94`) and the `Call<>` argument following; the folder's 3rd such fix after `devicemanagement`'s 2, and the first where the name-divergence scan flagged it in a TU whose parameter names otherwise match the SDK. + a 64-line TU comment (`:16-79`); file 44 -> 108 lines, pure ASCII, 0 tabs, 0 trailing whitespace, CRLF throughout. New pre-scan item **P6** (bare `CallMsgFilter` is exported at 1C at RVA `00088CF0` - **the same RVA as `CallMsgFilterA`** - so a wrong binding there resolves and runs the ANSI entry point on wide arguments instead of throwing) and new open item **U27** (`STRICT` changes the identity of `HOOKPROC` and of every `DECLARE_HANDLE` type, hence the published mangled names: 3 of these 4 wrappers change name between the `STRICT` and `NO_STRICT` arms, measured with `tmp/_tmp_hookproc_probe.cpp`). **U15 4/4 dead**, **U19: 4 stems with 0 sites of any class** - the first section of the review with no caller *and* no SDK-import call anywhere, `HHOOK`/`HOOKPROC` appearing in no file of `src`/`examples`/`test` at all |
| 18..38 | not started (TU index; 20 left, see "Remaining queue") | | | |

**Session 17 (`hook.cpp`, 31 Oct 2026) - closed.** Row 17 above plus new items **P6** and **U27** carry the record, and the TU
narrative is the 64-line comment block now at `hook.cpp:16-79`, so this session has no separate write-up (the session-16
pattern). The consumed `hook.cpp` pre-flags are archived verbatim in `session-log-archive.md` under "Consumed pre-scan
notes"; one of them was wrong and is corrected in row 17 (the "2 more U17 instances" flag - both wrappers do carry the `T`
suffix).

**Next session: `icon.cpp`** (alphabetical from here; **18 TUs done** across sessions 1-17: `accelerator`,
`accessibility`, `brush`, `caret`, `clipboard`, `combobox`, `console`, `timer`, `coordtransform`, `cursor`, `desktop`,
`devicecontext`, `devicemanagement`, `dialog`, `errorhandling`, `filledshapes`, `help`, `hook`). The "Icon functions"
section is `user32.h:426-440`: **14 declarations, all `static`, all 14 defined here** (`icon.cpp:16`..`:107`, in header
order) - a 114-line TU with **no comment block**. `_tmp_sigcmp.ps1 -Tu icon.cpp -First 427 -Last 440` gives **14/14
decl==def ok** and 13/14 `Call ok` (run 31 Oct) - the single `CALL TYPE MISMATCH` at `icon.cpp:48` (`sdk 'const
::ICONINFO*' vs call 'const ICONINFO*'`) is the known `::`-spelling false positive class, see "Instrument caveats".

Measured pre-flags for `icon.cpp` (session 18) - every number below measured 31 Oct 2026, so they are facts, not guesses:
- **11 bare bindings + 3 `AW`, and the `AW` set is exactly the SDK's three Icon A/W families.** The export table
  (`tmp/_tmp_user32_exports.txt`) has `CopyIcon` 59, `CreateIcon` 6B, `CreateIconFromResource` 6C,
  `CreateIconFromResourceEx` 6D, `CreateIconIndirect` 6E, `DestroyIcon` B1, `DrawIcon` D5, `DrawIconEx` D6,
  `GetIconInfo` 160, `GetIconInfoExA` 161/`W` 162, `LoadIconA` 25A/`W` 25B, `LookupIconIdFromDirectory` 26F,
  `LookupIconIdFromDirectoryEx` 270, `PrivateExtractIconsA` 2B5/`W` 2B6. `tmp/_tmp_winuser_aw_macros.txt` has exactly
  three `*Icon*` macros (`LoadIcon`, `GetIconInfoEx`, `PrivateExtractIcons`) - precisely the three `AW` sites at
  `icon.cpp:82`, `:89`, `:110`, so 0 P5 wrong-stem flags and 0 D13 conversions needed. The live probe (rows staged
  31 Oct, run 31 Oct): all **11 bare stems RESOLVED**, and bare `GetIconInfoEx`/`LoadIcon`/`PrivateExtractIcons`
  **MISSING** with their `W` halves RESOLVED - the three `AW` bindings are load-bearing (a bare binding there asks for a
  name `user32.dll` does not export) and the eleven bare ones are the D13 spelling. Still re-run `cl /EP` into its own
  file to confirm the literals, as every session does.
- **U17: 0 instances** - the three A/W wrappers already carry the folder's `T` suffix (`GetIconInfoExT`, `LoadIconT`,
  `PrivateExtractIconsT`), and the census (525/108/404/11/2, re-run 31 Oct) is unchanged.
- **U16: at least 3 of the 14 prototypes are version-gated, and one is the ill-formed-type sub-case.** Guard stacks
  measured 31 Oct with `tmp/_tmp_guard_stack_scan.ps1`: `LookupIconIdFromDirectoryEx` (`winuser.h:10819`) sits under
  `#if(WINVER >= 0x0400)` (`:10815`) and `DrawIconEx` (`:10948`) under `:10883` (also `0x0400`); `GetIconInfoExW`
  (`:11026`) sits under `#if(_WIN32_WINNT >= 0x0600)` (**`:10986`, closed `:11034`**) - and `ICONINFOEXA`/`ICONINFOEXW`
  plus the `PICONINFOEX` typedef are at **`:10987`-`:11014`, inside that same guard**, so `user32.h:436`'s
  `GetIconInfoExT(HICON, ::PICONINFOEX)` does not merely lose its prototype below `_WIN32_WINNT 0x0600`, it becomes
  ill-formed on an unknown type - the `HDEVNOTIFY`/`DEVMODE`/`MSGBOXPARAMSW`/`PFLASHWINFO` sub-case again, and the
  highest version threshold of them. The rest (`LoadIconW` `:10750`, `PrivateExtractIconsW` `:10775`, `CreateIcon`
  `:10793`, `CreateIconIndirect`/`CopyIcon`/`GetIconInfo` `:10965`-`:10982`) sit under nothing but the Desktop partition
  + `NOUSER` (`:64`) - no version test at all. Four names still need their stacks read
  (`CreateIconFromResource`/`Ex`, `DestroyIcon`, `LookupIconIdFromDirectory`, `GetIconInfoExA`).
- **U19: 17 stems -> 16 LAYER / 0 INDIRECT / 7 BYPASS / 0 MEMBER** (31 Oct, corrected instrument). All 16 **LAYER** sites
  are in one **built** file: `ui/gui/gdiobjects/icon.cpp:18`, `:24`, `:30`, `:36`, `:42`, `:48`, `:54`, `:62`, `:73`,
  `:79`, `:84`, `:89`, `:94`, `:99`, `:104`, `:109` (`gui/CMakeLists.txt:49`) - the first family of the review where
  every wrapper has a compiled call site, so the session's liveness work is the one-level-up question, and **U20** is
  already open on the `Icon` ownership model in exactly that file. The 7 **BYPASS** are all in **unbuilt** targets: 5 in
  `ui/gl/windowimpl.cpp:191`, `:1603`, `:1608`, `:1642`, `:1645` (`ui/CMakeLists.txt:4 #add_subdirectory(gl)`) and 2 in
  `examples/.../retropad/controller.cpp:43`, `retropad.cpp:940` (`osspecific/windows/CMakeLists.txt:9`) -> **0 bypass in
  built code**.
- **Section coverage:** `PrivateExtractIconExA` 2B3 / `PrivateExtractIconExW` 2B4 are exported (both RESOLVED in the
  probe) and declared **nowhere** in `user32.h`, one name away from the wrapped `PrivateExtractIcons` - decide whether
  the section should include them. `ArrangeIconicWindows` E (`user32.h:135`), `IsIconic` 237 (`:181`) and `OpenIcon` 29F
  (`:190`) are Icon-adjacent exports declared in the **Window** section instead (a U21-shaped placement note), and
  `InternalGetWindowIcon` 221 is internal. `winuser.h:10681` defines `CopyCursor(pcur)` as a macro over `CopyIcon`, so
  the cursor family silently depends on this TU's `CopyIcon`.
- Style to check against U14/U25 in the session: the file spells all 14 sites **tight** (`Call<HICON, HICON>`), it has no
  TU comment block at all, and `icon.cpp:44`/`:48` use `const ::ICONINFO*` / `const ICONINFO*` - the `::`-spelling split
  in a `Call<>` template argument (the `dialog.cpp` class, cured by spelling the site like the declaration).

Take the pre-scan P1/P2/P3/P5 entries into account only when their owning TU comes up; do not fix other TUs outside
their own session.

**What is deliberately not in the queue: there is no `dde.cpp`** - the 34-declaration DDE
section (`user32.h:501-534`) has no TU anywhere, which is a U4 surface decision, not a TU session (see the U4 entry).

**Consumed pre-scan notes.** The `help.cpp` (session 16) pre-flags that used to sit in this space are superseded in full
by the session-16 row in the table above, the **U26** entry and task **T4**; the pre-flag text itself is kept verbatim in
`session-log-archive.md` under "Consumed pre-scan notes". Sessions 15/16's `filledshapes`/`help` pre-flags were consumed
the same way and are not repeated here.


## Consolidated session notes (sessions 1-16) - the narratives moved out

**Where each kind of fact lives now.** The session-log table at the top of this file is the per-TU verdict-and-action
record. The ground rules (G1-G6), open items (U1-U26), pre-scan items (P1-P5) and deferred tasks (T1-T4) above are the
authority for every cross-cutting finding, count and decision, and they were left intact by this consolidation. The
**full per-TU narrative write-ups of sessions 2-15 are archived verbatim in `session-log-archive.md`** (1,214 lines /
122 KB, moved out of this file on 31 Oct 2026 when it passed 237 KB) together with the `help.cpp` pre-flags of sessions 15
and 16 and the `accelerator.cpp` write-up; nothing was deleted, and every load-bearing fact from those write-ups is also
carried by the registers above. Session 16 (`help.cpp`) has no narrative of its own - its record is the table row, U26, T4
and the section "Session 16 residue" below.


### Session 16 residue (facts not carried by the registers)
- `dumpbin /linkermember:1` on the built `winqor_api_user.lib` proves the help section publishes exactly three members, so
  the `user32.h:642-643` menu pair is absent from the binary as well as from the sources (T4).
- Help coverage: the SDK's help-id neighbourhood is 4 entry points and this header declares all 4 (2 wrapped, 2 not); the
  help command vocabulary (`HELP_CONTEXT` .. `HELP_TCARD_OTHER_CALLER`, `winuser.h:12333-12355`) and
  `HELPWININFO`/`HELPWININFOW` have **no QOR-side spelling anywhere**, which is why the only caller passes a bare
  `unsigned int` for `uCommand`; and the successor API Microsoft points to, `HtmlHelp`, is declared in **no** api_layer
  header at all, even though `comctl32.cpp:324` wraps `MenuHelp` - recorded under U21/U12 placement, not a TU fix.
- U16 measured **0 of 3** for this TU although the version guard is real (`winuser.h:9105` `WINVER >= 0x0400` + Desktop
  for the four help-id prototypes; `:12270` `#ifndef NOHELP` + Desktop for `WinHelpA/W`): the difference from
  `FLASHWINFO` (session 14) and `HDEVNOTIFY` (session 12) is that **no type these wrappers name is version-gated**, so a
  lower `WINVER` leaves the header well-formed. U16 counts *type* dependencies, not macro adjacency.
- Liveness: the three wrappers' only sites are `ui/gui/controllers/windowcontroller.cpp:785`, `:790`, `:795`
  (`SetContextHelpId`, `GetContextHelpId`, `Help` - declared `windowcontroller.h:284-286`), and those three methods have
  no call site in `src`/`examples`/`test`; the gui's own `WM_HELP` path (`ui/gui/view/handlers/interactive/help.cpp:46`)
  reads the id out of the `HELPINFO` payload, so it never goes through the wrapper. No U24 instance here - all three SDK
  returns are genuine success flags or plain values, unlike `FlashWindow`'s previous-state `BOOL`.
- **Two numbers a later session should re-run**: session 16's guard-scan count and its `git diff --stat` line were taken
  while the agent-side shell validator was rejecting most `run_commands` inputs, so treat those two as unconfirmed; every
  other number here was measured with a working shell.
- Bookkeeping at the time of writing: `help.cpp` (session 16) and this file were **uncommitted**; HEAD is `9ecbf581`
  (session 15). Session 12's `devicemanagement.cpp` was likewise uncommitted when its row was written, so a working-tree
  diff of this folder can show more than one file.


### How this file was consolidated (31 Oct 2026, before session 17)
Before: 2,151 lines / 237 KB (session-15 commit) plus session 16's uncommitted additions. After: **1,201 lines / 141 KB**
(`[System.IO.File]::ReadAllLines`, trailing blank lines included; this section is the last thing in the file).
The register sections (convention, pre-scan, session log, deferred tasks, method, ground rules, open items) were kept
verbatim; session 16's additions were folded back in (table row 16, task **T4**, item **U26**, the queue and the
next-session block, all of which a `git checkout` had momentarily reverted); and the per-TU narrative was moved verbatim
to `session-log-archive.md` (1,214 lines) and replaced by the census above.
**Completeness check, run mechanically:** every non-blank line of the session-15 commit (2,049 lines) was tested for
presence in `status.md` + `session-log-archive.md`. First pass: 23 not found - the superseded `help.cpp` pre-flags (with
session 16's confirmation/correction pass over them) and the superseded queue/"not started" bookkeeping rows; all 23 were
then archived verbatim in that file's last sections. Re-run: **0 lines lost**. A fact now appears once: either in a TU's
table row, in the U/G/P/T register entry it feeds, or in this consolidated section.


