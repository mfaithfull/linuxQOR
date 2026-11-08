---
Type: Index
Decisions:
  - D1: Unit of work = one CMake library
    File: ./decisions/D1.md
  - D2: ctags per lib, not global
    File: ./decisions/D2.md
  - D3: Verification = compiler as judge
    File: ./decisions/D3.md
  - D4: STATE.MD / TODO.md re-read every turn
    File: ./decisions/D4.md
  - D5: Duplicate qor_http (http_old) — RESOLVED (user)
    File: ./decisions/D5.md
  - D6: header-only core modules — RESOLVED
    File: ./decisions/D6.md
  - D7: ctags availability — RESOLVED (supersedes earlier hand-index assumption)
    File: ./decisions/D7.md
  - D8: no ctest — RESOLVED (user, 20 Oct 2026)
    File: ./decisions/D8.md
  - D9: performance verification needs a per-parse timer + a noise floor — RESOLVED (22 Oct 2026)
    File: ./decisions/D9.md
  - D10: Threadpool callback-environment API is inlined
    File: ./decisions/D10.md
  - D11: Variant API wrapper for header-inline Windows API entry points - RULE (26 Oct 2026)-IMPLEMENTED: the variant wired in for the six; the two gap declarations added unwrapped (26 Oct 2026)
    File: ./decisions/D11.md
  - D12: the shell32.dll wrapper layer (winqor_api_shell) - 60 stateless wrappers (26 Oct 2026)
    File: ./decisions/D12.md
  - D13: A/W binding spelling for export-bound api_layer wrappers - RULE (29 Oct 2026)
    File: ./decisions/D13.md
  - D14: an exported wrapper with no reachable in-tree caller is not a review finding - POLICY (2 Nov 2026)
    File: ./decisions/D14.md
  - D15: Vthe `Library::Call<>` template-argument spacing split (U14) is closed as a review item - POLICY (3 Nov 2026)
    File: ./decisions/D25.md
  - D16: the T7 dead-include sweep is executed - ACTION (5 Nov 2026)
    File: ./decisions/D16.md
  - D17: Windows support is 64-bit (amd64) only - no 32-bit x86 target - POLICY (5 Nov 2026)
    File: ./decisions/D17.md
  - D18: winqor_api_user's purpose is a full wrapper set over user32.dll's exports - a wrapper's signature mirrors the export - POLICY (5 Nov 2026)addendum (5 Nov 2026) the scope of the "full set" is the documented set, superseded families excluded
    File: ./decisions/D18.md
  - D19: the 6 held-open legacy names are in scope - wrap all of them - POLICY (7 Nov 2026)
    File: ./decisions/D19.md
  - D20: the variadic wrapper shape - `Library::varCall` and one header-inline `wsprintfT` member - POLICY (7 Nov 2026)
    File: ./decisions/D20.md
  - D21: U16 is closed by pinning the SDK version macros in kernel32.h - RULE (7 Nov 2026)
    File: ./decisions/D21.md
  - D22: T1 - add the two Authorization definitions in a new `security.cpp` TU and make them `static` - ACTION (8 Nov 2026)
    File: ./decisions/D22.md
  - D23: T5 - wrap the IME/WINNLS block in `ime.cpp`, make the pair `static`, keep the obsolete member on the interface - ACTION (8 Nov 2026)
    File: ./decisions/D23.md
  - D24: T6 - wrap the "//List Box functions" trio in a new `listbox.cpp` TU and make all three `static` - ACTION (8 Nov 2026)
    File: ./decisions/D24.md
  - D25: T2 - type the Desktop / Window Station callback parameters as the SDK's W spellings - ACTION (8 Nov 2026)
    File: ./decisions/D25.md
  - D26: U4's remaining 86 declared-but-undefined members land as a tranche queue (T9) — POLICY (8 Nov 2026)
    File: ./decisions/D26.md
---