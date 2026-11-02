# `mouse.cpp` (session 25, 2 Nov 2026) - narrative (verbatim)

**Scope:** 10 wrappers - the whole `user32.h:555`-`:565` `//Mouse functions` section; definitions now `mouse.cpp:97`-`:164`
in header order (`DragDetect`, `GetCapture`, `GetDoubleClickTime`, `GetMouseMovePointsEx`, `mouse_event`,
`ReleaseCapture`, `SetCapture`, `SetDoubleClickTime`, `SwapMouseButton`, `TrackMouseEvent`). TU grew 86 -> **167 lines**
(81-line TU comment block). **0 `user32.h` lines moved**, header still **695 lines**.

**Fixes: 11 lines, and this is the folder's biggest behavioural fix since U6.** (1) **All 10 bindings** -
`qor_pp_useswinapiAW` -> bare `qor_pp_useswinapi` at `:100`,`:107`,`:114`,`:121`,`:128`,`:135`,`:142`,`:149`,`:156`,`:163`;
(2) the **wrong stem** at the old `:82`, which bound the parameter **type** `LPTRACKMOUSEEVENT`; (3) **U14(b)** - the
`TrackMouseEvent` call site is now `Call< BOOL, LPTRACKMOUSEEVENT >` (`:164`). No other line touched.

**Binding (D13) - the inverse of the P2 class, and a whole-TU P1 hit.** No mouse stem has an A/W family: `user32.dll`
exports the bare names only - `DragDetect` `CB`/RVA `0002EC30`, `GetCapture` `125`, `GetDoubleClickTime` `152`,
`GetMouseMovePointsEx` `18E`, `mouse_event` `3F5`, `ReleaseCapture` `2FE`, `SetCapture` `320`, `SetDoubleClickTime` `339`,
`SwapMouseButton` `3A4`, `TrackMouseEvent` `3B3` (`tmp/_tmp_user32_exports.txt`, 1018 names, has **no** `<stem>A` or
`<stem>W` row for any of them) - and `winuser.h` declares each once with **no `#define <stem>` macro at all**
(`tmp/_tmp_mouse_class_s25.ps1`: `macro=False` 10/10, `P6-shaped 0`). `qor_pp_useswinapiAW` pastes the suffix itself
(`_NAME##W` at `kernel32.h:52`, `W` because `qor_pp_unicode` is on) without consulting the SDK, so `/EP` over the pre-fix
TU emitted `"DragDetectW" ... "LPTRACKMOUSEEVENTW"` (pp lines 453254-453317 of `tmp/_tmp_mouse_pp_pre_s25.txt`) - **10
names the DLL does not have**. `pFunc` is a function-local `static const`, so the NULL was cached for the process
lifetime (U5) and all ten threw `"Missing library function."` from `library.h:50`-`:53` on every call. Live probe
`tmp/_tmp_mouse_probe_s25.cpp` + `tmp/_tmp_run_mouse_probe_s25.bat` -> `tmp/_tmp_mouse_probe_s25.txt`: the 10 old names
**MISSING**, the 10 bare stems **RESOLVED** at the table RVAs, and `TrackMouseEventW`/`A`/`T` + `DragDetectA` **MISSING**
(discharges every alternative spelling). The probe needed `LoadLibraryW` - `GetModuleHandleW(L"user32")` fails with
`gle=126` in a bare console process (G5). Folder census **11 -> 1 unresolvable** of 428; the survivor is
`window.cpp:124 CreateWindow` (`tmp/_tmp_p1census_pre_s25.txt`, `_post_s25b.txt`).

**P5 (wrong stem) - its second instance, now 0 live.** The old `qor_pp_useswinapiAW(user32, LPTRACKMOUSEEVENT)` bound a
**type name**, so even a bare binding there would have asked `GetProcAddress` for `LPTRACKMOUSEEVENT`.
`tmp/_tmp_wrong_stem_scan.ps1`: 4 rows -> 3, all three known-benign (P3's `cursor.cpp:64`, U11's two unqualified
`resource.cpp` definitions).

**Signatures: 0 divergences, 0 parameter-name divergences** over 10/10 (`tmp/_tmp_name_divergence_scan.ps1 -Tu mouse.cpp`
-> `tmp/_tmp_namediv_pre_s25.txt`, 491 header declarations parsed). Against `winuser.h:1353`-`:1361`, `:2770`-`:2774`,
`:3643`-`:3647`, `:4273`-`:4277`, `:4279`-`:4283`, `:6102`-`:6110`, `:6961`-`:6965`, `:6967`-`:6971`, `:6973`-`:6977`,
`:8152`-`:8157`. Two notes: `SetDoubleClickTime`'s prototype gives its `UINT` **no name** (`:4283`), so `uInterval` is
this layer's own; `DragDetect` takes `POINT` **by value** as the SDK does, and after U6 `Library::Call` builds its

