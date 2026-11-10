# winqor_api_user - U1 plan: install `Win32ErrorHandler _;` at every wrapper call site
> **Superseded 11 Nov 2026** - this narrative is charted as the plan tree
> [`User32ErrorHandlerPass/plan.md`](./User32ErrorHandlerPass/plan.md) (Level 0), which carries every
> section of this file. Kept as provenance only; the tree is the record.



Written 10 Nov 2026 (plan pass, no source touched). Target folder:
`src/platform/os/windows/api_layer/user` (the `User32` wrappers over `user32.dll`).
Item: `registers/u1.md`. Every count below was **measured this pass**, not inherited.

## Decisions settled for this plan (user, this pass)

1. **Reverse the settled policy.** The 29 Oct 2026 decision "leave as-is: no
   `Win32ErrorHandler _;` here" is superseded for the **handler half only**. It lands as a new
   decision record **D50** (`docs/agent/DECISIONS.md` + `docs/agent/decisions/D50.md`).
   **U2 is untouched** - no `CheckReturn`/`CheckNonZeroHandle` is introduced anywhere.
2. **Exclude `wsprintfT`.** The header-inline variadic member (`user32.h:799`+, D20) stays as it is.
   Consequence: **`user32.h` is not edited**, so the 122-source all-libraries gate is **not** in play -
   the folder gate is the audit.
3. **U1 stays OPEN until the whole pass is done.** Do not touch `registers/u1.md`'s head,
   `status.md`'s `Open:` list, or `registers/README.md`'s closed list until all **53 TUs / 597 wrappers**
   carry the handler and the final folder gate is dated clean. Tick the working record per TU as it is
   measured; close only at the end.

## 0. The policy conflict (read before executing)

`registers/u1.md:3-4`:

    **U1 - no call-site error handler anywhere in this library.** **Policy settled 29 Oct 2026 (user):
    accepted as-is, not to be fixed TU by TU** - see the "Convention decision" section.

The section it points at is `archive/status-v1-2026-11-04.md:94-97`:

    **Return-check / error-handler policy (settled 29 Oct 2026, user decision, session 3) - leave as-is:
    no Win32ErrorHandler _;, no CheckReturn/CheckNonZeroHandle here**, though api_layer/shell and
    api_layer/kernel use both and G6 makes CheckReturn always active. Sessions *record* the sites (U1/U2),
    change nothing - a FALSE/NULL/0 failure is silent and a swallowed SEH fault is indistinguishable
    from a legitimate zero (U7).

Corroborating state measured today:

| Fact | Measured |
|---|---|
| `status.md:149` "Open:" list | `U1, U2` - the only two items left open, both deliberately not fixed |
| Code occurrences of `Win32ErrorHandler` in the folder | **0** (41 raw hits, every one prose inside a TU banner) |
| TUs including `../kernel/errorhandler.h` | **0** |
| TU banners asserting "no `Win32ErrorHandler` ... U1/U2 policy" | ~35 banners (`bitmap.cpp:74`, `menu.cpp:80`, `touch.cpp:76`, `window.cpp:71`, ...) |

So the requested change **reverses a settled user decision** and must land with `D50` plus a closure
line in `registers/u1.md` - otherwise the record contradicts the tree. Measured (sec. 4), the change is
almost behaviour-neutral, which is what makes the reversal defensible as a reporting fix.

## 1. Measured population (re-derived this pass)

`u1.md`'s "0 of 38 TUs" is stale - 38 was the review-queue size. The folder is now **54 sources**:
53 carry wrappers, `winqor_api_user.cpp` is the module marker (0 wrappers).

Comment-stripped census (code lines only):

**597 wrappers = 597 `qor_pp_fcontext;` code sites = 597 `qor_pp_useswinapi*` binding lines
= 597 `Library::Call`/`voidCall`/`varCall` call sites.**

Total edit count for the pass: **597 statement insertions + 53 include lines = 650 changes over 53 files.**

Per-file site list - the exact count the script must hit per TU (alphabetical):

