#Requires -Version 7.0
<#
.SYNOPSIS
  wiki-retire - the one wiki op that moves content, so the move is stamped or it does not happen.
.DESCRIPTION
  Subcommands: retire | stamp | restore | refuse (docs/WikiDesign.md 7 op 14, 8.1 "retire").
  A superseded PAGE moves to <wiki root>/archive/ with a written reason, its evidence and the commit
  that retired it - the three legs of the F11 guard. A reference never does: docs/raw/ is a queue that
  ingest consumes, and the page that ingested a reference is that reference's archive (4.4). That
  refusal is structural here, not a convention: this script acts only inside the wiki root it is
  given, so a raw entry is not a path it can move at all.
  The parse, the page listing and the run plumbing come from the shared shape
  (tools/wiki-census/wiki-shape.ps1), dot-sourced below - one shape, six instruments.
  Required args: -WikiRoot, -Page, -Report on every subcommand; -Reason, -Evidence and -Commit
  additionally for retire and stamp. No default encodes a task (docs/agent/TOOL-FORMAT.md).
  -Help prints the subcommands and touches no file.
#>
[CmdletBinding()]
param(
    [string]$Command,
    [string]$WikiRoot,
    [string]$RawRoot,
    [string]$TreeRoot,
    [string]$Page,
    [string]$Successor,
    [string]$Reason,
    [string]$Evidence,
    [string]$Commit,
    [string]$Report,
    [switch]$Help
)

$_shape = Join-Path (Split-Path -Path $PSScriptRoot -Parent) 'wiki-census/wiki-shape.ps1'
if (-not (Test-Path -LiteralPath $_shape -PathType Leaf)) {
    Write-Output 'USAGE wiki-retire.ps1 -Command <retire|stamp|restore|refuse> -WikiRoot <dir> -Page <path> [-Successor <path>] [-Reason <text>] [-Evidence <text>] [-Commit <sha>] [-RawRoot <dir>] [-TreeRoot <dir>] -Report <file>   (-Help prints this)'
    Write-Output ('MISSING args=shared-shape (no wiki-shape.ps1 at ' + ($_shape -replace '\\', '/') + ')')
    Write-Output 'VERDICT fail=1'
    exit 1
}
. $_shape

# ---------------------------------------------------------------- the stamp
$script:RetireHeading = '## Retirement'
$script:RetireLegs = @('reason', 'evidence', 'commit')

function Get-WikiRetirementStamp {
    # The stamp is three body lines under '## Retirement', not front matter: 5.1 makes front matter
    # the machine state, and schema.md's field list has no retirement field - so a page that was
    # retired in front matter would be a schema violation the moment it was retired.
    # -> @{ Found; HeadingLine; Values; Missing (legs absent or empty) }
    param([string[]]$Lines)
    $values = @{}
    $heading = -1
    for ($i = 0; $i -lt $Lines.Count; $i++) {
        if ($heading -lt 0) {
            if ($Lines[$i] -cmatch ('^\s*' + [regex]::Escape($script:RetireHeading) + '\s*$')) { $heading = $i }
            continue }
        if ($Lines[$i] -cmatch '^#{1,6}\s') { break }
        if ($Lines[$i] -cmatch '^\s*-\s+(reason|evidence|commit|from)\s*:\s*(.*?)\s*$') {
            $val = $Matches[2]
            if ($val -ne '') { $values[$Matches[1]] = $val } }
    }
    $missing = @()
    foreach ($leg in $script:RetireLegs) {
        if (-not $values.ContainsKey($leg)) { $missing += $leg } }
    return @{ Found = ($heading -ge 0); HeadingLine = ($heading + 1); Values = $values; Missing = $missing }
}

function New-WikiRetirementBlock {
    # The written block and nothing else. 'from' is the fourth line because a restore that guessed the
    # origin folder from PageType would be lossy for a page that had already moved once.
    param([string]$ReasonText, [string]$EvidenceText, [string]$CommitSha, [string]$FromRel)
    $block = @()
    $block += ''
    $block += '<!-- Retirement stamp, written by tools/wiki-retire/wiki-retire.ps1. Do not delete this -->'
    $block += '<!-- block: reason / evidence / commit is what makes a move auditable (F11). -->'
    $block += $script:RetireHeading
    $block += ''
    $block += ('- reason: ' + $ReasonText)
    $block += ('- evidence: ' + $EvidenceText)
    $block += ('- commit: ' + $CommitSha)
    $block += ('- from: ' + $FromRel)
    return $block
}

