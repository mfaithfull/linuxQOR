# U18 coverage - the export-vs-wrapper backlog, dispositioned (5 Nov 2026)

Scope is a user decision of 5 Nov 2026, recorded with D18 in `docs/agent/DECISIONS.md`: the "full set" in
`../purpose.md` is the **documented set** - the exports that `winuser.h` declares - **minus superseded families**, named
as excluded: the **DDE family** and the **`keybd_event` / `mouse_event`** pair. Everything else the header declares is in
scope; the exports with no prototype are not.

Measured with `_tmp_u18_coverage4.ps1` (member decls, export names, A/W + `T` normalisation) and `_tmp_u18_buckets.ps1`
(family split), against the `user32.dll` export table captured by pre-scan P4 and `winuser.h` 10.0.26100.0.
**1,018** export names / **525** members in `user32.h` / **428** bindings / **648** exports matched / **117** classified
as declared-and-unwrapped. That 117 reduces as follows, and the reduced list is the work:

| Bucket | Names | Disposition |
|---|--:|---|
| Pointer (`GetPointer*`, `Register*PointerInputTarget*`, synthetic-pointer, mouse-in-pointer) | 32 | **WRAPPED 6 Nov 2026** - T8 step 6a, new TU `pointer.cpp` |
| DPI / awareness (`*Dpi*`, `*Awareness*`, `*Hosting*`, `EnableNonClientDpiScaling`) | 21 | **WRAPPED 6 Nov 2026** - T8 step 6b, new TU `dpi.cpp` |
| Touch / gesture (`*Touch*`, `*Gesture*`) | 14 | **WRAPPED 6 Nov 2026** - T8 step 6c, new TU `touch.cpp` |
| Display config (`*DisplayConfig*`, `*DisplayAutoRotation*`) | 7 | **WRAPPED 6 Nov 2026** - T8 step 6d, new TU `displayconfig.cpp` |
| Shutdown / power (`CancelShutdown`, `*SuspendResumeNotification`) | 3 | **In scope** - tranche 6e |
| Misc (menu, clipboard, affinity, feedback, immersive, tooltip, timers, undocumented-but-declared) | 26 | **In scope** - tranche 6f; 2 of these are T8 step 5, below |
| Legacy / utility with no wrapped replacement (`GetWindowWord`, `SetWindowWord`, `SetMessageQueue`, `DragObject`, `wsprintfA/W`) | 6 | **WRAPPED 7 Nov 2026** - T8 step 6g, new TU `legacy.cpp` + the header-inline `wsprintfT` member (D20) |
| Superseded twins whose replacement the library **already wraps** (`GetClassLongA/W`, `SetClassLongA/W`, `SetWindowsHookA/W`, `UnhookWindowsHook`) | 7 | **Excluded** by the scope rule |
| Instrument artifact (`SetWindowsHookExAW`) | 1 | **Removed** |
| **Backlog to add** | **109** | 32+21+14+7+3+26+6 |
| **— re-measured 6 Nov 2026, after steps 5 and 6a** | **75** | the Pointer row (32) and the step-5 pair (2) are gone: 21+14+7+3+24+6 |
| **— re-measured 6 Nov 2026, after 6b** | **54** | the DPI / awareness row is empty too: 14+7+3+24+6 |
| **— re-measured 6 Nov 2026, after 6c** | **40** | the Touch / gesture row is empty too: 7+3+24+6 (raw gap 62 -> 48) |
| **— re-measured 6 Nov 2026, after 6d** | **33** | the Display config row is empty too: 3+24+6 (raw gap 48 -> 41) |
| **— re-measured 7 Nov 2026, after 6e / 6f + D19** | **6** | Misc and Shutdown / power empty too; the 6 held-open legacy names are the whole net backlog - tranche **6g** (D19: decided in scope, to wrap) |
| **— re-measured 7 Nov 2026, after 6g** | **0** | raw gap 8, every one excluded: 7 superseded twins + the `SetWindowsHookExAW` artifact. Coverage 751 -> **757 of 1,018**; the buckets instrument prints 0 for `Other` and for all six family rows, leaving only `4: Legacy hook family` and `4: Legacy word / class-long family`, both all-exclusions |

