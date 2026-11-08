---
Type: Context
Description: "Instrument blind spots - what a flag named in tooling.md does not prove"
---

Moved verbatim out of `gotchas.md` on 4 Nov 2026 by `tmp/_tmp_gotchas_split.ps1` - snapshot `tmp/_tmp_gotchas_presplit.md`, verbatim rejoin proven by `tmp/_tmp_gotchas_verify.ps1`.

## Instrument caveats - read a flag before acting on it
- **`_tmp_decl_no_def_repo_scan.ps1` cannot see a declaration that ends `) const;`** - its regex anchors on `);`. Measured
  8 Nov 2026 on the D26 pass: it reads **52** declared-and-undefined names while `_tmp_static_census.ps1` prints **86**
  non-static ones, and the 34-name difference is exactly the DDE block at `user32.h:507-540`, whose 34 declaration lines end
  `) const;` (the header has exactly 34 such lines). **52 + 34 = 86** - one population, two instruments. Quote the instrument
  with the count; the record's older 97 / 64 / 65 readings are the same population under different instruments and an older
  header, not three different defects.
- **`Where-Object { _ -match ... }` is not an implicit-`_` form in pwsh** - the block runs `_` as a command and the scan dies
  with `The term '_' is not recognized` thousands of times **while still printing its totals line**. Measured 8 Nov 2026: that
  run claimed `exact export: 0 / no export row: 86` for names that all carry export rows. Use the array operator instead -
  `@( $E -match ('^\s*\d+\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f]+\s+' + $name + '\s*$') )` and test its `.Count` - which returned the
  true 80 / 5 / 1 split. A totals line beside error lines is not a clean scan (same class as the `-Stems` caveat above).
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
- **A first-hit name search reports the USE site, not the DEFINITION - so it cannot answer "where is this type
  declared".** Measured 6 Nov 2026 on the T8 6d pre-flags: the claim that `DISPLAYCONFIG_PATH_INFO` /
  `DISPLAYCONFIG_MODE_INFO` / `DISPLAYCONFIG_TOPOLOGY_ID` are declared in `winuser.h` at `:13213`, `:13215`, `:13229` came
  from a first-hit scan, and all three lines are parameter slots inside the `SetDisplayConfig` / `QueryDisplayConfig`
  prototypes. The types are defined in `wingdi.h` (`:2931`, `:3005`, `:3022`), together with the `QDC_*` / `SDC_*` groups
  the `flags` parameters take (`:3330`, `:3341`) - which is what makes the family's `_WINGDI_` / `NOGDI` guard pair
  legible. Ask the question with a definition pattern (`typedef[^;]*\bType\b`) over every `um` + `shared` header
  (`tmp/_tmp_t8_6d_types.ps1`, 1,798 files). The same run also settled a documentation claim: **`windisp.h` does not exist
  in this SDK** even though the online page for these functions names it.
- **The SDK writes some prototypes as `Name (`, with a space before the parenthesis.** Measured 6 Nov 2026 on the T8 6e
  pre-scan: `winuser.h:4009` `RegisterSuspendResumeNotification (` and `:4017` `UnregisterSuspendResumeNotification (`. A
  `'\b' + $name + '\('` pattern therefore reports **no declaration at all** for a name that is exported and declared - the
  first run found 0 hits for both and the tranche looked out of scope. Every name-matching pattern in a pre-scan must be
  `\bName\s*\(` (the instruments `tmp/_tmp_t8_6e_pre.ps1` and its clones now are).
- **A citation auditor that resolves a bare `:NNNN` to the header named nearest above it reports true numbers as
  OUT-OF-RANGE.** Measured 7 Nov 2026 on the T8 6d banner (`tmp/_tmp_t8_6d_selfcite.ps1`): the audit tracks one
  running header per line and tests `wingdi.h` **before** `winuser.h`, so a line like `(block closes :16083)`
  whose nearest header mention above is `wingdi.h` gets checked against a 6,493-line file and fails - while
  `winuser.h` has 16,475 lines. Four rows were flagged and every number in them was correct. The fix belongs in
  the cited text, not the instrument: put the header on the same line as the number (`(winuser.h block closes
  :16083)`, `(winuser.h:13213, :13215)`), after which the audit prints **0 OUT-OF-RANGE over all 55 citations**
  and quotes the SDK line for each one (`:16083` -> `#endif /* WINVER >= 0x0602 */`, `:13240` ->
  `DisplayConfigSetDeviceInfo(`, `:13213`/`:13215` -> the two `_In_reads_opt_` array parameters).
- **A `define X` scan over the tree can match the prose of the very TU it is checking.** Measured 7 Nov 2026 on
  the same run: the 6d banner claims 0 in-tree sources define `NOGDI`, and the scan over 2,855 `src` files
  reported exactly **1** hit - `displayconfig.cpp:40`, the sentence "a consumer that defines NOGDI would not
  compile this header". A claim of 0 is therefore only true once the file under audit is excluded from its own
  scan; otherwise the count lies by exactly that self-match.
- **A scan whose per-hit work re-slices the whole text is quadratic, and its output grows with it.** Measured 7 Nov 2026:
  `_tmp_t8_6f_sdk.ps1` looped `$T.Substring($pos)` over `WinUser.h` as one `ReadAllText` string and recomputed the line number
  with `($T.Substring(0,$abs) -split "\n").Count` per hit; `_tmp_t8_6f_ppdig2.ps1` read the 6.9 MB `cl /EP` text with
  `ReadAllLines` and walked every line - the SAL-annotated ones are huge - with `AllMatches`. Both ran past their output files
  (688 MB and 415 MB at 2,157 s and 11,378 s CPU) and had to be killed. Working clones: `WinUser.h` as a **line array** with
  fixed windows around a known line (`_tmp_t8_6f_sdk2.ps1`, 8.8 KB of output) and **`Select-String` streaming** for the digest
  (`_tmp_t8_6f_ppdig3.ps1`), which bound memory and finished in seconds. A scan that prints one row per hit must not do
  whole-file work per hit.
- **A case-insensitive `[AW]$` test calls any name ending in lowercase `a`/`w` an A/W form.** The 6f digest printed
  `AW: GetClipboardMetadata` (the name ends `a`) beside the one real `AW: ChangeMenuW`, so a tranche's A/W-sibling count read
  one too many. Use `-cmatch 'A$|W$'` when the suffix decides a count - same class as the `-cmatch '^(.+)T$'` stem caveat above.

