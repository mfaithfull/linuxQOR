---
Type: Plan
Title: "Write the records"
Parent: "../plan.md"
Level: 1
Items:
  - Title: Write the D50 decision entry
    Status: Atomic
    Content:
  - Title: Add the D50 index line
    Status: Atomic
    Content:
  - Title: Update the status boot line
    Status: Atomic
    Content:
  - Title: Update the registers closed list
    Status: Atomic
    Content:
  - Title: Write the U1 working record
    Status: Atomic
    Content:
  - Title: Record the close in completed work
    Status: Atomic
    Content:
  - Title: Update the active context focus
    Status: Atomic
    Content:
---

# Write the records

Slug: `WriteRecords` - from title "Write the records" ("the" dropped, PascalCased, ASCII, 12 chars, inside the 24-char cap). No sibling collision: the sibling folders of the parent are `BatchSmallTUs` and `BatchLargeTUs`. Uniqueness is stored here, not recomputed.

## Goal

Land the paper this pass needs at its close - one decision record (**D50**) and the five records that close **U1** - each item a single write of one named file, so the pass's audit is `git show` and no record is opened twice.

## Context

Inherited from `../plan.md` (Level 0, the U1 pass; source narrative `docs/future/winqor_api_user_U1_plan.md`, superseded provenance). Parent items in order: `Probe the accelerator TU` (Atomic), `Batch the small TUs` (Refined, 33 TUs / 124 sites), `Batch the large TUs` (Refined, 19 TUs / 468 sites), `Gate the folder` (Atomic), **this item** (`Write the records`), `Commit the pass` (Atomic), `All-libraries gate` (Deferred). Parent goal: give every wrapper in `winqor_api_user` a call-site `Win32ErrorHandler _;` - **597 sites over the 53 wrapper TUs** - so a Windows failure prints its decoded error text before it escalates, closing `U1` as **D50** while leaving `U2` untouched.

**Write only at the close.** Every record here is a close record: it is legal only once all 53 TUs / 597 sites carry the handler **and** the dated folder gate reads 54/54 exit 0 with 0 errors / 0 warnings. Nothing in this plan is written mid-pass, and nothing is written before the code pass runs.

**State measured this pass (11 Nov 2026, pre-record tree, nothing written) - and the code pass has not started.** `git status --porcelain` lists only `docs/future/TODO.md` and `docs/future/...`; **0 sources touched**. Folder re-census: 54 `.cpp`, **0** `Win32ErrorHandler` code sites (all 41 hits are prose inside TU banners), **0** `errorhandler.h` includes. So every figure a record carries must come from the code pass and the gate item - never from here.