**Excluded - superseded twins (measured, not assumed).** `GetClassLongA`/`GetClassLongW` and `SetClassLongA`/`SetClassLongW`
are exported (12C/12F, 323/326) but their `*Ptr` replacements are exports too (12D/12E, 324/325) and are wrapped at
`windowclass.cpp:76` and `:118`. `SetWindowsHookA`/`SetWindowsHookW` (389/38D) and `UnhookWindowsHook` (3BE) are the
pre-Ex forms of `SetWindowsHookExT` / `UnhookWindowsHookEx`, wrapped at `hook.cpp:93` and `:100` - `hook.cpp:68-69`
already recorded these three as "exported and named nowhere in this library", which under this rule is now correct rather
than a gap. The DDE family and `keybd_event`/`mouse_event` are **already fully wrapped** (28 DDE members against 33 DDE
exports, every A/W pair behind a `T` member), so the exclusions the user named remove **nothing** from the backlog: they
protect existing wrappers from deletion and keep the 253 undocumented exports out, which is where their effect is.

**Decided - 6 names, in scope (D19, 7 Nov 2026: the user decided to wrap all 6).** `GetWindowWord`, `SetWindowWord` (the 16-bit word twins of the wrapped
`Get/SetWindowLong*`), `SetMessageQueue`, `DragObject`, `wsprintfA`, `wsprintfW`. These are legacy or utility but have no
wrapped replacement, so the "superseded twin" test does not decide them - the **user decision** does, and it lands them **in**:
the target stays **109** (the "excluding them takes it to 103" branch is **not** taken), and they are the last tranche, **6g**.
U18 closes only once 6g wraps them; the decision touches no source, header or CMake file. The measured shape 6g must solve
is in the "The 6 measured again for the decision" block at the foot of this file.

**Artifact removed.** `SetWindowsHookExAW` (38B) is a real export that **no header declares** (`hook.cpp:31`); it entered
the "declared" bucket only because the classifier credits an export whose name ends `A`/`W` with its base stem's
declaredness (`SetWindowsHookExAW` -> `SetWindowsHookExA`, which is declared). Not wrapper work.

**A/W still open (U17).** Exactly two A/W pairs remain in scope after the exclusions - `ChangeMenuA/W` and
`wsprintfA/W` - so U17's suffix question moves the count by one wrapper, not by dozens. 109 is a **name** count; under the
`W` + `T`-alias convention it is ~107 new members.

**Cross-reference to T8 step 5.** `LogicalToPhysicalPointForPerMonitorDPI` and `PhysicalToLogicalPointForPerMonitorDPI`
sit in the Misc row and are scheduled ahead of the backlog (T8 step 5) because U18 was opened over them; they are part of
the 109, not additional to it. **Done 6 Nov 2026** - declared at `user32.h:189`, `:193`, defined `window.cpp:513`, `:543`.

## Tranche 6a closed - the Pointer family, 6 Nov 2026

All 32 names in the Pointer row are now wrapped: new TU `pointer.cpp`, 32 definitions, 32 declarations in a new
`//Pointer input functions` section at `user32.h:569-601`. The bucket count reconciles name-for-name against the export
table: **41** `*Pointer*` names there, **9 with no `winuser.h` prototype** (`GetExtendedPointerDeviceProperty`,
`GetPointerDeviceInputSpace`, `GetPointerDeviceOrientation`, `GetPointerFrameArrivalTimes`, `GetPointerFrameTimes`,
`InitializePointerDeviceInjection`, `InitializePointerDeviceInjectionEx`, `InjectPointerInput`,
`SetPointerDeviceInputSpace`), and 41 - 9 = 32. The 9 stay out by the scope rule, not by choice.

