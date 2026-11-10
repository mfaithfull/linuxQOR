# registers/ - index

**The record protocol lives in `../status.md` and is not restated here.** Under protocol v3 nothing is appended to a
register: a register holds one item's rule, its decision and its measured detail, while per-TU instances live in the
review banner at the top of the `.cpp` that found them, so an instance is read from the source instead of tallied here.
Open a register only for an id you are acting on, and read its entry head. A **new** open item gets its own `u<n>.md` -
the shape is `u28.md`.

The `Lines` and `Was registers.md` columns below are historical, **not maintained**, and not to be trusted - the
generators that filled them are retired. Use the `Register ids` column only; the id-to-file map is derivable from the id.

Closed / frozen - append nothing: `u4` (D26->D34), `u5` (D36), `u7` (D35), `u8` (D38), `u9` (10 Nov 2026: `purpose.md`), `u10` (D39: Microsoft-documented, verbatim block), `u17` (D40: Option A rename, 21 sites in 11 files), `u21` (D42: subsystem grouping by design, no code change), `u12` (session 2), `u13` (session 7), `u14` (D15), `u15` (D14), `u25` (7 Nov 2026 `CONST`->`const` pass), `u29` (D38), `u6`, `u3`, and **P1, P2, P4, P5** in `prescan.md` (P4 CLOSED 11 Nov 2026 (D49); P1, P2, P5 at 4 Nov 2026:
428 bindings, 0 unresolvable). Frozen as provenance, never read at session start: `census.md`, `u16.md`, `u19.md`,
`u26.md` - U26 is now **CLOSED 11 Nov 2026 (D45)**: the register file stays frozen provenance and gained only a head
closure line, because the close's deliverable is six comment blocks in five published `winqor_gui` headers
(`icon.h` `IconInfoEx`, `scrollbarcontroller.h` `ScrollInfo`, `windowcontroller.h` `WindowPlacement`, `message.h` `Message`,
`monitor.h` `MonitorEnumProc` + `MonitorInfoEx`), not a new register - U26 closes as **cannot be fixed within this library**
(the wrappers are faithful to the exports they bind per D18, the divergence is another library's published mirror - the
U20 rule; no project-wide `/DUNICODE` was added and the `*T` published spelling stays the SDK's, so the narrow-spelling
residual for a consumer that sees `<windows.h>` first stays recorded as D21's residual): 0 sources touched, no gate run. **U8 and U29 CLOSED 10 Nov 2026 (D38)** - the out-of-line null-binding report, gates at zero diff. **U10 CLOSED 10 Nov 2026 (D39)** - the console-internal types are Microsoft-documented and the block is verbatim, no code change. **U17 CLOSED 10 Nov 2026 (D40)** - Option A: the 9 A/W members declared without the folder's `T` renamed to SDK stem + `T`, 21 sites in 11 files, gates at zero diff, census 600 / 119 / 479 / 0 / 2. **U19 CLOSED 11 Nov 2026 (D41)** - the bypass census lands in `doc/winqor_api_user_bypasses.txt` (114 sites in 13 files: 112 rows in trees that are not in the build, 2 rows in built `winqor_gui` at `paintsession.cpp:41`/`:47`), closed as **cannot be fixed within this library**, 0 sources touched and no gate run. **U21 CLOSED 11 Nov 2026 (D42)** - TU's do not match SDK families exactly **by design**: sections and TUs group members by consumer subsystem, not by the SDK's feature-macro family regions, so a family split across sections/TUs and an audit-from-one-TU under-count are expected shape; 0 correctness defects, the "not even declared" collateral is discharged by T8 6f/6g and `ScrollChildren` is prototype-less - accepted as-is, 0 sources touched and no gate run. **U22 CLOSED 11 Nov 2026 (D43)** - the note lands where the error actually is (a comment block at the `Release()` member of `src/platform/os/windows/ui/gui/gdiobjects/devicecontext.cpp`), and U22 closes as **cannot be fixed within this library**: `User32::ReleaseDC` is a faithful pass-through and the HWND/DC pairing is wrong at the `winqor_gui` callers (the U20 rule); comment-only note, no fact in `u22.md` re-verified, 0 sources touched and no gate run. **U24 CLOSED 11 Nov 2026 (D44)** - fixed by hand in `winqor_gui` (way-out #3): `WindowController::Flash` switched to `User32::FlashWindowEx` (`FLASHW_ALL | FLASHW_TIMERNOFG`), returning a genuine success flag; a **single-point exception to the U20 rule** (the rule stays in force elsewhere), 0 `winqor_api_user` sources touched, no gate run. **U20 CLOSED 11 Nov 2026 (D47)** - the notes move to the calling layers and U20 closes as **cannot be fixed within winqor_api_user**: nine comment blocks over eight files (`ui/gui/gdiobjects/cursor.h`, `icon.h` above the `own = true` ctor, `cursor.cpp` above `~Cursor()` and above `Show()`, `icon.cpp` above `~Icon()`, `ui/gui/controllers/a_h/buttoncontroller.cpp`, `ui/gui/factories/windowfactory.cpp`, `ui/gui/factories/windowclass.cpp`, `src/platform/os/windows/common/handles/handle.cpp` above `~PrimitiveHandle()`), the entry's cites carried as written, and the ownership taxonomy stated there (a handle is a value not an owner; must release / must never release / ours-but-still-live; the release verb is per family, not `Close`); 0 `winqor_api_user` sources touched, no gate run, no fact in `u20.md` re-measured. **U20 CLOSED 11 Nov 2026 (D47)** - the notes move to the calling layers and U20 closes as **cannot be fixed within winqor_api_user**: nine comment blocks over eight files (`ui/gui/gdiobjects/cursor.h`, `icon.h` above the `own = true` ctor, `cursor.cpp` above `~Cursor()` and above `Show()`, `icon.cpp` above `~Icon()`, `ui/gui/controllers/a_h/buttoncontroller.cpp`, `ui/gui/factories/windowfactory.cpp`, `ui/gui/factories/windowclass.cpp`, `src/platform/os/windows/common/handles/handle.cpp` above `~PrimitiveHandle()`), the entry's cites carried as written, and the ownership taxonomy stated there (a handle is a value not an owner; must release / must never release / ours-but-still-live; the release verb is per family, not `Close`); 0 `winqor_api_user` sources touched, no gate run, no fact in `u20.md` re-measured.
**U27 CLOSED 11 Nov 2026 (D46)** - policy decision, no code change: the `STRICT`/`NO_STRICT` arm decides the published (mangled) name of every member whose signature names a STRICT-sensitive SDK type (`HOOKPROC` and every `DECLARE_HANDLE` type - 382 of `user32.h`'s 600 member-declaration lines name a handle type other than bare `HANDLE`, 3 name a STRICT-sensitive callback typedef), and the invariant now in force is that **the layer publishes the STRICT arm and does not override a consumer's `NO_STRICT`**; U27 closes as a **published-signature property, not a defect in this library**. The proposed `#undef NO_STRICT` at `kernel32.h:36` was measured and rejected: **inert** here (`NO_STRICT` defined nowhere in the repo, `cmake/` carries no `/D` or `/U`, so both gates come out byte-identical - no diff, so nothing to show), **order-dependent** (a TU that reads `<windows.h>` before any QOR header keeps `HHOOK` as `void *` - the same residual D21 left unfixed), and **harmful** (it swaps U27's unresolved external for `C2440` compile errors in a `NO_STRICT` consumer's own code). `kernel32.h` unchanged, 0 sources touched, no gate run, `registers/p6-u27.md` gained only a head closure line; **P6 stays open**. **P6 CLOSED 11 Nov 2026 (D48)** - measured 0-instance close, no code change: the P6 trap class has a **7-stem population** in a fresh `user32.dll` export table (`CallMsgFilter`, `BroadcastSystemMessage`, `IsDialogMessage`, `GetAltTabInfo`, `GetWindowModuleFileName`, `RealGetWindowClass`, `TranslateAccelerator`) - not the 2 the register knew - all 7 bound `qor_pp_useswinapiAW` (`accelerator.cpp:55`, `dialog.cpp:130`, `hook.cpp:82`, `message.cpp:108`, `window.cpp:283`/`:388`/`:560`) and **0 bare**, SDK `stem -> stemW` macros on all 7 with 0 `#undef` overlap - **0 live instances**, `registers/p6-u27.md` gained only a head closure line. **P4 CLOSED 11 Nov 2026 (D49)** - a property of the export table, not a defect: the four NTDLL-forwarded rows (`DefDlgProcA/W`, `DefWindowProcA/W`) print no RVA but are named exports and resolve transparently through `GetProcAddress`; the only 2 bindings touching them (`dialog.cpp:53` `DefDlgProcW`, `windowprocedure.cpp:85` `DefWindowProcW`) resolve, 0 touch the A spellings, and the U6 convention hazard the entry pointed at is discharged by U6 and mooted by D17; no code change, 0 sources touched, no gate run, `registers/prescan.md` gained only a head closure line.

| File | Lines | Register ids | Was registers.md |
|---|--:|---|---|
| `census.md` | 337 | - | 1032-1063 |
| `ground-rules.md` | 54 | G1, G2, G3, G4, G5, G6, G4 | 86-126 |
| `open-items.md` | 4 | - | 127-128 |
| `p6-u27.md` | 19 | P6, U27 | 77-85 |
| `preamble.md` | 9 | - | 1-7 |
| `prescan.md` | 152 | P1, P2, P3, P4, P5, P5, P1, P5, P1, P5 | 8-76 |
| `README.md` | 45 | - | - |
| `tasks.md` | 161 | T1, T2, T3, T4, T5, T6, T7, T8 | 958-1031 |
| `u1.md` | 10 | U1 | 129-136 |
| `u10.md` | 20 | U10 | 209-226 |
| `u11.md` | 42 | U11, U11 | 227-236 |
| `u12.md` | 26 | U12 | 237-249 |
| `u13.md` | 17 | U13 | 250-264 |
| `u14.md` | 111 | U14, U14, U14, U14, U14, U14, U14 | 265-291 |
| `u15.md` | 222 | U15 | 292-447 |
| `u16.md` | 302 | U16, U16, U16, U16, U16, U16, U16, U16 | 448-574 |
| `u17.md` | 235 | U17 | 575-616 |
| `u18.md` | 105 | U18, U18, U18 | 617-630 |
| `u18-coverage.md` | 49 | (U18 backlog disposition list, not a register id) | - |
| `u19.md` | 284 | U19, U19, U19, U19, U19, U19, U19 | 631-741 |
| `u2.md` | 68 | U2, U2, U2, U2, U2 | 137-142 |
| `u20.md` | 79 | U20 | 742-818 |
| `u21.md` | 136 | U21, U21 | 819-834 |
| `u22.md` | 20 | U22 | 835-852 |
| `u23.md` | 36 | U23 | 853-867 |
| `u24.md` | 23 | U24 | 868-888 |
| `u25.md` | 98 | U25, U25 | 889-916 |
| `u26.md` | 175 | U26, U26 | 917-957 |
| `u29.md` | 14 | U29 | - (new item, 10 Nov 2026; CLOSED D38) |
| `u3.md` | 9 | U3 | 143-149 |
| `u4.md` | 78 | U4 | 150-186 |
| `u5.md` | 5 | U5 | 187-189 |
| `u6.md` | 64 | U6, U6 | 190-199 |
| `u7.md` | 7 | U7 | 200-202 |
| `u8.md` | 7 | U8 | 203-205 |
| `u9.md` | 5 | U9 | 206-208 |