**U16: 3 of 10 prototypes guarded** - `GetMouseMovePointsEx` under `#if(WINVER >= 0x0500)` (`:1329`/`:1366`),
`DragDetect` under `#if(WINVER >= 0x0400)` (`:8107`/`:8162`), `TrackMouseEvent` inside two nested
`#if(_WIN32_WINNT >= 0x0400)` (`:2743`, `:2758`; closed `:2779`, `:2784`). The SDK's `#endif` **comments are crossed**
here: `:2756` reads `/* _WIN32_WINNT >= 0x0400 */` but closes the `#ifndef NOTRACKMOUSEEVENT` at `:2744` and gates only
the `TME_*` constants. `GetCapture`/`SetCapture`/`ReleaseCapture` sit in the **`DESKTOP | PC_APP`** block at `:6892` -
this item's first package-partition case - the other seven are Desktop-only. Instrument
`tmp/_tmp_mouse_guards_s25.ps1` -> `tmp/_tmp_mouse_guards_s25.txt`.

**Contracts:** `mouse_event` returns nothing; 3 returns are states not success flags (`SwapMouseButton` = were they
already swapped, `GetCapture` NULL = this window does not own the capture, `DragDetect` FALSE = no drag); the other six
are failure-indicating (`FALSE`, NULL, 0, -1). Two `cbSize` fields and `GetMouseMovePointsEx`'s `resolution`
(`GMMP_USE_*_POINTS`, `winuser.h:1347`-`:1348`) are caller-supplied and unvalidatable here. Per the standing U1/U2
policy: no `Win32ErrorHandler`, no `CheckReturn` (U7 stays open).

**U19: 9 LAYER / 7 INDIRECT / 0 BYPASS / 0 MEMBER** (`tmp/_tmp_mouse_bypass_s25.ps1` -> `_s25.txt`), unchanged since
session 20 - but the fix changes what those 9 are worth: every one (`i_m/mouse.cpp:39`,`:44`,`:49`,`:54`,`:59`,`:64`,
`:69`,`:74` + `windowcontroller.cpp:673`) is compiled into `winqor_gui` and every one used to throw. `TrackMouseEvent`'s
only through-layer caller is `windowcontroller.cpp:673`.

**U17: 0** (no A/W family, so no `T` suffix owed; folder census unchanged). **U26: 0** (no `TCHAR` parameter, and neither
`TRACKMOUSEEVENT` `:2763`-`:2768` nor `MOUSEMOVEPOINT` `:1334`-`:1339` has a `TCHAR` field). **U25: 0** (no `const` in
the ten declarations; `::POINT` qualified identically on both sides). **U4/U11/U13: 0** - 10/10 declared==defined, all
`static`, all qualified. **U15: not recorded** (D14); reachability measured only because P1/P5 needed latent-vs-live.

**Gate:** `tmp/_tmp_cl_all_s25c.log` - 38/38 echoes at `/W4`, exit 0, 0 `Cnnnn:` diagnostics, logged 22:18:27 **after**
the final `mouse.cpp` edit at 22:14:54 (`_tmp_cl_all_s25b.log` at 22:06:32 predates that last citation fix). `cl /EP`
post3 `tmp/_tmp_mouse_pp_post3_s25.txt` (453,402 lines) vs pre-fix (453,321): **0 removed, 81 added, every added line
blank** -> the comment pass emitted nothing; post2 vs post3 = **0/0**. Classifier over post3: 10 literals, **0 outside the
export table**. TU: 167 lines, 0 tabs, 0 trailing-space lines, 0 non-ASCII, longest 144. Close-out verifier
`tmp/_tmp_s25_verify.ps1` -> `tmp/_tmp_s25_verify.txt`.

**Registers touched:** `prescan.md` (P1 11 -> 1, P5 0 live), `census.md` (folder totals + the gate), `u14.md` (425/3),
`u16.md` (3/10 + the crossed-comment and PC_APP facts), `u19.md` (LAYER column nominal while the bindings were NULL),
`tasks.md` (queue 12, hand-offs). **Instruments added:** `_tmp_mouse_class_s25.ps1`, `_tmp_mouse_probe_s25.cpp` +
`_tmp_run_mouse_probe_s25.bat`, `_tmp_mouse_pp_s25.bat`, `_tmp_mouse_guards_s25.ps1`, `_tmp_mouse_bypass_s25.ps1`,
`_tmp_s25_verify.ps1`; re-used `_tmp_kb_p1census_s20.ps1`, `_tmp_wrong_stem_scan.ps1`, `_tmp_u14_census.ps1`,
`_tmp_tsuffix_census.ps1`, `_tmp_name_divergence_scan.ps1`, `_tmp_compile_one.ps1`, `_tmp_cl_all.bat`.

`__stdcall` `fPtr` from the arguments the site spells (`library.h:43`-`:63`), so it matches on both arches.