function Set-WikiFrontMatterScalar {
    # Set one front-matter scalar in place, or add it before the closing fence when the page has
    # none. Everything else is copied through byte for byte: retiring a page rewrites its machine
    # state, never its prose. -> @{ Lines; Added; Replaced; HasFrontMatter }
    param([string[]]$Lines, [string]$Key, [string]$Value)
    $doc = Split-WikiDocument -Lines $Lines
    if (-not $doc.HasFrontMatter) {
        return @{ Lines = $Lines; Added = $false; Replaced = $false; HasFrontMatter = $false } }
    $fenceEnd = $doc.BodyStartLine - 1
    $existed = [bool]($doc.Fields.Contains($Key))
    $out = @()
    $done = $false
    for ($i = 0; $i -lt $Lines.Count; $i++) {
        if ((-not $done) -and ($i -lt $fenceEnd) -and ($Lines[$i] -cmatch ('^' + [regex]::Escape($Key) + ':(\s|$)'))) {
            $out += ($Key + ': "' + $Value + '"')
            $done = $true
            continue }
        if ((-not $done) -and ($i -eq $fenceEnd)) {
            $out += ($Key + ': "' + $Value + '"')
            $done = $true }
        $out += $Lines[$i]
    }
    return @{ Lines = $out; Added = (-not $existed); Replaced = $existed; HasFrontMatter = $true }
}


function Set-WikiLinkTarget {
    # Rewrite only the links that name the moved page's path, in either coordinate a page may write
    # it (wiki-relative, or prefixed docs/wiki/). Everything else in the file - including a [[stem]]
    # link and unrelated prose - is copied through byte for byte.
    # -> @{ Lines; Rewrites }
    param([string[]]$Lines, [string]$OldRel, [string]$NewRel)
    $pattern = '(?<![A-Za-z0-9_./])(?<pre>docs/wiki/)?' + [regex]::Escape($OldRel) + '(?![A-Za-z0-9_])'
    $out = @()
    $count = 0
    foreach ($line in $Lines) {
        if ($line -match $pattern) {
            $out += [regex]::Replace($line, $pattern, { param($m) ($m.Groups.Item('pre').Value + $NewRel) })
            $count = $count + 1
            continue }
        $out += $line }
    return @{ Lines = $out; Rewrites = $count }
}

function Remove-WikiFrontMatterScalar {
    # Drop one front-matter key. Restore needs this: a restored page that keeps SupersededBy: is a
    # page the graph still points away from. -> @{ Lines; Removed }
    param([string[]]$Lines, [string]$Key)
    $doc = Split-WikiDocument -Lines $Lines
    if (-not $doc.HasFrontMatter) { return @{ Lines = $Lines; Removed = $false } }
    $fenceEnd = $doc.BodyStartLine - 1
    $out = @()
    $removed = $false
    for ($i = 0; $i -lt $Lines.Count; $i++) {
        if ((-not $removed) -and ($i -lt $fenceEnd) -and ($Lines[$i] -cmatch ('^' + [regex]::Escape($Key) + ':(\s|$)'))) {
            $removed = $true
            continue }
        $out += $Lines[$i] }
    return @{ Lines = $out; Removed = $removed }
}


# ---------------------------------------------------------------- placement
function Get-WikiPagePlacement {
    # Where the named file sits, and whether this script may act on it at all. The refusal that
    # protects the queue lives here: the only tree a retire acts on is the wiki root it was given,
    # so a docs/raw/ entry - or anything else outside that root - can be refused but never moved.
    # -> @{ Ok; Rel; Abs; Folder; Kind; Reason }   Kind: page|nav|purpose|archive|raw|outside|missing
    param([Parameter(Mandatory = $true)][string]$Arg,
          [Parameter(Mandatory = $true)][string]$WikiFull,
          [string]$RawFull)
    $result = @{ Ok = $false; Rel = ''; Abs = ''; Folder = ''; Kind = ''; Reason = '' }
    $probe = $Arg
    if ($probe -notmatch '^[A-Za-z]:[\\/]') { $probe = Join-Path (Get-Location).Path $probe }
    $probeNorm = ($probe -replace '\\', '/')
    if ($RawFull -ne '') {
        $rawNorm = ((Resolve-Path -LiteralPath $RawFull).Path -replace '\\', '/')
        if ($probeNorm.StartsWith($rawNorm + '/', [StringComparison]::OrdinalIgnoreCase)) {
            $result.Kind = 'raw'
            $result.Reason = 'docs/raw/ is a queue that ingest consumes, not a stock it archives: a ' +
                             'superseded reference held only a pointer, the page that ingested it ' +
                             'copied that pointer, so the page is the archive (4.4)'
            return $result } }
    $abs = ''
    foreach ($candidate in @($Arg, (Join-Path $WikiFull $Arg))) {
        if (Test-Path -LiteralPath $candidate -PathType Leaf) {
            $abs = (Resolve-Path -LiteralPath $candidate).Path
            break } }
    if ($abs -eq '') {
        $result.Kind = 'missing'
        $result.Reason = ('no file there, neither as given nor relative to ' +
                          (Get-WikiRelPath -Path $WikiFull))
        return $result }
    $wikiNorm = ((Resolve-Path -LiteralPath $WikiFull).Path -replace '\\', '/')
    $absNorm = ($abs -replace '\\', '/')
    if (-not $absNorm.StartsWith($wikiNorm + '/', [StringComparison]::OrdinalIgnoreCase)) {
        $result.Kind = 'outside'
        $result.Reason = ('not under the wiki root ' + $wikiNorm + ': this script acts only inside ' +
                          'the wiki root it is given, which is what makes archiving a reference ' +
                          'unrepresentable rather than discouraged')
        return $result }
    $result.Abs = $abs
    $result.Rel = (Get-WikiRelPath -Path $abs -Base $WikiFull)
    $parts = @($result.Rel -split '/')
    $result.Folder = ''
    if ($parts.Count -ge 2) { $result.Folder = $parts[0] }
    if ($result.Folder -eq 'archive') { $result.Kind = 'archive' }
    elseif ($parts[-1] -eq 'purpose.md') { $result.Kind = 'purpose' }
    elseif ($result.Folder -eq '' -and ($script:WikiNavFiles -contains $parts[-1])) { $result.Kind = 'nav' }
    else { $result.Kind = 'page' }
    $result.Ok = $true
    return $result
}