**New limit on `_tmp_u18_coverage4.ps1`, found here.** The instrument walks the **export** table, so a name the header
declares but the DLL does not export never appears as a gap. `EnableMouseInPointerForThread` is exactly that: declared at
`winuser.h:6715` (inside `#if WDK_NTDDI_VERSION >= NTDDI_WIN10_RS3`) but absent from the local `user32.dll` capture - only
ordinal `EA` `EnableMouseInPointer` is there. Wrapping it would install a permanently null `GetProcAddress` binding (the
U5 class). Before wrapping anything **not** on the gap list, check the export table; the gap list cannot flag this.

Folder figures re-measured the same day (`_tmp_u18_coverage4.ps1`): **559 members** / 1,018 export names / **682 covered**
/ **83** declared-and-unwrapped / 253 no-prototype. 525 -> 559 is step 5 (+2) plus 6a (+32); 648 -> 682 the same +34.
`_tmp_u18_buckets.ps1` now prints the Pointer row empty (`0: Pointer family []`) and 83 raw, which is the 75 net once the
7 excluded twins and the 1 artifact are removed. Binding sites over the 39 sources re-count at **462** (`_tmp_wrong_stem_scan.ps1`:
462 definitions, 462 bindings, 1-to-1) - 428 at the session-33 census, plus step 5's 2, plus 6a's 32.

## Tranche 6b closed - the DPI / awareness family, 6 Nov 2026

All 21 names in the DPI row are now wrapped: new TU `dpi.cpp`, 21 definitions, 21 declarations in a new
`//DPI awareness functions` section at `user32.h:603-627`. **Nothing shed**: unlike 6a (41 exported `*Pointer*` names minus
9 prototype-less), every one of the 21 is **both** in the export capture and declared in `winuser.h` 10.0.26100.0, and none
is a function-like macro or has an `A`/`W` sibling (`_tmp_t8_6b_pre.ps1`: 21 exported with ordinals, 0 macros, 0 A/W
siblings, 0 already declared). 21 in, 21 wrapped, 0 excluded - the cleanest tranche so far.

Guard stacks re-measured per name (`_tmp_t8_6b_guards.ps1`, forward pass over the `#if` nesting): **13** names innermost
`#if(WINVER >= 0x0605)` (10 inside the block at `:14458`, `SetProcessDpiAwarenessContext` under its own `:14535`,
`GetSystemMetricsForDpi` at `:7414`, `SystemParametersInfoForDpi` at `:13275`), **3** innermost `#if(WINVER >= 0x0606)`
(the hosting trio `:14560`, `:14566`, `:14571`), **4** with no `WINVER` test at all (the dialog quartet, behind `NOCTLMGR`
`:4912` + `WINAPI_PARTITION_DESKTOP` `:5006`), and **1** behind `NTDDI_VERSION` - `GetDpiAwarenessContextForProcess`
`:14550`, under `#if (NTDDI_VERSION >= NTDDI_WIN10_19H1)` at `:14545`. That last one is the only backlog name in the whole
U18 set whose floor is `NTDDI_VERSION` rather than `WINVER`; live under the `sdkddkver.h` default, and the layer adds no
`#if`, so the declaration and the wrapper stay live together.

Liveness and signatures were **compiled**, not eyeballed: `tmp/_tmp_t8_6b_protos.cpp` declares 21 function-pointer typedefs
equal to the 21 published signatures and initialises each with `&<SDK name>`; at `/W4` with no QOR define and no version
macro it compiles clean (exit 0). A compiled-out declaration would be undeclared, and a wrong signature would be C2440 -
so one run settles both questions. Worth reusing for every later tranche.

