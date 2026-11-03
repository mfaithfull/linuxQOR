# `resource.cpp` - review narrative (session 29, 3 Nov 2026)

**30 lines -> 41 lines, 2 wrappers, 1 real defect fixed (U11 = U4 x2), 2 whitespace nits, 0 header lines moved.**
The smallest reviewed TU except the 14-line glue file: the whole `//Resource functions` section of `user32.h` (`:442`
label, members `:443 CopyImage` / `:444 LoadImageT`), both `HANDLE`-returning, both static members.

## The defect: the folder's only U11 pair, and it was both directions of U4 at once

Both definitions were **free functions**: `resource.cpp:16 HANDLE CopyImage(...)` and `:23 HANDLE LoadImageT(...)` inside
`namespace qor::win::api`, with no `User32::` qualifier. `user32.h:443`/`:444` declare both as `static` members of
`class qor_pp_module_interface(QOR_WINAPIUSER) User32` (`user32.h:66`), so at once:

- the two declared members had **no definition anywhere** in the folder - `_tmp_undefined_static.ps1` listed exactly these
  two and nothing else folder-wide, i.e. U4 x2 and the only remaining live instances of that direction; and
- the two emitted free functions were **not just dead but unpublished**: `dumpbin /exports` of the built DLL gives 439
  named exports and `dumpbin /linkermember:1` of the import library 854 `api@win@qor` member symbols, and neither name is
  in either list (`tmp/_tmp_res_exports_s29.txt`, `_tmp_res_lnkimp_s29.txt`; artefacts dated 29 Oct / 22 Oct, valid as
  "before" evidence only because this TU had not been touched since - recorded in `registers/u11.md`).

That this never surfaced as a link failure is simple: **nothing calls either wrapper**. The U19 scan over the 8 suffix
spellings of both stems gives 0 LAYER / 0 INDIRECT / 0 BYPASS / 0 MEMBER; the 3 `LoadImageW(` hits in
`ui/gl/windowimpl.cpp` (`:1322`, `:1326`, `:2339`) all sit inside the comment blocks `1303-1507` and `2221-2498`
(`tmp/_tmp_comment_state.ps1`), so there is no bypass either. Under **D14** unreachability is not a finding by itself, and
here it also means the fix cannot break a caller - which is what made the qualifier safe to apply in-session.

Measured before/after by compiling and dumping the TU's own object (`tmp/_tmp_res_ver_s29.cpp`, then
`tmp/_tmp_res_final_s29.log` step 3):

```
before  ?CopyImage@api@win@qor@@YAPEAXPEAXIHHI@Z                       (free: YA)
        ?LoadImageT@api@win@qor@@YAPEAXPEAUHINSTANCE__@@PEB_WIHHI@Z
after   ?CopyImage@User32@api@win@qor@@SAPEAXPEAXIHHI@Z                (static member: SA)
        ?LoadImageT@User32@api@win@qor@@SAPEAXPEAUHINSTANCE__@@PEB_WIHHI@Z
```

`tmp/_tmp_res_ref_s29.cpp` is the proof that the pair now closes: a consumer TU taking the address of both members
compiles, and linking it against `resource.obj` leaves **0** unresolved externals for either name. The 11 that remain are
`ThisModule`, `qor::FunctionContext`, the `qor::When/Where/What/Serious` flyers and `Kernel32::GetModuleHandleW`/
`GetProcAddress` - normal cross-module noise when a single object is linked alone. Reading that log needs the gotchas.md
predicate: the member symbols appear in it as the *referencing* function of those noise rows, so a bare grep of
`tmp/_tmp_res_link_final_s29.txt` reports 10 "unresolved members" where the truth is 0.

Also fixed inside the same two definitions, both flagged beforehand as separate nits: `int , UINT` -> `int, UINT` in the
first `Call<>` template list, and `>( pFunc` -> `>(pFunc` - the folder's **last** spaced call-paren site, census now
421 tight / 0 spaced.

## What needed no change

- **Bindings, both already D13-correct.** `CopyImage` is a single export with no A/W family and no SDK macro, so the bare
  `qor_pp_useswinapi(user32, CopyImage)` at `:30` is its correct spelling and `/EP` emits the literal `"CopyImage"`.
  `LoadImage` is the textbook P6/D13 shape - bare **not** exported, `LoadImageA` (`winuser.h:10906`) and `LoadImageW`
  (`:10916`) exported, `#define LoadImage LoadImageW` - so `qor_pp_useswinapiAW(user32, LoadImage)` at `:37` is required
  and emits `"LoadImageW"`. Both literals byte-identical across the three `/EP` captures; bare `"LoadImage"` and
  `"LoadImageA"` appear 0 times. **0 P1, 0 P2, 0 P6.**
