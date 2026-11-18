#Requires -Version 7.0
<#
.SYNOPSIS
  wiki-lint - does every page obey the schema, and does every claim carry provenance.
.DESCRIPTION
  Subcommands: schema | provenance | freshness | purpose | size.
  The governance layer made of checks (docs/WikiDesign.md 6, 11): a page that breaks the shape,
  smuggles a URL or a secret, claims a green it cannot date, sits in a folder with no purpose.md,
  or outgrows the budget is reported here, on every run, with a count.
  The parse, the page listing and the freshness measure come from the shared shape
  (tools/wiki-census/wiki-shape.ps1), dot-sourced below - one shape, five instruments.
  Required args: -WikiRoot, -Report, plus -TreeRoot for freshness. No default encodes a task
  (docs/agent/TOOL-FORMAT.md). -Help prints the subcommands and touches no file.
#>
[CmdletBinding()]
param(
    [string]$Command,
    [string]$WikiRoot,
    [string]$RawRoot,
    [string]$TreeRoot,
    [string]$Schema,
    [string]$Report,
    [switch]$Help
)

$shapePath = Join-Path (Split-Path -Path $PSScriptRoot -Parent) 'wiki-census/wiki-shape.ps1'
if (-not (Test-Path -LiteralPath $shapePath -PathType Leaf)) {
    Write-Output 'USAGE wiki-lint.ps1 -Command <schema|provenance|freshness|purpose|size> -WikiRoot <dir> [-TreeRoot <dir>] [-Schema <file>] -Report <file>   (-Help prints this)'
    Write-Output ('MISSING args=shared-shape (no wiki-shape.ps1 at ' + ($shapePath -replace '\\', '/') + ')')
    Write-Output 'VERDICT fail=1'
    exit 1
}
. $shapePath

# ---------------------------------------------------------------- the schema
# The governance file has a shape of its own, and the instrument that reads it is the only thing
# that can say the shape is there. The ids are the seven sections the governance plan enumerates
# and the five ops the op table owns; a missing one is a rule nobody wrote down (F10), and an op
# row with no verdict line is an op no instrument can close.
$script:WikiSchemaSections = @('fields','pages','links','provenance','archive','budget','ops')
$script:WikiSchemaOps = @('ingest','refresh','wiki-reconcile','wiki-file','wiki-retire')

