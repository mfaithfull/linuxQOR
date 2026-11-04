# G1-G6 ground rules established, measured not assumed (verbatim)

## Ground rules established (measured, not assumed)

- **G1 - the SDK A/W macro rewrites the binding name before it is stringized.** `_ATXT(_NAME)` inside
  `qor_pp_useswinapi` (`kernel32.h:50`) is a *nested* macro call, so `_NAME` is macro-expanded first.
  Proof: `qor_pp_useswinapi(user32, CreateAcceleratorTable)` preprocesses to
  `GetProcAddress(..., "CreateAcceleratorTableW")`. So a bare stem binds the **W** entry point whenever the
  SDK still defines the `stem -> stemW` macro, and only binds the literal stem when that macro has been
  `#undef`'d. `UNICODE`/`_UNICODE` is set from `qor_pp_unicode` at `kernel32.h:7-10`, so the QOR switch and
  the SDK switch are coupled through include order (`user32.h:7` includes `kernel32.h` first).
- **G2 - the nine stems `user32.h:9-17` `#undef`s are the real missing-export risk.** All nine resolve to
  the literal stem, and all nine literals are *not* exported while their `W` variants are (live probe:
  `WinHelp` MISSING / `WinHelpW` RESOLVED; same for `SystemParametersInfo`, `GetRawInputDeviceInfo`,
  `DlgDirList`, `DlgDirSelectEx`, `DlgDirListComboBox`, `DlgDirSelectComboBoxEx`, `LoadBitmap`,
  `RegisterDeviceNotification`). Any wrapper that binds one of those bare stems throws
  `Missing library function.` at run time. Measured status of all nine after session 6: **2 fixed in this review**
  (`DlgDirListComboBox`/`DlgDirSelectComboBoxEx` in `combobox.cpp`, now `qor_pp_useswinapiAW` and renamed `...T`);
  **2 defined and already safe** (`RegisterDeviceNotification` `devicemanagement.cpp:76`, `WinHelp` `help.cpp:35`, both
  `qor_pp_useswinapiAW`, which token-pastes `W` before the SDK macro could be consulted); **5 not defined anywhere in the
  folder** (`SystemParametersInfo` `user32.h:652`, `GetRawInputDeviceInfo` `:540`, `DlgDirList` `:546`, `DlgDirSelectEx`
  `:547`, `LoadBitmap` `:672` - U4 members, so nothing binds them today). Check this list before re-flagging G2 in a
  later session.
- **G3 - the previous audit report is partly wrong.** `tmp/_tmp_audit_report.txt` section A ("run-time
  failures") was produced by grepping the source spelling, so it reported a false failure for every name
  whose SDK A/W macro is still in scope - `CreateAcceleratorTable`, `LoadAccelerators`, `MessageBox`,
  `TranslateAccelerator` and friends all bind correctly today. Only the G2 names, plus genuinely absent
  names such as `GetDlgItemTextT` (the `T` stringizes with it), are real. Re-derive that list per TU with
  method step 3 rather than trusting the report.
- **G4 - module token case is cosmetic.** `_TXT(User32)` stringizes to `L"User32"` and the module lookup is
  case-insensitive: `GetComboBoxInfo` resolved against `User32`, `USER32` and `user32` alike. The
  `combobox.cpp` / `scrollbar.cpp` spelling `User32` is a style break, not a bug.
- **G5 - the "already loaded image" assumption is not universal.** In a bare console process
  `GetModuleHandleW(L"user32")` returned NULL (measured), so `qor_pp_useswinapi` caches a NULL `pFunc` in a
  function-local `static const` and every later call throws even after the DLL is mapped. See item U5.
- **G6 - return checking is silently always on.** `returncheck.h:19` tests
  `qor_pp_parameter_checking == qor_pp_yes` (the checked typedef `:20`, `#else` `:21`, bare typedef `:22`, `#endif` `:23`;
  the citation read `:40` until session 14 re-measured it); neither macro is defined anywhere in the repository, so the
  `#if` evaluates `0 == 0` and `CheckReturn<...>::TType` is always the checked wrapper type, never the bare
  return. Adopting `CheckReturn` in a TU therefore changes run-time behaviour (a `continuable(...)` issue is
  raised), and the `#if` itself relies on undefined-macro semantics. See item U2.

- **G4 re-measured 3 Nov 2026 (session 30, `scrollbar.cpp`): the population is now zero, and the wording above names a
  file that no longer belongs to it.** The rule was written when the folder had two spelling sites; `tmp/_tmp_s30_token_direct.ps1`
  found exactly one left - **all 12 bindings of `scrollbar.cpp`** spelled `qor_pp_useswinapi(User32, ...)`, i.e. 12 of the
  folder's 428 bindings - and `combobox.cpp` (named above) no longer spells it that way. `kernel32.h:50` stringizes the
  token, so `/EP` emitted `GetModuleHandleW(( L"User32" ))` in those 12 wrappers while the other 416 emit
  `L"user32"`. The claim "the lookup is case-insensitive" was **re-measured for this section's 12 entry points on x64 and
  x86** (`tmp/_tmp_scroll_g4probe_s30.cpp` + `.bat`, output `_x64.txt`/`_x86.txt`): `user32`, `User32`, `USER32` and
  `user32.dll` all resolve to one and the same `HMODULE`, a deliberately bogus module name resolves to `NULL` (the probe
  discriminates), and `GetProcAddress` returns **the identical address under every spelling for 12 of 12 entry points -
  0 of 12 differ on either arch**. The 12 tokens were normalized to `user32`; folder census after the fix (`tmp/_tmp_s30_gatecheck.txt`):
  **428 bindings, 428 `user32` tokens, 0 files with a non-lowercase token.** G4 stays open as a *style* rule (it is what
  made the 12 invisible to every earlier check), but its population is now 0 and the rule is measured, not assumed.
