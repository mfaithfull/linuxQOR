---
Type: Context
Description: "Caveats measured in sessions 31-33 (systemshutdown, text, window)"
---

Moved verbatim out of `gotchas.md` on 4 Nov 2026 by `tmp/_tmp_gotchas_split.ps1` - snapshot `tmp/_tmp_gotchas_presplit.md`, verbatim rejoin proven by `tmp/_tmp_gotchas_verify.ps1`.

## Session 31 (`systemshutdown.cpp`, 4 Nov 2026)

- **A convention token on a member definition is a compile error on one arch only.** `C2373 "redefinition; different type
  modifiers"` appears **only when the TU is compiled for x86**; every x64 gate in 30 sessions passed with the same source.
  So "the folder gate is clean" is **not** evidence about calling conventions - for anything in `library.h` /
  `qor_pp_uses*` / definition-head shape, add an x86 compile (`vcvarsamd64_x86.bat`, then `cl` with the same flags).
  Beware: **the tree does not build for x86 at all** - 93 of 106 api_layer TUs die at `kernel32.h:92`
  (`PUNWIND_HISTORY_TABLE` is inside `#if defined(_AMD64_)||defined(_ARM_)||defined(_ARM64_)`, `um/winnt.h:21939`), so an
  x86 run's *baseline* is 93 failures, not 0; compare error sets against `tmp/_tmp_s31_x86_apiall_s31.txt`.
  **Retired as a step 5 Nov 2026 by decision D17 (`docs/agent/DECISIONS.md`): Windows support is 64-bit only, so do not add
  an x86 compile to any gate.** The blindness this entry measured still stands as a hazard - x64 gates cannot see
  arch-dependent defects - but it is now closed by the x64-only contract rather than by probing for it.
- **`dumpbin /disasm` before/after is the cheapest convention-neutrality proof, but diff it in memory and cap the print.**
  42,310 lines per dump; `Compare-Object` prints the whole difference set, so `Select-Object -First 12` it - and expect the
  `Dump of file ...` header line to differ, which in a genuinely neutral change is the *only* difference.
- **`dumpbin /symbols` separates the two halves of a convention question**: the defining object shows the member symbol as
  `notype () External`, a caller object shows it `UNDEF`; identical decorated names on both sides proves there is no
  dangling declaration. `/disasm` proves the code, `/symbols` proves the ABI name.
