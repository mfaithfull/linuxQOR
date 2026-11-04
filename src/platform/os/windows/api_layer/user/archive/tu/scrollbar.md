# `scrollbar.cpp` - session 30 (3 Nov 2026)

**Shape.** 12 wrappers, `user32.h:573` `//ScrollBar functions`, declarations `:574`-`:585`; reviewed at 102 physical
lines / 4,079 B and closed at **156 lines / 9,347 B** - the contract block `:14`-`:68` is the whole delta, no code line
added or removed. Definitions now `:70 :77 :84 :91 :98 :105 :113 :120 :128 :135 :142 :149`; bindings `:73 :80 :87 :94
:101 :108 :116 :123 :131 :138 :145 :152`; `Library::Call<>` `:74 :81 :88 :95 :102 :109 :117 :124 :132 :139 :146 :153`.
All 12 bindings are bare `qor_pp_useswinapi(user32, Stem)` - **0 `AW`**, which is correct: all 12 stems are bare single
exports (no A/W family, **0** `#define Stem` UNICODE aliases; `tmp/_tmp_scroll_regions_s30.txt`). No `T` suffix anywhere,
so the section is P3/U17-free. 0 TABs, 0 trailing-whitespace lines, 0 non-ASCII (`tmp/_tmp_s30_hygiene.txt`).

**Fixed - the only defect class found: the module token.** All 12 bindings spelled it `User32`, i.e.
`qor_pp_useswinapi(User32, ...)`, the **only** such spelling site in the folder (12 of its 428 bindings). Ground rule G4
named `combobox.cpp` as a co-offender; that file no longer spells it that way, so G4's named population was stale as
well as its wording. `kernel32.h:50` stringizes the token, so `/EP` emitted `GetModuleHandleW(L"User32")` where the other
416 bindings emit `L"user32"`. Normalized with `tmp/_tmp_s30_fix_tokens.ps1`, and proved neutral three ways:

- `/EP` before vs after (`tmp/_tmp_scroll_neutral_s30.ps1` -> `tmp/_tmp_scroll_neutral_s30.txt`): identical line count
  (453,337), **12 differing lines, every one of them the module literal and nothing else**, binding-literal multiset
  identical (12 vs 12).
- a live probe built **x64 and x86** (`tmp/_tmp_scroll_g4probe_s30.cpp` + `.bat`): `user32` / `User32` / `USER32` /
  `user32.dll` all resolve to one `HMODULE`, a bogus name resolves to NULL (so the probe discriminates), and
  `GetProcAddress` yields the **same address under every spelling for 12 of 12 entry points - 0 of 12 differ** on either
  arch (`tmp/_tmp_scroll_g4probe_s30_x64.txt`, `_x86.txt`).
- the added comment block is code-neutral: raw `/EP` +54 lines, **code-only lines 154,346 both sides with identical
  SHA-256** `557C147D279FC46D...` (`tmp/_tmp_s30_ep_commentproof.ps1`).

Folder census after the fix (`tmp/_tmp_s30_gatecheck.txt`, `tmp/_tmp_s30_final_measures.txt`): **428 bindings, 428 tokens
`user32`, 0 files with a non-lowercase token.**

**Signatures.** `tmp/_tmp_sigcmp.ps1 -Tu scrollbar.cpp -First 574 -Last 585`: 12/12 `decl==def ok` (return type,
parameter count, types, names). Its single `CALL TYPE MISMATCH` row - `ScrollDC` args 3-4, declaration
`const ::RECT *lprcScroll` vs the `Call<>` list's `const ::RECT*` - is a spelling artifact of a **new kind**: pointer
`*` spacing, not U25's `CONST::` shape. The instrument must normalize whitespace; the source is not to be bent to
satisfy it (recorded in `gotchas.md`). Three-way name compare, header / definition / SDK, case-sensitive
(`tmp/_tmp_scroll_names_s30.ps1`): **0 header-vs-definition divergences**; def-vs-SDK 4 names over 3 members -
`wSBFlags` vs SDK `wSBflags`, `fnBar` vs `nBar` (in both `GetScrollInfo` and `SetScrollInfo`), `fRedraw` vs `redraw`.
U18's layer-vs-SDK half: published names, not touched.

