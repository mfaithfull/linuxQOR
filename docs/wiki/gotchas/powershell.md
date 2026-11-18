---
Type: WikiPage
PageType: gotcha
Title: "powershell"
Status: current
LastVerified: 2026-11-18
Claims: 5
Summary: "The scan-shaped PowerShell traps in this repo: -Include silently counts 0 on a plain -Path, and every one of them hides behind a count that looks right."
Anchors:
  - { kind: in-tree, path: memory-bank/gotchas/powershell.md, symbol: parallel, verified: 2026-11-18 }
Judges:
  - Get-ChildItem -Path src/platform/os/windows/api_layer/user -Include *.cpp -File
---

# powershell

`memory-bank/gotchas/powershell.md` is a running record of shell traps hit while working this tree;
this page names the scan-shaped ones and what they cost now. The file is the record, this is the
pointer.

- The flagship trap, and the reproduction in `Judges:`: `Get-ChildItem -Include <pattern>` is
  silently **dropped** when `-Path` is a plain directory - the scan returns nothing and raises no
  error. Measured over `src/platform/os/windows/api_layer/user`: the recorded probe printed
  `A=0, B=52, C=52` - wildcard in `-Path` or `-Recurse -Depth 0` both return the real 52.
- Its present cost: a per-folder lookup reads "this TU defines none of the neighbours", and the
  section-boundary argument built on that 0 is unfalsified. A 0 on a folder you know is populated
  **is** the bug, not a finding.
- The same signature - right count, empty content - recurs across the file: a pwsh
  `[regex]::Matches()` match has no usable `.Text` (rsp built with the right line count and empty
  paths), `Select-Object -ExpandProperty Path` finds nothing on a file list, a `>` redirect writes
  UTF-16 so `Select-String -Path` reads 0 matches on a file you just wrote. The recorded rule:
  print the tail of what you built, never just its length.
- Parsing traps that kill a scan before it runs: `For-Each` is not a cmdlet, a bare `true`/`false`
  on an assignment RHS dies, a leading range-subscript parses as an array. Each cost was a re-run,
  not a wrong answer - the file dates them 9 Nov 2026.
- The file's first section states the batch rule the whole repo runs by: do not run shell commands
  in parallel - they race - and `parallel` in the anchor is that heading. The file is the one gotcha
  source still receiving entries, so this page's anchor may need re-probing, not editing.

## See also

[[git]] - same folder, same pointer-not-prose rule. Not yet paged: `memory-bank/gotchas/windows.md`.
