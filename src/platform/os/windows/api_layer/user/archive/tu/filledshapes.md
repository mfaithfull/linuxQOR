# narrative: filledshapes.cpp, session 15

## filledshapes.cpp - session 15 (31 Oct 2026)

- **Scope:** the "Filled Shapes functions" section, `user32.h:608-611` - **3 declarations** (`:609-611`), 3 definitions in
  the header's order, all `static`, all defined `User32::`-qualified. No U4 member (the section is fully covered here), no
  U11, no U13. The TU was 38 lines before this session and 99 after (the 61-line section comment now at `:16-76`);
  `user32.h` untouched; the only code-side edit is removing one stray blank line between the comment and the first
  definition. In `add_library` at `user/CMakeLists.txt:31`.
- **0 binding / correctness / UB defects, and the bare spelling is provably right for all three.** `cl /EP`
  (`tmp/_tmp_pp_fs.bat` -> `tmp/_tmp_fs_pp.txt`) yields the literals `"FillRect"`, `"FrameRect"`, `"InvertRect"`
  (`:453304`, `:453311`, `:453318` of the post-edit run, in definition order); the export table gives 110, 117, 226
  (`tmp/_tmp_user32_exports.txt:1779`, `:1786`, `:2057`); and the extended live probe reports all three **RESOLVED** with
  `FillRectA/W`, `FrameRectA/W`, `InvertRectA/W` **MISSING**. So `qor_pp_useswinapiAW` would have broken all three
  bindings - this is the D13 case again, and it is worth stating because the three signatures look exactly like the A/W
  families elsewhere in the folder (`Rect`+`HBRUSH` arguments, no `T` suffix, and the SDK does spell the pointer
  `CONST RECT *`). None of the three stems is among the nine `user32.h:9-17` `#undef`s and none has a `stem -> stemW`
  macro in `winuser.h` (`tmp/_tmp_winuser_aw_macros.txt` has no match for any of them), so neither half of G1 can fire and
  the source spelling is literally the `GetProcAddress` argument. `tmp/_tmp_wrong_stem_scan.ps1` flagged **nothing** in
  this file.