function Get-WikiInboundLink {
    # Pages whose links land on one page. The two forms behave differently once it moves: a
    # [[stem]] still resolves, because archive/ is a category folder of the shape, while a markdown
    # link that names the path does not - so only the path form is rewritten
    # (docs/WikiDesign.md 8.1: "rewrites inbound links, prints the diff count").
    # -> @{ Count; StemForms; PathForms; Hits }
    param([Parameter(Mandatory = $true)][string]$WikiFull,
          [Parameter(Mandatory = $true)][string]$TargetRel)
    $hits = @()
    foreach ($page in @(Get-WikiPage -WikiRoot $WikiFull)) {
        if ($page.Rel -eq $TargetRel) { continue }
        foreach ($link in @(Get-WikiWikilink -Page $page)) {
            $resolved = Resolve-WikiWikilink -WikiRoot $WikiFull -Link $link
            if ($resolved -ne '' -and (($resolved -replace '\\', '/') -eq $TargetRel)) {
                $hits += [pscustomobject]@{ From = $page.Rel; Form = $link.Form; Target = $link.Target } } } }
    return @{
        Count = $hits.Count
        StemForms = @($hits | Where-Object { $_.Form -ne 'markdown' })
        PathForms = @($hits | Where-Object { $_.Form -eq 'markdown' })
        Hits = $hits }
}

function Repair-WikiInboundLink {
    # The link half of a move, used by retire and restore alike: the path forms are rewritten, the
    # [[stem]] forms are counted and left alone because archive/ is a category folder of the shape,
    # so they still resolve. Writes only the files that name the moved path.
    # The inbound set is passed in, not looked up here: a link that named the old path stops
    # resolving the moment the move happens, and a scan run afterwards would find nothing and
    # report a clean rewrite of zero links.
    # -> @{ Rewritten; Named; Lines; Writes }
    param([Parameter(Mandatory = $true)][string]$WikiFull,
          [Parameter(Mandatory = $true)]$Inbound,
          [Parameter(Mandatory = $true)][string]$OldRel,
          [Parameter(Mandatory = $true)][string]$NewRel)
    $lines = @()
    $writes = @()
    $inbound = $Inbound
    $byFile = @{}
    foreach ($hit in $inbound.PathForms) {
        if (-not $byFile.ContainsKey($hit.From)) { $byFile[$hit.From] = 0 }
        $byFile[$hit.From] = $byFile[$hit.From] + 1 }
    foreach ($fromRel in ($byFile.Keys | Sort-Object)) {
        $abs = Join-Path $WikiFull ($fromRel -replace '/', [IO.Path]::DirectorySeparatorChar)
        $before = @(Read-WikiLines -Path $abs)
        $edit = Set-WikiLinkTarget -Lines $before -OldRel $OldRel -NewRel $NewRel
        if ($edit.Rewrites -lt 1) {
            $lines += ('  SKIP ' + $fromRel + ' : the link text does not name ' + $OldRel + ' literally')
            continue }
        @($edit.Lines) | Out-File -LiteralPath $abs -Encoding utf8
        $writes += (Get-WikiRelPath -Path $abs)
        $lines += ('  rewrote ' + $edit.Rewrites + ' link(s) in ' + $fromRel) }
    $lines += ('  wikilinks left alone = ' + $inbound.StemForms.Count +
               '  ([[stem]] still resolves: archive/ is a category folder of the shape)')
    return @{ Rewritten = $writes.Count; Named = $inbound.PathForms.Count; Lines = $lines; Writes = $writes }
}

# ---------------------------------------------------------------- the gate
function Get-RetireGate {
    # Every subcommand sends its page through this one gate, so the raw refusal cannot be forgotten
    # by one of them and a refusal reads the same wherever it comes from.
    # -> @{ Ok; Place; Lines; Fail }
    param([Parameter(Mandatory = $true)][string]$Arg,
          [Parameter(Mandatory = $true)][string]$WikiFull,
          [string]$RawFull,
          [Parameter(Mandatory = $true)][string[]]$WantKinds)
    $lines = @()
    $place = Get-WikiPagePlacement -Arg $Arg -WikiFull $WikiFull -RawFull $RawFull
    if ($place.Kind -eq 'raw') {
        $lines += ('  REFUSED ' + $Arg + ' : ' + $place.Reason)
        return @{ Ok = $false; Place = $place; Lines = $lines; Fail = 1 } }
    if (-not $place.Ok) {
        $lines += ('  REFUSED ' + $Arg + ' : ' + $place.Reason)
        return @{ Ok = $false; Place = $place; Lines = $lines; Fail = 1 } }
    if ($WantKinds -notcontains $place.Kind) {
        $why = 'not one of [' + ($WantKinds -join ' ') + ']'
        switch ($place.Kind) {
            'nav' { $why = 'a nav file of section 6 is generated or governed, never archived' }
            'purpose' { $why = 'a purpose.md is a folder contract, not a page that can be superseded' }
            'archive' { $why = 'already under archive/ - this subcommand does not act twice on one page' }
        }
        $lines += ('  REFUSED ' + $place.Rel + ' : ' + $why)
        return @{ Ok = $false; Place = $place; Lines = $lines; Fail = 1 } }
    return @{ Ok = $true; Place = $place; Lines = $lines; Fail = 0 }
}

