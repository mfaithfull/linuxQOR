#Requires -Version 7.0
<#
.SYNOPSIS
  wiki-shape - the one shape every wiki instrument reads (docs/WikiDesign.md 5).
.DESCRIPTION
  The parse of front matter and fields, the page listing, the queue entries, the target
  declaration, the anchor resolver and the judge vocabulary - owned here so five instruments
  share one definition (LayerInstruments: "every instrument reads one shared shape and no
  instrument opens prose"). Dot-source this file; it holds no param block, so dot-sourcing
  defines functions and script constants and touches nothing. The CLI over this shape is
  tools/wiki-census/wiki-census.ps1.
  The YAML subset the shape allows (docs/WikiDesign.md 5.1): a top-level `Field: value`, a
  block list (`Field:` then `- item` at any indent), a flow list (`Field: [a, b]`), the empty
  list written as `Field:` or `Field: []`, and a list of inline maps
  (`- {k: v, k2: v2}`). No anchors, no multiline scalars, no comment after a value, and no
  comma inside a quoted flow item - a page that needs them is a shape violation, which is the
  point: the shape is small so that five instruments can read it identically.
#>

# ---------------------------------------------------------------- the one shape
$script:WikiShapeVersion = 'wiki-shape-5'
# Repo root = two levels above tools/wiki-census. Every printed path is relative to it, so a
# report reads `docs/raw/pending.md` and not a machine-specific absolute path.
$script:WikiRepoRoot = Split-Path -Path (Split-Path -Path $PSScriptRoot -Parent) -Parent
# Root-level wiki files that are NOT pages (docs/WikiDesign.md sections 5 and 6).
$script:WikiNavFiles = @('purpose.md','schema.md','index.md','log.md','meta.md','gaps.md',
                         'architecture.md','dependencies.md','tour.md')
# The taxonomy folders of section 5; 'archive' is a lifecycle folder, not a category.
$script:WikiCategoryFolders = @('modules','symbols','concepts','invariants','decisions','gotchas','questions','archive')
$script:WikiFolderPageType = @{ modules = 'module'; symbols = 'symbol'; concepts = 'concept'
                                invariants = 'invariant'; decisions = 'decision'; gotchas = 'gotcha'
                                questions = 'question' }
$script:WikiPageTypes = @('module','symbol','concept','invariant','decision','gotcha','question')
$script:WikiStatuses = @('current','stale','superseded','draft','retired')
$script:WikiKinds = @('in-tree','out-of-tree','event','artifact')
$script:WikiProducers = @('hook','manual','learning','refresh','reconcile')
# The nine queue fields, in order - docs/raw/pending.md "Entry shape".
$script:WikiQueueFields = @('id','added_by','kind','path','anchor','pin','verified','target','why')
# Page front matter of 5.1 plus Summary (section 6) and Target (target granularity, 13 Q2).
$script:WikiPageFields = @('Type','PageType','Title','Anchors','Sources','GraphNodes','Status',
                           'SupersededBy','Claims','LastVerified','Judges','Summary','Target')
$script:WikiPageListFields = @('Anchors','Sources','GraphNodes','Judges')
$script:WikiPageRequiredFields = @('Type','PageType','Title','Status','LastVerified','Judges')
$script:WikiPageBudgetLines = 200
# Judges the four rule sections of docs/agent/verification.md do not name. Printed, not hidden:
# docs/WikiDesign.md section 11 and the BuildLLMWiki decision "an invariant page's Judges are a
# build, a test binary or a dumpbin call". schema.md (root item 3) is where this becomes a rule.
$script:WikiJudgeExtraHeads = @('dumpbin')

# ---------------------------------------------------------------- text plumbing
function Read-WikiLines {
    # One read path for every instrument: utf8, BOM stripped, always an array.
    param([Parameter(Mandatory = $true)][string]$Path)
    if (-not (Test-Path -LiteralPath $Path -PathType Leaf)) { return @() }
    $raw = @(Get-Content -LiteralPath $Path -Encoding utf8)
    $clean = foreach ($line in $raw) {
        if ($null -eq $line) { '' } else { ([string]$line).TrimStart([char]0xFEFF) }
    }
    return @($clean)
}

function Get-WikiRelPath {
    # Forward-slash, Base-relative when possible - anchors are relative paths (4.3 rule 5).
    # No Base means "relative to the repo root", which is how every report prints a path.
    param([Parameter(Mandatory = $true)][string]$Path, [string]$Base = '')
    $full = $Path
    try { $full = (Resolve-Path -LiteralPath $Path -ErrorAction Stop).Path } catch {
        if ($Path -notmatch '^[A-Za-z]:[\\/]') { $full = Join-Path (Get-Location).Path $Path } }
    if ($Base -eq '') { $Base = [string]$script:WikiRepoRoot }
    $sep = [string][IO.Path]::DirectorySeparatorChar
    if (-not $Base.EndsWith($sep)) { $Base = $Base + $sep }
    if ($Base -ne $sep -and $full.StartsWith($Base, [StringComparison]::OrdinalIgnoreCase)) {
        $full = $full.Substring($Base.Length).TrimStart('\','/') }
    return ($full -replace '\\', '/')
}

function ConvertFrom-WikiInlineMap {
    # "{kind: in-tree, path: src/x, symbol: qor::y, verified: 2026-11-08}" -> hashtable
    param([string]$Text)
    $result = @{}
    $body = $Text.Trim()
    if ($body.StartsWith('{') -and $body.EndsWith('}')) { $body = $body.Substring(1, $body.Length - 2) }
    foreach ($part in ($body -split ',')) {
        if ($part -match '^\s*([A-Za-z_][A-Za-z0-9_]*)\s*:\s*(.*?)\s*$') {
            $value = ($Matches[2] -replace '^"(.*)"$', '$1' -replace "^'(.*)'$", '$1')
            if ($value -ne '') { $result[$Matches[1]] = $value } } }
    return $result
}

function ConvertFrom-WikiFrontMatter {
    # front-matter lines (fences removed) -> ordered hashtable of scalar | list | list-of-maps
    param([string[]]$Lines)
    $fields = [ordered]@{}
    $currentKey = ''
    foreach ($line in $Lines) {
        if ($line -match '^\s*$' -or $line -match '^\s*#') { continue }
        if ($line -cmatch '^\s*-\s+(.*)$') {
            # A block-list item, at any indent under its key (YAML block lists are indented).
            if ($currentKey -eq '') { continue }
            $item = $Matches[1].Trim()
            if ($item.StartsWith('{')) {
                [void]$fields[$currentKey].Add((ConvertFrom-WikiInlineMap -Text $item)) }
            else {
                [void]$fields[$currentKey].Add(($item -replace '^"(.*)"$', '$1')) }
            continue }
        if ($line -cmatch '^([A-Za-z_][A-Za-z0-9_]*):(?:\s*(.*))?$') {
            $key = $Matches[1]
            $val = ''
            if ($null -ne $Matches[2]) { $val = $Matches[2].Trim() }
            if ($val -eq '' -or $val -eq '[]') {
                # `Field:` and `Field: []` are both the empty list in this subset of YAML.
                $fields[$key] = [System.Collections.Generic.List[object]]::new()
                $currentKey = $(if ($val -eq '') { $key } else { '' }) }
            elseif ($val.StartsWith('[') -and $val.EndsWith(']')) {
                # A flow list: `- ["a", b]`. A comma inside a quoted item is not split - see the
                # subset note in this file's header.
                $list = [System.Collections.Generic.List[object]]::new()
                $inner = $val.Substring(1, $val.Length - 2)
                foreach ($item in ($inner -split '(?<!\\),')) {
                    $text = $item.Trim()
                    if ($text -eq '') { continue }
                    [void]$list.Add(($text -replace '^"(.*)"$', '$1' -replace "^'(.*)'$", '$1')) }
                $fields[$key] = $list
                $currentKey = '' }
            else {
                $fields[$key] = ($val -replace '^"(.*)"$', '$1')
                $currentKey = '' }
            continue }
    }
    return $fields
}

function Split-WikiDocument {
    # -> @{ HasFrontMatter; Fields; BodyStartLine }. Front matter must start at line 1.
    param([string[]]$Lines)
    $empty = @{ HasFrontMatter = $false; Fields = [ordered]@{}; BodyStartLine = 0 }
    if ($Lines.Count -lt 2) { return $empty }
    if ($Lines[0] -cnotmatch '^---\s*$') { return $empty }
    $end = -1
    for ($i = 1; $i -lt $Lines.Count; $i++) {
        if ($Lines[$i] -cmatch '^---\s*$') { $end = $i; break } }
    if ($end -lt 0) { return $empty }
    $fm = @()
    if ($end -gt 1) { $fm = @($Lines[1..($end - 1)]) }
    return @{ HasFrontMatter = $true; Fields = (ConvertFrom-WikiFrontMatter -Lines $fm); BodyStartLine = $end + 1 }
}



# ---------------------------------------------------------------- pages
function Get-WikiPage {
    # Every .md under the wiki root, parsed. Classification is the caller's job (strays are data).
    param([Parameter(Mandatory = $true)][string]$WikiRoot)
    $pages = @()
    $rootFull = (Resolve-Path -LiteralPath $WikiRoot).Path
    $files = @(Get-ChildItem -LiteralPath $rootFull -Recurse -Filter '*.md' -File -Force)
    foreach ($file in $files) {
        $rel = Get-WikiRelPath -Path $file.FullName -Base $rootFull
        $parts = @($rel -split '/')
        $folder = ''
        if ($parts.Count -ge 2) { $folder = $parts[0] }
        $lines = @(Read-WikiLines -Path $file.FullName)
        $doc = Split-WikiDocument -Lines $lines
        $pages += [pscustomobject]@{
            Rel = $rel; Abs = $file.FullName; Folder = $folder; Name = $file.Name
            Stem = [System.IO.Path]::GetFileNameWithoutExtension($file.Name)
            Depth = $parts.Count; LineCount = $lines.Count
            IsPurpose = ($file.Name -eq 'purpose.md')
            HasFrontMatter = $doc.HasFrontMatter
            Fields = $doc.Fields; BodyStartLine = $doc.BodyStartLine
            Body = @($lines | Select-Object -Skip $doc.BodyStartLine)
        }
    }
    return $pages
}

function Get-WikiPageClass {
    # page | nav | purpose | stray - wiki-census 'pages' and wiki-lint 'schema' share this.
    param([Parameter(Mandatory = $true)]$Page)
    if ($Page.Depth -eq 1) {
        if ($Page.IsPurpose) { return @{ Kind = 'purpose'; Reason = '' } }
        if ($script:WikiNavFiles -contains $Page.Name) { return @{ Kind = 'nav'; Reason = '' } }
        return @{ Kind = 'stray'; Reason = 'root .md that is not a nav file of section 6' } }
    if ($script:WikiCategoryFolders -notcontains $Page.Folder) {
        return @{ Kind = 'stray'; Reason = ('folder is not in the taxonomy: ' + $Page.Folder) } }
    if ($Page.IsPurpose) { return @{ Kind = 'purpose'; Reason = '' } }
    if (-not $Page.HasFrontMatter) { return @{ Kind = 'stray'; Reason = 'no front matter' } }
    if (([string]$Page.Fields['Type']) -cne 'WikiPage') {
        return @{ Kind = 'stray'; Reason = ('Type is ' + ([string]$Page.Fields['Type']) + ', not WikiPage') } }
    return @{ Kind = 'page'; Reason = '' }
}

function Get-WikiPageSet {
    # The classification every instrument starts from: files, and which of them are pages.
    # -> @{ All; Pages; Strays; NavCount; PurposeCount }  (Strays is @{ Rel; Reason })
    param([Parameter(Mandatory = $true)][string]$WikiRoot)
    $all = @(Get-WikiPage -WikiRoot $WikiRoot)
    $pages = @()
    $strays = @()
    $navs = 0
    $purposes = 0
    foreach ($page in $all) {
        $class = Get-WikiPageClass -Page $page
        if ($class.Kind -eq 'page') { $pages += $page }
        elseif ($class.Kind -eq 'stray') { $strays += , @{ Rel = $page.Rel; Reason = $class.Reason } }
        elseif ($class.Kind -eq 'nav') { $navs = $navs + 1 }
        else { $purposes = $purposes + 1 } }
    return @{ All = $all; Pages = $pages; Strays = $strays; NavCount = $navs; PurposeCount = $purposes }
}

function Get-WikiFieldList {
    param($Fields, [string]$Key)
    if ($null -eq $Fields) { return @() }
    if (-not $Fields.Contains($Key)) { return @() }
    $val = $Fields[$Key]
    if ($val -is [System.Collections.IEnumerable] -and $val -isnot [string]) { return @($val) }
    return @($val)
}

function Get-WikiAnchorRecord {
    # A page's Anchors: entries, normalised to the queue's words (path / anchor / pin / verified).
    param($Fields)
    $out = @()
    foreach ($entry in (Get-WikiFieldList -Fields $Fields -Key 'Anchors')) {
        if ($entry -isnot [System.Collections.IDictionary]) {
            $out += [pscustomobject]@{ Kind = 'unparsed'; Path = ''; Anchor = ([string]$entry)
                                       Pin = ''; Verified = ''; Symbol = '' }
            continue }
        $kind = 'in-tree'
        if ($entry.Contains('kind')) { $kind = [string]$entry['kind'] }
        $path = ''
        if ($entry.Contains('path')) { $path = [string]$entry['path'] }
        $symbol = ''
        if ($entry.Contains('symbol')) { $symbol = [string]$entry['symbol'] }
        $anchor = ''
        if ($entry.Contains('anchor')) { $anchor = [string]$entry['anchor'] }
        elseif ($symbol -ne '') { $anchor = ('#' + $symbol) }
        $verified = ''
        if ($entry.Contains('verified')) { $verified = [string]$entry['verified'] }
        $pin = ''
        if ($entry.Contains('pin')) { $pin = [string]$entry['pin'] }
        $out += [pscustomobject]@{ Kind = $kind; Path = $path; Anchor = $anchor; Pin = $pin
                                   Verified = $verified; Symbol = $symbol }
    }
    return $out
}

function Get-WikiWikilink {
    # [[target]], [[target|text]] and markdown .md links in a page body.
    param($Page)
    $links = @()
    $text = ($Page.Body -join "`n")
    foreach ($m in [regex]::Matches($text, '\[\[([^\]\|#]+)(?:\|[^\]]*)?\]\]')) {
        $links += [pscustomobject]@{ Page = $Page.Rel; Form = 'wikilink'; Target = $m.Groups.Item(1).Value.Trim() } }
    foreach ($m in [regex]::Matches($text, '\[[^\]]*\]\(([^)\s]+\.md)(?:#[^)]*)?\)')) {
        $links += [pscustomobject]@{ Page = $Page.Rel; Form = 'markdown'; Target = $m.Groups.Item(1).Value.Trim() } }
    return $links
}

function Resolve-WikiWikilink {
    # One link target -> the wiki-root-relative path it names, or '' when nothing matches.
    # Order: the docs/wiki/ prefix a page may write; then the target read from the wiki root (the
    # form that already names a folder, `modules/qor_old.md`, which is how a page cites a page in
    # another folder); then the page's own folder; then, for a bare stem only, every category
    # folder - a target that names a folder is never resolved by scanning folders.
    param([Parameter(Mandatory = $true)][string]$WikiRoot,
          [Parameter(Mandatory = $true)]$Link)
    $tries = @()
    $target = $Link.Target
    if ($target -match '^(?:docs/)?wiki/') {
        $tries += (Join-Path $WikiRoot ($target -replace '^(?:docs/)?wiki/', '')) }
    $name = ($target -replace '\.md$', '') + '.md'
    $tries += (Join-Path $WikiRoot $name)
    $pageFolder = (Split-Path -Path ($Link.Page -replace '/', [IO.Path]::DirectorySeparatorChar) -Parent)
    if ($null -ne $pageFolder -and $pageFolder -ne '') {
        $tries += (Join-Path (Join-Path $WikiRoot $pageFolder) $name) }
    if ($name -notmatch '/') {
        foreach ($folder in $script:WikiCategoryFolders) {
            $tries += (Join-Path (Join-Path $WikiRoot $folder) $name) } }
    foreach ($try in $tries) {
        # Wiki-root-relative, because that is the coordinate every page's Rel uses. Repo-relative
        # here would make an inbound-link match impossible without ever printing a miss.
        if (Test-Path -LiteralPath $try -PathType Leaf) { return (Get-WikiRelPath -Path $try -Base $WikiRoot) } }
    return ''
}

# ---------------------------------------------------------------- queue
function Get-WikiQueueEntry {
    # docs/raw/pending.md: an entry is nine lines whose first is '- id:' at column 0.
    param([Parameter(Mandatory = $true)][string]$QueueFile)
    $entries = @()
    if (-not (Test-Path -LiteralPath $QueueFile -PathType Leaf)) { return $entries }
    $lines = @(Read-WikiLines -Path $QueueFile)
    $cur = $null
    for ($i = 0; $i -lt $lines.Count; $i++) {
        $line = $lines[$i]
        if ($line -cmatch '^- id:\s*(\S.*?)\s*$') {
            if ($null -ne $cur) { $entries += $cur }
            $cur = [ordered]@{ LineNumber = ($i + 1); Id = $Matches[1]; Seen = @('id'); Malformed = '' }
            foreach ($field in $script:WikiQueueFields) { if ($field -ne 'id') { $cur[$field] = '' } }
            continue }
        if ($null -eq $cur) { continue }
        if ($line -cmatch '^  ([A-Za-z_][A-Za-z0-9_]*):\s*(.*?)\s*$') {
            $key = $Matches[1]
            $val = $Matches[2]
            if ($script:WikiQueueFields -notcontains $key) { continue }
            if ([string]$cur[$key] -ne '') { $cur['Malformed'] = 'duplicate field ' + $key; continue }
            $cur[$key] = ($val -replace '^"(.*)"$', '$1' -replace "^'(.*)'$", '$1')
            $cur['Seen'] = @($cur['Seen']) + @($key)
            continue }
        if ($line -cmatch '^\s+\S' -and $line -cnotmatch '^\s*(#|<!--)') {
            $cur['Malformed'] = 'continuation line, values must be one line: ' + $line.Trim() }
    }
    if ($null -ne $cur) { $entries += $cur }
    return $entries
}

function Test-WikiQueueEntryComplete {
    # All nine fields, in the declared order, with legal vocabulary. Returns '' when complete.
    param($Entry)
    $seen = @($Entry['Seen'])
    foreach ($field in $script:WikiQueueFields) {
        if ($seen -notcontains $field) { return ('missing field ' + $field) } }
    $order = @($seen | Where-Object { $script:WikiQueueFields -contains $_ })
    $want = @($script:WikiQueueFields | Where-Object { $seen -contains $_ })
    if (($order -join ',') -ne ($want -join ',')) { return 'fields out of order' }
    if ([string]$Entry['Malformed'] -ne '') { return [string]$Entry['Malformed'] }
    if ($script:WikiProducers -notcontains [string]$Entry['added_by']) {
        return ('added_by is not a producer: ' + [string]$Entry['added_by']) }
    if ($script:WikiKinds -notcontains [string]$Entry['kind']) {
        return ('kind is not a kind: ' + [string]$Entry['kind']) }
    return ''
}

# ---------------------------------------------------------------- targets
function Get-WikiTargetDeclaration {
    # The unit of wiki work (13 Q2): one 'set(target qor_X)' in src/**/CMakeLists.txt.
    param([Parameter(Mandatory = $true)][string]$SrcRoot)
    $decls = @()
    if (-not (Test-Path -LiteralPath $SrcRoot -PathType Container)) { return $decls }
    $files = @(Get-ChildItem -LiteralPath $SrcRoot -Recurse -Filter 'CMakeLists.txt' -File -Force)
    foreach ($file in $files) {
        $rel = Get-WikiRelPath -Path $file.FullName
        $absFile = (Resolve-Path -LiteralPath $file.FullName).Path
        $lines = @(Read-WikiLines -Path $file.FullName)
        for ($i = 0; $i -lt $lines.Count; $i++) {
            if ($lines[$i] -cmatch 'set\(\s*target\s+([A-Za-z_][A-Za-z0-9_]*)') {
                $decls += [pscustomobject]@{ Name = $Matches[1]; File = $rel; Abs = $absFile
                                             Line = ($i + 1)
                                             IsQor = ($Matches[1] -cmatch '^qor_') } } }
    }
    return $decls
}

# ---------------------------------------------------------------- anchors
$script:WikiLineCountCache = @{}
$script:WikiGrepCache = @{}
$script:WikiPathCommitCache = @{}

function Get-WikiFileLineCount {
    param([string]$Abs)
    if ($script:WikiLineCountCache.ContainsKey($Abs)) { return $script:WikiLineCountCache[$Abs] }
    $count = -1
    if (Test-Path -LiteralPath $Abs -PathType Leaf) { $count = @(Read-WikiLines -Path $Abs).Count }
    $script:WikiLineCountCache[$Abs] = $count
    return $count
}

function Test-WikiSymbolInFile {
    # Full symbol, else the last '::' component - a C++ qualified name is written both ways.
    param([string]$Abs, [string]$Symbol)
    $key = $Abs + '|' + $Symbol
    if ($script:WikiGrepCache.ContainsKey($key)) { return $script:WikiGrepCache[$key] }
    $result = @{ Found = $false; Form = '' }
    $lines = @(Read-WikiLines -Path $Abs)
    $forms = @($Symbol)
    if ($Symbol.Contains('::')) { $forms += (($Symbol -split '::') | Where-Object { $_ -ne '' } | Select-Object -Last 1) }
    foreach ($form in $forms) {
        if ($form -eq '') { continue }
        $pattern = '(?<![A-Za-z0-9_:])' + [regex]::Escape($form)
        foreach ($line in $lines) {
            if ($line -match $pattern) { $result = @{ Found = $true; Form = $form }; break } }
        if ($result.Found) { break } }
    $script:WikiGrepCache[$key] = $result
    return $result
}

function Resolve-WikiAnchor {
    # One resolver for every anchor of every kind - wiki-ref-check and wiki-reconcile share it.
    # -> @{ Status = resolved|dead|tmp|derived_only|unpinned|unknown; Reason = '' }
    param([Parameter(Mandatory = $true)]$Anchor,
          [Parameter(Mandatory = $true)][string]$TreeRoot)
    $path = [string]$Anchor.Path
    $kind = [string]$Anchor.Kind
    if ($path -match '(^|/|\\)tmp(/|\\|$)') {
        return @{ Status = 'tmp'; Reason = 'anchor into tmp/ (4.3 rule 5)' } }
    if ($kind -eq 'out-of-tree') {
        $pin = ([string]$Anchor.Pin).Trim()
        $verified = ([string]$Anchor.Verified).Trim()
        if ($pin -eq '') { return @{ Status = 'unpinned'; Reason = 'out-of-tree anchor with no pin (F11)' } }
        if ($verified -cnotmatch '^\d{4}-\d{2}-\d{2}$') {
            return @{ Status = 'unpinned'; Reason = 'out-of-tree anchor with no verified date (F11)' } }
        return @{ Status = 'resolved'; Reason = 'pinned; never fetched, reconcile is free (9(2))' } }
    if ($path -eq '' -and $kind -eq 'event') {
        $anchorText = [string]$Anchor.Anchor
        if ($anchorText -cmatch '^D\d+$') {
            $decisions = Join-Path $TreeRoot 'docs/agent/DECISIONS.md'
            if (Test-Path -LiteralPath $decisions -PathType Leaf) {
                $hit = @(Select-String -LiteralPath $decisions -Pattern ('(^|[^0-9])' + $anchorText + '([^0-9]|$)'))
                if ($hit.Count -ge 1) { return @{ Status = 'resolved'; Reason = $anchorText + ' in DECISIONS.md' } } }
            return @{ Status = 'dead'; Reason = $anchorText + ' not in DECISIONS.md' } }
        return @{ Status = 'unknown'; Reason = 'event anchor with no path: ' + $anchorText } }
    if ($path -eq '') { return @{ Status = 'dead'; Reason = 'anchor with no path' } }
    $abs = $path
    if ($path -notmatch '^[A-Za-z]:[\\/]') { $abs = Join-Path $TreeRoot $path }
    if (-not (Test-Path -LiteralPath $abs)) {
        return @{ Status = 'dead'; Reason = ('path does not exist: ' + $path) } }
    if ($kind -eq 'artifact' -or $kind -eq 'event') {
        return @{ Status = 'resolved'; Reason = ($kind + ' file exists') } }
    if (-not (Test-Path -LiteralPath $abs -PathType Leaf)) {
        return @{ Status = 'resolved'; Reason = 'directory exists (a module anchor)' } }
    $anchorText = [string]$Anchor.Anchor
    if ($anchorText -cmatch '^commit:(.+)$') {
        $sha = $Matches[1]
        git -C $TreeRoot cat-file -e ($sha + '^{commit}') 2>&1 | Out-Null
        if ($LASTEXITCODE -eq 0) { return @{ Status = 'resolved'; Reason = ('commit ' + $sha) } }
        return @{ Status = 'dead'; Reason = ('commit not in history: ' + $sha) } }
    $symbol = [string]$Anchor.Symbol
    if ($symbol -eq '' -and $anchorText -cmatch '^#(.+)$') { $symbol = $Matches[1] }
    if ($symbol -ne '') {
        $found = Test-WikiSymbolInFile -Abs $abs -Symbol $symbol
        if ($found.Found) { return @{ Status = 'resolved'; Reason = ('symbol ' + $found.Form + ' present') } }
        return @{ Status = 'dead'; Reason = ('symbol not in file: ' + $symbol) } }
    if ($anchorText -cmatch '^:?\d+(?:-\d+)?$') {
        $total = Get-WikiFileLineCount -Abs $abs
        $high = [int]($anchorText -replace '^:', '').Split('-')[1]
        if ($null -eq $high -or $high -eq 0) { $high = [int]($anchorText -replace '^:', '').Split('-')[0] }
        if ($high -gt $total) {
            return @{ Status = 'dead'; Reason = ('line ' + $high + ' past EOF (' + $total + ')') } }
        return @{ Status = 'derived_only'; Reason = ('line anchor only over ' + $total + ' lines (4.3 rule 1)') } }
    return @{ Status = 'resolved'; Reason = 'file exists, anchor unspecified' }
}

function Get-WikiPathLastCommit {
    # The tree's current pin for one path: '<date>,<short-sha>' from a single git call, cached.
    # This is the only place the instruments ask git what a path's head is.
    param([string]$TreeRoot, [string]$RelPath)
    if ($script:WikiPathCommitCache.ContainsKey($RelPath)) { return $script:WikiPathCommitCache[$RelPath] }
    $value = ''
    $out = @(git -C $TreeRoot log -1 --format='%ad,%h' --date=short -- $RelPath 2>$null)
    if ($LASTEXITCODE -eq 0 -and $out.Count -ge 1) { $value = ([string]$out[0]).Trim() }
    $script:WikiPathCommitCache[$RelPath] = $value
    return $value
}

function Get-WikiPathLastCommitDate {
    # 'YYYY-MM-DD' of that head, or '' when the path has no commit.
    param([string]$TreeRoot, [string]$RelPath)
    $pair = Get-WikiPathLastCommit -TreeRoot $TreeRoot -RelPath $RelPath
    if ($pair -eq '') { return '' }
    return (($pair -split ',')[0]).Trim()
}

# ---------------------------------------------------------------- judge vocabulary
function Get-WikiJudgeVocabulary {
    # docs/agent/verification.md is the vocabulary of green. Read it, never copy it (13 Q2, 11).
    # Only the rule sections ABOVE '## Results' are rules; the Results log is evidence.
    param([Parameter(Mandatory = $true)][string]$VerificationFile)
    $lines = @(Read-WikiLines -Path $VerificationFile)
    $resultsIdx = $lines.Count
    for ($i = 0; $i -lt $lines.Count; $i++) {
        if ($lines[$i] -cmatch '^##\s+Results\s*$') { $resultsIdx = $i; break } }
    $headings = @()
    $commands = @()
    $greenRule = ''
    $sections = @()
    $current = $null
    for ($i = 0; $i -lt $resultsIdx; $i++) {
        $line = $lines[$i]
        if ($line -cmatch '^##\s+\S') {
            $headings += $line
            $current = @{ Heading = $line; CommandCount = 0 }
            $sections += $current
            continue }
        if ($line -cmatch '^\s*#\s*green\s*=') { $greenRule = $line.Trim(); continue }
        if ($line -match '^\s*(cmake\s|clang-tidy|&\s*\S|\.\\|\./)') {
            $commands += $line.Trim()
            if ($null -ne $current) { $current['CommandCount'] = $current['CommandCount'] + 1 }
            continue } }
    # A rule section is READ when it yielded at least one command: '4 of 4' in a report means "every
    # section in the file above the log gave me something", which degrades honestly if the file moves.
    $sectionsRead = @($sections | Where-Object { $_['CommandCount'] -gt 0 }).Count
    if ($greenRule -ne '') { $sectionsRead = $sectionsRead + 1 }

    $heads = @()
    foreach ($command in $commands) {
        if ($command -match '^cmake\s+--build\b') { $heads += 'cmake --build' }
        elseif ($command -match '^cmake\s+-S\b') { $heads += 'cmake -S' }
        elseif ($command -match '^clang-tidy\b') { $heads += 'clang-tidy' }
        elseif ($command -match '\.exe\b') { $heads += 'test-exe' }
        else { $heads += ($command -split '\s+')[0] } }
    $logBullets = 0
    for ($i = $resultsIdx; $i -lt $lines.Count; $i++) { if ($lines[$i] -cmatch '^\s*- ') { $logBullets++ } }
    return @{
        File = $VerificationFile
        TotalLines = $lines.Count
        RuleHeadings = $headings
        RuleHeadingCount = $headings.Count
        GreenRule = $greenRule
        RuleSectionCount = ($headings.Count + $(if ($greenRule -ne '') { 1 } else { 0 }))
        RuleSectionsRead = $sectionsRead
        Sections = $sections
        Commands = $commands
        Heads = @($heads | Sort-Object -Unique)
        ExtraHeads = $script:WikiJudgeExtraHeads
        LogBulletsExcluded = $logBullets
    }
}

function Get-WikiJudgeHead {
    # The head of a judge command, mapped onto the vocabulary's words.
    param([string]$Judge)
    $text = $Judge.Trim()
    if ($text -match '^cmake\s+--build\b') { return 'cmake --build' }
    if ($text -match '^cmake\s+-S\b') { return 'cmake -S' }
    if ($text -match '^clang-tidy\b') { return 'clang-tidy' }
    if ($text -match '^dumpbin\b') { return 'dumpbin' }
    if ($text -match '\.exe\b') { return 'test-exe' }
    if ($text -ne '') { return (($text -split '\s+')[0]) }
    return '(empty judge)'
}

function Test-WikiJudgeInVocabulary {
    # -> @{ InVocab; Head; Extra } - Extra is the builtin allowance, printed and never hidden.
    param([string]$Judge, $Vocab)
    $head = Get-WikiJudgeHead -Judge $Judge
    if ($Vocab.Heads -contains $head) { return @{ InVocab = $true; Head = $head; Extra = $false } }
    if ($Vocab.ExtraHeads -contains $head) { return @{ InVocab = $true; Head = $head; Extra = $true } }
    return @{ InVocab = $false; Head = $head; Extra = $false }
}

# ---------------------------------------------------------------- prose anchors
function Get-WikiProseAnchorCandidate {
    # Backticked repo-relative paths in prose. Conservative: the parent directory must exist,
    # so a prose example (`docs/.../file.cpp`) is never reported as a dead anchor.
    param([Parameter(Mandatory = $true)][string]$AbsFile,
          [Parameter(Mandatory = $true)][string]$TreeRoot)
    $found = @()
    $lines = @(Read-WikiLines -Path $AbsFile)
    $rel = Get-WikiRelPath -Path $AbsFile
    for ($i = 0; $i -lt $lines.Count; $i++) {
        foreach ($m in [regex]::Matches($lines[$i], '`((?:src|docs|memory-bank|tools|test|examples|sdk|cmake)/[A-Za-z0-9_.\-/]+?\.[A-Za-z0-9]+)(?::\d+)?(?:#[^`]*)?`')) {
            $candidate = $m.Groups.Item(1).Value
            $abs = Join-Path $TreeRoot $candidate
            if (Test-Path -LiteralPath $abs -PathType Leaf) {
                $found += [pscustomobject]@{ File = $rel; Line = ($i + 1); Path = $candidate; Status = 'resolved' } }
            elseif (Test-Path -LiteralPath ([IO.Path]::GetDirectoryName($abs)) -PathType Container) {
                $found += [pscustomobject]@{ File = $rel; Line = ($i + 1); Path = $candidate; Status = 'dead' } } }
    }
    return $found
}

# ---------------------------------------------------------------- judge evidence
# Shared by wiki-reconcile (verify, refresh, audit) and wiki-lint (freshness): one page's claim
# against the evidence its Judges: name. Two copies of this is two notions of stale.
function Get-WikiJudgeEvidence {
    # The file a judge command names, when it names one. Its write date is the judge's own timestamp,
    # which is what keeps reconcile free (9(2)): freshness is read, not built, at session start.
    # -RunJudges executes the command in the tree root instead, then re-reads that timestamp.
    # -> @{ Judge; Artifact; Date; Ran; Green; Detail }
    param([string]$Judge, [string]$TreeRoot, [switch]$RunJudges)
    $artifact = ''
    $date = $null
    foreach ($token in ($Judge -split '[\s"]+')) {
        $clean = $token.TrimStart('&').Trim()
        if ($clean -eq '' -or $clean -match '^#') { continue }
        if ($clean -notmatch '(^[A-Za-z]:[\\/]|[\\/])') { continue }
        $candidate = ($clean -replace '\\', '/')
        $abs = $candidate
        if ($candidate -notmatch '^[A-Za-z]:') { $abs = Join-Path $TreeRoot $candidate }
        if (Test-Path -LiteralPath $abs -PathType Leaf) {
            $artifact = Get-WikiRelPath -Path $abs
            $date = (Get-Item -LiteralPath $abs).LastWriteTime.Date
            break } }
    $ran = $false
    $green = $false
    $detail = ''
    if ($RunJudges) {
        $ran = $true
        $previous = (Get-Location).Path
        try {
            Set-Location -LiteralPath $TreeRoot
            $output = @(Invoke-Expression $Judge 2>&1)
            if ($LASTEXITCODE -eq 0) { $green = $true }
            else {
                $tail = ''
                if (@($output).Count -ge 1) { $tail = ([string](@($output)[-1])) }
                $detail = 'exit ' + $LASTEXITCODE + ' : ' + $tail } }
        catch { $detail = 'could not run: ' + $_.Exception.Message }
        finally { Set-Location -LiteralPath $previous }
        if ($green -and $artifact -ne '') {
            $again = Join-Path $TreeRoot $artifact
            if (Test-Path -LiteralPath $again -PathType Leaf) {
                $date = (Get-Item -LiteralPath $again).LastWriteTime.Date } } }
    return @{ Judge = $Judge; Artifact = $artifact; Date = $date; Ran = $ran; Green = $green; Detail = $detail }
}

function Get-WikiPageFreshness {
    # One page's claim against its evidence: LastVerified: versus the newest judge timestamp.
    # -> @{ LastVerified; Newest; Basis; Evidenced; JudgeCount; Ran; Red; Details }
    param($Page, [string]$TreeRoot, [switch]$RunJudges)
    $judges = @(Get-WikiFieldList -Fields $Page.Fields -Key 'Judges')
    $newest = $null
    $basis = ''
    $evidenced = 0
    $ranCount = 0
    $red = 0
    $details = @()
    foreach ($judge in $judges) {
        $evidence = Get-WikiJudgeEvidence -Judge ([string]$judge) -TreeRoot $TreeRoot -RunJudges:$RunJudges
        if ($evidence.Ran) { $ranCount = $ranCount + 1 }
        if ($evidence.Ran -and -not $evidence.Green) {
            $red = $red + 1
            $details += ('  RED JUDGE ' + $Page.Rel + ' : ' + $evidence.Judge + ' : ' + $evidence.Detail)
            continue }
        if ($null -eq $evidence.Date) { continue }
        $evidenced = $evidenced + 1
        if ($null -eq $newest -or $evidence.Date -gt $newest) {
            $newest = $evidence.Date
            $basis = $evidence.Artifact } }
    return @{
        LastVerified = (Get-WikiDateOnly -Text ([string]$Page.Fields['LastVerified']))
        Newest = $newest
        Basis = $basis
        Evidenced = $evidenced
        JudgeCount = $judges.Count
        Ran = $ranCount
        Red = $red
        Details = $details
    }
}


# ---------------------------------------------------------------- run plumbing
function Exit-WikiUsage {
    # The tool-format contract: USAGE + MISSING args= + VERDICT fail=1 + exit 1, no file touched.
    param([Parameter(Mandatory = $true)][string]$Usage,
          [Parameter(Mandatory = $true)][string]$Missing,
          [string[]]$ExtraLines = @())
    Write-Output ('USAGE ' + $Usage)
    foreach ($extra in $ExtraLines) { Write-Output $extra }
    Write-Output ('MISSING args=' + $Missing)
    Write-Output 'VERDICT fail=1'
    exit 1
}

function Exit-WikiHelp {
    # -Help prints the subcommands and their count, and touches no file.
    param([Parameter(Mandatory = $true)][string]$ToolName,
          [Parameter(Mandatory = $true)][string[]]$Subcommands,
          [Parameter(Mandatory = $true)][string]$Usage,
          [string[]]$ArgLines = @())
    Write-Output ('tool = ' + $ToolName + '  shape = ' + $script:WikiShapeVersion)
    Write-Output ('USAGE ' + $Usage)
    foreach ($argLine in $ArgLines) { Write-Output $argLine }
    foreach ($sub in $Subcommands) { Write-Output ('  subcommand: ' + $sub) }
    Write-Output ('subcommands = ' + $Subcommands.Count + ' of ' + $Subcommands.Count)
    Write-Output 'VERDICT fail=0'
    exit 0
}

function Write-WikiRun {
    # Same lines to the console and to the report; the report path is printed, resolved.
    # An empty -Report must never degrade into a bind error and a green verdict: refuse loudly.
    param([string]$ReportPath,
          [Parameter(Mandatory = $true)][string[]]$Lines)
    if ([string]::IsNullOrWhiteSpace($ReportPath)) {
        foreach ($line in $Lines) { Write-Output $line }
        Write-Output 'ERROR report=refused (an empty -Report would write nothing while looking like a run)'
        Write-Output 'VERDICT fail=1'
        exit 1 }
    $dir = [IO.Path]::GetDirectoryName($ReportPath)
    if ($null -ne $dir -and $dir -ne '' -and -not (Test-Path -LiteralPath $dir -PathType Container)) {
        New-Item -ItemType Directory -Path $dir -Force | Out-Null }
    @($Lines) | Out-File -LiteralPath $ReportPath -Encoding utf8
    foreach ($line in $Lines) { Write-Output $line }
    Write-Output ('report = ' + (Get-WikiRelPath -Path $ReportPath))
}

function Resolve-WikiRequiredRoot {
    # A root that is absent or unreadable is a usage failure, not an empty read (no report, no side
    # effect). $Value is NOT Mandatory: a Mandatory [string] refuses to bind '' and throws a
    # non-terminating error, after which the caller carries on with $null and prints VERDICT fail=0.
    # That is the false green this function exists to prevent, so emptiness is tested, not bound.
    param([Parameter(Mandatory = $true)][string]$Name,
          [AllowEmptyString()][AllowNull()][string]$Value,
          [Parameter(Mandatory = $true)][string]$Usage,
          [switch]$File)
    if ([string]::IsNullOrWhiteSpace($Value)) { Exit-WikiUsage -Usage $Usage -Missing $Name }
    $kind = 'Container'
    if ($File) { $kind = 'Leaf' }
    if (-not (Test-Path -LiteralPath $Value -PathType $kind)) {
        Exit-WikiUsage -Usage $Usage -Missing ($Name + ' (not readable: ' + $Value + ')') }
    $resolved = Resolve-Path -LiteralPath $Value -ErrorAction SilentlyContinue
    if ($null -eq $resolved) {
        Exit-WikiUsage -Usage $Usage -Missing ($Name + ' (does not resolve: ' + $Value + ')') }
    return $resolved.Path
}

function Get-WikiDateOnly {
    # 'YYYY-MM-DD' -> DateTime; anything else -> $null. Never a silent parse.
    param([string]$Text)
    $value = ([string]$Text).Trim()
    if ($value -cnotmatch '^\d{4}-\d{2}-\d{2}$') { return $null }
    $parsed = [datetime]::MinValue
    if ([datetime]::TryParseExact($value, 'yyyy-MM-dd', [Globalization.CultureInfo]::InvariantCulture,
                                  [Globalization.DateTimeStyles]::None, [ref]$parsed)) { return $parsed.Date }
    return $null
}

# ---------------------------------------------------------------- census subcommands
function Invoke-WikiCensusPending {
    param($WikiRoot, $RawRoot)
    $lines = @()
    $fail = 0
    $queueFile = Join-Path $RawRoot 'pending.md'
    $seen = @(Select-String -LiteralPath $queueFile -Pattern '^- id: ')
    $entries = @(Get-WikiQueueEntry -QueueFile $queueFile)
    $complete = 0
    $malformed = @()
    foreach ($entry in $entries) {
        $reason = Test-WikiQueueEntryComplete -Entry $entry
        if ($reason -eq '') { $complete = $complete + 1 }
        else { $malformed += ('  entry line ' + $entry['LineNumber'] + ': ' + $reason) } }
    $lines += ('queue = ' + (Get-WikiRelPath -Path $queueFile))
    $lines += ('pending = ' + $complete + ' of ' + $seen.Count)
    $lines += ('entry_parse = ' + $entries.Count + ' of ' + $seen.Count + '  (id lines that became entries)')
    $lines += ('entry_fields = 9  [' + ($script:WikiQueueFields -join ' ') + ']')
    if ($entries.Count -ne $seen.Count) { $fail = $fail + 1; $lines += '  FAIL: an id line did not parse into an entry' }
    if ($complete -ne $entries.Count) { $fail = $fail + 1; $lines += $malformed }
    $srcIds = @{}
    foreach ($page in @(Get-WikiPage -WikiRoot $WikiRoot)) {
        if ((Get-WikiPageClass -Page $page).Kind -ne 'page') { continue }
        foreach ($source in (Get-WikiFieldList -Fields $page.Fields -Key 'Sources')) {
            if ($source -ne '') { $srcIds[$source] = 1 } } }
    $lines += ('consumed = ' + $srcIds.Count + '  (Sources ids held by pages; the entries themselves are deleted)')
    if ($seen.Count -eq 0) { $lines += 'vacuity: the queue is empty - the legal steady state, not a pass by itself' }
    return @{ Lines = $lines; Fail = $fail }
}

function Invoke-WikiCensusTarget {
    param($SrcRoot)
    $lines = @()
    $fail = 0
    # -File alone hides a folder named CMakeLists.txt (memory-bank/gotchas/powershell.md), so the
    # scan lists every entry of that name and counts what it could not read.
    $all = @(Get-ChildItem -LiteralPath $SrcRoot -Recurse -Force | Where-Object { $_.Name -eq 'CMakeLists.txt' })
    $files = @($all | Where-Object { -not $_.PSIsContainer })
    $unreadable = $all.Count - $files.Count
    $decls = @(Get-WikiTargetDeclaration -SrcRoot $SrcRoot)
    $qor = @($decls | Where-Object { $_.IsQor })
    $other = @($decls | Where-Object { -not $_.IsQor })
    $distinct = @($qor | ForEach-Object { $_.Name } | Sort-Object -Unique)
    $revalidated = 0
    foreach ($name in $distinct) {
        $decl = @($qor | Where-Object { $_.Name -eq $name })[0]
        $abs = [string]$decl.Abs
        $stillThere = @(Select-String -LiteralPath $abs -Pattern ('set\(\s*target\s+' + [regex]::Escape($name) + '\b'))
        if ($stillThere.Count -ge 1) { $revalidated = $revalidated + 1 } }
    $dupes = @()
    foreach ($group in ($qor | Where-Object { $null -ne $_ } | Group-Object -Property Name)) {
        if ($group.Count -gt 1) { $dupes += $group.Name } }
    $dupes = @($dupes | Sort-Object)
    $lines += ('src_root = ' + (Get-WikiRelPath -Path $SrcRoot))
    $lines += ('cmakelists = ' + $files.Count)
    $lines += ('scan_unreadable = ' + $unreadable + ' of ' + $all.Count + '  (entries named CMakeLists.txt that are not files)')
    $lines += ('declarations_qor = ' + $qor.Count)
    $lines += ('targets = ' + $revalidated + ' of ' + $distinct.Count + '  (distinct names re-read in their own CMakeLists.txt)')
    $lines += ('declarations_other = ' + $other.Count + '  (target names not starting qor_ - not wiki units; informational)')
    $lines += ('declared_twice = [' + ($dupes -join ',') + ']')
    if ($unreadable -gt 0) { $fail = $fail + 1; $lines += '  FAIL: an artifact named CMakeLists.txt could not be read - the scan is blind to it' }
    if ($distinct.Count -eq 0) {
        $fail = $fail + 1
        $lines += 'vacuity: targets = 0 over ' + $files.Count + ' CMakeLists.txt is a broken scan, not an empty wiki' }
    if ($revalidated -ne $distinct.Count) { $fail = $fail + 1; $lines += '  FAIL: a target name did not re-read in its CMakeLists.txt' }
    return @{ Lines = $lines; Fail = $fail }
}

function Invoke-WikiCensusCoverage {
    param($WikiRoot, $SrcRoot)
    $lines = @()
    $fail = 0
    $decls = @(Get-WikiTargetDeclaration -SrcRoot $SrcRoot)
    $targets = @($decls | Where-Object { $_.IsQor } | ForEach-Object { $_.Name } | Sort-Object -Unique)
    $modulePages = @()
    foreach ($page in @(Get-WikiPage -WikiRoot $WikiRoot)) {
        if ($page.Folder -ne 'modules' -or $page.IsPurpose) { continue }
        if ((Get-WikiPageClass -Page $page).Kind -ne 'page') { continue }
        $modulePages += $page }
    $pagedNames = @($modulePages | ForEach-Object { $_.Stem })
    $covered = @($targets | Where-Object { $pagedNames -contains $_ })
    $uncovered = @($targets | Where-Object { $pagedNames -notcontains $_ })
    $dangling = @($pagedNames | Where-Object { $targets -notcontains $_ })
    $lines += ('targets = ' + $targets.Count)
    $lines += ('module_pages = ' + $modulePages.Count)
    $lines += ('coverage = ' + $covered.Count + ' of ' + $targets.Count)
    $lines += ('uncovered_targets = ' + $uncovered.Count)
    $lines += ('dangling_module_pages = ' + $dangling.Count + '  [' + (($dangling | Select-Object -First 10) -join ',') + ']')
    $lines += 'note: coverage reports, it does not judge - the verdict pair is pending=0 + reconcile fail=0'
    if ($dangling.Count -gt 0) { $fail = $fail + 1; $lines += '  FAIL: a module page names no target in src/' }
    if ($targets.Count -eq 0) {
        $fail = $fail + 1
        $lines += 'vacuity: coverage = ' + $covered.Count + ' of 0 - there are no targets to be covered by, so this number means nothing' }
    return @{ Lines = $lines; Fail = $fail }
}

function Invoke-WikiCensusPage {
    param($WikiRoot)
    $lines = @()
    $fail = 0
    $set = Get-WikiPageSet -WikiRoot $WikiRoot
    $all = @($set.All)
    $pages = @($set.Pages)
    $strays = @($set.Strays)
    $navs = [int]$set.NavCount
    $purposes = [int]$set.PurposeCount
    $lines += ('wiki_root = ' + (Get-WikiRelPath -Path $WikiRoot))
    $lines += ('files = ' + $all.Count)
    $lines += ('pages = ' + $pages.Count)
    $lines += ('strays = ' + $strays.Count)
    $lines += ('nav_files = ' + $navs + '  purpose_files = ' + $purposes)
    foreach ($folder in $script:WikiCategoryFolders) {
        $count = @($pages | Where-Object { $_.Folder -eq $folder }).Count
        $lines += ('  ' + $folder + ' = ' + $count) }
    foreach ($stray in $strays) {
        $fail = $fail + 1
        $lines += ('  STRAY ' + $stray.Rel + ' : ' + $stray.Reason) }
    $missingNav = @()
    foreach ($name in @('schema.md','index.md','log.md','meta.md','gaps.md')) {
        if (-not (Test-Path -LiteralPath (Join-Path $WikiRoot $name) -PathType Leaf)) { $missingNav += $name } }
    $lines += ('nav_missing = ' + $missingNav.Count + ' of 5 [' + ($missingNav -join ' ') + ']  (informational: root items 3-4 write them)')
    if ($pages.Count -eq 0) { $lines += 'vacuity: pages=0 - every count here is over an empty wiki' }
    return @{ Lines = $lines; Fail = $fail }
}

