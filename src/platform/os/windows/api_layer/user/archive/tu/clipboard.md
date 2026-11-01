# narrative: clipboard.cpp, session 5

## clipboard.cpp - session 5 (29 Oct 2026)

The "//Clipboard functions" section (`user32.h:106-126`): twenty wrappers, twenty bindings, in exactly the header's
order. Second TU reviewed with nothing wrong in the wrappers (`caret.cpp` was the first) and the first that was already
**fully D13-conformant before the session started** - both A/W families bound with `qor_pp_useswinapiAW` and carrying
the `T` suffix, the other eighteen genuine single exports.

| Wrapper (definition line in the edited file) | SDK prototype (`winuser.h` 10.0.26100) | Resolved binding (`cl /EP`) | Export |
|---|---|---|---|
| `AddClipboardFormatListener(HWND hwnd) -> BOOL` (`:38`) | `:5568-5572` `_In_ HWND` [WINVER>=0x0600] | `"AddClipboardFormatListener"` | yes (1) |
| `ChangeClipboardChain(HWND hWndRemove, HWND hWndNewNext) -> BOOL` (`:45`) | `:5459-5464` `_In_` both | `"ChangeClipboardChain"` | yes (25) |
| `CloseClipboard(void) -> BOOL` (`:52`) | `:5424-5428` `CloseClipboard(VOID)` | `"CloseClipboard"` | yes (4F) |
| `CountClipboardFormats(void) -> int` (`:59`) | `:5510-5514` `(VOID)` | `"CountClipboardFormats"` | yes (5C) |
| `EmptyClipboard(void) -> BOOL` (`:66`) | `:5542-5546` `(VOID)` | `"EmptyClipboard"` | yes (E8) |
| `EnumClipboardFormats(UINT format) -> UINT` (`:73`) | `:5516-5520` `_In_ UINT` | `"EnumClipboardFormats"` | yes (F8) |
| `GetClipboardData(UINT uFormat) -> HANDLE` (`:80`) | `:5473-5477` `_In_ UINT` | `"GetClipboardData"` | yes (136) |
| `GetClipboardFormatNameT(UINT format, LPTSTR lpszFormatName, int cchMaxCount) -> int` (`:87`) | `:5522-5540` A/W pair, `_Out_writes_(cchMaxCount)`, macro `:5537/:5539` | `"GetClipboardFormatNameW"` | yes (138) |
| `GetClipboardOwner(void) -> HWND` (`:94`) | `:5441-5445` `(VOID)` | `"GetClipboardOwner"` | yes (13A) |
| `GetClipboardSequenceNumber(void) -> DWORD` (`:101`) | `:5433-5437` `(VOID)` [WINVER>=0x0500] | `"GetClipboardSequenceNumber"` | yes (13B) |
| `GetClipboardViewer(void) -> HWND` (`:108`) | `:5453-5457` `(VOID)` | `"GetClipboardViewer"` | yes (13C) |
| `GetOpenClipboardWindow(void) -> HWND` (`:115`) | `:5561-5565` `(VOID)` | `"GetOpenClipboardWindow"` | yes (191) |
| `GetPriorityClipboardFormat(UINT* paFormatPriorityList, int cFormats) -> int` (`:122`) | `:5554-5559` `_In_reads_(cFormats)` | `"GetPriorityClipboardFormat"` | yes (1AC) |
| `GetUpdatedClipboardFormats(PUINT lpuiFormats, UINT cFormats, PUINT pcFormatsOut) -> BOOL` (`:129`) | `:5580-5586` `_Out_writes_(cFormats)` [WINVER>=0x0600] | `"GetUpdatedClipboardFormats"` | yes (1D8) |
| `IsClipboardFormatAvailable(UINT format) -> BOOL` (`:136`) | `:5548-5552` `_In_ UINT` | `"IsClipboardFormatAvailable"` | yes (230) |
| `OpenClipboard(HWND hWndNewOwner) -> BOOL` (`:143`) | `:5418-5422` `_In_opt_ HWND` | `"OpenClipboard"` | yes (29C) |
| `RegisterClipboardFormatT(LPCTSTR lpszFormat) -> UINT` (`:150`) | `:5494-5508` A/W pair (`LPCSTR`/`LPCWSTR`), macro `:5505/:5507` | `"RegisterClipboardFormatW"` | yes (2E2) |
| `RemoveClipboardFormatListener(HWND hwnd) -> BOOL` (`:157`) | `:5574-5578` `_In_ HWND` [WINVER>=0x0600] | `"RemoveClipboardFormatListener"` | yes (301) |
| `SetClipboardData(UINT uFormat, HANDLE hMem) -> HANDLE` (`:164`) | `:5466-5471` `_In_ UINT`, `_In_opt_ HANDLE` | `"SetClipboardData"` | yes (328) |
| `SetClipboardViewer(HWND hWndNewViewer) -> HWND` (`:171`) | `:5447-5451` `_In_ HWND` | `"SetClipboardViewer"` | yes (329) |


