# narrative: coordtransform.cpp, session 8

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


