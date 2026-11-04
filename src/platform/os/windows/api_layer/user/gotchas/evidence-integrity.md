---
Type: Context
Description: "Dating and quoting compile-gate logs, /EP captures and verifier output"
---

Moved verbatim out of `gotchas.md` on 4 Nov 2026 by `tmp/_tmp_gotchas_split.ps1` - snapshot `tmp/_tmp_gotchas_presplit.md`, verbatim rejoin proven by `tmp/_tmp_gotchas_verify.ps1`.

## Evidence integrity - gate logs, `/EP` captures, verifier output
**Compile-log gotcha (measured 30 Oct 2026):** `_tmp_compile_all.ps1` writes the `cl` output to the fixed path
`tmp/_tmp_cl_all.log` regardless of the caller, so redirecting the script's own stdout to a per-session file captures only
its summary line (`exit=N loglines=M`) plus its internal diagnostic grep - it is not the `cl` log. The summary's `exit=0` is
already sufficient for the 38/38 claim (one `cl` invocation over all 38 sources fails as a whole if any TU fails), and the
belt-and-braces check is in `tmp/_tmp_cl_all.log`: 38 `\.cpp$` echo lines and 38 `.obj` files in `tmp/obj` newer than the
run - both confirmed for the session-11 run.
- `_tmp_compile_all.ps1`'s `pwsh -File` wrapper returned exit 1 in session 15 while the `.bat` it writes reported
  `cl_exit=0` with a clean log - run `cmd /c tmp/_tmp_cl_all.bat` and read the log, don't trust that wrapper's exit code.
- `cl /EP` output must go to a **per-TU file**: the shared `_tmp_pp_one.bat` writes `_tmp_pp_out.txt`, and reading it
  before `cl` finished once produced a whole session's worth of another TU's text.
- **A copied gate log cannot date its own run** (session 23's interrupted close-out pass). `Copy-Item` preserves the
  source's `LastWriteTime`, and every passing gate run writes output of the same shape, so a copy of an older log is
  indistinguishable from a new run by timestamp alone. That pass produced `tmp/_tmp_cl_all_s23final.log` and `_s23pre.log`
  that are **byte-identical to `_tmp_cl_all_s22final.log`** (SHA256 `7EB28546A9062CBCC...`) and therefore predate both
  session-23 source edits. Either run `tmp/_tmp_cl_all.bat` with its redirect **outside** the batch and snapshot the file
  afterwards, or verify: `tmp/obj/<tu>.obj` `LastWriteTime` must be later than the edited source's, and the new log's hash
  must differ from the previous session's.
- **Re-run `/EP` after the last edit.** `tmp/_tmp_msg_pp_post_s23.txt` (11:17) predates the `user32.h` `(void)` edit (11:36),
  so the neutral-conversion proof had to be redone as `_tmp_msg_pp_final_s23.txt`; diff the captures whose timestamps bracket
  the final edit, never the first "post" file you find.
- **Locator pointer verification:** use `tmp/_tmp_u15_locator_verify.ps1`, not an ad-hoc matcher - it knows the one
  deliberate exception (U27's pointer targets the shared `p6-u27.md` extract bullet, which does not start with
  `- **U27**`). Re-read the totals it prints after every register edit; the exception count is the only number there that
  may not move.
- **Comment-only edits have a free neutrality proof - use it instead of arguing** (session 24). `cl /EP` before and after
  a large comment block differ by exactly the block's own line count, **all of them blank, 0 removed, 0 non-blank added**
  (`/EP` drops the comment text but keeps one output line per comment line), i.e. the emitted code is identical. Three
  counts from `Compare-Object` prove it: total, `<=`-side count, non-blank `=>` count.
- **`GetModuleHandleW(L"user32")` fails with `gle=126` in a bare console process**, so every live name probe must
  `LoadLibraryW` first and print that it did (G5); `monitor`, `mdi` and `keyboard` probes all take that path.
- **Verify a cited artefact before you cite it.** Session 25's records cited a verifier report while its script was still
  being edited: the file was a 22:22 mid-session snapshot (`archive/tu/mouse.md: ABSENT`, 0 citations counted), and
  re-running `tmp/_tmp_s25_verify.ps1` - the point of that script - changed several of its lines. Close-out verifiers run
  last, after every record edit, and their output is quoted only from that final run.
