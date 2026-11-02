# `monitor.cpp` (session 24, 2 Nov 2026) - narrative (verbatim)

**Scope:** 5 wrappers - the whole `user32.h:587`-`:592` `//Monitor functions` section; definitions
`monitor.cpp:127`-`:155` in header order. TU grew 51 -> **162 lines** (111-line TU comment block). **0 `user32.h` lines
moved**, so no `user32.h:NNN` citation in the record shifted; the header stays **695 lines**.

**Fixes: 0.** The cleanest TU of the review so far - every D13/U14/U18/U25/U27 check passed as found, and nothing in the
section needed changing. The value of the session is measurement, not repair: the section was never audited before.

**Binding (D13):** 4 bare + 1 `AW`, and both halves are correct as written. `tmp/_tmp_mon_class_s24.ps1` (clone of
`_tmp_msg_class_s23.ps1`) classifies all five stems three ways against the export table (`tmp/_tmp_user32_exports.txt`,
1018 names), `<stem>A`/`<stem>W` presence and whether `winuser.h` defines a `#define <stem>` macro: `EnumDisplayMonitors`
(ordinal `FE`, RVA `0002ED90`), `MonitorFromPoint` (`28E`), `MonitorFromRect` (`28F`), `MonitorFromWindow` (`290`) are
single exports with no A/W spelling and no macro, so bare is the only spelling that could resolve; `GetMonitorInfo` is an
A/W family (`18C`/`18D`) with **no bare export**, so the `qor_pp_useswinapiAW` at `:137` is **load-bearing** - a bare
binding would ask for a name the DLL does not have and `pFunc` would cache NULL for the process lifetime (G5 + U5).
**0 P2, 0 P6** here (`tmp/_tmp_mon_class_pre_s24.txt`, `..._post_s24.txt` - identical). Live probe
`tmp/_tmp_mon_probe_s24.cpp` + `tmp/_tmp_run_mon_probe_s24.bat` -> `tmp/_tmp_mon_probe_s24.txt`: the five emitted literals
all `RESOLVED` at the table RVAs, while bare `GetMonitorInfo`, every `*T` spelling and the non-existent
`GetMonitorInfoExW/A` are `MISSING`; the probe had to `LoadLibraryW` first (`GetModuleHandleW` gle=126 - G5). Wrong-stem
scan (`tmp/_tmp_wrongstem_s24.txt`): 0 `monitor.cpp` rows (4 folder rows, unchanged). Folder census unchanged at
**11 unresolvable of 428** with no `monitor.cpp` row (`tmp/_tmp_p1census_pre_s24.txt`, `_post_s24.txt` and
`_post_s24b.txt` 18:55 - all three identical over this TU).

**Comment-only proof:** `cl /EP` before and after the 111-line block (`tmp/_tmp_mon_pp_pre_s24.txt` 14:56 vs
`tmp/_tmp_mon_pp_post_s24b.txt` 18:57) differ by **111 lines, all of them blank, 0 removed, 0 non-blank added**
(453,286 -> 453,397) - `/EP` strips the comments but keeps one output line each, so the emitted code is identical.
Cheapest neutrality proof of the review; instrument `tmp/_tmp_mon_pp_s24.bat`. `_tmp_mon_pp_final_s24.txt` (18:22) and
`_tmp_mon_pp_post_s24.txt` (18:12) both predate the last `monitor.cpp` edit (18:55) and are not the post state.

**Signatures:** **0 return-type and 0 parameter-type divergences** over all five, against `winuser.h:13607`-`:13612`
(`MonitorFromPoint`), `:13614`-`:13619`, `:13621`-`:13626`, `:13684`-`:13689` / `:13690`-`:13695`
(`GetMonitorInfoA/W`), `:13704`-`:13711` (`EnumDisplayMonitors`); the parameter names match the SDK's too.
`MonitorFromPoint` takes `POINT` **by value** exactly as the SDK does - after the U6 fix `Library::Call` builds its
`fPtr` from the template arguments the site spells and qualifies it `__stdcall` (`library.h:43`-`:63`), so the by-value
struct and the `MONITORENUMPROC` callback travel through a matching type on both arches. All five `Call<>` lists are
explicit (U14b) and each restates its own signature - no U14 instance, unlike `mdi.cpp`'s contradicting list.

