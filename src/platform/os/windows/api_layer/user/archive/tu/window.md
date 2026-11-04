# TU narrative - `window.cpp` (session 33, 4 Nov 2026)

The folder's largest TU: **85 wrappers** over **615 lines** before the session, **692 after** (a 67-line in-file contract
block at `:16`-`:82` plus 4 short body comments). It carries the *whole*
`user32.h:129-212` "Window functions" section (84 declarations, **all 84 `static`**) **plus `GetDpiForWindow`**, whose
declaration sits outside that range at `user32.h:639` (System Information section) - the only definition in the folder
whose declaration is in a different header section, so it is invisible to any section-range-scoped instrument.
Census.md's "50 wrappers / 270 lines" row for this file was wrong (corrected 4 Nov 2026 before the session).
Post-session hygiene: 692 lines = 79 comment + 90 blank + 523 code, longest line **213**, `qor_pp_fcontext` 85,
binding macros 85 (75 `qor_pp_useswinapi` + 10 `qor_pp_useswinapiAW`), `Library::Call<` 84 + `Library::voidCall<` 1
(the void one is `SwitchToThisWindow` `:643`, which the SDK really does declare `void`), 0 `Win32ErrorHandler`,
0 `CheckReturn`.

## Fixes made (6 code edits, all in this TU + its 4 header mirrors)

Line numbers below are **pre-fix** unless marked `(final)`. Post-block map (`tmp/_tmp_s33_verify.ps1`): the first wrapper
`AdjustWindowRect` is `:84`, the last `GetDpiForWindow` `:685`.

1. **`CreateWindowT` binding - run-breaking, the folder's last P1 instance.** `qor_pp_useswinapiAW(user32, CreateWindow)`
   at pre-fix `:124` pastes `W` and stringizes the result, so `GetProcAddress` was handed `"CreateWindowW"`. `user32.dll`
   exports **no** `CreateWindow` and **no** `CreateWindowW` (`tmp/_tmp_user32_exports.txt`: `CreateWindowExA` 75,
   `CreateWindowExW` 76, `CreateWindowInBand/InBandEx/Indirect`, `CreateWindowStationA/W` - nothing else), because
   `CreateWindow`/`CreateWindowW` are *function-like macros* in the SDK (`winuser.h:4464`/`:4468`) that forward to
   `CreateWindowExA/W` with `dwExStyle = 0`. Every call would therefore have thrown `Library::` "Missing library
   function." Now binds `CreateWindowEx` (AW) and passes the same zero extended style, with a comment giving the reason.
   `/EP` proof, before: 85 binding literals of which **1 not in the export table - `MISSING CreateWindowW`**; after:
   85 lines, 84 distinct names, **0 missing**, 0 `…WW`, 0 `…T`, module literal `L"user32"` 85/85
   (`tmp/_tmp_win_pp_s33.bat` + `_tmp_win_ppclass_s33.ps1`).
2. **U18 closed - `LogicalToPhysicalPoint` (`:437`) and `PhysicalToLogicalPoint` (`:460`).** Were `void` +
   `Library::voidCall<HWND, ::LPPOINT>`; the SDK returns **BOOL** (`winuser.h:9526`, `:9533`, `WINVER >= 0x0600`) and
   FALSE is the "point is not in a physical/logical display area" case the DPI-aware caller must handle. Now `BOOL` +
   `Library::Call<BOOL, HWND, ::LPPOINT>`; header mirrors `user32.h:188`, `:191`. This closes the U18 entry opened in
   session 8 over these two members.
3. **`SetWindowPlacement` (`:525`) parameter constness.** SDK: `_In_ const WINDOWPLACEMENT *` (`winuser.h:4775`);
   layer said `::WINDOWPLACEMENT*`, i.e. it dropped a const the SDK promises. Now `const ::WINDOWPLACEMENT*` in both
   definition and `user32.h:200`.
4. **`TileWindows` (`:581`) parameter constness.** SDK: `_In_opt_ CONST RECT *` (`winuser.h:12240`); layer said
   `::RECT*`. Now `const ::RECT*` in both definition and `user32.h:208`.
