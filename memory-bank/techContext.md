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
- **The api_layer pins the SDK version triple in `kernel32.h`** (decision D21, 7 Nov 2026): `_WIN32_WINNT`/`WINVER` `0x0A00` and `NTDDI_VERSION` `0x0A000010` (`NTDDI_WIN11_GE`), each `#ifndef`-guarded, with a floor `#error` if a consumer pins lower. The values are what the unpinned build measured from `sdkddkver.h` on Windows Kits 10.0.26100, so the pin preserves the surface this tree already used and closes U16 for anything that reaches the api_layer through QOR headers. The shrink macros (`NOGDI`, `NOUSER`, `NOCTLMGR`, `NOMB`, `NOMSG`, `NOSYSPARAMSINFO`, `WINAPI_FAMILY`) are **not** pinned and still decide the SDK surface.
- **Windows targets are 64-bit (amd64) only - no 32-bit x86** (decision D17, `docs/agent/DECISIONS.md`; in-tree note `src/platform/os/windows/purpose.md:3`). `CMakeSettings.json` has the single `x64-Debug` profile, the tree does not compile for x86, and no agent gate or verification pass runs an x86 compile.

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
- **Clean up `tmp/` when the open task set is empty.** Scratch scripts and logs are not durable artifacts; delete them once the work is verified rather than leaving them behind.
- Anti-pattern to avoid: loose `tmp_review_*.cmd` / `tmp_review_check*.ps1` files left at the repository root are neither ignored nor durable — they must live in `tmp/` and be deleted.

## Repo conventions

- `.gitignore` covers generated objects, libraries, executables, build trees, and generated IDE/CMake artifacts.
- `sdk/` is generated for external applications.
- `docs/agent/` is the parallel agent-state system: `STATE.MD`, `TODO.md`, `DECISIONS.md`, `verification.md`, ctags, repo map, dependency graph, and skill files. Keep it in sync with the memory bank.
- Agent scratch files under `docs/agent/` are transient and not durable documentation.
- CMake uses `##` comments for disabled targets/dependencies; check before assuming a module is built.

## Reusable tooling tree (pinned 13 Nov 2026)

- Reusable scripts live in the **tracked** `tools/<slug>/` tree, never in `tmp/`. Each tool folder carries the script(s) plus a doc `memory-bank/tools/<slug>/tool.md` (`Type: Tool`, listing `Scripts:` with `File:` / `Usage:` / `LastRun:` / `Output:`).
- `memory-bank/tools.md` is the index (`Type: Index`, entries keyed by `UseWhen` + `Scope` + `File:`). Index `File:` paths are relative to `memory-bank/`; `Scripts:` `File:` paths are relative to the repo root. `docs/agent/TOOL-FORMAT.md` pins both shapes.
- **Read the index before writing any new script.** If a capability is indexed, refine and reuse that tool - do not clone it. Cloning instruments is where this workspace's recurring bugs came from: the `[int]($skip/$take)+1` slice-tag bug (wrote `_s8` for slice 3) and the site classifier missing ANSI/Unicode twin headers were both introduced by copy-paste clones, and each recurred across clones.
- `tmp/` is scratch only (gitignored at `.gitignore:41`) and is deleted when the task is verified. Measured 13 Nov 2026: **3,878 files / 1.07 GB** of per-task debris had accumulated there despite the rule. Consequence recorded in `activeContext.md`: source comments that cite `tmp/` artifacts become dangling pointers once `tmp/` is cleaned, so a scratch path must never be a durable citation.
- `.gitattributes` pins `*.md text eol=crlf`, which makes the memory-bank record diffs quiet and stable; without it every `git diff` / `git add` prints CRLF warnings that push the numbers out of readable output (`memory-bank/gotchas/git.md`).

