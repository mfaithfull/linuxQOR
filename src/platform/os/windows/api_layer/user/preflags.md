# Folder-level leads - the TU queue is complete (session 37, 5 Nov 2026)

Written wholesale at the session-37 close. All 38 sources in `src/platform/os/windows/api_layer/user` are reviewed, so
there is no next TU and this file no longer pre-flags one; it lists what a further session in this folder would pick up.
**Write-once**: read it once at boot, edit it never, replace it wholesale if the focus changes.

## Where the per-TU record is

Each reviewed `.cpp` carries its own `//=== api_layer conventions ====` banner at the top - that is the narrative, and
`winqor_api_user.cpp:4-72` is the last of them. `status.md` holds the queue (all rows `closed`), the id-to-file map and
the compile gate. Sessions 1-15 and 21-33 also have `archive/tu/<tu>.md` (frozen, not boot material).

## Open work in this folder, by class

- **Fixable defects: none queued.** Every TU-level defect found in sessions 1-37 was either fixed in place or booked to
  an open item; the queue having emptied means there is no known unfixed wrapper.
- **Open items** (rule + decision + measured detail in `registers/u<n>.md`): U1, U2, U4, U5, U7, U8, U9, U10, U12, U13,
  U16, U17, U18, U19, U20, U21, U22, U23, U24, U25, U26, **U28**. Frozen - append nothing: P1, P2, P5, U11, U14 (D15),
  U15 (D14), U6, U3. The newest, **U28** (`registers/u28.md`), is four wrappers binding entry points that exist only in
  the 64-bit `user32.dll` image; it is latent because x86 does not build at all.
- **Tasks T1-T6** (`registers/tasks.md`) and **P3, P4, P6** are the other actionable entries. **T7 is CLOSED (5 Nov 2026,
  D16)** - the 35 dead `#include "../returncheck.h"` lines are deleted, only `processthread.cpp` and `errorhandling.cpp`
  still include it, and the scan (`_tmp_rcinclude_scan.ps1`, include test now line-anchored) reads **2 includes /
  0 without use**. What T7 left behind is **the citation shift**: deleting line 11 moved every line below it up by one in
  those 35 files, so a bare `:NNN` self-citation written under line 11 reads **one lower** than the source now says
  (703 bare tokens with `N >= 12` measured there, 315 of them candidates, 388 already naming other files). Repairing it
  is a semantic paragraph-level pass of its own - `gotchas/evidence-integrity.md` has the inheritance rule - not a TU pass.
- **Architectural blocker**: x86 - session 31 measured 93 of 106 `api_layer` TUs failing at `kernel32.h:92`
  (`registers/u6.md`, frozen). Nothing in this folder can be validated on x86 until that moves.

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
