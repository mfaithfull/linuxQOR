# `message.cpp` (session 23, 2 Nov 2026) - narrative (verbatim)

**Scope:** 24 wrappers - the whole `user32.h:289`-`:313` `//Message and Message Queue functions` section; definitions
`message.cpp:106`-`:267` in header order. TU grew 184 -> 274 lines (90-line TU comment block). 0 `user32.h` lines moved,
so no `user32.h:NNN` citation in the record shifted; the header stays **695 lines**.

**Fixes (4 binding conversions + 1 signature nit + 3 spacing nits):**
- the entire remaining **P2** list - `qor_pp_useswinapi` -> `qor_pp_useswinapiAW` at `:214 RegisterWindowMessage`,
  `:235 SendMessageTimeout`, `:242 SendMessage`, `:249 SendNotifyMessage`. **Behaviour-neutral**, proved the same way as
  `dialog.cpp`'s `MessageBox` and `menu.cpp`'s pair: pre-fix `cl /EP` already emitted the four `"*W"` literals (the SDK
  macro rewrote the bare stem before stringizing), post-fix emits the same four via `##W` pasting (`kernel32.h:52`).
  Evidence: `tmp/_tmp_msg_pp_pre_s23.txt` vs `tmp/_tmp_msg_pp_post_s23.txt` (differ in the `InSendMessageEx` lines only),
  `tmp/_tmp_msg_class_{pre,post}_s23.txt` (24 literals, 24/24 in the export table, 0 unresolved), and the folder census
  **15 -> 11** unresolvable of 428 with no `message.cpp` row left (`tmp/_tmp_p1census_post_s23.txt`).
- **U14(c) closed**: `DWORD User32::InSendMessageEx()` at `:176` -> `(void)`, matching declaration `user32.h:300`. It was
  the folder's last zero-argument `()`; the folder now reads 38 defs / 38 decls all `(void)`.
- comma-space in the binding macro at `:19`, `:26`, `:68` (folder norm 414 spaced vs 15 not; after this TU 414/12, the
  remainder all in `scrollbar.cpp`).

**Binding verdict for the other 20:** 12 bare bindings are each a genuine single export - `tmp/_tmp_msg_class_s23.ps1`
checks every stem three ways (export table `tmp/_tmp_user32_exports.txt` 1018 names, `<stem>A`/`<stem>W` presence, and
whether `winuser.h` defines a `#define <stem>` macro at all) and for all 12 the bare name is exported while no A/W spelling
and no macro exist, so bare is the only spelling that could ever resolve. The 12 `AW` stems are exactly the 12 with a live
`winuser.h` macro.

**New finding - P6's second instance (avoided):** bare `BroadcastSystemMessage` **is** exported (ordinal `14`, RVA
`00092270` = the same RVA as `BroadcastSystemMessageA`), while `BroadcastSystemMessageW` is a separate entry point at
`00007240`. A bare binding there would resolve and run the ANSI entry point on wide arguments rather than throw - the
`CallMsgFilter` trap. This TU's `AW` binding asks for the `W` name, so it does not apply. `tmp/_tmp_msg_class_s23.ps1`
prints this as the TU's only "P6-shaped" stem.

**Signatures:** **0 return-type divergences over all 24**, which needed a new instrument - `tmp/_tmp_msg_ret_s23.ps1`
reads the SDK return type off the line above the stem, because `winuser.h` puts it there (the session-22 caveat). Split:
11 `BOOL`, 3 `LRESULT`, 2 `long`, 2 `LPARAM`, 3 `DWORD`, 1 `LONG`, 1 `UINT`, 1 `VOID`. All parameter types agree too.
The one deliberate deviation is `InSendMessageEx` (`:176`): published with no parameter where the SDK has
`_Reserved_ LPVOID lpReserved` (`winuser.h:4253`); the `Call<DWORD, LPVOID>` list passes `nullptr` into that reserved
slot, so the call shape matches the export. Both `CALL TYPE MISMATCH` flags in the pre-flags were instrument artifacts
(`tmp/_tmp_msg_pre_s23.txt` - the tool read this library's own prototype as the SDK side; `PostQuitMessage`'s flags were
the *next* function's `Call<>` list).

**Registers touched:** `prescan.md` (**P2 CLOSED, 0 remain**), `p6-u27.md` (2nd instance), `u16.md` (4 of 24 version-guarded:
`SetMessageExtraInfo` WINVER 0x0400, `BroadcastSystemMessageEx*` `_WIN32_WINNT >= 0x0501`, `BroadcastSystemMessage*`
WINVER 0x0400 + `defined(_WIN32_WINNT)`, `InSendMessageEx` WINVER 0x0500), `u19.md` (172 LAYER / 12 INDIRECT / 68 BYPASS /
2 MEMBER; **all 24 wrappers have a through-layer site** - the first TU with no zero-site family - and **0 bypass in built
code**), `u21.md` (fifth instance: `NOMSG` opens 6x and `NOWINMESSAGES` 6x in `winuser.h`; `SetMessageQueue` is exported
and declared nowhere in this library; `CallMsgFilter*`/`TranslateAccelerator*`/`IsDialogMessage*`/`TranslateMDISysAccel`
are the SDK's message family in four other TUs), `u25.md` (0 `CONST` here; a fourth spelling - `const MSG*` with no `::`),
`u26.md` (the hand-off from session 21 re-measured for x64 and x86: x64 identical 48/48, x86 `MSG` 28 vs `Message` 40 -
divergent from `lParam` on; the layer's own signatures are SDK-exact, so the fix belongs to the caller's `winqor_gui`
mirror header), `census.md` (folder totals).

**Gate:** `tmp/_tmp_cl_all_s23recover.log` - 38/38 TUs at `/W4`, exit 0, 0 diagnostics, re-run 2 Nov 13:11 after the last
source edit (the `_s23final`/`_s23pre` copies left by the interrupted pass are session 22's log byte-for-byte, see the
`census.md` note); `cl /EP` of the TU exits 0 (`tmp/_tmp_msg_pp_final_s23_err.txt` = the TU name only).
Final `/EP` output `tmp/_tmp_msg_pp_final_s23.txt` (11:38) is the one to diff - the `_post_` capture (11:17) predates the
`InSendMessageEx(void)` header edit, so it is not the post-fix state. Instrument set: `tmp/_tmp_msg_pp_s23.bat`,
`_tmp_msg_class_s23.ps1`, `_tmp_msg_ret_s23.ps1`, `_tmp_msg_guards_s23.ps1`, `_tmp_msg_bypass_s23.ps1`,
`_tmp_msg_u19_s23.ps1`, `_tmp_msg_regions_s23.ps1`, `_tmp_msg_pos_s23.ps1`.