5. **U14(b) - the folder's only three argument-deducing call sites, all here.** `CloseWindow` (`:118`),
   `GetLastActivePopup` (`:259`), `GetWindowModuleFileNameT` (`:322`) used `Call<RETURN>(pFunc, …)` with no parameter
   types. Now explicit. Folder-wide census after: **explicit 428 / bare 0** (was 425/3).
6. Header edits in service of 2-4 (`user32.h:188`, `:191`, `:200`, `:208`) - no other declaration changed.

## Verification

Single-TU `/W4` compile clean; **full-folder gate** (`tmp/_tmp_cl_all.bat`, 38 TUs) exit 0 with **0** `warning C` /
`error C` lines; consumer-side probe (`tmp/_tmp_win_consumer_s33.bat` compiling
`ui/gui/controllers/windowcontroller.cpp`, the TU that calls 60+ of these members) `CL_EXIT=0` after the signature
changes - the only warnings there are pre-existing `C4100`/`C4189` in the `ui` layer. Post scans: P1 census
**428 bindings / 0 unresolvable**; U14 census **428 explicit / 0 bare**; wrong-stem scan now flags **2** -
`cursor.cpp:64 CopyCursorT -> CopyIcon` (pre-existing deliberate alias) and this TU's
`CreateWindowT -> CreateWindowEx` (the deliberate alias above), so P5's 428 tally is unchanged.

## Binding / SDK facts measured for this TU

* 85 bindings = **75 direct + 10 AW**; after the fix **85/85 literal-exported** (`tmp/_tmp_s33_pre.ps1` section C). The
  ten AW members: `CreateWindowT`, `CreateWindowExT`, `FindWindowT`, `FindWindowExT`, `GetAltTabInfoT`,
  `GetWindowModuleFileNameT`, `GetWindowTextT`, `GetWindowTextLengthT`, `RealGetWindowClassT`, `SetWindowTextT`; the SDK
  stem macro is live for all nine A/W families (`SetWindowText` macro at `winuser.h:8991` etc.), i.e. the G1 case, not P2
  churn.
* SDK pairing (section D, `winuser.h` 10.0.26100): **1 divergence** - `GetGUIThreadInfo` p1 `PGUITHREADINFO` (SDK) vs
  `LPGUITHREADINFO` (layer), the same typedef, so recorded, not churned. **2 members with no SDK prototype**:
  `CascadeChildWindows` (`:79`), an undocumented user32 export the layer wraps (compare `InternalGetWindowText` (`:360`),
  undocumented-but-declared), and `CreateWindow` itself (the SDK macro, fix 1). Parameter *names* diverge from the SDK on
  6 members (`ChildWindowFromPointEx`, `GetGUIThreadInfo`, `GetWindowModuleFileNameT`, `InternalGetWindowText`,
  `RealGetWindowClassT`, `SwitchToThisWindow`) - published names, left alone.
* **U16 floors: 34 of 85** members sit under a version floor: 11× `WINVER >= 0x0500`, 7× `WINVER >= 0x0400`,
  5× `_WIN32_WINNT >= 0x0500`, 4× `WINVER >= 0x0600`, 2× `_WIN32_WINNT >= 0x0600` + `WINVER >= 0x0500`, and one each
  `WINVER >= 0x0605` (`AdjustWindowRectExForDpi`), `WINVER >= 0x0605` + `0x0500` (`GetDpiForWindow`),
  `_WIN32_WINNT >= 0x0501`, `_WIN32_WINNT >= 0x0600`, `_WIN32_WINNT >= 0x0501` + `0x0500`. Guard names new to the review:
  **`NOWINABLE`** (11 members), **`NODEFERWINDOWPOS`** (3), **`NOMDI`** (3 - `ArrangeIconicWindows`, `CascadeWindows`,
  `TileWindows`; `ArrangeIconicWindows` has no version floor, so `NOMDI` alone removes it); `SoundSentry`
  (`:635` final) is the only member also in `WINAPI_PARTITION_PC_APP`. 83/85 DESKTOP, 9 also GAMES
  (`AdjustWindowRect`, `AdjustWindowRectEx`, `CreateWindowExT`, `DestroyWindow`, `GetClientRect`, `IsWindowVisible`,
  `MoveWindow`, `SetWindowPos`, `ShowWindow`). The per-member dump marks **2 rows `floor[PARSER MISS]`** -
  `CascadeChildWindows` and `CreateWindowT` - because they have no SDK declaration to read a guard from, not because the
  walk failed.
