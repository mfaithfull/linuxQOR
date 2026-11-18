---
Type: WikiPage
PageType: gotcha
Title: "git"
Status: current
LastVerified: 2026-11-18
Claims: 5
Summary: "git traps in this repo: the log pager never returns inside a tool call, CRLF warnings bury the numbers, and batched git commands race."
Anchors:
  - { kind: in-tree, path: memory-bank/gotchas/git.md, symbol: pager, verified: 2026-11-18 }
Judges:
  - git log -5 --format='%h %ad %s' --date=short
---

# git

`memory-bank/gotchas/git.md` records three traps around running git here; this page is the pointer
plus what each costs now, not a restatement of the file.

- The trap: a bare `git log` opens the interactive pager and never returns. The recorded reproduction
  (`Judges:`) over this repo stayed running past the 300 s tool cap; the batch auto-proceeded on the
  partial subject dump while the pager kept the process alive.
- Its present cost: the next clean `git status` in the same batch read cleanly and made the partial
  log look like a finding - the damage is a believed truncation, not just a timeout. The guard is
  `git --no-pager log ...` (or `-c core.pager=cat`) leading every git batch.
- `git diff` / `git add` print CRLF conversion warnings **before** the numbers, pushing the
  `N files changed` line into the middle-truncated part of a tool response; the recorded cost is
  mis-read diff stats. Capture the value (`$a = git diff --shortstat HEAD`) instead of reading the
  block; the warning-carrying files are the `memory-bank/` record `.md` files.
- Git commands batched in parallel race (the file's first section, `Running git commands`): trust the
  reported status only after the command has actually completed.
- The source file is dated `Measured 13 Nov 2026` and re-checked at the U2 close; its heads are held
  in the `GotchaPages` plan's probe slot, and `wiki-ref-check anchors` says the day the anchor stops
  resolving.

## See also

The folder rule and why a command-line judge leaves this page `undated`:
`docs/wiki/gotchas/purpose.md`. Not yet paged: the other two trap files in the same source folder,
`memory-bank/gotchas/powershell.md` and `memory-bank/gotchas/windows.md`.
