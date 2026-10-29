# Tech Context

## Language and compiler requirements

- C++20 required: `CMAKE_CXX_STANDARD 20`, `CMAKE_CXX_STANDARD_REQUIRED ON`.
- GCC/Clang pass `-std=c++2a`; RTTI is enabled; position-independent code is enabled.
- Supported compiler families:
  - MSVC / Visual Studio 2022 CL: `/EHsc /GR /utf-8`, hidden visibility, `/INCREMENTAL:NO` for RelWithDebInfo.
  - GCC 13.1/13.3 on Ubuntu 24.04: `-std=c++2a -fcoroutines -fvisibility=hidden`; Debug adds pedantic flags.
  - clang 18 on Ubuntu 24.04: `-std=c++2a -fvisibility=hidden`; Debug adds pedantic flags.
- Global define `qor_pp_unicode` is supplied through `cmake/flags.cmake`.
- Heavy precompiled-header and macro usage; C++ language modules are not used.

## Windows SDK header resolution

- The MSVC include tree contains CRT/C++ headers but not Windows API headers.
- `<Windows.h>` and related Windows API headers resolve from Windows Kits 10 include directories supplied through the environment; CMake does not normally add a `/I` for them.
- Windows SDK `FORCEINLINE` definitions are visible in api_layer translation units, enabling header-inline wrappers.
- `WINAPI_FAMILY_PARTITION` and `_WIN32_WINNT` decide which SDK definitions exist; threadpool callback variants may be APP/partition, WIN7-gated, or DESKTOP-only.

## Build system and verification

- CMake minimum 3.10.0; CMake 4+ recommended. Top-level project is `QOR VERSION 0.5.0`.
- Active configured tree is `cmakebuild`. Regenerate after any `CMakeLists.txt` change; the generated Visual Studio project files otherwise become stale and can cause `LNK2019` unresolved externals.
- Use `cmake -B cmakebuild -S . -DCMAKE_BUILD_TYPE=Debug` to regenerate; do not pass a `-G` unless the cached generator needs changing.
- Build one target per command. Two simultaneous builds in one command can race on generated artifacts.
- Run test executables from their own build directory because tests address `test/data` with relative paths calibrated to the executable location.
- Test judge = executable output plus shell exit code. Green requires `[OK](n/n)` and exit 0. ctest is not used.
- Capture stdout and stderr when running tests; failure summaries can be on stderr.
- Grep large test logs for markers such as `FAIL`, `OK(`, and the case name; new assertion blocks should be early enough to remain findable.
- Transient build/run logs belong in the ignored scratch area, not the repository root.

## Running shell commands (Windows / PowerShell)

- Agent commands run through `pwsh.exe`, where nested quotes, backticks and here-strings break easily. When a command needs non-trivial quoting, regex, embedded string literals, or multi-line logic, **write the script to a `.ps1` file under the ignored `tmp/` directory and execute the file** instead of issuing a long inline one-liner.
- Scratch goes in `tmp/` at the repository root, which is git-ignored (`.gitignore:41` `/tmp`) — never in the repository root itself. Transient build/run logs belong there too.
- **Clean up `tmp/` at the end of the task.** Scratch scripts and logs are not durable artifacts; delete them once the work is verified rather than leaving them behind.
- Anti-pattern to avoid: loose `tmp_review_*.cmd` / `tmp_review_check*.ps1` files left at the repository root are neither ignored nor durable — they must live in `tmp/` and be deleted.

## Repo conventions

- `.gitignore` covers generated objects, libraries, executables, build trees, and generated IDE/CMake artifacts.
- `sdk/` is generated for external applications.
- `docs/agent/` is the parallel agent-state system: `STATE.MD`, `TODO.md`, `DECISIONS.md`, `verification.md`, ctags, repo map, dependency graph, and skill files. Keep it in sync with the memory bank.
- Agent scratch files under `docs/agent/` are transient and not durable documentation.
- CMake uses `##` comments for disabled targets/dependencies; check before assuming a module is built.
