# Folder-level leads - the queue is closed, T8 step 6c is next (6 Nov 2026)

Written wholesale at the T8 step 6b close. All 38 review-queue sources are `closed` and `pointer.cpp` (6a) and `dpi.cpp`
(6b) are closed with them, so there is no next TU: this file lists what a further session in this folder picks up, and its
lead is **T8 step 6c - the Touch / gesture family, 14 names**.
**Write-once**: read it once at boot, edit it never, replace it wholesale if the focus changes.

## Current folder figures (re-measured 6 Nov 2026 with `_tmp_u18_coverage4.ps1` + `_tmp_u18_buckets.ps1`)

**40 sources / 580 members / 483 bindings / 1,018 export names / 703 covered / 62 declared-and-unwrapped raw / 253 with no
prototype.** Raw 62 minus the 7 excluded superseded twins (`Get/SetClassLongA/W`, `SetWindowsHookA/W`,
`UnhookWindowsHook`) and the 1 instrument artifact (`SetWindowsHookExAW`, no prototype) = **54 net backlog**: Touch/gesture
**14**, Display config 7, Shutdown/power 3, Misc 24, held-open legacy 6. Per-name dispositions:
`registers/u18-coverage.md`.

## Step 0 - re-derive the gap list before measuring anything

Every tranche pre-scan filters `tmp/_tmp_u18_gap_sdk.txt`, which `tmp/_tmp_u18_coverage4.ps1` writes, so **run that first**
- a stale gap list feeds the previous family's names back to you. Then copy `tmp/_tmp_t8_6c_pre.ps1` (the 6b pre-scan with
its bucket filter changed to `Touch|Gesture` and a new output path): per name it prints the export-table row, the
`winuser.h` declaration line, the prototype text, the guard stack open at that line, the macro count, the A/W siblings and
whether the stem is already declared in `user32.h`.

## T8 step 6c - the 14 Touch / gesture names, with the SDK facts already measured

**The bucket reconciles with nothing shed, and with a double-wrap trap.** 19 exports match
`Touch|Gesture|HitTesting`:

- **4 are already wrapped** - `GetPointerTouchInfo` (`winuser.h:6574`), `GetPointerTouchInfoHistory` (`:6581`),
  `GetPointerFrameTouchInfo` (`:6589`), `GetPointerFrameTouchInfoHistory` (`:6597`). They went into `pointer.cpp` with the
  6a Pointer tranche because they are `POINTER_TOUCH_INFO` members; a name-based bucket on `Touch` finds them again, so
  check `user32.h` before wrapping.
- **1 has no prototype** - `MITSynthesizeTouchInput` (0 hits in `winuser.h`), out by the D18 documented-set rule.
- **14 remain**, and all 14 are exported **and** declared, with **0** function-like macros, **0** `A`/`W` siblings and
  **0** already declared in `user32.h`. So all 14 bind **bare `qor_pp_useswinapi`** under D13 and `user32.h:9-17` needs
  **no new `#undef`** - `RegisterTouchWindow` / `IsTouchWindow` have no `W` twin, unlike `ChangeMenu` in the Misc bucket.

Three guard groups, and the layer adds **no `#if`** - `WINVER` defaults to `0x0A00` (`sdkddkver.h`) and nothing in this
build sets it:

- **Touch input + window registration - 5 names, `#ifndef NOUSER` (`:64`) inside `#if(WINVER >= 0x0601)`**:
  `GetTouchInputInfo` `:6243`, `CloseTouchInputHandle` `:6252`, `RegisterTouchWindow` `:6272`,
  `UnregisterTouchWindow` `:6279`, `IsTouchWindow` `:6285`.
- **Touch injection + hit testing - 4 names, `NOUSER` inside `#if(WINVER >= 0x0602)`**: `InitializeTouchInjection`
  `:6485`, `InjectTouchInput` `:6492`, `RegisterTouchHitTestingWindow` `:6726`,
  `PackTouchHitTestingProximityEvaluation` `:6773`.
- **Gesture - 5 names, NO `NOUSER` guard at all**, only `#if(WINVER >= 0x0601)` plus the desktop partition:
  `GetGestureInfo` `:15770`, `GetGestureExtraArgs` `:15783`, `CloseGestureInfoHandle` `:15801`, `SetGestureConfig`
  `:15869`, `GetGestureConfig` `:15884`.