function Get-WikiSchemaVocabulary {
    # docs/wiki/schema.md is the authority (it is one file, by the BuildLLMWiki decision). It does
    # not exist until the governance layer writes it, so the builtin vocabulary of the shape is used
    # and PRINTED as builtin: a lint that quietly invents its own rules is the F1 shape.
    # The reader is tolerant and its read is printed, so a wrong read is visible, not silent.
    # The file governs pages AND itself: the seven section ids and the five op rows are its own
    # shape, and a governance file missing one is F10 (governance drift) - so they are read here,
    # by the only instrument that reads this file, rather than by a second script that re-reads it.
    param([string]$WikiFull, [string]$SchemaArg)
    $file = $SchemaArg
    if ($file -eq '') { $file = Join-Path $WikiFull 'schema.md' }
    $result = @{
        File = $file; Exists = (Test-Path -LiteralPath $file -PathType Leaf)
        Fields = @($script:WikiPageFields); BudgetLines = [int]$script:WikiPageBudgetLines
        ReadLines = 0; Source = 'builtin'
        Sections = @(); MissingSections = @()
        Ops = @(); MissingOps = @(); OpsNoVerdict = @()
        # Declared is what the FILE names; Fields is the vocabulary pages are judged against (the
        # shape plus anything this file adds). They are not the same list and never were: seeding
        # Declared from the shape made 'fields = N of N' true for a file that declares nothing, and
        # UndeclaredFields empty by construction - the governance plan's stop condition grading itself.
        Declared = @(); DeclaredShapeFields = @($script:WikiPageFields); UndeclaredFields = @(); Drift = 0
    }
    if (-not $result.Exists) {
        $result.MissingSections = @($script:WikiSchemaSections)
        $result.MissingOps = @($script:WikiSchemaOps)
        # The builtin IS the shape, so it declares every field of the shape: the pair prints 13 of 13
        # and the drift stays 0, because there is no file here to disagree with.
        $result.DeclaredShapeFields = @($script:WikiPageFields)
        $result.UndeclaredFields = @()
        $result.Drift = 0
        return $result
    }
    $lines = @(Read-WikiLines -Path $file)
    $found = @()
    $sections = @()
    $ops = @{}
    $budget = 0
    $inOps = $false
    foreach ($line in $lines) {
        $result.ReadLines = $result.ReadLines + 1
        # A heading whose id is one of the seven switches sections; 'ops' also opens the op table,
        # whose rows are read below it and nowhere else (a `word` elsewhere is prose, not an op).
        if ($line -cmatch '^#{1,6}\s+([A-Za-z][A-Za-z_-]*)\s*$') {
            $id = $Matches[1].ToLower()
            if ($script:WikiSchemaSections -contains $id) {
                if ($sections -notcontains $id) { $sections += $id } }
            $inOps = ($id -eq 'ops')
            continue }
        if ($inOps -and $line -match '^\|\s*`{0,1}([a-z][a-z_-]*)`{0,1}\s*\|') {
            $op = $Matches[1]
            if ($script:WikiSchemaOps -contains $op) {
                $ops[$op] = ($ops[$op] -or ($line -cmatch 'VERDICT')) } }
        foreach ($m in [regex]::Matches($line, '`([A-Z][A-Za-z]+)`')) {
            if ($script:WikiPageTypes -contains $m.Groups.Item(1).Value.ToLower() -or
                $script:WikiStatuses -contains $m.Groups.Item(1).Value.ToLower()) { continue }
            $found += $m.Groups.Item(1).Value }
        if ($line -match '(?i)\bbudget\b[^0-9]*(\d{2,4})|(\d{2,4})\s*lines\b') {
            # An unmatched capture group is $null, and $null -eq '' is FALSE in PowerShell: testing
            # for '' silently dropped every "N lines" budget and left the builtin 200 in force.
            $num = [string]$Matches[1]
            if ([string]::IsNullOrWhiteSpace($num)) { $num = [string]$Matches[2] }
            if ($budget -eq 0 -and $num -match '^\d+$') { $budget = [int]$num } }
    }
    # Declared is only what the file itself names. The vocabulary pages are judged against is the
    # union, so a field this file adds is legal for pages; a shape field it omits is drift, below.
    $declared = @($found | Sort-Object -Unique)
    $union = @($script:WikiPageFields)
    foreach ($field in $declared) {
        if ($union -notcontains $field) { $union += $field } }
    $result.Declared = $declared
    $result.Fields = $union
    if ($budget -gt 0) { $result.BudgetLines = $budget }
    $result.Sections = @($sections)
    $result.MissingSections = @($script:WikiSchemaSections | Where-Object { $sections -notcontains $_ })
    $result.Ops = @($ops.Keys)
    $result.MissingOps = @($script:WikiSchemaOps | Where-Object { -not $ops.ContainsKey($_) })
    $result.OpsNoVerdict = @($ops.Keys | Where-Object { -not $ops[$_] } | Sort-Object)
    # The file governs the pages, so it must declare the fields the shape carries: a page field no
    # section names is a rule enforced by the instrument and written down nowhere (F10).
    $result.DeclaredShapeFields = @($script:WikiPageFields | Where-Object { $declared -contains $_ })
    $result.UndeclaredFields = @($script:WikiPageFields | Where-Object { $declared -notcontains $_ })
    # Drift is judged only once the file exists. An absent schema is the legal pre-governance state
    # and is reported by schema_absent, not counted: otherwise the instrument fails for a rule that
    # has not been written yet, and the first page ever written could not pass.
    $result.Drift = 0
    if ($result.Exists) {
        $result.Drift = @($result.MissingSections).Count + @($result.MissingOps).Count +
                        @($result.OpsNoVerdict).Count + @($result.UndeclaredFields).Count }
    $result.Source = 'schema.md'
    return $result
}