- **All twenty correct as written; neither pre-scan P1 nor P2 touches this TU.** No clipboard stem is among the nine
  `#undef`s at `user32.h:9-17`, so G2 cannot bite here, and `winuser.h` defines a `stem -> stemW` macro for exactly two
  clipboard names (`:5505/:5507` and `:5537/:5539`) - both of which are already bound explicitly, so G1 is not
  load-bearing anywhere in this file. `/EP` after the edits resolves 18 literal stems plus `GetClipboardFormatNameW`
  and `RegisterClipboardFormatW`; every one is in the `user32.dll` export table with the ordinal shown above.
- **`T` suffix correct by construction**: present on exactly the two A/W families, absent on the other eighteen - the
  inverse of the P1 state in `keyboard.cpp`/`dialog.cpp`, where `T`-suffixed wrappers are bound bare.
- **A/W consistency argument (new, worth keeping):** `qor_pp_unicode` is 1 (`src/configuration/configuration.h:13`),
  so `kernel32.h:7-10` defines `UNICODE`/`_UNICODE`; `qor_pp_useswinapiAW` (`kernel32.h:52`) then picks the `W` entry
  point **and** `TCHAR`/`LPTSTR`/`LPCTSTR` are wide. One switch drives both halves, so a `*T` wrapper cannot end up
  wide-named with narrow parameters. `LPCTSTR` in `RegisterClipboardFormatT` is const (`LPCWSTR` in this build) exactly
  as the SDK's `_In_ LPCWSTR` wants, and `LPTSTR` in `GetClipboardFormatNameT` is non-const as its `_Out_writes_`
  parameter requires.
- Signature pairing exact against the SDK for all twenty - return type, parameter types **and parameter names**
  (`hwnd`, `hWndRemove`, `hWndNewNext`, `uFormat`, `hMem`, `format`, `lpszFormatName`, `cchMaxCount`,
  `paFormatPriorityList`, `cFormats`, `lpuiFormats`, `pcFormatsOut`, `lpszFormat`, `hWndNewOwner`). All twenty are
  declared `static`, so the section is not part of U4's 97 and none has U13's shape; `tmp/_tmp_static_census.ps1`
  re-run after the edits still reports exactly one defined-but-non-static member folder-wide (`ConsoleControl`).
- **Fixed (convention, U14(c)):** `()` -> `(void)` at `clipboard.cpp:52,59,115` and the matching declarations
  `user32.h:109,110,118` - the SDK spells these `(VOID)` and 33 of the folder's 38 zero-argument definitions already
  did. Zero behaviour change.
- **Fixed (documentation):** the TU had no comment. It now records the bare-stem rationale for the eighteen, the
  one-switch A/W argument, the affinity/ownership contracts (`OpenClipboard` takes the clipboard for the calling
  thread's task and must be matched by `CloseClipboard` on that thread; `EmptyClipboard` needs ownership;
  `SetClipboardData` transfers `hMem` to the system only on success; `GetClipboardData` is a borrowed system-owned
  handle) and the coverage/deprecation note.