- **PowerShell hygiene for the record scripts:** `Get-Content` has **no `-Skip`** parameter (use `Select-Object -Skip N`);
  merging large arrays with `$a + $b` can yield a fixed-size collection, so build with `$frag=@(); $frag+=$a; $frag+=$b`
  when the result must stay mutable (a `.RemoveAt()` on a merged array throws "Collection was of a fixed size"); a
  `pwsh` inline command containing `` `t `` inside double quotes breaks the parser - put such scans in a `.ps1` file. For
  the non-ASCII scan use `[^\x00-\x7F]`; `[\x{2019}]` is not valid .NET regex. Inline `pwsh -Command "...$_..."` strings
  lose `$_` to the outer quoting level - a scan that needs `$_` goes in a `.ps1` file too.
- **Never verify an append in the same command that made it**: in session 31 `Add-Content` followed by
  `Get-Content -Last 8` printed the pre-append tail. Confirm appends with a line-count delta plus
  `Select-String -Pattern 'Session NN'`, and print the new block's first and last line separately.



## Session 32 (`text.cpp`, 4 Nov 2026) - instruments and records

**`tmp/_tmp_cl_all.bat` writes to stdout, not to a log file.** Redirect it (`cmd /c ...bat > log 2>&1`) or you will
report a log you never wrote. Worse, a **clean gate log is byte-identical across sessions** (744 bytes, 38 TU echoes,
`SHA256 7EB28546A9062CBCCCC808BC1CA2C74B6243862FA38DEBCF6A84760646FB2004` since session 17), so a hash comparison proves
nothing - in session 32 the first "gate" I quoted was a **3 Nov copy** that had been carried forward. Date the gate by
**log mtime + `.obj` freshness** instead: `tmp/_tmp_s32_gatecheck.ps1` counts TU echoes and diagnostics and reports any
`.obj` in `tmp/obj` older than the `.cpp` it should come from.

**The tool runner executes the commands in one call concurrently.** Pairing a compile with its checker in the same call
makes the checker read a half-written log (this happened twice in session 32 - a 43-line log read as 2 lines). Sequence
them across calls.

**`/EP` captures are ~453k lines - never print them; diff them.** `tmp/_tmp_s32_neutral.ps1` compares two captures
line-by-line ignoring blank lines: a comment-only edit must show **0 non-blank differences** and a line-count delta equal
to the comment lines added (text.cpp: 154,296 non-blank before and after, +80 lines). When naming the captures, use
**exact file names** - a prefix-based split (`_tmp_text_pp_s32*`) merged the `.err` capture into the `.txt` stream and
produced a bogus "0 splits".

**`_tmp_sigcmp.ps1`'s const test was wrong and understated U25/U18.** It matched only the **first** `CONST` on a line, so
the typedef-const spellings (`CONST LPINT`, `CONST INT *` where `CONST` is a macro) were **never compared**. The
corrected matcher in `tmp/_tmp_s32_names.ps1` is `(const[ ]+[A-Za-z_][A-Za-z0-9_]*\*+)`. Any earlier "0 type
divergences" statement that relied on the old test covers `CONST X*`/`CONST::X*` only.

**`tmp/_tmp_user32_exports.txt` is a `dumpbin /exports` dump, not a list of names.** Anchoring a stem to a line
(`(?m)^DrawText$`) never matches and silently classifies **every** binding as unresolved - the first run of
`tmp/_tmp_s33_prebind.ps1` reported all 85 `window.cpp` bindings as `NEITHER`. Match with word boundaries
(`(?<![A-Za-z0-9_])stem(?![A-Za-z0-9_])`) and treat a saved `.txt` beside an instrument as untrusted until the
instrument itself has been re-run against it.

**`CONST LPINT` in a member-function parameter list is `INT * const`, not `const int *`.** Do not read it off the
spelling: build the probe (`tmp/_tmp_s32_u23_probe.cpp` compiles, `tmp/_tmp_s32_u23_probe_neg.cpp` gives `C2664`). The
same trap produced the folder's first U23 instance (`MessageBoxIndirectT`).

**Use the tool's own line limit when appending to a record file, and verify after.** `editor.insert_line` is one-based on
that tool's view of the file; guessing a number from an earlier `Get-Content .Count` is how a session-32 insert landed at
`116` in a 17-line file and failed. An out-of-range error states the exact EOF boundary - use it. Also: a batch that
mixes `cd` with relative-path `Add-Content` is a race, not a sequence - session 32 created three stray
`u18.md`/`u23.md`/`u25.md` in the repo root that way (deleted; the real registers were untouched, confirmed by re-reading
them). Always pass absolute paths.

**A comment-block edit can silently delete part of the block it was meant to rewrite.** In session 32 a large in-place
rewrite of `text.cpp`'s closure block removed ~40 lines of it while reporting success. After any multi-line comment
rewrite: **grep for the block's markers** (`//=== api_layer conventions ===`, `//closure:`) and **count the file's lines**
before believing the edit, then re-prove neutrality.

## Session 33 (`window.cpp`, 4 Nov 2026)

- **A per-member U16 walk that prints nothing is a silent zero.** `tmp/_tmp_s33_floors.ps1` ran clean and left
  `tmp/_tmp_s33_floors.txt` with **no fact rows**. The version-floor figures for this TU therefore come from the
  per-member table `tmp/_tmp_s33_pre.ps1` sections C-D (`tmp/_tmp_s33_final.txt`, 85 `bind:` rows, 34 with
  `floor[if ...]`), not from the floors script. Re-use the table, or check the output for rows before quoting a count.
- **`floor[PARSER MISS]` in that table means "no SDK declaration", not "the walk failed"** - here exactly 2 rows
  (`CascadeChildWindows`, an undocumented export, and `CreateWindowT`, whose SDK spelling is a macro).
- **Filter the `/EP` capture to the binding lines before classifying it.** The capture carries SDK header text that
  mentions `GetProcAddress`; `tmp/_tmp_win_ppclass_s33.ps1` only became correct once it kept lines matching
  `DefProc pFunc`. Its literal pairing is a plain `"`-scan too - an earlier `(?<![Ll])` lookbehind skipped the opening
  quote of `L"user32"` and captured the text **between** the two literals.
