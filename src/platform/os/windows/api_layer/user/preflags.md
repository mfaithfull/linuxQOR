# Folder-level leads - the review queue and all of T8 step 6 are closed (7 Nov 2026)

Written wholesale at the T8 step 6f close. All 38 review-queue sources are `closed` and the six coverage TUs `pointer.cpp` (6a),
`dpi.cpp` (6b), `touch.cpp` (6c), `displayconfig.cpp` (6d), `powermanagement.cpp` (6e) and `misc.cpp` (6f) are closed with them,
so there is no next review TU and no family tranche left: this file lists what a further session in this folder picks up, and
its lead is the **6 held-open legacy names** - the one open question `registers/u18-coverage.md` records, which a **user decision**
closes. **Write-once**: read it once at boot, edit it never, replace it wholesale if the focus changes.

## Current folder figures (re-measured 7 Nov 2026 with `_tmp_u18_coverage4.ps1` + `_tmp_u18_buckets.ps1`)

**44 sources / 627 members / 530 bindings / 1,018 export names / 751 covered / 14 declared-and-unwrapped raw / 253 with no
prototype.** Raw 14 minus the 7 excluded superseded twins (`Get/SetClassLongA/W`, `SetWindowsHookA/W`, `UnhookWindowsHook`) and
the 1 instrument artifact (`SetWindowsHookExAW`, which no header declares) = **6 net backlog**, all of it the held-open legacy
names. The buckets instrument prints six drained families - `Other` is the Misc bucket and is now 0 - and two rows:
`4: Legacy hook family` (all excluded) and `10: Legacy word / class-long family` (4 excluded twins, the artifact, and the 6).

## Step 0 - re-derive the gap list before measuring anything

Every tranche pre-scan filters `tmp/_tmp_u18_gap_sdk.txt`, which `tmp/_tmp_u18_coverage4.ps1` writes, so **run that first** -
a stale gap list feeds the previous family's names back to you. Then run `tmp/_tmp_t8_6g_pre.ps1` (written at the 6f close: the
6f instrument with its bucket filter changed to "the gap list less the 7 excluded twins less the artifact", so the bucket is
derived, not typed). Per name it prints the export-table row, the declaring header and line, the prototype, the guard stack open
at that line (forward pass, never a backward pop), the macro count and whether a replacement stem is already declared in
`user32.h`. Its name patterns are `\bName\s*\(`, not `\bName\(` - the SDK writes some prototypes with a space before the paren
(see `gotchas/instrument-flags.md`).

## T8 step 6f - the 24 Misc names, with the SDK facts measured 7 Nov 2026 (`tmp/_tmp_t8_6f_pre.txt`)

All 24 are **exported and declared**, all carry **0** function-like macros, **0** A/W-sibling rows and **0** rows already in
`user32.h`, so all 24 bind **bare `qor_pp_useswinapi`** under D13 and `user32.h:9-17` gains **no new `#undef`**. Ordinal /
`winuser.h` line per name: `CalculatePopupWindowPosition` 1B / :7798, `ChangeMenuA` 2A / :7519, `ChangeMenuW` 2B / :7528,
`ChangeWindowMessageFilterEx` 2D / :15665, `EvaluateProximityToPolygon` 10C / :6764, `EvaluateProximityToRect` 10D / :6756,
`GetAutoRotationState` 122 / :16057, `GetCIMSSM` 124 / :15993, `GetClipboardMetadata` 139 / :5490,
`GetCurrentInputMessageSource` 13E / :15987, `GetUnpredictedMessagePos` 1D5 / :3671, `GetWindowDisplayAffinity` 1E3 / :4788,
`GetWindowFeedbackSetting` 1E6 / :6800, `InheritWindowMonitor` 20D / :14529, `IsImmersiveProcess` 238 / :16094,
`IsWindowArranged` 249 / :16403, `RegisterForTooltipDismissNotification` 2E8 / :16382,
`SetAdditionalForegroundBoostProcesses` 31F / :16359, `SetCoalescableTimer` 32A / :7106, `SetDebugErrorLevel` 32F / :13541,
`SetProcessRestrictionExemption` 35B / :16100, `SetThreadCursorCreationScaling` 36D / :10874,
`SetWindowDisplayAffinity` 37B / :4795, `SetWindowFeedbackSetting` 37C / :6810.


### Guards (measured, forward pass over the declaring header)

Three guard stacks matter: 12 names sit inside `#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)` only, 11 additionally
inside `#if(WINVER >= 0x0600)` / `0x0601` / `0x0602` / `0x0603`, and `GetUnpredictedMessagePos` sits behind
`#if defined(WINNT)`. `WINVER` defaults to `0x0A00` from `sdkddkver.h` and **0** files under `src`, `cmake`, `examples` or
`test` define `WINVER`, `_WIN32_WINNT`, `NTDDI_VERSION`, `NOUSER` or `NOGDI` (measured over 3,557 files), so all 24 are live
in this build and the layer adds no `#if` - but a caller that pins `WINVER` below the block loses that name, and
`GetUnpredictedMessagePos` is the one that also needs `WINNT`.
