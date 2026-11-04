# P1-P5 pre-scan of not-yet-reviewed TUs (verbatim)

## Pre-scan of the not-yet-reviewed TUs (folder-wide, ahead of the per-TU sessions)

`tmp/_tmp_aw_checklist.ps1` cross-checks every `qor_pp_useswinapi` / `qor_pp_useswinapiAW` binding in the
folder against `tmp/_tmp_user32_exports.txt`. It is a *hint list* - the per-TU `/EP` check (method step 3)
stays the authority, since the script cannot see which SDK macros are in scope. Results that matter, all in
TUs still to be reviewed:

- **P1 - bindings that cannot resolve at run time (export table + live probe + source line).** Six at the first scan;
  the two in `combobox.cpp` are now fixed, two more were found (`window.cpp:124` in session 6, `mouse.cpp:82` in
  session 8) and the three in `keyboard.cpp` were fixed in session 20, so **eleven bindings in two TUs remain**
  (`window.cpp` x1, `mouse.cpp` x10 - the mouse.cpp ten were found by the session-20 closing scan, see the NEW entry
  below). Fix each in the owning TU's session:
  - ~~`keyboard.cpp:82` `qor_pp_useswinapi(user32, GetKeyboardLayoutNameT)`, `:243` `VkKeyScanT`,
    `:250` `VkKeyScanExT`~~ - **FIXED 1 Nov 2026 (session 20)**: the wrapper's `T` suffix stringizes into the name,
    and `*T` is not an export. Now `qor_pp_useswinapiAW(user32, <stem without the T>)`; `/EP` emits the 7 `*W`
    literals and the live probe resolves all 34 names this TU asks for (`tmp/_tmp_kb_bindings_s20.txt`,
    `tmp/_tmp_kb_probe_s20.txt`). Live, not latent - unlike `dialog.cpp`, these three have callers.
  - ~~`dialog.cpp:89` `qor_pp_useswinapi(user32, GetDlgItemTextT)`~~ - **FIXED 30 Oct 2026 (session 13)**: same class.
    `/EP` pre-fix resolves the literal `"GetDlgItemTextT"` (live probe: MISSING), post-fix `"GetDlgItemTextW"` (RESOLVED).
    Latent rather than live - that wrapper has no call site in built code (see the U15 `dialog.cpp` entry).
  - **found in session 6:** `window.cpp:124` `qor_pp_useswinapiAW(user32, CreateWindow)` (inside
    `User32::CreateWindowT`, `window.cpp:121`) asks for `CreateWindowW`, which does not exist. `CreateWindow` is the
    SDK's macro for `CreateWindowEx`; the table has `CreateWindowExA/W` (`:1626,1627`) and no plain `CreateWindow*`
    entry, and the live probe says `CreateWindowW` MISSING / `CreateWindowExW` RESOLVED. Before "fixing" the binding,
    decide whether `CreateWindowT` should exist at all: `CreateWindowExT` (`window.cpp:129`) already wraps the same
    export, so the honest fix is either bind `CreateWindowEx` with a `0` extended-style argument or drop the member.
  - ~~`combobox.cpp:20` `qor_pp_useswinapi(User32, DlgDirListComboBox)`, `:27` `DlgDirSelectComboBoxEx`~~ - **FIXED
    29 Oct 2026 (session 6)**: both stems are `#undef`'d at `user32.h:15,16`, so the macro cannot rescue them (G2); the
    literals are not exported while the `W` entries are (live probe). Now `qor_pp_useswinapiAW(user32, ...)`. The G4
    `User32` module token went in the same pass: 416 of the folder's 428 bindings now spell `user32`, the remaining 12
    are `scrollbar.cpp`'s, for its own session.
  - **found in session 8 (by the new P5 scan, not by the export-table script):** `mouse.cpp:82`
    `qor_pp_useswinapiAW(user32, LPTRACKMOUSEEVENT)` - the parameter **type** name went into the binding slot, so the AW
    macro asks `GetProcAddress` for `LPTRACKMOUSEEVENTW`. Live probe: that name MISSING, and so is `TrackMouseEventW`,
    while bare `TrackMouseEvent` is RESOLVED (`winuser.h:2773`, inside `_WIN32_WINNT >= 0x0400` - a U16 instance for
    that session too). So `mouse.cpp:79-84` needs *both* halves fixed: the stem and the AW-ness (it is a single export,
    so bare `qor_pp_useswinapi(user32, TrackMouseEvent)` is the D13 spelling). Its `Call< BOOL >` is already listed in
    U14(b). Owned by `mouse.cpp`'s session.
  - **NEW (session 20 closing scan, 1 Nov 2026) - the mirror-image class, and it is all of `mouse.cpp`:** the other 9 of
    its 10 wrappers use `qor_pp_useswinapiAW` on stems that have **no** A/W pair, so the AW macro pastes a `W` suffix
    that no export carries. Checked against the export table: `DragDetect`, `GetCapture`, `GetDoubleClickTime`,
    `GetMouseMovePointsEx`, `mouse_event`, `ReleaseCapture`, `SetCapture`, `SetDoubleClickTime`, `SwapMouseButton` and
    `TrackMouseEvent` each appear **only bare** (no `<stem>A`, no `<stem>W`), so `mouse.cpp:19,26,33,40,47,54,61,68,75,82`
    ask for `DragDetectW`, `GetCaptureW`, ... `LPTRACKMOUSEEVENTW` and cache a NULL `pFunc` - **10 of 10 wrappers in
    that TU throw on first call**. `mouse.cpp`'s session is therefore a whole-TU binding fix, not a one-line one; the
    `AW`-on-a-single-export shape is invisible to P5 (stem == member) and was only caught by re-running the folder-wide
    AW checklist after the keyboard fix. `tmp/_tmp_kb_awpair_s20.ps1` is the per-stem bare/A/W check. Re-checked by hand
    at the session-20 close: all ten stems are present in the table **bare only**, so the whole-TU fix is dropping `AW`
    for plain `qor_pp_useswinapi` on all ten, plus the stem half of `:82` (`LPTRACKMOUSEEVENT` -> `TrackMouseEvent`).
    **Decision 1 Nov 2026 (user, at the session-20 close): no out-of-turn hotfix** - these ten stay recorded and are
    fixed in `mouse.cpp`'s own session (25th alphabetically), so `mouse.cpp` stays broken for the sessions in between.
    What makes that tolerable is measured, not assumed (`tmp/_tmp_mouse_bypass_s20.ps1`): all ten **do** have
    through-layer call sites - 9 LAYER (`ui/gui/controllers/i_m/mouse.cpp:39,44,49,54,59,64,69,74` plus
    `windowcontroller.cpp:673`) and 7 INDIRECT, 0 BYPASS - so they are compiled into `winqor_gui`; but the enclosing
    `qor::platform::win::Mouse` controller (`i_m/mouse.h:34`) has **no** call site in src/examples/test - 0 member-call
    sites repo-wide, and its header has exactly one includer, its own `.cpp` - so no built code reaches them at run
    time. `windowcontroller.cpp:673` is `WindowController::DragDetect`, which is itself U15-unreachable.
