# winqor_api_user - review method (per TU)

The procedure one session follows, one translation unit at a time. The instruments these steps name are indexed in
`tooling.md`, and `tooling.md` points at `gotchas.md`, which says what each instrument does and does not prove - read the
caveat before acting on the flag it qualifies.

The numbered steps below moved verbatim out of `status.md` in the 1 Nov 2026 split. On 2 Nov 2026 the tooling index and
the instrument caveats moved out of this file into `tooling.md` / `gotchas.md`; the steps are unchanged.

## Review method (validated on accelerator.cpp)

1. Read the TU; pair every definition against its declaration in `user32.h` (return type, parameter types, `T` suffix).
2. Compare each wrapper signature with the SDK prototype in `winuser.h` and with the export table in
   `tmp/_tmp_user32_exports.txt` (`dumpbin /exports` of `C:\Windows\System32\user32.dll`, 1018 names).
3. Resolve the binding name the wrapper *actually* asks for - preprocess the TU with `cl /EP` and grep the
   `qor_pp_useswinapi` expansion. Never trust the source spelling; see ground rule G1.
 3b. Check **binding-vs-member agreement** with `tmp/_tmp_wrong_stem_scan.ps1` (added after session 8, which found
    three wrappers calling `GetDCEx`). Step 3 proves the bound name resolves; it does not prove the wrapper calls
    the entry point it wraps. Run the scan and expect this TU's sites to be clean. A flag is a defect until disproved -
    the only benign patterns so far are a deliberate alias to a different export (`cursor.cpp` `CopyCursorT` ->
    `CopyIcon`, pre-scan P3) and a definition not qualified with `User32::` (`resource.cpp`, open item U11).
4. Check the call-site idiom against `api_layer/kernel` + `api_layer/shell`: `qor_pp_fcontext;`,
   `Win32ErrorHandler _;`, `qor_pp_useswinapi*`, `Library::Call<RET, Args...>` with explicit template
   arguments, `CheckReturn<...>::TType` where zero/false means failure.
5. Look for an in-tree caller of every wrapper (`src`/`examples`/`test`). Name the consumer chain one level deep -
   which `ui/gui` controller or handler calls it - then stop: whether that controller should itself be called is a
   `winqor_gui` question. **Since 2 Nov 2026 an unreachable wrapper is not recorded at all** - open item U15, which used
   to collect these, is CLOSED by user decision (policy **D14**, `docs/agent/DECISIONS.md`); there is no U15 tally to
   append to. Do the check anyway when another item's severity depends on reachability - whether a P1/P5 mis-binding is
   latent or live, U18's divergent parameter at `TrackPopupMenu`, U19's LAYER-vs-BYPASS split - and record the fact in
   that item's entry. `registers/u15.md` keeps the instances measured before the closure as frozen history.
