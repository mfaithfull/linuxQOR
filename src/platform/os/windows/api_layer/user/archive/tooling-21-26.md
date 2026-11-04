# archive/tooling-21-26.md - tooling sections for sessions 21-26 (verbatim slice)
Sliced out of `tooling.md` on 3 Nov 2026 (session 30). Was `tooling.md:73-184`. Read-only history: append new
tooling to `tooling.md`, never here.

## Tooling added in session 21 (`mdi.cpp`)
- `tmp/_tmp_int_sites_s21.ps1` - the **`Call<>`/`voidCall<>` template-argument correctness** scan. It extracts every
  `(Call|voidCall)\s*<[^>]*>` on a line and tests it case-**sensitively** for a standalone `INT` token, which is how a
  template list can contradict the signature of the function it sits in. Caveats: case-sensitivity is the whole point
  (`int` is the correct spelling for many SDK parameters, and a case-insensitive scan flags well over a hundred
  legitimate sites across the folder); the regex stops at the first `>`, so it only reads single-level type lists; and it
  can only see sites where the arguments were spelled out - the 4 deducing sites of U14(b) are invisible to it. Its
  denominator is raw source text, so comments inflate it: `tmp/_tmp_callcount_s21.ps1` counts 435 `Call<>`/
  `voidCall<>` template lists against 429 `qor_pp_useswinapi`/`...AW` occurrences and the P1 census' 428 code-line
  bindings (`mdi.cpp` alone reads 7 because its TU comment names `Call<>` and the macros). Quote 435 for this scan,
  428 for the census; the one site it found (`DefFrameProcT`) is now **0**. Pair it with `tmp/_tmp_sigcmp.ps1`, the
  only instrument that compares a call site against the declaration at all.
