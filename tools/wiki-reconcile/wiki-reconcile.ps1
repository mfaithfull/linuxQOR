#Requires -Version 7.0
<#
.SYNOPSIS
  wiki-reconcile - what is compiled still matches the tree, printed as counts.
.DESCRIPTION
  Subcommands: anchors | verify | refresh | audit | history (docs/WikiDesign.md 9(2)).
  The pair half: `wiki-census pending` answers "is anything waiting to be compiled in"; this answers
  "does what is compiled still hold". Neither implies the other, so neither prints the other's line.
  Level 2 of 9(2) is deterministic and FREE: no LLM, no fetch, no build. A judge's freshness is
  therefore read from the artifact the judge command names - that file's write date IS the day the
  judge last produced it - and -RunJudges is the explicit opt-in that executes judges instead
  (running one writes build output, which is why it can never be the default of a session-start check).
  It flags, it never rewrites: no page and no prose file is edited here. The only file this writes,
  besides -Report, is the queue it appends to, and that path is printed resolved (the decision
  "refresh and audit enqueue, they never enqueue silently": the LLM acts on the entry, not the script).
  The judge vocabulary is READ from docs/agent/verification.md - its rule sections above the Results
  log - so the wiki never invents a second notion of green, and the count read is printed by every run.
  The parse, the page listing, the anchor resolver and the vocabulary come from the shared shape
  (tools/wiki-census/wiki-shape.ps1) - one shape, five instruments.
  Required args: -WikiRoot, -RawRoot, -TreeRoot, -Report; -Ref additionally for history.
  Parameters are deliberately NOT [Mandatory] (a bare run must print the contract, not prompt).
  -Help prints the subcommands and touches no file.
#>
[CmdletBinding()]
param(
    [string]$Command,
    [string]$WikiRoot,
    [string]$RawRoot,
    [string]$TreeRoot,
    [string]$Ref,
    [string]$Report,
    [switch]$RunJudges,
    [switch]$DryRun,
    [switch]$Help
)

$_shape = Join-Path (Split-Path -Path $PSScriptRoot -Parent) 'wiki-census/wiki-shape.ps1'
if (-not (Test-Path -LiteralPath $_shape -PathType Leaf)) {
    Write-Output 'USAGE wiki-reconcile.ps1 -Command <anchors|verify|refresh|audit|history> -WikiRoot <dir> -RawRoot <dir> -TreeRoot <dir> [-Ref <git-ref>] -Report <file>   (-Help prints this)'
    Write-Output ('MISSING args=shared-shape (no wiki-shape.ps1 at ' + ($_shape -replace '\\', '/') + ')')
    Write-Output 'VERDICT fail=1'
    exit 1
}
. $_shape

# judge evidence: Get-WikiJudgeEvidence and Get-WikiPageFreshness live in the shared shape (tools/wiki-census/wiki-shape.ps1), dot-sourced above - wiki-lint freshness must weigh the same evidence wiki-reconcile verifies, and two copies is two notions of stale.

