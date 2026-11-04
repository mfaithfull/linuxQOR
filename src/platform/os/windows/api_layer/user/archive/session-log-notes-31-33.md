# session log - close-out notes for sessions 31 (`systemshutdown.cpp`) and 33 (`window.cpp`)

Moved verbatim out of `archive/session-log.md` on 4 Nov 2026 when that file passed the 40 KB ceiling (40,916 bytes).
Source range: lines 
226
-
347
 inclusive. Moved block `SHA256 
1F2CF8F8522031B1D70A99B73DB58AB544DF0E1792A086176701F81A487D7540
` (computed before
the move; the block below is that block verbatim). Rejoin verified by hash at slice time. The table rows for these
sessions (rows 31 and 33) stay in `archive/session-log.md`.

### Session 31 - `systemshutdown.cpp` (4 Nov 2026)

5 wrappers, `user32.h:601`-`:606` under the header's own misspelled label `//System Shutdwown functions`, 51 -> **116
lines** (contract block only). **One defect class, fixed:** `:37` and `:44` read `BOOL WINAPI User32::ShutdownBlockReasonDestroy`
/ `...Query` - the **only two `WINAPI`-qualified `User32::` definition heads in all 38 TUs**, while `user32.h:605`/`:606`
declare those members with no convention token. Removed. Not a style call: MSVC rejects the disagreement when the TU is
compiled **for x86** with `error C2373: 'qor::win::api::User32::ShutdownBlockReasonDestroy': redefinition; different type
modifiers` (same at `:44`), i.e. the TU was **ill-formed** - and on x64, the only arch the tree configures
(`CMakeSettings.json:4` `x64-Debug`, generator `Visual Studio 17 2022`), the modifier is dropped, so those two heads changed
nothing. Proof of neutrality, three ways: a caller probe (`tmp/_tmp_s31_winapi_probe_s31.cpp`) shows the symbol MSVC emits
and the symbol a consumer of the header declaration references are **the same `__cdecl` decoration** on both sides
(`?ShutdownBlockReasonDestroy@User32@api@win@qor@@SAHPEAUHWND__@@@Z`, `tmp/_tmp_s31_winapi_s31.txt`); `dumpbin /disasm` of
the object is **identical before and after** (42,310 lines each, 2 differing lines, both the dump header:
`tmp/_tmp_s31_x64_pre_s31.asm` / `_post_s31.asm`); and the `/EP` diff is **55 lines, of which 51 blank**, the other 4 being
only the two definition heads (`BOOL __stdcall User32::...` -> `BOOL User32::...`) - **non-blank lines 154,302 both sides**,
all 5 binding literals byte-identical.
**Binding (D13):** 5/5 bare single exports, no A/W family - `ExitWindowsEx` 10F, `LockWorkStation` 26C,
`ShutdownBlockReasonCreate` 39C, `ShutdownBlockReasonDestroy` 39D, `ShutdownBlockReasonQuery` 39E; a family walk over the
1,018-name table gives **exactly 1 export per stem** and the `um` headers carry **0 `#define <Stem>` alias sites**, so the
bare `qor_pp_useswinapi` at `:19, :26, :33, :40, :47` is the only spelling that can bind them (`/EP` emits exactly those
5 literals against `L"user32"`); 0 P1, 0 P2, 0 P6.
**Signatures:** 5/5 `decl==def` on return type, parameter types, parameter names and `Call<>` arity
(`_tmp_sigcmp.ps1 -Tu systemshutdown.cpp -First 602 -Last 606`), 0 name divergences of any kind - the first TU with no U18
name traffic at all.
**U16:** 5/5 prototypes are `WINAPI_PARTITION_DESKTOP`, **exactly one version-gated** - `LockWorkStation` under
`#if(_WIN32_WINNT >= 0x0500)` (`winuser.h:14912`); `ExitWindowsEx` (`:3639`, also inside `#ifndef NOUSER`) and the
`ShutdownBlockReason` trio (the `#pragma region Desktop Family` block opened `:15919`, which also defines
`MAX_STR_BLOCKREASON 256`) carry **no version floor**. This **refutes the session-30 U16 lead** ("expect the highest
version floor yet, likely 0x0601") - the trio has none, and no guard name outside the population already recorded appears.
**U19:** 0 callers anywhere - the comment-stripped scan over `src`, `examples` and `test` with all five stems prints
**0 LAYER / 0 INDIRECT / 0 BYPASS / 0 MEMBER**; published surface, so **no U19 or U15 row** (D14).
**U1/U2:** 5/5 return `BOOL`, 0 `Win32ErrorHandler`, 0 `CheckReturn`, and **0 `_Check_return_` on the 5 prototypes**;
`ShutdownBlockReasonDestroy`'s `FALSE` distinguishes `ERROR_SHUTDOWN_IN_PROGRESS` from `ERROR_NOT_FOUND`, so the
session-28 answer-shaped-`BOOL` caution applies. `:11` includes `../returncheck.h` and uses none of it - **T7 36 -> 37**.
**Build note (recorded in `registers/u6.md`, no new register):** `kernel32.h:92` names `PUNWIND_HISTORY_TABLE`, which the
SDK typedefs only under `#if defined(_AMD64_)||defined(_ARM_)||defined(_ARM64_)` (`winnt.h:21939`-`:21963`), so that one
header line makes **93 of the 106 api_layer TUs fail with C2061 on x86** (`tmp/_tmp_s31_x86_apiall_s31.txt`, digest
`tmp/_tmp_s31_x86_digest_s31.ps1`); the 13 survivors are the library glue TUs, and `kerrorhandling.cpp:104` then fails with
C2065 `'TargetGp': undeclared identifier`. The same x86 blind spot is what kept U6's stack corruption invisible until the
x86 probe caught it, and it is what hid this TU's C2373 pair from 30 sessions of x64-only gates.
**Gate:** single TU `cl_exit=0` (`tmp/_tmp_compile_one.ps1`); library `tmp/_tmp_cl_all_s31c.log` **38 echoes / 0
diagnostics**, dated by `.obj` freshness (all 38 newer than the edited source). Census unchanged: 428 bindings, 428
lowercase `user32` module tokens, call-parens 421 tight / 0 spaced, this TU contributes 0 spaced `Call< ` sites (U14 frozen,
D15). Narrative `archive/tu/systemshutdown.md`.
**Pre-flags for the next TU (`text.cpp`, 44 lines / 2,028 B, `user32.h:567`-`:571` under `//Font and Text functions`, 4
members `DrawTextT`, `DrawTextExT`, `GetTabbedTextExtentT`, `TabbedTextOutT`):**
- **All 4 bindings are `qor_pp_useswinapiAW` and all 4 stems are A/W-only with no bare export**: `DrawTextA` DB `00036A30`
  / `DrawTextW` DE `00024390`, `DrawTextExA` DC `00053630` / `DrawTextExW` DD `00023E50`, `GetTabbedTextExtentA` 1CB / W
  1CC, `TabbedTextOutA` 3AB / W 3AC; a word-boundary walk of the export table prints **`DrawText`, `DrawTextEx`,
  `GetTabbedTextExtent`, `TabbedTextOut` all count=0** - `AW` is D13-correct and P6 cannot arise (unlike `CallMsgFilter` /
  `BroadcastSystemMessage`).
- **The bare names exist only as SDK macros or a managed inline**: `#define DrawText DrawTextA` (`winuser.h:8271`,
  `!UNICODE`) plus `#if defined(_M_CEE)` `__inline int DrawText(` at `:8274`-`:8278`, while `DrawTextEx`,
  `GetTabbedTextExtent` and `TabbedTextOut` have **0 SDK declarations**. The binding macro stringizes the pasted name, so
  the macro never expands there - but consumers see the macro, which is a U26 include-order question for these 4 names.
- **`user32.h:571` spells the parameter type `CONST::LPINT`** (no space) where `:570` uses `CONST LPINT`, and `text.cpp:36`
  copies `CONST::LPINT` in the head but `CONST ::LPINT` in its `Call< >` list - a U14(a)-family token-spacing site,
  cosmetic only (U14 itself is frozen by D15).
- **Hygiene:** longest line **175**, 3 comment lines, **no contract block** - add one. Parse signatures with
  `_tmp_sigcmp.ps1 -Tu text.cpp -First 568 -Last 571` (the section head is `:567`; `-First 655` lands in
  `//Authorization functions` and parses nothing).
- **Return policy (U1/U2):** `int`, `int`, `DWORD`, `LONG`; `DrawText` returns the text height and `DrawTextEx` a line
  count or error code, so test the session-27 rule ("only sound where the failure value is not also a legal success value")
  against 0 before any check idea. 0 checks present, and `:11` includes `../returncheck.h` unused - **T7 37 -> 38** if left.
- **Expect U16 traffic:** the four prototypes sit at `winuser.h:8248`, `:8260`, `:8306`, `:8320`, `:8448`, `:8460`, `:8478`,
  `:8487` - region-walk the guards with the depth-counted walk, never a backward scan (`winuser.h:4121`).
### Session 33 - `window.cpp` (4 Nov 2026)

**The last TU of the review with defects, and the largest: 615 -> 737 lines, 85 wrappers, 6 code fixes.** Row 33 carries
the verdict; narrative `archive/tu/window.md`. The four that matter:

- **P1 is now closed folder-wide.** `CreateWindowT` asked `GetProcAddress` for `CreateWindowW`, which **does not exist
  and cannot**: `CreateWindow`/`CreateWindowW` are *function-like macros* in `winuser.h:4464`/`:4468` that expand to
  `CreateWindowExA/W(..., 0, ...)`, and the binding macro stringizes the pasted token before any macro can expand
  (ground rule G1). Fixed by binding the real export `qor_pp_useswinapiAW(user32, CreateWindowEx)` and passing the
  `dwExStyle = 0` literal at the call (`window.cpp:189`-`:200`). Every spelling of that stem is therefore a P1, and the
  folder census went **1 -> 0 unresolvable of 428** (`tmp/_tmp_p1census_pre_s33.txt` ->
  `tmp/_tmp_p1census_final2_s33.txt`). Two instruments agreed independently, which is why the fix is stated as fact: the
  source-level classifier `tmp/_tmp_s33_prebind.ps1` and the **`/EP`-authority** classifier
  `tmp/_tmp_win_ppclass_s33.ps1` (pre: `MISSING CreateWindowW`, 2 `…WW`, 17 `…T`; final: 0/0/0).
- **U18's founding pair fixed in place**: `LogicalToPhysicalPoint` (`:505`) and `PhysicalToLogicalPoint` (`:528`) now
  return `BOOL` via `Library::Call<BOOL, HWND, ::LPPOINT>`; the `void` + `voidCall` pair discarded the only failure
  signal the SDK offers (`winuser.h:9526`, `:9533`). Declarations `user32.h:188`, `:191` changed with them - the review's
  largest **published-signature** change, taken deliberately, with a consumer-side compile probe
  (`tmp/_tmp_win_consumer_s33.bat` -> `tmp/_tmp_win_consumer_s33c.log`, compiling `ui/gui/controllers/windowcontroller.cpp`)
  at `CL_EXIT=0`, 0 `error C`, 11 warnings (C4100 x9, C4189 x2, all pre-existing in `ui`). Three captures exist -
  `_s33.log` (pre-fix), `_s33b.log` (post-fix), `_s33c.log` (post-final-edit) - and all three are **line-identical**, which
  is the neutrality proof.
- **Two `const`s the layer had dropped** (`SetWindowPlacement` `:593`/`:597`, `TileWindows` `:649`/`:653`) restored to
  the SDK's `_In_ const WINDOWPLACEMENT *` (`winuser.h:4775`) / `_In_opt_ CONST RECT *` (`:12240`), header `:200`, `:208`.
- **U14(b)'s three deducing sites closed** (`:186`, `:327`, `:390`) - the folder is **428 explicit / 0 deducing**
  (`tmp/_tmp_u14_final2_s33.txt`) while U14(a) stays at its D15 freeze (163 spaced / 265 tight), i.e. this TU was 0
  spaced before and after.

**Gate:** `tmp/_tmp_cl_all_final3_s33.log` - 38 TU echoes, 0 diagnostics, run 22:25:10, dated by freshness not by log name
(`tmp/_tmp_s33_gatecheck.ps1`: **0 stale `.obj`**, `window.obj` 22:25:10 vs `window.cpp` 20:11:31). The `final2` capture
(19:43:37) and every `_tmp_cl_all_s33*.log` before it are **not** the post-edit gate - `final2` was clean but pre-dated a
later comment-only edit to the same TU, which is how the close-out caught it.

**Instruments written this session** (indexed in `tooling.md`): `_tmp_s33_preflags.ps1` (**broken head regex - reported 0
heads, do not trust**), `_tmp_s33_prebind.ps1`, `_tmp_s33_pre.ps1` (per-member bind/export/SDK-line/guard table),
`_tmp_win_pp_s33.bat` + `_tmp_win_ppclass_s33.ps1`, `_tmp_win_bypass_s33.txt` + `_tmp_s33_u19sum.ps1`,
`_tmp_win_mirror_s33.cpp` + `.bat` (x64 + x86 struct mirror), `_tmp_win_consumer_s33.bat` (captures `_s33.log`, `_s33b.log`,
`_s33c.log`), `_tmp_s33_verify.ps1`,
`_tmp_s33_gatecheck.ps1`, `_tmp_s33_floors.ps1` (**produced no facts - see `gotchas/sessions-31-33.md`**),
`_tmp_s33_rec1.ps1` (record-side digest of the saved captures).

**Record actions:** registers updated `prescan.md` (P1 CLOSED, P5 re-measured), `census.md` (session-33 block),
`u14.md`, `u16.md`, `u18.md`, `u19.md`, `u25.md`; `archive/tu/window.md` corrected (it was written mid-session and its
`NOMDI` list, U25 count, call-paren count and comment-line count were wrong - see the gotcha entry); `status.md` live
state rewritten and its locator regenerated; `memory-bank/activeContext.md` + `progress.md` booked.

**Next session: `windowclass.cpp`** (114 lines, 14 wrappers - 12 `qor_pp_useswinapiAW` + 2 bare `qor_pp_useswinapi`
(`:47 GetClassWord`, `:82 SetClassWord`), 14 `qor_pp_fcontext`, 14 `Library::Call<`, **0** `voidCall`, **0** `WINAPI`
tokens, 3 comment lines, longest 124, **no contract block**). Header section `user32.h:620`-`:633`. Measured pre-flags:
- **AW risk class, and this TU is the highest-risk residue of it:** `GetClassLongPtr` / `SetClassLongPtr` /
  `GetWindowLongPtr` / `SetWindowLongPtr` / `GetWindowLong` / `SetWindowLong` are bound through `…AW`, so each needs the
  four-way export check (`<Stem>A`, `<Stem>W`, bare) against `tmp/_tmp_user32_exports.txt` before anything else -
  `GetWindowLong`/`SetWindowLong` bare exports **do exist**, which is the `CallMsgFilter`/P6 shape (a bare name that
  resolves to the ANSI entry point on wide arguments), while `*Ptr` families need confirming that `…A`/`…W` exist at all.
- **`CONST` traffic in the header**: `user32.h:626`/`:627` spell `CONST WNDCLASS*` / `CONST WNDCLASSEX*` - U25's shape,
  and `_tmp_sigcmp.ps1` cannot parse the typedef-const form (use `_tmp_s32_names.ps1`).
- **U16**: expect version floors on the `*Ptr` and `GetClassInfoEx` members; walk with a depth counter only.
- Run `_tmp_s33_verify.ps1`-style counts with **`-CaseSensitive`** for `CONST`/`const`, and date the gate by `.obj`
  freshness. Remaining queue after it: `windowprocedure.cpp` (30), `windowproperties.cpp` (51), `winqor_api_user.cpp`
  (14, glue) - 209 lines total.

