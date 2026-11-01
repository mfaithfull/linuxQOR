# narrative: combobox.cpp, session 6

## combobox.cpp - session 6 (29 Oct 2026)

The "//Combo Box functions" section (`user32.h:550-553`): three wrappers, three bindings, in header order, and the
first TU in this review that contains **bindings that could never work** - two of the three asked `GetProcAddress` for
a name `user32.dll` does not export, so those two wrappers could only ever throw.

| Wrapper (spelling after this session) | SDK entry (`winuser.h` 10.0.26100) | Binding before -> after (`cl /EP`) | Export |
|---|---|---|---|
| `DlgDirListComboBoxT(HWND, LPTSTR, int, int, UINT) -> int` | `:11643-11665` `DlgDirListComboBoxA/W`, `#define DlgDirListComboBox DlgDirListComboBoxW` at `:11662` (`#undef`'d at `user32.h:15`) | `"DlgDirListComboBox"` MISSING -> `"DlgDirListComboBoxW"` | yes (C3/C2, probe) |
| `DlgDirSelectComboBoxExT(HWND, LPTSTR, int, int) -> BOOL` | `:11667-11687` `DlgDirSelectComboBoxExA/W`, macro at `:11684` (`#undef`'d at `user32.h:16`) | `"DlgDirSelectComboBoxEx"` MISSING -> `"DlgDirSelectComboBoxExW"` | yes (C6/C5, probe) |
| `GetComboBoxInfo(HWND, PCOMBOBOXINFO) -> BOOL` | `:14771-14787` `COMBOBOXINFO` + `WINUSERAPI BOOL WINAPI GetComboBoxInfo(_In_ HWND, _Inout_ PCOMBOBOXINFO)` | `"GetComboBoxInfo"` (unchanged) | yes (13D) |

- **Fixed (correctness).** Both `DlgDir*` bindings are now `qor_pp_useswinapiAW(user32, <stem>)`. The reason the bare
  spelling is fatal *for these two specifically* is that `user32.h:15,16` `#undef` the stems, so G2's macro rescue
  cannot fire and `_ATXT(stem)` stringizes the literal, un-exported name. `qor_pp_useswinapiAW` token-pastes `W`
  before expansion, so the `#undef` is irrelevant to it. Live probe after the fix: all three names RESOLVED.
- **Fixed (naming, user decision).** `user32.h:551-552` and the two definitions now read `DlgDirListComboBoxT` /
  `DlgDirSelectComboBoxExT`, restoring the folder's `T`-suffix norm for A/W wrappers. Zero call sites existed for
  either name anywhere in `src`/`examples`/`test`, so no call-site edits were needed and the rename cannot change
  behaviour in this tree. `GetComboBoxInfo` keeps the bare SDK name (no A/W family; D13 names it as the example).
  The remaining 11 such members are U17, and the mirror case (`T` with no A/W family) is pre-scan P3.
- **Fixed (consistency).** All three bindings' module token `User32` -> `user32` (the `_TXT()` argument of
  `GetModuleHandle`; the run-time lookup is case-insensitive, so this is spelling only - G4). Folder-wide census over
  all 428 bindings: 416 now `user32`, 12 `User32`, all in `scrollbar.cpp`, which keeps them for its own session.
- **Fixed (documentation).** New TU comment: why the third binding is bare, why the pair must be AW, the `_Inout_`
  `lpPathSpec` / `_Out_writes_(cchOut)` `lpString` buffer contracts, the `DDL_*` filter set, the
  `cbSize`-pre-set-and-child-handles-are-control-owned contract of `GetComboBoxInfo`, the `T`-suffix rationale and the
  section-coverage claim.
- **Verified correct as written:** all three return types, parameter types, parameter count and order against the SDK;
  `PCOMBOBOXINFO` is used as the SDK type rather than a QOR stand-in; `qor_pp_fcontext` in all three; 4-space indent,
  file-local includes last, `}}}//qor::win::api`; the whole `//Combo Box functions` section is `static` (unlike the
  adjacent List Box and DDE blocks - U4) and all three members are defined, so this TU is not part of U4's gap.
- **Left alone deliberately:** the tight `Call<...>` spacing, 3/3 explicit template arguments (U14 - no churn); no
  `Win32ErrorHandler _;` and no `CheckReturn` at any of the 3 sites (settled policy, U1/U2 - this TU's tally is 3 call
  sites, 0 handlers, 0 checks), so all three failures are silent today; `returncheck.h` included and unused; the
  binding still caches in a function-local `static const` for process lifetime (U5); `Call`'s `fPtr` is still the
  default-`__cdecl` typedef against `WINAPI` exports - 3 more U6 sites; a swallowed SEH fault returns a default
  `int`/`BOOL` (U7); U8 (no test, no caller) applies, and 2 of the 3 wrappers became U15 instances.
- **Return-contract note for a future return-check decision:** `DlgDirListComboBoxT`'s documented result is a list-box
  index with `-1` on failure, so neither a `CheckReturn` non-zero test nor `!= 0` is valid for it (index 0 is
  legitimate); `DlgDirSelectComboBoxExT` and `GetComboBoxInfo` are plain `BOOL`s where `CheckNonZeroHandle`-style
  checking would be the `shell`-library spelling.
- **Section coverage: complete.** `user32.dll` exports exactly three `*Combo*` entry points (`GetComboBoxInfo` plus the
  two `DlgDir*ComboBox*` families), all wrapped; the combo-box *messages* (`CB_ADDSTRING`, `CB_DIR`, ...) are not
  entry points and are reached through `SendMessageT` from `ui/gui`.
- **Found while cross-checking, not fixed (not this TU): `window.cpp:124`** binds
  `qor_pp_useswinapiAW(user32, CreateWindow)` inside `User32::CreateWindowT` - `CreateWindowW` is MISSING, `CreateWindow`
  being the SDK's macro for `CreateWindowEx`. Now the largest remaining P1 item; see the pre-scan entry.
- Verified: `cl /EP` post-edit resolves `"DlgDirListComboBoxW"`, `"DlgDirSelectComboBoxExW"`, `"GetComboBoxInfo"`
  against `L"user32"`; live probe RESOLVED for all three; `combobox.cpp` compiles standalone at `/W4` exit 0; whole
  library **38/38 at `/W4`, exit 0, 0 diagnostics** (`tmp/_tmp_cl_all.log`) - the header rename broke no other TU.


