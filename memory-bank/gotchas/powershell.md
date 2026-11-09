---
Type: Context
---

## Editing these records (and any long text file) from PowerShell
- **PowerShell `-replace` is case-INsensitive: `-replace 'T$'` eats a trailing `t`, not only the folder's `T` suffix**
  (session 26). `_tmp_paint_pre_s26.ps1` stripped the "SDK stem" with `$name -replace 'T$',''`, which silently produced
  `BeginPain`, `EndPain`, `GetUpdateRec`, `DrawFocusRec`, `InvalidateRec`, `ValidateRec` - six prototypes that exist
  nowhere - and the TU looked like it had 6 SDK names unmatched (`sdk=0` rows) plus 6 phantom parameter-name
  divergences. Use `-creplace 'T$',''` (or an explicit `-cmatch 'T$'` guard) whenever a suffix strip is case-significant,
  and treat an unexpected `0`/`not found` count as a bug in the instrument until a direct grep proves otherwise. The same
  pattern in `_tmp_sdk_param_name_scan.ps1:242` is harmless only because that script tries the unstripped stem first and
  keeps the stripped arm last (`:238`-`:241`), so a false strip just adds candidates.
- **Don't use single character variable names**
  Single character variable names risk clashes with builtin PowerShell aliases like $h
- **An `if` is a statement, not an expression: it cannot sit inside a string concatenation**
  `Write-Output ('first_quote_at=' + $at + '  prev_char=' + (if($at -gt 0){ $s[$at-1] }else{ '' }))` fails with
  `The term 'if' is not recognized as a name of a cmdlet` - the parser treats the `(` as the start of a command. Pre-compute
  the value into a variable, or use the ternary `? :` (9 Nov 2026, `_tmp_t9_9b_probe.ps1:14`, which lost the probe's whole
  output line to that parse error). Same class of bug as the `$want = {}` hashtable initialiser, which makes `$want[$n] = 1`
  an "index into a string" and silently corrupts the counts: initialise hashtable-valued lookups with `@{}`, never `{}`.