function Get-WikiSchemaVocabularyLines {
    # Every run prints which rules it was measured against - the file, its source and its counts.
    param([string]$WikiFull, [string]$SchemaArg)
    $vocab = Get-WikiSchemaVocabulary -WikiFull $WikiFull -SchemaArg $SchemaArg
    $lines = @()
    $lines += ('schema = ' + (Get-WikiRelPath -Path $vocab.File) + '  source = ' + $vocab.Source)
    if (-not $vocab.Exists) {
        $lines += 'schema_absent = 1  (the governance layer has not written it: the builtin vocabulary of wiki-shape is in force, and every count below is measured against it)';
    }
    $lines += ('schema_fields = ' + @($vocab.Declared).Count + '  [' + (@($vocab.Declared) -join ' ') + ']' +
               '  (named by this file; the vocabulary pages are judged against is ' + @($vocab.Fields).Count + ')')
    # The stop condition the governance plan states is a pair, so it is printed as one: of the fields
    # the shape carries, how many does this file declare. 12 of 13 is a rule nobody wrote down.
    $fieldsNote = ''
    if (@($vocab.UndeclaredFields).Count -gt 0) {
        $fieldsNote = '  UNDECLARED [' + (@($vocab.UndeclaredFields) -join ' ') + ']' }
    if ($vocab.Source -eq 'builtin') { $fieldsNote = '  - the builtin shape declares itself by definition' }
    $lines += ('fields = ' + @($vocab.DeclaredShapeFields).Count + ' of ' + @($script:WikiPageFields).Count +
               '  (page fields the shape carries that this file declares)' + $fieldsNote)
    if ($vocab.Exists) {
        $lines += ('schema_read = ' + $vocab.ReadLines + ' lines  declared_fields_read = ' + @($vocab.Declared).Count)
        $lines += ('schema_sections = ' + @($vocab.Sections).Count + ' of ' + @($script:WikiSchemaSections).Count +
                   '  [' + (@($script:WikiSchemaSections) -join ' ') + ']' +
                   $(if (@($vocab.MissingSections).Count -gt 0) { '  MISSING [' + (@($vocab.MissingSections) -join ' ') + ']' } else { '' }))
        $lines += ('schema_ops = ' + @($vocab.Ops).Count + ' of ' + @($script:WikiSchemaOps).Count +
                   '  [' + (@($script:WikiSchemaOps) -join ' ') + ']' +
                   $(if (@($vocab.MissingOps).Count -gt 0) { '  MISSING [' + (@($vocab.MissingOps) -join ' ') + ']' } else { '' }))
        $lines += ('schema_ops_no_verdict = ' + @($vocab.OpsNoVerdict).Count +
                   '  (an op row with no verdict line is an op no instrument can close)' +
                   $(if (@($vocab.OpsNoVerdict).Count -gt 0) { '  [' + (@($vocab.OpsNoVerdict) -join ' ') + ']' } else { '' })) }
    $lines += ('budget_lines = ' + $vocab.BudgetLines)
    return @{ Lines = $lines; Vocab = $vocab }
}


