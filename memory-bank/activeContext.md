# Active Context

Last updated: 2 Nov 2026 (session 25 `mouse.cpp` CLOSED - **10 P1 binding fixes**, the review's largest single-TU binding
defect: all ten wrappers asked `GetProcAddress` for a `*W` that `user32.dll` has never exported). Session 24 is **committed**
as `3a66dc25` (2 Nov 21:40, which also reorganised this memory bank); session 25's working tree is **13 modified + 2 new**
per `git status`. **Next:
session 26 `painting.cpp`** (25 wrappers, `user32.h:262`-`:287`), pre-flags measured and in `status.md`. Record sizes:
`status.md` 174 lines / ~16.6 KB of its 180-line ceiling; **`method.md` 405 lines / 41,466 B is OVER its 40 KB ceiling -
slice it before adding to it.** Completed-session records stay in `memory-bank/archive/`, never here.

## Current focus: winqor_api_user source review (one TU per session)

- Reviewing `src/platform/os/windows/api_layer/user` (class `qor::win::api::User32` over `user32.dll`) for
  correctness, consistency, safety, UB and convention conformance. **Protocol: exactly one translation unit
  per session**; small unambiguous defects fixed in place, everything behavioural recorded as an open item.
- **Session 25 (`mouse.cpp`) closed 2 Nov 2026 - 10 P1 + 1 P5 + 1 U14(b) fixed; 26 of 38 TUs reviewed.** All ten bindings
  were `qor_pp_useswinapiAW` over stems with **no A/W family at all** (the inverse of the P2 class): `winuser.h` defines no
  `#define <stem>` for any of them and `user32.dll` exports only the bare name, so `##W` asked for `DragDetectW` ...
  `LPTRACKMOUSEEVENTW` and every call threw from `library.h:50`. Proven by the `/EP` literals plus a live `GetProcAddress`
  probe (all 10 `MISSING`, all 10 bare stems `RESOLVED` at the export-table RVAs), so bare `qor_pp_useswinapi` is **D13's
  other half**, not a style call. The old `:82` also bound the parameter *type* - **P5's second instance** - so folder
  unresolvable bindings went **11 -> 1** of 428 and live wrong-stem sites **1 -> 0**; U14(b) **425 explicit / 3 deducing**.
  Narrative `archive/tu/mouse.md`, row 25; gate `tmp/_tmp_cl_all_s25c.log` (38/38 `/W4`, exit 0, dated after the last edit).
- Session 24 (`monitor.cpp`, 0 fixes - the first clean TU) is committed in `3a66dc25`; its detail is `archive/tu/monitor.md`
  + row 24 of `archive/session-log.md`, not here.

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
   whole tree. `../method.md` holds the procedure and the instrument caveats.
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
