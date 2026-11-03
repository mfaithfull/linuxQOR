# session log table rows 27-28 + their close-out notes (moved out of archive/session-log.md on 3 Nov 2026, session 29)

Rows 27 (`processthread.cpp`) and 28 (`rectangle.cpp`) and the two close-out notes that followed them are kept here
verbatim - the live table passed the ~40 KB ceiling with row 29, so they were sliced out by exact line match.
Session numbers in the `#` column are the original ones; nothing below was edited.

| # | TU | Wrappers | Verdict | Action taken |
|---|---|---|---|---|
| 27 | `processthread.cpp` | 5 | **0 binding, signature, return or idiom defects; whitespace-only fixes** (the folder's last TAB line at old `:29`, three stray spaces before `)` at old `:21`/`:46`/`:54`, and the TU's 5 `Call<>` sites normalised to the folder's tight spelling by user decision - U14(a) moved deliberately for the first time) | Whole `user32.h:255 //Process and Thread functions` section (`:256`-`:260`): 5/5 decl/def pairs in header order, 0 U4/U11/U13/U17/U18, **0 hdr-vs-def and 0 def-vs-sdk parameter renames**, returns all agree (`tmp/_tmp_pt_ret_s27.txt`), `_tmp_sigcmp.ps1` 5/5 `decl==def ok` **and** 5/5 call-site `ok` - that instrument had to be fixed first, it read one line so 3 of these 5 wrapped calls printed `CALL NOT PARSED`. Binding: 5 bare single exports, **no A/W family and no `winuser.h` macro**, so the bare `qor_pp_useswinapi` it already had is the only spelling that can bind them - 5/5 `/EP` literals resolve, 0 P1/P2/P5/P6 from this TU (`tmp/_tmp_pt_class_s27.ps1`); no live probe needed (no `AW` site, no name to disprove). | TU 57 -> **122** lines (64-line contract block); `/EP` pre 453,292 -> final 453,357 with the non-blank multiset differing only by the 7 whitespace-touched lines and the 5 binding literals byte-identical. Registers: **U16** 3 of 5 prototypes version-guarded, 5 of 5 DESKTOP-only, second `_WIN32_WINNT >= 0x0501` (`IsWow64Message`) plus the **empty** `#if (_WIN32_WINNT >= 0x0602)`/`#endif` pair at `winuser.h:4121`-`:4122` that a guard scan without a depth counter reports as a real guard; **U2** 3 of the library's 4 `CheckReturn` sites are here + its **inverse case** (`WaitForInputIdle` returns 0 for success, so `CheckNonZero` would invert it); **U19** 1 LAYER / 0 INDIRECT / 0 BYPASS (`ui/gui/view/handlers/messagehandler.cpp:246`). New folder measures: **coverage** 370 of 1,018 exports unwrapped, 114 SDK-declared (`tmp/_tmp_pt_coverage_s27.ps1`), and the **`returncheck.h` include census** 37 include / 2 use / **35 dead** -> registered as **T7**. Folder U14(a) **174 spaced / 254 tight**, census 428 bindings / **1** unresolvable, wrong-stem 3 rows / 0 live. Gate `tmp/_tmp_cl_all_s27.log` 38/38 `/W4` exit 0 after the 08:00:59 edit. Narrative `tu/processthread.md`. |
| 28 | `rectangle.cpp` | 11 | **0 binding, signature, return or idiom defects; spacing-only fixes** - `:20` a space before the argument list's `)`, `:69` a space before a comma (`int , int`, a shape new to the review), and the TU's 11 `Call<>` sites normalised to the folder's tight spelling by user decision, which **closes U14 (policy D15)** because this was the last reviewed TU with spaced sites | Whole `user32.h:446 //Rectangle functions` section (`:447`-`:457`): 11/11 decl/def pairs in header order, 0 U4/U11/U13/U17/U18, **0 hdr-vs-def and 0 def-vs-sdk parameter renames** (the section copies the SDK's names verbatim - the cleanest naming reviewed), returns all `BOOL` and all agree (`tmp/_tmp_rect_ret_s28.ps1`). Binding: **11 bare single exports, no A/W family and no `winuser.h` macro** (`5B`,`10B`,`20C`,`223`,`23E`,`29B`,`2B8`,`360`,`361`,`3A3`,`3C0`), so P2/P6 cannot arise here by construction and no live probe is needed - 11/11 `/EP` literals resolve (`tmp/_tmp_rect_class_s28.ps1`: 0 unresolved, 0 macro, 0 P6-shaped); folder census unchanged 428 bindings / 1 unresolvable, no wrong-stem row. `_tmp_sigcmp.ps1` 11/11 `decl==def ok` but **7 `CALL TYPE MISMATCH` rows carrying 11 mismatched arguments** - every one the **U25** spelling artifact (`CONST::RECT*` in the definition vs `CONST ::RECT*` in the template argument, and one list even mixes `CONST RECT*`), so arity/types were re-checked field-for-field against the SDK dump instead of trusting it. **U2's biggest increment**: the eleven `BOOL`s are three different kinds - answers (`EqualRect`, `IsRectEmpty`, `PtInRect`, where `BoolCheck` would report a legitimate FALSE as an error), documented-outcome zeros (`IntersectRect` writes an all-empty `RECT` **and** returns 0; `SubtractRect`/`UnionRect` likewise), and 5 success-or-failure calls whose FALSE is discarded silently by this layer *and* by `ui/gui/view/drawing/rect.cpp:53`/`:86`/`:93` one level up. **U16 0 of 11** - the first TU with **no** version-guarded prototype at all (the whole block sits in `#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)` at `winuser.h:9717`). **U19** 11 LAYER / 2 "INDIRECT" / 3 BYPASS, and both INDIRECT rows are a **new false-positive class**: same-named member *definitions*, not calls into the DLL (`EditController::SetRect` sends `EM_SETRECT` via `SendMessageT`; `Region::SetRect` calls `GDI32::SetRectRgn`). The 3 BYPASS sites are live in the built `winqor_gl` target (`ui/gl/CMakeLists.txt:5`): `ui/gl/windowimpl.cpp:516`, `:521` (unqualified `OffsetRect` beside `WindowController::AdjustRect*` calls) and `:1756` (`::SetRect`); a 4th grep hit at `:1470` is inside the `/* */` block `:1303`-`:1507` and the scan's comment filter excluded it correctly. New coverage findings: `SetSysColors` is declared at `user32.h:651` and **no TU defines it** (a U4 instance surfaced from this section), `GetWindowWord`/`SetWindowWord` have no member anywhere, and over the 51 export names containing `Rect` 41 are SDK-declared / 39 wrapped (`tmp/_tmp_rect_region_s28.ps1`, `tmp/_tmp_rect_family_s28.ps1` - the region scan's `GetWindowLongA/W` "no member" rows are an instrument artifact, the member is `GetWindowLongT`). | TU 93 -> **174** lines (81-line contract block); `/EP` 453,328 -> 453,409 with the **non-blank multiset identical at 154,338** (22 diff entries = the 11 intended call lines) and all 11 binding literals byte-identical (`tmp/_tmp_rect_neutral_s28.txt`). Folder U14(a) 174/254 -> **163 spaced / 265 tight**, U14 CLOSED with the tally frozen and the residual located (30 spaced in five unreviewed TUs, 133 in reviewed ones). Registers: **U2**, **U16**, **U19**, **U25** appended; **U14 closed**. Gate `tmp/_tmp_cl_all_s28b.log` **38/38 at `/W4`, 0 diagnostics**, log newer than the last edit; TU hygiene 0 TAB / 0 trailing space / 0 non-ASCII, longest line 124. Narrative `tu/rectangle.md`. |

## Session 27 close-out note (`processthread.cpp`, 3 Nov 2026)

The quietest session of the review so far: 5 wrappers, 0 binding defects, 0 signature defects, and every source change
was whitespace. That is worth recording as a result, not just as an absence - the TU's five stems are single exports with
no A/W family and no `winuser.h` macro, so the bare bindings it already had are load-bearing and the only question was
whether the layer had drifted anywhere near them (it had not: 0 P1/P2/P5/P6, 0 name divergences, 0 return divergences).
The user's decision on the one open judgement call - normalise all five `Call<>` sites to the folder's **tight** majority
rather than this TU's local spaced majority - is the first time U14(a) has been moved deliberately rather than
opportunistically, so folder U14(a) is now **174 spaced / 254 tight** and `processthread.cpp` 0/5.

Three findings that outlive the TU. (1) **U16's empty guard**: `winuser.h:4121`-`:4122` is an `#if (_WIN32_WINNT >=
0x0602)` with nothing between it and its `#endif`, and it sits between `AttachThreadInput` and `WaitForInputIdle` - a
backward guard scan without a depth counter attributes 0x0602 to `WaitForInputIdle`. Every future U16 pass needs the
depth counter (`_tmp_pt_guards_s27.ps1` has it; `_tmp_sdk_guard_scan.ps1` replays nesting properly). (2) **U2's inverse
case**: `WaitForInputIdle` returns `WAIT_OBJECT_0` (0) for success, so the `CheckNonZero` its neighbour uses would invert
it - a checked-failure site is only correct when the SDK's failure value is not also a success value. (3) **coverage was
measured, not assumed**: the contract block originally claimed "the only two user32 exports with no wrapper anywhere",
which was invented; `_tmp_pt_coverage_s27.ps1` says **370 of 1,018** exports have no `user32.h` member at all (114 of
them declared in `winuser.h`), and of the two names cited, one (`DdeSetUserHandle`) is in fact declared - at `:528`, as
part of U4's 34-member DDE block. The paragraph was rewritten against the measurement before the gate ran.