function Get-WikiRetirementBlockRange {
    # The lines the stamp owns: the heading and its leg lines, plus this script's own marker comment
    # above them when present. Everything outside the range is copied through untouched.
    # -> @{ Found; Heading; Start; End }   (zero-based inclusive)
    param([string[]]$Lines)
    $found = Get-WikiRetirementStamp -Lines $Lines
    if (-not $found.Found) { return @{ Found = $false; Heading = -1; Start = -1; End = -1 } }
    $heading = $found.HeadingLine - 1
    $start = $heading
    if ($start -ge 2 -and $Lines[$start - 1] -match '^<!-- block:' -and $Lines[$start - 2] -match '^<!-- Retirement stamp') {
        $start = $start - 2
        if ($start -ge 1 -and [string]$Lines[$start - 1] -eq '') { $start = $start - 1 } }
    $end = $heading
    for ($i = $heading + 1; $i -lt $Lines.Count; $i++) {
        $line = $Lines[$i]
        if ($line -cmatch '^#{1,6}\s') { break }
        if (([string]$line -eq '') -or ($line -cmatch '^\s*-\s+(reason|evidence|commit|from)\s*:')) { $end = $i; continue }
        break }
    return @{ Found = $true; Heading = $heading; Start = $start; End = $end }
}

function Remove-WikiRetirementBlock {
    # A restored page must not keep the stamp that said it was gone: the stamp would outlive the
    # fact. Only the stamped block goes - prose above and below it is copied through.
    # -> @{ Lines; Removed }
    param([string[]]$Lines)
    $range = Get-WikiRetirementBlockRange -Lines $Lines
    if (-not $range.Found) { return @{ Lines = $Lines; Removed = $false } }
    $out = @()
    if ($range.Start -gt 0) { $out += @($Lines[0..($range.Start - 1)]) }
    if ($range.End -lt ($Lines.Count - 1)) { $out += @($Lines[($range.End + 1)..($Lines.Count - 1)]) }
    return @{ Lines = $out; Removed = $true }
}

function Set-WikiRetirementLeg {
    # Fill one leg of a stamp that already exists but is incomplete: a line is added under the
    # heading when absent and replaced in place when present, so an empty '- commit:' is filled
    # rather than duplicated. -> @{ Lines; Changed }
    param([string[]]$Lines, [string]$Leg, [string]$Value)
    $range = Get-WikiRetirementBlockRange -Lines $Lines
    if (-not $range.Found) { return @{ Lines = $Lines; Changed = $false } }
    $body = @($Lines[$range.Start..$range.End])
    $headIdx = $range.Heading - $range.Start
    $insertAt = $headIdx
    $replaced = $false
    for ($i = 0; $i -lt $body.Count; $i++) {
        if ($body[$i] -cmatch ('^\s*-\s+' + [regex]::Escape($Leg) + '\s*:')) {
            $body[$i] = ('- ' + $Leg + ': ' + $Value)
            $replaced = $true
            continue }
        if ($body[$i] -cmatch '^\s*-\s+(reason|evidence|commit|from)\s*:') { $insertAt = $i } }
    if (-not $replaced) {
        $merged = @()
        if ($insertAt -gt 0) { $merged += @($body[0..$insertAt]) }
        $merged += ('- ' + $Leg + ': ' + $Value)
        if ($insertAt -lt ($body.Count - 1)) { $merged += @($body[($insertAt + 1)..($body.Count - 1)]) }
        $body = @($merged) }
    $out = @()
    if ($range.Start -gt 0) { $out += @($Lines[0..($range.Start - 1)]) }
    $out += $body
    if ($range.End -lt ($Lines.Count - 1)) { $out += @($Lines[($range.End + 1)..($Lines.Count - 1)]) }
    return @{ Lines = $out; Changed = $true }
}

