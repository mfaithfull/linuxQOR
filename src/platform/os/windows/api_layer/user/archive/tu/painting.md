# Session 26 - painting.cpp (closed 3 Nov 2026)

25 wrappers, the whole `user32.h:262 //Painting functions` section (`:263-:287`), header order, all static, all defined
here: 0 U4, 0 U11, 0 U13. TU 191 -> 261 lines; the only source edits were **two whitespace nits** - `GetUpdateRect`'s
`Call<>` at `:153` spaced none of its four arguments where the other 24 sites all do, and `ValidateRect` at `:244` had a
stray space before `)`. Nothing else in the TU needed changing, which the evidence below shows rather than asserts.

**Bindings (D13).** All 25 `/EP` literals resolve against the user32 export table (`tmp/_tmp_paint_class_s26.ps1` over
`tmp/_tmp_paint_pp_pre_s26.txt`: 25 stems, 0 literals missing from the table, 0 P2-shaped, 0 P6-shaped). 23 stems are
single exports with no A/W family and no winuser.h macro at all, so their bare `qor_pp_useswinapi` is load-bearing.
Only `DrawState` and `GrayString` are A/W families (`DrawStateA D9`, `DrawStateW DA`, `GrayStringA 1FD`, `GrayStringW
1FE`), and they are exactly the two members that carry the `T` suffix and exactly the two `AW` bindings - the folder's
first TU where the existing `AW` usage is not just harmless but *required*, so `painting.cpp` adds 0 to P1 and 0 to P2.
The live probe (`tmp/_tmp_paint_probe_s26.cpp`, x64 + x86) resolves all 25 spellings at the table RVAs and reports
bare `DrawState`/`GrayString` and both `*T` names MISSING with gle=127.

**Signatures.** 25 members against 27 SDK prototypes; every return agrees (tmp/_tmp_paint_ret_s26.txt - the SDK puts
returns on their own line, session 22's caveat); 0 hdr-vs-def parameter divergences, 2 def-vs-sdk renames
(`DrawCaption`, `DrawStateT`) that are names, not signatures.

**Registers fed.** U16: 8 of 27 prototypes guarded, 7 members, first `_WIN32_WINNT >= 0x0501` case (`GetWindowRgnBox`,
`winuser.h:8668`). U19: 17 LAYER / 11 INDIRECT / 3 BYPASS rows, of which 2 real - `ui/gui/painting/paintsession.cpp:41`
and `:47` call the SDK `BeginPaint`/`EndPaint` thunks directly while `windowcontroller.cpp:520`/`:537` use the layer,
the first layer-vs-thunk side-by-side in this section (the third row, `windowcontroller.h:235`, is a member
declaration: scanner artifact). U26: `DrawState`/`GrayString` are the width-invisible pair - the payload travels in an
opaque `LPARAM`/`int`, so no caller-side type reveals the A/W choice; measured at runtime, the choice changes what is
drawn (ANSI buffer 7797 vs 357 changed px for DrawState; 36 vs 85 wide buffer; 3427 vs 3371 for GrayString) on both
arches. Failure mode is wrong output, not corruption.

**Instrument bug found and fixed.** The three-way compare stripped `T` with `-replace 'T$'` - case-insensitive, so it
ate the trailing `t` of six stems (`BeginPaint` -> `BeginPain`, likewise EndPaint, DrawFocusRect, GetUpdateRect,
InvalidateRect, ValidateRect), which printed as `sdk=0` rows. Fixed with `-creplace`; recorded in `tooling.md`. The
2016-era `_tmp_sdk_param_name_scan.ps1` had documented the trap and worked around it by trying exact spellings first.

**Idiom.** 25/25 `qor_pp_fcontext`, 25/25 explicit `Call<>` (0 deducing; folder 425/3 unchanged), 25/25 tight `Call<`.
No return checks here per the U1/U2 policy; the handle/integer-returning members (BeginPaint, GetWindowDC, WindowFromDC,
the five region calls) would be U7 blind anyway.

**Gate.** `tmp/_tmp_cl_all_s26.log`: 38/38 at `/W4`, exit 0, 0 diagnostics, after the final TU edit at 01:03:33. `/EP`
pre 453,426 -> final 453,496 lines with the non-blank multiset differing by exactly the two edited `Call<>` lines; the
25 binding literals byte-identical pre vs final.