# ---------------------------------------------------------------- schema
function Invoke-LintSchema {
    param([string]$WikiFull, $Vocab)
    $lines = @()
    $set = Get-WikiPageSet -WikiRoot $WikiFull
    $pages = @($set.Pages)
    $offences = @()
    foreach ($page in $pages) {
        $fields = $page.Fields
        foreach ($key in @($fields.Keys)) {
            if ($Vocab.Fields -notcontains $key) {
                $offences += ('  ILLEGAL-FIELD ' + $page.Rel + ' : ' + $key + ' is declared in no schema') } }
        foreach ($key in $script:WikiPageRequiredFields) {
            if (-not $fields.Contains($key) -or ([string]$fields[$key]) -eq '') {
                $offences += ('  MISSING-FIELD ' + $page.Rel + ' : ' + $key + ' is required by the schema') } }
        $pageType = [string]$fields['PageType']
        if ($pageType -ne '' -and $script:WikiPageTypes -notcontains $pageType) {
            $offences += ('  BAD-ENUM ' + $page.Rel + ' : PageType ' + $pageType + ' is not a page type') }
        $status = [string]$fields['Status']
        if ($status -ne '' -and $script:WikiStatuses -notcontains $status) {
            $offences += ('  BAD-ENUM ' + $page.Rel + ' : Status ' + $status + ' is not a status') }
        $expected = ''
        if ($script:WikiFolderPageType.ContainsKey($page.Folder)) {
            $expected = [string]$script:WikiFolderPageType[$page.Folder] }
        if ($expected -ne '' -and $pageType -ne '' -and $pageType -ne $expected) {
            $offences += ('  WRONG-FOLDER ' + $page.Rel + ' : PageType ' + $pageType + ' sits in ' + $page.Folder + '/, which holds ' + $expected) } }
    $lines += ('pages = ' + $pages.Count + ' of ' + $set.All.Count + '  (files that carry the WikiPage shape)')
    $lines += ('strays = ' + $set.Strays.Count + '  (files in the wiki that are not pages: a stray is schema debt)')
    $lines += ('illegal_fields = ' + $offences.Count + '  (undeclared, missing, bad enum, or a page in the wrong folder)')
    # The pages are judged against the file, and the file is judged against the shape it governs: a
    # section or op row that was never written is F10, governance drift, and it fails the run. An
    # absent file reports schema_absent instead of failing, so the wiki is not red before 3 lands.
    $lines += ('governance_drift = ' + [int]$Vocab.Drift + '  (sections + ops + op rows with no verdict line +' +
               ' shape fields the file does not declare; 0 by definition while the file is absent)')
    $lines += $offences
    foreach ($stray in @($set.Strays)) {
        $lines += ('  STRAY ' + $stray.Rel + ' : ' + $stray.Reason) }
    if ($pages.Count -eq 0) {
        $lines += 'vacuity: pages=0 - illegal_fields = 0 here proves the parse ran, not that the wiki is legal (9)' }
    return @{ Lines = $lines; Fail = ($offences.Count + $set.Strays.Count + [int]$Vocab.Drift) }
}

# ---------------------------------------------------------------- provenance
$script:WikiProvenancePatterns = @{
    'url' = '(?i)(https?://|www\.)'
    'secret' = '(?i)\b(api[_-]?key|secret|token|password|passwd|credential)s?\b\s*[:=]\s*\S'
    'abspath' = '(?:^|[\s"''(=])((?:[A-Za-z]:[\\/])|(?:/)(?:home|Users|mnt|proc|var)/)'
}

function Get-WikiProvenanceHit {
    # A page holds claims and pointers, not the things themselves: no URL (the queue holds the
    # reference, 4.1), no secret (a tracked file is a published file), no absolute path (an anchor is
    # a relative path, 4.3 rule 5 - an absolute one resolves on the machine that wrote it and nowhere
    # else). The kind is printed with the hit, so a false positive is arguable, not invisible.
    param([Parameter(Mandatory = $true)]$Page)
    $hits = @()
    $numbered = @(Read-WikiLines -Path $Page.Abs)
    for ($i = 0; $i -lt $numbered.Count; $i++) {
        $line = [string]$numbered[$i]
        if ($line -match '^\s*(<!--|#)') { continue }
        foreach ($kind in @('url', 'secret', 'abspath')) {
            if ($line -match $script:WikiProvenancePatterns[$kind]) {
                $text = $line.Trim()
                if ($text.Length -gt 90) { $text = $text.Substring(0, 90) + '...' }
                $hits += ('  PROVENANCE ' + $Page.Rel + ':' + ($i + 1) + ' [' + $kind + '] : ' + $text) } } }
    return $hits
}

