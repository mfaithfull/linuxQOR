# winqor_api_shell - the shell32.dll API as it was

Intends to provide access to the shell32.dll Shell API as was: one stateless QOR wrapper per documented entry
point, no argument rewriting, no QOR-side semantics. The wrapper layer keeps the SDK parameter types verbatim so
callers see exactly what the Windows API sees.

shell32.dll is a normal (not implicitly linked) Windows DLL: the module binds its entry points the same way
`winqor_api_kernel` does - `qor_pp_useswinapi(shell32, Name)` resolves the symbol from the image loaded in the
process image at call time (`GetModuleHandle` + `GetProcAddress`), so there is no import library and no build-time
dependency on shell32.dll. `winqor_api_kernel` is a link dependency of this library because `Win32ErrorHandler`
(the call-site exception filter every wrapper runs under) lives there.

## Surface - 60 entry points (audited 26 Oct 2026)

The scope is the modern Shell API declared in the SDK `<shellapi.h>`, both the ANSI (`*A`) and wide (`*W`)
variants of each family, plus the HRESULT-only entry points that have no A/W pair. Legacy Shell Object
Library declarations (`<shell>`, `<shobjidl.h>`, ~127 names) and the ~290 undocumented internal exports seen in
`dumpbin /exports` of `C:\Windows\System32\shell32.dll` are explicitly out of scope.

Declaration split over seven implementation files, header grouping in `shell.h` mirrors it one-to-one:

| file | count | group |
|---|---|---|
| shell.cpp | 14 | command execution and environment substitution |
| drag.cpp | 5 | drag-and-drop file query |
| fileops.cpp | 11 | batch file operations, shell file info, free space, shortcuts |
| icon.cpp | 12 | icon extraction, stock icons, image lists, overlay identifiers |
| notify.cpp | 7 | taskbar/ notification icons, app bar, notification state |
| recycle.cpp | 6 | recycle bin and printer commands |
| shellprops.cpp | 5 | localized names, property store, association classes |

Class: `qor::win::api::Shell` (`class qor_pp_module_interface(QOR_WINAPISHELL)`), namespace `qor::win::api`.
No library base, no Init/Term, no QOR state: the module is a pure API surface, and the QOR type qualifier is
therefore absent from every signature. SDK handle types (`HWND`, `HICON`, `HINSTANCE`, `HDROP`, `POINT`) are
`::`-qualified in the header on purpose - QOR redefines some of them (e.g. `HWND`) as its own window-handle
alias inside the namespace, and the SDK type is what the call expects.

## Decisions taken while implementing (26 Oct 2026)

- **Return-check policy follows the kernel layer exactly.** SDK `HRESULT` -> `CheckReturn<HRESULT>::TType`
  (raises on a failed `FAILED(hres)`); SDK `BOOL` -> `CheckReturn<BOOL>::TType` (raises on `FALSE`);
  `void` -> `Library::voidCall`. Non-checkable values (`DWORD_PTR`, `UINT`, `HICON`, `HINSTANCE`, `INT`,
  `LPWSTR*`) return raw from `Library::Call<...>` - in several of those families a zero/negative result is a
  legitimate value, so raising on it would be wrong.
- **REFIID is unqualified in every signature** (`SHGetPropertyStoreForWindow`, `SHGetImageList`,
  `AssocCreateForClasses`). The QOR prelude makes bare `REFIID` its own `UUIDGMP` type while `::REFIID` is the
  SDK GUID type; the unqualified spelling is the one that compiles against this module's include set. A
  QOR::UUIDGMP argument is therefore what these three wrappers take - see DECISIONS.md D12 for the follow-up.
- **`ShellExecute` / `ShellExecuteEx` A and W variants are separate exports** and are wrapped separately; they
  are not aliases.
- **`SHTestTokenMembership` is in scope** even though the SDK declares it with `STDAPI_(BOOL)` rather than
  `SHSTDAPI_`; it is a shell32.dll export and a documented shellapi.h entry point.
- **`Shell_NotifyIconA` / `Shell_NotifyIconGetRect` / `SHAppBarMessage` are DESKTOP-partition only**
  (`WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)` in the SDK). They are wrapped unconditionally -
  consistent with the kernel layer's treatment of DESKTOP-only entry points; a non-DESKTOP WINAPI_FAMILY
  build of this library must drop them or the call will fail to resolve.
- **`AssocCreateForClasses` is Vista-era**; MSVC headers carry no SAL declaration for it in this SDK, but
  shellapi.h does and it is a live shell32.dll export.

## Known surface gap (open, not implemented)

`shellapi.h` also declares seven entry points that were excluded from the locked 60-name scope:
`SHEnumerateUnreadMailAccountsA/W`, `SHGetUnreadMailCountA/W`, `SHSetUnreadMailCountA/W` and `SHGetDriveMedia`.
The four wide variants (`SHEnumerateUnreadMailAccountsW`, `SHGetUnreadMailCountW`, `SHSetUnreadMailCountW`,
`SHGetDriveMedia`) ARE exported by the live `shell32.dll`; the three ANSI variants are not. Adding them is
four wrappers' work following the existing pattern; the scope decision (60 endpoints) kept them out.

## Verification - 26 Oct 2026

`cmake --build cmakebuild --target winqor_api_shell --parallel 8` -> exit 0, zero `error C` / `fatal error`
lines in `tmp/build_shell2.log`; 16/16 objects linked into
`cmakebuild/src/platform/os/windows/api_layer/shell/Debug/winqor_api_shell.dll`.
`dumpbin /imports` on the fresh DLL shows `winqor_api_kernel.dll`, `qor_error.dll`, `qor_interception.dll`,
`qor_issue.dll`, `qor_current.dll`, `qor_host.dll`, `KERNEL32.dll`, the CRT shims - and no `SHELL32.DLL`,
which is the expected shape for GetProcAddress-resolved bindings.

Audit (`tmp/_tmp_shell_full_audit.py`, run against `shellapi.h` with prototypes joined before matching):
sdk names 60, header decls 60, impl defs 60 (60 listings, no duplicates), `header ^ impl` empty,
`impl - sdk` = `SHTestTokenMembership` only (the `STDAPI_` spelling), `impl not exported` empty -
every wrapped name is a real `shell32.dll` export. See docs/agent/verification.md for the same record.
