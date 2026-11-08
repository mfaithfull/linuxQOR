# Folder-level leads - the review queue, all of T8 and T1-T6 are closed; T2 is the only open task

Written wholesale at the T6 close (8 Nov 2026) and replaced wholesale at every later close. All 38 review-queue sources are `closed` and the nine coverage TUs `pointer.cpp` (6a),
`dpi.cpp` (6b), `touch.cpp` (6c), `displayconfig.cpp` (6d), `powermanagement.cpp` (6e), `misc.cpp` (6f), `legacy.cpp` (6g), `security.cpp` (T1) and `listbox.cpp` (T6) are closed with them,
so there is **no next review TU and no family tranche left**: U18 and T8 are CLOSED (D19 put the last 6 names in scope,
D20 gave `wsprintfA`/`wsprintfW` a variadic wrapper). This file lists what a further session in this folder picks up.
**Write-once**: read it once at boot, edit it never, replace it wholesale if the focus changes.

## Current folder figures (re-measured 8 Nov 2026 with `_tmp_static_census.ps1`, then `_tmp_u18_coverage4.ps1` + `_tmp_u18_buckets.ps1` run in sequence)

**47 sources / 634 member-declaration lines (548 static, 86 non-static) / 547 binding lines / 1,018 export names / 759 covered / 8 declared-and-unwrapped raw / 251 with no
prototype.** `_tmp_static_census.ps1` also prints 0 defined-but-not-static, 0 defined-without-a-declaration and 547 distinct defined member names; the coverage instrument counts 635 members, so quote the instrument with the figure. The raw 8 is the whole remaining gap and every one of the 8 carries a written exclusion: the 7 superseded twins
(`Get/SetClassLongA/W`, `SetWindowsHookA/W`, `UnhookWindowsHook`, whose `*Ptr`/`Ex` replacement this library already wraps) and
the classifier artifact `SetWindowsHookExAW`, which no header declares. The buckets instrument prints **0 for every family
except those two all-exclusion rows** - `Other` is 0 and the 6 legacy names are out of the gap - so the backlog is empty and
there is no coverage lead to pick up.

## What is left to work on in this folder

- Open items the `status.md` map names: U1, U2, U4, U5, U7, U8, U9, U10, U12, U13, U17, U19-U27 less the frozen ids,
  P3, P4, P6 and T2. **T1, T3, T4, T5, T6, T7 and T8 are CLOSED** (8 Nov 2026: `security.cpp` defines the Authorization pair + `static` at
  `user32.h:760-761`; `powermanagement.cpp` defines the Power Management pair + `static` at `user32.h:788-789`; `help.cpp`
  defines the menu context-help-id pair + `static` at `user32.h:744-745`; U4's and U18's instances for all three pairs are
  gone, the ids stay open for the rest of the folder - the DDE section and the four System Information members are still declared and defined nowhere - and
  `ime.cpp` closed T5 by wrapping the IME/WINNLS block, and a new `listbox.cpp` closed T6 by
  defining `DlgDirList` (exports C1/C4, bound AW), `DlgDirSelectEx` (C7/C8, bound AW) and `GetListBoxInfo` (174, bound
  bare) with `static` at `user32.h:552-554` and the SDK's parameter names on the `DlgDirSelectEx` declaration). **U16 is CLOSED** (7 Nov 2026, D21) - it is no longer on that list, and `registers/u16.md` is frozen provenance, so do not open it at session start. None of the remaining ids is a coverage gap; U17 (the A/W suffix
  question) is unaffected by 6g, which kept `wsprintfA`/`wsprintfW` as **one** `wsprintfT` member over
  `qor_pp_useswinapiAW`.
- What D21 changed under every session's feet: `kernel32.h:22-30` now pins `_WIN32_WINNT`/`WINVER` `0x0A00` and
  `NTDDI_VERSION` `0x0A000010` (`NTDDI_WIN11_GE`) before `<Windows.h>`, guarded `#ifndef`, with a floor `#error` at
  `:33-35`. Two consequences for any edit here: do not lower those macros and do not re-`#define` them - a consumer
  that pins a lower floor before `kernel32.h` now fails with one `C1189` rather than with a batch of `C2039`/`C2061`;
  and because the pin is in a shared header, any edit that touches `kernel32.h` needs the 113-source all-libraries
  gate as a diff. The version triple is pinned, so an SDK prototype that vanished behind a `WINVER`/`NTDDI` guard is
  no longer a U16 finding - the shrink macros (`NOGDI`, `NOSYSPARAMSINFO`, `NOUSER`, `NOCTLMGR`, `NOMB`, `NOMSG`,
  `WINAPI_FAMILY`) are **not** pinned and remain findings if a consumer sets them.
- The one shape this folder now carries that no other folder does: `wsprintfT` at `user32.h:835-840` is the only
  header-inline member and `library.h`'s `varCall` (variant (V), `__cdecl` variadic `fPtr`) its only variadic call form.
  A session that adds another `WINAPIV` export or edits that section has to keep the body in the header - a body defined
  in one TU leaves consumers unresolved (measured LNK2019/LNK1120) - and keep the `__cdecl` spelling, which is what a
  32-bit build would need even though this tree is x64-only (D17).
- Gate for any edit here: 47/47 folder at `/W4`, and because `library.h` and `user32.h` are shared, a library-header edit
  needs the all-libraries run as a **diff** against a pre-fix run (115 sources since `legacy.cpp` + `security.cpp` + `listbox.cpp`; standing baseline exit
  **2**, 1 `C1083` at `opengl/wgl.cpp:31`, 165 warning lines - the diff is the signal, never the exit code).
