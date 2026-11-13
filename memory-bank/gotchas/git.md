---
Type: Context
---

## Running git commands
- always run git commands sequentially to prevent data races. Ensure commands have completed before trusting the reported status.

## `git log` opens a pager and the command never returns
- **Always `git --no-pager log ...` (or `git -c core.pager=cat log ...`)**
  `git log -5 --format='%h %ad %s' --date=short` over this repo stayed running past 300 seconds, so the tool auto-proceeded and returned only a partial subject dump while the pager kept the process alive. Measured 13 Nov 2026 at the U2 close, where the LATER `git status --short` in the same batch read cleanly and made the partial log look like a finding. Lead every git batch with `--no-pager`.

## CRLF conversion warnings eat the lines you are trying to read
- **`git diff` / `git add` on this repo prints warnings BEFORE the numbers; capture the value, do not read the block**
  `git diff --shortstat` and `git add -A` each printed `warning: in the working copy of 'memory-bank/productContext.md', LF will be replaced by CRLF the next time Git touches it` twice, pushing the `N files changed` line down into the middle-truncated part of the response. `$a = git diff --shortstat HEAD; 'staged=' + $a` printed only the line wanted. Measured 13 Nov 2026: the warning-carrying files are the memory-bank record `.md` files (`productContext.md`, `projectbrief.md`). A `.gitattributes` line (`*.md text eol=crlf`) makes those record diffs quiet AND stable, which is why they belong in the commit at all.
