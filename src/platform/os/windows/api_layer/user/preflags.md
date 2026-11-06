# Folder-level leads - the queue is closed, T8 step 6b is next (6 Nov 2026)

Written wholesale at the T8 step 6a close. All 38 review-queue sources are `closed` and `pointer.cpp` (6a coverage, not a
review session) is closed with them, so there is no next TU: this file lists what a further session in this folder picks
up, and its lead is **T8 step 6b - the DPI / awareness family, 21 names**.
**Write-once**: read it once at boot, edit it never, replace it wholesale if the focus changes.

## Current folder figures (re-measured 6 Nov 2026 with `_tmp_u18_coverage4.ps1` + `_tmp_u18_buckets.ps1`)

**39 sources / 559 members / 462 bindings / 1,018 export names / 682 covered / 83 declared-and-unwrapped raw / 253 with no
prototype.** Raw 83 minus the 7 excluded superseded twins and the 1 instrument artifact = **75 net backlog**: DPI/awareness
**21**, Touch/gesture 14, Display config 7, Shutdown/power 3, Misc 24, held-open legacy 6. Per-name dispositions:
`registers/u18-coverage.md`.

## T8 step 6b - the 21 DPI / awareness names, with the SDK facts already measured

All 21 are in the export table **and** declared in `winuser.h` 10.0.26100.0, and none of the 21 is a function-like macro or
has an `A`/`W` sibling (the guard scan and the macro/A-W probe both give 0 hits), so all 21 bind **bare
`qor_pp_useswinapi`** under D13 and `user32.h:9-17` needs **no new `#undef`** - unlike `SystemParametersInfo` (:11), whose
macro rewrite is why that `#undef` exists; `SystemParametersInfoForDpi` is a separate name and is not rewritten.

Five guard groups, and the layer adds **no `#if`** - `WINVER` defaults to `0x0A00` and `NTDDI_VERSION` to
`WDK_NTDDI_VERSION = NTDDI_WIN11_GE` (`sdkddkver.h:166`, `:236-242`), so every group is live:

- **Dialog DPI change - 4 names, `winuser.h:5340`, `:5347`, `:5363`, `:5370`.** Guarded only by `NOCTLMGR` (`:4912`) plus
  `WINAPI_PARTITION_DESKTOP` (`:5006`) - **no `WINVER` test at all**. The two `Get*` members return the **enum**, not
  `BOOL`: `DIALOG_CONTROL_DPI_CHANGE_BEHAVIORS` (`:5332`) and `DIALOG_DPI_CHANGE_BEHAVIORS` (`:5355`), each behind
  `DEFINE_ENUM_FLAG_OPERATORS`, and the setters take a mask/values pair.
- **Two DPI variants of already-wrapped members - `GetSystemMetricsForDpi` `:7418` (`NOSYMETRICS` `:7242`,
  `#if(WINVER >= 0x0605)`) and `SystemParametersInfoForDpi` `:13280` (`NOSYSPARAMSINFO` `:12433`, same `WINVER`).** Their
  stems are wrapped (`user32.h:686`, `:688` today), so U12's section question has an answer beside them.
- **The `DPI_AWARENESS_CONTEXT` block - 11 names at `:14462`, `:14468`, `:14474`, `:14480`, `:14486`, `:14492`, `:14499`,
  `:14511`, `:14517`, `:14523`, `:14539`, all inside `#if(WINVER >= 0x0605)` opening `:14458`.** `DPI_AWARENESS_CONTEXT` is
  a `DECLARE_HANDLE` at `windef.h:256` and `DPI_AWARENESS` an enum at `windef.h:259` - both reach the layer through
  `kernel32.h:12 #include <Windows.h>`, exactly as the `POINTER_*` types did in 6a. Return types to get right:
  `SetThreadDpiAwarenessContext` returns the **previous context handle**, not `BOOL`, while
  `SetProcessDpiAwarenessContext` (`:14539`, its own `#if(WINVER >= 0x0605)` at `:14535`) returns `BOOL`; `GetDpiForSystem`
  and `GetSystemDpiForProcess` return `UINT`. `GetDpiForWindow` (`:14505`) is **already wrapped** (`user32.h:675`).
