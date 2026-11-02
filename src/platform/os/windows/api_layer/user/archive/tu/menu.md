# narrative: menu.cpp, session 22

## menu.cpp - session 22 (2 Nov 2026)

- **Scope:** 39 wrappers, the whole `//Menu functions` section (`user32.h:214`, declarations `:215-253`), all 39
  `static`, all 39 defined here in header order (`menu.cpp:82` `AppendMenuT` .. `:348` `TrackPopupMenuEx` post-fix) - so
  **0 U4** and **0 U13** instances. `tmp/_tmp_sigcmp.ps1 -Tu menu.cpp -First 214 -Last 254` parses all 39 decl/def pairs
  and reports **0 signature flags** (`tmp/_tmp_menu_pre_s22.txt`): the `mdi.cpp` uppercase-`INT` class and the
  `keyboard.cpp` decl/def-parameter class are both absent here. `user32.h` untouched at 695 lines.
- **Bindings (method steps 3 + 3b) - the folder's largest bare block, and it is correct:** 9 stems are SDK A/W families
  (`AppendMenuW` C, `GetMenuItemInfoW` 182, `GetMenuStringW` 186, `InsertMenuW` 220, `InsertMenuItemW` 21F, `LoadMenuW`
  265, `LoadMenuIndirectW` 264, `ModifyMenuW` 28D, `SetMenuItemInfoW` 34F - the `A` halves exist too) and bind through
  `qor_pp_useswinapiAW`; the other 30 bind bare. The 30 were not assumed - `tmp/_tmp_menu_class_s22.ps1` checks
  every stem four ways (the literal `cl /EP` actually emits, the export table, the SDK declaration count, and whether
  `winuser.h` defines a `<stem> -> <stem>W` macro at all) and for all 30 the result is **bare exported, no A/W spelling,
  no macro**, i.e. bare is the only name that could ever resolve (`TrackPopupMenu` 3B4, `EnableMenuItem` E9,
  `CheckMenuItem` 43, `GetSubMenu` 1C4, `SetMenu` 349, `IsMenu` 23A, `GetMenuCheckMarkDimensions` 17B - the deprecated
  one, still a real export, etc). The 9 macro-bearing stems are exactly the 9 that need `AW`, and no bare spelling of
  any of them is an export, so bare there would be a run-time throw (G1/G2).
- **Fixed (2 defects, both the pre-scan P2 class):** `GetMenuString` (`:225`) and `InsertMenuItem` (`:260`) bound the
  bare stem over an A/W family. Both are **behaviour-neutral**: `cl /EP` before the fix already emitted
  `"GetMenuStringW"` and `"InsertMenuItemW"` because the SDK macro rewrote the stem before it was stringized, and after
  the fix the same two literals appear, now produced by `##W` pasting (`kernel32.h:52`). Measured pre- and post- with
  `tmp/_tmp_menu_pp_s22.bat` -> `tmp/_tmp_menu_pp_s22.txt` and `tmp/_tmp_menu_class_post_s22.txt` - **39 literals, all
  39 in the export table, 0 unresolvable in this TU**. Folder-wide the P1 census goes **17 -> 15**
  (`tmp/_tmp_p1census_post_s22.txt`) and no `menu.cpp` line remains in it.
- **New measurement - return types had never been compared per-TU.** In `winuser.h` the return type sits on its **own**
  line above the stem, so the declaration-line instrument `_tmp_menu_proto_s22.ps1` could not see it; `tmp/_tmp_menu_ret_s22.ps1`
  reads it from there and reports **0 return-type divergences of 39** - 24 `BOOL`, 7 `HMENU`, 3 `UINT`, 3 `int`, 1 `DWORD`
  (`CheckMenuItem`), 1 `LONG` (`GetMenuCheckMarkDimensions`).