Four pre-flag figures corrected by measurement, all in `preflags.md`'s 6b section: the dialog flag enums were cited at
`:5332` / `:5355`, which are the **closing** lines - the typedefs open at `:5328` / `:5350`; `DPI_AWARENESS` was cited at
`windef.h:259`, which is its first enumerator (the `typedef enum` is `:258`), and that `windef.h` is
`Include\10.0.26100.0\shared\windef.h`, not the non-existent `um\windef.h`; and the preflags said the two `ForDpi` twins'
stems "are wrapped", which is wrong - `GetSystemMetrics` (`user32.h:712`) and `SystemParametersInfo` (`:714`) are
**declared and defined by no TU** (that is U4), so the "place the twins beside their stems" reading of U12 did not apply
and all 21 went into one new section after `//Pointer input functions`, the way 6a did.

Folder figures re-measured the same day (`_tmp_u18_coverage4.ps1` + `_tmp_u18_buckets.ps1`): **580 members** / 1,018 export
names / **703 covered** / **62** declared-and-unwrapped / 253 no-prototype; 559 -> 580 and 682 -> 703 are both exactly +21.
The buckets instrument prints `0: Pointer family []` **and** `0: DPI / awareness family []`, so the family is drained with
nothing left unaccounted. 62 raw = 54 net backlog (Touch/gesture 14, Display config 7, Shutdown/power 3, Misc 24, held-open
legacy 6) + 7 excluded superseded twins + 1 instrument artifact. Binding sites over the 40 sources re-count at **483**
(`_tmp_wrong_stem_scan.ps1`: 40 files, 483 `User32::` definitions, 483 bindings, 1-to-1, and the 2 mismatches it still
reports are the pre-existing `CopyCursorT -> CopyIcon` and `CreateWindowT -> CreateWindowEx` - `dpi.cpp` contributes 0).

**U19 for this family: 0 LAYER, 1 live BYPASS, 1 dead.** `ui/gl/windowimpl.cpp:516` calls `GetSystemMetricsForDpi` through
the SDK import while the line above routes through the layer (`WindowController::GetDPI` -> `GetDpiForWindow`);
`:550` `EnableNonClientDpiScaling` is inside the block comment that opens `:530` and closes `:1301`, so it is dead text.
The same file's four `GetDpiForWindow` sites (`:361`, `:1130`, `:1212`, `:1462`) are **all** inside block comments
(`325-408`, `530-1301`, `530-1301`, `1303-1507`), so the already-wrapped name has no live bypass either. `winqor_gl` is not
in the build (`ui/CMakeLists.txt:4`), which is why none of this links. U19 is FROZEN at session 34, so the split is booked
in `dpi.cpp`'s banner, not appended to the register.

## Tranche 6c closed - the Touch / gesture family, 6 Nov 2026

All 14 names in the Touch / gesture row are now wrapped: new TU `touch.cpp`, 14 definitions, 14 declarations in a new
`//Touch and gesture functions` section at `user32.h:629-646`. **Nothing shed**: the bucket filter (`Touch|Gesture`) over
the gap list matches 19 export names, of which **4** were already wrapped by 6a in `pointer.cpp` (`GetPointerTouchInfo`
`winuser.h:6574`, `GetPointerTouchInfoHistory` `:6581`, `GetPointerFrameTouchInfo` `:6589`,
`GetPointerFrameTouchInfoHistory` `:6597` - they are `POINTER_TOUCH_INFO` accessors, so a name-based filter sees them
twice), **1** is exported with no prototype in `winuser.h` at all (`MITSynthesizeTouchInput`, 0 hits - the
prototype-less class this file already flags over `EnableMouseInPointerForThread`, out under the D18 rule), and the
remaining **14** are wrapped. `_tmp_t8_6c_pre.ps1`: 14 exported with ordinals (51, 52, 15C, 15D, 15E, 1D4, 214, 21B, 245,
2A5, 2F9, 2FA, 33F, 3CE), 0 function-like macros, 0 `A`/`W` siblings, 0 already declared - 14 in, 14 wrapped.