- **Signatures.** `tmp/_tmp_sigcmp.ps1` over `user32.h:443-444` vs the TU: return type, every parameter type, every
  parameter name and the `Call<>` arity/type lists all agree - 2 declarations, 2 definitions, **0** divergences - and both
  restate the SDK prototypes `HANDLE CopyImage(HANDLE, UINT, int, int, UINT)` /
  `HANDLE LoadImageW(HINSTANCE, LPCWSTR, UINT, int, int, UINT)`. `LoadImageT` carries the folder's `_T` suffix (A/W
  family), `CopyImage` correctly carries none (single export) - the T-suffix census agrees.
- **Return policy unchanged.** Both return the raw `HANDLE`; no `CheckReturn`, no `Win32ErrorHandler`, no
  `qor_pp_fresult` - a NULL failure is silent here, the settled policy since session 3, recorded not changed. The
  `../returncheck.h` include at `:11` is unused (T7, 35 such includes folder-wide) and stays.
- **`LPCTSTR` at `user32.h:444` is the already-recorded U26 interaction**, not a new item: `qor_pp_useswinapiAW` pastes
  `LoadImageW` unconditionally while `LPCTSTR`'s width follows the include-order `UNICODE` define. With the library's own
  include chain it is `LPCWSTR` and matches the binding; a TU that sees `<windows.h>` before `kernel32.h` would pass a
  narrow string to the wide entry point. Instance noted, nothing touched.
- **SDK guard (U16):** both declarations sit under `#if(WINVER >= 0x0400)` at `winuser.h:10883`. The installed header
  groups them by `#pragma region Desktop Family` (`:10900`); the label `help.cpp:27`-`:32` cites - the classic SDK's
  "Shell Support Functions" - occurs **0** times in any installed `um` header, so the section-label lead closes as a
  wording difference between SDK generations, not a placement defect (U12/U21-shaped; nothing moved).

## Edits and evidence

Three code edits, all inside the two definitions, then the contract block: the `User32::` qualifier x2, the two whitespace
nits, and a 10-line block inside the namespace (`:16`-`:25`) recording what is wrapped, the U11 fix and both binding
rationales - `resource.cpp` was the only reviewed TU without one (`tmp/_tmp_res_blockchk_s29.ps1`; the remaining
block-less sources are exactly the 7 unreviewed TUs plus the glue TU). Final shape: **41 lines, 2,281 B, 13 comment
lines, 0 TAB / trailing space / non-ASCII, longest line 142 (`:38`)**; definitions `:27`/`:34`, bindings `:30`/`:37`,
`Call<>` sites `:31`/`:38` (spaced bracket `Call< ` x2 left alone - U14 is CLOSED by D15).

Gate: single-TU `/W4` `cl_exit=0` (`tmp/_tmp_res_cl_s29.log`); full library `tmp/_tmp_cl_all_s29b.log` - 38 echoes,
0 diagnostics, exit 0, run 15:00:28 with all 38 TU objects newer than the 14:58:40 source. `/EP` neutrality across the
three captures (`tmp/_tmp_res_neutral_final_s29.txt`): 453,265 -> 453,265 -> 453,276 lines, **non-blank identical at
154,284 throughout**, exactly 4 non-blank lines differing each way (the two signatures + the two `Call<>` lines), both
binding literals byte-identical; the block contributes 11 emitted lines, all blank. `user32.h` untouched at 695 lines.

Folder instruments after the session (`tmp/_tmp_res_scan_final_s29.txt`): 428 bindings, **1** unresolvable
(`window.cpp:124 CreateWindow`) - unchanged; wrong-stem scan **3 rows -> 1** (only `cursor.cpp:64` `CopyCursorT` binds
`CopyIcon`, P3); **0** `static` declarations with no definition folder-wide; 0 parameter-name divergence over 428
definitions; call-paren census **421 tight / 0 spaced**; bracket census unchanged at 265 tight / 163 spaced.

Two instrument faults of my own, both in the neutrality scripts, both recorded in `gotchas.md`: a
`Where-Object { $arr -notcontains $_ }` multiset diff over 154,284 non-blank lines is O(n^2) and did not finish in 300 s
- twice, because the rewrite of the first script reintroduced the same shape. The hash/`Group-Object` versions are the
ones cited. And a `dumpbin | findstr` symbol filter matched noise, since the member/free distinction lives in the mangled
`SA`/`YA` letters rather than in anything `findstr` can word-match.

**Verdict: 1 of 2 wrappers broken at the symbol level in both directions at once - the declared member undefined, the
defined function unpublished - now repaired and proven from the consumer side. Bindings, signatures and return policy
were already correct.** U11 is CLOSED; U4's `static`-declaration direction drops by 2 to **0** folder-wide; the last
contract-block gap among reviewed TUs is closed.