**SDK-side (U16).** 12 prototypes: **12 inside a `WINAPI_PARTITION_DESKTOP` block, 0 PC_APP-only, 0 unguarded**, only
**2 version-guarded** (`tmp/_tmp_scroll_guards_s30.txt`, `tmp/_tmp_scroll_stack_s30.txt` - depth-counted). Those two are
`SetScrollInfo` (`winuser.h:12052`) and `GetScrollInfo` (`:12061`) under `#if(WINVER >= 0x0400)` opened `:12026` and
closed `:12069` - this item's **first 0x0400 gate** (all earlier cases were 0x0500/0x0501). Innermost guard per
prototype: 7 under `#ifndef NOSCROLL` (`:8811`, the item's first `NOSCROLL` case), `ScrollDC`/`ScrollWindow`/
`ScrollWindowEx` at the file-wide `#ifndef NOUSER` (`:64`) inside the partition block at `:8751`, and `GetScrollBarInfo`
(`:14763`) under `#ifndef NOWINABLE` (`:13716`, first `NOWINABLE` case) whose innermost **version** guard is the
file-wide `#if(WINVER >= 0x0500)` at `:13594`. `user32.h:574`-`:585` declares all twelve unconditionally and the tree
still sets no `WINVER`/`_WIN32_WINNT`/`NTDDI_VERSION`, so the deferral stands (session 5).

**Family coverage (U21).** `user32.dll` exports whose name contains "Scroll": **13**, 12 of them wrapped. The 13th,
**`ScrollChildren`** (ordinal 2283 / 0x30D), is exported but **not declared anywhere in the installed SDK** - the only
`ScrollChildren` token under `Include\10.0.26100.0` is the `SW_SCROLLCHILDREN` flag at `winuser.h:8801`
(`tmp/_tmp_scroll_family_s30.ps1`, `tmp/_tmp_scroll_children_s30.ps1`), so it is not wrappable from the headers and is
not a gap in this layer. Each of the 12 wrapped stems has exactly **1** declaration site in `winuser.h` - no A/W pair, no
second spelling anywhere.

**Callers (U19).** 8 of the 12 wrappers have exactly one in-tree caller, `qor::platform::win::ScrollBarController`
(`ui/gui/controllers/s_s/scrollbarcontroller.cpp:52, :57, :62, :67, :72, :75, :82, :87`), which is compiled into
`winqor_gui` (`ui/gui/CMakeLists.txt:33`). `GetScrollBarInfo`, `ScrollDC`, `ScrollWindow` and `ScrollWindowEx` have **no
in-tree caller** (D14: published surface, no row). **0 BYPASS, 0 MEMBER.** The scan's 8 "INDIRECT" rows are again **not
calls into `user32.dll` at all** - they are the controller's own same-named member *definitions*, the false-positive
class already recorded for `rectangle.cpp` in session 28, so every past row's INDIRECT figure remains an upper bound.
Nothing in-tree constructs `ScrollBar` or `ScrollBarController` (`tmp/_tmp_scroll_callers_s30.ps1` ->
`tmp/_tmp_scroll_callers_s30.txt`), so this whole consumer chain is latent, not live.

**Caller-side mirror (U26).** The controller passes `reinterpret_cast< SCROLLINFO * >(&si)` at
`scrollbarcontroller.cpp:57` and `:72`. Its struct `ScrollInfo` (`scrollbarcontroller.h:33`-`:42`) was mirrored against
the SDK `SCROLLINFO` (`winuser.h:12037`-`:12046`) with `sizeof`/`offsetof`, **x64 and x86**
(`tmp/_tmp_scroll_mirror_s30.cpp` + `.bat`): **same size (28 bytes) and all seven field offsets identical on both
arches** - the casts are sound, unlike the `MSG`/`Message` mirror. The genuine finding is inside the mirror: `nPos`
(`h:40`) and `nTrackPos` (`h:41`) have **no default member initializer** while the other five fields do (`cbSize{sizeof}`,
`fMask{0}`, `nMin{0}`, `nMax{0}`, `nPage{0}`), so a default-constructed `ScrollInfo` handed to `GetScrollInfo`/
`SetScrollInfo` without setting those two carries an indeterminate `int` through the layer - and `SetScrollInfo`
**reads** both. `GetScrollBarInfo`'s `::PSCROLLBARINFO` parameter has no caller-side mirror in the tree at all, so
`SCROLLBARINFO` (60 bytes, `rgstate[CCHILDREN_SCROLLBAR+1]` = 6 entries) is unexposed.

**Return policy (U2).** 0 `CheckReturn`, 0 `Win32ErrorHandler`, and **0 of the 12 prototypes carry `_Check_return_`**
(`tmp/_tmp_s30_measure2.txt`: "total SAL return-qualifiers over the 12 prototypes: 0"), so a discarded failure is silent
on both sides. The 4 `int`-returning members all have **0 as a legal value** - `GetScrollPos` (current position),
`SetScrollPos` (previous position), `SetScrollInfo` (resulting position), `ScrollWindowEx` (flag/error code) - which is
exactly the case session 27's rule bars a `CheckNonZero` in; the 8 `BOOL` members are success-or-failure but all 8 drop
the flag with no trace. Policy unchanged (recorded, nothing touched).

**T7.** `scrollbar.cpp:11` includes `../returncheck.h` and uses none of it, so T7's dead-include count moves **35 -> 36**.

**Gate.** Single-TU `/W4` compile `cl_exit=0`. Full library `tmp/_tmp_cl_all_s30b.log`: **exit 0 as seen by pwsh (not the
`cl` exit - `gotchas.md`), 43 log lines, 38 TU echo lines, 1 diagnostic-looking grep hit that is the filename
`errorhandling.cpp`, and 38 `.obj` files newer than the edited source** (`scrollbar.obj` 19:34:09 vs source 19:23:55) -
the dating rule session 29 established, since a clean 38-TU log is byte-identical to its predecessors.

**Pre-scan artefacts for this TU** (kept for re-derivation): `tmp/_tmp_scroll_pre_s30.txt`,
`tmp/_tmp_scroll_pre_s30b.txt`, `tmp/_tmp_scroll_ret_s30.txt`, `tmp/_tmp_scroll_names_s30.ps1`,
`tmp/_tmp_scroll_guards_s30.txt`, `tmp/_tmp_scroll_regions_s30.txt`, `tmp/_tmp_scroll_stack_s30.txt`,
`tmp/_tmp_scroll_family_s30.ps1`, `tmp/_tmp_scroll_children_s30.ps1`, `tmp/_tmp_scroll_callers_s30.txt`,
`tmp/_tmp_scroll_mirror_s30_{x64,x86}.txt`, `tmp/_tmp_scroll_g4probe_s30_{x64,x86}.txt`,
`tmp/_tmp_scroll_neutral_s30.txt`, `tmp/_tmp_s30_ep_commentproof.txt`, `tmp/_tmp_s30_hygiene.txt`,
`tmp/_tmp_s30_measure2.txt`, `tmp/_tmp_s30_final_measures.txt`, `tmp/_tmp_s30_gate_final.txt`,
`tmp/_tmp_s30_gatecheck.txt`.
