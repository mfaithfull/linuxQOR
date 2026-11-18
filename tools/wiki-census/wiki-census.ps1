#Requires -Version 7.0
<#
.SYNOPSIS
  wiki-census - the wiki's state as counts.
.DESCRIPTION
  Subcommands: pending | targets | coverage | pages (docs/WikiDesign.md 9).
  The shape this counts is defined once, in tools/wiki-census/wiki-shape.ps1, which this script
  dot-sources and which the other five wiki instruments dot-source too - "every instrument reads
  one shared shape" (docs/WikiDesign.md 5).
  Parameters are deliberately NOT [Mandatory]: a Mandatory parameter would prompt when a tool is
  run bare. Requiredness is enforced by the dispatcher, which prints USAGE + MISSING args= +
  VERDICT fail=1 + exit 1 without touching a file (docs/agent/TOOL-FORMAT.md, Contract).
  Reports go to the path given by -Report; tmp/ is the disposable place for them.
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

$_shape = Join-Path $PSScriptRoot 'wiki-shape.ps1'
if (-not (Test-Path -LiteralPath $_shape -PathType Leaf)) {
    Write-Output 'USAGE wiki-census.ps1 -Command <pending|targets|coverage|pages> -WikiRoot <dir> [-RawRoot <dir>] [-TreeRoot <dir>] -Report <file>   (-Help prints this)'
    Write-Output ('MISSING args=shared-shape (no wiki-shape.ps1 beside this script)')
    Write-Output 'VERDICT fail=1'
    exit 1
}
. $_shape

# ---------------------------------------------------------------- dispatcher
# The dispatcher. Requiredness lives here, not in a Mandatory attribute, so a bare run
# prints the contract instead of prompting (docs/agent/TOOL-FORMAT.md).
$subcommands = @('pending', 'targets', 'coverage', 'pages')
$usage = 'wiki-census.ps1 -Command <pending|targets|coverage|pages> -WikiRoot <dir> [-RawRoot <dir>] [-TreeRoot <dir>] -Report <file>   (-Help prints this)'
$argLines = @('  required: -WikiRoot always; -RawRoot for pending; -TreeRoot for targets and coverage;',
              '            -Report is the file the run writes (tmp/ is the disposable place for it)',
              '  <TreeRoot>/src is the only derived path; it is resolved and printed by every targets/coverage run.')
if ($Help) {
    Exit-WikiHelp -ToolName 'wiki-census' -Subcommands $subcommands -Usage $usage -ArgLines $argLines }
if ([string]::IsNullOrWhiteSpace($Command)) {
    Exit-WikiUsage -Usage $usage -Missing 'Command' -ExtraLines @('  subcommand: ' + ($subcommands -join ' | ')) }
if ([string]::IsNullOrWhiteSpace($Report)) {
    Exit-WikiUsage -Usage $usage -Missing 'Report' }
if ([string]::IsNullOrWhiteSpace($WikiRoot)) {
    Exit-WikiUsage -Usage $usage -Missing 'WikiRoot' }
$wikiFull = Resolve-WikiRequiredRoot -Name 'WikiRoot' -Value $WikiRoot -Usage $usage
$srcFull = ''
$rawFull = ''
switch -CaseSensitive ($Command) {
    'pending' {
        if ([string]::IsNullOrWhiteSpace($RawRoot)) { Exit-WikiUsage -Usage $usage -Missing 'RawRoot' }
        $rawFull = Resolve-WikiRequiredRoot -Name 'RawRoot' -Value $RawRoot -Usage $usage
        $run = Invoke-WikiCensusPending -WikiRoot $wikiFull -RawRoot $rawFull }
    'targets' {
        if ([string]::IsNullOrWhiteSpace($TreeRoot)) { Exit-WikiUsage -Usage $usage -Missing 'TreeRoot' }
        $treeFull = Resolve-WikiRequiredRoot -Name 'TreeRoot' -Value $TreeRoot -Usage $usage
        $srcFull = Resolve-WikiRequiredRoot -Name 'TreeRoot/src' -Value (Join-Path $treeFull 'src') -Usage $usage
        $run = Invoke-WikiCensusTarget -SrcRoot $srcFull }
    'coverage' {
        if ([string]::IsNullOrWhiteSpace($TreeRoot)) { Exit-WikiUsage -Usage $usage -Missing 'TreeRoot' }
        $treeFull = Resolve-WikiRequiredRoot -Name 'TreeRoot' -Value $TreeRoot -Usage $usage
        $srcFull = Resolve-WikiRequiredRoot -Name 'TreeRoot/src' -Value (Join-Path $treeFull 'src') -Usage $usage
        $run = Invoke-WikiCensusCoverage -WikiRoot $wikiFull -SrcRoot $srcFull }
    'pages' { $run = Invoke-WikiCensusPage -WikiRoot $wikiFull }
    default { Exit-WikiUsage -Usage $usage -Missing ('Command (unknown: ' + $Command + ')') }
}
$out = @()
$out += ('wiki-census command=' + $Command + '  shape=' + $script:WikiShapeVersion)
$out += ('cwd = ' + ((Get-Location).Path -replace '\\', '/'))
$out += $run.Lines
$out += ('VERDICT fail=' + $run.Fail)
Write-WikiRun -ReportPath $Report -Lines $out
if ($run.Fail -gt 0) { exit 1 }
exit 0
