# narrative: cursor.cpp, session 9

## cursor.cpp - session 9 (30 Oct 2026)

Sixteen wrappers, sixteen bindings - the largest TU reviewed and **0 defects of any class** (binding, signature, UB,
convention). It is therefore the TU where the method had to earn its keep: nothing to fix, so everything the earlier
sessions had been asserting by inspection was measured instead. Two instruments changed as a result, one open item (U20)
was opened from the caller side, and the pre-flagged P3 case was settled as correct.

| Wrapper (`user32.h:381-397`, all 16 `static`) | Definition | Binding (`cl /EP` literal) | Export (ordinal, RVA) | SDK prototype (`winuser.h` 10.0.26100) | Verdict |
|---|---|---|---|---|---|
| `ClipCursor(const RECT*) -> BOOL` | `:53` | `user32, ClipCursor` -> `"ClipCursor"` | 4E, 0002EA90 | `:9594 BOOL WINAPI ClipCursor(_In_opt_ CONST RECT*)` | correct |
| `CopyCursorT(HCURSOR) -> HCURSOR` | `:60` | `user32, CopyIcon` -> `"CopyIcon"` | 59, 00027DF0 | `:10976 HICON WINAPI CopyIcon(_In_ HICON)`; `CopyCursor` is the `:10681` macro | correct (see below) |
| `CreateCursor(HINSTANCE,int,int,int,int,const VOID*,const VOID*) -> HCURSOR` | `:67` | `"CreateCursor"` | 60, 0004CF40 | `:10659` | correct |
| `DestroyCursor(HCURSOR) -> BOOL` | `:74` | `"DestroyCursor"` | AF, 00029250 | `:10671` | correct |
| `GetClipCursor(LPRECT) -> BOOL` | `:81` | `"GetClipCursor"` | 134, 0002EE40 | `:9447` | correct |
| `GetCursor(void) -> HCURSOR` | `:88` | `"GetCursor"` | 13F, 0002EE90 | `:9453 HCURSOR WINAPI GetCursor(VOID)` | correct |
| `GetCursorInfo(PCURSORINFO) -> BOOL` | `:95` | `"GetCursorInfo"` | 141, 0002EEA0 | `:14661` | correct |
| `GetCursorPos(LPPOINT) -> BOOL` | `:102` | `"GetCursorPos"` | 142, 0002B230 | `:9426` | correct |

| `GetPhysicalCursorPos(LPPOINT) -> BOOL` | `:109` | `"GetPhysicalCursorPos"` | 193, **0002B230** | `:9439` | correct |
| `LoadCursorT(HINSTANCE,LPCTSTR) -> HCURSOR` | `:116` | `useswinapiAW` -> `"LoadCursorW"` | 259, 00015C30 | `:10619` (macro `:10623`) | correct |
| `LoadCursorFromFileT(LPCTSTR) -> HCURSOR` | `:123` | `useswinapiAW` -> `"LoadCursorFromFileW"` | 258, 00048300 | `:10642` (macro `:10645`) | correct |
| `SetCursorPos(int,int) -> BOOL` | `:130` | `"SetCursorPos"` | 32E, 0002FA50 | `:9398` | correct |
| `SetPhysicalCursorPos(int,int) -> BOOL` | `:137` | `"SetPhysicalCursorPos"` | 354, **0002FA50** | `:9406` | correct |
| `ShowCursor(BOOL) -> int` | `:144` | `"ShowCursor"` | 395, 0002FD90 | `:9386 int WINAPI ShowCursor(_In_ BOOL)` | correct |
| `SetCursor(HCURSOR) -> HCURSOR` | `:151` | `"SetCursor"` | 32C, 0002A790 | `:9420 HCURSOR WINAPI SetCursor(_In_opt_ HCURSOR)` | correct |
| `SetSystemCursor(HCURSOR,DWORD) -> BOOL` | `:158` | `"SetSystemCursor"` | 36A, 0007EB90 | `:10728` | correct |