- **P2 - bindings that work today only because the SDK macro rewrites them; convert to explicit AW
  under D13.** **P2 is CLOSED - 0 remain (2 Nov 2026, session 23).** ~~`message.cpp` `RegisterWindowMessage`,
  `SendMessage`, `SendMessageTimeout`, `SendNotifyMessage`~~ - **all four converted in session 23** at
  `message.cpp:214`, `:235`, `:242`, `:249`; same **no behaviour change** proof - pre-fix `/EP` already emitted the four
  `"*W"` literals and post-fix `/EP` emits the same four by `##W` pasting
  (`tmp/_tmp_msg_class_{pre,post}_s23.txt`), and the folder census drops **15 -> 11** unresolvable with no `message.cpp`
  line left in it (`tmp/_tmp_p1census_post_s23.txt`). ~~`menu.cpp` `GetMenuString`, `InsertMenuItem`~~ - **both converted 2 Nov 2026 (session 22)** at
  `menu.cpp:225` and `:260`; same **no behaviour change** finding as below - pre-fix `/EP` already emitted
  `"GetMenuStringW"`/`"InsertMenuItemW"`, post-fix emits the same two literals by `##W` pasting
  (`tmp/_tmp_menu_class_post_s22.txt`), and the folder census drops 17 -> 15 unresolvable
  (`tmp/_tmp_p1census_post_s22.txt`). ~~`dialog.cpp` `MessageBox`~~ - **converted 30 Oct 2026
  (session 13)** at `:124`; **no behaviour change** - the pre-fix `/EP` literal was already `"MessageBoxW"` because the
  SDK's `UNICODE` mapping rewrites the stem before stringizing (G1); the AW form reaches the same literal by `##W`
  pasting (`kernel32.h:52`), which is why the conversion is behaviour-neutral.
