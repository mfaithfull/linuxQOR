# winqor_api_user - source review: STATUS (per-session boot brief)

Last updated 5 Nov 2026 - **session 36 (`windowprocedure.cpp`) CLOSED; 1 TU left**: `winqor_api_user.cpp` (the module
glue, 14 lines). Reviewing
`src/platform/os/windows/api_layer/user` (the `User32` wrappers over `user32.dll`) for correctness, consistency, safety,
UB and conformance to the api_layer conventions. One translation unit per session, and **commit at close**: a clean
`git status` plus this file is all a session needs to resume after a context reset.

## Record protocol v2 (adopted 5 Nov 2026, session 34)

The record grew to 88 files / 762 KB and the read-set this file told a session to open reached **269 KB**, which forced a
compaction before anything was measured and re-reading afterwards. v2 removes that cost. **No record content was
deleted** - the audit on 5 Nov found the prose records are primary, not duplicate: only 3 of the 38 sources carry an
in-source review banner, and sessions 16-20 exist nowhere outside the session-log rows. What v2 retires is the **derived**
layer - the part that had to be regenerated at every close and re-verified because it kept drifting. The superseded
`status.md` (session 33, 204 lines, with its locator table and per-session summaries) is kept verbatim at
`archive/status-v1-2026-11-04.md` - not boot material, wording reference only.

1. **The TU's in-source review block is the narrative.** Every reviewed TU gets a `//=== api_layer conventions ====`
   banner at the top of its `.cpp` (`windowclass.cpp:16-57` is the model: counts, the instrument slug behind each count,
   the verdict for each id it touches, and why anything was recorded rather than fixed). It cannot drift, because it sits
   on the code it describes, and it needs no index entry. `archive/tu/<tu>.md` (sessions 1-15, 21-33) stays as frozen
   history - open one only to research a closed TU.
2. **Write-once records.** A record file is created by the session that writes it and never edited afterwards; the only
   file a session edits is this one. Consequence: no slicing, no splices, no hash-verified rejoins, no line-count
   columns - and the ~40 KB ceiling stops being a live rule, because nothing grows any more.
3. **No generated index.** The id-to-file map is derivable from the id (below). Retired: the register locator, its
   generator `_tmp_status_index_gen_s33.ps1`, the README line-count refreshers, and `_tmp_s32_queue_audit.ps1`, which
   re-derived the queue by counting session-log rows. The queue table below is authoritative; `git log --oneline --` this
   folder and a grep for the review banner let a session confirm its state without re-deriving anything.
4. **Registers hold the rule, not a running tally.** Open a register only for an id being acted on, and read its entry
   head. Per-TU instances now go in the TU block, so `registers/u16.md`, `registers/u19.md`, `registers/u26.md` and
   `registers/census.md` are **frozen at their session-34 state**: kept for provenance, never read at session start, never
   appended to. A **new** open item still gets its own `registers/u<n>.md` - the shape is `u28.md`.
5. **Never cite `tmp/` artefacts in durable text.** `tmp/` is deleted at task end (`memory-bank/techContext.md:36-38`), so
   such a citation is dead as it is written; name the instrument's script slug instead. Retired with this rule: the
   `tmp/` cite audit and the locator verify.
6. **Boot read-set = this file + `method.md` + the current TU** (about 30 KB). Never open an `/EP` capture or
   `tmp/_tmp_*_pp_*.txt`; never read a whole register; `archive/` and `gotchas/sessions-*.md` are opened only to research
   a named closed session. **This file stays under 120 lines.**

## Queue - 1 TU left, alphabetical

| # | TU | lines | wrappers | status |
|--:|---|--:|--:|---|
| 37 | `winqor_api_user.cpp` (glue) | 14 | 0 | next |

Sessions 1-36 are closed: **37 of the 38 sources reviewed** (some sessions covered two TUs). History is write-once and is
not boot material: `archive/session-log*.md` (rows plus verdict detail, and the only narrative for sessions 16-20) and
`archive/tu/<tu>.md` (narratives for sessions 1-15, 21-33). Do not append to either.

## Session 36 - `windowprocedure.cpp` (closed 5 Nov 2026)

