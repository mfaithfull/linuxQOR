#Requires -Version 7.0
<#
.SYNOPSIS
  wiki-ref-check - does every pointer resolve, and is every pointer still the tree's?
.DESCRIPTION
  Subcommands: anchors | refs | orphans | drift.
  The pointer layer only. This answers "every pointer resolves"; wiki-reconcile answers "what is
  compiled still matches the tree" (docs/WikiDesign.md 5). They share the parse and never share a
  verdict.
  The parse, the page listing and the anchor resolver come from the shared shape
  (tools/wiki-census/wiki-shape.ps1), dot-sourced below - one shape, five instruments.
  Required args: -WikiRoot, -RawRoot, -TreeRoot, -Report. No default encodes a task
  (docs/agent/TOOL-FORMAT.md). -Help prints the subcommands and touches no file.
#>
[CmdletBinding()]
param(
    [string]$Command,
    [string]$WikiRoot,
    [string]$RawRoot,
    [string]$TreeRoot,
    [string]$Report,
    [switch]$Help
)

$_shape = Join-Path (Split-Path -Path $PSScriptRoot -Parent) 'wiki-census/wiki-shape.ps1'
if (-not (Test-Path -LiteralPath $_shape -PathType Leaf)) {
    Write-Output 'USAGE wiki-ref-check.ps1 -Command <anchors|refs|orphans|drift> -WikiRoot <dir> -RawRoot <dir> -TreeRoot <dir> -Report <file>   (-Help prints this)'
    Write-Output ('MISSING args=shared-shape (no wiki-shape.ps1 at ' + ($_shape -replace '\\', '/') + ')')
    Write-Output 'VERDICT fail=1'
    exit 1
}
. $_shape

# ---------------------------------------------------------------- anchors
function Invoke-RefAnchor {
    param($WikiFull, $TreeFull)
    $lines = @()
    $fail = 0
    $set = Get-WikiPageSet -WikiRoot $WikiFull
    $pages = @($set.Pages)
    $resolved = 0
    $total = 0
    $derived = 0
    $deadItems = @()
    $unpinnedItems = @()
    $tmpItems = @()
    $byKind = @{ 'in-tree' = 0; 'out-of-tree' = 0; 'event' = 0; 'artifact' = 0; 'unparsed' = 0 }
    foreach ($page in $pages) {
        foreach ($anchor in @(Get-WikiAnchorRecord -Fields $page.Fields)) {
            $total = $total + 1
            if ($byKind.ContainsKey($anchor.Kind)) { $byKind[$anchor.Kind] = $byKind[$anchor.Kind] + 1 }
            else { $byKind['unparsed'] = $byKind['unparsed'] + 1 }
            $result = Resolve-WikiAnchor -Anchor $anchor -TreeRoot $TreeFull
            $label = $page.Rel + ' -> ' + $anchor.Path + $anchor.Anchor
            switch ($result.Status) {
                'resolved' { $resolved = $resolved + 1 }
                'derived_only' { $resolved = $resolved + 1; $derived = $derived + 1 }
                'dead' { $fail = $fail + 1; $deadItems += ('  DEAD ' + $label + ' : ' + $result.Reason) }
                'tmp' { $fail = $fail + 1; $tmpItems += ('  TMP ' + $label + ' : ' + $result.Reason) }
                'unpinned' { $fail = $fail + 1; $unpinnedItems += ('  UNPINNED ' + $label + ' : ' + $result.Reason) }
                default { $fail = $fail + 1; $deadItems += ('  UNKNOWN ' + $label + ' : ' + $result.Reason) }
            }
        }
    }
    $lines += ('pages = ' + $pages.Count + ' of ' + $set.All.Count + '  (files that carry the WikiPage shape)')
    $lines += ('anchors = ' + $resolved + ' of ' + $total + '  (Anchors: entries that re-resolve against the tree)')
    $lines += ('dead = ' + $deadItems.Count + '  (path, symbol or commit that no longer resolves)')
    $lines += ('unpinned = ' + $unpinnedItems.Count + '  (out-of-tree with no pin or verified date: F11)')
    $lines += ('tmp_refs = ' + $tmpItems.Count + '  (an anchor into tmp/ is a contract violation: 4.3 rule 3)')
    $lines += ('derived_only = ' + $derived + '  (line-anchor only - legal, never the only anchor: 4.3 rule 1)')
    $lines += ('in_tree=' + $byKind['in-tree'] + ' out_tree=' + $byKind['out-of-tree'] +
               ' event=' + $byKind['event'] + ' artifact=' + $byKind['artifact'] +
               ' unparsed=' + $byKind['unparsed'] + '  (nothing is fetched: reconcile stays free, 9(2))')
    $lines += $deadItems
    $lines += $unpinnedItems
    $lines += $tmpItems
    if ($pages.Count -eq 0) {
        $lines += 'vacuity: pages=0 - anchors = 0 of 0 here proves the parse ran, not that the wiki is clean (9)' }
    return @{ Lines = $lines; Fail = $fail }
}

