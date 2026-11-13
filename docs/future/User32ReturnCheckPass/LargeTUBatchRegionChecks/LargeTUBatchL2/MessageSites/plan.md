---
Type: Plan
Title: "message sites"
Parent: "../plan.md"
Level: 3
Items:
  - Title: message slice 1
    Status: Atomic
    Content:
  - Title: message slice 2
    Status: Atomic
    Content:
  - Title: message slice 3
    Status: Atomic
    Content:
---

# message sites

Slug: `MessageSites` - from title "message sites" (no drop-words, PascalCased, 12 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **24** `message.cpp` sites in 3 slices of at most 10 sites.

## Resume

Cursor: **terminal** - all 3 items `Atomic`, all 3 closed 12 Nov 2026. All **24** `message.cpp` sites are in one of the three end states: 10 carry the `CheckReturn< >` token (exactly the 10 `checked` rows), 13 carry a written reason for their `unchecked` row, and `PostQuitMessage` is the `void` site with the one-line VOID reading and no token. `tmp/_tmp_u2_code_census.txt` `message.cpp rows=24 fcontext=24 useswinapi=24 (12 bare + 12 AW) LibraryCall=24 Win32ErrorHandler=24 CheckReturn-code=10 table-checked=10 table-unchecked=13 returncheck-includes=1 ok`; the single-TU compile gate `tmp/_tmp_u2_one_tu.bat message.cpp` `TU_EXIT=0` with a 1-line log (no errors, no `warning C`).

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, caller audit for nonzero counts, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: desktop sites, clipboard sites, dpi sites, dialog sites, misc sites, painting sites. 12 of 24 rows have nonzero in-tree callers - each `checked` row among them needs the site-level read before its token moves; `PostQuitMessage` (3 callers) is the heaviest.

## message slice 1

10 sites in `message.cpp`: `BroadcastSystemMessage`, `BroadcastSystemMessageEx`, `DispatchMessage`, `GetInputState`, `GetMessage`, `GetMessageExtraInfo`, `GetMessagePos`, `GetMessageTime`, `GetQueueStatus`, `InSendMessage`. Land only the `checked` rows. Stop condition: re-read the sites, then re-census `message.cpp` against its `checked` row count.

## message slice 2

10 sites in `message.cpp`: `InSendMessageEx`, `PeekMessage`, `PostMessage`, `PostQuitMessage`, `PostThreadMessage`, `RegisterWindowMessage`, `ReplyMessage`, `SendMessageCallback`, `SendMessageTimeout`, `SendMessage`. Caller audit first for `PostQuitMessage`. Stop condition: re-read the sites, then re-census `message.cpp` - its counts must already match the slice-1 census.

## message slice 3


## Cursors

message slice 1: {"tu":"message.cpp","slice_start":0,"slice_len":10,"done":true}
message slice 2: {"tu":"message.cpp","slice_start":10,"slice_len":10,"done":true}
message slice 3: {"tu":"message.cpp","slice_start":20,"slice_len":4,"done":true}

## Closed 12 Nov 2026 - evidence

- Slice 1 landed 3 tokens (`BroadcastSystemMessage`, `BroadcastSystemMessageEx` at `TCheckFailureValue< long, -1 >`, `GetMessage` at `TCheckFailureValue< BOOL, -1 >`) plus the one `#include "../returncheck.h"` (message.cpp:12) and reason blocks at the 7 `unchecked` rows.
- Slice 2 landed 5 tokens (`PostMessage`, `PostThreadMessage`, `RegisterWindowMessage` at `TRangeCheck< UINT, 0xC000, 0xFFFF >`, `SendMessageCallback`, `SendMessageTimeout` at `CheckNonZero< LRESULT >`) and reason blocks at the 5 `unchecked` rows there.
- Slice 3 landed 2 tokens (`SendNotifyMessage`, `WaitMessage`) and reason blocks at its 2 `unchecked` rows.
- Class bases follow `sdkRetType`, with the published type only where the SDK column is `NO-PROTOTYPE` or blank: `BroadcastSystemMessageEx` (long), `SendMessageCallback` (BOOL), `RegisterWindowMessage` (UINT), `SetMessageExtraInfo` (LPARAM, unlanded). `GetMessage`'s `TCheckFailureValue< BOOL, -1 >` is sound because `BOOL` is `typedef int` in this build's Windows headers, so -1 survives the comparison.
- Caller audits: 12 of 24 rows have nonzero counts (1 each, `PostQuitMessage` 3). The audit found `messagehandler.cpp` compiled (`gui/CMakeLists.txt:60`) but `MessageHandler::Wait()` has **no in-tree invocation**, so every nonzero count here is a declaration-only or unreached site - recorded in each block's `AUDIT:` line, and no call site was changed.
- The `unchecked` reasons are required by the contract at `LargeTUBatchRegionChecks/plan.md:50`, not only the `checked` ones: all 13 now carry one, and the last three (`SendMessage`, `SetMessageExtraInfo`, `TranslateMessage`) were written in this final pass.
- The `SendMessage` / `SendMessageTimeout` pair is the sharpest contrast in the TU: the twin's page names no failure value at all (the return is the window procedure's own answer), while `SendMessageTimeout` reserves 0 for both failure and timeout and adds its own reading rule, so one is unchecked and the other checked.
- `PostQuitMessage` (3 callers, the heaviest) is `void` in the SDK, so it takes `Library::voidCall` and the one-line VOID reading - no token, no test - matching the `SetDebugErrorLevel` precedent in `misc.cpp`.

4 sites in `message.cpp`: `SendNotifyMessage`, `SetMessageExtraInfo`, `TranslateMessage`, `WaitMessage`. Stop condition: re-read the sites, then re-census `message.cpp` - its counts must already match the slice-2 census.
