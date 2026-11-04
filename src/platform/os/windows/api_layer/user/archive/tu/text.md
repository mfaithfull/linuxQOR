# `text.cpp` - session 32 narrative (4 Nov 2026)

**Verdict: 0 defects, 0 code edits.** 4 wrappers over the whole `user32.h:567` *Font and Text functions* section
(`:568` `DrawTextT`, `:569` `DrawTextExT`, `:570` `GetTabbedTextExtentT`, `:571` `TabbedTextOutT`) - a header range the
previous session's pre-flags stated one line too wide (`:567-:571`; `:567` is the section comment).

## What this TU is

The folder's **only TU where every bound stem has its SDK A/W macro live at the binding site**:
`#define DrawText DrawTextW` (`winuser.h:8269`), `DrawTextEx` (`:8331`), `GetTabbedTextExtent` (`:8494`), `TabbedTextOut`
(`:8470`), each with the A form in the `#else`. All four call sites use `qor_pp_useswinapiAW(user32, <bare stem>)`, which
pastes `W` onto the name (`_NAME##W`, `kernel32.h:52`) **before** `<bare stem>` could macro-expand - so the macro cannot
double-append, and the four literals that reach the loader are exactly `"DrawTextW"`, `"DrawTextExW"`,
`"GetTabbedTextExtentW"`, `"TabbedTextOutW"` (`cl /EP`, `tmp/_tmp_text_pp_s32.txt`). The exports table confirms the other
side: bare `DrawText`/`DrawTextEx`/`GetTabbedTextExtent`/`TabbedTextOut` are **not** exported (count 0) while the A and W
forms all are (`DrawTextW` ordinal DE, `DrawTextExW` DD, `GetTabbedTextExtentW` 1CC, `TabbedTextOutW` 3AC). So P1, P2 and
P6 are **correct by construction** here - the G1 ground rule working as intended, and the `…WW` exports that a
double-suffixed literal would need do not exist (a wrong spelling would have been an unresolvable binding, not a silent
call into the wrong function).

**Return-shape caution.** The SDK marks `DrawTextA/W` and `DrawTextExA/W` `_Success_(return)` (`winuser.h:8245`, `:8257`,
`:8303`, `:8317`), so a non-zero test would match the SDK's own assertion for those two; `TabbedTextOutA/W` and
`GetTabbedTextExtentA/W` carry no annotation and return **measurements** (a width; a packed `DWORD` with LOWORD width /
HIWORD height), where 0 is a legitimate result. 0 of 4 members checks its return (U1/U2 stay folder-wide). This also
**refutes the pre-flag lead** that `GetTabbedTextExtent` might return `SIZE` - both SDK forms and the wrapper return
`DWORD`.


## Two contracts the layer cannot state (register items, not fixed)

1. **U23, second instance.** `GetTabbedTextExtentT`/`TabbedTextOutT` take the tab-stop array as `CONST LPINT`
   (`user32.h:570`/`:571`, definitions `text.cpp:76`/`:83`). In a member-function parameter list that is `INT * const` -
   const on the **pointer** - whereas the SDK prototype is `const int *lpTabStopPositions` (`winuser.h:8458`, `:8489`).
   Proved rather than asserted: `tmp/_tmp_s32_u23_probe.cpp` (a class member taking `CONST LPINT`, called with a
   `const int[]`) **compiles**, and the negative control `tmp/_tmp_s32_u23_probe_neg.cpp` (SDK type, same call) fails
   `C2664` - "cannot convert const int* to int * const; conversion loses type qualifiers". Consequence: a caller handing
   over a read-only array gets **no compile-time protection** against a callee that writes it, so the wrapper's contract
   is weaker than the contract it forwards. Not fixed: header surface, goes with U17/U13 in the folder-wide pass.
2. **No register id (recorded at `registers/u23.md:19`).** With the `DT_MODIFYSTRING` format bit the SDK declares the
   text buffer `_Inout_z_` and requires room for **four more characters** (`winuser.h:8250`, `:8309`). The wrapper's
   parameter is `LPCTSTR`, which matches the SDK's own `LPCWSTR` spelling - so there is **no divergence to fix**, only a
   caller duty the signature hides. Documentation, not a defect.

## Convention and register tallies

