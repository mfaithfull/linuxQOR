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
| DPI / awareness (`*Dpi*`, `*Awareness*`, `*Hosting*`, `EnableNonClientDpiScaling`) | 21 | **In scope** - tranche 6b |
| Touch / gesture (`*Touch*`, `*Gesture*`) | 14 | **In scope** - tranche 6c |
| Display config (`*DisplayConfig*`, `*DisplayAutoRotation*`) | 7 | **In scope** - tranche 6d |
| Shutdown / power (`CancelShutdown`, `*SuspendResumeNotification`) | 3 | **In scope** - tranche 6e |
| Misc (menu, clipboard, affinity, feedback, immersive, tooltip, timers, undocumented-but-declared) | 26 | **In scope** - tranche 6f; 2 of these are T8 step 5, below |
| Legacy / utility with no wrapped replacement (`GetWindowWord`, `SetWindowWord`, `SetMessageQueue`, `DragObject`, `wsprintfA/W`) | 6 | **Open question** - counted in until decided |
| Superseded twins whose replacement the library **already wraps** (`GetClassLongA/W`, `SetClassLongA/W`, `SetWindowsHookA/W`, `UnhookWindowsHook`) | 7 | **Excluded** by the scope rule |
| Instrument artifact (`SetWindowsHookExAW`) | 1 | **Removed** |
| **Backlog to add** | **109** | 32+21+14+7+3+26+6 |
| **— re-measured 6 Nov 2026, after steps 5 and 6a** | **75** | the Pointer row (32) and the step-5 pair (2) are gone: 21+14+7+3+24+6 |

**Excluded - superseded twins (measured, not assumed).** `GetClassLongA`/`GetClassLongW` and `SetClassLongA`/`SetClassLongW`
are exported (12C/12F, 323/326) but their `*Ptr` replacements are exports too (12D/12E, 324/325) and are wrapped at
`windowclass.cpp:76` and `:118`. `SetWindowsHookA`/`SetWindowsHookW` (389/38D) and `UnhookWindowsHook` (3BE) are the
pre-Ex forms of `SetWindowsHookExT` / `UnhookWindowsHookEx`, wrapped at `hook.cpp:93` and `:100` - `hook.cpp:68-69`
already recorded these three as "exported and named nowhere in this library", which under this rule is now correct rather
than a gap. The DDE family and `keybd_event`/`mouse_event` are **already fully wrapped** (28 DDE members against 33 DDE
exports, every A/W pair behind a `T` member), so the exclusions the user named remove **nothing** from the backlog: they
protect existing wrappers from deletion and keep the 253 undocumented exports out, which is where their effect is.

**Held open - 6 names, one question.** `GetWindowWord`, `SetWindowWord` (the 16-bit word twins of the wrapped
`Get/SetWindowLong*`), `SetMessageQueue`, `DragObject`, `wsprintfA`, `wsprintfW`. These are legacy or utility but have no
wrapped replacement, so the "superseded twin" test does not decide them. They are counted **in** the 109 until a decision
says otherwise; excluding them takes it to 103.

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
