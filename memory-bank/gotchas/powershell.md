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
- **`Get-ChildItem -Include <pattern>` is silently dropped when `-Path` is a plain directory - the scan returns NOTHING**
  `Get-ChildItem -Path $dir -Include *.cpp -File` over the 51-source `api_layer/user` folder counts **0** and raises no error,
  so a per-folder lookup reads "this TU defines none of the five neighbours" and the section-boundary argument it feeds is
  unfalsified. Put the wildcard in `-Path` (`Get-ChildItem -Path (Join-Path $dir '*.cpp') -File` -> 52) or add `-Recurse`
  (`-Recurse -Depth 0` with `-Include` -> 52); both return the same tails. Measured by `tmp/_tmp_t9_9e_gchild.ps1`
  (`tmp/_tmp_t9_9e_gchild.txt`: A=0, B=52, C=52, F=61 files and G=3 dirs over the 64 entries the bare `-Path` lists).
  The bug that hid it: `_tmp_t9_9e_guards.ps1:46` printed `folder_sources=0` and `DEFINES ... count=0` for all seven names
  over a 51-TU folder, and the same scan done right (`_tmp_t9_9e_definers.ps1`) read `folder_sources=51` with 1 hit per
  name - a 0 on a folder you know is populated is this bug, not a finding. `-Include` only filters when `-Path` carries a
  wildcard or `-Recurse` is on.
- **`For-Each` is not a cmdlet - the pipe verb is `ForEach-Object`, and a hyphen inside the verb name parses as a command**
  `Select-String ... | For-Each { $_.LineNumber.ToString() + '|' + $_.Line.Trim() }` fails with
  `For-Each: The term 'For-Each' is not recognized as a name of a cmdlet, function, script file, or executable program`
  (exit 1, and the suggestion it offers is `foreach`, the STATEMENT keyword, not the pipe verb). Write
  `ForEach-Object`, or the `%` alias. Hit twice while reading the SDK's `LoadBitmap` context out of
  `um\WinUser.h` at the 9f close (9 Nov 2026), and the cost is one re-run of the same `Select-String` - the search
  itself is fine, only the formatting pipe stage dies.
- **`editor` with `insert_line` does not create a missing file, and a big payload can fail as unparseable JSON**
  An `editor` call that carries `insert_line` against a path that does not exist dies with
  `ENOENT: no such file or directory` - creation needs `new_text` with NO `insert_line`; the insert form only edits an
  existing file. Separately, one very large `new_text` (a whole ~100-line TU banner in a single call) came back as
  `Tool call arguments could not be parsed as JSON`. The working shape is: create with the first chunk, then extend with
  `old_text`/`new_text` pairs at a known tail, a few dozen lines at a time, and read the file back (`read_files` on the
  line range) rather than trusting the `File created successfully` line - a chunked write can silently drop its tail.
  Measured landing `bitmap.cpp` at the 9f close (9 Nov 2026): the create-then-extend sequence and a read-back of
  `:96-136` confirming the banner, the includes and the one definition all landed.
- **`Get-ChildItem -Filter` takes ONE string - a comma list is an array and the call dies, silently scoping the scan to NOTHING**
  `Get-ChildItem -Path $tmp -Filter '*wrong_stem_scan*','*bypass_scan*','*sdk_guard_scan*','*user32_exports*'` fails with
  `Cannot convert 'System.Object[]' to the type 'System.String' required by parameter 'Filter'. Specified method is not
  supported.` (exit 1), and the temptation on a failed lookup is to conclude "those artefacts do not exist". `-Filter` accepts
  a single pattern only; for several names either repeat the call, put the alternatives in one pattern (`'*t9_9f*(scan|exports)*'`)
  or filter afterwards on `$_.Name`. Hit once while listing the 9f artefacts before writing the 9g instruments (9 Nov 2026);
  `cmd /c dir /T W <file1> <file2>` is the alternative that also prints the timestamps the dating rule needs.
- **A command-subscripted path in a command-argument position is parsed as an array, not as one string**
  `cmd /c 'dir /T W c:\...\a.ps1 c:\...\b.ps1'` works, but `(Get-Content -LiteralPath 'x')[23..31] | ForEach-Object { ... }`
  passed as the FIRST element of the tool's `commands` array came back `ParserError: Missing type name after '['` - the range
  subscript at the start of a command line is not a valid statement head. Lead the statement with an assignment
  (`$all = Get-Content ...; $all[23..31]`) or with a cmdlet (`Get-Content ... | Select-Object -Skip 23 -First 9`). Same class
  as the earlier `For-Each` typo: the parse dies before the scan runs, so the "empty result" you see next is not a finding
  (9 Nov 2026, reading `memory-bank/activeContext.md` at the 9g close).
- **A nested `Where-Object` shadows the outer `$_`, and a never-assigned `$l` makes the set difference read "everything differs"**
  The standing all-libraries gate diff (`_tmp_t9_9g_apialldiff.ps1`, cloned into `_tmp_t9_9h_apialldiff.ps1`) computes
  "diagnostics only in post" as `@($post | Where-Object { ($_ -match 'warning C|error C') -and (@($pre | Where-Object { $_ -eq $l }).Count -eq 0) })`:
  the inner `$_` is the INNER item, and `$l` is never assigned in that expression - it is whatever an earlier
  `foreach($l in ...)` loop last left in it. On the 9h close, whose pre and post logs are **byte-identical** (SHA-256
  equal), that diff printed `only_in_post_count=166` and `only_in_post` = every diagnostic, plus the mirror 166 for
  pre. A full-count difference on logs you can hash-equal is this bug, not a finding. The working shape is
  `_tmp_t9_9h_apialldiff2.ps1`: pre-count a name-keyed hashtable, then walk post decrementing it and collect leftovers,
  then walk pre over the leftovers - a real multiset difference with explicit variables, which read **0/0**.
- **A pwsh `>` redirect writes UTF-16 on this tree, so `Select-String -Path <that file>` reads 0 matches on a dump you just wrote**
  `dumpbin /exports C:\Windows\System32\user32.dll > tmp\_tmp_t9_9h_exports_fresh.txt 2>&1` then
  `Select-String -Path <that file> -Pattern 'Dde'` returns **0**, and so do `ordinal` and `DDE` - while
  `Get-Content <that file> | Select-String -CaseSensitive 'Dde'` returns the 38 rows. The pipe decodes the BOM; the
  `-Path` form does not. A 0 from `-Path` on a file you know you just wrote is this bug; read dumps through
  `Get-Content`, or write them with `cmd /c ... >` or `| Out-File -Encoding utf8`. Cost one re-measure of the DDE export
  dump at the 9h close (9 Nov 2026).

