# session log - close-out notes for sessions 24, 25, 29 and 30 (plus the 27-28 moved-out pointer)

Moved verbatim out of `archive/session-log.md` on 4 Nov 2026 when that file passed the 40 KB ceiling (44,228 bytes).
Source range: lines 216-344 inclusive. Moved-block SHA256 =
`09136B67A3331B040BA9A3CA6E01E4808FFCC9C073047C123A49AA549A064852` (computed before the move; the block below is that
block verbatim). Rejoin verified by hash at slice time.

## Session 24 close-out note (`monitor.cpp`, 2 Nov 2026)

Zero-fix session, so the close-out work was all record hygiene and hand-off. Measured again after the edits: folder
binding census **11 of 428** unchanged (`tmp/_tmp_p1census_pre_s24.txt` == `_post_s24.txt` - a comment-only edit must move
nothing, and re-running the census is how that is shown rather than asserted); wrong-stem scan still 4 rows; T-suffix
census still 525/108/404/11/2; `user32.h` **695 lines**, untouched. TU hygiene after the 98-line insert: 0 tabs, 0
trailing-space lines, 0 non-ASCII bytes, longest line 124, and the three in-file self-citations (`:114`/`:142` definition
range, `:124` the `AW`, `:132` the by-value `POINT`) re-pointed to post-insert line numbers - the insert moved the
definitions, which is the one class of stale citation a comment-only edit can create. `archive/tu/monitor.md` added (50
lines) and indexed; `archive/session-log.md` 193 -> 195 lines / 34,847 B with row 24 and the placeholder corrected
`24..38` -> `25..38` (13 left); registers appended at **EOF only** so no locator line number moved (`u16.md` 186 -> 202,
`u19.md` 177 -> 197, `u21.md` 83 -> 106, `u25.md` 40 -> 54, `u26.md` 98 -> 128, `census.md` 105 -> 125 - all under the
40 KB ceiling, none re-sliced); `status.md` rewritten at **180 lines / under its 18 KB ceiling** with session 25's
`mouse.cpp` pre-flags measured fresh (all 10 stems are single exports - `CB`, `125`, `152`, `18E`, `3F5`, `2FE`, `320`,
`339`, `3A4`, `3B3` - and `qor_pp_useswinapiAW` pastes `##W` (`kernel32.h:52`), so all 10 ask for a `*W` name that does
not exist: session 25 is a whole-TU binding fix, not a review). New instruments: `_tmp_mon_pp_s24.bat`,
`_tmp_mon_class_s24.ps1`, `_tmp_mon_mirror_s24.{cpp,bat}`, `_tmp_mon_probe_s24.cpp`, `_tmp_run_mon_probe_s24.bat`
(`_tmp_mon_pre_s24.ps1`, `_tmp_mon_pre2_s24.ps1`, `_tmp_mon_guards_s24.ps1`, `_tmp_mon_bypass_s24.ps1` dated from the
pre-flag stage). Two caveats learned the hard way and written into `method.md`: (1) an inserted block must not re-open
the namespace line - the first attempt duplicated `namespace qor { namespace win { namespace api {`, which the standalone
compile caught as `C2888` + `C1075` before anything else ran; (2) `run_commands` executes its array **concurrently**, so
chaining a producer and its consumer (`cl /EP` then the classifier that reads its output) in one call races and fails -
chain dependent steps across calls or in one shell command.

## Session 25 close-out note (`mouse.cpp`, 2 Nov 2026)

The session the pre-scan predicted: `mouse.cpp` was already known to hold 10 broken bindings, so the work was to prove
the diagnosis three ways before touching anything (`/EP` literals, the export table, a live `GetProcAddress` probe) and
then fix 11 lines. Measured after the edits: folder census **11 -> 1 unresolvable of 428**, wrong-stem scan **4 -> 3 rows
and 0 live**, U14(b) **424/4 -> 425/3**, U17's T-suffix census **unchanged** (525/108/404/11+2 - all 10 mouse members are
in the 404 single-export class, which is the evidence that `AW` was a defect, not a style choice), name-divergence scan
**0/0 over 10 definitions**, `user32.h` **695 lines**, untouched. `cl /EP` neutrality: pre-fix 453,321 lines, post3
453,402 - **0 removed, 81 added, every added line blank**; and post2 (22:06:35) vs post3 (22:18:29) = **0/0**, which is how the
last comment-wording tweak was shown to be code-neutral. Gate `tmp/_tmp_cl_all_s25c.log` (22:18:27, 3 min after the last
edit at 22:14:54) - the 22:06 `_s25b.log` predates that edit and is **not** the gate of record, the same trap as session
24's `_s24.log`. Verifier `tmp/_tmp_s25_verify.ps1` -> `tmp/_tmp_s25_verify.txt` re-checks gate order, both `/EP` diffs,
the 10 literals, TU hygiene and the record ceilings in one run.

