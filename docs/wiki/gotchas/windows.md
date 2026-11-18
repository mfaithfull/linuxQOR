---
Type: WikiPage
PageType: gotcha
Title: "windows"
Status: current
LastVerified: 2026-11-18
Claims: 4
Summary: "The windows gotcha file holds no traps itself - it is a pointer; reading it alone undercounts every measured api_layer caveat behind it."
Anchors:
  - { kind: in-tree, path: memory-bank/gotchas/windows.md, symbol: api_layer, verified: 2026-11-18 }
Judges:
  - Get-ChildItem src/platform/os/windows/api_layer/user/gotchas -File
---

# windows

`memory-bank/gotchas/windows.md` is seven lines and carries **no trap text at all**: it forwards the
measured set of Windows `api_layer` caveats to an index of its own elsewhere in the tree.

- The trap is the stop-at-the-pointer read: a reader (or an agent) that consults this file and stops
  finds zero traps in it, while the measured caveats actually live behind
  `src/platform/os/windows/api_layer/user/gotchas.md` (a `Type: Index` file) and that folder's
  `gotchas/` sub-folder, one context file per kind routed by trigger.
- Its present cost, re-measured 18 Nov 2026 by the command in `Judges:`: the sub-folder holds **5**
  context files (`evidence-integrity`, `instrument-flags`, `record-editing`, `sessions-28-30`,
  `sessions-31-33`) plus the index - an undercount of five measured caveat records if the forward is
  not followed.
- The forward is a prose pointer, not an `Anchors:` entry: this page's anchor is the `memory-bank/`
  file that records the trap, and a second anchor source was not a unit this leaf owns. A day the
  index moves, the cost is re-measured against the tree, not re-stated here.
- Same shape as its `memory-bank/` index siblings (the file names the same shape as theirs): a
  gotcha file may itself be a router - which is exactly why counting files at one level, per
  [[powershell]], is the first thing this trap defeats.

## See also

[[git]] [[powershell]] - the other two pages of this folder, and the reason the census prints
`  gotchas = N` against the source-file listing.