- **PowerShell `-match` is case-insensitive.** An export-table membership test written with `-match` reads a wrong-case
  name as exported; the script now uses `-cnotmatch`. The same trap inflates the U25 count: a case-insensitive
  `\bCONST\b` scan over `window.cpp` code lines reports **8 lines / 12 tokens** where the case-sensitive truth is
  **1 line / 1 token**.
- **A space before the closing `>` is not U14's spaced class.** A P5-shaped regex
  `(Call|voidCall)\s*<[^<>]*\s>\s*\(` flags `window.cpp:206` and `:264`; U14's census reads the character after `Call<`,
  so those sites are tight and must be left alone (D15).
- **Write narratives from final measurements.** `archive/tu/window.md` was drafted mid-session and had to be corrected at
  close-out on four points: `NOMDI` membership, the comment-line count, the U25 `CONST` count, and the call-paren split.
- **A late comment-only edit silently invalidates a gate that already ran clean.** `tmp/_tmp_cl_all_final2_s33.log`
  (19:43:37) was 38 echoes / 0 diagnostics, but `window.cpp`'s contract-block line-number corrections touched the file at
  **20:11:31**, so the citable gate became `tmp/_tmp_cl_all_final3_s33.log` (22:25:10). The rule: run the gate **after** the
  last source touch, and when in doubt run `tmp/_tmp_s33_gatecheck.ps1` - it prints every `tmp/obj/<tu>.obj` older than its
  `.cpp`, which is how the stale run was caught rather than argued about. Re-run the consumer probe in the same breath
  (`tmp/_tmp_win_consumer_s33.bat > tmp/_tmp_win_consumer_s33c.log 2>&1`; the bat writes `CL_EXIT=` to stdout, it takes no
  output-file argument, so the capture name is the redirect).
- **Instrument names in records must come from a directory listing, never from memory.** The close-out cite audit
  (`tmp/_tmp_s33_closeaudit.ps1`, output `tmp/_tmp_s33_closeaudit.txt`) found two citations with no matching file:
  `tmp/_tmp_s33_gatecheck.ps1` (cited in `status.md`, `census.md`, `tooling.md` and the session notes **before the script
  existed** - it was written and run at close-out, and is now true) and `tmp/_tmp_win_consumer_s33b.bat` (never existed; the
  probe is `tmp/_tmp_win_consumer_s33.bat`, which produced `_s33.log`, `_s33b.log`, `_s33c.log`). Run the audit **before**
  the close-out is called done.
- **`Select-String`'s `\bCONST\b` is case-insensitive by default, so it also counts lowercase `const`.** Counting this TU's
  `CONST` typedef spellings without `-CaseSensitive` printed "8 lines / 12 tokens"; with `-CaseSensitive` the file has
  **1 `CONST` token on 1 line** (`window.cpp:158`) and 11 lowercase `const` tokens on 8 lines. `tmp/_tmp_s33_verify.ps1`
  carries the unqualified pattern, so its `CONST typedef spellings in CODE lines` number is an **upper bound**.
- **Contract blocks are not marked uniformly, so a marker scan under-counts them.** Scanning the 38 sources for the text
  `=== api_layer conventions` finds **2** files (only `text.cpp` and `window.cpp` use that header); the real population is
  measured by comment-line count instead (`tmp/_tmp_s33_rec4.ps1` -> `tmp/_tmp_s33_blocks.txt`: 5 sources with `< 8`
  comment lines). Use the comment-count measure for the "who still has no block" census.
- **A resumed session must re-read the record before appending to it.** This session was resumed after a context
  compaction and appended a **second** "Session 33 census update" block to `registers/census.md` (the first had already
  been written before the compaction). Detection instrument: `tmp/_tmp_s33_rec8.ps1` greps every record file for
  `session 33|Session 33|s33` and prints the hit line numbers per file - run it **before** writing, and again after.
- **Inline `pwsh -Command "..."` loses `$`-variables to the outer quoting level, and a reused terminal replays old
  scrollback into the result.** Two `run_commands` calls this session failed with `Missing expression after unary
  operator '++'` because `$i` arrived empty, and one returned 40 KB of earlier commands' output instead of its own. Rule
  unchanged but now doubly confirmed: every scan with a loop or `$_` goes in a `.ps1`, its output goes to a `tmp/` file,
  and the file is read with the file reader rather than the terminal.