| TU | sites | TU | sites | TU | sites | TU | sites |
|---|--:|---|--:|---|--:|---|--:|
| accelerator.cpp | 5 | accessibility.cpp | 4 | bitmap.cpp | 1 | brush.cpp | 1 |
| buttoncontrol.cpp | 3 | caret.cpp | 8 | clipboard.cpp | 20 | combobox.cpp | 3 |
| console.cpp | 1 | coordtransform.cpp | 3 | cursor.cpp | 15 | desktop.cpp | 18 |
| devicecontext.cpp | 8 | devicemanagement.cpp | 2 | dialog.cpp | 22 | displayconfig.cpp | 7 |
| dpi.cpp | 21 | errorhandling.cpp | 4 | filledshapes.cpp | 3 | help.cpp | 5 |
| hook.cpp | 4 | icon.cpp | 14 | ime.cpp | 5 | keyboard.cpp | 34 |
| legacy.cpp | 4 | listbox.cpp | 3 | mdi.cpp | 4 | menu.cpp | 39 |
| message.cpp | 24 | misc.cpp | 23 | monitor.cpp | 5 | mouse.cpp | 10 |
| painting.cpp | 25 | pointer.cpp | 32 | powermanagement.cpp | 4 | printing.cpp | 1 |
| processthread.cpp | 5 | rawinput.cpp | 7 | rectangle.cpp | 11 | resource.cpp | 2 |
| scrollbar.cpp | 12 | security.cpp | 2 | strings.cpp | 34 | synchronization.cpp | 2 |
| systeminfo.cpp | 4 | systemshutdown.cpp | 6 | text.cpp | 4 | timer.cpp | 2 |
| touch.cpp | 14 | window.cpp | 86 | windowclass.cpp | 14 | windowprocedure.cpp | 2 |
| windowproperties.cpp | 5 | winqor_api_user.cpp | 0 | | | | | |

Sums to **597** over the 53 wrapper TUs. Cross-check against the folder's standing census:
`user32.h` 817 lines / 598 member declarations / **597 bindings** (post-P3, 12 Nov 2026) - the
binding count agrees exactly, so the census anchor is the whole population, with no wrapper missing
a `qor_pp_fcontext;`.

## 2. The idiom, copied from `kernel`/`shell` (not invented)

- **Include.** `kernel/kconsole.cpp:8` spells `#include "errorhandler.h"` because that TU lives in
  `kernel`; a `user/` TU must spell it **`#include "../kernel/errorhandler.h"`** - the form
  `shell/drag.cpp:11` uses. Place it in the **generic** block, immediately after
  `#include "src/qor/flyers/error/error.h"` and before the
  `//Windows specific headers must be last ...` comment. It is a generic QOR header
  (`errorhandler.h:28` pulls `src/qor/flyers/error/defaulterrorhandler.h`), so it must not go
  below the Windows-specific marker.
- **Statement.** `kernel/kconsole.cpp:20` and `shell/drag.cpp:24` both place
  `Win32ErrorHandler _;` **immediately after `qor_pp_fcontext;`, before the `qor_pp_useswinapi*`
  line**. In this folder that is 8-space indentation.

Target shape (`accelerator.cpp:22-27`):

    BOOL User32::CopyAcceleratorTableT(HACCEL hAccelSrc, LPACCEL lpAccelDst, int cAccelEntries)
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapiAW(user32, CopyAcceleratorTable);
        return Library::Call<int, HACCEL, LPACCEL, int>(pFunc, hAccelSrc, lpAccelDst, cAccelEntries);
    }

## 3. Why the anchor is safe (and the one trap)

The census prints `fcontext == bindings == calls` for all 53 TUs, so every wrapper carries the anchor
and no non-wrapper body does. **Trap:** a raw grep for `qor_pp_fcontext;` over the folder reads
**598**, one above the 597 code sites - exactly one TU banner spells the anchor with its semicolon in
prose. The inserter must anchor on lines whose first non-whitespace token is `qor_pp_fcontext;` on a
**comment-stripped** line, never on a substring match. Banner text is never edited (protocol 7).


## 4. Measured run-time delta (what the record must state)

Chain, read this pass:

- `IssueHandler<T>`'s ctor `Push()`es onto the flyer chain and its dtor `Pop()`s
  (`src/qor/flyers/error/handler.h:37-45`) - the handler covers the wrapper's **dynamic extent** only.
- `Win32ErrorHandler` (`api_layer/kernel/errorhandler.h:32`) derives from
  `qor::IssueHandler<qor::ErrorBase>`, so it registers under `SeverityIssueHandlerGUID` (the
  **ErrorBase** one), not `SeriousIssueHandlerGUID`/`ContinuableIssueHandlerGUID`.