- **Section complete in both directions:** 20 header declarations -> 20 definitions, none of them defined in another TU
  (no U12 transposition, no U4 gap, no U11 unqualified definition); and 20 of the 21 clipboard entry points
  `winuser.h:5418-5587` declares are wrapped. The one unwrapped SDK entry point is `GetClipboardMetadata`
  (`:5487-5492` + `GETCLIPBMETADATA`). `user32.dll` exports two further clipboard names,
  `CheckProcessForClipboardAccess` and `GetClipboardAccessToken`, that have no public SDK declaration and are therefore
  out of reach of this class by design.
- **U16 opened from this TU:** 4 of the 20 SDK declarations (`GetClipboardSequenceNumber`,
  `AddClipboardFormatListener`, `RemoveClipboardFormatListener`, `GetUpdatedClipboardFormats`) sit behind `WINVER`
  guards, and the build pins no `WINVER`/`_WIN32_WINNT`/`NTDDI_VERSION` anywhere - the prototypes are visible only
  through the SDK default (`sdkddkver.h:227-229`, `:246-253` = `0x0A00`).

- **U1/U2 instance count: 20 call sites, 0 `Win32ErrorHandler`, 0 return checks** (settled policy). Under the
  `shell`/`kernel` policy 11 sites would take a mechanical check (8 `BOOL` -> `CheckReturn<BOOL, BoolCheck>`,
  2 `HANDLE` -> `CheckNonZeroHandle<HANDLE>`, `RegisterClipboardFormatT` -> `CheckNonZero<UINT>`) and 9 must stay raw
  because zero/NULL is a legitimate answer (`CountClipboardFormats`, `EnumClipboardFormats` where 0 ends the
  enumeration, `GetClipboardOwner`, `GetClipboardViewer`, `GetOpenClipboardWindow`, `SetClipboardViewer`,
  `GetClipboardSequenceNumber` where 0 also means "not associated"), while `GetClipboardFormatNameT` is ambiguous by
  contract (0 for a standard format *and* for failure) and `GetPriorityClipboardFormat` is **inverted** - 0 = failure,
  -1 = "none of the listed formats" - so the folder's non-zero idiom would fire on its success path, and
  `IsClipboardFormatAvailable` would raise an issue on every ordinary "not available" answer. This TU is the strongest
  instance of the U2 decision: a mechanical rule would be wrong on a third of the surface.
- **U14 contributes nothing to change:** 20/20 sites state the full SDK argument list (0 deducing) and 20/20 use the
  spaced `Call< BOOL, HWND >` form, so neither half of U14(a)/(b) applies; nothing was retouched on those grounds.
- **U15: all 20 wrappers are instances** (analysis in the U15 list). The notable part is that the tree does do clipboard
  work - through a **parallel direct-SDK path** in `ui/gl` (`platform.cpp:143-158`, `:170-202` - 9 live sites), complete
  with its own `while (!OpenClipboard(...))` retry-on-busy loop, which never reaches `GetProcAddress`. gui's
  `ClipboardHandler` handles the clipboard messages but calls no wrapper, and its slot `m_clipboardOwner` has no writer,
  so it is never installed. *(Corrected 30 Oct 2026, session 8: this entry also cited `windowimpl.cpp:2395-2410` as part
  of that path. It is not - the whole region sits inside a block comment that opens at `windowimpl.cpp:2221` and closes at
  `:2498`, verified with `tmp/_tmp_comment_state.ps1`. The 9 `platform.cpp` sites were re-checked the same way and are all
  CODE. Counted as open item U19's first instance.)*
- U5 (binding cached for the process lifetime), U6 (20 more default-`__cdecl` `fPtr` call sites), U7 (a swallowed SEH
  fault returns `FALSE`/`NULL`/`0`, indistinguishable from a real clipboard failure - and clipboard code is exactly
  where a use-after-`CloseClipboard` fault is plausible) and U8 all apply unchanged. No cross-boundary type work in
  this TU: no QOR struct is reinterpreted as a clipboard SDK struct and `GetUpdatedClipboardFormats`'s buffer is
  caller-owned, so no layout-compatibility check of the session-2/session-4 kind was needed.
- Verified: `/EP` after the edits resolves exactly the twenty names in the table; `clipboard.cpp` compiles standalone
  at `/W4` exit 0; whole library **38/38 at `/W4`, exit 0**, no diagnostics (`tmp/_tmp_cl_all.log`).