# ---------------------------------------------------------------- subcommands
function Invoke-RetireRetire {
    # The move. Everything is computed and verified before a byte is written, because a page in two
    # places, or in none, is worse than a page that did not move.
    param([Parameter(Mandatory = $true)][string]$WikiFull,
          [string]$RawFull,
          [Parameter(Mandatory = $true)][string]$Arg,
          [string]$SuccessorArg,
          [Parameter(Mandatory = $true)][string]$ReasonText,
          [Parameter(Mandatory = $true)][string]$EvidenceText,
          [Parameter(Mandatory = $true)][string]$CommitSha)
    $lines = @()
    $gate = Get-RetireGate -Arg $Arg -WikiFull $WikiFull -RawFull $RawFull -WantKinds @('page')
    $lines += $gate.Lines
    if (-not $gate.Ok) {
        $lines += 'retired = 0 of 1'
        return @{ Lines = $lines; Fail = 1 } }
    $place = $gate.Place

    # A SupersededBy: that resolves to nothing is an F1 written into the stamp, so the successor is
    # placed by the same gate before anything moves.
    $successorRel = ''
    if ($SuccessorArg -ne '') {
        $sgate = Get-WikiPagePlacement -Arg $SuccessorArg -WikiFull $WikiFull -RawFull $RawFull
        if ((-not $sgate.Ok) -or $sgate.Kind -ne 'page') {
            $why = 'not a page of this wiki'
            if ($sgate.Reason -ne '') { $why = $sgate.Reason }
            $lines += ('  FAIL successor does not resolve: ' + $SuccessorArg + ' : ' + $why)
            $lines += 'retired = 0 of 1'
            return @{ Lines = $lines; Fail = 1 } }
        $successorRel = $sgate.Rel
        if ($successorRel -eq $place.Rel) {
            $lines += '  FAIL a page cannot supersede itself'
            $lines += 'retired = 0 of 1'
            return @{ Lines = $lines; Fail = 1 } } }

    $before = @(Read-WikiLines -Path $place.Abs)
    $existing = Get-WikiRetirementStamp -Lines $before
    if ($existing.Found) {
        $lines += ('  FAIL ' + $place.Rel + ' already carries a retirement stamp at line ' +
                   $existing.HeadingLine + ' - retiring it twice would overwrite the reason it was retired')
        $lines += 'retired = 0 of 1'
        return @{ Lines = $lines; Fail = 1 } }

    $statusValue = 'retired'
    if ($successorRel -ne '') { $statusValue = 'superseded' }
    $edit = Set-WikiFrontMatterScalar -Lines $before -Key 'Status' -Value $statusValue
    if (-not $edit.HasFrontMatter) {
        $lines += ('  FAIL ' + $place.Rel + ' has no front matter, so it has no Status: to flip - fix the page first')
        $lines += 'retired = 0 of 1'
        return @{ Lines = $lines; Fail = 1 } }
    $new = @($edit.Lines)
    if ($successorRel -ne '') {
        $new = @((Set-WikiFrontMatterScalar -Lines $new -Key 'SupersededBy' -Value $successorRel).Lines) }
    $new = @($new) + @(New-WikiRetirementBlock -ReasonText $ReasonText -EvidenceText $EvidenceText `
                        -CommitSha $CommitSha -FromRel $place.Rel)

    # Verify the stamp on the bytes about to be written, then write. An unstamped move is a design
    # failure, so the check runs before the file exists rather than after.
    $written = Get-WikiRetirementStamp -Lines $new
    if ((-not $written.Found) -or $written.Missing.Count -gt 0) {
        $lines += ('  DESIGN FAILURE: the stamp this script builds is incomplete [' +
                   ($written.Missing -join ' ') + '] - reported, and nothing moved')
        $lines += 'retired = 0 of 1'
        return @{ Lines = $lines; Fail = 1 } }

    $name = ($place.Rel -split '/')[-1]
    $leaf = [System.IO.Path]::GetFileNameWithoutExtension($name) + '.md'
    $archiveDir = Join-Path $WikiFull 'archive'
    $destAbs = Join-Path $archiveDir $leaf
    $destRel = 'archive/' + $leaf
    if (Test-Path -LiteralPath $destAbs -PathType Leaf) {
        $lines += ('  FAIL ' + $destRel + ' already exists - retiring this page would destroy the ' +
                   'evidence of the page already archived under that stem')
        $lines += 'retired = 0 of 1'
        return @{ Lines = $lines; Fail = 1 } }
    if (-not (Test-Path -LiteralPath $archiveDir -PathType Container)) {
        New-Item -ItemType Directory -Path $archiveDir -Force | Out-Null }

    # The inbound scan runs while the old path still exists: after the move nothing resolves to it.
    $inbound = Get-WikiInboundLink -WikiFull $WikiFull -TargetRel $place.Rel
    @($new) | Out-File -LiteralPath $destAbs -Encoding utf8
    Remove-Item -LiteralPath $place.Abs

    $lines += 'retired = 1 of 1'
    $lines += ('  from = ' + $place.Rel)
    $lines += ('  to = ' + (Get-WikiRelPath -Path $destAbs))
    if ($successorRel -ne '') { $lines += ('  status = ' + $statusValue + '  superseded_by = ' + $successorRel) }
    else { $lines += ('  status = ' + $statusValue) }
    $lines += ('  stamp_legs = ' + (3 - $written.Missing.Count) + ' of 3 [' + ($script:RetireLegs -join ' ') + ']')
    $lines += ('  wrote = ' + (Get-WikiRelPath -Path $destAbs))
    $lines += ('  deleted = ' + (Get-WikiRelPath -Path $place.Abs))

    $repair = Repair-WikiInboundLink -WikiFull $WikiFull -Inbound $inbound -OldRel $place.Rel -NewRel $destRel
    $lines += $repair.Lines
    $lines += ('  inbound_rewrites = ' + $repair.Rewritten + ' file(s), ' + $repair.Named +
               ' link(s) named the moved path')
    foreach ($w in $repair.Writes) { $lines += ('  wrote = ' + $w) }
    return @{ Lines = $lines; Fail = 0 }
}

function Invoke-RetireStamp {
    # The F11 guard on its own, for a page already in the archive whose stamp is incomplete. Refusing
    # is the whole point: a stamp is written with three legs or not at all.
    param([Parameter(Mandatory = $true)][string]$WikiFull,
          [string]$RawFull,
          [Parameter(Mandatory = $true)][string]$Arg,
          [string]$ReasonText,
          [string]$EvidenceText,
          [string]$CommitSha)
    $lines = @()
    $given = @{ reason = $ReasonText; evidence = $EvidenceText; commit = $CommitSha }
    $emptyLegs = @()
    foreach ($leg in $script:RetireLegs) {
        if ([string]$given[$leg] -eq '') { $emptyLegs += $leg } }
    $gate = Get-RetireGate -Arg $Arg -WikiFull $WikiFull -RawFull $RawFull -WantKinds @('archive')
    $lines += $gate.Lines
    if ($emptyLegs.Count -gt 0) {
        $lines += ('  REFUSED ' + $Arg + ' : missing stamp leg(s) [' + ($emptyLegs -join ' ') +
                   '] - an unstamped retirement is F11, so nothing is written')
        $lines += 'unstamped = 1'
        return @{ Lines = $lines; Fail = 1 } }
    if (-not $gate.Ok) {
        $lines += 'unstamped = 1'
        return @{ Lines = $lines; Fail = 1 } }
    $place = $gate.Place
    $before = @(Read-WikiLines -Path $place.Abs)
    $existing = Get-WikiRetirementStamp -Lines $before
    $new = @($before)
    if ($existing.Found -and $existing.Missing.Count -eq 0) {
        $lines += ('  ' + $place.Rel + ' is already stamped at line ' + $existing.HeadingLine + ' - nothing written')
        $lines += 'unstamped = 0'
        return @{ Lines = $lines; Fail = 0 } }
    if ($existing.Found) {
        foreach ($leg in $script:RetireLegs) {
            $new = @((Set-WikiRetirementLeg -Lines $new -Leg $leg -Value ([string]$given[$leg])).Lines) } }
    else {
        $lines += ('  NOTE ' + $place.Rel + ' carries no stamp at all - the block is added with an empty ' +
                   'from: leg, which restore will refuse to guess')
        $new = @($new) + @(New-WikiRetirementBlock -ReasonText $ReasonText -EvidenceText $EvidenceText `
                            -CommitSha $CommitSha -FromRel '') }
    $after = Get-WikiRetirementStamp -Lines $new
    if ((-not $after.Found) -or $after.Missing.Count -gt 0) {
        $lines += ('  DESIGN FAILURE: the stamp is still incomplete [' + ($after.Missing -join ' ') +
                   '] after writing - reported, the file is left as it was')
        $lines += 'unstamped = 1'
        return @{ Lines = $lines; Fail = 1 } }
    @($new) | Out-File -LiteralPath $place.Abs -Encoding utf8
    $lines += ('  stamped ' + $place.Rel)
    $lines += ('  stamp_legs = 3 of 3 [' + ($script:RetireLegs -join ' ') + ']')
    $lines += ('  wrote = ' + (Get-WikiRelPath -Path $place.Abs))
    $lines += 'unstamped = 0'
    return @{ Lines = $lines; Fail = 0 }
}