- **New U25 from this TU - the `CONST::RECT*` spelling.** `CONST` is the SDK's object-like macro for `const`
  (`shared/minwindef.h:153`), so the header's `CONST::RECT*` and the call sites' `CONST ::RECT*` are one type written two
  ways; the `/EP` dump proves it (`tmp/_tmp_fs_pp.txt:453301`: `int User32::FillRect(HDC hDC, const::RECT* lprc, HBRUSH
  hbr)`). Recorded, not fixed: 17 header lines use `CONST::`, 7 use `const ::`, and the same family reaches
  `rectangle.cpp`, `painting.cpp` and `text.cpp`, so it joins the folder-wide style pass with U14(a). The only real smell
  found inside that family (`user32.h:571`'s `CONST::LPINT`) is U23-shaped and is recorded in the U25 entry for
  `text.cpp`'s session.
- **U18 - no new instance.** The SDK is `int FillRect(...)`, `int FrameRect(...)`, `BOOL InvertRect(...)`
  (`winuser.h:9726-9747`), which is exactly what `user32.h:609-611` publishes, so the `int`/`int`/`BOOL` returns need no
  change and the failure values (0 / 0 / FALSE) are at least representable. U1/U2 instance: 3 call sites, 0
  `Win32ErrorHandler`, 0 `CheckReturn` - per the settled policy, so a failed paint is silent (and per U7 an SEH fault
  inside the callee would look identical to a legitimate 0). The only in-tree code that reads any of the three results is
  `ui/gui/gdiobjects/devicecontext.cpp:85`, which folds `InvertRect`'s `BOOL` into `bool`; the two `int` results are never
  inspected.
- **U16 - this TU adds 0 of 3, and it is the first TU whose guard stack has no version macro at all.** Measured with
  `tmp/_tmp_sdk_guard_scan.ps1` on `winuser.h` 10.0.26100: the prototypes at `:9729`, `:9737`, `:9745` are open only under
  `#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)` (`:9717`) inside `#ifndef NOUSER` (`:64`). There is also no
  type-level sub-case, and the direction is the **opposite** of the `HDEVNOTIFY`/`FLASHWINFO` shape U16 usually records:
  `RECT` is declared at `shared/windef.h:154-160` under the *Application/Games/System* partition (`:152`), i.e. in a
  broader partition than the prototypes that use it, so a non-Desktop partition would delete the SDK prototypes while
  `user32.h:609-611` keeps compiling - which is harmless here because the wrapper binds by name, never through the
  prototype.


- **Section coverage (U21's shape again - the header's section is a slice of the SDK's block, and the SDK's block is
  spread over three of this library's TUs).** The SDK's own region is the contiguous Desktop-Family block
  `winuser.h:9716-9852`: `DrawFocusRect`, `FillRect`, `FrameRect`, `InvertRect`, `SetRect`, `SetRectEmpty`, `CopyRect`,
  `InflateRect`, `IntersectRect`, `UnionRect`, `SubtractRect`, `OffsetRect`, `IsRectEmpty`, `EqualRect`, `PtInRect` - 15
  entry points - plus `GetWindowWord`/`SetWindowWord` inside `#ifndef NOWINOFFSETS` (`:9832-9847`). This library declares
  **15 of the 15** but files them in three places: the 3 here, the 11 rect helpers at `user32.h:447-457`
  (`rectangle.cpp` - `CopyRect`, `EqualRect`, `InflateRect`, `IntersectRect`, `IsRectEmpty`, `OffsetRect`, `PtInRect`,
  `SetRect`, `SetRectEmpty`, `SubtractRect`, `UnionRect`, 11 definitions), and `DrawFocusRect` at `user32.h:267`
  (`painting.cpp:44`). So this session's section is complete on its own terms and the folder's coverage of the SDK block
  is complete too - what is missing is two exported names the header never declares at all: **`AlignRects`** (ordinal 6)
  and **`EvaluateProximityToRect`** (ordinal 10D), both **RESOLVED** in the probe, both Windows 8-era additions that
  Microsoft documents as internal ("do not use") - which is probably why they were skipped, and both are candidates for
  the same "declared nowhere" list U21 keeps.
- **Convention counts:** U13 0 of 3; U14(b) 0 deducing (3/3 explicit - all three spell the full SDK argument list);
  U17 0 (no A/W family, so no `T` suffix is due); U4 0; U11 0; U15 **3 of 3**; U19 **3 LAYER / 3 INDIRECT / 0 BYPASS /
  0 MEMBER** (`tmp/_tmp_fs_bypass.txt`); U16 **0 of 3**; U25 **3 declaration lines + 3 call-site template-argument
  spellings** (the largest single-TU contribution to the folder-wide `CONST::` family so far).
- **Liveness (U15):** each wrapper has exactly **one** call site, all three in `DeviceContext::FillRect`/`FrameRect`/
  `InvertRect` (`ui/gui/gdiobjects/devicecontext.cpp:75`, `:80`, `:85`, built at `gui/CMakeLists.txt:44`). Those three
  methods are non-virtual (`devicecontext.h:232-234`) and have **no call site anywhere** in `src`/`examples`/`test`, so
  all three wrappers are compiled, exported and never executed. The distinguishing feature against the other dead chains:
  the device context they would run on *is* live and *is* used by built code - `examples/.../mainwindow/mainwindow.cpp:84`
  (`WindowController::GetDC`) and `:173` (`PaintSession::GetDC`) both produce a `DeviceContext`, and `OnPaint` calls
  `GetTextExtentPoint32T`/`TextOutT` on it at `:179`, `:180`, with a `Rect` in hand at `:174` - a real painted window with
  a real DC and a real rect that simply never goes through `FillRect`. That makes this the most "reachable" dead chain in
  the folder so far, and the reason the caller-side cast below had to be checked rather than waved away.
- **Caller-side cast checked rather than assumed:** the three sites pass `reinterpret_cast<const RECT*>(&rc)` where `rc`
  is `qor::platform::win::Rect` (`ui/gui/view/drawing/rect.h:33-38`: `long m_left, m_top, m_right, m_bottom`) against
  `RECT` (`LONG left, top, right, bottom` at `shared/windef.h:154-160`). Field-for-field the same order and widths on this
  toolchain, so the alias is sound in practice - the same hand-mirror pattern as `COMBOBOXINFO` (U15) and
  `MSGBOXPARAMS` (U23), and now confirmed on a **by-value-painted** path rather than a struct-with-`cbSize` path. No UB
  observed, but it is still a `reinterpret_cast` between unrelated class types, i.e. technically an invalid alias: the
  gui side should convert (or derive), which is a `winqor_gui` change and the third instance of the same underlying
  decision. Recorded under the U23/U15 mirror-struct notes rather than as a new item.
- **Instrument note for later sessions (second `sigcmp` false-positive class):** `_tmp_sigcmp.ps1` compares the
  declaration's parameter spelling with the `Library::Call<>` template argument spelling as **strings**, so any site where
  the header writes `CONST::X*` and the call site writes `CONST ::X*` prints `CALL TYPE MISMATCH` - here 3 of 3 sites,
  all false (U25). Read the pair before believing the row: after preprocessing both are `const ::RECT*`. Added to the
  known-bad rows from session 14 (the `voidCall` row prints `(no Library::Call - voidCall or inlineCall?)` by design).
  `_tmp_name_divergence_scan.ps1` on this TU: 0 of 3 definitions, 0 parameter-name divergences - the folder's zero
  (established by session 12, re-confirmed by 13 and 14) holds for a fourth consecutive TU.
- **Verified at exit:** `filledshapes.cpp` compiles standalone at `/W4` **exit 0** after every edit
  (`tmp/_tmp_compile_one.ps1`); the whole library compiles **38/38 exit 0** with the edits in place
  (`cmd /c tmp/_tmp_cl_all.bat` -> `cl_exit=0`, `tmp/_tmp_cl_all.log` 42 lines, `filledshapes.cpp` echoed, and the only
  line matching `error` is the file name `errorhandling.cpp`); the post-edit `/EP` re-run (`pp_exit=0`) still yields the
  three bare literals in definition order; `user32.h` untouched (`git diff --stat` for the folder: 1 file changed,
  `filledshapes.cpp` +62/-1).
- **Instruments this session leaves behind:** `tmp/_tmp_pp_fs.bat` (+ `tmp/_tmp_fs_pp.txt`, `tmp/_tmp_fs_pp_err.txt`),
  `tmp/_tmp_fs_bypass.txt`, and `tmp/_tmp_name_resolve_probe.cpp` extended with 12 filled-shape spellings - the 3 stems x
  3 spellings plus `DrawFocusRect`, `AlignRects`, `EvaluateProximityToRect`, the last two being the coverage probes for
  the names this header omits - plus 5 help-family names pre-staged for session 16 (`WinHelpA`,
  `Set`/`GetWindowContextHelpId`, `Set`/`GetMenuContextHelpId`; all five **RESOLVED** in the post-edit run, and `WinHelp`
  itself **MISSING** with `WinHelpA`/`WinHelpW` **RESOLVED**, re-confirmed from the `:51-52` entries an earlier session
  added). `tmp/_tmp_cl_all.ps1`'s `pwsh -File` wrapper returned exit 1 this session while the generated
  `tmp/_tmp_cl_all.bat` it writes returned `cl_exit=0` with a clean 42-line log - run the `.bat` and read the log directly
  rather than trusting that wrapper's exit code.
  **Encoding trap checked rather than "fixed" (session 15):** `status.md` and the memory-bank files contain en dashes,
  em dashes and ellipses, and they are committed convention - counting them by piping `git show HEAD:<path>` through
  PowerShell reports **0** of each because PowerShell re-decodes the blob (each 3-byte dash arrives as three cp1252
  characters of mojibake), which invites a pointless "normalize the encoding" edit. Measure with
  `[System.IO.File]::ReadAllText` on the working file and compare code-point groups instead:
  `memory-bank/progress.md` U+2013 x56 / U+2014 x39 / U+2192 x20, and the working tree matches HEAD for those files.
  Source `.cpp` files in this folder stay pure ASCII - `filledshapes.cpp` has **0** non-ASCII bytes after this session's
  comment block.



---

