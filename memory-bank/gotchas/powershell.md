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
- **A pwsh `[regex]::Matches()` match object has no usable `.Text`** - `$names += $m.Text` appends an empty string, so a
  folder rsp built from a CMake source list comes out with the right line **count** and empty paths (the tails are the bare
  directory). `9 Nov 2026`, `_tmp_t9_9c_folder_mk.ps1`: `sources=50` printed correctly while `has_rawinput=0` and every
  tail was `...\api_layer\user\`. Use `.Value` or `$m.Groups.Item(0)`, or skip the regex and tokenise with
  `-replace '\)',' ' -split '\s+'`. A count that looks right and content that is empty is the signature of this bug -
  always print the tail of what you built, not just its length.
- **Bare `true`/`false` are not value literals on an assignment RHS** - `$started = false` fails with
  `The term 'false' is not recognized as the name of a cmdlet`; write `$true`/`$false` (`_tmp_t9_9c_folder_mk.ps1:5`/`:7`).
- **`+` is left-associative, so an int on the left coerces the right operand** - `($n) + '|' + $sk[$n-1]` in a line dump
  fails with `Cannot convert value "|" to type "System.Int32"` once per dumped line and prints nothing useful. Lead the
  concatenation with a string - `($n).ToString() + '|' + ...` - or use `-join` (`_tmp_t9_9c_sdkctx.ps1`).
- **`Select-Object -ExpandProperty Path` on a file list finds nothing** - a FileInfo has no `Path` member, so the listing
  dies with `Property "Path" cannot be found` and a repo-wide lookup silently returns no candidates. Use
  `ForEach-Object { $_.FullName }` (finding `completed_work.md`, 9 Nov 2026).
- **`Get-ChildItem`/`Get-Item` here return an EMPTY `LastModifyDate` - the compile-gate dating rule cannot be read from pwsh**
  on this tree: `Select-Object Name, Length, LastModifyDate` prints a blank column for every file, and
  `Select-Object -ExpandProperty DateTimeLastModified` dies with `Property "DateTimeLastModified" cannot be found`, so a
  `tmp/obj/<tu>.obj` newer-than-source check silently compares nothing (`-gt` on two `$null`s is false, which reads as
  "not dated" and costs a re-run). Read the dating with `cmd /c dir /T W <file>` instead, which prints the timestamp
  (`9 Nov 2026`, dating the `buttoncontrol.cpp` gate: source 17:14, `tmp/obj/buttoncontrol.obj` 17:17).