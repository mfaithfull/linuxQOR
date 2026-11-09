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
- `status.md` - the per-session boot brief: the record protocol, the queue of all 49 sources, the open-item map and the
  gate definition. Live state only - closed-work detail is not in it.
- `completed_work.md` - read-only reference: the close detail of finished tasks and of the review phase that is over.
  Never boot material - open it only to research a named closed task or TU, and never edit what it already holds.
- `preflags.md` - the next TU's pre-flags: written wholesale at the previous close, read once at boot, replaced wholesale,
  never edited in place.
## Reading cost - the four rules that still hold (5 Nov 2026)

The record tree is 91 markdown files / 788 KB (measured 5 Nov 2026) and is read at session start, so its reading cost
**is** the review's cost.
These four survive; the rest of the old "Session start-up cost" section (register appending, fragment assemblers, the
220-line ceiling, the three-audit close) is retired and kept verbatim at `archive/protocol-v1-2026-11-05.md`.

- **Never open a `/EP` capture or any `tmp/_tmp_*_pp_*.txt`** - each is ~453k lines / 7 MB. Every capture gets a digest
  produced by the same script that made it; the digest is what is read. Compare captures with a script that prints only the
  difference count.
- **One pre-scan script per TU, one short output** covering steps 1-5 (header section, bindings + export/A/W existence, SDK
  prototypes, hygiene, U19 spellings, T7 include). Do not run five instruments and read five outputs.
- **A register is opened for one id at a time and only its entry head is read.** Registers are never appended to and a
  closed or frozen one is never opened at session start.
- **The boot read-set is `status.md` + `method.md` + `preflags.md` + the current TU.** `archive/`, `completed_work.md` and
  `gotchas/sessions-*.md` are opened only to research a named TU, session or closed task.

## Closing a session under protocol v3 - the close ends with the commit (5 Nov 2026)

Steps 1-7 of the review are unchanged. The write pass is bounded: **every artefact is written once, nothing is read back,
and the session is not closed until the tree is clean.** The v2 close is retired and kept verbatim at
`archive/protocol-v1-2026-11-05.md`.

1. Write the review banner **into the TU** - `//=== api_layer conventions ====` at the top of the `.cpp` - from final
   measurements: the counts, the instrument slug behind each count, the verdict for every id the TU touches, and why
   anything was recorded rather than fixed. This is the session's whole narrative.
2. Create a `registers/u<n>.md` **only** for a genuinely new open item (`u28.md` is the shape). Append nothing to an
   existing register, and nothing at all to a closed or frozen id.
3. Touch an existing register only when this TU **changes that item's status**, and then add one line naming the session.
4. Edit `status.md`: change that TU's queue row from `next` to `closed`. Nothing else in it - no session section, no
   verdict summary, no hand-off bullets. The banner in the source holds all of that.
5. Write `preflags.md` **wholesale** for the next TU, measured from that TU's source and never inherited. When the review
   is over, write the folder verdict there instead and say so.
6. Gate: edited TU standalone at `/W4` exit 0, then the whole library 38/38 exit 0, dated by `tmp/obj/<tu>.obj` being
   newer than the source. Re-run it **after** the banner is added. This is the only audit.
7. Commit the TU, any new register, `status.md`, `preflags.md` and `memory-bank/activeContext.md` in one commit - then
   **stop**. Do not re-open any of them to check that the write took: `git show` of the commit is the audit, and the
   read-back pass is what turned sessions 34-36 into a compaction loop.
8. Never run, and never write back into the procedure: the register-locator generator, the README line-count refreshers,
   the queue audit that counts session-log rows, the `tmp/` cite audit, the locator verify, dash counting, and every
   hash-verified slice or splice. None is needed once no record file is edited twice.

### Method reminders that survive every protocol revision

- Re-run the standalone compile after the banner is added - the gate must be dated after the last edit.
- Date the gate by `tmp/obj/<tu>.obj` being newer than the source; a clean log's hash cannot date it, because clean runs
  are byte-identical (this cost a re-run in sessions 31 and 34).
- Count `CONST` / `const` case-sensitively.
- Write the banner from final measurements, not from interim scans.
- A pre-flag is a lead, not a finding: re-measure anything you act on. The section retired at this close had a heading
  naming one TU over another TU's measurements.