- Raise path: a null binding in `Library::Call`/`voidCall`/`varCall` calls
  `ReportMissingLibraryFunction()` -> `serious("Missing library function.")`
  (`api_layer/library.h:30-33`, reached from `:56-59` / `:90-92` / `:123-126`);
  a SEH fault goes through `Library::ExceptionFilterFunction` (`library.h:18-27`), which also
  calls `serious(...)`. Both then enter `Serious::Handle()` (`src/qor/flyers/error/serious.cpp:22-34`),
  which tries `IssueHandler<Serious>` (absent in this library) and falls back to
  `IssueHandler<ErrorBase>` - **our** handler once it is installed.
- `Win32ErrorHandler::Handle` (`api_layer/kernel/errorhandler.cpp:64`+) emits
  `OutputDebugStringA` with the issue text plus the decoded Windows last-error string, and returns
  **false** for `Continuable_Error` / `Serious_Error` / `Fatal_Error` (**true** only for `Note`/`Warning`).
- `Issue::Resolve(bool)` (`src/qor/flyers/issue/issue.h:126-132`): `Resolve(false)` -> `Escalate()` ->
  `throw(*this)`.

| Path | Today (no handler) | With `Win32ErrorHandler _;` |
|---|---|---|
| null binding / SEH fault (`serious`) | `Serious::Handle()` else-branch -> `Escalate()` -> `throw` | handler runs, emits the Windows error string, returns false -> `Resolve(false)` -> `Escalate()` -> **the same `throw`** |
| the 4 existing `CheckReturn` sites (`continuable`) | escalate | emit + escalate |
| `Note`/`Warning` raises | none exist on this library's call path | would be resolved (not escalated) - **measure, do not assert** |

**Delta: control flow and the exception are unchanged; the gain is the diagnostic.** Link is already
covered - `user/CMakeLists.txt` ends `target_link_libraries(${target} qor_module qor_error qor_issue
winqor_api_kernel winqor_sharedbootstrap)`, so the `Win32ErrorHandler` vtable and `Handle` definition
resolve from `winqor_api_kernel` (the gate is compile-only, so this is a read-verification, not gate
proof). `user32.h:11` already pulls `../library.h`, so the include graph gains only `errorhandler.h`.


## 5. Execution plan (batched, gate-cheap, record-honest)

1. **Probe one TU first: `accelerator.cpp`** (5 sites) plus its include line. Run the standalone
   `/W4` gate on it. This is the only step that can surface a new diagnostic (`C4101`/`C4189` on the
   unused `_`, or an include-order `C1083` class). Resolve anything it prints **before** mass-applying.
   Date the standalone leg by `tmp/obj/accelerator.obj` being newer than the source.