All 14 also sit inside `#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)`. Export ordinals from the folder's capture:
`51` CloseGestureInfoHandle, `52` CloseTouchInputHandle, `15C` GetGestureConfig, `15D` GetGestureExtraArgs, `15E`
GetGestureInfo, `1D4` GetTouchInputInfo, `214` InitializeTouchInjection, `21B` InjectTouchInput, `245` IsTouchWindow,
`2A5` PackTouchHitTestingProximityEvaluation, `2F9` RegisterTouchHitTestingWindow, `2FA` RegisterTouchWindow, `33F`
SetGestureConfig, `3CE` UnregisterTouchWindow.

## Signature traps in this family (measure each one, do not recall it)

- **`PackTouchHitTestingProximityEvaluation` returns `LRESULT`**, not `BOOL` - the only unusual return in the bucket, and
  the one a `CheckReturn`-shaped reflex would get wrong.
- **`GetTouchInputInfo` `:6243`, `SetGestureConfig` `:15869` and `GetGestureConfig` `:15884` have multi-line prototypes
  with inline parameter comments.** The pre-scan prints the first line only; open the header and copy the whole prototype,
  since D18 keeps the SDK's parameter names.
- **`GetGestureConfig`'s `pcIDs` is `_Inout_ PUINT`** (in/out buffer count) while **`SetGestureConfig`'s `cIDs` is `_In_
  UINT`** - same shape as 6a's `GetPointerFrame*` count parameters: the buffer contract stays the caller's, no
  `CheckReturn`, semantics in the banner instead (D15 / D18 pass-through).
- **`IsTouchWindow`'s second parameter is `_Out_opt_ PULONG pulFlags`** - nullable out, so a `nullptr` is legal.
- **`HTOUCHINPUT` and `HGESTUREINFO` are `DECLARE_HANDLE`s the OS hands out** (`WM_TOUCH` / `WM_GESTURE` `wParam`), and
  `CloseTouchInputHandle` / `CloseGestureInfoHandle` close handles the caller never created. State that caller contract
  in the TU, exactly as `dpi.cpp` stated it for the `Get*ForProcess` handle.
- Types needed: `TOUCHINPUT`, `GESTURECONFIG`, `GESTUREINFO`, `TOUCH_HIT_TESTING_INPUT`,
  `TOUCH_HIT_TESTING_PROXIMITY_EVALUATION`, `POINTER_TOUCH_INFO` (**already used by `pointer.cpp`**) - every one arrives
  through `kernel32.h:12`'s `#include <Windows.h>`; declare no type of your own.

## Placement, TU and gate shape

- **`user32.h`**: a new `//Touch and gesture functions` section after `//DPI awareness functions` (which opens `:603` and
  ends `:627`). No stem in this family is already declared, so nothing pulls a declaration beside a wrapped sibling - the
  U12 question resolves by placement, as it did for 6a (`:569`) and 6b (`:603`). Expect the section comment plus 14
  declarations, and remember a header insert shifts every later self-citation in `user32.h`.
- **New TU `touch.cpp`** (family-named, like `pointer.cpp` and `dpi.cpp`), 14 definitions in header order, each
  `qor_pp_fcontext;` + `Win32ErrorHandler` **not** used (U1 stands) + bare `qor_pp_useswinapi(user32, Name)` +
  `Library::Call<...>` with explicit template arguments (U14(b)). No `voidCall` - all 14 return something.
- **`CMakeLists.txt`**: `dpi.cpp` is currently last (`:42`); `touch.cpp` joins it → **41 sources**, folder leg **41/41**,
  all-libraries leg **109** echoes.
- **Gate**: standalone `/W4` exit 0 → folder exit 0 → `/EP` binding-literal check (expect 14 literals, module `user32`,
  each byte-equal to an export name) → all-libraries pre/post **diff** because `user32.h` changes. Date everything by
  `tmp/obj/<tu>.obj` newer than the source.
- **Expected re-measure after the tranche**: 594 members / 497 bindings / 717 covered / 48 declared-and-unwrapped raw /
  **40 net backlog** (Display config 7, Shutdown/power 3, Misc 24, held-open legacy 6).

