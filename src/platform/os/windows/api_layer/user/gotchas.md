---
Type: Index
Description: "Index of context hints for specific scenarios. Read if Trigger matches situation"
index:
  - Trigger: "Before acting on an instrument flag - a count, a MISMATCH / CALL NOT PARSED / NEITHER row, a BYPASS or INDIRECT row"
    File: "gotchas/instrument-flags.md"
  - Trigger: "Before dating or quoting a compile-gate log, an /EP capture or a verifier report"
    File: "gotchas/evidence-integrity.md"
  - Trigger: "Before editing a record file, or any long text file, from PowerShell or the editor tool"
    File: "gotchas/record-editing.md"
  - Trigger: "Reviewing rectangle.cpp, resource.cpp or scrollbar.cpp, or reusing an instrument that sessions 28-30 built"
    File: "gotchas/sessions-28-30.md"
  - Trigger: "Reviewing systemshutdown.cpp, text.cpp or window.cpp, or touching calling conventions, CONST LPINT, gate-log dating, /EP diffs"
    File: "gotchas/sessions-31-32.md"
---

# winqor_api_user - instrument and record-editing caveats

Referenced from `tooling.md`: every instrument in that index has a measured blind spot, and editing these records from
PowerShell has its own set. **Read the entry that qualifies a flag before acting on the flag.** Procedure: `method.md`;
instrument index: `tooling.md`.

Entries are grouped by kind, not by session; each names the session or pass that measured it, and the instrument slug is
the handle to search for in `tooling.md`. Text moved here verbatim from `method.md` on 2 Nov 2026, with two exceptions:
caveats measured twice are merged into one entry, and exact record file sizes and line numbers were dropped where they
were illustration rather than measurement - those citations go stale faster than they inform.

## How this file is split (4 Nov 2026)

The caveats themselves live in `gotchas/`, one file per kind, and the block above routes by trigger - a session opens
the file its current task names instead of the whole set.
