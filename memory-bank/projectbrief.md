# Project Brief - QOR (Querysoft Open Runtime)

Workspace: `linuxQOR`
Repository: https://github.com/mfaithfull/linuxQOR. Author: Matthew Faithfull.
Language: C++ 20
Scale: more than 100 libraries, more than 250,000 lines of code. Measured Nov 2026.

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
- "QOR module" means a runtime library/module, not a C++ language module.
- C++20 is required; macros and precompiled headers remain central.
- Opinionated C++ style; contributions are welcome once intent/style is understood.

Status sources:
- Trust `git log`, `docs/agent/DECISIONS.md`, `docs/agent/verification.md`, and `memory-bank/activeContext.md` for newest state.
- `STATUS.md` trails the repository; `CHANGES.md` tracks shipped milestones; `FEATURES.md` tracks maturity.
- Project docs: `README.md`, `KEY_CONCEPTS.md`, `LAYOUT.md`, `BUILDING.md`, `FEATURES.md`, `STATUS.md`, `CHANGES.md`, `FAQ.md`, `CONTRIBUTING.md`, `CREDITS.md`, `docs/skills/QORconventions.md`.