# ---------------------------------------------------------------- orphans
function Invoke-RefOrphan {
    param($WikiFull, $RawFull)
    $lines = @()
    $queueFile = Join-Path $RawFull 'pending.md'
    $entries = @(Get-WikiQueueEntry -QueueFile $queueFile)
    $set = Get-WikiPageSet -WikiRoot $WikiFull
    $pages = @($set.Pages)
    $sources = @{}
    foreach ($page in $pages) {
        foreach ($source in @(Get-WikiFieldList -Fields $page.Fields -Key 'Sources')) {
            if ($source -ne '') { $sources[[string]$source] = 1 } } }
    $orphans = @()
    $carried = @()
    foreach ($entry in $entries) {
        $label = [string]$entry['Id'] + ' : ' + [string]$entry['path'] + [string]$entry['anchor'] +
                 ' (line ' + $entry['LineNumber'] + ', target ' + [string]$entry['target'] + ')'
        if ($sources.ContainsKey([string]$entry['Id'])) { $carried += ('  CARRIED-NOT-DELETED ' + $label) }
        else { $orphans += ('  ORPHAN ' + $label) } }
    $lines += ('pages = ' + $pages.Count + '  (the Sources: of these decide whether an entry is carried)')
    $lines += ('queue_entries = ' + $entries.Count + '  (entries still in the queue - ingest deletes an entry once carried, 4.4)')
    $lines += ('orphans = ' + $orphans.Count + ' of ' + $entries.Count +
               '  (a queued anchor no page holds yet: open work, and the legal state of a pending queue)')
    $lines += ('carried_not_deleted = ' + $carried.Count +
               '  (a page already cites this id but the entry survives: the leak that makes carry verifiable)')
    $lines += $orphans
    $lines += $carried
    if ($entries.Count -eq 0) {
        $lines += 'vacuity: pages=0 and an empty queue - orphans = 0 of 0 here proves the parse ran, not that carry is verifiable (9)' }
    return @{ Lines = $lines; Fail = $carried.Count }
}

