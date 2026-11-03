# Active Context

Last updated: 3 Nov 2026 (session 27 `processthread.cpp` CLOSED - the review's **cleanest TU**: 5 wrappers, 0 binding,
signature, return or idiom defects; fixes were the folder's last TAB line, three stray spaces and (user decision) the TU's
5 `Call<>` sites normalised to tight, moving folder U14(a) to **174 spaced / 254 tight**; new material U16's second 0x0501
gate, a coverage measure and **T7** - 35 dead `returncheck.h` includes). Session 26 is committed (`7ed35348`, which carries
sessions 1-26); session 27's working tree is **17 paths** per `git status` (15 modified, 2 new -
`archive/tu/processthread.md`, `archive/session-log-23-24.md`). **Next: session 28 `rectangle.cpp`** (11
wrappers, `user32.h:446`-`:457`), pre-flags measured and in `status.md`. Record hygiene: `status.md` sits just under its
line ceiling and its register locator is the part that grows, so rewrite the prose there rather than adding; `method.md` was
**split on 2 Nov 2026** into `method.md` (the numbered steps), `tooling.md` (the instrument index) and `gotchas.md` (the
caveats it points at). Completed-session records stay in `memory-bank/archive/`, never here.

## Current focus: winqor_api_user source review (one TU per session)

- Reviewing `src/platform/os/windows/api_layer/user` (class `qor::win::api::User32` over `user32.dll`) for
  correctness, consistency, safety, UB and convention conformance. **Protocol: exactly one translation unit
  per session**; small unambiguous defects fixed in place, everything behavioural recorded as an open item.