function Invoke-LintProvenance {
    param([string]$WikiFull)
    $lines = @()
    $set = Get-WikiPageSet -WikiRoot $WikiFull
    $pages = @($set.Pages)
    $hits = @()
    foreach ($page in $pages) { $hits += @(Get-WikiProvenanceHit -Page $page) }
    $sum = 0
    foreach ($page in $pages) { $sum = $sum + [int]$page.LineCount }
    $lines += ('pages = ' + $pages.Count + ' of ' + $set.All.Count)
    $lines += ('lines_read = ' + $sum)
    $lines += ('violations = ' + $hits.Count + '  (a URL, a secret or an absolute path inside a page)')
    $lines += $hits
    if ($pages.Count -eq 0) {
        $lines += 'vacuity: pages=0 - violations = 0 here proves the read ran, not that the wiki is clean (9)' }
    return @{ Lines = $lines; Fail = $hits.Count }
}

# ---------------------------------------------------------------- freshness
function Invoke-LintFreshness {
    # The same measure wiki-reconcile 'verify' weighs, read from the shared shape: a page stamped
    # after its evidence is a claim, and a claim with no stamp is unproven (6).
    param([string]$WikiFull, [string]$TreeFull)
    $lines = @()
    $set = Get-WikiPageSet -WikiRoot $WikiFull
    $pages = @($set.Pages)
    $unstamped = 0
    $stale = 0
    $undated = 0
    $evidenced = 0
    $judges = 0
    $offences = @()
    foreach ($page in $pages) {
        $fresh = Get-WikiPageFreshness -Page $page -TreeRoot $TreeFull
        $judges = $judges + [int]$fresh.JudgeCount
        if ($null -eq $fresh.LastVerified) {
            $unstamped = $unstamped + 1
            $offences += ('  UNSTAMPED ' + $page.Rel + ' : LastVerified is ' + ([string]$page.Fields['LastVerified']) + ', not a date')
            continue }
        if ($null -ne $fresh.Newest) { $evidenced = $evidenced + 1 }
        if ($null -eq $fresh.Newest) { $undated = $undated + 1; continue }
        if ($fresh.Newest -gt $fresh.LastVerified) {
            $stale = $stale + 1
            $offences += ('  STALE ' + $page.Rel + ' : LastVerified ' + $fresh.LastVerified.ToString('yyyy-MM-dd') +
                          ' is older than ' + $fresh.Basis + ' (' + $fresh.Newest.ToString('yyyy-MM-dd') + ')') } }
    $lines += ('pages = ' + $pages.Count + ' of ' + $set.All.Count)
    $lines += ('judges = ' + $judges + '  evidenced = ' + $evidenced + '  (a judge naming a file that exists dates the page)')
    $lines += ('unstamped = ' + $unstamped + '  (no LastVerified: the claim has never been checked)')
    $lines += ('stale = ' + $stale + '  (newest judge evidence is newer than the stamp)')
    $lines += ('undated = ' + $undated + '  (stamped, but no judge names a file: unproven here; informational)')
    $lines += $offences
    if ($pages.Count -eq 0) {
        $lines += 'vacuity: pages=0 - stale = 0 here proves the read ran, not that the wiki is fresh (9)' }
    return @{ Lines = $lines; Fail = ($unstamped + $stale) }
}

# ---------------------------------------------------------------- purpose
function Get-WikiPurposeScope {
    # The folders this instrument judges: the two layers the design owns, wiki and raw. Every other
    # folder under docs/ carries the same repo rule and the same pre-existing debt, so it is counted
    # and printed as debt, not judged - a permanently red gate is a gate nobody runs.
    param([string]$WikiFull, [string]$RawFull)
    $roots = @($WikiFull)
    if ($RawFull -ne '') { $roots += $RawFull }
    $judged = @()
    foreach ($root in $roots) {
        $judged += [pscustomobject]@{ Root = $root; Abs = $root }
        foreach ($dir in @(Get-ChildItem -LiteralPath $root -Recurse -Directory -Force)) {
            $judged += [pscustomobject]@{ Root = $root; Abs = $dir.FullName } } }
    return @($judged)
}