- `tmp/_tmp_msg_mirror_s21.cpp` + `tmp/_tmp_msg_mirror_s21.bat` - the **mirror-struct layout probe** (U26's method). The
  caller-side field lists are copied verbatim into a probe TU next to the SDK type and `sizeof`/`offsetof` printed for
  every field. The rule it establishes: **build it twice**, x64 (`vcvars64.bat`) and x86 (`vcvarsamd64_x86.bat`) - on
  x64 alone this probe reported "identical" and U26's third instance would have been missed. Two caveats measured:
  `offsetof` on a member that the SDK guards with `#ifdef _MAC` (`MSG::lPrivate`) is a hard C2039/C2618 error, so print
  only fields both sides have; and launching a freshly-built exe from inside the batch failed
  (`'..._x64.exe' is not recognized` while cmd had already created the `.txt` redirect file), so **run the two exes from
  a separate invocation** and keep the batch for the builds. Same lesson as the U6 disassembly probe
  (`archive/tooling-13-20.md`): x64-only evidence is structurally blind to this defect class.
- Per-TU instrument naming settled this session: `<family>_{prescan,bypass,guards,pp,probe}_s<N>` plus the shared
  censuses re-run with an `_s<N>` output suffix (`_tmp_p1census_s21.txt`, `_tmp_tsuffix_s21_full.txt`,
  `_tmp_cl_all_s21.log`). The `_tmp_mdi_guards_s21.ps1` wrapper exists only because `tmp/_tmp_sdk_guard_scan.ps1` takes a
  `-Names` **array** and `pwsh -File script.ps1 -Names A,B` delivers one string - the same trap as the U19 stem wrapper
  recorded in `archive/tooling-13-20.md`. A probe that resolves names must also record that it had to `LoadLibraryW` first:
  `GetModuleHandleW(L"user32")` fails with gle=126 in a bare console process (G5), and a probe that reports every name
  MISSING is measuring the loader, not the export table.

## Tooling added in session 22 (`menu.cpp`)
- **The SDK return type is on its own line, so a declaration-line instrument cannot see it.** In `winuser.h` a
  prototype is `WINUSERAPI` / `BOOL` / `WINAPI` / `Stem(` on four separate lines, so `tmp/_tmp_menu_proto_s22.ps1`
  printed the stem line plus the parameters and reported nothing about the return type - method step 2 looks satisfied
  and is not. `tmp/_tmp_menu_ret_s22.ps1` walks back from the stem up to 4 lines, skips `WINAPI`/`WINUSERAPI`/`CALLBACK`
  and blank lines, and takes the first remaining token as the return type; that is what produced the 39/39 agreement
  here. Reuse the pattern for every future TU, and treat "return types match" as unmeasured unless such an instrument
  ran.
- **A Win32 family is defined by a macro region, not by a line range, and a header can open the region twice.**
  `tmp/_tmp_menu_nomenus_s22.ps1` walks `#ifndef NOMENUS` ... `#endif` spans with a depth counter and counts the
  declaration heads inside each; it found **two** such regions (`winuser.h:7432`-`:8101` and `:10426`-`:10535`) and the
  second holds exactly one stem (`CheckMenuRadioItem`), which is why that one wrapper pairs with a `WINVER` guard
  instead of the region's own condition. Same shape for `NOMDI`/`NOSYSTEM` etc - always enumerate the markers, never
  assume one span.

## Record-editing tooling from the U15 closure pass (2 Nov 2026, no TU reviewed)
- **A closed register entry must be removed from the generator's own instructions**, not just from the register file:
  `tmp/_tmp_status_index_gen_s22.ps1` writes the `registers/README.md` preamble that lists the accumulating entries, so
  leaving `u15` in that list would re-tell the next session to append to a frozen entry. It now names
  `(u16, u17, u19, u20)` and states that u15 is closed (policy D14).
- **`tmp/_tmp_u15_locator_verify.ps1`** (read-only): parses every `status.md` locator bullet and asserts the
  `registers/<file>:<line>` it names really starts that id's entry. Every bullet must resolve except the one deliberate
  exception - U27 points at the shared `registers/p6-u27.md` P6 extract bullet. Run it after any register-entry insert or
  deletion, not just after a split.

## Tooling added in session 23 (`message.cpp`)
- **`tmp/_tmp_msg_class_s23.ps1`** - per-stem bare-vs-AW classifier (for each stem: is `<stem>` exported, do `<stem>A`/`<stem>W`
  exist, does `winuser.h` define a `Stem` macro?). This is the instrument that answers the D13 question per TU and it is the
  one to clone for the remaining bare-binding TUs (`mouse.cpp`, `window.cpp`); session 22's `_tmp_menu_class_s22.ps1` was its
  first form. Output `tmp/_tmp_msg_class_pre_s23.txt` / `_post_s23.txt` - **compare them**, they are byte-identical when a
  bare -> `AW` conversion is behaviour-neutral.
- **`tmp/_tmp_msg_ret_s23.ps1`** - session 22's return-type instrument pointed at `winuser.h`; monitor/message stems live in
  `winuser.h` too, where the return type is on the line above the stem and the qualifier lines (`WINUSERAPI`, `_Check_return_`)
  above that. It reported **0 of 24** divergences; the two `CALL TYPE MISMATCH` rows it emits for `InSendMessageEx` and
  `PostQuitMessage` are artifacts - the "sdk ''" side is this library's own prototype, and the reported "types" are the *next*
  function's `Call<>` list. Always read a `CALL TYPE MISMATCH` against the source lines before acting.
- **`tmp/_tmp_msg_regions_s23.ps1`** (U21 region census: every `#ifdef`/`#ifndef` name that opens a region containing a
  section), **`_tmp_msg_guards_s23.ps1`** (U16 `WINVER` guards per SDK declaration), **`_tmp_msg_bypass_s23.ps1`** +
  **`_tmp_msg_u19_s23.ps1`** (U19 LAYER/INDIRECT/BYPASS/MEMBER), **`_tmp_msg_pos_s23.ps1`** (def positions vs header order),
  **`_tmp_msg_pp_s23.bat`** (own `/EP` output files, per the shared-`_tmp_pp_out.txt` caveat).
- **`tmp/_tmp_u14c_s24.ps1`** closes the U14(c) count and is named `_s24` although session 23 ran it - a naming slip; keep
  instrument names tied to the session that ran them, later searches for `_s23` miss it.
- **`tmp/_tmp_s23_closeout.ps1`** - the **record close-out audit**, six checks: gate log (dated by its own redirect, not by
  a copy), `session-log` slice rejoin hashes + row-id uniqueness across the four slices, record ceilings, `status.md`
  locator pointer resolution, `archive/` + `registers/` README coverage, and edited-TU hygiene/binding counts. Run it
  before writing a session's row and again before committing; output `tmp/_tmp_s23_closeout.txt`. Two counting rules it
  encodes, both of which bit this session: a range placeholder row (`| 24..38 | not started |`) is **not** a session id, and
  a TU's contract block names the binding macros in prose (`message.cpp:20`), so binding counts must exclude comment lines -
  the real `message.cpp` split is **12 `AW` / 12 bare over 24 definitions**, which is what its contract block claims.

## Tooling added in session 24 (`monitor.cpp`)
Instruments (all session-scoped names, `tmp/`): **`_tmp_mon_pp_s24.bat`** (own `/EP` output pair, per the
shared-`_tmp_pp_out.txt` caveat); **`_tmp_mon_class_s24.ps1`** - the per-stem bare/AW/P6 classifier, a clone of
`_tmp_msg_class_s23.ps1` with the stems swapped, parameterised by `-PpFile` so the pre and post `/EP` outputs run through
the same code path; **`_tmp_mon_mirror_s24.cpp` + `_tmp_mon_mirror_s24.bat`** - the third struct-mirror probe
(`MONITORINFO`/`MonitorInfo`, `MONITORINFOEXW/A`/`MonitorInfoExW/A`, `POINT`/`Point`, `RECT`/`Rect`,
`MONITORENUMPROC` argument widths), built for **x64 and x86 in one batch run**; **`_tmp_mon_probe_s24.cpp` +
`_tmp_run_mon_probe_s24.bat`** - the live `GetProcAddress` name probe (the `*T` and `Ex` spellings are included so the
probe also disproves the spellings the TU does *not* ask for); `_tmp_mon_pre_s24.ps1`, **`_tmp_mon_region_s24.ps1`** - the
SDK region walker (walks the `#if(WINVER >= 0x0500)` block opened at `winuser.h:13594` with a `#if/#endif` depth counter
and prints every declaration head **plus which TU defines it**, which is how U21's 45-head region got its 6/17/22
attribution; the earlier `_tmp_mon_pre2_s24.txt` hand-count of the same region guessed "34 in `window.cpp`" and was
wrong - measure, do not bucket), `_tmp_mon_guards_s24.ps1`, `_tmp_mon_bypass_s24.ps1`, and
**`_tmp_s24_closeout.ps1`** - the record close-out audit, seven checks (gate log dated by its own redirect and compared
against the last source edit; record ceilings; every locator pointer resolving to the named line; the generated locator
matching the spliced block; `archive/` + `registers/` index coverage; TU hygiene; `/EP` multiset neutrality).

Session 26 (`painting.cpp`, 3 Nov 2026) instruments: `_tmp_paint_pp_s26.bat` (own `/EP` output files), `_tmp_paint_ret_s26.ps1`
(multi-line winuser.h return+params dump for the 25 stems), `_tmp_paint_class_s26.ps1` (per-stem bare/AW/P6 classifier),
`_tmp_paint_pre_s26.ps1` (header/def/SDK three-way name compare for the section), `_tmp_paint_guards_s26.ps1` (innermost-guard
walk), `_tmp_paint_probe_s26.cpp` + `_tmp_paint_probe_s26.bat` (x64 + x86 GetProcAddress probe AND a runtime A/W mirror: it
measures what `DrawStateA` vs `DrawStateW` / `GrayStringA` vs `GrayStringW` actually draw with identical opaque arguments),
`_tmp_s26_pre.ps1` (one wrapper that re-runs the four folder-wide scans; remember `pwsh -File x.ps1 -Stems A,B` delivers ONE
string, not an array - call the scans from a wrapper script instead). **New PowerShell gotcha, same family as the `-ne` case
trap above:** `-replace 'T$'` is **case-insensitive**, so stripping the folder's `T` suffix from a name ending in a lowercase
`t` eats the wrong character - `BeginPaint` became `BeginPain` and six stems silently fell out of the SDK-side compare until
switched to `-creplace`. `_tmp_sdk_param_name_scan.ps1:238-244` had already documented this trap and avoided it by trying the
exact spelling first; any new instrument that strips `T` MUST use `-creplace`. Two more for the close-out:
`_tmp_paint_lines_s26.ps1` (prints the current def/binding/call-site line numbers of the TU - the only safe way to
re-point a contract block's self-citations after an insertion) and `_tmp_s26_closecheck.ps1` (block self-citations +
spot-checks + record sizes). It improves on `_tmp_s25_verify.ps1`: external pointers are classified by an explicit
`-ExternalZones` line-range list rather than flagged as stale, so the run ends at **0 out of range** instead of 4.