Guard stacks re-measured per name (same forward-pass instrument): **5** innermost `#if(WINVER >= 0x0601)` inside
`#ifndef NOUSER` (`GetTouchInputInfo:6243`, `CloseTouchInputHandle:6252`, `RegisterTouchWindow:6272`,
`UnregisterTouchWindow:6279`, `IsTouchWindow:6285`), **4** innermost `#if(WINVER >= 0x0602)` inside `NOUSER`
(`InitializeTouchInjection:6485`, `InjectTouchInput:6492`, `RegisterTouchHitTestingWindow:6726`,
`PackTouchHitTestingProximityEvaluation:6773`), and **5** gesture names (`GetGestureInfo:15770`,
`GetGestureExtraArgs:15783`, `CloseGestureInfoHandle:15801`, `SetGestureConfig:15869`, `GetGestureConfig:15884`) with
`#if(WINVER >= 0x0601)` and **no `NOUSER` test at all** - the gesture half of the family sits outside the `NOUSER`
region that the touch half sits in. All 14 are additionally inside
`#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)`. `WINVER` defaults to `0x0A00`, so all 14 are live and the layer
adds no `#if`; a caller that pins `WINVER` below `0x0602` loses the injection / hit-testing quartet from the SDK while
`user32.h` still declares it. The `_tmp_t8_6c_protos.cpp` run (14 function-pointer typedefs initialised with
`&<SDK name>`, `/W4`, no QOR define, no version macro, exit 0) settles liveness and signature conformance at once.


Two pre-flag prototype claims were wrong and the published text was copied from `winuser.h` itself, not from the digest:
the pre-scan **truncates** `GetTouchInputInfo`, `SetGestureConfig` and `GetGestureConfig` - those three prototypes are
multi-line and their inline parameter comments contain `;`, which is where the digest's end-of-prototype scan stops - and
the pre-flags gave `GetGestureConfig`'s count parameter as `_Inout_ PUINT`, whereas the SDK has `_In_ PUINT pcIDs`
(`:15888`) and puts the in/out on the buffer (`_Inout_updates_(*pcIDs) PGESTURECONFIG`, `:15890`). An instrument that
ends a prototype at the first `;` is unsafe for any `winuser.h` name with commented parameters; read the declaration
block in the header whenever the digest's `proto=` line ends mid-list.

Folder figures re-measured the same day (`_tmp_u18_coverage4.ps1` + `_tmp_u18_buckets.ps1`): **594 members** / 1,018
export names / **717 covered** / **48** declared-and-unwrapped / 253 no-prototype; 580 -> 594 and 703 -> 717 are both
exactly +14. The buckets instrument prints `0: Touch / gesture family []` alongside the already-empty Pointer and DPI
rows, so three families are drained. 48 raw = **40 net backlog** (Display config 7, Shutdown/power 3, Misc 24, held-open
legacy 6) + 7 excluded superseded twins + 1 instrument artifact. Binding sites over the 41 sources re-count at **497**
(`_tmp_wrong_stem_scan.ps1`: 41 files, 497 `User32::` definitions, 497 bindings, 1-to-1; the 2 mismatches it still
reports are the pre-existing `CopyCursorT -> CopyIcon` and `CreateWindowT -> CreateWindowEx`, and `touch.cpp` adds 0).

**U19 for this family: 0 LAYER, 0 BYPASS** - 0 raw hits for all 14 names over the 3,228 `.h`/`.hpp`/`.cpp`/`.cxx` of
`src`, `examples` and `test` (`_tmp_t8_6c_consumers.ps1`), so unlike 6b there is no split to book. U19 stays FROZEN at
session 34.


## Tranche 6d closed - the Display configuration family, 6 Nov 2026

All 7 names in the Display config row are now wrapped: new TU `displayconfig.cpp`, 7 definitions, 7 declarations in a new
`//Display configuration functions` section at `user32.h:648-662`. **Nothing shed**: all 7 are exported with ordinals
(`BE`, `BF`, `14B`, `14C`, `2BA`, `334`, `335`), all 7 are declared in an SDK header, and `_tmp_t8_6d_pre.ps1` measures 0
function-like macros, 0 `A`/`W` siblings and 0 already declared - 7 in, 7 wrapped. It is the first tranche whose bucket
filter needs no judgement call: unlike 6a/6c, no second family re-matches a name in the gap list.

