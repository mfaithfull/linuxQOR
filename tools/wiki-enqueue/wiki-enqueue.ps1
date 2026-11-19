#Requires -Version 7.0
<#
.SYNOPSIS
  wiki-enqueue - the post-commit hook's queue half of ingest-commit.
.DESCRIPTION
  Queue, do not act (docs/WikiDesign.md 9(3)): one commit in, one entry per touched QOR target
  appended to docs/raw/pending.md with added_by: hook; wiki-ingest is the only consumer.
  The exclusion guard (9(3b), 13.1(3), F9) drops prefixes whose commits are wiki/plan churn -
  docs/wiki/, docs/raw/, docs/future/, docs/archive/ - so the ingest's own commit and any
  plan-tree move cannot re-enqueue themselves. A commit whose subject starts 'wiki:' enqueues
  nothing for the same reason.
  Parameters are deliberately NOT [Mandatory]: requiredness is enforced by the dispatcher,
  which prints USAGE + MISSING args= + VERDICT fail=1 + exit 1 without touching a file
  (docs/agent/TOOL-FORMAT.md, Contract). Reports go to the path given by -Report; tmp/ is the
  disposable place for them.
#>
[CmdletBinding()]
param(
    [string]$Commit,
    [string]$TreeRoot,
    [string]$RawRoot,
    [string]$Report,
    [string]$Excludes,
    [switch]$DryRun,
    [switch]$Help
)

$_shape = Join-Path (Split-Path -Path $PSScriptRoot -Parent) 'wiki-census/wiki-shape.ps1'
if (-not (Test-Path -LiteralPath $_shape -PathType Leaf)) {
    Write-Output 'USAGE wiki-enqueue.ps1 [-Commit <git-ref>] -TreeRoot <dir> -RawRoot <dir> -Report <file> [-Excludes "p1,p2,..."] [-DryRun]   (-Help prints this)'
    Write-Output ('MISSING args=shared-shape (no wiki-shape.ps1 at ' + ($_shape -replace '\\', '/') + ')')
    Write-Output 'VERDICT fail=1'
    exit 1
}
. $_shape

# --------------------------------------------------------------- the exclusion guard
# The prefixes whose churn is not code churn. A commit under any of these enqueues nothing,
# which is what keeps pending=0 reachable (13.1(3): without the exclusion the wiki commit
# re-enqueues forever and the design has a runaway instead of a cost).
$script:WikiEnqueueDefaultExcludes = @('docs/wiki/', 'docs/raw/', 'docs/future/', 'docs/archive/')

function Get-WikiEnqueueDiff {
    # One commit's changed files, slash-relative. -r without -c/-m names nothing for a merge,
    # so the caller reads Merge from the parent count rather than mistaking silence for zero churn.
    param([Parameter(Mandatory = $true)][string]$TreeFull,
          [Parameter(Mandatory = $true)][string]$Ref)
    $verify = @(git -C $TreeFull rev-parse --verify ($Ref + '^{commit}') 2>&1)
    if ($LASTEXITCODE -ne 0) {
        return @{ Ok = $false; Sha = ''; Subject = ''; Files = @(); Merge = $false
                  Reason = ((@($verify) | Select-Object -First 1) -join ' ') } }
    $sha = ((@(git -C $TreeFull rev-parse $Ref) | Select-Object -First 1) -join '').Trim()
    $subject = ((@(git -C $TreeFull log -1 --format=%s $Ref) | Select-Object -First 1) -join '').Trim()
    $parents = @((git -C $TreeFull rev-list --parents -n 1 $Ref) -split '\s') | Where-Object { $_ -ne '' }
    $merge = ($parents.Count -gt 2)
    $files = @()
    $diff = @(git -C $TreeFull diff-tree --no-commit-id --name-only -r $Ref 2>&1)
    if ($LASTEXITCODE -ne 0) {
        return @{ Ok = $false; Sha = $sha; Subject = $subject; Files = @(); Merge = $merge
                  Reason = ((@($diff) | Select-Object -First 1) -join ' ') } }
    foreach ($line in $diff) {
        $clean = (([string]$line) -replace '\\', '/').Trim()
        if ($clean -ne '') { $files += $clean } }
    return @{ Ok = $true; Sha = $sha; Subject = $subject; Files = $files; Merge = $merge; Reason = '' }
}

function Split-WikiEnqueueFiles {
    # excluded: the path starts with an excluded prefix; candidates: everything else.
    param([string[]]$Files, [string[]]$ExcludedPrefixes)
    $excluded = @{}
    foreach ($prefix in $ExcludedPrefixes) { $excluded[$prefix] = 0 }
    $otherExcluded = @()
    $candidates = @()
    foreach ($file in $Files) {
        $hit = ''
        foreach ($prefix in $ExcludedPrefixes) {
            if ($file.StartsWith($prefix, [StringComparison]::OrdinalIgnoreCase)) { $hit = $prefix; break } }
        if ($hit -ne '') {
            $excluded[$hit] = $excluded[$hit] + 1
            continue }
        $candidates += $file }
    return @{ Excluded = $excluded; Candidates = $candidates }
}