- **All sixteen resolve, and by two independent routes.** `cl /EP` of the TU prints the sixteen `GetProcAddress` literals
  (`tmp/_tmp_cursor_pp.txt:453241-453346`), and a live probe that resolves them exactly the way `qor_pp_useswinapi` does
  (`GetModuleHandleW(L"user32")` then `GetProcAddress`) resolves **16 of 16 RESOLVED**, plus the A/W alternatives, plus the
  names that should fail (`tmp/_tmp_cursor_probe.cpp`: `CopyCursor` MISSING, `SetCursorContents`/`ShowSystemCursor`/
  `GetCursorFrameInfo` RESOLVED). Note the probe's first line: `GetModuleHandleW(L"user32")` returns NULL in a process that
  has not mapped the DLL - the library never `LoadLibrary`s, so `pFunc` would be cached NULL forever. That is G5/U5
  demonstrated rather than asserted; recorded, not fixed.
- **None of the sixteen stems is in the `user32.h:9-17` `#undef` list** (measured against the header: 9 names -
  `RegisterDeviceNotification`, `WinHelp`, `SystemParametersInfo`, `GetRawInputDeviceInfo`, `DlgDirList`,
  `DlgDirSelectEx`, `DlgDirListComboBox`, `DlgDirSelectComboBoxEx`, `LoadBitmap`). So G2 - the SDK `stem -> stemW` macro
  making a bare-stem binding a live P1 failure - cannot bite in this family, which is why fourteen bare
  `qor_pp_useswinapi` spellings are correct here rather than a defect class.
- **The pre-scan P3 flag (`CopyCursorT` -> `CopyIcon`) is a correct binding, and now for a measured reason.** `CopyCursor`
  is not an export at all: `winuser.h:10681` is `#define CopyCursor(pcur) ((HCURSOR)CopyIcon((HICON)(pcur)))` (inside
  `#ifndef _MAC`), and the live probe says `CopyCursor` MISSING / `CopyIcon` RESOLVED. It is also the mirror case the U17
  census reports for this section (`T` suffix with no A/W family). Left as-is: renaming a member exported through
  `QOR_WINAPIUSER` is a published-surface change and belongs with the folder-wide U17/P3 decision.
- **The type story behind that wrapper, measured rather than assumed:** `HCURSOR` *is* `HICON` in this build.
  `windef.h:88` is `DECLARE_HANDLE(HICON)` and `:131` is `typedef HICON HCURSOR;`; the separate
  `DECLARE_HANDLE(HCURSOR)` at `:133` is the `#else` arm of the same `#ifndef _MAC`, so it is not what this TU gets.
  `/EP` of `cursor.cpp` contains exactly one HCURSOR spelling - `typedef HICON HCURSOR;`
  (`tmp/_tmp_cursor_pp.txt:174186`). So `Library::Call< HCURSOR, HCURSOR >` against the `HICON WINAPI(HICON)` export is
  type-identical and needs no cast; the only remaining hazard in that path is U6's `__cdecl` `fPtr`, shared by every
  wrapper in the folder.
- **Session 8's defect class is closed for this family by construction, and that was tested.** A wrapper in this class
  cannot call the SDK entry point of the same name even by accident: inside `User32`, an unqualified `SetCursorPos(x, y)`
  binds to the member itself. Probe `tmp/_tmp_recursion_probe.cpp` compiles that shape and MSVC gives
  `warning C4717: 'User32::SetCursorPos' : recursive on all control paths, function will cause runtime stack overflow` -
  a warning, not an error, so it would still build and then recurse. All 16 bodies here go through `pFunc`, and
  `tmp/_tmp_wrong_stem_scan.ps1` over the whole folder now flags `cursor.cpp:63` only as the known `CopyIcon` alias: 4
  flags folder-wide after session 8's fix, this one, `mouse.cpp:82` (`TrackMouseEvent` binding
  `winapiAW(user32, LPTRACKMOUSEEVENT)` - a live P1-class find for mouse.cpp's session), and the two `resource.cpp`
  "binding outside a `User32::` definition" lines that reproduce U11 mechanically.
- **Two run-time facts the SDK declarations do not show** (export table, this `user32.dll`): the logical/physical pairs are
  *one implementation exported twice* - `GetCursorPos` 142 and `GetPhysicalCursorPos` 193 both at RVA **0002B230**,
  `SetCursorPos` 32E and `SetPhysicalCursorPos` 354 both at RVA **0002FA50**. Whatever DPI behaviour the
  `WINVER >= 0x0600` declarations draw, it is not observable through this table on this build.