**U16: 6 of 6 - the first TU whose entire section shares one guard.** `#if(WINVER >= 0x0500)` at `winuser.h:13594` owns
the whole multimonitor block, over the Desktop partition (`:13605`, `:13638`). A consumer pinning `WINVER < 0x0500` loses
all 6 prototypes while the 5 declarations and 6 exports stay, i.e. the section becomes wholly uncallable through the
header. Nothing in `cmake/`, the root `CMakeLists.txt`, `CMakeSettings.json` or `src` defines `WINVER`/`_WIN32_WINNT`/
`NTDDI_VERSION` (0 hits), so no pin - session 5's deferral stands (`tmp/_tmp_mon_guards_s24.ps1` -> `.txt`).

**U21 sixth instance, the largest region yet:** `winuser.h` has **no `NOMONITOR` macro at all**, so this family has no
opt-out marker, and the guard at `:13594` owns the region to `:14905` - **45** distinct declaration heads, only 6 of them
monitor. Who wraps the other 39 was **measured, not guessed** (`tmp/_tmp_mon_region_s24.ps1` -> `tmp/_tmp_mon_region_s24.txt`,
which takes all 45 heads to `user32.h` and over the 38 TUs): **6 heads here** (the five wrappers cover all six), **17 in
seven other TUs** - `window.cpp` 8, `accessibility.cpp` 4, one each in `combobox.cpp`, `cursor.cpp`, `keyboard.cpp`,
`menu.cpp`, `scrollbar.cpp` - and **22 with no wrapper anywhere** (the DPI-awareness-contexts family, the
`GetAltTabInfo`/`RealGetWindowClass`/`GetWindowModuleFileName` A/W pairs, `GetListBoxInfo`, `InheritWindowMonitor`). Those
23 non-monitor declarations sit in **nine** header sections, so one SDK region feeds ten sections across eight TUs and no
single TU sees more than 8 of its 45 heads. An earlier draft of this entry claimed "34 of the 39 belong to `window.cpp`";
that was an unmeasured guess from the region print and is superseded by the numbers above. Unlike
`keyboard.cpp`/`devicecontext.cpp`, this section's label and its contents agree. **Hand-off with teeth:** `GetListBoxInfo`
is declared at `user32.h:548` with **no definition in any TU** and no call site in `src`/`examples`/`test`, while
`user32.dll` exports it (ordinal `174`, RVA `0002EFF0`) - a U4 instance for the `listbox.cpp` session, recorded in `u21.md`
so it is not lost.

**U19: 8 LAYER / 0 INDIRECT / 4 BYPASS / 0 MEMBER over the 8 spellings of the five stems - and the first BYPASS found in
built code.** All five wrappers are live through the layer (`i_m/monitor.cpp:56`, `:62`, `:187`, `:228`, `:236`, `:242`,
`:248` plus `windowcontroller.cpp:679`, all compiled into `winqor_gui` at `ui/gui/CMakeLists.txt:25`/`:27`). Three bypass
rows are the usual unbuilt kind (`GetMonitorInfoW` thunks at `ui/gl/monitorimpl.cpp:79`, `ui/gl/windowimpl.cpp:435`,
`:1902` - `winqor_gl` is commented out at `ui/CMakeLists.txt:4` and `monitorimpl.cpp` is in no source list). The fourth is
new: `i_m/monitor.cpp:163` calls the `GetMonitorInfoW` import **from the same TU** that calls `User32::GetMonitorInfoT` at
`:56`/`:62`, so U19's defining shape - one entry point reached both through the layer and past it - is now measured live
and in built code. Recorded, not fixed (collapsing it is a `winqor_gui` change). The callback convention is the SDK's own:
`::MONITORENUMPROC` (`winuser.h:13702`, `CALLBACK`) is the published parameter type, so no `__cdecl`/`__stdcall` mismatch
arises on either arch.