Registers were appended **at EOF only**, so no locator pointer moved: `u16.md` 202 -> 218, `u19.md` 197 -> 208,
`u14.md` 65 -> 73, `prescan.md` 106 -> 123 (P1 11 -> 1, P5 0 live), `census.md` 131 -> 151 - all still far under the
40 KB ceiling. `tasks.md` is the one in-place edit (the stale queue paragraph at `:74`-`:83`, 14 TUs -> 12), kept at
exactly 10 lines so `T5` at `:87` and `T6` at `:102` still resolve. `archive/tu/mouse.md` added (78 lines) and indexed.
`archive/session-log.md` passed the ceiling (37,988 B) before this row could go in, so rows **20-22** (`keyboard`, `mdi`,
`menu`) moved verbatim to `archive/session-log-20-22.md` (11 lines / 10,350 B), each row **sha256-verified** against the
snapshot `tmp/_tmp_s25_preslice_sessionlog.md` (`tmp/_tmp_s25_logslice.ps1` prints 3 `MATCH` + `0` unmatched original
lines); the placeholder was corrected `25..38` -> `26..38` (12 left) and the file's title line points at the slice. After
row 25 and this note it is **259 lines / ~35.5 KB** - 4.5 KB under the ceiling. Session 26 measured that headroom first:
row 26 plus this note fit without slicing (**264 lines / 37,515 B**, ~2.4 KB spare), so the predicted second slice was
not needed - rows **23-24** stay the natural candidates and **session 27 must slice before its row goes in**. Session 26's
record bookkeeping: `archive/tu/painting.md` (41 lines) added and indexed, `archive/README.md` rows for both, `tooling.md`
+the 6 `paint` instruments and the `-replace`/`-creplace` caveat, `gotchas.md` +the same caveat, `census.md` row 26,
`u16`/`u19`/`u26` extended.

Three instrument lessons, all now in `method.md`: (1) a PowerShell function called `H` is **not** usable - `H` is the
built-in `Get-History` alias, and the argument binder then tries to parse the row text as an `-Id` integer; the slice
script aborted before writing the source, which is why it must verify before it rewrites; (2) `run_commands` still
executes its array concurrently, so a `Get-ChildItem` of a file another element is rewriting reported the **pre-write**
size - re-measure in a later call, never in the same one; (3) any edit that changes a comment block's **line count**
invalidates every in-file self-citation in that block (`:97`/`:160`/`:100`-`:163`/`:164`/`:125` here), so citations are
re-pointed after the block settles and then re-checked mechanically - the first cleanup pass moved them by 2 lines and
the citation fixer caught it. `_tmp_s25_verify.ps1` counts the comment citations (27 here; the 4 it flags as
out-of-range are `user32.h:555`/`:556`/`:565` and `winuser.h` citations, i.e. pointers into another file, not stale ones
- it does not yet distinguish scope, a known limitation of that check).




## Session 27-28 close-out notes moved out

The close-out notes for sessions 27 and 28 are the tail of `archive/session-log-27-28.md`.

## Session 29 close-out note (`resource.cpp`, 3 Nov 2026)

Row 29 above carries the verdict; the narrative is `archive/tu/resource.md` (101 lines). Record bookkeeping: rows **27-28**
and their close-out notes went to `archive/session-log-27-28.md` (15,231 B, 103 lines) via `tmp/_tmp_s29_logslice.ps1`
**before** row 29 was added - the live file was 39,382 B against the ~40 KB ceiling. Both rows and all 92 note lines were
sha256-matched against `tmp/_tmp_s29_preslice_sessionlog.md` before the rewrite.
`tooling.md` crossed 40 KB during this session, so it was sliced too: was `:65-126` (sessions 13-16, sessions 17-20, the
U6 off-protocol fix) -> `archive/tooling-13-20.md` (66 lines, 6,428 B) via `tmp/_tmp_tooling_slice_s29.ps1`, rejoin
hash-verified against `tmp/_tmp_tooling_preslice_s29.md`, and the two "as ... above" references the slice orphaned were
re-pointed at the archive file. `tooling.md` is **the next file to need slicing** - slice it at the *start* of session 30,
before that session's instruments are added. Sizes at close: `tmp/_tmp_s29_sizes.txt` (from `tmp/_tmp_s29_sizes.ps1`);
this note deliberately does not state this file's own size, since writing it changes it.

The session closed with a **record-integrity pass**: `tmp/_tmp_s29_cite_audit.ps1` harvests every `tmp/` citation from the
live records and checks it against the filesystem - **194 cited artefacts, 1 missing**, and that one was a five-session-old
typo in `registers/census.md:132` (the binding census instrument is `_tmp_kb_p1census_s20.ps1`, not
`_tmp_p1census_s20.ps1`), now corrected. Re-run: **0 missing**. Run it at every session close.