- **Ownership is now written where it belongs.** `CreateCursor`, `LoadCursorFromFileT` and `CopyCursorT` return handles the
  caller must `DestroyCursor`; `LoadCursorT` and `GetCursor` return shared system handles that must not be destroyed;
  `SetSystemCursor` takes no ownership (its SDK declaration carries no ownership SAL). `ClipCursor` is `_In_opt_` (NULL
  clears the clip rect) and `GetCursorInfo` requires `pci->cbSize` from the caller first. `ShowCursor` returns the signed
  display *count*, so 0 or negative is a normal hide - there is no failure channel for a return check to read. All of that
  is now in the TU comment; none of it is stated in the callers, which is **open item U20** (four measured caller-side
  findings: default-`true` ownership in the `Cursor`/`Icon` ctors, `ButtonController` omitting the flag its sibling
  `StaticController` passes, a spurious `PrimitiveHandle` leak warning on every destruction because `~Cursor` never clears
  `m_handle`, and `Cursor::Show` collapsing the count to `bool`). Nothing outside `cursor.cpp` was touched.
- **U19 - the cleanest family of the review: 0 bypass sites in built code.** 17 **LAYER** sites (one per wrapper,
  `LoadCursorT` twice, all in `ui/gui/gdiobjects/cursor.cpp`, built at `gui/CMakeLists.txt:43`), 13 raw **BYPASS** hits, of
  which 9 are not SDK calls at all once the enclosing declaration is read (`BlobBuffer::SetCursor`, `Input::DestroyCursor`,
  the two-argument `Platform::SetCursor`, two member declarations). That is why `_tmp_bypass_scan.ps1` gained a fourth
  class, **MEMBER**. The remaining 4 are live SDK calls, all in one file and all in a target that is **not in the build**:
  `ui/gl/windowimpl.cpp:247` (`SetCursor` + `LoadCursorW`), `:254`, `:272` (`::ClipCursor(nullptr)`), `:2219`
  (`::SetCursorPos`) - `winqor_gl` is commented out at `ui/CMakeLists.txt:4` and has no project anywhere in
  `cmakebuild/QOR.sln` (149 projects scanned). Those four lines are CODE, not comment (`tmp/_tmp_comment_state.ps1`; the
  file is 1503 comment-only lines of 2637).
- **U15 - the first TU with a call site for every wrapper, and the sharpest illustration of the item.** At wrapper level
  16/16 are called; one level up, the twelve `Cursor` methods that make those calls (`Show`, `Clip`, `GetClip`,
  `GetCurrent`, `GetInfo`, `GetPosition`, `GetPhysicalPosition`, `SetPosition`, `SetPhysicalPosition`, `SetAsCurrent`,
  `SetAsSystemCursor`, `Clone`) have no call site anywhere in `src`/`examples`/`test`, so fifteen wrappers are linked and
  never executed. Only `LoadCursorT` is live end to end: `examples/experimental/osspecific/windows/mainwindow/mainwindow.cpp:339`
  (a project that *is* in `QOR.sln`) -> `ui/gui/factories/windowfactory.cpp:90`
  (`windowClass->SetCursor(Cursor(IDC_ARROW))`) -> `Cursor::Cursor(const wchar_t* id)` (`gdiobjects/cursor.cpp:49-53`,
  `m_owning = false` - correct for a shared system cursor) -> `User32::LoadCursorT` -> `LoadCursorW`. That path is also
  what makes the U20 findings observable rather than theoretical.