# ---------------------------------------------------------------- drift
function Invoke-RefDrift {
    param($WikiFull, $RawFull, $TreeFull)
    $lines = @()
    $checks = @()
    # A pin carries the version the reference implies (4.3 rule 4). Drift is that pin being older
    # than the tree's current head for the same path - the anchor resolves and is still wrong.
    $queueFile = Join-Path $RawFull 'pending.md'
    foreach ($entry in @(Get-WikiQueueEntry -QueueFile $queueFile)) {
        $checks += , @{ Label = ('queue ' + [string]$entry['Id'])
                        Path = [string]$entry['path']; Pin = [string]$entry['pin'] } }
    $set = Get-WikiPageSet -WikiRoot $WikiFull
    foreach ($page in @($set.Pages)) {
        foreach ($anchor in @(Get-WikiAnchorRecord -Fields $page.Fields)) {
            if ($anchor.Kind -ne 'in-tree') { continue }
            $checks += , @{ Label = ('page ' + $page.Rel); Path = $anchor.Path; Pin = $anchor.Pin } } }
    $total = 0
    $drifted = @()
    $pinned = 0
    $noHead = 0
    foreach ($check in $checks) {
        $total = $total + 1
        $path = [string]$check['Path']
        if ($path -eq '' -or $path -match '^https?:') { continue }
        $pin = ([string]$check['Pin']).Trim()
        if ($pin -eq '') { continue }
        $pinned = $pinned + 1
        $head = Get-WikiPathLastCommit -TreeRoot $TreeFull -RelPath $path
        if ($head -eq '') { $noHead = $noHead + 1; continue }
        $headDate = ($head -split ',')[0]
        $headSha = ($head -split ',')[-1]
        $pinDate = Get-WikiDateOnly -Text ($pin -replace '^.*?(\d{4}-\d{2}-\d{2}).*$', '$1')
        $pinShaMatch = ($pin -match ('(^|[^0-9a-f])' + [regex]::Escape($headSha) + '($|[^0-9a-f])'))
        if ($pinShaMatch) { continue }
        if ($null -ne $pinDate) {
            $treeDate = Get-WikiDateOnly -Text $headDate
            if ($null -ne $treeDate -and $pinDate -lt $treeDate) {
                $drifted += ('  DRIFT ' + $check['Label'] + ' : ' + $path + ' pinned ' + $pinDate.ToString('yyyy-MM-dd') +
                             ' < tree head ' + $headDate + ' (' + $headSha + ')')
                continue }
            if ($null -ne $treeDate -and $pinDate -eq $treeDate) { continue } }
        $drifted += ('  DRIFT ' + $check['Label'] + ' : ' + $path + ' pin does not name the tree head ' + $headDate + ' (' + $headSha + ')')
    }
    $lines += ('checks = ' + $total + '  (queue pins and in-tree page anchors)')
    $lines += ('pinned = ' + $pinned + '  (of those, the ones that carry a pin to compare)')
    $lines += ('no_head = ' + $noHead + '  (a pinned path git has no commit for: untracked or gone)')
    $lines += ('drift = ' + $drifted.Count + ' of ' + $pinned +
               '  (pin older than the tree head for that path; an unpinned in-tree anchor is legal)')
    $lines += $drifted
    if ($total -eq 0) {
        $lines += 'vacuity: pages=0 and an empty queue - drift = 0 of 0 here proves the parse ran, not that pins are fresh (9)' }
    return @{ Lines = $lines; Fail = $drifted.Count }
}

# ---------------------------------------------------------------- refs
function Invoke-RefLink {
    param($WikiFull)
    $lines = @()
    $set = Get-WikiPageSet -WikiRoot $WikiFull
    $pages = @($set.Pages)
    $links = @()
    foreach ($page in $pages) {
        $links += @(Get-WikiWikilink -Page $page)
        if ($null -ne $page.Fields -and $page.Fields.Contains('SupersededBy')) {
            $target = [string]$page.Fields['SupersededBy']
            if ($target -ne '' -and $target -cne 'null') {
                $links += [pscustomobject]@{ Page = $page.Rel; Form = 'front-matter'; Target = $target } } } }
    $resolved = 0
    $brokenItems = @()
    foreach ($link in $links) {
        $hit = Resolve-WikiWikilink -WikiRoot $WikiFull -Link $link
        if ($hit -ne '') { $resolved = $resolved + 1 }
        else { $brokenItems += ('  BROKEN ' + $link.Page + ' [' + $link.Form + '] -> ' + $link.Target) } }
    $lines += ('pages = ' + $pages.Count + ' of ' + $set.All.Count)
    $lines += ('links = ' + $resolved + ' of ' + $links.Count +
               '  ([[target]], [[target|text]], markdown .md links and SupersededBy)')
    $lines += ('broken = ' + $brokenItems.Count)
    $lines += $brokenItems
    if ($pages.Count -eq 0) {
        $lines += 'vacuity: pages=0 - links = 0 of 0 here proves the parse ran, not that the wiki is clean (9)' }
    return @{ Lines = $lines; Fail = $brokenItems.Count }
}

