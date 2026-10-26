# Active Context

Last updated: 26 Oct 2026.

## Current focus: Windows api_layer

- `winqor_api_shell` is complete: 60 modern `shellapi.h` entry points are wrapped as stateless `qor::win::api::Shell` functions in `src/platform/os/windows/api_layer/shell`. The build and audit are green; the DLL does not import `SHELL32.DLL` because entry points bind at call time.
- Open shell decisions: decide whether to wrap the four live but out-of-scope entry points `SHEnumerateUnreadMailAccountsW`, `SHGetUnreadMailCountW`, `SHSetUnreadMailCountW`, and `SHGetDriveMedia`; decide whether REFIID wrappers should use the SDK `::REFIID` GUID spelling; decide whether DESKTOP-partition-only entry points should be guarded out for non-DESKTOP WINAPI_FAMILY builds.

- `winqor_api_kernel` D11 is implemented: the six threadpool callback-environ functions (`InitializeThreadpoolEnvironment`, `DestroyThreadpoolEnvironment`, and four `SetThreadpoolCallback*` functions) are wrapped with header-inline API calls using `Library::inlineCall` / `Library::voidInlineCall`, explicit template arguments, `::` SDK names, and the `qor_pp_usesinlineapi` marker.
- Two threadpool callback names are declared but deliberately unwrapped: `SetThreadpoolCallbackPriority` and `SetThreadpoolCallbackPersistent`. The SDK exposes them only under `_WIN32_WINNT >= _WIN32_WINNT_WIN7` and `WINAPI_PARTITION_DESKTOP`, respectively, so wiring them into a multi-configuration library is an open decision.

Next actions:
- Review the two unwrapped threadpool names and the four shell entry points above.
- Extend export-bound/header-inline audit classification to other api_layer DLL libraries.

Recent completed context:
- `qor_mime` is complete and verified green.
- JSON generator pipeline integration, JSON roundtrip testing, and the full `QORTest` regression run are complete and green.
- JSON parse-performance verification concluded neutral; all three micro-edits were kept by user decision.
- Structural JSON parse-performance refactoring is deferred until greater resources are available.