- **Session 27 (`processthread.cpp`) closed 3 Nov 2026 - 0 defects found; 28 of 38 TUs reviewed.** All 5 bindings resolve
  (bare stems, no A/W family, no `winuser.h` macro, so P2/P6 cannot arise), header/definition/SDK parameter names are
  identical 5/5 and every `Call<>` template list restates the definition exactly. Fixes: TAB + stray-space whitespace and
  the user-decided `Call<>` tight normalisation. What the session added to the record: `registers/u2.md` (3 of this
  library's 4 `CheckReturn` sites; `WaitForInputIdle` correctly unchecked because **0 is its success value**),
  `registers/u16.md` (`IsWow64Message` under `_WIN32_WINNT >= 0x0501`, plus the **empty** `#if (_WIN32_WINNT >= 0x0602)` /
  `#endif` pair at `winuser.h:4121-4122` that a guard walk without an `#endif` depth counter reports as a real guard),
  `registers/u19.md` (1 LAYER caller - `messagehandler.cpp:246` - 0 bypass) and a new **coverage** measure
  (`tmp/_tmp_pt_coverage_s27.txt`: 370 of 1,018 exports have no wrapper, 114 of them SDK-declared). Narrative
  `archive/tu/processthread.md`, row 27; gate `tmp/_tmp_cl_all_s27.log` (38/38 `/W4`, exit 0, dated after the last edit).
- Sessions 24-26 (`monitor`, `mouse`, `painting`) are closed and committed; their detail is `archive/tu/monitor.md`,
  `archive/tu/mouse.md`, `archive/tu/painting.md` and rows 24-26 of the session log, not here. The load-bearing result of
  session 25 stays in force: `AW` over a stem with **no A/W family** throws at run time on every call, so bare
  `qor_pp_useswinapi` is **D13's other half**, not a style call.

## Decisions in force (pointer only - not restated here)

- **D13** (explicit `qor_pp_useswinapiAW` for A/W families, bare only for single exports), **D14** (U15 CLOSED - an
  exported wrapper with no in-tree caller is published surface, not a finding) and the **U1/U2** return-check policy
  (no `Win32ErrorHandler`, no `CheckReturn` in these wrappers) are recorded once in
  `src/platform/os/windows/api_layer/user/status.md` "Convention decision" and decided in `docs/agent/DECISIONS.md`.
  They govern every TU session; the memory bank deliberately does not duplicate their text.
- **Session-20 deferral, still in force:** the 10 broken `mouse.cpp` bindings get no out-of-turn hotfix - they are
  fixed in `mouse.cpp's` own session (session 25), keeping the one-TU-per-session rule.
- Superseded audit assumptions (the G1-G6 corrections and the old P1/P2 tallies) are archived, not restated here;
  the live tallies are in `status.md`.

## Where the record lives (boot order)

1. `src/platform/os/windows/api_layer/user/status.md` - the review boot file: live state, TU queue, register locator,
   verification gate and the current TU's pre-flags.
2. `src/platform/os/windows/api_layer/user/registers/` - one file per open item; open at the locator line, never the
   whole tree. `../method.md` holds the procedure, `../tooling.md` the instrument index and `../gotchas.md` the caveats
   that index points at.
3. `src/platform/os/windows/api_layer/user/archive/` - per-TU narratives (`tu/<name>.md`) and the row index
   (`session-log.md`). This is the authoritative session history; the memory bank keeps no per-session narrative.
4. `memory-bank/archive/` - session records moved out of this file (index: `memory-bank/archive/README.md`; **not boot
   material** - do not read it at session start).

**Rule adopted 2 Nov 2026 (user):** this file carries the current focus, the decisions in force and pointers - **never a
per-session narrative**. At each session close, the outgoing "recent changes" entry is either reduced to the two or three
lines a new session needs to start, or moved to `memory-bank/archive/`. Session detail belongs in the project record tree
(item 3), not here. The file was 455 lines / 81 KB before this rule and must stay near this size.

## Current focus: Windows api_layer

- `winqor_api_shell` is complete: 60 modern `shellapi.h` entry points are wrapped as stateless `qor::win::api::Shell` functions in `src/platform/os/windows/api_layer/shell`. The build and audit are green; the DLL does not import `SHELL32.DLL` because entry points bind at call time.
- Open shell decisions: decide whether to wrap the four live but out-of-scope entry points `SHEnumerateUnreadMailAccountsW`, `SHGetUnreadMailCountW`, `SHSetUnreadMailCountW`, and `SHGetDriveMedia`; decide whether REFIID wrappers should use the SDK `::REFIID` GUID spelling; decide whether DESKTOP-partition-only entry points should be guarded out for non-DESKTOP WINAPI_FAMILY builds.

- `winqor_api_kernel` D11 is implemented: the six threadpool callback-environ functions (`InitializeThreadpoolEnvironment`, `DestroyThreadpoolEnvironment`, and four `SetThreadpoolCallback*` functions) are wrapped with header-inline API calls using `Library::inlineCall` / `Library::voidInlineCall`, explicit template arguments, `::` SDK names, and the `qor_pp_usesinlineapi` marker.
- Two threadpool callback names are declared but deliberately unwrapped: `SetThreadpoolCallbackPriority` and `SetThreadpoolCallbackPersistent`. The SDK exposes them only under `_WIN32_WINNT >= _WIN32_WINNT_WIN7` and `WINAPI_PARTITION_DESKTOP`, respectively, so wiring them into a multi-configuration library is an open decision.

Next actions:
- Review the two unwrapped threadpool names and the four shell entry points above.
- Extend export-bound/header-inline audit classification to other api_layer DLL libraries.

## winqor_gui documentation (docs/ui/windows)

- Incremental documentation of `winqor_gui` (`src/platform/os/windows/ui/gui`, 215 files, no unit tests — exercised only through `examples/experimental/osspecific/windows/mainwindow`) lives in `docs/ui/windows`: one page per area, grown one part per session, every claim cited to file and line. `docs/ui/windows/README.md` carries the part index, the ground rules, and the session protocol.
- Detailed so far: 10 core root, 20 factories, 30 controllers (levels 1–4), 40 window types, 50 view handlers (**root + `parts/` + `interactive/` + `rendered/` — all 74 files complete**), 90 mainwindow example, 60 gdiobjects (**60a+60b+60c — all 32 files complete**), and 70 painting (**all 8 `gui/painting/` files, 747 lines**), and 80 geometry/layout (**all 12 files, 1,111 lines**). Still summary: 00 overview only (kept high level by design) — the library walk is complete.
- Open next actions on that page set: no `summary` part remains except 00 (kept high level by design), so the
  remaining work is the per-part cross-questions on the pages - e.g. part 50's `StyleHandler::ProcessMessage`
  comparing `wParam == gwlExStyle` (a field index used as a message discriminator) and part 30's
  `SetPos(insertAfter = nullptr)` as the layout sink. The 26-27 Oct 2026 per-part walk narratives are in
  `memory-bank/archive/winqor-gui-doc-increments.md`.
