---
Type: Context
Description: "Caveats measured in sessions 31-32 (systemshutdown, text)"
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