- Clean TU: 2/2 pairs in header order, 0 U4 / U11 / U13 / U17, 0 return or parameter divergence against `winuser.h:4146`,
  `:4159`, `:4181`, `:4190`, 2/2 bindings resolve (`CallWindowProcW`, `DefWindowProcW`), nothing to fix. Narrative: the
  banner at `windowprocedure.cpp:16-73`.
- Two measured facts that outlive the TU: `WNDPROC` is a genuine type check in this build, but a wrong **calling
  convention** procedure only fails it on x86 - the arch that does not build (U6); and `Get/SetWindowLongPtrT` answer
  `LONG_PTR`, not convertible to `WNDPROC` on either arch, so "read the procedure back out of `GWLP_WNDPROC` and hand it
  to `CallWindowProcT`" needs a cast no in-tree caller makes. U7 lands hardest here: `Library::Call`'s SEH handler hands a
  0 **to the OS** from a window procedure.
- U19: 1 live layer call (`ui/gui/window.cpp:106`, reached from 32 handler sites) plus 2 live SDK bypasses in the main
  window procedure itself (`windowfactory.cpp:71`, `:75`), which skip this layer's `qor_pp_fcontext` and SEH frame.

## Session 35 - `windowproperties.cpp` (closed 5 Nov 2026)

- Clean TU: 5/5 pairs, 5/5 bindings, nothing to fix; its two blemishes (a U14 spacing nit, the dead `returncheck.h`
  include) left as-is by user decision. Narrative: the banner at `windowproperties.cpp:16-77`. It also corrected a stale
  hand-off: `SetLayeredWindowAttributes` **is** defined, at `window.cpp:565`.

## Open items - id to file (derivable; there is no index to maintain)

- `U<n>` -> `registers/u<n>.md` | `P1`-`P5` -> `registers/prescan.md` | `P6`, `U27` -> `registers/p6-u27.md` |
  `G1`-`G6` -> `registers/ground-rules.md` | `T1`-`T7` -> `registers/tasks.md` | folder totals -> `registers/census.md`.
- Closed / frozen - append nothing: **P1, P2, P5** (4 Nov 2026: 428 bindings, 0 unresolvable), **U11** (session 29,
  3 Nov 2026), **U14** (D15), **U15** (D14), **U6**, **U3**.
- Open: U1, U2, U4, U5, U7, U8, U9, U10, U12, U13, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25, U26, **U28**,
  P3, P4, P6, T1-T7. Each statement and its measured detail is in the file the map names - open that file, never the tree.
- Decisions in force (D13, the return-check policy, D14, D15) are in `docs/agent/DECISIONS.md` and are not restated here.

## Gate (identical every session)

`tmp/` does not persist between sessions, so the gate is the **command line**, not a script to be found - recreate the
wrapper by slug if it is missing. Compile the whole folder with:

    cl /nologo /c /std:c++20 /EHsc /GR /utf-8 /W4 /DQOR_WINAPIUSER `
       /Dqor_pp_ver_major=0 /Dqor_pp_ver_minor=5 /Dqor_pp_ver_patch=0 `
       /I c:\RAID0\Develop\linuxQOR /Fo c:\RAID0\Develop\linuxQOR\tmp\obj\ <all 38 .cpp>

after `call vcvars64.bat` (`C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat`).
Edited TU standalone at `/W4` exit 0, then the whole library **38/38** exit 0 with no diagnostics. Date the gate by
`tmp/obj/<tu>.obj` being newer than the source - a clean 38-TU log **cannot** be dated by its hash, because clean runs are
byte-identical (this cost a re-run in sessions 31 and 34). `cl /EP` after the last edit must still yield the expected
binding literals. `user32.h` untouched unless the session says otherwise; a library-header edit additionally needs the
106-source all-libraries gate, read as a diff against a pre-fix run.

## Pre-flags for session 37 (`winqor_api_user.cpp`) - measured 5 Nov 2026 (the last TU)