Guard stacks (`_tmp_t8_6d_pre.ps1`, forward pass): **5** innermost `#if(WINVER >= 0x0601)` inside
`#ifdef _WINGDI_ ;; #ifndef NOGDI` (the WINVER block runs `winuser.h:13198`-`:13243`, inside the enclosing NOGDI /
`_WINGDI_` pair that closes at `:13246` / `:13247`), **2** innermost `#if(WINVER >= 0x0602)`
with no `_WINGDI_` test (block closes `:16083`); all 7 additionally inside the desktop partition. `WINVER` defaults to
`0x0A00`, so all 7 are live and the layer adds no `#if`. `_tmp_t8_6d_protos.cpp` (7 function-pointer typedefs initialised
with `&<SDK name>`, `/W4`, no QOR define, no version macro, exit 0) settles liveness and signature conformance at once.

**Two pre-flag claims were wrong, and one of them is a new instrument limit.** The 6c-close pre-flags stated every
`DISPLAYCONFIG_*` type is declared in `winuser.h` itself, citing `:13213`, `:13215`, `:13229` - those lines are the
**parameter use sites** inside `SetDisplayConfig` and `QueryDisplayConfig`, which is exactly what a first-hit name search
finds. A definition scan over all 1,798 `um` + `shared` headers of SDK 10.0.26100.0 (`_tmp_t8_6d_types.ps1`) puts the types
in **`wingdi.h`**: `DISPLAYCONFIG_MODE_INFO` `:2931`, `PATH_SOURCE_INFO` `:2950`, `PATH_TARGET_INFO` `:2973`,
`PATH_INFO` `:3005`, `TOPOLOGY_ID` `:3022`, `DEVICE_INFO_TYPE` `:3053`, `DEVICE_INFO_HEADER` `:3062`. The same file holds
the `QDC_*` (`:3330`-`:3335`) and `SDC_*` (`:3341`-`:3360`) groups the `flags` parameters take, so those two parameter
lists cannot be read from `winuser.h` alone - the pre-flags quoted `SDC_SAVE`, which does not exist: the name is
`SDC_SAVE_TO_DATABASE` (`:3352`). And **`windisp.h` does not exist in this SDK** at all (absent from the tree, 0 `um`
headers include it) although the online documentation names it as this family's header. The guard consequence: `NOGDI`
removes the types and the five prototypes together, while `user32.h` declares them unconditionally, so a consumer that
defines `NOGDI` would not compile this header - 0 in-tree sources define `NOGDI` or `NOUSER` (measured over `src`,
`cmake`, `examples`, `test`). The published signatures were taken from the `winuser.h` declaration block, not from the
digest, for the reason 6c recorded: the `proto=` line truncates on the first `;`, and these parameter lists carry inline
SAL comments.

Folder figures re-measured the same day (`_tmp_u18_coverage4.ps1` + `_tmp_u18_buckets.ps1`): **601 members** / 1,018
export names / **724 covered** / **41** declared-and-unwrapped / 253 no-prototype; 594 -> 601 and 717 -> 724 are both
exactly +7. The buckets instrument prints `0: Display config family []` alongside the three already-empty rows, so four
families are drained. 41 raw = **33 net backlog** (Shutdown / power 3, Misc 24, held-open legacy 6) + 7 excluded
superseded twins + 1 instrument artifact. Binding sites over the 42 sources re-count at **504**
(`_tmp_wrong_stem_scan.ps1`: 42 files, 504 `User32::` definitions, 504 bindings, 1-to-1; the 2 mismatches it still reports
are the pre-existing `CopyCursorT -> CopyIcon` and `CreateWindowT -> CreateWindowEx` - `displayconfig.cpp` contributes 0).

