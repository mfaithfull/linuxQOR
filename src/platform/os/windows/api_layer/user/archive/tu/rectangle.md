# rectangle.cpp - session 28 narrative (3 Nov 2026)

11 wrappers, the whole `user32.h:446` `//Rectangle functions` section (`:447`-`:457`). **Verdict: 0 binding, 0 signature,
0 return, 0 idiom defects; 11 lines touched for spacing** - `:20` a space before the argument list's `)`, `:69` a space
before a comma (`int , int`, a shape the review had not seen), and the TU's 11 `Library::Call<>` sites moved from the
spaced to the tight spelling by user decision. TU 93 -> **174** lines (81-line contract block). `user32.h` untouched,
still 695 lines.

**Section shape.** All 11 declarations are `static`, all 11 definitions are in this TU, all through the `User32::`
qualifier, all in header order: `:97` CopyRect, `:104` EqualRect, `:111` InflateRect, `:118` IntersectRect, `:125`
IsRectEmpty, `:132` OffsetRect, `:139` PtInRect, `:146` SetRect, `:153` SetRectEmpty, `:160` SubtractRect, `:167`
UnionRect (post-block lines). So 0 U4 (declared, never defined), 0 U11 (definition unqualified), 0 U13 (defined but
declared non-static), 0 U17 (an A/W family without the folder's `T` suffix) and 0 U18 (a return type contradicting the
SDK). `tmp/_tmp_rect_pair_s28.ps1` pairs header-declaration, TU-definition and SDK prototype for every member: **0
hdr-vs-def and 0 def-vs-sdk parameter-name divergences** - this section copies the SDK's parameter names verbatim
(`lprcDst`, `lprcSrc1`, `xLeft`, `yTop`, ...), the cleanest naming of any TU reviewed.

**Binding (D13).** All 11 stems are **bare single exports**: no `<stem>A`/`<stem>W` row in `tmp/_tmp_user32_exports.txt`
(`5B`, `10B`, `20C`, `223`, `23E`, `29B`, `2B8`, `360`, `361`, `3A3`, `3C0`) and **no `winuser.h` macro** for any stem,
so the bare `qor_pp_useswinapi` at `:100`, `:107`, `:114`, `:121`, `:128`, `:135`, `:142`, `:149`, `:156`, `:163`, `:170`
is the only spelling that could ever bind them - P2 and P6 cannot arise here by construction, and no live probe is needed
to prove it (there is no name being asked for that the DLL lacks). `cl /EP` over the pre-edit TU emits exactly those 11
literals and all 11 are in the export table (`tmp/_tmp_rect_pp_pre_s28.txt`, `tmp/_tmp_rect_class_s28.ps1`: 0 unresolved,
0 macro, 0 P6-shaped). Folder measures unchanged by this TU: 428 bindings, **1** unresolvable (`window.cpp:124`), and the
wrong-stem scan has no `rectangle.cpp` row.

**Signatures.** Eleven members against eleven SDK prototypes, every return `BOOL` (`winuser.h:9752`, `:9762`, `:9768`,
`:9775`, `:9783`, `:9791`, `:9799`, `:9807`, `:9815`, `:9821`, `:9828` - read with `tmp/_tmp_rect_ret_s28.ps1`, which
walks back off the stem line for the return type because the SDK puts it on its own line). `tmp/_tmp_sigcmp.ps1`
`-Tu rectangle.cpp -First 447 -Last 457`: **11/11 `decl==def ok`**, and of the 11 call sites **4 `ok`** and **7
`CALL TYPE MISMATCH` rows holding 11 mismatched arguments** - every one of those 11 is the U25 spelling artifact
(`CONST::RECT*` in the definition vs `CONST ::RECT*` in the template argument), not a wrong type. Arity and types were
therefore checked field-for-field against `tmp/_tmp_rect_ret_s28.txt` instead of trusting the instrument: all 11 template
lists restate the definition's parameter types, and no site deduces its arguments.

**Return conventions (U1/U2).** No `Win32ErrorHandler`, no `CheckReturn` (policy: record, do not touch), and none of the
11 SDK prototypes carries `_Check_return_` - over `winuser.h:9749`-`:9827` the only qualifier lines are `WINUSERAPI` /
`BOOL` / `WINAPI`, so a discarded `FALSE` is silent on the SDK side too. What this section adds to U2 is that the eleven
`BOOL`s are **not one kind of value**: three are answers (`EqualRect`, `IsRectEmpty`, `PtInRect`), three return zero for
a documented outcome rather than a failure (`IntersectRect` writes an all-zero rectangle when the sources do not
intersect, `SubtractRect` when the result is empty, `UnionRect` when no nonempty rectangle results), and only five are
success-or-failure (`CopyRect`, `InflateRect`, `OffsetRect`, `SetRect`, `SetRectEmpty`). A `BoolCheck` would report a
legitimate `FALSE` as an error in six of the eleven today (the `WaitForInputIdle` inverse case from session 27 generalises
to this whole section - three answers plus three documented zero outcomes);
the five real failure cases discard the flag with no trace, and U7 keeps a swallowed SEH fault indistinguishable from a
legitimate zero.