Two instrument fixes. `_tmp_sigcmp.ps1` matched `Library::Call<...>(pFunc, ...);` on **one** line, so any wrapped call
printed `CALL NOT PARSED` - 3 of this TU's 5 sites, and the same shape already recorded in `painting.cpp`,
`monitor.cpp` and `mouse.cpp`; it now joins lines to the terminating `;`, and all 5 sites here report arity + type `ok`.
New `_tmp_pt_cite_s27.ps1` resolves every `:NNN` in the contract block against the file it belongs to (bare citations
inherit the last file marker in the paragraph, and "(in this TU: ...)" marks a self-citation), asserts the 5 def + 5
binding self-citations land on the right construct, and printed 28 citations / 0 unresolved. That check caught the
`gui/CMakeLists.txt` -> `ui/gui/CMakeLists.txt` path error and, before it, three citations that had drifted by 6 lines
when the coverage paragraph grew - the standing rule that any edit changing a block's line count invalidates its own
citations still holds.


Record bookkeeping: rows **23-24** sliced out to `archive/session-log-23-24.md` (10 lines / 8,842 B, each row
sha256-matched against `tmp/_tmp_s27_preslice_sessionlog.md` before the rewrite; the live file was 37,515 B and could not
have taken row 27 + this note), `archive/tu/processthread.md` added (52 lines) and indexed, `census.md` row 27 appended,
`u2`/`u14`/`u16`/`u19` extended **at EOF only** so no locator pointer moved, `tooling.md` + the 10 `pt` instruments and
the two scanner fixes, `gotchas.md` + the coverage-claim and citation-context caveats. Instruments:
`_tmp_pt_{pre,ret,guards,class,lines,edit,cite,coverage,neutral}.ps1`, `_tmp_pt_pp_s27.bat`, `_tmp_s27_pre.ps1`,
`_tmp_s27_logslice.ps1`.

