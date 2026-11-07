# Folder-level leads - the queue is closed, T8 step 6e is next (6 Nov 2026)

Written wholesale at the T8 step 6d close. All 38 review-queue sources are `closed` and `pointer.cpp` (6a), `dpi.cpp` (6b),
`touch.cpp` (6c) and `displayconfig.cpp` (6d) are closed with them, so there is no next review TU: this file lists what a
further session in this folder picks up, and its lead is **T8 step 6e - the Shutdown / power family, 3 names**.
**Write-once**: read it once at boot, edit it never, replace it wholesale if the focus changes.

## Current folder figures (re-measured 6 Nov 2026 with `_tmp_u18_coverage4.ps1` + `_tmp_u18_buckets.ps1`)

**42 sources / 601 members / 504 bindings / 1,018 export names / 724 covered / 41 declared-and-unwrapped raw / 253 with no
prototype.** Raw 41 minus the 7 excluded superseded twins (`Get/SetClassLongA/W`, `SetWindowsHookA/W`,
`UnhookWindowsHook`) and the 1 instrument artifact (`SetWindowsHookExAW`, no prototype) = **33 net backlog**: Shutdown /
power **3**, Misc 24, held-open legacy 6. Per-name dispositions: `registers/u18-coverage.md`. The buckets instrument now
prints four drained families (`0: Pointer family []`, `0: DPI / awareness family []`, `0: Touch / gesture family []`,
`0: Display config family []`).

## Step 0 - re-derive the gap list before measuring anything

Every tranche pre-scan filters `tmp/_tmp_u18_gap_sdk.txt`, which `tmp/_tmp_u18_coverage4.ps1` writes, so **run that first**
- a stale gap list feeds the previous family's names back to you. Then run `tmp/_tmp_t8_6e_pre.ps1` (already written; the
6d pre-scan with its bucket filter changed to `SuspendResume|CancelShutdown`): per name it prints the export-table row, the
declaring header and line, the prototype text, the guard stack open at that line, the macro count, the A/W siblings and
whether the stem is already declared in `user32.h`. Re-run it after the gap list is refreshed - the file
`tmp/_tmp_t8_6e_pre.txt` under this session's date is the version quoted below. **Its name patterns are `\bName\s*\(`, not
`\bName\(`**: `winuser.h` writes `RegisterSuspendResumeNotification (` with a space before the parenthesis, and a
`\bName\(` pattern reports the declaration as absent (see `gotchas/instrument-flags.md`).

## T8 step 6e - the 3 Shutdown / power names, with the SDK facts already measured

All 3 are **exported and declared**, all carry **0** function-like macros, **0** `A`/`W` siblings and **0** rows already in
`user32.h`, so all 3 bind **bare `qor_pp_useswinapi`** under D13 and `user32.h:9-17` gains **no new `#undef`**. Ordinals:
`CancelShutdown` `22`, `RegisterSuspendResumeNotification` `2F6`, `UnregisterSuspendResumeNotification` `3CD`. Declaration
lines: `CancelShutdown` `winuser.h:13587`, `RegisterSuspendResumeNotification` `:4009`,
`UnregisterSuspendResumeNotification` `:4017`. Prototypes as the SDK writes them: `BOOL CancelShutdown(VOID)`,
`HPOWERNOTIFY RegisterSuspendResumeNotification(IN HANDLE hRecipient, IN DWORD Flags)`,
`BOOL UnregisterSuspendResumeNotification(IN HPOWERNOTIFY Handle)`.

Two guard groups, and the layer adds **no `#if`**:

- **`CancelShutdown` - no version guard at all.** Its guard stack is `#ifndef _WINUSER_ ;;`
  `#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)`: unlike every other backlog name so far it sits **outside
  `NOUSER`** and outside any `WINVER` / `_WIN32_WINNT` test - the same shape the 6c gesture five had. Nothing to say about
  liveness except that it is unconditional.
- **The two notification names - innermost `#if (_WIN32_WINNT >= 0x0502)`**, inside `#if(WINVER >= 0x0500)`, inside
  `#ifndef NOUSER`, plus the desktop partition. `0x0502` is the first backlog floor of that kind: 6b's single
  `NTDDI_VERSION` name (`GetDpiAwarenessContextForProcess`) was the only non-`WINVER` floor before it. `sdkddkver.h`
  defaults `_WIN32_WINNT` to `0x0A00` and nothing in this build sets it, so both are live and the wrapper stays live with
  the declaration.

**The handle contract is the thing to state, not code.** `HPOWERNOTIFY` is `typedef PVOID HPOWERNOTIFY`
(`winuser.h:3985`, inside `#if !defined(_HPOWERNOTIFY_DEF_)` at `:3981`) - an OS-created handle the layer never makes, and
`UnregisterSuspendResumeNotification` closes exactly it. `Flags` selects what `hRecipient` is:
`DEVICE_NOTIFY_WINDOW_HANDLE` (`:3946`, `0x0`, an `HWND`) or `DEVICE_NOTIFY_SERVICE_HANDLE` (`:3947`, `0x1`, a service
status handle), and the notification itself arrives as `WM_POWERBROADCAST` (`:2328`, `0x0218`). No scope guard on the
handle, exactly as `touch.cpp` keeps none for `HTOUCHINPUT` / `HGESTUREINFO` and `dpi.cpp` none for the
`SetThreadDpiAwarenessContext` previous-value pair. Note the `NOUSER` coupling: the typedef and the two prototypes sit in
the same `#ifndef NOUSER` region, so `user32.h` declares members whose **type** disappears for a `NOUSER` consumer - 0
in-tree sources define `NOUSER` or `NOGDI` (measured over `src`, `cmake`, `examples`, `test`), which is why the layer adds
no guard rather than dropping the members.

**Liveness and signatures are already probed.** `tmp/_tmp_t8_6e_protos.cpp` (3 function-pointer typedefs initialised with
`&<SDK name>`, built by `tmp/_tmp_t8_6c_probe.bat`: `/W4`, no QOR define, no version macro) compiles **exit 0** - a
guard-compiled-out declaration would be undeclared and a wrong signature would be `C2440`, so that one run settles both.
The TU itself then compiles under the layer's own include chain (`_tmp_t8_6d_single.bat` is the wrapper to clone).