function Invoke-RetireRestore {
    # The undo, and it is why retire writes a from: leg: the archive is flat, so the folder a page
    # belongs to is a fact only the move recorded. PageType would be a guess, and a page that already
    # moved once makes the guess wrong.
    param([Parameter(Mandatory = $true)][string]$WikiFull,
          [string]$RawFull,
          [Parameter(Mandatory = $true)][string]$Arg)
    $lines = @()
    $gate = Get-RetireGate -Arg $Arg -WikiFull $WikiFull -RawFull $RawFull -WantKinds @('archive')
    $lines += $gate.Lines
    if (-not $gate.Ok) {
        $lines += 'restored = 0 of 1'
        return @{ Lines = $lines; Fail = 1 } }
    $place = $gate.Place
    $before = @(Read-WikiLines -Path $place.Abs)
    $stamp = Get-WikiRetirementStamp -Lines $before
    $fromRel = ''
    if ($stamp.Values.ContainsKey('from')) { $fromRel = [string]$stamp.Values['from'] }
    if ($fromRel -eq '') {
        $lines += ('  REFUSED ' + $place.Rel + ' : no from: leg, so there is no recorded folder to put it ' +
                   'back in - stamp one, or move it by hand and say so in log.md')
        $lines += 'restored = 0 of 1'
        return @{ Lines = $lines; Fail = 1 } }
    $destAbs = Join-Path $WikiFull ($fromRel -replace '/', [IO.Path]::DirectorySeparatorChar)
    if (Test-Path -LiteralPath $destAbs -PathType Leaf) {
        $lines += ('  FAIL ' + $fromRel + ' is occupied - restoring would overwrite the page that is live there')
        $lines += 'restored = 0 of 1'
        return @{ Lines = $lines; Fail = 1 } }
    $new = @((Remove-WikiRetirementBlock -Lines $before).Lines)
    $edit = Set-WikiFrontMatterScalar -Lines $new -Key 'Status' -Value 'draft'
    if ($edit.HasFrontMatter) {
        $new = @($edit.Lines)
        $new = @((Remove-WikiFrontMatterScalar -Lines $new -Key 'SupersededBy').Lines) }
    $dir = Split-Path -LiteralPath $destAbs -Parent
    if (-not (Test-Path -LiteralPath $dir -PathType Container)) {
        New-Item -ItemType Directory -Path $dir -Force | Out-Null }
    # Before the move, for the same reason retire scans before it moves: the archive path stops
    # resolving the moment the file is gone.
    $inbound = Get-WikiInboundLink -WikiFull $WikiFull -TargetRel $place.Rel
    @($new) | Out-File -LiteralPath $destAbs -Encoding utf8
    Remove-Item -LiteralPath $place.Abs
    $lines += 'restored = 1 of 1'
    $lines += ('  from = ' + $place.Rel)
    $lines += ('  to = ' + (Get-WikiRelPath -Path $destAbs))
    $lines += '  status = draft  (a restored page is not verified: LastVerified: still dates to the retired evidence)'
    $lines += ('  wrote = ' + (Get-WikiRelPath -Path $destAbs))
    $lines += ('  deleted = ' + (Get-WikiRelPath -Path $place.Abs))
    $repair = Repair-WikiInboundLink -WikiFull $WikiFull -Inbound $inbound -OldRel $place.Rel -NewRel $fromRel
    $lines += $repair.Lines
    $lines += ('  inbound_rewrites = ' + $repair.Rewritten + ' file(s), ' + $repair.Named +
               ' link(s) named the moved path')
    foreach ($w in $repair.Writes) { $lines += ('  wrote = ' + $w) }
    return @{ Lines = $lines; Fail = 0 }
}