**Consistency (U25).** This TU is the folder's densest `CONST` site: `CONST` is the SDK macro (`minwindef.h:153`,
`ntdef.h:227` - `#define CONST const`), and here it is spelled **three ways in the same construct** - 7 definition lines
write `CONST::RECT*`, the matching 7 `Library::Call<>` lines write `CONST ::RECT*`, and 2 of those template lists also
contain a fourth shape with no `::` at all (`EqualRect :108` and `UnionRect :171` each carry one `CONST RECT*` next to a
`CONST ::RECT*`). Folder census over code lines only (`tmp/_tmp_rect_const_s28.txt`): `CONST::` 17 lines in 4 files,
`CONST ::` 18 lines in 5 files, bare `CONST X` 10 lines in 4 files; `user32.h` itself is 17/0/4. `/EP` proves they are the
same type - `const::RECT` 31, `const ::RECT` 16, `const RECT` 49 literal occurrences, byte-identical pre and post - so
this is presentation only and no edit was made. The instrument consequence is recorded in `gotchas.md`: it is why
`_tmp_sigcmp.ps1`'s call-site compare can only ever flag spelling here, never a type.

**Version guards (U16): 0 of 11.** No prototype in this section is version-gated. The guard stack is identical for all
eleven - innermost `#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)` at `winuser.h:9717`, then the file-wide
`#ifndef NOUSER` at `:64` and `#ifndef _WINUSER_` at `:13` (`tmp/_tmp_rect_pre_s28.ps1` -> `tmp/_tmp_rect_pre_s28.txt`,
11 hits, 0 version guards). Only `ime.cpp` (0 of 1) has previously produced a TU with no guarded prototype at all. The
tree still sets no `WINVER`/`_WIN32_WINNT`/`NTDDI_VERSION`, so the U16 deferral stands.

**Reachability (U19 / D14).** Each of the eleven has exactly one layer caller, the `qor::platform::win::Rect` facade in
`ui/gui/view/drawing/rect.cpp` (`:37` CopyFrom, `:42` Equal, `:47` Inflate, `:53` FromIntersection, `:59` IsEmpty,
`:64` Offset, `:70` IsPointInRect, `:75` Set, `:80` SetEmpty, `:86` FromSubtraction, `:93` FromUnion). Over the 44
spellings of the eleven stems: **11 LAYER / 2 intermediate-qualified / 3 BYPASS / 0 member-call**
(`tmp/_tmp_rect_bypass_s28.txt`). Both "INDIRECT" rows are the **new false-positive class** - they are same-named member
*definitions*, not calls into the DLL (`EditController::SetRect` at `ui/gui/controllers/a_h/editcontroller.cpp:220` sends
`EM_SETRECT` through `SendMessageT`, and `Region::SetRect` at `ui/gui/gdiobjects/region.cpp:48` calls
`GDI32::SetRectRgn`). The 3 BYPASS rows are live code in the built `winqor_gl` target (`ui/gl/CMakeLists.txt:5`):
`ui/gl/windowimpl.cpp:516` and `:521` call an unqualified `OffsetRect` right next to
`WindowController::AdjustRectForDpi`/`AdjustRect` calls, and `:1756` calls `::SetRect`. A fourth grep-visible
`OffsetRect` at `windowimpl.cpp:1470` is **not** a call site - it sits inside the block comment that opens at `:1303` and
closes at `:1507`, which the scan's comment filter excluded correctly (`tmp/_tmp_comment_state.ps1`). One level above the
layer, `rect.cpp:53`, `:86` and `:93` discard the `BOOL` and return the `RECT` the SDK filled, so "the sources do not
intersect" and "the result is empty" are indistinguishable to every caller of this façade.

**Coverage (method step 7).** All eleven section exports are wrapped. In the same SDK sub-block, `DrawFocusRect`
(`:9722`) is wrapped in `painting.cpp`, `FillRect` (`:9729`), `FrameRect` (`:9737`) and `InvertRect` (`:9745`) in
`filledshapes.cpp`; `SetSysColors` (`:9706`) is declared at `user32.h:651` and **no TU defines it** (a U4 instance found
from this section); `GetWindowWord` (ordinal `1FB` / `00086890`, `winuser.h:9837`) and `SetWindowWord` (`388` /
`0002FD20`, `:9844`) have **no member anywhere in `user32.h`**. The region scan (`tmp/_tmp_rect_region_s28.ps1`,
`winuser.h:9700`-`:9870`) counted 20 prototype heads, 15 with a member; its two `GetWindowLongA`/`GetWindowLongW` "no
member" rows are an instrument artifact (the member is `GetWindowLongT`, `user32.h:625`) and are recorded in `gotchas.md`.
The family scan (`tmp/_tmp_rect_family_s28.ps1`) over the 51 export names containing `Rect`: 41 declared in `winuser.h`,
39 with a member, so the SDK-visible gaps are `EvaluateProximityToRect` and `GetPointerDeviceRects` (pointer family) plus
the two `*WindowWord` names. No header line was touched.

**Idiom and gate.** 11/11 `qor_pp_fcontext`, 11/11 explicit `Call<>` template arguments (0 deducing), 11/11 bare
`qor_pp_useswinapi(user32, Stem)`. After the user-authorised normalisation the TU is **0 spaced / 11 tight**, moving the
folder to **163 spaced / 254+11 = 265 tight** (`tmp/_tmp_u14_post_s28.txt`) and, being the last reviewed TU with spaced
sites, closing U14 by decision **D15**. TU hygiene final: 0 TAB, 0 trailing-space, 0 non-ASCII lines, longest line 124.
Gate: the edited TU compiles standalone, and the library compiles **38/38 at `/W4` with no diagnostics**
(`tmp/_tmp_cl_all_s28b.log`, run after the last edit); `/EP` before and after gives 453,328 -> 453,409 physical lines with
the **non-blank multiset identical at 154,338** except the 11 intended call lines, and all 11 binding literals
byte-identical (`tmp/_tmp_rect_neutral_s28.txt`). `user32.h` untouched at 695 lines; `:11` still includes
`../returncheck.h` and uses nothing from it (T7's 35 dead includes).