**U19 for this family: 0 LAYER, 0 BYPASS** - 0 raw hits for all 7 names over the 3,228 `.h`/`.hpp`/`.cpp`/`.cxx` of `src`,
`examples` and `test` (`_tmp_t8_6d_consumers.ps1`), which is worth recording because the pre-flags expected the opposite:
display enumeration was likelier than touch to have a live in-tree SDK call, and it has none. U19 stays FROZEN at session 34.


**6e (Shutdown / power, 3) DONE 7 Nov 2026** - the bucket drained: `CancelShutdown` (ordinal 22) joined the existing
`//System Shutdwown functions` section and `RegisterSuspendResumeNotification` (2F6) / `UnregisterSuspendResumeNotification`
(3CD) joined the existing `//Power Management functions` section, so the header grew 792 -> 795 lines with **no new section**;
`powermanagement.cpp` is the new TU (2 members / 2 bindings) and `systemshutdown.cpp` gained `CancelShutdown` (6 / 6). All 3
bind bare `qor_pp_useswinapi` - 0 function-like macros and 0 A/W-sibling rows, so `user32.h:9-17` gained no `#undef`. The
handle type is `HPOWERNOTIFY` = `PVOID`, declared at winuser.h:3985 behind `#if !defined(_HPOWERNOTIFY_DEF_)` inside
`#if(_WIN32_WINNT >= 0x0502)`, and the SDK writes the suspend/resume prototypes with a **space** before the paren, which is
why every name pattern in these scans allows `\s*(`. Gate: probe exit 0, both touched TUs standalone at /W4 exit 0, folder
43/43 exit 0, all-libraries PRE 110 -> POST 111 .cpp echoes at exit 2 both sides (standing fatal C1083 at opengl/wgl.cpp:31)
with a diagnostics set-difference of 0 new / 0 gone, and the `cl /EP` digests print 6 + 2 binding literals, all module literal
"user32", all byte-equal to export names, 0 duplicates. Counts (comment-stripped): **43 sources / 604 members / 507 bindings**.
Coverage re-measure: **727 of 1,018** export names covered, **38** declared-and-unwrapped raw, 253 no-prototype; the buckets
instrument prints `0: Shutdown / power notification []`, and the raw 38 = Misc **24** + legacy hook 4 + legacy word / class-long
10, of which 7 are excluded superseded twins and 1 is the instrument artifact, so the net backlog is **30** = Misc 24 + held-open
legacy 6. U19 stays FROZEN at session 34 (0 raw consumer hits for all 3 names over 3,228 sources). Next tranche is **6f - Misc,
24 names**, whose measured SDK facts are written into `preflags.md` (replaced wholesale); `tmp/_tmp_t8_6f_pre.ps1` derives the
bucket from `tmp/_tmp_u18_gap_sdk.txt`, so run `tmp/_tmp_u18_coverage4.ps1` first or a stale gap list feeds the previous family
back to you.

## Tranche 6f closed - the Misc bucket (the `Other` row), 7 Nov 2026

All 24 Misc names are now wrapped: new TU `misc.cpp` (23 members / 23 bindings, banner `:12`) plus one new `//Misc functions`
section at `user32.h:789-816`, so the header is **823** lines and the folder is **44 sources / 627 members / 530 bindings**.
24 names over 23 members because `ChangeMenuA`/`ChangeMenuW` (2A/2B) is the only A/W pair left in the bucket and has **no bare
export** (`ChangeMenu=0` measured), so the T-suffix convention collapses it to one `ChangeMenuT` member bound over
`qor_pp_useswinapiAW` - and `qor_pp_unicode` is 1 (`configuration.h:13`), so this build binds the W form and the A form stays
unresolved at runtime, which is what keeps U17's A/W question one wrapper rather than dozens. `user32.h:9-17` gained no `#undef`
(0 function-like macros over the 24). Coverage re-measure: **751 of 1,018** covered, **14** declared-and-unwrapped raw, 253 with
no prototype; the buckets instrument prints `Other` 0 beside five other drained families, and the 14 = legacy hook **4** +
legacy word / class-long **10**, of which 7 carry the superseded-twin exclusion and 1 (`SetWindowsHookExAW`) is the classifier
artifact, so the **net backlog is 6** - the held-open names above, and they are now the folder lead.