5b. For every stem whose callers are established, classify the call sites with `tmp/_tmp_bypass_scan.ps1`
   (**LAYER** through `User32::` vs **BYPASS** through the SDK import) and record the split against open item U19.
   **Never trust a raw grep for liveness**: comments must be stripped first (`tmp/_tmp_comment_state.ps1` for one file,
   the bypass scan's own stripper for a sweep) - `ui/gl/windowimpl.cpp` is 57% commented-out code and produced eight false
   bypass hits (see U19's method-hazard note).
   **Feed the scan every suffix variant of the stem** (`stem`, `stemA`, `stemW`, `stemT`): it matches `stem\s*\(`, so a
   run with bare stems alone misses every QOR layer call (which uses the `T`-suffixed member) and every bypass that
   calls the `W` export directly. Measured 30 Oct 2026 on the display family: bare stems gave 8 LAYER / 4 BYPASS, the
   full variant list with the pre-fix scan gave 16 LAYER / 10 BYPASS, and with both instrument fixes in place the same
   family gives **16 LAYER / 12 BYPASS** - the bare run hid all four `ui/gdi/displaydevices.cpp` and four
   `i_m/monitor.cpp` layer sites and six of the `*W` bypass sites. Also record, for each bypass file, whether it is
   in a CMake target at all: a bypass site in a file no target compiles is source debt, not run-time behaviour.
   **Second instrument fix this session (30 Oct 2026):** `::stem(` must be matched anywhere in the line. The previous
   pattern required either `^\s*::stem(` or a non-word, non-`:` character immediately before the stem, so a mid-line
   `if (::ChangeDisplaySettingsExW(...)` or `const HDC dc = ::GetDC(NULL);` fell through every branch - two sites in
   `ui/gl/monitorimpl.cpp` were invisible (`:138`, `:263`). Earlier sessions' per-family BYPASS counts were taken with the
   unfixed pattern and are therefore **lower bounds**; re-run the scan with the full variant list before quoting any of
   them (the cursor-family and coordinate-family entries under U19 both need that re-run).
6. Compile the TU standalone (`tmp/_tmp_compile_one.ps1 <name>.cpp`, `/W4`, must be exit 0).
7. Note which SDK prototypes the wrappers pair with are emitted only behind a version guard (`#if WINVER >= ...`,
   `#if NTDDI_VERSION >= ...`, `_WIN32_WINDOWS`, `_WIN32_IE`) and which SDK-visible entry points relevant to the
   section are absent from the installed SDK at that guard level. Record them against open item U16 as a per-TU list;
   do **not** change any build-wide version macro - the pin is deferred to the end of the review (user decision,
   session 5).

## Also in this folder

- `tooling.md` - the `tmp/` instrument index: what each scan prints, when it was added, which TU it was built for.
- `gotchas.md` (index) + `gotchas/` - referenced from `tooling.md`: the instrument caveats and the traps in editing these records from PowerShell, one file per kind, routed by the trigger block in the index.
- `status.md` - the per-session boot brief; it owns the record's file index, the register locator and the gate definition.
## Session start-up cost (rules adopted 3 Nov 2026, session 30)

The record tree is read at every session start, so its reading cost **is** the review's cost, and a session that reads
large artefacts loses its own context before it measures anything. Rules in force:

- **Never open a `/EP` capture or any `tmp/_tmp_*_pp_*.txt`** - each is ~453k lines / 7 MB. Every capture gets a digest
  produced by the same script that made it; the digest is what is read. Compare captures with a script that prints only the
  difference count.
- **One pre-scan script per TU, one short output** covering steps 1-5 (header section, bindings + export/A/W existence, SDK
  prototypes, hygiene, U19 spellings, T7 include) - `tmp/_tmp_s31_preflags.ps1` is the current shape, ~110 output lines.
  Do not run five instruments and read five outputs.
- **Appending to an accumulating register**: read that file's entry head plus its last ~25 lines, never the whole tally,
  and never a closed register (D14/D15).
- **Write the record from fragment files with one assembler** (`tmp/_s30_*.txt` + `tmp/_tmp_s30_asm*.ps1`), so the write
  pass never re-reads the record it is editing.
- **`status.md` stays under 220 lines / 22 KB**: at close, compress the outgoing session bullet instead of adding one, and
  keep the pre-flags section under 40 lines.
- **Commit at session close** - an uncommitted tree makes every later `git status` check noise.
- Close every session with three audits, not one: the **compile gate** (38/38 dated by `.obj` freshness), the **cite audit**
  (`tmp/_tmp_s29_cite_audit.ps1` - every `tmp/` artefact the records cite must exist; 0 of 198 missing at session 30) and
  the **locator verify** (`tmp/_tmp_u15_locator_verify.ps1`). **Superseded 5 Nov 2026 by protocol v2 below** - the cite
  audit and the locator verify are retired; the compile gate alone stands, and the commit is what makes the close auditable.

## Closing a session under protocol v2 (adopted 5 Nov 2026, session 34)

Steps 1-7 above are the review and are unchanged. What changed is the **write pass**: the record's read-set had grown to
about 269 KB, so a session compacted before it could measure anything and then re-read to convince itself nothing was
lost. The protocol itself is in `status.md`; a close is six writes and one gate.

1. Write the review banner **into the TU** - `//=== api_layer conventions ====` at the top of the `.cpp` - from final
   measurements: the counts, the instrument slug behind each count, the verdict for every id the TU touches, and why
   anything was recorded rather than fixed. This is the session's narrative.
2. Create a `registers/u<n>.md` **only** for a genuinely new open item (`u28.md` is the shape). Append nothing to an
   existing register, and nothing at all to a closed or frozen id.
3. Touch an existing register only when this TU **changes that item's status**, and then add one line naming the session.
4. Edit **`status.md` and nothing else**: tick the queue, reduce the closing session to pointers into its own banner, and
   measure the next TU's pre-flags from the source instead of inheriting them.
5. Gate: edited TU standalone at `/W4` exit 0, whole library 38/38 exit 0, dated by `tmp/obj/<tu>.obj` being newer than
   the source. This is the only audit v2 keeps.
6. Commit the TU, any new register, `status.md` and `memory-bank/activeContext.md` in one commit. **A session is not
   closed until the tree is clean** - an uncommitted close is what forces the next session to re-derive what happened.
7. Retired - do not run them: the register-locator generator, the README line-count refreshers, the queue audit that
   counts session-log rows, the `tmp/` cite audit, the locator verify, dash counting, and every hash-verified slice or
   splice. None is needed once no record file is edited twice.