2. **Batch the remaining 52 TUs smallest-first**, ticking the working record per TU the moment it is
   verified (the U17 lesson - deferring ticks to the close is what forced a whole pass to be re-derived):

   | Batch | TUs | Sites |
   |---|---|--:|
   | B1 (1-3) | `bitmap` `brush` `console` `printing` / `devicemanagement` `resource` `security` `synchronization` `timer` `windowprocedure` / `buttoncontrol` `combobox` `coordtransform` `filledshapes` `listbox` (15) | 31 |
   | B2 (4-8) | 4: `accessibility` `errorhandling` `hook` `legacy` `mdi` `powermanagement` `systeminfo` `text`; 5: `accelerator`(the probe in step 1) `help` `ime` `monitor` `processthread` `windowproperties`; 6: `systemshutdown`; 7: `displayconfig` `rawinput`; 8: `caret` `devicecontext` (19) | 98 |
   | B3 (10-15) | `mouse` `rectangle` `scrollbar` `cursor` `icon` `touch` `windowclass` (7) | 90 |
   | B4 (18-25) | `desktop` `clipboard` `dpi` `dialog` `message` `misc` `painting` (7) | 153 |
   | B5 (32-39) | `pointer` `keyboard` `strings` `menu` (4) | 139 |
   | B6 | `window.cpp` alone (1) | 86 |

   Batch totals: **53 TUs / 597 sites** (`accelerator.cpp`'s 5 counted in B2). Any batch that re-sums
   differently means the inserter missed or double-hit a site - stop and re-measure, do not proceed.
3. **Edit mechanics:** scripted insertion; per file snapshot first, then
   `if($null -eq $t) throw` and `if($out.Length -lt 1) throw` immediately before any write (the
   `record-editing.md` truncation trap), re-measure after each file, keep sources **pure ASCII**, keep
   indentation exact, and add the include **only if absent** so a re-run is a no-op.
4. **Re-census after every batch:** the invariant is per-TU
   `qor_pp_fcontext; == qor_pp_useswinapi* == Library::*Call< == Win32ErrorHandler`, and folder totals
   **597 / 597 / 597 / 597** with 53 TUs carrying the include.


5. **Gate** (`status.md:158-165`): edited TU standalone at `/W4` exit 0, then the folder gate
   **54/54 exit 0, 0 errors, 0 warnings** - recreate `tmp/_tmp_u1_gate54.bat` by slug from the standing
   command line (the `.bat` files are scratch and uncommitted; `tmp/_tmp_u8_gate54.bat` is the shape to
   copy). `/Fo` must be **attached** to the switch (`/Fo c:\RAID0\Develop\linuxQOR\tmp\obj\`); the spaced
   form writes `.obj` into the current directory and prints `D9024`/`D9027`. Date the run with
   `cmd /c dir /T W`, never pwsh (`Get-ChildItem` returns an empty `LastModifyDate` on this tree).
   Run `cl /EP` after the last edit and confirm the 597 binding literals are byte-unchanged -
   `/EP` output is one line per source line, so the literal survives while its line number shifts
   (the banner weight of this change is +597 statement lines +53 include lines).
6. **All-libraries 122-source gate: not run** - decision 2 excludes `wsprintfT`, so `user32.h` and every
   shared header stay untouched. If a later session edits `user32.h`, the 122-source diff becomes
   mandatory (run **once**, diff not exit code; baseline 1 `C1083` at `opengl\wgl.cpp:31`, 166
   `warning C|error C` lines, exit 2).
7. **Records, at the close only** (decision 3): `D50` in `docs/agent/DECISIONS.md` + `D50.md`, a
   **working record** in `registers/u1.md` in `u17.md`'s shape (checklist + tick state + measured
   post-pass figures), `status.md`'s boot line and its `Open:` list (`U1` -> closed with D50; `U2`
   stays open), `registers/README.md`'s closed list, `completed_work.md`, `memory-bank/activeContext.md`.
   **Never edit the ~35 TU banners** - protocol 7 freezes review banners; the stale "0
   `Win32ErrorHandler`" wording is corrected in `u1.md`/`D50`, per the U17/D40 precedent.
8. **Commit** the 53 sources + the records in one commit, then stop - no read-back pass; `git show` is the audit.

## 6. Known unknowns to settle before/during the pass (do not guess)

- **`/W4` on an unused named object.** `kernel` and `shell` already carry ~1,500 `Win32ErrorHandler _;`
  sites at `/W4` exit 0, so `C4101` (unreferenced local) is **not** emitted by MSVC - but the
  `accelerator.cpp` probe (step 1) proves it for this folder rather than inheriting the claim.
- **`qor_pp_fcontext` interaction.** The macro expands to a context object whose dynamic extent must
  **outlast** the handler for the issue's `Where`/`When` payload to stay valid. `kernel`/`shell` place
  the handler after `qor_pp_fcontext;`, which is the ordering this plan copies; if the probe prints an
  ordering diagnostic, that is where the fix belongs.
- **Destruction order at scope exit.** Handler dtor `Pop()`s before the context object dtor runs -
  same as the sibling layers, but confirm at the probe that no issue raised during unwinding reads a
  popped context (measure, do not assume).
- **The one prose `qor_pp_fcontext;` hit** (sec. 3) - its exact location must be instrumented before the
  inserter is trusted, so the anchor cannot be fooled.

## 7. What this plan does NOT do

- No `CheckReturn`/`CheckNonZeroHandle` anywhere (U2 untouched, and its policy stays settled).
- No `user32.h` edit, no `wsprintfT` handler, no shared-header edit (decision 2).
- No TU banner edits, no append to frozen/closed records (`tooling.md` is frozen at session 34).
- No project-wide `UNICODE`/`WINVER`/`_WIN32_WINNT` pin, no `NO_STRICT` change (D21/D46 residuals stay).
- No change to the 4 existing `CheckReturn` sites' semantics, and none to `errorhandler.h`/`errorhandler.cpp`.