function Invoke-LintPurpose {
    param([string]$WikiFull, [string]$RawFull)
    $lines = @()
    $docsFull = Split-Path -Path $WikiFull -Parent
    $scope = @(Get-WikiPurposeScope -WikiFull $WikiFull -RawFull $RawFull)
    $missing = @()
    foreach ($folder in $scope) {
        if (-not (Test-Path -LiteralPath (Join-Path $folder.Abs 'purpose.md') -PathType Leaf)) {
            $missing += (Get-WikiRelPath -Path $folder.Abs) } }
    $lines += ('purpose_roots = ' + $scope.Count + '  (the wiki layer' + $(if ($RawFull -ne '') { ' and the raw layer' } else { '' }) + '; -RawRoot not given would shrink this)')
    $lines += ('folders = ' + $scope.Count)
    $lines += ('missing_purpose = ' + $missing.Count + ' of ' + $scope.Count + '  (repo rule: every folder states what it is)')
    foreach ($rel in $missing) { $lines += ('  MISSING-PURPOSE ' + $rel + '/purpose.md') }
    if ($docsFull -ne '' -and (Test-Path -LiteralPath $docsFull -PathType Container)) {
        $all = @(Get-ChildItem -LiteralPath $docsFull -Recurse -Directory -Force)
        $debt = @($all | Where-Object {
            -not (Test-Path -LiteralPath (Join-Path $_.FullName 'purpose.md') -PathType Leaf) })
        $lines += ('docs_debt = ' + $debt.Count + ' of ' + ($all.Count + 1) +
                   '  (other docs/ folders without purpose.md: the rule predates the wiki, counted not judged here)') }
    if ($scope.Count -eq 0) {
        $lines += 'vacuity: 0 folders in scope - missing_purpose = 0 here proves the scan ran, not that the rule holds (9)' }
    return @{ Lines = $lines; Fail = $missing.Count }
}

# ---------------------------------------------------------------- size
function Invoke-LintSize {
    param([string]$WikiFull, $Vocab)
    $lines = @()
    $set = Get-WikiPageSet -WikiRoot $WikiFull
    $pages = @($set.Pages)
    $over = @()
    $max = 0
    foreach ($page in $pages) {
        if ([int]$page.LineCount -gt $max) { $max = [int]$page.LineCount }
        if ([int]$page.LineCount -gt $Vocab.BudgetLines) {
            $over += ('  OVER-BUDGET ' + $page.Rel + ' : ' + $page.LineCount + ' lines over ' + $Vocab.BudgetLines) } }
    $lines += ('pages = ' + $pages.Count + ' of ' + $set.All.Count)
    $lines += ('budget_source = ' + $Vocab.Source + '  (a page longer than the budget must be split before it can be read, 6)')
    $lines += ('max_lines = ' + $max)
    $lines += ('over_budget = ' + $over.Count + '  (pages over ' + $Vocab.BudgetLines + ' lines)')
    $lines += $over
    if ($pages.Count -eq 0) {
        $lines += 'vacuity: pages=0 - over_budget = 0 here proves the count ran, not that the wiki is short (9)' }
    return @{ Lines = $lines; Fail = $over.Count }
}

