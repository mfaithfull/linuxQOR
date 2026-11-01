# narrative: devicecontext.cpp, session 11

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


