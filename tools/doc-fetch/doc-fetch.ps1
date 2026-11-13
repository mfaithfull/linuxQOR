$ErrorBehavior = 'Off'
# doc-fetch - one curl per URL the docref probe marked resolved, Return value
# section verbatim. Generalised from the eight <slug>_fetch clones in tmp/.
# args (in order, all optional): want slug take skip slice [prefix] [dir]
$want = 'misc.cpp'
$slug = 'misc'
$take = 10
$skip = 0
$slice = 1
$prefix = '_tmp_u2_'
$dir = 'c:\RAID0\Develop\linuxQOR\tmp'
$arg = @($Args)
if ($arg.Count -ge 1) { $want = [string] $arg[0] }
if ($arg.Count -ge 2) { $slug = [string] $arg[1] }
if ($arg.Count -ge 3) { $take = [int] $arg[2] }
if ($arg.Count -ge 4) { $skip = [int] $arg[3] }
if ($arg.Count -ge 5) { $slice = [int] $arg[4] }
if ($arg.Count -ge 6) { $prefix = [string] $arg[5] }
if ($arg.Count -ge 7) { $dir = [string] $arg[6] }
$tab = "$dir\${prefix}endpoint_table.csv"
$pro = "$dir\${prefix}docref_probe.txt"
$cache = "$dir\${prefix}probe_cache.txt"
$tag2 = ''
if ($slice -ge 2) { $tag2 = '_s' + [string] $slice }
$rep = "$dir\${prefix}${slug}_fetch" + $tag2 + '.out.txt'

$proMap = @{}
foreach ($pl in (Get-Content -LiteralPath $pro | Select-Object -Skip 4)) {
  $p = $pl -split '\|'
  if ($p.Count -lt 8) { continue }
  if ($p[1] -ne $want) { continue }
  $u = $p[5]
  if (-not $u) { $u = $p[4] }
  $stt = $p[3]
  if ($stt -ne 'resolved' -and $p[5] -and ($p[5] -like 'http*') -and ($p[5] -ne $p[4])) { $stt = 'resolved-via' }
  $proMap[$p[0]] = @{ 'status' = $stt; 'url' = $u; 'tag' = $p[7] }
}
$cacheMap = @{}
foreach ($cl in (Get-Content -LiteralPath $cache | Select-Object -Skip 1)) {
  $cp = $cl -split '::'
  if ($cp.Count -lt 4) { continue }
  if ($cp[1] -eq $want) { $cacheMap[$cp[0]] = (($cp | Select-Object -Skip 3) -join '::') }
}
$stems = @()
$seen = 0
foreach ($l in (Get-Content -LiteralPath $tab | Select-Object -Skip 1)) {
  $f = $l -split '\|'
  if ($f.Count -lt 12) { continue }
  if ($f[1] -ne $want) { continue }
  if ($seen -lt $skip) { $seen++; continue }
  $stems += , @($f[0], $f[2], $f[3], $f[4], $f[7])
  if ($stems.Count -ge $take) { break }
}
function GetSection($raw) {
  $pos = @()
  $s = 0
  while ($true) {
    $k = $raw.IndexOf('<h2', $s)
    if ($k -lt 0) { break }
    $pos += $k
    $s = $k + 3
  }
  $pos = @($pos) + $raw.Length
  for ($j = 0; $j -lt $pos.Count - 1; $j++) {
    $chunk = $raw.Substring($pos[$j], $pos[$j + 1] - $pos[$j])
    $txt = ($chunk -replace '<[^>]+>', ' ' -replace '&nbsp;', ' ' -replace '&amp;', '&' -replace '\s+', ' ').Trim()
    if ($txt -like 'Return value*') { return $txt }
  }
  return ''
}
$ok = 0
$bad = 0
$np = 0
$lines = @('doc-fetch slice batch - one curl per URL the probe marked resolved (a row whose page lives only under a suffixed twin slug is fetched there and printed CORRECTED), Return value section verbatim; a row with no page at all is booked NO-PAGE and decided from the probe line.')
$lines += ('CONTRACT want=' + $want + ' slug=' + $slug + ' take=' + $take + ' skip=' + $skip + ' slice=' + $slice + ' prefix=' + $prefix + ' dir=' + $dir)
$lines += ('PATHS table=' + $tab + ' probe=' + $pro + ' cache=' + $cache + ' report=' + $rep + ' html=' + ("$dir\${prefix}${slug}_<stem lower>.html"))
$lines += ('ROWS selected=' + $stems.Count + ' first=' + ($stems[0][0]) + ' last=' + ($stems[$stems.Count - 1][0]))
foreach ($st in $stems) {
  $stem = $st[0]
  $pr = $proMap[$stem]
  if (-not $pr) { $lines += ('NOPROBE ' + $stem); $bad++; continue }
  if ($pr['status'] -eq 'resolved-via') { $lines += ('CORRECTED ' + $stem + ' url=' + $pr['url']) }
  if ($pr['status'] -ne 'resolved' -and $pr['status'] -ne 'resolved-via') {
    $cq = $cacheMap[$stem]
    if ($cq) {
      $ok++
      $lines += ('FROM-CACHE ' + $stem + ' ' + $st[1] + ' pub=' + $st[2] + ' sdk=' + $st[3] + ' callers=' + $st[4] + ' probe=' + $pr['status'] + ' cache_len=' + $cq.Length)
      $lines += ('SECTION ' + $stem + ': Return value ' + $cq)
      continue
    }
    $np++
    $lines += ('NO-PAGE ' + $stem + ' ' + $st[1] + ' pub=' + $st[2] + ' sdk=' + $st[3] + ' callers=' + $st[4] + ' probe=' + $pr['status'] + ' tag=' + $pr['tag'] + ' - no Return value text to fetch; the row is decided from the probe line and its docQuote stays -')
    continue
  }
  $html = "$dir\${prefix}${slug}_" + $stem.ToLower() + '.html'
  $code = curl.exe -s -L --max-time 45 -A 'Mozilla/5.0' -o $html -w '%{http_code}' $pr['url']
  $raw = Get-Content -LiteralPath $html -Raw
  $sec = GetSection $raw
  $cq = $cacheMap[$stem]
  $same = if ($cq -eq ($sec -replace '^Return value ', '')) { 'match-cache' } else { 'differs-from-cache' }
  if ($code -ne '200') { $lines += ('HTTP-BAD ' + $stem + ' http=' + $code); $bad++; continue }
  $ok++
  $lines += ('FETCH ' + $stem + ' ' + $st[1] + ' pub=' + $st[2] + ' sdk=' + $st[3] + ' callers=' + $st[4] + ' http=' + $code + ' bytes=' + $raw.Length + ' section_len=' + $sec.Length + ' ' + $same)
  $lines += ('SECTION ' + $stem + ': ' + $sec)
  if ($cq -and $same -eq 'differs-from-cache') { $lines += ('CACHE ' + $stem + ': ' + $cq) }
}
$lines += ('fetched=' + $ok + ' of ' + $stems.Count + '  bad=' + $bad + '  no-page=' + $np)
$lines += ('VERDICT fail=' + [int](($bad -gt 0)))
Set-Content -LiteralPath $rep -Value $lines
$lines