# ---------------------------------------------------------------- dispatcher
# Requiredness is tested here, not bound by a Mandatory attribute, so a bare run prints the contract
# instead of prompting (docs/agent/TOOL-FORMAT.md). $WikiRoot and $Report are the only args every
# subcommand needs; freshness adds -TreeRoot (judge artifacts are tree-relative) and purpose takes
# -RawRoot to widen its scope. Both are printed by the run that used them.
$subcommands = @('schema', 'provenance', 'freshness', 'purpose', 'size')
$usage = 'wiki-lint.ps1 -Command <schema|provenance|freshness|purpose|size> -WikiRoot <dir> [-RawRoot <dir>] [-TreeRoot <dir>] [-Schema <file>] -Report <file>   (-Help prints this)'
$argLines = @('  required: -WikiRoot and -Report on every subcommand; -TreeRoot additionally for freshness,',
              '  because a judge names a build artifact relative to the tree root.',
              '  derived path: <WikiRoot>/schema.md, unless -Schema names another - it is resolved, read and',
              '  printed on every run, together with whether it existed. It does not until the governance',
              '  layer writes it, in which case the builtin vocabulary of wiki-shape is in force and the run',
              '  says so: a lint that silently invented its own rules is the F1 shape.',
              '  optional: -RawRoot widens purpose over the raw layer too (the queue folder is a folder).')
if ($Help) {
    Exit-WikiHelp -ToolName 'wiki-lint' -Subcommands $subcommands -Usage $usage -ArgLines $argLines }
if ([string]::IsNullOrWhiteSpace($Command)) {
    Exit-WikiUsage -Usage $usage -Missing 'Command' -ExtraLines @('  subcommand: ' + ($subcommands -join ' | ')) }
if ([string]::IsNullOrWhiteSpace($Report)) { Exit-WikiUsage -Usage $usage -Missing 'Report' }
if ([string]::IsNullOrWhiteSpace($WikiRoot)) { Exit-WikiUsage -Usage $usage -Missing 'WikiRoot' }
$wikiFull = Resolve-WikiRequiredRoot -Name 'WikiRoot' -Value $WikiRoot -Usage $usage
$rawFull = ''
if (-not [string]::IsNullOrWhiteSpace($RawRoot)) {
    $rawFull = Resolve-WikiRequiredRoot -Name 'RawRoot' -Value $RawRoot -Usage $usage }
$treeFull = ''
if ($Command -eq 'freshness') {
    if ([string]::IsNullOrWhiteSpace($TreeRoot)) {
        Exit-WikiUsage -Usage $usage -Missing 'TreeRoot (freshness weighs a page stamp against the artifacts its Judges: name, and those paths are tree-relative)' }
    $treeFull = Resolve-WikiRequiredRoot -Name 'TreeRoot' -Value $TreeRoot -Usage $usage }
$vocabRun = Get-WikiSchemaVocabularyLines -WikiFull $wikiFull -SchemaArg $Schema
switch -CaseSensitive ($Command) {
    'schema' { $run = Invoke-LintSchema -WikiFull $wikiFull -Vocab $vocabRun.Vocab }
    'provenance' { $run = Invoke-LintProvenance -WikiFull $wikiFull }
    'freshness' { $run = Invoke-LintFreshness -WikiFull $wikiFull -TreeFull $treeFull }
    'purpose' { $run = Invoke-LintPurpose -WikiFull $wikiFull -RawFull $rawFull }
    'size' { $run = Invoke-LintSize -WikiFull $wikiFull -Vocab $vocabRun.Vocab }
    default { Exit-WikiUsage -Usage $usage -Missing ('Command (unknown: ' + $Command + ')') }
}
if ($null -eq $run) {
    Exit-WikiUsage -Usage $usage -Missing ('internal (subcommand ' + $Command + ' returned no result)') }
$out = @()
$out += ('wiki-lint command=' + $Command + '  shape=' + $script:WikiShapeVersion)
$out += ('wiki_root = ' + (Get-WikiRelPath -Path $wikiFull) + $(if ($rawFull -ne '') { '  raw_root = ' + (Get-WikiRelPath -Path $rawFull) } else { '' }) + $(if ($treeFull -ne '') { '  tree_root = ' + (Get-WikiRelPath -Path $treeFull) } else { '' }))
$out += $vocabRun.Lines
$out += $run.Lines
$out += ('VERDICT fail=' + $run.Fail)
Write-WikiRun -ReportPath $Report -Lines $out
if ($run.Fail -gt 0) { exit 1 }
exit 0