function Invoke-RetireRefuse {
    # The guard tested on its own: every candidate a human proposed for archiving is run past the same
    # gate retire uses, and the run fails if the gate's verdict and the page classification ever
    # disagree - that disagreement is what "references never archive" would silently mean.
    param([Parameter(Mandatory = $true)][string]$WikiFull,
          [string]$RawFull,
          [Parameter(Mandatory = $true)][string[]]$Candidates)
    $lines = @()
    $refused = 0
    $pages = 0
    $guardLeaks = 0
    foreach ($candidate in $Candidates) {
        $place = Get-WikiPagePlacement -Arg $candidate -WikiFull $WikiFull -RawFull $RawFull
        $gate = Get-RetireGate -Arg $candidate -WikiFull $WikiFull -RawFull $RawFull -WantKinds @('page')
        if ($gate.Ok) {
            $pages = $pages + 1
            $lines += ('  MOVABLE ' + $place.Rel + ' : a page of the wiki is what retire moves; the guard does not fire')
            if ($place.Kind -ne 'page') {
                $guardLeaks = $guardLeaks + 1
                $lines += ('  LEAK the gate moved a ' + $place.Kind + ' : the guarantee is broken') }
            continue }
        $refused = $refused + 1
        $why = $place.Reason
        if ($why -eq '') { $why = 'not a page of the wiki' }
        switch ($place.Kind) {
            'archive' { $why = 'already archived - retiring it again would act on one page twice' }
            'nav' { $why = 'a nav file of section 6 is generated or governed, never archived' }
            'purpose' { $why = 'a purpose.md is a folder contract, not a page that can be superseded' }
        }
        $lines += ('  REFUSED [' + $place.Kind + '] ' + $candidate + ' : ' + $why)
        if ($place.Kind -eq 'page') {
            $guardLeaks = $guardLeaks + 1
            $lines += ('  LEAK a page was refused: retire would not move what it is for moving') } }
    $lines += ('refused = ' + $refused + ' of ' + $Candidates.Count)
    $lines += ('pages_ok_to_retire = ' + $pages + '  (pages retire; this subcommand only reports them)')
    $lines += ('guard_leaks = ' + $guardLeaks + '  (gate verdict and classification disagree: the guarantee broken)')
    return @{ Lines = $lines; Fail = $guardLeaks }
}
# ---------------------------------------------------------------- dispatcher
# Requiredness lives here, not in a Mandatory attribute: a Mandatory [string] refuses to bind '' and
# throws a non-terminating bind error the caller walks straight past into a false green. Every
# refusal below prints the contract instead (docs/agent/TOOL-FORMAT.md; the shape's
# Resolve-WikiRequiredRoot carries the same note).
$subcommands = @('retire', 'stamp', 'restore', 'refuse')
$usage = 'wiki-retire.ps1 -Command <retire|stamp|restore|refuse> -WikiRoot <dir> -Page <path> -Report <file> [-RawRoot <dir>] [-Successor <path>] [-Reason <text>] [-Evidence <text>] [-Commit <sha>]   (-Help prints this)'
$argLines = @('  required: -WikiRoot, -Page and -Report on every subcommand. retire additionally requires',
              '  -Reason, -Evidence and -Commit, because a move without the three legs is F11 and this',
              '  script makes that unrepresentable rather than reviewable.',
              '  stamp takes the same three legs and does NOT refuse at the binder when one is missing:',
              '  the missing leg is the guard''s business, so it prints unstamped = 1 and VERDICT fail=1.',
              '  -Commit is the sha that records the retirement. When the move and its log.md row land in',
              '  one commit, pass the sha being prepared, or move first and re-stamp afterwards with stamp.',
              '  refuse additionally requires -RawRoot: what it proves is that the queue cannot be',
              '  archived, and it cannot see the queue without the root. -Page is a comma-separated list.',
              '  -RawRoot is accepted by the other subcommands too, and passing it strengthens the gate:',
              '  a path inside the queue is then refused as raw instead of as outside-the-wiki-root.',
              '  Every file written is printed as wrote = <path>; a move prints wrote = and deleted =, so',
              '  the pair is on the record or neither half happened.')
