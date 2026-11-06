# Folder-level leads - the queue is closed, T8 step 6d is next (6 Nov 2026)

Written wholesale at the T8 step 6c close. All 38 review-queue sources are `closed` and `pointer.cpp` (6a), `dpi.cpp` (6b)
and `touch.cpp` (6c) are closed with them, so there is no next TU: this file lists what a further session in this folder
picks up, and its lead is **T8 step 6d - the Display configuration family, 7 names**.
**Write-once**: read it once at boot, edit it never, replace it wholesale if the focus changes.

## Current folder figures (re-measured 6 Nov 2026 with `_tmp_u18_coverage4.ps1` + `_tmp_u18_buckets.ps1`)

**41 sources / 594 members / 497 bindings / 1,018 export names / 717 covered / 48 declared-and-unwrapped raw / 253 with no
prototype.** Raw 48 minus the 7 excluded superseded twins (`Get/SetClassLongA/W`, `SetWindowsHookA/W`,
`UnhookWindowsHook`) and the 1 instrument artifact (`SetWindowsHookExAW`, no prototype) = **40 net backlog**: Display
config **7**, Shutdown/power 3, Misc 24, held-open legacy 6. Per-name dispositions: `registers/u18-coverage.md`. The
buckets instrument now prints three drained families (`0: Pointer family []`, `0: DPI / awareness family []`,
`0: Touch / gesture family []`).

## Step 0 - re-derive the gap list before measuring anything

Every tranche pre-scan filters `tmp/_tmp_u18_gap_sdk.txt`, which `tmp/_tmp_u18_coverage4.ps1` writes, so **run that first**
- a stale gap list feeds the previous family's names back to you. Then copy `tmp/_tmp_t8_6d_pre.ps1` (already written: the
6c pre-scan with its bucket filter changed to `DisplayConfig|DisplayAutoRotation`): per name it prints the export-table
row, the `winuser.h` declaration line, the prototype text, the guard stack open at that line, the macro count, the A/W
siblings and whether the stem is already declared in `user32.h`. Re-run it after the gap list is refreshed - the file
`tmp/_tmp_t8_6d_pre.txt` under this session's date is the version quoted below.

## T8 step 6d - the 7 Display configuration names, with the SDK facts already measured

All 7 are **exported and declared**, all carry **0** function-like macros, **0** `A`/`W` siblings and **0** rows already in
`user32.h`, so all 7 bind **bare `qor_pp_useswinapi`** under D13 and `user32.h:9-17` gains **no new `#undef`**. Ordinals:
`DisplayConfigGetDeviceInfo` BE, `DisplayConfigSetDeviceInfo` BF, `GetDisplayAutoRotationPreferences` 14B,
`GetDisplayConfigBufferSizes` 14C, `QueryDisplayConfig` 2BA, `SetDisplayAutoRotationPreferences` 334,
`SetDisplayConfig` 335. Declaration lines: `GetDisplayConfigBufferSizes` `:13203`, `SetDisplayConfig` `:13211`,
`QueryDisplayConfig` `:13221`, `DisplayConfigGetDeviceInfo` `:13234`, `DisplayConfigSetDeviceInfo` `:13240`,
`GetDisplayAutoRotationPreferences` `:16063`, `SetDisplayAutoRotationPreferences` `:16077`.

Two guard groups, and the layer adds **no `#if`** (`WINVER` defaults to `0x0A00`, nothing in this build sets it):

- **The five `DISPLAYCONFIG_*` names - innermost `#if(WINVER >= 0x0601)`, inside `#ifndef NOSYSPARAMSINFO`, inside
  `#ifdef _WINGDI_` and `#ifndef NOGDI`**, plus the desktop partition. The `_WINGDI_` / `NOGDI` pair is new to this
  backlog: those declarations only exist if `wingdi.h` was parsed first. Measured, not assumed - see the probe below.
- **The two auto-rotation names - `#if(WINVER >= 0x0602)`** plus the desktop partition, with **no** `NOSYSPARAMSINFO` and
  no `_WINGDI_` test; `ORIENTATION_PREFERENCE` is the enum at `:16038`, not a `DWORD`.

**Liveness is already probed.** `tmp/_tmp_t8_6d_protos.cpp` (7 function-pointer typedefs initialised with `&<SDK name>`,
built by `tmp/_tmp_t8_6c_probe.bat`: `/W4`, no QOR define, no version macro) compiles **exit 0** - so `wingdi.h` really is
parsed before those `winuser.h` lines under `<Windows.h>`, the seven signatures above type-check, and none of the seven
guards is compiled out. The typedef text was copied from the digest's `proto=` rows and accepted by the compiler, which
also means the digest did **not** truncate any of the seven (they are single-line SAL annotations, not commented
parameter lists). Re-run the probe once the TU exists so the same check runs under the layer's own include chain.