- **P3 - naming flags from the new T-suffix census (`tmp/_tmp_tsuffix_census.ps1`, session 6).** Two members carry the
  folder's `T` suffix where no A/W export pair exists: `user32.h:144 CreateWindowT` (see the P1 entry) and
  `user32.h:383 CopyCursorT`, defined `cursor.cpp:23`, whose binding is `qor_pp_useswinapi(user32, CopyIcon)`
  (`cursor.cpp:26`) - a correct binding of a genuinely single export, but a wrapper name that hides which export it is
  (the SDK's `CopyCursor` is a `CopyIcon` cast macro, not an entry point). Both are decisions for the `window.cpp` and
  `cursor.cpp` sessions, together with the U17 list.
- **P4 - four exports in the table are forwarders, which the binding question cannot see (found in session 7).** While
  checking whether `ConsoleControl` is one, note that `dumpbin /exports` marks exactly four of the 1018 names as
  `(forwarded to NTDLL.xxx)` - `DefDlgProcA/W` (ordinals 1668/1669 -> `NTDLL.NtdllDialogWndProc_A`/`_W`) and
  `DefWindowProcA/W` (1675/1676 -> `NTDLL.NtdllDefWindowProc_A`/`_W`). They still print a real RVA, and the marker text
  is "forwarded to", **not** the `->` form an earlier grep assumed - that false negative is why the forwarder question
  was re-asked with a fresh `dumpbin`. Forwarding is transparent to `GetProcAddress`, so no binding breaks, but the code
  that finally runs lives in `ntdll.dll`: take it into account in `windowprocedure.cpp`'s session, where U6's
  default-`__cdecl` `fPtr` reinterprets an NTDLL `__stdcall` entry point rather than a `user32.dll` one.
- **P5 - the "wrong stem bound" defect class, and the instrument that finds it (`tmp/_tmp_wrong_stem_scan.ps1`, session
  8, 30 Oct 2026).** `coordtransform.cpp` bound `GetDCEx` in all three of its wrappers. Nothing in the pre-scan or in the
  per-TU method could see it: `GetDCEx` is a real export (ordinal 144), the module token was right, `cl /EP` produced a
  valid string literal, the TU compiles, and the call site type-checks because `Library::Call` never sees a prototype. The
  only check that catches it is comparing the stem inside `qor_pp_useswinapi*` against the member that encloses it. New
  script, folder-wide and cheap: 428 bindings over 38 TUs, **7 flags at the first run, 4 after the session-8 fix**, and of
  those 4 only one is a live defect (`mouse.cpp:82`, added to P1); `cursor.cpp:26` is the P3 correct-binding/odd-name
  case and the two `resource.cpp` flags are the script failing to see an enclosing member because both `Resource`
  definitions are unqualified free functions - i.e. the script also reproduces U11 mechanically. Re-run it in every session
  and treat a new flag as a P1-class finding until disproved.
- **P5** re-measured 3 Nov 2026 (session 29, `resource.cpp`): 428 bindings over 38 TUs, **3 flags -> 1**. The two
  `resource.cpp` flags the paragraph above describes disappeared with the `User32::` qualifier (U11), which confirms the
  script's mechanical reading of U11: qualifying the definitions made the enclosing member visible to the scan, and the
  two stems then matched their own members. The single surviving flag is `cursor.cpp:64` `CopyCursorT` binding `CopyIcon`
  - the P3 correct-binding/odd-name case (the row has moved from the `:26` cited above as `cursor.cpp` grew). `window.cpp`
  is the largest surface (85 bindings, 75 bare) and will need the most care.
- Everything else in the folder already either uses `qor_pp_useswinapiAW` (e.g. `ime.cpp`, `mouse.cpp`,
  `text.cpp`, `windowprocedure.cpp`, `windowproperties.cpp`, `windowclass.cpp`) or binds a genuine single
  export. `window.cpp` is the largest surface (85 bindings, 75 bare) and will need the most care.
  **Corrected by session 20:** listing `mouse.cpp` here as an AW exemplar was wrong in the harmful direction - all 10 of
  its AW bindings are over stems with no A/W pair, so the AW spelling is what breaks them (see the NEW session-20 P1
  entry). This pre-scan could not see it because it only checked "is the AW spelling ever needed", never "does the pasted
  `W` name exist".


- **P1 re-measured after session 25 (`mouse.cpp`, 2 Nov 2026): 1 unresolvable binding left, and it is not in a reviewed
  TU.** `mouse.cpp`'s ten `qor_pp_useswinapiAW` bindings (`:19`,`:26`,`:33`,`:40`,`:47`,`:54`,`:61`,`:68`,`:75`,`:82` - the
  whole TU) are now bare `qor_pp_useswinapi` at the fixed file's `:100`,`:107`,`:114`,`:121`,`:128`,`:135`,`:142`,`:149`,
  `:156`,`:163`, which takes the folder census from **11 unresolvable to 1** (`tmp/_tmp_p1census_pre_s25.txt` ->
  `tmp/_tmp_p1census_post_s25b.txt`, both 428 bindings over 38 TUs). The sole survivor is `window.cpp:124 CreateWindow`,
  queue item 34's, and it is P3-shaped (`CreateWindowT` over the `CreateWindowEx` family) rather than mouse-shaped.
  Verified live, not by table: `tmp/_tmp_mouse_probe_s25.cpp` -> `tmp/_tmp_mouse_probe_s25.txt` reports **MISSING for all
  ten old `*W` names** and **RESOLVED for all ten bare stems** at the export-table RVAs, plus MISSING for
  `TrackMouseEventW`/`A`/`T` and `DragDetectA`, which disproves every alternative spelling. The probe needed
  `LoadLibraryW(L"user32")` - `GetModuleHandleW` fails with `gle=126` in a bare console process (G5).