Two things that outlive the TU. (1) **A missing `User32::` qualifier is not cosmetic**: it leaves the declared member
undefined *and* the emitted function unexported, so the pair is invisible to the linker in both directions at once. Any
future U4 / T1-T6 fix should measure both directions rather than assume them - the instrument is the consumer-side link
probe (`tmp/_tmp_res_ref_s29.cpp`, described in `tooling.md`), and the counting trap is the `LNK2019` double-quote
problem in `gotchas.md`. (2) **A clean 38-TU gate log cannot be dated by its hash** - the 3 Nov runs for sessions 28 and
29 are byte-identical (`7EB28546A9062CBCC...`) because a passing log contains only the 38 TU echoes; date such a run by
`tmp/obj/<tu>.obj` being newer than the source, as `tmp/_tmp_res_finalchk_s29.txt` prints.

Pre-flags for the next TU (`scrollbar.cpp`, 102 lines, `//ScrollBar functions` at `user32.h:573`, 12 wrappers) are in
`tmp/_tmp_scroll_pre_s30.txt` - all 12 stems are bare single exports, so that TU is expected to be P1/P2/P6-free; its one
U16 candidate is `GetScrollBarInfo` (`winuser.h:14763`, `#if(WINVER >= 0x0500)`).

## Session 30 (`scrollbar.cpp`, 3 Nov 2026)

12 wrappers, `user32.h:573`-`:585`, 102 -> **156 lines** (contract block only). **One defect class, fixed:** all 12
bindings spelled the binding macro's module token `User32` - `qor_pp_useswinapi(User32, ...)`, the only such site left in
the folder (12 of 428 bindings; `combobox.cpp`, the co-offender named in G4, no longer spells it that way, so G4's
population was stale too). `kernel32.h:50` stringizes the token, so `/EP` emitted `GetModuleHandleW(L"User32")` against
the folder's `L"user32"`. Normalized, and proved behaviour-neutral: `/EP` line count unchanged with **12 differing lines,
all of them the module literal**; a **x64 + x86** probe showing `user32`/`User32`/`USER32`/`user32.dll` resolve to one
`HMODULE`, a bogus name to NULL, and **0 of 12 entry points** differing by module spelling; and code-only `/EP` output
**SHA-256 identical** across the comment-block insertion (154,346 code lines both sides). Folder census after the fix:
**428 bindings, 428 `user32` tokens, 0 non-lowercase sites.** Signatures clean 12/12 (`_tmp_sigcmp.ps1`), its one
`CALL TYPE MISMATCH` being the new `const ::RECT *p` vs `const ::RECT*` **pointer-spacing** artifact (gotchas); 0
header-vs-definition name divergence, 4 def-vs-SDK name divergences over 3 members (U18, published names). U16: 12/12 in
a DESKTOP partition block, only 2 version-guarded - `GetScrollInfo`/`SetScrollInfo` under `#if(WINVER >= 0x0400)`
(`winuser.h:12026`), the item's **first 0x0400 gate**, plus its first `NOSCROLL` and `NOWINABLE` guard names. U21: 13
"Scroll" exports, 12 wrapped; the 13th `ScrollChildren` is exported but **undeclared in the SDK**, so not wrappable.
U19: 8 LAYER callers (all one class, `ScrollBarController`, in `winqor_gui`), 0 BYPASS, 0 MEMBER, and its 8 "INDIRECT"
rows are the controller's own same-named member definitions - session 28's false-positive class again; nothing in-tree
constructs `ScrollBar`, so the chain is latent. U26: the caller mirror `ScrollInfo` is **layout-identical to
`SCROLLINFO` on both arches** (28 bytes, all 7 offsets), but **`nPos`/`nTrackPos` alone lack a default initializer**, so a
default-constructed struct carries an indeterminate `int` through `GetScrollInfo`/`SetScrollInfo` - the second one reads
both. U2: 0 checks, 0 `_Check_return_` on the prototypes, and all 4 `int`-returning members have 0 as a legal value. T7
**35 -> 36** (`scrollbar.cpp:11`). Gate: 38/38 TUs, exit 0, `scrollbar.obj` newer than the source (a clean 38-TU log
cannot be dated by its hash). Narrative `archive/tu/scrollbar.md`.
**Pre-flags for the next TU (`systemshutdown.cpp`, 51 lines / 1,734 B, `user32.h:601`-`:606`, 5 members
`ExitWindowsEx`, `LockWorkStation`, `ShutdownBlockReasonCreate/Destroy/Query`): the section label itself is misspelled -
`//System Shutdwown functions` at `user32.h:601`.**