- **`GetDpiAwarenessContextForProcess` `:14550` - the only backlog name guarded by `NTDDI_VERSION`, not `WINVER`**
  (`#if (NTDDI_VERSION >= NTDDI_WIN10_19H1)` at `:14545`). Live under the default; worth one comment line, since a caller
  that pins `NTDDI_VERSION` lower loses it.
- **The hosting-behavior trio - `:14560`, `:14566`, `:14571`, `#if(WINVER >= 0x0606)` at `:14555`.** All three return
  `DPI_HOSTING_BEHAVIOR` (`windef.h:272` region) and `SetThreadDpiHostingBehavior` returns the previous value.

Shape to follow is 6a's: one new TU for the family (6a used `pointer.cpp`), the `caret.cpp` / `pointer.cpp` body shape -
four generic includes, `user32.h` then `../library.h` last, per body `qor_pp_fcontext;` + bare
`qor_pp_useswinapi(user32, Name);` + explicit `Library::Call<...>`, a contract block at the head, no return checks (D15 /
D18 pass-through). Check for a consumer before booking anything under U19: 6a had none in-tree.

## Standing limits carried from 6a (they bound every later tranche)

- **The coverage instrument walks the export table, so "declared but not exported" is invisible to it.**
  `EnableMouseInPointerForThread` (`winuser.h:6715`) is exactly that case, and wrapping it would have installed a
  permanently null binding (the U5 class). Check the export table for every name the gap list gives, and the header for
  every name it does not - the gap list cannot see either direction.
- Export evidence is one `dumpbin /exports` capture of the local `System32\user32.dll`; signatures are read from SDK
  10.0.26100.0 only.
- **Forward flag for tranche 6d (Display config):** the `DISPLAYCONFIG_*` types are **not** in `WinUser.h` - they come from
  `wingdi.h:3005`, which `winuser.h:13213` onward only *uses*; the `#include <tvout.h>` at `winuser.h:13057` resolves to
  `shared/tvout.h`, not to a `um` header. `Get/SetDisplayAutoRotationPreferences` (`:16063`, `:16077`, inside
  `#if(WINVER >= 0x0602)` at `:16001`) are the two names in that bucket that need no GDI type at all.

## Everything else a session might pick up

- **Fixable defects: none queued by a TU pass.** D18 makes the published-signature divergences listed in `registers/u18.md`
  the defect population; T8 steps 1-5 of those are in, so only step 6 (the backlog) remains.
- **Open items**: U1, U2, U4, U5, U7-U10, U12, U13, U16, U17, U18, U19-U26, **U27** (`registers/p6-u27.md` - `STRICT`
  changes the identity of `HOOKPROC` and of every `DECLARE_HANDLE`). Frozen, append nothing: P1, P2, P5, U11, U14, U15,
  U6, U3, U28 (D17). **U17's `A`/`W` question still moves the count by exactly one wrapper** - `ChangeMenuA/W` and
  `wsprintfA/W` are the only in-scope pairs left.
- **Tasks T1-T6, T8** and **P3, P4, P6**. **T7 closed (D16)**, and what it left behind is the citation shift: deleting
  line 11 moved every later `:NNN` self-citation down by one in 35 files (`gotchas/evidence-integrity.md` has the rule).
- **Architecture: x64 only** (D17). No session here reopens an x86 question or runs an x86 compile.
- Session 37's unregistered build-system lead - a `qor_pp_ver_*` version string that can go missing silently across 163
  `CMakeLists.txt` - stays unregistered by user decision: act on it in `cmake/` or `src/macros/moduleversion.hpp`, not here.

## Gate if this folder is touched again

Unchanged, and it is the only gate: the edited TU standalone at `/W4`, then the folder's **39** sources in one `cl`
invocation - command line in `status.md`, "Gate" section. A coverage tranche is expected to change `user32.h`, so the
**107-source** all-libraries run is part of the gate, and it is signal only as a diff against a pre-fix capture.