**The 6 measured again for the decision (`tmp/_tmp_t8_6g_pre.ps1`, forward pass).** Ordinal / `winuser.h` line / innermost
guards: `DragObject` CC / `:8145` / `NOUSER`, `WINVER >= 0x0400`, DESKTOP · `GetWindowWord` 1FB / `:9837` / `NOUSER`,
`NOWINOFFSETS`, DESKTOP · `SetMessageQueue` 351 / `:3500` / `NOUSER`, `NOMSG`, DESKTOP · `SetWindowWord` 388 / `:9844` /
same · `wsprintfA` 3F6 / `:326` / `NOUSER`, DESKTOP · `wsprintfW` 3F7 / `:333` / same. All 6 are exported, declared, carry 0
function-like macros and 0 rows in `user32.h`. The decision has a **technical** side, measured not assumed: `wsprintfA`/`wsprintfW`
are `WINAPIV` (i.e. `__cdecl` **variadic**: `_Out_ LPSTR, _In_ _Printf_format_string_ LPCSTR, ...`) and the folder has **0**
variadic members. `Library::Call` (`library.h:43-46`) is a variadic template, but it synthesizes a **fixed-arity**
`ret(__stdcall *fPtr)(MethodArgs...)` from the argument list at the call site, so binding these would assert an arity the SDK
never states - the wrapper would have to be variadic itself, a shape no wrapper in this library has. The `__stdcall` vs `__cdecl`
half of that mismatch is immaterial here (D17: Windows is x64-only, one calling convention), so the undeclared arity is the
obstacle. `GetWindowWord`/`SetWindowWord` (`WORD` returns) are the 16-bit twins of `GetWindowLongT` (`user32.h:723`) and
`SetWindowLongT` (`:728`), which **are** wrapped, so the twin test nearly decides them and what keeps them open is the width
change. Only `SetMessageQueue` and `DragObject` have neither a replacement nor a wrapped cousin.

**6g landed - the 6 are wrapped, and the two measured halves of that decision (7 Nov 2026, `legacy.cpp` banner).** The four
ordinary names copy their `winuser.h` prototype verbatim and bind bare `qor_pp_useswinapi` (DragObject CC / `:8145`,
SetMessageQueue 351 / `:3500`, GetWindowWord 1FB / `:9837`, SetWindowWord 388 / `:9844`); `wsprintfA`/`wsprintfW` collapse
to the one `wsprintfT` member over `qor_pp_useswinapiAW`, which under `qor_pp_unicode` 1 binds `wsprintfW` (3F7). The
`WINAPIV` half is answered in `library.h`: `varCall` (variant (V)) synthesizes `ret(__cdecl *fPtr)(MethodArgs..., ...)`,
because `minwindef.h:128`/`:129` spell `WINAPI` `__stdcall` and `WINAPIV` `__cdecl` and the SDK states no tail arity. The
placement half is answered by measurement, not by style: a member template declared in `user32.h` and defined in one TU
gives **LNK2019 per tail shape / LNK1120 on the link**, so `wsprintfT` is the folder's one header-inline member and
`user32.h` gains the `../library.h` include its body needs. Two consequences are recorded rather than hidden - the binding
inside a template body is a function-local static (one `pFunc` per instantiation), and the body carries no `qor_pp_fcontext`
because the published header does not include the interception flyer. Consumers: 0 LAYER / 0 INDIRECT / 0 BYPASS / 0 MEMBER
over the eight stem spellings, so no caller above the layer had to change. The bucket is drained with nothing unaccounted.
