# winqor_api_user - source review: STATUS (per-session boot brief)

Last updated 5 Nov 2026 - **session 34 (`windowclass.cpp`) CLOSED; 3 TUs left**: `windowproperties.cpp` (51 lines),
`windowprocedure.cpp` (30), `winqor_api_user.cpp` (glue, 14) - **95 source lines remain**. Reviewing
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
| 35 | `windowproperties.cpp` | 51 | 5 | next |
| 36 | `windowprocedure.cpp` | 30 | 2 | - |
| 37 | `winqor_api_user.cpp` (glue) | 14 | 0 | - |

Sessions 1-34 are closed: **35 of the 38 sources reviewed** (some sessions covered two TUs). History is write-once and is
not boot material: `archive/session-log*.md` (rows plus verdict detail, and the only narrative for sessions 16-20) and
`archive/tu/<tu>.md` (narratives for sessions 1-15, 21-33). Do not append to either.

## Session 34 - `windowclass.cpp` (closed 5 Nov 2026)

- 14/14 declaration-definition pairs in header order; 0 U4 / U11 / U13 / U17; 0 return-type divergences against the SDK -
  the `LONG` vs `LONG_PTR` split the pre-flags flagged is faithful (`winuser.h:9868` against `:9908`, `:9927`, `:10031`,
  `:10050`), so no wrapper truncates; binding correct on both sides of D13 (12 `qor_pp_useswinapiAW` over stems that have
  `<Stem>A`/`<Stem>W` and no bare export, plus 2 bare `qor_pp_useswinapi` over `GetClassWord`/`SetClassWord`, which have
  no A/W family); 14/14 `cl /EP` literals resolve. Full detail: the banner at `windowclass.cpp:16-57`.
- **One new open item, recorded not fixed - U28** (`registers/u28.md`): the four `*Ptr` members bind
  `Get/Set*LongPtrW`, which exist only in the 64-bit image. `winuser.h:9897` and `:10020` put the `*Ptr` prototypes under
  `#ifdef _WIN64`, and the `#else` arms (`:9939`-`:9940`, `:9947`-`:9948`, `:10062`-`:10063`, `:10070`-`:10071`) make
  `*PtrA`/`*PtrW` **macros** over the non-`Ptr` entry points, so 32-bit `user32.dll` exports none of them. A live probe on
  both arches resolved 16/16 names on x64 and missed exactly those four on x86. Since `pFunc` is a function-local static
  const (G5), on x86 those four would cache NULL and throw `Missing library function.` on every call (U8). **Latent, not
  live** - x86 does not build at all (U6: 93 of 106 `api_layer` TUs fail at `kernel32.h:92`), so the arch-conditional
  binding fix travels with that blocker rather than with a TU pass.
- Gate: standalone compile of the edited TU exit 0, and the whole library **38/38, exit 0, 0 stale `.obj`** - both run on
  5 Nov 2026 after the +44-line banner (comment-only, no code change).

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

## Pre-flags for session 35 (`windowproperties.cpp`) - measured 5 Nov 2026

- `user32.h:594` `//Window Properties functions`, declarations `:595`-`:599`: **5 declarations, 5 definitions**, all
  `static`, all `T`-suffixed, defined here in header order at `:16`, `:23`, `:30`, `:37`, `:44` - so U4 / U11 / U13 / U17
  expected clean. 5 `qor_pp_useswinapiAW`, 5 explicit `Library::Call<...>`, 0 `voidCall` (no U18 converter class here).
- Stems `EnumProps`, `EnumPropsEx`, `GetProp`, `RemoveProp`, `SetProp`: re-dump the export table (`dumpbin /exports` of
  `C:\Windows\System32\user32.dll`) - `tmp/` is cleaned between sessions, so the old capture cannot be assumed to exist -
  and read the `cl /EP` literals rather than the source spelling (G1). If each stem has `A`/`W` exports and no bare
  export, the `AW` spelling is the only one that could bind, so P1 / P2 / P6 cannot arise in this TU.
- `GetPropT` / `RemovePropT` return `HANDLE`. `GetProp` hands back a handle the caller does not own, so weigh that return
  against **U20** (handle-ownership) rather than U18, and name the `winqor_gui` consumer one level deep.
- `LPCTSTR` parameters against the SDK's `LPCSTR` / `LPCWSTR` are U26's width convention - expected, not a defect. `:34`
  is missing the space before the call's `(`: a spacing nit only, and **U14 is frozen (D15)**, so book no tally.
- `#include "../returncheck.h"` at `:11` with 0 `CheckReturn` / `BoolCheck` / `Win32ErrorHandler` in the TU - one more
  **T7** dead include.
- **Correction to the session-34 hand-off.** The previous pre-flags placed `SetLayeredWindowAttributes` (with a
  double-gated `WINVER >= 0x0600` / `_WIN32_WINNT >= 0x0600` and the "first U18 type instance") in this TU at `:173` /
  `:211`. Measured 5 Nov 2026: that name appears **only at `user32.h:196`**, is declared `static`, is **not** in the Window
  Properties section, and has no definition in any of the 38 sources - so it is a **U4** (declared, never defined)
  candidate for the U4 sweep, not a session-35 defect, and it must not be "fixed" without that decision. A stale
  hand-written cross-reference is precisely what v2 stops maintaining.
- Method reminders that survive v2: re-run the standalone compile after the banner is added; date the gate by `.obj`
  freshness; count `CONST` / `const` case-sensitively; write the banner from final measurements, not from interim scans.