# ---------------------------------------------------------------- the sweep
function Get-WikiReconcileSweep {
    # One pass over the pages feeding every subcommand: each anchor resolved by the shared resolver,
    # each page's claim weighed against its judge evidence. Findings are the queue's future rows.
    # A page is never edited here - 9(2): "a page becomes flagged, not rewritten".
    param([string]$WikiFull, [string]$TreeFull, [switch]$RunJudges)
    $set = Get-WikiPageSet -WikiRoot $WikiFull
    $pages = @($set.Pages)
    $anchorTotal = 0
    $anchorResolved = 0
    $derived = 0
    $judgeTotal = 0
    $evidenced = 0
    $ranTotal = 0
    $red = 0
    $undated = 0
    $unstamped = 0
    $findings = @()
    $details = @()
    $flagged = @()
    foreach ($page in $pages) {
        $pageStuck = $false
        foreach ($anchor in @(Get-WikiAnchorRecord -Fields $page.Fields)) {
            $anchorTotal = $anchorTotal + 1
            $result = Resolve-WikiAnchor -Anchor $anchor -TreeRoot $TreeFull
            if ($result.Status -eq 'resolved') { $anchorResolved = $anchorResolved + 1; continue }
            if ($result.Status -eq 'derived_only') {
                $anchorResolved = $anchorResolved + 1
                $derived = $derived + 1
                continue }
            $pageStuck = $true
            $findings += [pscustomobject]@{ Finding = $result.Status; Page = $page.Rel
                                            Path = $anchor.Path; Anchor = $anchor.Anchor
                                            AnchorKind = $anchor.Kind; Pin = $anchor.Pin
                                            Target = [string]$page.Fields['Target']
                                            Reason = $result.Reason } }
        $fresh = Get-WikiPageFreshness -Page $page -TreeRoot $TreeFull -RunJudges:$RunJudges
        $judgeTotal = $judgeTotal + [int]$fresh.JudgeCount
        $evidenced = $evidenced + [int]$fresh.Evidenced
        $ranTotal = $ranTotal + [int]$fresh.Ran
        $red = $red + [int]$fresh.Red
        $details += @($fresh.Details)
        if ($null -ne $fresh.LastVerified -and [int]$fresh.JudgeCount -gt 0 -and
            [int]$fresh.Evidenced -eq 0 -and -not $RunJudges) {
            $undated = $undated + 1
            $details += ('  UNJUDGED ' + $page.Rel + ' : stamped ' +
                        $fresh.LastVerified.ToString('yyyy-MM-dd') +
                        ' but no judge names a file that exists, so nothing here proves the stamp') }
        if ($null -eq $fresh.LastVerified) {
            $unstamped = $unstamped + 1
            $pageStuck = $true
            $findings += [pscustomobject]@{ Finding = 'unstamped'; Page = $page.Rel; Path = ''
                                            Anchor = ''; AnchorKind = ''; Pin = ''
                                            Target = [string]$page.Fields['Target']
                                            Reason = 'no LastVerified: stamp to weigh' } }
        elseif ($null -ne $fresh.Newest -and $fresh.Newest -gt $fresh.LastVerified) {
            $pageStuck = $true
            $findings += [pscustomobject]@{ Finding = 'stale'; Page = $page.Rel
                                            Path = [string]$fresh.Basis; Anchor = ''
                                            AnchorKind = 'in-tree'; Pin = ''
                                            Target = [string]$page.Fields['Target']
                                            Reason = ('evidence ' + $fresh.Newest.ToString('yyyy-MM-dd') +
                                                      ' (' + $fresh.Basis + ') is newer than the stamp ' +
                                                      $fresh.LastVerified.ToString('yyyy-MM-dd')) } }
        if ([int]$fresh.Red -gt 0) {
            $pageStuck = $true
            $findings += [pscustomobject]@{ Finding = 'red'; Page = $page.Rel; Path = ''
                                            Anchor = ''; AnchorKind = ''; Pin = ''
                                            Target = [string]$page.Fields['Target']
                                            Reason = ([string]$fresh.Red + ' judge(s) ran red') } }
        if ($pageStuck) { $flagged += $page.Rel } }
    return @{
        Pages = $pages; PageCount = $pages.Count; FileCount = $set.All.Count
        AnchorTotal = $anchorTotal; AnchorResolved = $anchorResolved; Derived = $derived
        Findings = $findings; Flagged = $flagged; FlagCount = $flagged.Count
        JudgeTotal = $judgeTotal; Evidenced = $evidenced; Ran = $ranTotal; Red = $red
        Undated = $undated; Unstamped = $unstamped; Details = $details; Strays = $set.Strays }
}