**U26 fourth instance - the mirror question, and this time the mirror is sound.** `user32.h:588`-`:592` name no
TCHAR-bearing type, so unlike `GetIconInfoExT` the section cannot propagate the include-order width through its own
surface; but `GetMonitorInfo` is one contract with two struct shapes chosen by `cbSize` (`winuser.h:13640`-`:13682`),
there is **no `GetMonitorInfoEx*` export** (probe: both `MISSING`), and `qor_pp_useswinapiAW` pastes `W` unconditionally.
Measured for x64 and x86 (`tmp/_tmp_mon_mirror_s24.{cpp,bat}`): the caller mirror `MonitorInfo` (`i_m/monitor.h:47`-`:53`)
is **layout-identical to `MONITORINFO` on both arches** (40 bytes, `cbSize@0 rcMonitor@4 rcWork@20 dwFlags@36`) because it
spells SDK-width types - which is exactly what `message.h` does *not* do - and `MonitorInfoEx` (`:55`-`:58`) is
**104 bytes = `MONITORINFOEXW`** (`szDevice@40`, 32 `wchar_t`, `DeviceNameSize = 32` at `common/constants.h:72`). In the
narrow world the same source is **72 bytes = `MONITORINFOEXA`** against an export that still fills the 104-byte `W` shape:
the `icon.cpp` overrun with the roles reversed, and one switch wide. `Point`/`Rect` are byte-identical to `POINT`/`RECT`
on both arches, so the by-value `POINT` (`monitor.cpp:141` declaration, `:145` the `Call<>` list that passes it) and the
caller's casts at `i_m/monitor.cpp:236`/`:242` are
sound. **The one genuine x86 divergence in the section:** the caller's `MonitorEnumProc` (`monitor.h:38`) passes
`long long` where `MONITORENUMPROC` passes `LPARAM` - 8 bytes against 4 on x86 - and the cast hiding it is at
`i_m/monitor.cpp:187`. Latent (only `x64-Debug` is configured); the fix belongs to the published `winqor_gui` header.

**U25: 0 `CONST`, plus a fifth spelling variant - mixed `::` qualification inside three lines.** `:588` qualifies
`::MONITORENUMPROC` but leaves `LPCRECT` bare, `:590` qualifies `::POINT` while `:591` leaves `LPCRECT` bare, and the
`.cpp` mirrors the header exactly (`monitor.cpp:127`-`:159`). Not a defect, and `tmp/_tmp_sigcmp.ps1` reads both sides
identically here (0 flags, unlike the `CONST::` TUs), but the folder-wide normalization pass should settle it.

**Gate:** `tmp/_tmp_cl_all_s24b.log` - 38/38 at `/W4`, exit 0, 0 `Cnnnn:` diagnostics (the only `error` substring in the
log is the file name `errorhandling.cpp`), 38 echoes, 38 objects rebuilt, logged 19:08:42 **after** the final
`monitor.cpp` edit (18:55); the 15:14 `_tmp_cl_all_s24.log` predates that edit and is not the gate of record. `cl /EP`
exits 0 (`tmp/_tmp_mon_pp_post_s24b_err.txt` = the TU name only). **Instruments added:** `_tmp_mon_pp_s24.bat`,
`_tmp_mon_class_s24.ps1` (clone of `_tmp_msg_class_s23.ps1`), `_tmp_mon_mirror_s24.{cpp,bat}` (x64 + x86 in one run),
`_tmp_mon_probe_s24.cpp` + `_tmp_run_mon_probe_s24.bat`, `_tmp_mon_region_s24.ps1`, plus the pre-flag set
`_tmp_mon_pre_s24.ps1`, `_tmp_mon_pre2_s24.txt`, `_tmp_mon_guards_s24.ps1`, `_tmp_mon_bypass_s24.ps1`.
**Caveats written into `method.md`:** an inserted comment block must not re-open the
`namespace qor { namespace win { namespace api {` line - the first attempt duplicated it and the standalone compile
caught `C2888` + `C1075` before anything else ran; and the close-out audit's own false negative - an empty
`Compare-Object` assigns `$null`, so the *identical*-locator case printed `BLOCK NOT FOUND` until the found-condition was
tracked separately.

**Registers touched:** `u16.md` (6/6 guarded - first TU whose whole section shares one guard), `u19.md` (first bypass in
built code), `u21.md` (sixth instance + the `GetListBoxInfo` U4 hand-off), `u25.md` (fifth spelling), `u26.md` (fourth
instance, mirror measured on both arches), `census.md` (folder totals), `tasks.md` (hand-off row). **0 new P1/P2/P5/P6
instances and 0 U15 instances** (D14: record-only), so the folder's 11 unresolvable bindings are untouched by this
session and are all in `mouse.cpp` + `window.cpp` - which is what session 25 opens.


