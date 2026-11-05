# winqor_api_user - source review: STATUS (per-session boot brief)

Last updated 5 Nov 2026 - **session 35 (`windowproperties.cpp`) CLOSED; 2 TUs left**: `windowprocedure.cpp` (30 lines),
`winqor_api_user.cpp` (glue, 14) - **44 source lines remain**. Reviewing
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

## Queue - 3 TUs left, alphabetical

| # | TU | lines | wrappers | status |
|--:|---|--:|--:|---|
| 36 | `windowprocedure.cpp` | 30 | 2 | next |
| 37 | `winqor_api_user.cpp` (glue) | 14 | 0 | - |

Sessions 1-35 are closed: **36 of the 38 sources reviewed** (some sessions covered two TUs). History is write-once and is
not boot material: `archive/session-log*.md` (rows plus verdict detail, and the only narrative for sessions 16-20) and
`archive/tu/<tu>.md` (narratives for sessions 1-15, 21-33). Do not append to either.

## Session 35 - `windowproperties.cpp` (closed 5 Nov 2026)

- 5/5 declaration-definition pairs in header order (`user32.h:595`-`:599`, definitions at `:79`, `:86`, `:93`, `:100`,
  `:107` post-banner), all `static`, all `T`-suffixed, 5/5 `qor_pp_fcontext`: 0 U4 / U11 / U13 / U17. 0 return-type
  divergences against the SDK - `int`, `int`, `HANDLE`, `HANDLE`, `BOOL` against `winuser.h:8969`, `:8950`, `:8913`,
  `:8931`, `:8894` - so U18 clean, and the three `LPCTSTR` parameters are exact under this build (`is_same<LPCTSTR,LPCWSTR>
  =1`), i.e. U26's mapping, not a reinterpret. Full detail: the banner at `windowproperties.cpp:16-77`.
- Binding D13-clean: a fresh x64 `dumpbin /exports` (and the session-22 x86 capture) gives every stem `<stem>A` + `<stem>W`
  and **no bare export**, so `qor_pp_useswinapiAW` is the only spelling that could resolve; `cl /EP` after the banner: 5
  literals `EnumPropsW`, `EnumPropsExW`, `GetPropW`, `RemovePropW`, `SetPropW`, module `L"user32"` 5/5, 0 unresolved,
  0 `WW`, 0 `T`; wrong-stem scan 0 hits here (folder: 428 bindings, 2 flagged, both older TUs).
- **The two callback parameters are type-checked, not `FARPROC`.** Probe (`tmp/_tmp_s35_cbprobe.cpp` + `.bat`) measures
  `is_same<PROPENUMPROC,PROPENUMPROCW>=1`, `is_same<PROPENUMPROCEX,PROPENUMPROCEXW>=1`, and
  `is_convertible<consumer-callback,PROPENUMPROC>=1` is **not** a silent widening: `BOOL` **is** `int` and `LPCTSTR` **is**
  `LPCWSTR`, so `windowcontroller.cpp:684` is the same type as the SDK's `PROPENUMPROCW`. Only a `NO_STRICT` consumer
  turns them into `FARPROC` (`winuser.h:169`, `:172`) - and `STRICT` is set by the SDK itself (`minwindef.h:23`,
  `ntdef.h:787`), which `hook.cpp:67` already records.
- U1/U2: 0 handlers, 0 checks, and **0 `_Check_return_` on all 10 A/W prototypes**, and no check would be sound: `GetProp`
  and `RemoveProp` answer `NULL` for "no such property", which is also what `Library::Call` returns for a failed call
  (U7), so the layer cannot separate "absent" from "call failed"; `EnumPropsT`/`EnumPropsExT` return an `int` in which 0
  is legal. U20: those `HANDLE`s carry **pointers** the consumers cast back (`windowfactory.cpp:51`, `dialog.cpp:57`), so
  this TU feeds U20 through the pointer-as-handle channel with no destroy path on it. U19: 9 live callers, all through the
  layer; the four `GetPropW`/`SetPropW` sites in `gl/windowimpl.cpp` (`:534`, `:1427`, `:1597`, `:2131`) are inside block
  comments opened at `:530`, `:1303`, `:2092`, so 0 live bypass. U16: guard depth 3 (`_WINUSER_` / `NOUSER` / DESKTOP)
  with **no version floor** on any of the five.
- Two blemishes, **left as-is by user decision 5 Nov 2026**: the `:97` spacing nit (U14 frozen by D15) and the dead
  `../returncheck.h` include at `:11` (T7 waits for the folder-wide go-ahead, not a per-TU pass).
- **Hand-off correction.** Session 34's pre-flags said `SetLayeredWindowAttributes` is declared-without-definition. Wrong:
  declared `user32.h:196`, **defined** `window.cpp:565` with bare `qor_pp_useswinapi` (`:568`) - the D13-conformant
  spelling for a name with one export on both arches and no A/W family (prototype `winuser.h:4633`, DESKTOP family, no
  floor). Not a U4 candidate, and not this TU's: it belongs to `window.cpp` (session 33).
- Gate: standalone `/W4` exit 0, then the whole library **38/38, exit 0, no diagnostics**, both after the +63-line banner
  (comment-only); `tmp/obj/windowproperties.obj` dated after the source, and `cl /EP` re-run after the last edit still 5/5.

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

## Pre-flags for session 36 (`windowprocedure.cpp`) - measured 5 Nov 2026

Captures already made this session: `tmp/_tmp_user32_exports_s36.txt` (x64 export table) and `tmp/_tmp_win_pp_s36.txt`
(`cl /EP` of this TU, digest `tmp/_tmp_s36_ppclass.txt`: 2 binding literals `CallWindowProcW`, `DefWindowProcW`, module
`L"user32"` 2/2, 0 unresolved, 0 `WW`, 0 `T`). `tmp/` is git-ignored scratch and does get cleaned, so re-run
`tmp/_tmp_win_pp_s36.bat` first if they are gone, and re-read the `cl /EP` literals rather than the source spelling (G1).

Every number below is from one run: `tmp/_tmp_s36_pre.ps1` -> `tmp/_tmp_s36_pre.txt` (this session's pre-flag script,
copied from `tmp/_tmp_s35_pre.ps1` with the TU, stems and capture paths swapped), plus `tmp/_tmp_s36_ppclass.ps1`.

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