- **Checked and deliberately *not* changed (published header surface):**
  - `TrackPopupMenu`'s last parameter is `HWND prcRect` (`user32.h:252`, def `:341`) where the SDK has
    `_Reserved_ CONST RECT *prcRect` (`winuser.h:7759-7766`). `STRICT` is on in this build, so `HWND` is
    `struct HWND__ *` and a `RECT *` will not convert implicitly - the slot can carry `NULL` or a cast. Pointer-sized
    both architectures, no ABI effect, no UB, and the wrapper has no caller at all (U15), so it is latent. -> **U18**.
  - `InsertMenuT` (`:250`) and `ModifyMenuT` (`:292`) take `void* uIDNewItem` where the SDK takes `UINT_PTR`
    (`winuser.h:7658`, `:7667`, `:7704`, `:7713`), while `AppendMenuT` (`:82`) in the same section spells it correctly.
    The SDK's `UINT_PTR` exists so one slot carries either an item id or a string pointer; `void*` forces a cast on the
    id case, and both wrappers **are** live (`ui/gui/controllers/i_m/menu.cpp:173`, `:183`). -> **U18**.
  - The `Call<>` lists at `:254` and `:296` restate those divergent types, so a future signature fix must change the
    list too - the U23 shape.
  - Parameter names drift from the SDK in six wrappers (`CheckMenuRadioItem`, `GetMenuString` `nMaxCount`/`uFlag`,
    `HiliteMenuItem` `uItemHilite`, `ModifyMenu` `hMenu`, `InsertMenuItem`/`SetMenuItemInfo` `uItem`) - cosmetic.
  - No `Win32ErrorHandler`, no `CheckReturn` (U1/U2 policy). This family is the sharpest case for the end-of-review
    pass: failure is often reported **in band** (-1 from `GetMenuItemCount` and `MenuItemFromPoint`, 0xFFFFFFFF from
    `GetMenuState` and `GetMenuDefaultItem`) while 24 of the 39 return `BOOL` and none of it is inspected.
- **Open items this session feeds:**
  - **U18** - 0 return-type and 3 parameter-type divergences (the two shapes above), plus 6 parameter-name drifts. The
    `Call<>` lists at `:254` (`InsertMenuT`) and `:296` (`ModifyMenuT`) restate the divergent `void*`, so any fix is a
    three-place change (header decl + def + template list) - the U23 shape again.
  - **U16** - **12 of 39** pair with version-guarded SDK declarations, the largest single-TU count of the review
    (9 at `WINVER >= 0x0400`, 3 at `0x0500`). The pin decision stays an end-of-review item (session 5).
  - **U15** - 4 of 39 have no call site by any route: `GetMenuCheckMarkDimensions` (`:166`), `IsMenu` (`:264`),
    `LoadMenuT` (`:271`), `TrackPopupMenu` (`:341`).
  - **U19** - 40 LAYER / 10 INDIRECT / 18 BYPASS / 6 MEMBER; only 15 BYPASS rows are real calls and all 15 are in
    `retropad.cpp`, whose subdirectory is commented out at `examples/experimental/osspecific/windows/CMakeLists.txt:8` -
    **0 bypass in built code**. The 3 remaining BYPASS rows are `ui/gui` controller *declarations* shadowing SDK stems,
    an instrument false positive now recorded in the U19 entry.
  - **U21 fourth instance + T4** - the SDK menu family is `#ifndef NOMENUS` (`winuser.h:7432`-`:8101`, 38 stems) plus
    `CheckMenuRadioItem` in a second NOMENUS region (`:10426`-`:10535`, the stem at `:10509`). This section owns 36 + 1;
    `IsMenu` (`:4492`) and `GetMenuBarInfo` (`:14740`) pair outside the region; `ChangeMenuA/W` (`:7481`, `:7485`) and
    `CalculatePopupWindowPosition` (`:8073`) are **declared nowhere in this library** (grep over `user32.h` and all of
    `src`: 0 hits) - a sharper gap than U4, since a caller cannot even get a `LNK2019`; and the family's context-help
    pair lives in `user32.h:641 //Shell functions` (`:642-643`), the **T4** pair, still declared, defined in no TU, both
    real exports (`SetMenuContextHelpId` ordinal `34A`, `GetMenuContextHelpId` `17C`).
  - **U26** - the 9 A/W members are also the 9 whose parameter types follow the `UNICODE` switch (`LPCTSTR`,
    `LPMENUITEMINFO`/`LPCMENUITEMINFO`, `MENUTEMPLATE`) while D13 forces the `W` entry point by `##W`; that include-order
    dependency is now explicit in the TU block.