Glue, not wrappers: 14 lines, 0 `User32::` definitions, 0 bindings, 0 `qor_pp_fcontext`, no `user32.h` / `library.h` /
`../returncheck.h` include - steps 1-5b and ids U4 / U11 / U13 / U17 / U18 / U19 / U25 / U26 have nothing to bite on. The
measures that do apply were run this session by `tmp/_tmp_s37_pre.bat` and `tmp/_tmp_s37_ep.bat` (both `/EP` captures read
only by a grep for `QORModule`, never opened).

- `user32.h:321` `//Window Procedure functions`, declarations `:322`-`:323`: **2 declarations, 2 definitions**, both
  `static`, both `T`-suffixed, defined in header order at `:16` `CallWindowProcT`, `:23` `DefWindowProcT` - U4 / U11 /
  U13 / U17 expected clean. 2 `qor_pp_fcontext`, 2 `qor_pp_useswinapiAW`, 2 explicit `Library::Call`, 0 `voidCall`,
  0 `CONST` / 0 `const` (U25 clean), 7 includes with `../returncheck.h` at `:11` unused - one more **T7**.
- Stems `CallWindowProc`, `DefWindowProc`: no bare export, `A` and `W` both exported (x64 capture, x86 capture alike), so
  the `AW` spelling is forced and P1 / P2 / P6 cannot arise here.
- SDK prototypes: `DefWindowProcA/W` at `winuser.h:4146`, `:4159` - `LRESULT` / `WINAPI`; the name sits after an
  `#ifndef _MAC` split (`:4139`-`:4145`) whose **taken** arm is `LRESULT` + `WINAPI` (`:4140`-`:4141`) and whose untaken
  arm is `LRESULT` + `CALLBACK` (`:4143`-`:4144`), so a reader who takes the three lines immediately above the name reads
  the wrong calling convention - the pre-flag script's "ret/conv" line does exactly that here.
  `CallWindowProcA/W` appear **twice**: `:4181`, `:4190` under `STRICT` with `_In_ WNDPROC lpPrevWndFunc`, and `:4207`,
  `:4216` under `!STRICT` with `_In_ FARPROC`. Guard depth 3 (`_WINUSER_` / `NOUSER` / DESKTOP **| GAMES**) on all four,
  no version floor - so U16 has nothing to record.
- `WNDPROC` itself is `LRESULT (CALLBACK*)(HWND, UINT, WPARAM, LPARAM)` at `winuser.h:91`, outside the `STRICT` arms, and
  there is **no** `typedef FARPROC WNDPROC`, so the layer's parameter survives `NO_STRICT` while the SDK's own prototype
  switches to `FARPROC`. `CallWindowProcT` is the only wrapper in the folder that hands a **caller-authored callback**
  straight to the OS: run the session-35 probe shape (`is_same` / `is_convertible`, `tmp/_tmp_s35_cbprobe.cpp`) against
  `WNDPROC` before writing the verdict, the same way U18's callback question was settled here.
- Liveness: **one live in-tree call between the two wrappers** - `ui/gui/window.cpp:106` (`User32::DefWindowProcT`), which
  `window.h:67` exposes as the `Window` member that 32 `view/handlers/**` sites call as `window.DefWindowProcT(...)`;
  `CallWindowProcT` has **0 callers** (D14: published surface, so no finding). The scan's single "BYPASS" row is its own
  match of the `Window::DefWindowProcT` **definition** (`window.cpp:104`), not an SDK call. Two genuine SDK bypasses are
  in `examples/experimental/osspecific/windows/retropad/retropad.cpp:890` and `controller.cpp:324` (`DefWindowProc` macro)
  - examples, so U19's scope note applies to them.
- Return semantics: `LRESULT` is an answer, never a status, so the session-27 soundness test bars any zero-check (U2 -
  and 0 `_Check_return_` on these prototypes); check the handler idiom that throws the value away
  (`... ? 0 : window.DefWindowProcT(...)`) rather than proposing a check. `:20` writes
  `Library::Call<LRESULT,WNDPROC, HWND, UINT, WPARAM, LPARAM>(` - glued comma and no space before the `(`: **U14 is frozen
  (D15)**, so book no tally, and record only what a decision asks for.
- Method reminders that survive v2: re-run the standalone compile after the banner is added; date the gate by `.obj`
  freshness; count `CONST` / `const` case-sensitively; write the banner from final measurements, not from interim scans.