function Get-WikiTargetForFile {
    # The nearest set(target ...) declaration wins: a file belongs to the CMakeLists.txt that
    # owns its directory (or its closest owning ancestor). Returns '' when nothing owns the
    # file - a counted case (unmapped_files), never a silently enqueued none.
    param([string]$File, [object[]]$Decls)
    $best = ''
    $bestName = ''
    foreach ($decl in $Decls) {
        $dir = (Split-Path -Path ([string]$decl.Abs) -Parent).TrimEnd('\') + '\'
        $abs = $File.Replace('/', '\')
        if ($abs.StartsWith($dir, [StringComparison]::OrdinalIgnoreCase)) {
            if ($dir.Length -gt $best.Length) { $best = $dir; $bestName = [string]$decl.Name } } }
    return $bestName
}

# ---------------------------------------------------------------- dispatcher
$subcommands = @('(none - this tool has one behaviour: enqueue <Commit, default HEAD>)')
$usage = 'wiki-enqueue.ps1 [-Commit <git-ref>] -TreeRoot <dir> -RawRoot <dir> -Report <file> [-Excludes "p1,p2,..."] [-DryRun]   (-Help prints this)'
$argLines = @('  required: -TreeRoot, -RawRoot and -Report; -Commit defaults to HEAD.',
              '  <RawRoot>/pending.md must exist - the queue file is scaffolding, its absence is a usage failure.',
              '  <TreeRoot>/src is the only derived path: it is where set(target ...) declarations live (13 Q2).',
              '  -Excludes overrides the four default prefixes (docs/wiki/, docs/raw/, docs/future/, docs/archive/).',
              '  -DryRun prints the rows it would append and writes nothing.')
if ($Help) {
    Exit-WikiHelp -ToolName 'wiki-enqueue' -Subcommands $subcommands -Usage $usage -ArgLines $argLines }
if ([string]::IsNullOrWhiteSpace($Report)) { Exit-WikiUsage -Usage $usage -Missing 'Report' }
if ([string]::IsNullOrWhiteSpace($TreeRoot)) { Exit-WikiUsage -Usage $usage -Missing 'TreeRoot' }
if ([string]::IsNullOrWhiteSpace($RawRoot)) { Exit-WikiUsage -Usage $usage -Missing 'RawRoot' }
$treeFull = Resolve-WikiRequiredRoot -Name 'TreeRoot' -Value $TreeRoot -Usage $usage
$rawFull = Resolve-WikiRequiredRoot -Name 'RawRoot' -Value $RawRoot -Usage $usage
$queueFile = Join-Path $rawFull 'pending.md'
if (-not (Test-Path -LiteralPath $queueFile -PathType Leaf)) {
    Exit-WikiUsage -Usage $usage -Missing ('RawRoot/pending.md (not readable: ' + $queueFile + ')') }
if ([string]::IsNullOrWhiteSpace($Commit)) { $Commit = 'HEAD' }
$excludeList = $script:WikiEnqueueDefaultExcludes
if (-not [string]::IsNullOrWhiteSpace($Excludes)) {
    $excludeList = @($Excludes -split ',' | ForEach-Object { $_.Trim().Replace('\', '/') } | Where-Object { $_ -ne '' }) }

$lines = @()
$fail = 0
$commitRef = $Commit
$change = Get-WikiEnqueueDiff -TreeFull $treeFull -Ref $commitRef
if (-not $change.Ok) {
    $lines += ('ref = ' + $commitRef + '  (does not resolve or diff failed)')
    $lines += ('  GITFAIL ' + $change.Reason)
    $lines += ('VERDICT fail=1')
    Write-WikiRun -ReportPath $Report -Lines $lines
    exit 1 }
$short = $change.Sha.Substring(0, 10)
$lines += ('commit = ' + $short + '  subject = ' + $change.Subject)
$split = Split-WikiEnqueueFiles -Files $change.Files -ExcludedPrefixes $excludeList
$excludedTotal = 0
foreach ($prefix in $excludeList) { $excludedTotal += [int]$split.Excluded[$prefix] }
$lines += ('files = ' + $change.Files.Count + '  excluded = ' + $excludedTotal +
           '  candidates = ' + $split.Candidates.Count)
$exParts = @()
foreach ($prefix in $excludeList) { $exParts += ($prefix + '=' + [int]$split.Excluded[$prefix]) }
$lines += ('excluded_by_prefix: ' + ($exParts -join '  '))

# Loop guard, second half (13.1(3)): a wiki:-prefixed commit is maintenance output, not code churn.
$wikiCommit = ($change.Subject -imatch '^wiki:')
$queued = 0
$skipped = 0
$unmapped = @()
$rows = @()
if ($change.Files.Count -eq 0) {
    $note = 'vacuity: the diff named no files'
    if ($change.Merge) { $note = 'note: diff-tree -r names nothing for a merge; re-run with -Commit <each side> if the merge carried conflict resolutions' }
    $lines += ($note + ' - queued = 0 here is not a measured zero churn')
}
elseif ($wikiCommit) {
    $lines += 'loop_guard = subject starts wiki: - a wiki commit enqueues nothing (13.1(3), F9)' }
elseif ($split.Candidates.Count -eq 0) {
    $lines += 'exclusion_guard = every file sat under an excluded prefix - queued = 0 is the guard working, not a missed diff' }
else {
    $decls = @(Get-WikiTargetDeclaration -SrcRoot (Join-Path $treeFull 'src') |
               Where-Object { $_.IsQor })
    $byTarget = [ordered]@{}
    foreach ($file in $split.Candidates) {
        $name = Get-WikiTargetForFile -File (Join-Path $treeFull $file) -Decls $decls
        if ($name -eq '') { $unmapped += $file; continue }
        if (-not $byTarget.Contains($name)) { $byTarget[$name] = @() }
        $byTarget[$name] += $file }
    $lines += ('targets_touched = ' + $byTarget.Count + '  [' + (($byTarget.Keys | Sort-Object) -join ', ') + ']')
    if ($unmapped.Count -gt 0) {
        $lines += ('unmapped candidate files (owned by no set(target qor_*) declaration - counted, not enqueued):')
        foreach ($file in ($unmapped | Select-Object -First 10)) { $lines += ('  UNMAPPED ' + $file) }
        if ($unmapped.Count -gt 10) { $lines += ('  UNMAPPED ... ' + ($unmapped.Count - 10) + ' more') } }
    $existing = @{}
    foreach ($entry in @(Get-WikiQueueEntry -QueueFile $queueFile)) {
        $existing[(([string]$entry['kind']) + '|' + ([string]$entry['path']) + '|' + ([string]$entry['anchor']))] = 1 }
    $dateStamp = (Get-Date).ToString('yyyyMMdd')
    $verified = $dateStamp.Substring(0, 4) + '-' + $dateStamp.Substring(4, 2) + '-' + $dateStamp.Substring(6, 2)
    $queueBefore = @(Get-WikiQueueEntry -QueueFile $queueFile).Count
    foreach ($target in ($byTarget.Keys | Sort-Object)) {
        $files = @($byTarget[$target])
        $decl = @($decls | Where-Object { $_.Name -eq $target })[0]
        $path = [string]$decl.File
        $anchor = '#' + $target
        $key = 'in-tree|' + $path + '|' + $anchor
        if ($existing.ContainsKey($key)) {
            $skipped = $skipped + 1
            $lines += ('  SKIPPED ' + $target + ' : the same target is already queued')
            continue }
        $existing[$key] = 1
        $why = ('post-commit hook on ' + $short + ' touched ' + $files.Count + ' file(s): ' + ($files -join ', '))
        if ($why.Length -gt 180) { $why = $why.Substring(0, 180) }
        $rows += , @(
            ('- id: hook-' + $short + '-' + $target)
            '  added_by: hook'
            '  kind: in-tree'
            ('  path: ' + $path)
            ('  anchor: ' + $anchor)
            ('  pin: commit:' + $change.Sha)
            ('  verified: ' + $verified)
            ('  target: ' + $target)
            ('  why: ' + $why)
        )
        $lines += ('  QUEUE - id: hook-' + $short + '-' + $target + ' : ' + $target +
                   ' (anchor ' + $path + $anchor + ')')
        $queued = $queued + 1 }
    if ($queued -gt 0 -and -not $DryRun) {
        $block = @()
        foreach ($row in $rows) { $block += $row }
        Add-Content -LiteralPath $queueFile -Value $block -Encoding utf8 }
    $queueAfter = @(Get-WikiQueueEntry -QueueFile $queueFile).Count
    $landed = $queueAfter - $queueBefore
    if (-not $DryRun -and $queued -gt 0 -and $landed -ne $queued) {
        $fail = $fail + 1
        $lines += ('  FAIL: queued = ' + $queued + ' but the queue grew by ' + $landed +
                   ' - an append that did not land is never reported as one')
        $queued = $landed }
    if ($DryRun) { $queued = 0 }
}
$pending = @(Get-WikiQueueEntry -QueueFile $queueFile).Count
$dryInt = 0
if ($DryRun) { $dryInt = 1 }
$lines += ('queued = ' + $queued + '  skipped_duplicate = ' + $skipped +
           '  unmapped_files = ' + $unmapped.Count + '  dry_run = ' + $dryInt +
           '  pending = ' + $pending)
$lines += ('VERDICT fail=' + $fail)
Write-WikiRun -ReportPath $Report -Lines $lines
if ($fail -gt 0) { exit 1 }
exit 0
