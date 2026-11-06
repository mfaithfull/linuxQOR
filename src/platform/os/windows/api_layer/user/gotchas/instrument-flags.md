---
Type: Context
Description: "Instrument blind spots - what a flag named in tooling.md does not prove"
---

Moved verbatim out of `gotchas.md` on 4 Nov 2026 by `tmp/_tmp_gotchas_split.ps1` - snapshot `tmp/_tmp_gotchas_presplit.md`, verbatim rejoin proven by `tmp/_tmp_gotchas_verify.ps1`.

## Instrument caveats - read a flag before acting on it
- `pwsh -NoProfile -File script.ps1 -Stems A,B -Roots x,y` **does not deliver arrays** - each comma list arrives as one
  literal string. `_tmp_bypass_scan.ps1` then reports `stems: 1` and can die at `:12` with a misleading
  `Get-ChildItem: A parameter cannot be found ...` **while still printing a totals line of 0 BYPASS from an empty file
  list**; `_tmp_sdk_guard_scan.ps1` printed `hits: 0` for four real names (a silent false negative). Correct forms: use
  the script's default roots and pass one stem, or `pwsh -Command "& script.ps1 -Stems @('A','B')"`, or a wrapper script.
  A totals line printed next to an error line is not a clean scan.
- `_tmp_sigcmp.ps1` compares **spellings as strings**, so these rows are false positives: `voidCall` rows print
  `(no Library::Call - voidCall or inlineCall?)` by design; `sdk '::DLGPROC' vs call 'DLGPROC'` (cured by spelling the
  call sites `::DLGPROC`); `CONST::X*` vs `CONST ::X*` (3/3 sites in `filledshapes.cpp`, and it will do the same in
  `rectangle.cpp`, `painting.cpp`, `text.cpp` - U25; normalize the instrument, not the source).
- `_tmp_returncheck_usage.ps1` counts `CheckReturn<` in **comments**, so its folder count over-reads (3 shown, 2 real);
  same over-count class as the `Win32ErrorHandler` matches (5, all comments).
- **PowerShell string comparison is case-insensitive by default, and that silently truncates stems.** `-match '^(.+)T$'`
  to strip the folder's `T` suffix also matches every name ending in a lowercase `t`, so `GetClientRect` was compared as a
  stem with its final `t` removed. Measured 5 Nov 2026 on the U18 coverage instrument: the member-vs-export check
  reported **111** members matching no export when the truth is **3** (`CopyCursorT`, `CreateWindowT`,
  `mxDdeCreateStringHandle`) - every false row ended in `t`. Use `-cmatch` / `-creplace` / `-cne` and re-check any count
  that moves by an order of magnitude after the fix.
- **Export-table parsing caveat:** `tmp/_tmp_user32_exports.txt` lists 1,018 names but an ordinal+RVA regex parses
  **1,014**; the four misses are the `(forwarded to NTDLL...)` rows (P4), which carry no RVA. Quote 1,014 as the parsed
  count and do not read the gap as missing exports.
- **Bypass classification false positives are now known, not guessed.** The U19 classifier reads an unqualified member
  **declaration** in another library (`ui/gui/controllers/.../menu.h:95 Menu* GetSubMenu(int)`) as a BYPASS call site:
  3 of the 18 menu rows are that. Quote row counts and real-call counts separately, and check the file/line kind before
  calling a site a bypass.
- **SAL annotations carry other parameters' names, so they must be stripped before a parameter name is read.** A
  last-identifier rule applied to `_In_reads_(cAccel) LPACCEL paccel` returns `cAccel`. Measured 6 Nov 2026 on
  `tmp/_tmp_t8_sigcmp.ps1`'s first run: of 137 rows, ~20 were that artifact (`ToUnicode` p3 `cchBuff`,
  `MapWindowPoints` p2 `cPoints`, `PrivateExtractIconsW` p0 `MAX_PATH`, `RealGetWindowClassW` and
  `CopyAcceleratorTableW` p1 `return`, `SendInput` p1 `cInputs`). Strip `^_[A-Za-z0-9_]*_ (\( ... \))?` repeatedly
  first, then read the trailing identifier.
- **A stem that is a function-like macro has no live prototype.** `CopyCursor` is `#define CopyCursor(pcur)
  ((HCURSOR)CopyIcon((HICON)(pcur)))` (`winuser.h:10681`); a `WINAPI name(...)` regex only sees the inactive `#else` arm's
  `CopyCursor(HCURSOR hCursor)`. Comparing a wrapper against such a name aligns it to the wrong export - `CopyCursorT`
  binds `CopyIconW`, so its parameter is `hIcon`. The instrument now prints `EXCLUDED` for these and the alignment is done
  by hand against the export in the `qor_pp_useswinapi` line.
- **The SDK sometimes declares a parameter with no name at all** (`SetDoubleClickTime(_In_ UINT)`,
  `RegisterClassExW(_In_ CONST WNDCLASSEXW *)`, `GetMenuInfo(_In_ HMENU, _Inout_ LPMENUINFO)`). Six library members sit on
  that; there is nothing to align to, so a name-diff instrument must report them separately (`SDK-UNNAMED` rows) rather
  than as divergences.
- **Renaming a parameter is two edits, not one.** The header declaration and the definition's parameter list *and its body
  uses* all carry the name; renaming only the declaration leaves a definition that no longer matches, which compiles but
  re-introduces the divergence the scan is meant to remove (`tmp/_tmp_t8_rename.ps1` therefore edits both, scoped to one
  member's extent so a common name like `uFlags` in other declarations is untouched, and uses `-creplace` so `hmenu` ->
  `hMenu` cannot touch the type `HMENU`).