Record-file anchors measured this pass (anchor on the named text, not the number: an earlier write in this item list shifts a later file's line numbers):

| File | Measured |
|---|---|
| `docs/agent/DECISIONS.md` | 102 lines; index ends at D49, `File: ./decisions/D49.md` at `:102` |
| `docs/agent/decisions/` | **49** files `D1.md`-`D49.md`, **no D50** - D50 is the next free id, re-measured rather than inherited |
| `docs/agent/decisions/D49.md` | the D-entry shape: front matter `Type: Decision` / `ID` / `Title` / `Index: docs/agent/DECISIONS.md`, then `## D49: ... (11 Nov 2026), CLOSES P4`, `### What the close measured`, `### What D49 settles, and what it deliberately does not`, `## Record` |
| `src/.../api_layer/user/status.md` | 180 lines; boot line at `:3` ("Last updated 12 Nov 2026 - **P3 CLOSED ..."); Open list at `:149` reads exactly `- Open: U1, U2.`; the gate command line at `:158-165` is live state and stays untouched |
| `src/.../api_layer/user/registers/README.md` | 60 lines; the closed/frozen paragraph starts at `:12` ("Closed / frozen - append nothing: `u4` (D26->D34), ..."); the `Lines` / `Was registers.md` table below is historical, **not maintained, not to be trusted** |
| `src/.../api_layer/user/registers/u1.md` | 10 lines; `:3-4` carries the settled-29-Oct wording, `:5` the stale "0 of 38 TUs include `../kernel/errorhandler.h`" |
| `src/.../api_layer/user/registers/u17.md` | 235 lines; `:3` "WORKING RECORD - the U17 rename pass (10 Nov 2026, **Option A = rename**, recorded as **D40**)", tick table `:9-19`, "### Measured at the pass" at `:21` - the shape this pass's record copies |
| `src/.../api_layer/user/completed_work.md` | 81 lines; "## Closed / frozen ids - append nothing, never open at session start" at `:71`, its record row at `:73`; "## How to keep this file" at `:75-80` |
| `memory-bank/now.md` | current-focus entry at `:3` (the P3 CLOSED paragraph); "## Decisions in force (pointer only - not restated here)" at `:88` |

**Protocol v3 (`status.md:20-40`) governs the whole item list:** a close ends with `git commit` (the parent's next Atomic item), every artefact is written **once and not opened again** in the same session, and the commit diff is the audit - no cite audit, no read-back pass. Protocol 7: frozen text is never amended - the ~35 TU banners and every closed record keep their wording, and corrections live in D50 and `u1.md` (the U17/D40 precedent at `u17.md:70-74`).

## Decisions

Inherited from `../plan.md`'s `## Decisions`, linked and not restated: **D50** is the closure record for reversing the settled 29 Oct 2026 "leave as-is" for the **handler half only** (`docs/agent/DECISIONS.md` index + `docs/agent/decisions/D50.md`); **U2 stays settled and untouched** - no `CheckReturn`/`CheckNonZeroHandle` is introduced anywhere and the folder's 4 live `CheckReturn` sites keep their semantics; **`wsprintfT` is excluded**, so `user32.h` and every shared header stay untouched and the 122-source all-libraries gate is **not** in play (the folder gate is the whole audit); **U1 stays OPEN until the whole pass and the dated folder gate are clean** - this plan is what closes it; **TU banners are never edited** (protocol 7); **nothing is appended to frozen or closed records** (`tooling.md` is frozen at session 34, `completed_work.md` and `archive/` are write-once); the **D21/D46/D48 residuals stay** unfixed - no project-wide `UNICODE`/`WINVER`/`_WIN32_WINNT` pin, no `NO_STRICT` change.

Cut rule for this plan: one record per file, one write per file, in the order the D49 close left it (`D49.md:32`'s `## Record` paragraph names exactly this sequence: the D-entry, `DECISIONS.md` index, `status.md` boot line + Open list, `registers/README.md` closed list, the register's own head line, `completed_work.md` closed-ids record, `memory-bank/now.md` focus). That is 7 files and 7 items - the breadth cap, with no item needing refinement, so this plan is terminal on creation and the pass needs no further planning node.

## Write the D50 decision entry

Atomic: **one new file**, `docs/agent/decisions/D50.md`, written once. Shape is `D49.md`'s, not invented: front matter `Type: Decision` / `ID: D50` / `Title: "<one line: U1 - the handler half of the settled 29 Oct leave-as-is is reversed: every wrapper gets a call-site Win32ErrorHandler _; - POLICY (<date>), CLOSES U1, CODE CHANGE (53 sources, 650 edits)>"` / `Index: docs/agent/DECISIONS.md`, then `## D50: ...` and the sections `### What the close measured`, `### Why the reversal is defensible`, `### What D50 settles, and what it deliberately does not`, `## Record`.

The entry must carry the measured run-time delta the parent states (`../plan.md:90-98`) in its own words: `IssueHandler<T>` ctor `Push()`es and dtor `Pop()`es (`src/qor/flyers/error/handler.h:37-45`), `kernel/errorhandler.cpp:64`+ emits `OutputDebugStringA` with the issue text plus the decoded Windows last-error string and returns **false** for `Continuable_Error`/`Serious_Error`/`Fatal_Error` (**true** only for `Note`/`Warning`), `Issue::Resolve(bool)` (`src/qor/flyers/issue/issue.h:126-132`) turns `Resolve(false)` into `Escalate()` -> `throw(*this)`. **Delta: control flow and the exception are unchanged; the gain is the diagnostic.** The `Note`/`Warning` row stays "measure, do not assert". The entry also states the frozen-banner consequence (the ~35 banners still say "no `Win32ErrorHandler` ... U1/U2 settled policy" and are not amended, per `u17.md:70-74`) and the post-pass measured figures (597 handlers / 53 TUs carrying the include / 650 edits / gate 54/54 exit 0 / `cl /EP` binding literals byte-unchanged). Link every other D-entry by id; never restate one.

## Add the D50 index line

Atomic: **one file, one insertion**, `docs/agent/DECISIONS.md`. Append after the D49 entry (`File: ./decisions/D49.md`, measured at `:102` of 102) the index's own two-line shape: `  - D50: U1 - ... - POLICY (<date>), CLOSES U1, CODE CHANGE (53 of the folder's 54 sources, 650 edits)` then `    File: ./decisions/D50.md`. Indent as the index does (2 spaces for the `- D<n>:` line, 4 for `File:`). Nothing else in the index is touched - no rewording of D1-D49, no renumbering, and the index stays the only machine state it has ever carried.

## Update the status boot line

Atomic: **one file, two edits**, `src/platform/os/windows/api_layer/user/status.md`. (a) the boot line at `:3`: lead it with the U1 close dated this session in the existing style - "**U1 CLOSED <date> (D50 - the handler half of the settled 29 Oct leave-as-is reversed, 597 call-site `Win32ErrorHandler _;` over 53 TUs, +53 include lines, folder gate 54/54 exit 0 / 0 errors / 0 warnings, `user32.h` untouched, U2 untouched)**" - and, per protocol v3 cause (c) (the file's own 120-line ceiling), move the outgoing boot detail into `completed_work.md` in the same session rather than growing this file past its ceiling. (b) the Open list at `:149`: change `- Open: U1, U2.` to `- Open: U2.` and nothing else on that line. The gate command line at `:158-165`, the record protocol at `:20-40`, and the id map at `:143-148` are live state that this pass does not edit.

## Update the registers closed list

Atomic: **one file, one insertion**, `src/platform/os/windows/api_layer/user/registers/README.md`. Add to the closed/frozen paragraph at `:12`, in that paragraph's own bolded style: "**U1 CLOSED <date> (D50)** - the call-site handler is installed at all 597 wrapper sites over 53 TUs; the reversal is a reporting fix, control flow and the exception unchanged; 0 banners edited, the stale banner wording is corrected in `u1.md` and D50 (protocol 7)". Do not touch the historical `Lines` / `Was registers.md` table (`:23-60`) - the README says those columns are retired and "not to be trusted", so `u1.md`'s row at `:33` stays as history.

## Write the U1 working record

Atomic: **one file**, `src/platform/os/windows/api_layer/user/registers/u1.md`, rewritten once into `u17.md`'s working-record shape (the model the parent names): head `## WORKING RECORD - the U1 error-handler pass (<date>, recorded as **D50**)`, the tick table `| # | Item | Sites | Status |` whose rows are this pass's units (`Probe accelerator.cpp` 5, the five small batches S1-S5, the three large batches L1-L3, `window.cpp` 86, the folder gate, the census invariant), `### Measured at the pass` carrying the post-pass figures, then the frozen entry kept verbatim below with a **closure line** above it that corrects the stale `:5` wording ("0 of 38 TUs" - 38 was the review-queue size; the folder is 54 sources / 53 wrapper TUs) and points at D50 for the reversal of `:3-4`'s settled policy. The tick table is filled **per TU as measured** during the code pass (the U17 lesson the parent carries: deferring ticks to the close forced a whole pass to be re-derived); this item's write is the closure line and the measured block. Never restate D50 here - point at it.

## Record the close in completed work

Atomic: **one file, one insertion**, `src/platform/os/windows/api_layer/user/completed_work.md`. Prepend the U1 close to the closed-ids row at `:73` (the list under "Closed / frozen ids - append nothing, never open at session start" at `:71`): "**U1** (<date>, D50: **the handler half reversed, code change** - 53 of the 54 sources, 650 edits, folder gate 54/54 exit 0, `user32.h` unchanged at 817 lines / 598 members / 597 bindings, no banner edited)". The file's own "How to keep this file" rule at `:77-80` governs: one write, nothing already here reworded, re-measured or re-read; never write live state here.

## Update the active context focus

Atomic: **one file, one write**, `memory-bank/now.md`. Replace the head slot at `:3` with the new current focus: 2-3 lines, the measured post-pass figures, and the D50 pointer - the rule adopted 2 Nov 2026 says this file carries the current focus and **pointers, never a per-session narrative**, with the outgoing entry reduced or moved to `memory-bank/archive/`. Do not restate any D-entry text here (`:88` "Decisions in force (pointer only - not restated here)"), and do not touch the boot-order section at `:94-107`.