* **U26: 8 caller-side structs reach this TU through `reinterpret_cast`** in `windowcontroller.cpp` (`Point`, `Rect`,
  `Size`, `BlendFunction`, `WindowPlacement`, `WindowInfo`, `UpdateLayeredWindowInfo`, `AltTabInfo`). Probe
  `tmp/_tmp_win_mirror_s33.cpp` + `.bat`, **x64 and x86**: every `sizeof` equal and **every field offset equal**
  (`WINDOWPLACEMENT` 44, `WINDOWINFO` 60, `UPDATELAYEREDWINDOWINFO` 80 x64 / 40 x86, `ALTTABINFO` 40) - the casts are
  layout-sound. Residual contract the layer cannot state (U23/U27 class): the SDK requires
  `lpwndpl->length == sizeof(WINDOWPLACEMENT)` before `Get/SetWindowPlacement`, and `WindowPlacement::length`
  (`windowcontroller.h:86`) has **no initializer** - the same shape as session 30's uninitialized `ScrollInfo` fields.
  `WindowController::GetPlacement` has **0 in-tree callers**, so the hazard is latent and the fix belongs to the
  published `winqor_gui` header, not to this TU.
* **U19**: 340 stem variants scanned - 71 through-layer (`User32::`), 49 intermediate-qualified, 49 "SDK bypass",
  74 member-call. Of the 49, only two groups are genuine SDK calls that skip this layer:
  `ui/gl/helperwindow.cpp:59`,`:75` (`::CreateWindowExW`, `::ShowWindow`) and `ui/gl/windowimpl.cpp` (10 sites -
  `AdjustWindowRectEx` ×5, `AdjustWindowRectExForDpi` ×5); the rest are same-named non-SDK members
  (`WindowController::GetParent`, `Element::GetParent`, `Console::GetWindow`, …) or the raw-SDK
  `examples/experimental/.../retropad` (15 sites). No `TrackPopupMenu` hit in this TU - that lead belonged to `menu.cpp`
  (session 22) and is discharged here.
* **U1/U2**: 0 `Win32ErrorHandler`, 0 `CheckReturn` (the `returncheck.h` include is unused in this TU).
  **U25**: **1 `CONST` token on 1 code line** in this TU - `CascadeWindows`'s call list at `:158` (final) spells
  `CONST ::RECT*` while that member's own definition at `:154` spells `const ::RECT*`; the file's other **11 lowercase
  `const` tokens over 8 code lines** are `:154`, `:158`, `:593`, `:597`, `:649`, `:653`, `:664`, `:668` (final).
  `user32.h:129`-`:212` has **0** `CONST` tokens and 4 lowercase-`const` lines. Measured case-sensitively -
  `tmp/_tmp_s33_verify.ps1`'s `\bCONST\b` test is **case-insensitive** and reports "8" here.
  **Call-paren**: U14 census **85 tight / 0 spaced** (the census reads the character just after `Call<`). Two sites have
  a space **before the closing `>`** - `:206` (`CreateWindowT`) and `:264` (`EnumWindows`) - which
  `_tmp_s33_verify.ps1`'s P5-shaped regex flags but which is not U14's spaced class; left alone (D15).
  Comment lines 3 -> **79** (the 67-line contract block plus the 4 body comments); an earlier draft of this narrative said
  "12 comment lines, no closure block" - that was the pre-block state, corrected during close-out.

## Verdict

**5 defect classes fixed with 6 edits** (1 run-breaking binding, 1 return-type contradiction ×2 members, 2 dropped
`const`s, 3 argument-deducing call sites). Nothing else in the 85 wrappers is wrong at the binding, signature or
call-site level. Everything else found here is record-only: the `GetGUIThreadInfo` `P`/`LP` typedef spelling, 6 SDK
name divergences, the cross-section `GetDpiForWindow` declaration, the `CascadeChildWindows` / `CreateWindow`
"no SDK prototype" facts, the uninitialized `WindowPlacement::length` caller contract, and the `ui/gl` SDK-bypass
sites.