4/4 declarations pair with a definition here, all `static`, all `User32::`-qualified, all in header order -> **0 U4, 0
U11, 0 U13, 0 U17, 0 U18** (5th consecutive TU with no U18 traffic). `tmp/_tmp_sigcmp.ps1 -Tu text.cpp -First 568 -Last
571` is clean but for its known `CONST ::LPINT` artifact, and `tmp/_tmp_s32_names.ps1` reports **0 definition-vs-SDK type
divergences** and **0 header-vs-definition parameter-name divergences**; definition-vs-SDK *names* differ on all four
members (13 names, e.g. `nCount` vs `chCount`, `X` vs `x`) - published names only, no ABI effect. **U25 0 instances**
(8 const sites, all `CONST`/`CONST::` prefixed), with one data point for the folder-wide pass: the same SDK pointer type
is spelled `CONST LPINT` at `:570` and `CONST::LPINT` at `:571`, and the definitions mirror the split (`text.cpp:76` /
`:83`). **U16**: 4/4 members sit in the `WINAPI_PARTITION_DESKTOP` block; only `DrawTextExT` has a version floor
(`#if(WINVER >= 0x0400)`, `winuser.h:8301`-`:8335`); `DrawTextT` is gated only by **`NODRAWTEXT`** (`:8241`/`:8340`) - a
guard name this review had not met before - and the Tabbed pair (`:8456`-`:8495`) has neither. **P5** call-paren: 4 tight
/ 0 spaced. **U19 0 instances**: the `DT_*` flag family (19 flags) appears **only inside comments** in-tree
(`themedata.cpp:168`, `:192`), and the one caller in that file passes only `DT_EDITCONTROL`, which is not even in the
SDK's `DT_*` macro list - the flag-usage scan is therefore recorded as unusable for this family until comments are
stripped. **U26 0** (no mirrored struct; `RECT`/`LPRECT` are referenced, not re-declared). **U15: D14 applies** - all
four stems have live out-of-folder consumers (12 in `ui/gui/painting/themedata.cpp` over 13 call parens, 2 in
`winqor_kernelobj.cpp`), so no instance is recorded: 0 wrong-stem bindings, 0 dead exports, P5 stays 428.

**Comment-only `User32` token measure closed** (`registers/u15.md:211`): `user32.h` 14 -> **12** (2 of the 14 are live
code - `:102` `User32::DragDetect` defined at `menu.cpp:27`, `:290` `User32::GetCursor` at `cursor.cpp:37`) and
`text.cpp` 0 -> **12**; every remaining token is either live code or review prose, so the measure stops here.

## How the (comment-only) edit was proved

Neutrality: diffed the `cl /EP` captures of the TU with `tmp/_tmp_s32_neutral.ps1` / `_neutral2.ps1` (interim) and
**`tmp/_tmp_s32_neutral3.ps1` - the final proof, original pre-edit capture vs the finished file** -
**non-blank lines 154,296 before and after, 0 non-blank differences**, line-count delta exactly the comment lines added
(+81 over the session), each of the four W literals still present exactly once on a binding line, and **0** occurrences of
`"DrawText"` or `"DrawTextWW"` in the post capture (`_tmp_text_pp_s32_post3.txt`, taken after the final
next-TU-line correction). Full-library gate: `tmp/_tmp_cl_all_s32.log` (mtime **4 Nov 2026 05:09:39**), **38/38 TU
echoes, 0 errors, 0 warnings** at `/W4 /std:c++20 /utf-8`, and `tmp/_tmp_s32_gatecheck.ps1` reports
**0 stale or missing `.obj`** in `tmp/obj`. The log's SHA256 is byte-identical to earlier sessions' clean logs, which is
precisely why freshness - not the hash - dates the gate (`gotchas.md`).

## Process failures caught inside the session (now `gotchas.md` entries)

- A large in-place rewrite of this TU's closure block **silently deleted ~40 comment lines** while reporting success.
  Detected by grepping for the block markers (`//=== api_layer conventions ===`, `//closure:`) and counting lines. After
  any multi-line comment rewrite: verify the markers, count the file, then re-prove neutrality (the final proof above was
  run against the *repaired* file).
- A batch that mixed `cd` with relative-path `Add-Content` created three stray `u18.md`/`u23.md`/`u25.md` in the repo
  root (deleted; the registers were re-read and confirmed intact). Always absolute paths, never a `cd` in the same batch.
- The first "gate" quoted this session was a **3 Nov log copy** carried forward - the batch that was supposed to capture
  the run had the compile and its checker racing. `tmp/_tmp_s32_gatecheck.ps1` now checks TU-echo count, diagnostics and
  `.obj` freshness together, and the gate log is captured with `cmd /c ... > log 2>&1` in its own call.
- `tmp/_tmp_s32_queue_audit.ps1` re-derived the review queue from the session logs and found the **Queue table** in
  `status.md` was wrong: a duplicated `text.cpp` row, a missing `windowprocedure.cpp` row, and `window.cpp` at 270 lines
  when it is **615**. 33 of 38 TUs are reviewed; the five left are `window.cpp`, `windowclass.cpp`,
  `windowprocedure.cpp`, `windowproperties.cpp`, `winqor_api_user.cpp`.