Two measures came out of the record pass rather than the TU. **`returncheck.h` include census** -
`_tmp_rcinclude_scan.ps1`, written to settle whether the include this TU carries is a layer convention: **37 of 38**
`user` sources include it, **2** use any name it declares, so **35 includes are dead**; the same scan over
`api_layer/kernel` is **22 includes / 20 TUs with hits**, so it is a `user` habit. Registered as **T7**
(`registers/tasks.md:121`) by user decision - mechanical, but the go-ahead was what was missing, not the analysis.
Instruments added beyond the plan: `_tmp_pt_coverage_s27.ps1`, `_tmp_rcinclude_scan.ps1`, `_tmp_pt_cite_s27.ps1` (the
citation resolver), `_tmp_rect_pre_s28.ps1` (the next TU's pre-flag sweep).
`_tmp_pt_neutral_s27.ps1` was caught comparing the non-blank multiset over the `_final_` capture against the binding
literals of the `_post_` capture - two different captures of the same TU; the result held here, and the script now uses
one capture for both halves.

**Record incident, recorded because it changed the file.** While tidying this note, an edit statement piped a variable
into `[IO.File]::WriteAllText` after a `[string]::Replace` overload-resolution failure had left that variable `$null`, and
the call wrote a **0-byte** `archive/session-log.md`. It was rebuilt the same session from
`tmp/_tmp_s27_preslice_sessionlog.md` (the snapshot the slice script took) by replaying `_tmp_s27_logslice.ps1`'s exact
transformation, re-inserting row 27 and re-applying this note - `tmp/_tmp_s27_logrestore.ps1` performs the replay and
prints the proof (rows 23-24 sha256-match the slice file; every snapshot line except the two rows, the title and the
placeholder paragraph is reproduced byte-identically). The caveat now in `gotchas.md`: `[string]::Replace(a, b)` binds to
the **char** overload when an argument is one character long, so the exception and the `$null` are easy to hit; never write
a record file from a variable produced by a statement that can throw - guard it (`if($null -eq $t) { throw }`) and prefer
the `editor` tool for record edits.

One reading error of my own, caught by the verifier rather than by eye: `_tmp_s27_verify.ps1`'s first pass reported **6
bindings in a 5-wrapper TU**, because it counted every line mentioning `qor_pp_useswinapi` - including the contract block
that quotes the macro in prose. The count is now taken over code lines only (122 lines = 55 code + 67 comment, 5 defs,
5 bindings, 0 tabs, 5 tight `Call<>`). Same lesson as the U14 census stripping comments before it counts: **any TU measure
must exclude the contract block it is standing inside**, and the verifier output
(`tmp/_tmp_s27_verify.txt`, 9 sections) is what the close-out numbers in row 27 come from.

---

**Session 28 close-out (`rectangle.cpp`, 3 Nov 2026).** Record bookkeeping: rows **25-26** sliced to
`archive/session-log-25-26.md` (10 lines / 5,534 B, each row sha256-matched against
`tmp/_tmp_s28_preslice_sessionlog.md` before the rewrite - the live file was 38,226 B and row 28 is ~4.5 KB), the
placeholder corrected `28..38` -> `29..38` (**9 TUs left**) and the title line now points at all three slices.
`tmp/_tmp_s28_logslice.ps1` prints the proof (rows located at lines 39-40, both matched, 33,452 B after).

Three process faults of my own, all caught before they reached a record, recorded because each one is reproducible:
**(1) a whitespace pass that names its target lines breaks the moment a contract block is inserted above them.** The
first `_tmp_rect_edit_s28.ps1` carried `$targets = @(20, 27, ...)`; after the block those same 11 call sites were at
`:101`-`:170`, so the script would have "fixed" 11 prose lines. It is now self-locating (every `Library::Call` line
from the first `BOOL User32::` on) and asserts that exactly 11 lines moved. **(2) order-dependent steps must not go in
one parallel batch** - a batch that held `git checkout`, that edit script and the contract-block splice ran them
concurrently, so the splice read the file the checkout had just restored. **(3) same class, records:** the register
index generator, the `status.md` locator splice and the locator verifier went in one batch, so the splice read the
*previous* session's index file. Run sequentially: `12:35` splice gave 59 lines (stale), the re-run gave 63 and the
verifier then reported **62/63 pointing right + the single deliberate `U27` exception**.

One instrument limitation found, not fixed: `_tmp_rect_region_s28.ps1` reports `GetWindowLongA`/`GetWindowLongW` as
"no member" because `user32.h` declares the family only as `GetWindowLongT` (`:625`). A coverage scan must resolve
`stem -> stemT` before calling a name unwrapped; `_tmp_rect_family_s28.ps1` does, `_tmp_pt_coverage_s27.ps1` partially
does (`gotchas.md`).