function Get-WikiQueueFinding {
    # The queue's own anchors, resolved by the same resolver: a queued reference that no longer
    # resolves is work the queue has promised and cannot deliver (4.3, the anti-rot contract).
    param([string]$RawFull, [string]$TreeFull)
    $entries = @(Get-WikiQueueEntry -QueueFile (Join-Path $RawFull 'pending.md'))
    $resolved = 0
    $findings = @()
    foreach ($entry in $entries) {
        $anchor = [pscustomobject]@{ Kind = [string]$entry['kind']; Path = [string]$entry['path']
                                     Anchor = [string]$entry['anchor']; Pin = [string]$entry['pin']
                                     Verified = [string]$entry['verified']; Symbol = '' }
        $result = Resolve-WikiAnchor -Anchor $anchor -TreeRoot $TreeFull
        if ($result.Status -eq 'resolved' -or $result.Status -eq 'derived_only') {
            $resolved = $resolved + 1
            continue }
        $findings += [pscustomobject]@{ Finding = 'queue_' + $result.Status
                                        Page = 'queue:' + [string]$entry['Id']
                                        Path = [string]$entry['path']; Anchor = [string]$entry['anchor']
                                        AnchorKind = [string]$entry['kind']; Pin = [string]$entry['pin']
                                        Target = [string]$entry['target']; Reason = $result.Reason } }
    return @{ Entries = $entries; Count = $entries.Count; Resolved = $resolved; Findings = $findings }
}

# ---------------------------------------------------------------- the append
function Add-WikiQueueEntry {
    # The queue append: one nine-line entry per finding, in the order docs/raw/pending.md declares
    # (id, added_by, kind, path, anchor, pin, verified, target, why). Deduped against the queue's own
    # rows first, because refresh runs at every session start and an entry already queued is not new
    # work. Nothing is silent: each row and each skip is printed, and the queue path is printed
    # resolved. -DryRun prints the rows and writes none of them.
    param([string]$QueueFile, [object[]]$Findings, [string]$DateStamp, [switch]$DryRun)
    $existing = @{}
    foreach ($entry in @(Get-WikiQueueEntry -QueueFile $QueueFile)) {
        $key = ([string]$entry['kind']) + '|' + ([string]$entry['path']) + '|' + ([string]$entry['anchor'])
        $existing[$key] = 1 }
    $added = 0
    $skipped = @()
    $printed = @()
    $rows = @()
    $index = 0
    foreach ($finding in $Findings) {
        $kind = [string]$finding.AnchorKind
        if ($kind -eq '' -or $script:WikiKinds -notcontains $kind) { $kind = 'in-tree' }
        $path = [string]$finding.Path
        $anchor = [string]$finding.Anchor
        if ($finding.Finding -eq 'stale' -or $finding.Finding -eq 'red') {
            $path = [string]$finding.Page
            if ($finding.Finding -eq 'red') { $anchor = '#Judges' } else { $anchor = '#LastVerified' } }
        $key = $kind + '|' + $path + '|' + $anchor
        if ($existing.ContainsKey($key)) {
            $skipped += ('  SKIPPED duplicate of a queued row : ' + $kind + ' ' + $path + $anchor)
            continue }
        $existing[$key] = 1
        $index = $index + 1
        $target = [string]$finding.Target
        if ($target.Trim() -eq '') { $target = 'none' }
        $why = ('wiki-reconcile ' + $finding.Finding + ' on ' + [string]$finding.Page + ' : ' +
                ([string]$finding.Reason) -replace '[\r\n\t]', ' ')
        if ($why.Length -gt 180) { $why = $why.Substring(0, 180) }
        # NOTE: the comma operator binds tighter than '+', so a row built as
        # @('a: ' + $x, 'b: ' + $y) collapses to ONE string. Newline-separated elements
        # inside @() are the safe form: each of the nine queue fields is its own line, which
        # is also exactly the shape docs/raw/pending.md declares.
        $rows += , @(
            ('- id: refresh-' + $DateStamp + '-' + $index)
            '  added_by: refresh'
            ('  kind: ' + $kind)
            ('  path: ' + $path)
            ('  anchor: ' + $anchor)
            ('  pin: ' + ([string]$finding.Pin).Trim())
            ('  verified: ' + $DateStamp.Substring(0, 4) + '-' + $DateStamp.Substring(4, 2) + '-' + $DateStamp.Substring(6, 2))
            ('  target: ' + $target)
            ('  why: ' + $why)
        )
        $printed += ('  QUEUE - id: refresh-' + $DateStamp + '-' + $index + ' : ' +
                     $finding.Finding + ' ' + $kind + ' ' + $path + $anchor)
        $added = $added + 1 }
    if ($added -gt 0 -and -not $DryRun) {
        $block = @()
        foreach ($row in $rows) { $block += $row }
        Add-Content -LiteralPath $QueueFile -Value $block -Encoding utf8 }
    # The rows are returned, not written to the output stream: this function runs inside an
    # assignment, so a Write-Output here would land in the caller's $append and swallow its counts.
    return @{ Added = $added; Skipped = $skipped.Count; SkippedLines = $skipped; Rows = $rows; Printed = $printed }
}