### The four things in this family that a wrapper comment has to carry

1. **Five of the seven return `LONG`, not `BOOL`** - a Win32 error code (`ERROR_SUCCESS` on success,
   `ERROR_INVALID_PARAMETER` / `ERROR_INSUFFICIENT_BUFFER` / `ERROR_GEN_FAILURE` as the answer). The other two
   (`Get/SetDisplayAutoRotationPreferences`) are `BOOL`. `ChangeDisplaySettingsT` (`user32.h:328`) is the existing
   `LONG`-returning precedent in this header. Under D15 / D18 the code is returned unchanged and **no `CheckReturn` is
   introduced** - a `LONG` error code is not a success flag, and treating non-zero as failure inverts the meaning.
2. **The count parameters are `_Inout_`, not `_In_`**: `QueryDisplayConfig` takes
   `UINT32* numPathArrayElements` / `UINT32* numModeInfoArrayElements` that carry the buffer capacity **in** and the used
   count **out** (`_Inout_`, `:13224`, `:13226`), which is why `GetDisplayConfigBufferSizes` (`:13203`, two `_Out_`
   `UINT32*`) exists - call it first, size the arrays, then query. The wrapper cannot size or validate either array.
3. **`QueryDisplayConfig`'s last parameter is conditionally in/out by another parameter**:
   `_When_(!(flags & QDC_DATABASE_CURRENT), _Pre_null_) _When_(flags & QDC_DATABASE_CURRENT, _Out_)
   DISPLAYCONFIG_TOPOLOGY_ID* currentTopologyId` (`:13229`). Passing non-null with `QDC_DATABASE_CURRENT` set, or null
   without it, is the caller's error - state it, do not code a check.
4. **`SetDisplayConfig` is the only backlog name that changes machine state** - it applies a display topology to the
   console session, and with `SDC_APPLY` but not `SDC_SAVE` the change is undone at next logon. Its arrays are
   `_In_reads_opt_(count)` (`:13213`, `:13215`: null is legal when the counts are 0 or when `SDC_USE_DATABASE_CURRENT`
   is in `flags`). `DisplayConfigGetDeviceInfo` is `_Inout_ DISPLAYCONFIG_DEVICE_INFO_HEADER*` (`:13237`): the caller
   fills `size` / `type` / `id` and the same block is the out buffer; `DisplayConfigSetDeviceInfo` (`:13240`) is `_In_`.

### Types and placement

Every `DISPLAYCONFIG_*` type is declared in `winuser.h` itself - `DISPLAYCONFIG_PATH_INFO` `:13213`,
`DISPLAYCONFIG_MODE_INFO` `:13215`, `DISPLAYCONFIG_TOPOLOGY_ID` `:13229` - there is **no `windisp.h` in this SDK**
(verified: the header is absent and no `um` header includes it), so the types reach the TU through `kernel32.h:12`'s
`#include <Windows.h>` exactly as the 6c types did, and neither the TU nor `user32.h` declares any type of its own.
**Section question (U12):** no `DisplayConfig` stem is declared, so nothing pulls a declaration beside a wrapped sibling.
The nearest existing members are the display-mode five in the `//DC functions` section (`user32.h:328-332`:
`ChangeDisplaySettingsT`, `ChangeDisplaySettingsExT`, `EnumDisplayDevicesT`, `EnumDisplaySettingsT`,
`EnumDisplaySettingsExT`) and `//Monitor functions` at `:668`. 6a/6b/6c precedent is one new section per tranche placed
after the previous tranche's (`:569`, `:603`, `:629`); a `//Display configuration functions` section after
`//Touch and gesture functions` keeps that pattern, and the insert shifts every later `user32.h` self-citation by the
insert length (the T7 citation-shift rule).

### Consumers

Unknown until measured - clone `tmp/_tmp_t8_6c_consumers.ps1` with the name list read from `tmp/_tmp_t8_6d_pre.txt`. 6c
measured **0 LAYER / 0 BYPASS** over 3,228 files; `QueryDisplayConfig` / `SetDisplayConfig` are likelier than touch to
have an in-tree SDK call (display enumeration lives in `ui/`), so read the enclosing comment state before calling any hit
a bypass. `registers/u19.md` is **FROZEN at session 34**: book a split in the TU banner, never in the register.

