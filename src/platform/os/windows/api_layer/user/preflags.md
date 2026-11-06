# Folder-level leads - the TU queue is complete (session 37, 5 Nov 2026)

Written wholesale at the session-37 close and amended at the **D18 / purpose.md decision the same day** (5 Nov 2026). All
38 sources in `src/platform/os/windows/api_layer/user` are reviewed, so there is no next TU and this file no longer
pre-flags one; it lists what a further session in this folder would pick up, and its lead is T8.
**Write-once**: read it once at boot, edit it never, replace it wholesale if the focus changes.

## The actionable lead: T8 - the U18 conformance pass (5 Nov 2026)

The folder now has a purpose statement, `purpose.md`: the library "intends to provide a full set of wrapper functions for
the exports of the User32.dll Windows system library". That is policy **D18** in `docs/agent/DECISIONS.md`, and it changes
what counts as a defect here: a wrapper's published signature is the `winuser.h` prototype for the export it binds, and an
export with no wrapper is a gap. The work is **T8** (`registers/tasks.md`), the measured sites are the D18 block at the
foot of `registers/u18.md`. Headline measurements (5 Nov 2026): **1,018 export names**, **525 members**, **428 bindings**,
**648 exports already matched**, **117 SDK-declared exports unwrapped**, **253 unwrapped with no prototype**; U18's own
site list is 1 return type + 4 parameter types + 16 parameter names + 2 missing `*ForPerMonitorDPI` members.

Scope was decided the same day (**D18 addendum**): documented set only, superseded families (DDE, `keybd_event` /
`mouse_event`) excluded - so the 253 undocumented exports are out of scope and the backlog is **109 names**, dispositioned
by family in `registers/u18-coverage.md`.

T8 step 0 is **mostly settled**: scope is the documented set minus superseded families (D18 addendum). One sub-question is
still open - U17's `A`/`W` suffix policy - but it moves the backlog by one wrapper, and it does not gate T8 steps 1-5,
which are the fixes U18 was actually opened over. Start at T8 step 1.

## Where the per-TU record is

Each reviewed `.cpp` carries its own `//=== api_layer conventions ====` banner at the top - that is the narrative, and
`winqor_api_user.cpp:4-72` is the last of them. `status.md` holds the queue (all rows `closed`), the id-to-file map and
the compile gate. Sessions 1-15 and 21-33 also have `archive/tu/<tu>.md` (frozen, not boot material).

## Open work in this folder, by class

- **Fixable defects: none queued by a TU pass, but D18 creates a class of them.** Every TU-level defect found in sessions
  1-37 was either fixed in place or booked to an open item, and the queue having emptied means there is no known unfixed
  wrapper. Under `purpose.md` the published-signature divergences collected in `registers/u18.md` are no longer neutral
  surface decisions - they are the defect population, and T8 is how they get fixed.
- **Open items** (rule + decision + measured detail in `registers/u<n>.md`): U1, U2, U4, U5, U7, U8, U9, U10, U12, U13,
  U16, U17, U18, U19, U20, U21, U22, U23, U24, U25, U26, **U27** (in `registers/p6-u27.md` - `STRICT` changes the identity
  of `HOOKPROC` and of every `DECLARE_HANDLE`, so 3 of `hook.cpp`'s 4 wrappers change published name between the arms).
  Frozen - append nothing: P1, P2, P5, U11, U14 (D15), U15 (D14), U6, U3. **U28 is CLOSED 5 Nov 2026 by D17**: the four
  `*Ptr` wrappers in `windowclass.cpp` bind names only the 64-bit `user32.dll` image exports, which is now simply the
  correct spelling for the only supported target, so they need no change.
- **Tasks T1-T6 and T8** (`registers/tasks.md`) and **P3, P4, P6** are the other actionable entries - **T8 is the lead**,
  the U18 conformance pass this file opens with. **T7 is CLOSED (5 Nov 2026,
  D16)** - the 35 dead `#include "../returncheck.h"` lines are deleted, only `processthread.cpp` and `errorhandling.cpp`
  still include it, and the scan (`_tmp_rcinclude_scan.ps1`, include test now line-anchored) reads **2 includes /
  0 without use**. What T7 left behind is **the citation shift**: deleting line 11 moved every line below it up by one in
  those 35 files, so a bare `:NNN` self-citation written under line 11 reads **one lower** than the source now says
  (703 bare tokens with `N >= 12` measured there, 315 of them candidates, 388 already naming other files). Repairing it
  is a semantic paragraph-level pass of its own - `gotchas/evidence-integrity.md` has the inheritance rule - not a TU pass.
- **Architecture: x64 only** - user decision **D17** (5 Nov 2026, `docs/agent/DECISIONS.md`; in-tree note
  `src/platform/os/windows/purpose.md:3`). The x86 blocker that used to sit in this list is **closed as not-to-be-fixed**:
  session 31's measurement (93 of 106 `api_layer` TUs failing an x86 compile at `kernel32.h:92`) stands as history, the
  kernel-header follow-up sketched in `registers/u6.md` is withdrawn, and **no x86 compile is a step in any gate here** -
  which also retires the two-arch legs of the review's mirror/live probes (`tooling.md`).

## One measured lead deliberately left unregistered (session 37)

`qor_pp_ver_major/minor/patch` are supplied only by `target_compile_definitions` (this folder's `CMakeLists.txt:45-48`).
Compile without them and the module glue TU builds **clean at `/W4`, exit 0**: `qor_pp_stringize_i` is `#text`
(`src/macros/stringize.hpp:43`), so the module's version string becomes the literal `"qor_pp_ver_major" "." ...`.
The build number cannot go missing silently (the `#ifdef NDEBUG` include of `buildnumber.h` hard-fails with `C1083`);
the version triple can, in each of the **163** `src/**/CMakeLists.txt` that pass `qor_pp_ver_major` (measured 5 Nov 2026).
User decision 5 Nov 2026: **no register entry** - it is a build-system change (`cmake/`, or an `#error` in
`src/macros/moduleversion.hpp`), not a folder-pass one. If it is ever acted on, do it there, and re-run the
all-libraries gate.

## Gate if this folder is touched again

Unchanged, and it is the only gate: the edited TU standalone at `/W4`, then the folder's 38 sources in one `cl`
invocation - command line in `status.md`, "Gate" section. Date it by `tmp/obj/<tu>.obj` being newer than the source.