# ---------------------------------------------------------------- subcommands
function Get-WikiVocabularyLines {
    # The vocabulary line every run prints: reconcile reads docs/agent/verification.md rather than
    # copying it, so every verdict says which rules it was measured against (13 Q2).
    param([string]$TreeFull, [string]$Usage)
    $file = Resolve-WikiRequiredRoot -Name 'TreeRoot/docs/agent/verification.md' `
              -Value (Join-Path $TreeFull 'docs/agent/verification.md') -Usage $Usage -File
    $vocab = Get-WikiJudgeVocabulary -VerificationFile $file
    $lines = @()
    $lines += ('judge_vocabulary = ' + (Get-WikiRelPath -Path $file))
    $lines += ('rule_sections = ' + $vocab.RuleSectionsRead + ' of ' + $vocab.RuleSectionCount +
               '  (headings read above ## Results plus the green rule; ' +
               $vocab.LogBulletsExcluded + ' log bullets are evidence, not rules)')
    $lines += ('judge_heads = ' + (($vocab.Heads | Sort-Object) -join ', ') +
               '  [+ builtin: ' + ($vocab.ExtraHeads -join ', ') + ']')
    return @{ Lines = $lines; Vocab = $vocab }
}

function Invoke-ReconcileAnchor {
    param([string]$WikiFull, [string]$RawFull, [string]$TreeFull)
    $sweep = Get-WikiReconcileSweep -WikiFull $WikiFull -TreeFull $TreeFull
    $queue = Get-WikiQueueFinding -RawFull $RawFull -TreeFull $TreeFull
    $lines = @()
    $dead = @($sweep.Findings | Where-Object { $_.Finding -eq 'dead' })
    $tmp = @($sweep.Findings | Where-Object { $_.Finding -eq 'tmp' })
    $unpinned = @($sweep.Findings | Where-Object { $_.Finding -eq 'unpinned' })
    $lines += ('pages = ' + $sweep.PageCount + ' of ' + $sweep.FileCount + '  (files that carry the WikiPage shape)')
    $lines += ('anchors = ' + $sweep.AnchorResolved + ' of ' + $sweep.AnchorTotal +
               '  (page Anchors: re-resolved against the tree)')
    $lines += ('dead = ' + $dead.Count + '  (a path, symbol or commit that no longer resolves)')
    $lines += ('unpinned = ' + $unpinned.Count + '  (out-of-tree with no pin or verified date: F11)')
    $lines += ('tmp_refs = ' + $tmp.Count + '  (an anchor into tmp/ is a contract violation: 4.3 rule 3)')
    $lines += ('derived_only = ' + $sweep.Derived + '  (line-anchor only: legal, never the only anchor)')
    $lines += ('queue_entries = ' + $queue.Count + '  queue_anchors = ' + $queue.Resolved + ' of ' + $queue.Count)
    $lines += ('queue_dead = ' + $queue.Findings.Count +
               '  (a queued anchor that cannot resolve is work the queue cannot deliver)')
    foreach ($finding in @($sweep.Findings)) {
        $lines += ('  ' + $finding.Finding.ToUpper() + ' page ' + $finding.Page + ' : ' +
                   $finding.Path + $finding.Anchor + ' : ' + $finding.Reason) }
    foreach ($finding in @($queue.Findings)) {
        $lines += ('  ' + $finding.Finding.ToUpper() + ' ' + $finding.Page + ' : ' +
                   $finding.Path + $finding.Anchor + ' : ' + $finding.Reason) }
    if ($sweep.PageCount -eq 0 -and $queue.Count -eq 0) {
        $lines += 'vacuity: pages=0 and an empty queue - anchors = 0 of 0 here proves the parse ran, not that the wiki holds (9)' }
    $fail = $dead.Count + $tmp.Count + $unpinned.Count + $queue.Findings.Count
    return @{ Lines = $lines; Fail = $fail }
}

function Invoke-ReconcileVerify {
    param([string]$WikiFull, [string]$TreeFull, [switch]$RunJudges)
    $sweep = Get-WikiReconcileSweep -WikiFull $WikiFull -TreeFull $TreeFull -RunJudges:$RunJudges
    $lines = @()
    $stale = @($sweep.Findings | Where-Object { $_.Finding -eq 'stale' })
    $unstamped = @($sweep.Findings | Where-Object { $_.Finding -eq 'unstamped' })
    $lines += ('pages = ' + $sweep.PageCount + ' of ' + $sweep.FileCount)
    $lines += ('judges = ' + $sweep.Evidenced + ' of ' + $sweep.JudgeTotal +
               '  (Judges: commands naming an artifact that exists, which is what dates them)')
    if ($RunJudges) {
        $lines += ('judges_run = ' + $sweep.Ran + '  (executed in the tree root: this run was not free)') }
    else {
        $lines += ('judges_run = 0  (free by default, 9(2): freshness is read from the artifact each judge' +
                   ' names; -RunJudges executes them)') }
    $lines += ('red = ' + $sweep.Red + '  (a judge that ran and is not green: the page claims a green the tree lacks)')
    $lines += ('stale = ' + $stale.Count + '  (newest judge evidence is newer than the LastVerified: stamp)')
    $lines += ('undated = ' + $sweep.Undated + '  (stamped, but no judge names a file that exists: unproven here)')
    $lines += ('unstamped = ' + $unstamped.Count + '  (no LastVerified: stamp to weigh at all)')
    $lines += ('flagged = ' + $sweep.FlagCount + '  (pages needing work: flagged, never rewritten, 9(2))')
    $lines += @($sweep.Details)
    foreach ($finding in $stale) { $lines += ('  STALE page ' + $finding.Page + ' : ' + $finding.Reason) }
    foreach ($finding in $unstamped) { $lines += ('  UNSTAMPED page ' + $finding.Page + ' : ' + $finding.Reason) }
    if ($sweep.PageCount -eq 0) {
        $lines += 'vacuity: pages=0 - stale = 0 of 0 here proves the parse ran, not that the wiki is fresh (9)' }
    $fail = $stale.Count + $sweep.Red
    return @{ Lines = $lines; Fail = $fail }
}

function Invoke-ReconcileRefresh {
    param([string]$WikiFull, [string]$RawFull, [string]$TreeFull, [switch]$RunJudges, [switch]$DryRun)
    $sweep = Get-WikiReconcileSweep -WikiFull $WikiFull -TreeFull $TreeFull -RunJudges:$RunJudges
    $queue = Get-WikiQueueFinding -RawFull $RawFull -TreeFull $TreeFull
    $queueFile = Join-Path $RawFull 'pending.md'
    # One entry per dead anchor or stale judge, with the red and rotted-queue cases beside them.
    # 'unstamped' is page-side work with no reference to carry, so it is counted, never queued.
    $wanted = @('dead', 'tmp', 'unpinned', 'stale', 'red', 'queue_dead', 'queue_tmp', 'queue_unpinned')
    $all = @($sweep.Findings) + @($queue.Findings)
    $toQueue = @($all | Where-Object { $wanted -contains $_.Finding })
    $unstamped = @($sweep.Findings | Where-Object { $_.Finding -eq 'unstamped' })
    $stamp = (Get-Date).ToString('yyyyMMdd')
    $append = Add-WikiQueueEntry -QueueFile $queueFile -Findings $toQueue -DateStamp $stamp -DryRun:$DryRun
    $lines = @()
    $lines += ('pages = ' + $sweep.PageCount + ' of ' + $sweep.FileCount)
    $lines += ('findings = ' + $toQueue.Count + '  (dead, tmp, unpinned, stale, red, queue_*: the queueable half of the sweep)')
    $lines += ('not_queued_unstamped = ' + $unstamped.Count + '  (no reference to carry: a page-side gap, not a queue row)')
    $lines += ('added = ' + $append.Added + '  added_by = refresh  (one entry per finding, deduped below)')
    $lines += ('skipped_duplicate = ' + $append.Skipped + '  (already queued: refresh runs at every session start)')
    if ($DryRun) { $lines += 'dry_run = true  (the rows below were printed and not written)' }
    $lines += ('queue = ' + (Get-WikiRelPath -Path $queueFile))
    $lines += @($append.Printed)
    $lines += @($append.SkippedLines)
    if ($toQueue.Count -eq 0 -and $sweep.PageCount -eq 0 -and $queue.Count -eq 0) {
        $lines += 'vacuity: pages=0 and an empty queue - added = 0 here proves the parse ran, not that the wiki holds (9)' }
    return @{ Lines = $lines; Fail = $toQueue.Count }
}

function Invoke-ReconcileAudit {
    param([string]$WikiFull, [string]$TreeFull, [switch]$RunJudges)
    $sweep = Get-WikiReconcileSweep -WikiFull $WikiFull -TreeFull $TreeFull -RunJudges:$RunJudges
    # The wiki is not the only holder of anchors: every docs/ prose doc cites paths in backticks, and
    # a citation that no longer resolves rots exactly as a page anchor does. Only a citation whose
    # parent directory exists is counted, so a prose example (`docs/.../file.cpp`) is not a finding.
    $docsRoot = Join-Path $TreeFull 'docs'
    $proseFiles = @(Get-ChildItem -LiteralPath $docsRoot -Recurse -Filter '*.md' -File -Force |
                    Where-Object { $_.FullName -notmatch '[\\/](tmp|build|out)[\\/]' })
    $proseTotal = 0
    $proseResolved = 0
    $proseDead = @()
    foreach ($file in $proseFiles) {
        foreach ($found in @(Get-WikiProseAnchorCandidate -AbsFile $file.FullName -TreeRoot $TreeFull)) {
            $proseTotal = $proseTotal + 1
            if ($found.Status -eq 'resolved') { $proseResolved = $proseResolved + 1; continue }
            $proseDead += ('  DEAD PROSE ' + $found.File + ':' + $found.Line + ' : ' + $found.Path) } }
    $lines = @()
    $stale = @($sweep.Findings | Where-Object { $_.Finding -eq 'stale' })
    $dead = @($sweep.Findings | Where-Object { $_.Finding -eq 'dead' })
    $lines += ('pages = ' + $sweep.PageCount + ' of ' + $sweep.FileCount)
    $lines += ('prose_docs = ' + $proseFiles.Count + '  (docs/**/*.md, excluding tmp/, build/ and out/)')
    $lines += ('prose_anchors = ' + $proseResolved + ' of ' + $proseTotal +
               '  (backticked repo-relative paths whose parent directory exists)')
    $lines += ('prose_dead = ' + $proseDead.Count + '  (informational, never judged: planning prose' +
               ' names files it means to write - docs/wiki/schema.md is cited by 20-odd plan.md lines' +
               ' and does not exist yet. The anti-rot contract of 4.3 binds a page Anchors: field.)')
    $lines += ('page_dead = ' + $dead.Count + '  page_stale = ' + $stale.Count +
               '  (the judged half: a page claim the tree does not hold)')
    $lines += ('strays = ' + @($sweep.Strays).Count + '  (files in the wiki that are not pages: printed, judged by wiki-lint)')
    $lines += $proseDead
    foreach ($finding in @($sweep.Findings)) {
        $lines += ('  ' + $finding.Finding.ToUpper() + ' page ' + $finding.Page + ' : ' +
                   $finding.Path + $finding.Anchor + ' : ' + $finding.Reason) }
    if ($sweep.PageCount -eq 0 -and $proseTotal -eq 0) {
        $lines += 'vacuity: pages=0 and no prose anchor to resolve - fail = 0 here proves the parse ran, not that the docs hold (9)' }
    if ($sweep.PageCount -eq 0 -and $proseDead.Count -gt 0) {
        $lines += ('vacuity: pages=0 - the audit judged nothing; the ' + $proseDead.Count +
                   ' dead prose anchors above are counted, not judged') }
    $fail = $dead.Count + $stale.Count + $sweep.Red
    return @{ Lines = $lines; Fail = $fail }
}

function Invoke-ReconcileHistory {
    param([string]$WikiFull, [string]$TreeFull, [string]$Ref)
    $lines = @()
    $verify = @(git -C $TreeFull rev-parse --verify ($Ref + '^{commit}') 2>&1)
    if ($LASTEXITCODE -ne 0) {
        $lines += ('ref = ' + $Ref + '  (does not resolve to a commit in this tree)')
        $lines += ('  BADREF ' + $Ref + ' : ' + ((@($verify) | Select-Object -First 1) -join ' '))
        $lines += 'note: churn of nothing is not zero churn - a ref that does not resolve is a usage failure'
        return @{ Lines = $lines; Fail = 1 } }
    $commits = @((git -C $TreeFull rev-list --count ($Ref + '..HEAD')) | ForEach-Object { ([string]$_).Trim() })
    $changed = @()
    # One '..' range, not two arguments: 'git diff --name-only "A B"' is one opaque token and
    # returns fatal on it, which would otherwise read as the honest-looking 'moved = 0'.
    $diff = @(git -C $TreeFull diff --name-only ($Ref + '..HEAD') 2>&1)
    if ($LASTEXITCODE -ne 0) {
        $lines += ('ref = ' + $Ref + '  (git diff failed over the range ' + $Ref + '..HEAD)')
        $lines += ('  GITFAIL ' + ((@($diff) | Select-Object -First 1) -join ' '))
        return @{ Lines = $lines; Fail = 1 } }
    foreach ($line in $diff) {
        $clean = ([string]$line).Trim()
        if ($clean -ne '') { $changed += $clean } }
    $changedSet = @{}
    foreach ($path in $changed) { $changedSet[$path] = 1 }
    $set = Get-WikiPageSet -WikiRoot $WikiFull
    $pages = @($set.Pages)
    $moved = @()
    foreach ($page in $pages) {
        $hits = @()
        foreach ($anchor in @(Get-WikiAnchorRecord -Fields $page.Fields)) {
            $path = [string]$anchor.Path
            if ($path -eq '') { continue }
            if ($changedSet.ContainsKey($path)) { $hits += $path } }
        if ($changedSet.ContainsKey((Get-WikiRelPath -Path $page.Abs -Base $TreeFull))) {
            $hits += (Get-WikiRelPath -Path $page.Abs -Base $TreeFull) }
        if ($hits.Count -gt 0) {
            $moved += ('  MOVED ' + $page.Rel + ' : ' + (($hits | Sort-Object -Unique) -join ', ')) } }
    $lines += ('ref = ' + $Ref + '  commits = ' + (($commits | Select-Object -First 1)) +
               '  changed_files = ' + $changed.Count)
    $lines += ('pages = ' + $pages.Count)
    $lines += ('moved = ' + $moved.Count + '  (pages holding an anchor to a file that changed since the ref)')
    $lines += $moved
    if ($pages.Count -eq 0) {
        $lines += 'vacuity: pages=0 - moved = 0 here proves the sweep ran, not that nothing moved (9)' }
    return @{ Lines = $lines; Fail = 0 }
}

# ---------------------------------------------------------------- dispatcher
# Requiredness lives here, not in a Mandatory attribute, so a bare run prints the contract instead
# of prompting (docs/agent/TOOL-FORMAT.md). Every root is tested for emptiness before it is resolved.
$subcommands = @('anchors', 'verify', 'refresh', 'audit', 'history')
$usage = 'wiki-reconcile.ps1 -Command <anchors|verify|refresh|audit|history> -WikiRoot <dir> -RawRoot <dir> -TreeRoot <dir> [-Ref <git-ref>] -Report <file>   (-Help prints this)'
$argLines = @('  required: -WikiRoot, -RawRoot, -TreeRoot and -Report on every subcommand; -Ref additionally',
              '  for history. <TreeRoot>/docs/agent/verification.md is the only derived path: it is the judge',
              '  vocabulary, is resolved and printed by every run, and its rule-section count is printed too.',
              '  optional: -DryRun prints the queue rows refresh would append and writes none;',
              '            -RunJudges executes each page''s Judges: commands instead of reading the artifact',
              '            they name - it writes build output and takes minutes, which is why it is opt-in.')
if ($Help) {
    Exit-WikiHelp -ToolName 'wiki-reconcile' -Subcommands $subcommands -Usage $usage -ArgLines $argLines }
if ([string]::IsNullOrWhiteSpace($Command)) {
    Exit-WikiUsage -Usage $usage -Missing 'Command' -ExtraLines @('  subcommand: ' + ($subcommands -join ' | ')) }
if ([string]::IsNullOrWhiteSpace($Report)) { Exit-WikiUsage -Usage $usage -Missing 'Report' }
if ([string]::IsNullOrWhiteSpace($WikiRoot)) { Exit-WikiUsage -Usage $usage -Missing 'WikiRoot' }
if ([string]::IsNullOrWhiteSpace($RawRoot)) { Exit-WikiUsage -Usage $usage -Missing 'RawRoot' }
if ([string]::IsNullOrWhiteSpace($TreeRoot)) { Exit-WikiUsage -Usage $usage -Missing 'TreeRoot' }
$wikiFull = Resolve-WikiRequiredRoot -Name 'WikiRoot' -Value $WikiRoot -Usage $usage
$rawFull = Resolve-WikiRequiredRoot -Name 'RawRoot' -Value $RawRoot -Usage $usage
$treeFull = Resolve-WikiRequiredRoot -Name 'TreeRoot' -Value $TreeRoot -Usage $usage
if ($Command -eq 'history' -and [string]::IsNullOrWhiteSpace($Ref)) {
    Exit-WikiUsage -Usage $usage -Missing 'Ref (history measures churn since a ref; churn of nothing is not zero churn)' }
$vocabRun = Get-WikiVocabularyLines -TreeFull $treeFull -Usage $usage
switch -CaseSensitive ($Command) {
    'anchors' { $run = Invoke-ReconcileAnchor -WikiFull $wikiFull -RawFull $rawFull -TreeFull $treeFull }
    'verify' { $run = Invoke-ReconcileVerify -WikiFull $wikiFull -TreeFull $treeFull -RunJudges:$RunJudges }
    'refresh' {
        $run = Invoke-ReconcileRefresh -WikiFull $wikiFull -RawFull $rawFull -TreeFull $treeFull `
              -RunJudges:$RunJudges -DryRun:$DryRun }
    'audit' { $run = Invoke-ReconcileAudit -WikiFull $wikiFull -TreeFull $treeFull -RunJudges:$RunJudges }
    'history' { $run = Invoke-ReconcileHistory -WikiFull $wikiFull -TreeFull $treeFull -Ref $Ref }
    default { Exit-WikiUsage -Usage $usage -Missing ('Command (unknown: ' + $Command + ')') }
}
if ($null -eq $run) {
    Exit-WikiUsage -Usage $usage -Missing ('internal (subcommand ' + $Command + ' returned no result)') }
$out = @()
$out += ('wiki-reconcile command=' + $Command + '  shape=' + $script:WikiShapeVersion)
$out += ('wiki_root = ' + (Get-WikiRelPath -Path $wikiFull) + '  raw_root = ' + (Get-WikiRelPath -Path $rawFull) + '  tree_root = ' + (Get-WikiRelPath -Path $treeFull))
$out += $vocabRun.Lines
$out += $run.Lines
$out += ('VERDICT fail=' + $run.Fail)
Write-WikiRun -ReportPath $Report -Lines $out
if ($run.Fail -gt 0) { exit 1 }
exit 0