if ($Help) {
    Exit-WikiHelp -ToolName 'wiki-retire' -Subcommands $subcommands -Usage $usage -ArgLines $argLines }
if ([string]::IsNullOrWhiteSpace($Command)) {
    Exit-WikiUsage -Usage $usage -Missing 'Command' -ExtraLines @('  subcommand: ' + ($subcommands -join ' | ')) }
if ($subcommands -notcontains $Command) {
    # A typo'd subcommand is named before any other arg is inspected: a run cannot fix an arg it
    # was never asked to supply, and a typo'd verb would otherwise be reported as a missing -Page.
    Exit-WikiUsage -Usage $usage -Missing ('Command (unknown: ' + $Command + ')') -ExtraLines @('  subcommand: ' + ($subcommands -join ' | ')) }
if ([string]::IsNullOrWhiteSpace($Report)) { Exit-WikiUsage -Usage $usage -Missing 'Report' }
if ([string]::IsNullOrWhiteSpace($WikiRoot)) { Exit-WikiUsage -Usage $usage -Missing 'WikiRoot' }
if ([string]::IsNullOrWhiteSpace($Page)) { Exit-WikiUsage -Usage $usage -Missing 'Page' }
$wikiFull = Resolve-WikiRequiredRoot -Name 'WikiRoot' -Value $WikiRoot -Usage $usage
$rawFull = ''
if (-not [string]::IsNullOrWhiteSpace($RawRoot)) {
    $rawFull = Resolve-WikiRequiredRoot -Name 'RawRoot' -Value $RawRoot -Usage $usage }
if ($Command -eq 'refuse' -and $rawFull -eq '') {
    Exit-WikiUsage -Usage $usage -Missing 'RawRoot (refuse proves the queue cannot be archived, and cannot see the queue without it)' }
if ($Command -eq 'retire') {
    foreach ($leg in @('Reason', 'Evidence', 'Commit')) {
        $given = [string](Get-Variable -Name $leg -ValueOnly)
        if ([string]::IsNullOrWhiteSpace($given)) {
            Exit-WikiUsage -Usage $usage -Missing ($leg + ' (retire moves content: an unstamped retirement is F11)') } } }
switch -CaseSensitive ($Command) {
    'retire' {
        $run = Invoke-RetireRetire -WikiFull $wikiFull -RawFull $rawFull -Arg $Page -SuccessorArg $Successor `
              -ReasonText $Reason -EvidenceText $Evidence -CommitSha $Commit }
    'stamp' {
        $run = Invoke-RetireStamp -WikiFull $wikiFull -RawFull $rawFull -Arg $Page `
              -ReasonText $Reason -EvidenceText $Evidence -CommitSha $Commit }
    'restore' { $run = Invoke-RetireRestore -WikiFull $wikiFull -RawFull $rawFull -Arg $Page }
    'refuse' {
        $candidates = @(@($Page -split ',') | ForEach-Object { $_.Trim() } | Where-Object { $_ -ne '' })
        $run = Invoke-RetireRefuse -WikiFull $wikiFull -RawFull $rawFull -Candidates $candidates }
    default { Exit-WikiUsage -Usage $usage -Missing ('Command (unknown: ' + $Command + ')') }
}
if ($null -eq $run) {
    Exit-WikiUsage -Usage $usage -Missing ('internal (subcommand ' + $Command + ' returned no result)') }
$out = @()
$out += ('wiki-retire command=' + $Command + '  shape=' + $script:WikiShapeVersion)
$rawLabel = '(not given: a path in the queue is then refused as outside the wiki root, never as raw)'
if ($rawFull -ne '') { $rawLabel = (Get-WikiRelPath -Path $rawFull) }
$out += ('wiki_root = ' + (Get-WikiRelPath -Path $wikiFull) + '  raw_root = ' + $rawLabel)
$out += $run.Lines
$out += ('VERDICT fail=' + $run.Fail)
Write-WikiRun -ReportPath $Report -Lines $out
if ($run.Fail -gt 0) { exit 1 }
exit 0