- **Open items this session contributes to (recorded, not acted on):**
  - U1 - 16 more call sites, 0 with `Win32ErrorHandler _;` (0 of 38 TUs unchanged).
  - U2 - 0 `CheckReturn`, and this TU shows why the item is a behaviour decision rather than a typo: `CheckNonZero` on
    `ShowCursor` would be wrong by construction (a count), `CheckPointer` on the three allocating wrappers would turn a
    legitimate NULL-on-failure into an issue, and `SetCursor` returns NULL legitimately to mean "no previous cursor".
  - U3, U10, U11, U12, U13 - none. U4 - none: all sixteen members are declared `static` (`user32.h:382-397`) and defined.
  - U5 - 16 more process-lifetime-cached bindings; the probe above demonstrates the unmapped-module case.
  - U6 - 16 more `Library::Call` sites whose `fPtr` is default `__cdecl` against `__stdcall` exports.
  - U7 - applies to all sixteen, worst for the four handle-returning wrappers, where the swallowed default return
    (`nullptr`) is the same value the API legitimately returns for "no cursor"/"previous cursor was NULL".
  - U8 - untested; only the `LoadCursorT` path could be exercised without inventing a new caller (through
    `WindowFactory::AddWindowClass`).
  - U14 - (b) 16/16 sites state the full SDK argument list, so nothing to fix; (a) this is the first file with an
    *internal* spacing split - 15 `Call< BOOL, LPRECT >` against 1 `Call<BOOL, const RECT*>` (`cursor.cpp:57`) - left
    alone under the no-churn rule and recorded as the folder-wide pass's per-file deviation; (c) `GetCursor(void)` already
    follows the `(void)` norm.
  - U16 - **3 of 16** pair with guarded SDK declarations: `GetPhysicalCursorPos` (`:9439`) and `SetPhysicalCursorPos`
    (`:9406`) inside `#if(WINVER >= 0x0600)` (`:9435`, `:9402`); `GetCursorInfo` (`:14661`) and the
    `CURSORINFO`/`PCURSORINFO` types in its signature (`:14644-14650`) inside `#if(WINVER >= 0x0500)` (`:13594`) with
    `#ifndef NOWINABLE` (`:13716`) between. The other thirteen sit under nothing but `#ifndef NOUSER` plus a
    `WINAPI_FAMILY_PARTITION` (Desktop; Desktop|Games; Desktop|PC-App for `ClipCursor`). Guard stacks printed by
    `tmp/_tmp_sdk_guard_scan.ps1` are kept in `tmp/_tmp_cursor_sdk.txt`. Reverse half: nothing - the seven cursor exports
    not wrapped here are declared nowhere in `user32.h`, so no consumer-visible declaration outranks the SDK.
  - U17 - the mirror case `CopyCursorT` (`:383`) confirmed as *binding-correct, name-indirect*; census unchanged.
  - U18 - none: no return-type contradiction in this section (`ShowCursor`'s `int` is right; the loss is in the caller,
    which is U20).
  - U19, U20 - see the two entries above; U20 opened by this session.

- **Section coverage.** "Cursor functions" (`user32.h:381-397`) is exactly these sixteen. Not wrapped and not declared
  anywhere in `user32.h`: `GetCursorFrameInfo` (140), `SetCursorContents` (32D), `ShowSystemCursor` (399) - all three
  RESOLVED by the live probe - plus `SetThreadCursorCreationScaling` (36D), `GetPointerCursorId` (194),
  `GetPointerDeviceCursors` (196), `MITGetCursorUpdateHandle` (274). `SetSystemCursor`/`ShowSystemCursor` remain supported
  exports although Microsoft documents both as unsupported from Windows 8 up (a documentation claim, not re-measured).
- **Conventions checked and clean:** licence header, include order with the Windows headers last, `qor_pp_fcontext` at all
  16 sites, lowercase `user32` module token, section comment `//Cursor functions` in house style, no local `#undef`,
  4-space indent, no tabs, definitions in header order, all sixteen declaration/definition pairs present in one TU.
- **Fixed this session:** the TU comment block only (D13 binding rationale with ordinals and RVAs, the
  `CopyCursor`-is-a-macro note, the `HCURSOR`-is-`HICON` note, the shared-RVA finding, per-wrapper handle ownership, the
  `ShowCursor` count-not-failure contract, the guard exposure and the list of unwrapped cursor exports). No header change,
  no rename, no binding change.
- **Verified at exit:** `cursor.cpp` compiles standalone at `/W4` exit 0; the whole library compiles **38/38 at `/W4`,
  exit 0, no diagnostics** (`tmp/_tmp_cl_all.log`, re-run after the edit). No published surface moved, so no caller was
  affected.
- **Instruments this session leaves behind:** `tmp/_tmp_cursor_probe.cpp` + `tmp/_tmp_run_cursor_probe.bat` (the
  `GetModuleHandleW`/`GetProcAddress` probe for this family, including the negative controls),
  `tmp/_tmp_cursor_sdk_scan.ps1` -> `tmp/_tmp_cursor_sdk.txt` (SDK signature + guard stack per cursor declaration),
  `tmp/_tmp_recursion_probe.cpp` + `tmp/_tmp_recursion_run.bat` (proves a wrapper cannot call its own SDK name in this
  class - C4717), `tmp/_tmp_cursor_bypass_s9.txt` (the U19 split) and the **MEMBER** class added to
  `tmp/_tmp_bypass_scan.ps1`, which every later session's U19 count now benefits from.

