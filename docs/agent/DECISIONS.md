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
  - D27: T9 step 9a - wrap the 34 //String functions members in a new strings.cpp TU and make all 34 static - ACTION (9 Nov 2026)
    File: ./decisions/D27.md
  - D28: T9 step 9b - wrap the 7 //Raw Input functions members in a new rawinput.cpp TU and make all 7 static - ACTION (9 Nov 2026)
    File: ./decisions/D28.md
  - D29: T9 step 9c - wrap the 4 //System Information functions members in a new systeminfo.cpp TU and put static on all 4 declarations - ACTION (9 Nov 2026)
    File: ./decisions/D29.md
  - D30: T9 step 9d - wrap the 3 //Button Control functions members in a new buttoncontrol.cpp TU and put static on all 3 declarations - ACTION (9 Nov 2026)
    File: ./decisions/D30.md
  - D31: T9 step 9e - wrap the 2 //Synchronization functions members in a new synchronization.cpp TU, put static on both declarations and move bWaitAll to the SDK's fWaitAll - ACTION (9 Nov 2026)
    File: ./decisions/D31.md
  - D32: T9 step 9f - wrap the 1 //Bitmap functions member in a new bitmap.cpp TU, put static on it and bind qor_pp_useswinapiAW (the bare stem is not exported and the SDK puts a stem switch on it) - ACTION (9 Nov 2026)
    File: ./decisions/D32.md
  - D33: T9 step 9g - wrap the 1 //Printing functions member in a new printing.cpp TU, put static on it and bind qor_pp_useswinapi bare (the bare stem IS the export and the SDK puts no switch on it) - ACTION (9 Nov 2026)
    File: ./decisions/D33.md
  - D34: T9 step 9h - delete the 34-member //Dynamic Data Exchange functions block from user32.h (the family is outside D18's documented set - 0 of the 34 in um\WinUser.h, 27 in um\ddeml.h, 7 in neither, and the 34 `) const;` spellings copy no SDK prototype) - ACTION (9 Nov 2026)
    File: ./decisions/D34.md
  - D35: U7 - swallowing a caught SEH fault to a default return is accepted as-is (serious(...) already resolves it through the caller's handler or terminates before the default value is observed) - POLICY (10 Nov 2026)
    File: ./decisions/D35.md
  - D36: U5 - the run-time binding cached in a function-local `static const` (a failed resolution included) is accepted as by design (the cache is the design; a null binding re-throws on every later call, so the verdict is deterministic and observable) - POLICY (10 Nov 2026)
    File: ./decisions/D36.md
  - D37: U8 - report a null binding through the QOR issue facility instead of `throw(const char*)` - ACTION (10 Nov 2026) - MEASURED: C2712, the tree does not build (62 in the 54-source folder gate, 126 in the 122-source all-libraries gate, 2 new C1003 fatals); U8 NOT closed, the constraint is U29
    File: ./decisions/D37.md
  - D38: U8 - move the null-binding report out of line (`ReportMissingLibraryFunction`) and return the default explicitly: C2712 gone, gates at zero diff, a null pointer never called through - ACTION (10 Nov 2026), CLOSES U8 AND U29
    File: ./decisions/D38.md
  - D39: U10 - the console-internal types are Microsoft-documented (learn.microsoft.com/console consolecontrol; verbatim-identical block, no-import-library note authorises the GetProcAddress binding) - POLICY (10 Nov 2026), CLOSES U10, NO CODE CHANGE
    File: ./decisions/D39.md
  - D40: U17 - rename the 9 A/W-family members declared without the folder's T suffix to SDK stem + T (Option A) - 21 sites in 11 files, binding literals and the #undef block untouched, gates at zero diff, census 600 / 119 / 479 / 0 / 2 - ACTION (10 Nov 2026), CLOSES U17
    File: ./decisions/D40.md
  - D41: U19 - the bypass call-site census lands as doc/winqor_api_user_bypasses.txt (114 sites in 13 files, from a folder-wide census over 3,230 files reading LAYER 466 / INDIRECT 165 / MEMBER 207 / BYPASS 124 raw with 10 shadowed by same-named member declarations) and U19 is closed as cannot be fixed within this library (a bypass call site is another library's source to change; 112 of the 114 rows sit in trees that are not in the build at all) - POLICY (11 Nov 2026), CLOSES U19, NO CODE CHANGE
    File: ./decisions/D41.md
  - D42: U21 - the folder's sections and TUs group members by consumer subsystem, not by the SDK's feature-macro family regions: TU's do not match SDK families exactly by design (a section that over-promises its label, a family split across sections/TUs and an audit-from-one-TU under-count are expected shape; 0 correctness defects; the "not even declared" collateral is discharged by T8 6f/6g and ScrollChildren is prototype-less under D18's scope line) - POLICY (11 Nov 2026), CLOSES U21, NO CODE CHANGE
    File: ./decisions/D42.md
  - D43: U22 - the DeviceContext release route feeds ReleaseDC a NULL HWND (m_hWnd is set only by the two FromWindow factories, so a context built by PerThread::DeviceContextFromHandle releases as ReleaseDC(NULL, hdc) and the DC leaks): U22's deliverable is a note at the Release() member of ui/gui/gdiobjects/devicecontext.cpp, the source where the error actually is, and U22 closes as cannot be fixed within winqor_api_user (the wrapper is a faithful ReleaseDC pass-through; the HWND/DC pairing is wrong at the winqor_gui callers, another library's behaviour change - the U20 rule) - POLICY (11 Nov 2026), CLOSES U22, NO CODE CHANGE (comment-only note, no fact re-verified, no gate run)
    File: ./decisions/D43.md
  - D44: U24 - the one in-tree FlashWindow caller reads its BOOL as a success flag (FlashWindow returns the window's prior active state, not a success flag): fixed by hand in the winqor_gui layer, WindowController::Flash switched to User32::FlashWindowEx with FLASHW_ALL | FLASHW_TIMERNOFG (FLASHW_STOP when invert is false, way-out #3, returns a genuine success BOOL) - a single-point exception to the U20 rule (the rule stays in force elsewhere; U24 is the one gui-side defect the user fixed directly) - POLICY (11 Nov 2026), CLOSES U24 (gui-layer manual fix, no api_user code change, no gate run)
    File: ./decisions/D44.md

---