# narrative: mdi.cpp, session 21

## mdi.cpp - session 21 (1 Nov 2026)

- **Scope:** 4 wrappers, the whole `//Multi Document Interface functions` section (`user32.h:494`, declarations
  `:495-498`), all four `static`, all four defined here in header order (`mdi.cpp:84`, `:91`, `:98`, `:105`) - no U4
  (declared, never defined) and no U13 (defined, never declared static) instance. `tmp/_tmp_sigcmp.ps1 -Tu mdi.cpp
  -First 494 -Last 498` reports **4/4 `decl==def ok`** including parameter names, and post-fix **4/4 `Call ... ok`**;
  pre-fix it flagged exactly one site (see the fix below). This is the cleanest TU of the review so far: 0 published
  signature or parameter-name divergences, 0 binding defects, 0 U17/U11/U13 instances.
- **Bindings (method steps 3 and 3b) - all 4 correct as written, 0 defects:** three SDK A/W families bound
  `qor_pp_useswinapiAW` (`CreateMDIWindow` 6F/70, `DefFrameProc` A1/A2, `DefMDIChildProc` A3/A4 - no bare export for any
  of the three stems) and one genuine single export bound bare (`TranslateMDISysAccel` 3B9, no A/W spelling anywhere in
  the table). A bare macro over the three families **would still resolve today**, only because the SDK macros are live in
  the binding expression - `#define CreateMDIWindow CreateMDIWindowW` (`winuser.h:12230-12234`), `DefFrameProc`
  (`:12147-12151`), `DefMDIChildProc` (`:12179-12183`); that G1 reliance is precisely what D13 forbids, and it is why the
  three are `AW` and not "single-ok". `cl /EP` (`tmp/_tmp_pp_mdi.bat` -> `tmp/_tmp_mdi_pp.txt`) emits four literals at
  `:453322`, `:453329`, `:453336`, `:453343`: `CreateMDIWindowW`, `DefFrameProcW`, `DefMDIChildProcW`,
  `TranslateMDISysAccel`. Live probe `tmp/_tmp_mdi_probe_s21.txt`: those four **RESOLVED**, the three bare stems and the
  three `*T` spellings **MISSING**, the three `*A` halves RESOLVED - and the probe had to `LoadLibraryW` first, because
  `GetModuleHandleW(L"user32")` fails with **gle=126** in a bare console process (G5 measured directly, not assumed).
- **Fixed (1 defect, internal only):** `DefFrameProcT`'s call site (`:95`) read
  `Call< LRESULT, HWND, HWND, INT, WPARAM, LPARAM >` while the member (`:91`) and both SDK prototypes
  (`winuser.h:12132`, `:12141`) call that parameter `UINT`. Now `UINT`. A new instrument measures the scope of this
  shape - `tmp/_tmp_int_sites_s21.ps1`, case-sensitive over every `Call<>`/`voidCall<>` template list in the folder:
  **1 hit of 428 sites, this one; 0 after the fix**. Same width and same value for every real message id, so the emitted
  call is unchanged; the point of naming template arguments (U14) is that they restate the signature, and this one
  contradicted it. Plus a 67-line TU comment block (`mdi.cpp:16-82`); file 44 -> 112 lines, `user32.h` untouched at 695.
- **Checked and deliberately *not* changed:** all three A/W wrappers already carry the `T` suffix, and
  `TranslateMDISysAccel` correctly has none, so **U17 contributes nothing from this TU**; the four parameter types are
  exact, including `CreateMDIWindow`'s `DWORD dwStyle`, which older documentation renders as `LONG`; `LPMSG` in
  `user32.h:498` is the SDK typedef and stays; no return checks anywhere (U1/U2 policy), so a swallowed SEH fault and a
  legitimate 0 from `TranslateMDISysAccel` are indistinguishable (U7).