# ---------------------------------------------------------------- dispatcher
$subcommands = @('anchors', 'refs', 'orphans', 'drift')
$usage = 'wiki-ref-check.ps1 -Command <anchors|refs|orphans|drift> -WikiRoot <dir> -RawRoot <dir> -TreeRoot <dir> -Report <file>   (-Help prints this)'
$argLines = @('  required: -WikiRoot, -RawRoot, -TreeRoot and -Report on every subcommand (4.3: an anchor is',
              '  re-resolved against the tree, so the tree root is never a default); -Report is the file',
              '  the run writes, and tmp/ is the disposable place for it.')
if ($Help) {
    Exit-WikiHelp -ToolName 'wiki-ref-check' -Subcommands $subcommands -Usage $usage -ArgLines $argLines }
if ([string]::IsNullOrWhiteSpace($Command)) {
    Exit-WikiUsage -Usage $usage -Missing 'Command' -ExtraLines @('  subcommand: ' + ($subcommands -join ' | ')) }
if ([string]::IsNullOrWhiteSpace($Report)) { Exit-WikiUsage -Usage $usage -Missing 'Report' }
# Every root is required by every subcommand, and each is tested before it is resolved: passing ''
# to a Mandatory [string] parameter throws and lets the script run on with a $null root.
if ([string]::IsNullOrWhiteSpace($WikiRoot)) { Exit-WikiUsage -Usage $usage -Missing 'WikiRoot' }
if ([string]::IsNullOrWhiteSpace($RawRoot)) { Exit-WikiUsage -Usage $usage -Missing 'RawRoot' }
if ([string]::IsNullOrWhiteSpace($TreeRoot)) { Exit-WikiUsage -Usage $usage -Missing 'TreeRoot' }
$wikiFull = Resolve-WikiRequiredRoot -Name 'WikiRoot' -Value $WikiRoot -Usage $usage
$rawFull = Resolve-WikiRequiredRoot -Name 'RawRoot' -Value $RawRoot -Usage $usage
$treeFull = Resolve-WikiRequiredRoot -Name 'TreeRoot' -Value $TreeRoot -Usage $usage
switch -CaseSensitive ($Command) {
    'anchors' { $run = Invoke-RefAnchor -WikiFull $wikiFull -TreeFull $treeFull }
    'refs' { $run = Invoke-RefLink -WikiFull $wikiFull }
    'orphans' { $run = Invoke-RefOrphan -WikiFull $wikiFull -RawFull $rawFull }
    'drift' { $run = Invoke-RefDrift -WikiFull $wikiFull -RawFull $rawFull -TreeFull $treeFull }
    default { Exit-WikiUsage -Usage $usage -Missing ('Command (unknown: ' + $Command + ')') }
}
if ($null -eq $run) {
    Exit-WikiUsage -Usage $usage -Missing ('internal (subcommand ' + $Command + ' returned no result)') }
$out = @()
$out += ('wiki-ref-check command=' + $Command + '  shape=' + $script:WikiShapeVersion)
$out += ('wiki_root = ' + (Get-WikiRelPath -Path $wikiFull) + '  raw_root = ' + (Get-WikiRelPath -Path $rawFull) + '  tree_root = ' + (Get-WikiRelPath -Path $treeFull))
$out += $run.Lines
$out += ('VERDICT fail=' + $run.Fail)
Write-WikiRun -ReportPath $Report -Lines $out
if ($run.Fail -gt 0) { exit 1 }
exit 0