- **Folder totals re-measured after this session:** 428 bindings; unresolvable **17 -> 15**
  (`tmp/_tmp_p1census_post_s22.txt` - `message.cpp` 4, `mouse.cpp` 10, `window.cpp` 1, `menu.cpp` 0); the wrong-stem scan
  stays at 3 folder-wide flags (`tmp/_tmp_wrongstem_s22.txt`, none here); `/EP` over this TU gives 39 literals, all 39 in
  the export table (`tmp/_tmp_menu_pp_s22.txt`, `tmp/_tmp_menu_class_post_s22.txt`). `user32.h` untouched at 695 lines;
  `menu.cpp` 289 -> 355 lines, of which the TU comment block is 66 (`:15`-`:80`).
- **Gate:** folder-wide `tmp/_tmp_cl_all.log` (named copy `tmp/_tmp_cl_all_s22final.log`; earlier copies
  `_tmp_cl_all_s22.log`, `_tmp_cl_all_s22b.log`/`_s22d.log`) - exit 0, **38** TU
  echo lines, **38** fresh `.obj` files, **0** hits for `error C|warning C|fatal`. `cl /EP` re-run after the edit
  (`tmp/_tmp_menu_pp_s22.bat`, own output files) exit 0.
- **Instruments new this session:** `tmp/_tmp_menu_prescan_s22.ps1` (per-stem export facts + prototypes + macro spellings),
  `tmp/_tmp_menu_proto_s22.ps1` (SDK declaration line + parameters - **cannot see the return type**, superseded),
  `tmp/_tmp_menu_ret_s22.ps1` (the return type, read off its own line - the instrument step 2 actually needs),
  `tmp/_tmp_menu_class_s22.ps1` (the four-way D13 classification), `tmp/_tmp_menu_guardsum_s22.ps1` +
  `tmp/_tmp_menu_guardstack.ps1` (guard stack per stem), `tmp/_tmp_menu_tally_s22.ps1` (per-stem LAYER/INDIRECT/BYPASS/
  MEMBER), `tmp/_tmp_menu_bypass_s22.ps1`, `tmp/_tmp_menu_nomenus_s22.ps1` (SDK region walk + guard stack + export
  lookup), `tmp/_tmp_menu_pp_s22.bat`, `tmp/_tmp_s22_logsplit.ps1` + `tmp/_tmp_s22_logsplit2.ps1` (hash-verified record
  slices), `tmp/_tmp_s22_lineref_fix3.ps1`, `tmp/_tmp_s22_chelpid.ps1` (the four `ContextHelpId` declaration ranges behind
  T4), plus the record self-checks `tmp/_tmp_s22_audit.ps1`, `_tmp_s22_consistency.ps1`, `_tmp_s22_verify.ps1` (two bugs
  found in the last one: it counted the TU block's macro mentions as bindings, and an `int + string` concatenation aborted
  it after the first section). Re-run shared censuses: `_tmp_kb_p1census_s20.ps1`, `_tmp_wrongstem_s20.txt`
  scan, `_tmp_compile_all.ps1`.
- **Record hygiene done at the start of this session** (it was flagged by session 21): `archive/session-log.md` was
  **45,673 bytes**, over the ~40 KB ceiling. Rows 1-8 moved to `archive/session-log-1-8.md` (rejoin `MATCH` against sha
  `7B4BDF02…6DCAA9`), then rows 9-16 to `archive/session-log-9-16.md` (rejoin `MATCH` against pre-split sha
  `2E6938EE…B2C4C`), leaving **30,334 bytes / 160 lines** - with the session-22 row now **34,136 bytes / 171 lines**, still
  under the ceiling. Both slices are verbatim; the table header is repeated in each so they read standalone.
- **Hand-off to session 23 (`message.cpp`):** the last 4 P2 conversions (`RegisterWindowMessage`, `SendMessage`,
  `SendMessageTimeout`, `SendNotifyMessage`), the U14(c) `InSendMessageEx` `()` survivor, and the U26 `Message`/`::MSG`
  repair handed over by session 21 (7 call sites, x86 layout divergence from `lParam` down).