- **Open items this session feeds:**
  - **U21 third instance** - the SDK's MDI block is the `#ifndef NOMDI` region `winuser.h:12074`-`:12261` and holds
    **seven** entry points. This section owns four; `ArrangeIconicWindows` (`:12199`, ordinal E), `CascadeWindows`
    (`:12249`, 24) and `TileWindows` (`:12240`, 3AE) are declared in the header's `//Window functions` block
    (`:135`, `:139`, `:208`) and defined in `window.cpp:58`, `:86`, `:572`. Auditing MDI from this TU alone under-states
    it by three. Recorded, not moved. `TileWindows`/`CascadeWindows` also carry the only MDI version guard,
    `#if(WINVER >= 0x0400)` (`:12236`-`:12256`) - staged for `window.cpp`.
  - **U16 - 0 of 4 version-guarded**, the first TU of the review with none. All four sit inside `#ifndef NOMDI` and the
    Desktop partition (`:12092`), and `TranslateMDISysAccel` additionally inside `#ifndef NOMSG` (`:12185`-`:12194`).
    `NOMDI`/`NOMSG`/`NOUSER` are defined nowhere in this build, but `user32.h:498` names the SDK `LPMSG`, so a `NOMSG`
    consumer breaks that declaration outright - the `hook.cpp:52-56` opt-out-macro shape, not a version-guard one.
  - **U15 - 4 of 4 unreachable.** `CreateMDIWindowT`, `DefFrameProcT`, `DefMDIChildProcT` have no use site anywhere in
    `src`/`examples`/`test`. `TranslateMDISysAccel` has one compiled layer site -
    `MessageHandler::TranslateMDISysAccel` (`ui/gui/view/handlers/messagehandler.cpp:241`, declared
    `messagehandler.h:93`, built into `winqor_gui` at `ui/gui/CMakeLists.txt:60` via `ui/CMakeLists.txt:5`) - and that
    handler method has no caller of its own. `tmp/_tmp_comment_state.ps1` reports 254 lines and 0 comment-only lines, so
    the site is live code.
  - **U19 - 1 LAYER / 1 INDIRECT / 0 BYPASS / 0 MEMBER** over the 13 suffix variants of the four stems
    (`tmp/_tmp_mdi_bypass_s21.ps1` -> `tmp/_tmp_mdi_bypass_s21.txt`). **The first family of the review with zero SDK
    bypass**, and the reason is structural: there is no MDI host anywhere in the tree, so nobody can be calling
    `::DefFrameProcW` directly.
  - **U26 third mirror-struct instance** - `MessageHandler` hands a `Message*` (`ui/gui/view/handlers/message.h:33-44`)
    to the export as `::LPMSG`. `tmp/_tmp_msg_mirror_s21.cpp` + `.bat` put that mirror and its `Point`
    (`ui/gui/view/drawing/point.h:31-35`) beside the SDK `MSG` (`winuser.h:1877-1887`) and print both layouts **for x64
    and x86**: x64 identical (48 bytes each, `hwnd`/`message`/`wParam`/`lParam`/`time`/`pt` at 0/8/16/24/32/36); x86
    divergent from `lParam` on (`MSG` 28 bytes with `lParam@12 time@16 pt@20`; the mirror 40 bytes with
    `lParam@16 time@24 pt@28`), because it spells `wParam`/`lParam` as portable 64-bit integers where the SDK uses
    4-byte `WPARAM`/`LPARAM` and 8-byte alignment then pushes every later field. Not a one-wrapper problem - the same
    mirror feeds **7 call sites across 6 wrappers** (`messagehandler.cpp:136`, `:146`, `:181`, `:236`, `:241`,
    `windowcontroller.cpp:658`, `acceleratortable.cpp:83`), and `GetMessage`/`PeekMessage` **write** the struct, so on a
    32-bit target the export would fill 28 bytes while the caller reads 40 - wrong `lParam`, `time` and `pt`, no compile
    error. Only x64 is configured (`CMakeSettings.json` has the single `x64-Debug` profile), so the exposure is latent.
    Recorded, not fixed; the repair belongs with `message.cpp`.
- **Folder totals re-measured, unchanged:** 428 bindings, **17 unresolvable** (`tmp/_tmp_p1census_s21.txt`:
  `menu.cpp` 2, `message.cpp` 4, `mouse.cpp` 10, `window.cpp` 1) - this TU contributes 0; U14(b) 424 explicit / 4
  deducing; U14(c) 37 defs `(void)` / 1 `()`; T-suffix census 525 / 108 / 404 / 11 / 2
  (`tmp/_tmp_tsuffix_s21_full.txt`); `_tmp_wrong_stem_scan.ps1` 4 folder-wide mismatch flags, none here; U4 491 parsed /
  426 defined / 65 undefined; `user32.h` 695 lines.
- **Gate:** `mdi.cpp` alone at `/W4` exit 0; folder-wide `tmp/_tmp_cl_all_s21.log` **38/38** TUs compiled, exit 0,
  **0 diagnostics**.
- **Instruments:** new this session - `tmp/_tmp_mdi_prescan_s21.ps1` (export facts per stem + the SDK prototype block +
  the A/W macro spellings), `tmp/_tmp_mdi_bypass_s21.ps1`, `tmp/_tmp_mdi_guards_s21.ps1` (array-argument wrapper),
  `tmp/_tmp_pp_mdi.bat` (own output files, not the shared `_tmp_pp_one.bat`), `tmp/_tmp_mdi_probe_s21.cpp`,
  `tmp/_tmp_int_sites_s21.ps1`, `tmp/_tmp_msg_mirror_s21.cpp`/`.bat`, plus `tmp/_tmp_slog_fix_s21.ps1` and
  `tmp/_tmp_progress_fix_s21.ps1` (byte-safe markdown fixes for rows too long for the line editor) and
  `tmp/_tmp_status_index_gen_s21.ps1` (regenerated locator, 45 entries). Re-run shared censuses:
  `_tmp_kb_p1census_s20.ps1`, `_tmp_sigcmp.ps1`, `_tmp_u14_census.ps1`, `_tmp_tsuffix_census.ps1`,
  `_tmp_wrong_stem_scan.ps1`, `_tmp_comment_state.ps1`, `_tmp_cl_all.bat`.
- **Record gap flagged for session 22:** `archive/session-log.md` is at 45.4 KB, over the ~40 KB ceiling, and `archive/tu/`
  has no narrative for `help`, `hook`, `icon`, `ime` or `keyboard` - five reviewed TUs whose narrative lives only in the
  session-log row and the register slices. Both jobs belong at the start of the next session, not mid-review.
