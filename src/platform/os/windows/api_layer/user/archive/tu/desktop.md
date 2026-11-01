# narrative: desktop.cpp, session 10

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