- **P5 now has 0 live instances.** The scan's `mouse.cpp:82` row (`TrackMouseEvent` bound the parameter **type**
  `LPTRACKMOUSEEVENT`) went in the same edit as the ten bindings, so the folder scan (`tmp/_tmp_wrong_stem_scan.ps1`)
  drops **4 -> 3 rows** and all three survivors are known-benign: `cursor.cpp:64` is P3's correct-binding/odd-name case
  (`CopyCursorT` -> `CopyIcon`) and `resource.cpp:19`/`:26` are the script not seeing an enclosing member because both
  `Resource` definitions are unqualified free functions (U11). Re-run in every session; a new row is still a
  P1-class finding until disproved.


- **P1 CLOSED 4 Nov 2026 (session 33, `window.cpp`) - 0 unresolvable bindings left in the folder.** The sole survivor the
  entry above names (`window.cpp:124 CreateWindow`) was fixed in that TU's own session. Two independent pre-fix
  measurements agreed: `/EP` emitted 85 binding literals of which **1** is not in the export table
  (`MISSING CreateWindowW`, `tmp/_tmp_win_ppclass_s33_pre.txt`), and the folder census printed
  `FAIL window.cpp:124 AW(user32, CreateWindow) CreateWindowA=False CreateWindowW=False`
  (`tmp/_tmp_p1census_pre_s33.txt` - 428 bindings, **1** unresolvable). Post-fix: 85 binding lines, **84 distinct names,
  0 not in the export table**, 0 `…WW`, 0 `…T`, module literal `user32` 85/85 (`tmp/_tmp_win_pp_s33.bat` ->
  `tmp/_tmp_win_pp_s33.txt`, classified by `tmp/_tmp_win_ppclass_s33.ps1` -> `tmp/_tmp_win_ppclass_s33_final.txt` and
  `_post.txt`), and the folder census is **428 bindings / 0 unresolvable** (`tmp/_tmp_p1census_final2_s33.txt`).
  The class is now closed with a fact worth keeping: **`CreateWindow` and `CreateWindowW` are not exports and never can
  be** - `winuser.h:4464`/`:4468` define them as *function-like macros* forwarding to `CreateWindowExA/W` with
  `dwExStyle = 0`, and the binding macro stringizes the pasted name before any macro can expand (G1), so every spelling
  of that stem is a P1. `CreateWindowT` now binds `qor_pp_useswinapiAW(user32, CreateWindowEx)` and passes the literal
  `0` itself (`window.cpp:189-200`). With **P2** closed in session 23 and **P5** at 0 live instances, **no binding defect
  class remains open anywhere in this library**.
- **P5 re-measured 4 Nov 2026 (session 33): 1 -> 2 rows, and the new row is this session's own deliberate alias.**
  `tmp/_tmp_wrong_stem_scan.ps1` -> `tmp/_tmp_wrongstem_final_s33.txt`: 38 files, 428 `User32::` definitions, 428 bindings
  scanned, **mismatched 2** - `cursor.cpp:64 CopyCursorT -> CopyIcon` (P3's correct-binding/odd-name case) and
  `window.cpp:197 CreateWindowT -> winapiAW(user32, CreateWindowEx)` (the alias created by the P1 fix above). Both are
  known-benign, so **P5 keeps 0 live instances**; a later session that sees exactly these two rows reads them as clean,
  not as a finding.

