# Project Brief - QOR (Querysoft Open Runtime)

Workspace: `c:\RAID0\Develop\linuxQOR` (linuxQOR repository, migrated in Oct 2026 from `f:\Develop\linuxQOR`; stale `F:/Develop/...` paths are out of date).
Repository: https://github.com/mfaithfull/linuxQOR. Author: Matthew Faithfull.

Scale re-measured 19 Oct 2026 at HEAD `93668d3d`: about 2,810 source files in `src/`, 207 `CMakeLists.txt` under `src/`, and 66 cross-platform libraries per `STATUS.md`.

Core requirements:
- Separation of concerns: remove unnecessary coupling; portability and adaptability follow.
- Self-aware software: testing/TDD, building, logging/tracing/metrics/debug info, thread-pool scaling, memory management, porting, connecting modules/signals/delegates/RPC, reflection/serialization.
- Testable across compilers and target platforms.
- Easy to extend: adding a QOR module is short work.
- Flexible: most libraries are usable without the application framework.
- Portable: OS/compiler/architecture dependencies live under `src/platform`.
- Reliability over peak performance.

Scope: `src/qor`, `src/framework`, `src/components`, `src/platform`, `src/macros`, `src/configuration`, `sdk/`, `test/`, `examples/`, `3rdparty/`, `docs/`, `cmake/`.

Positions:
- Not the highest-performance implementation.
- "QOR module" means a runtime library/module, not a C++ language module.
- C++20 is required; macros and precompiled headers remain central.
- Opinionated C++ style; contributions are welcome once intent/style is understood.

Status sources:
- Trust `git log`, `docs/agent/STATE.MD`, `docs/agent/TODO.md`, `docs/agent/DECISIONS.md`, `docs/agent/verification.md`, and `memory-bank/activeContext.md` for newest state.
- `STATUS.md` trails the repository; `CHANGES.md` tracks shipped milestones; `FEATURES.md` tracks maturity.
- Project docs: `README.md`, `KEY_CONCEPTS.md`, `LAYOUT.md`, `BUILDING.md`, `FEATURES.md`, `STATUS.md`, `CHANGES.md`, `FAQ.md`, `CONTRIBUTING.md`, `CREDITS.md`, `docs/skills/QORconventions.md`.
