# narrative: devicemanagement.cpp, session 12

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

