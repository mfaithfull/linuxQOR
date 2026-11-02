# memory-bank/archive/ - index

**Not boot material.** The memory-bank boot set is `projectbrief.md`, `productContext.md`, `activeContext.md`,
`systemPatterns.md`, `techContext.md`, `progress.md` - nothing in this folder. Open a file here only when provenance for a
specific claim is actually needed.

Completed-session records moved out of `memory-bank/activeContext.md` on 2 Nov 2026 so that boot reads only active
context. Slices are **verbatim** (their source line ranges are in each header) and are **not authoritative** - derive any
figure from the project's own record tree before acting on it.

| File | Lines | Was | Status |
|---|--:|---|---|
| `api-layer-user-review-sessions.md` | 389 | `activeContext.md:34-414` (381 body lines) | stale by design; superseded by `user/status.md` + `user/archive/` |
| `winqor-gui-doc-increments.md` | 19 | `activeContext.md:433-446` (14 body lines) | superseded by `docs/ui/windows/README.md` |
| `completed-context.md` | 10 | `activeContext.md:450-454` (5 body lines) | done; kept for provenance |

Verified by reproducing every archived line against the pre-change copy
(`tmp/_tmp_activeContext_pre_mb_s25.md`, SHA256 `BD44543B16EF...`): 400 of 400 lines identical.

Instrument: `tmp/_tmp_mb_archive_s25.ps1` (dry run first; it asserts the slice anchors and that kept + archived lines
equal the original count before it writes anything). Caveat it taught: PowerShell's `-f` **stringifies an array LHS**, so
`@('a','{0}') -f 'x'` joins the elements instead of formatting each one - format rows individually.