## Consumers: none

0 hits for all 14 names across the 3,228 `.h/.hpp/.cpp/.cxx` of `src`, `examples` and `test`
(`tmp/_tmp_t8_6c_consumers.ps1`, the single-pass consumer scan). So there is no U19 LAYER/BYPASS split to book - and
`registers/u19.md` is **FROZEN at session 34**, so any split a later tranche finds is booked in the TU banner, never in the
register.

## What comes after 6c

1. **6d Display config - 7 names**: `QueryDisplayConfig`, `SetDisplayConfig`, `GetDisplayConfigBufferSizes`,
   `DisplayConfigGetDeviceInfo`, `DisplayConfigSetDeviceInfo`, `GetDisplayAutoRotationPreferences`,
   `SetDisplayAutoRotationPreferences`. The heaviest by-value structs in the backlog (`DISPLAYCONFIG_PATH_INFO` /
   `DISPLAYCONFIG_MODE_INFO` arrays behind `*pathInfo` / `*modeInfo`), so this is the tranche where the buffer and count
   contract matters most - `GetDisplayConfigBufferSizes` exists precisely to size them, and the two auto-rotation members
   use the `DISPLAYCONFIG_...` enum, not `DWORD`.
2. **6e Shutdown / power - 3 names**: `CancelShutdown`, `RegisterSuspendResumeNotification`,
   `UnregisterSuspendResumeNotification` (the notification pair takes an `HNOTIFICATION_TYPE` handle).
3. **Misc 24**, then **held-open legacy 6** (`DragObject`, `GetWindowWord`, `SetWindowWord`, `SetMessageQueue`,
   `wsprintfA`, `wsprintfW`). `tmp/_tmp_u18_buckets.ps1` prints the current family lists and the 5 A/W pairs that are both
   unwrapped (`ChangeMenu`, `GetClassLong`, `SetClassLong`, `SetWindowsHook`, `wsprintf`); the two legacy groups still
   **contain the 8 names already dispositioned excluded**, so read `registers/u18-coverage.md` before touching them, and
   remember `qor_pp_useswinapiAW` is the right spelling only where an A/W family exists (D13).

## Measuring reminders from the last two tranches (each of these cost a re-run)

- **Compile the signatures instead of reading them**: `tmp/_tmp_t8_6b_protos.cpp` + `tmp/_tmp_t8_6b_probe.bat` type-check
  every published signature against the SDK and prove every declaration live, in one `/W4` exit-0 compile, with no QOR
  define and no version macro. Keep the function-pointer typedefs - `void*` casts silence the check.
- **The all-libraries exit code is 2** (the standing `C1083` at `opengl/wgl.cpp:31`). Capture it inside a `.bat` - an
  inline `cmd /c "call x.bat > log & echo %ERRORLEVEL%"` expands at parse time and writes a false 0 - and read the
  **diff** against a pre-fix capture, never the exit code.
- **A raw `Select-String` hit is not a call**: read the enclosing comment state first (`ui/gl/windowimpl.cpp:550` looked
  like a live bypass and sits inside a `/* */` that closes 751 lines later), and add `-CaseSensitive` to any
  family-shaped pattern (`Dpi` alone matched `CallNamedPipe` and `SetBackgroundPixmap`).
- **Guard stacks are a forward pass** with an explicit stack; the backward `pop` form over `0..-1` silently iterates zero
  times. And a guard stack with no `WINVER` row means the declaration really is unguarded - it is not a scan miss.
- **Pair the new section with `tmp/_tmp_t8_6b_pair2.ps1`, not `_tmp_t8_6b_pair.ps1`** - the latter ends the section at its
  first `//` line, so a section with an in-section comment reads as truncated and reports a false
  `defined-but-not-declared` list. Count folder totals comment-stripped (`_tmp_t8_6b_counts.ps1`): raw text reads
  536 bindings against the true 483 because TU banners name the binding macros in prose.
- **Pre-flag line numbers age**: 6b's preflags cited the two dialog enums at their closing braces and `DPI_AWARENESS` at
  its first enumerator. Re-derive typedef lines from the header, and re-derive `user32.h` self-citations after a header
  insert rather than offsetting them by hand.
