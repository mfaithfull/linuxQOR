---
Type: Plan
Title: "message rows"
Parent: "../plan.md"
Level: 4
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

# message rows

Slug: `MessageRows` - from title "message rows" (no drop-words, PascalCased, 11 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **24** `message.cpp` rows in 3 slices of at most 10 URLs.

## Resume

Cursor: **terminal** - all 3 items `Atomic`, all 3 closed 12 Nov 2026. All **24** rows of `message.cpp` carry the verdict triple: `checked 10 / unchecked 13 / void 1`, `tmp/_tmp_u2_tu_state.ps1 message.cpp` `rows=24 checked=10 unchecked=13 void=1 blank=0 shape_bad=0`; the folder single-pass audit `tmp/_tmp_u2_folder_audit.ps1` `rows=597 filled=342 checked=205 unchecked=131 void=6 sum=342 class-shape violations=0 filled rows with no docQuote=0`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: desktop rows, clipboard rows, dpi rows, dialog rows, misc rows, painting rows.

Slices fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM message`: 24 rows in slices of 10/10/4 in table order. 12 of 24 rows have 0 in-tree callers, so the other 12 need the caller count recorded in `notes` before a check could land. `GetInputState`, `GetQueueStatus`, `InSendMessage*`, `WaitMessage` are the answer-shaped rows expected to end `unchecked` with the page's wording.

## message slice 1

Rows 1-10: `BroadcastSystemMessage`, `BroadcastSystemMessageEx`, `DispatchMessage`, `GetInputState`, `GetMessage`, `GetMessageExtraInfo`, `GetMessagePos`, `GetMessageTime`, `GetQueueStatus`, `InSendMessage`. One fetch batch of 10, then write those 10 rows.

## message slice 2

Rows 11-20: `InSendMessageEx`, `PeekMessage`, `PostMessage`, `PostQuitMessage`, `PostThreadMessage`, `RegisterWindowMessage`, `ReplyMessage`, `SendMessageCallback`, `SendMessageTimeout`, `SendMessage`. One fetch batch of 10, then write those 10 rows.

## message slice 3

Rows 21-24: `SendNotifyMessage`, `SetMessageExtraInfo`, `TranslateMessage`, `WaitMessage`. One fetch batch of 4, then write those 4 rows.

## Cursors

message slice 1: {"tu":"message.cpp","slice_start":0,"slice_len":10,"done":true}
message slice 2: {"tu":"message.cpp","slice_start":10,"slice_len":10,"done":true}
message slice 3: {"tu":"message.cpp","slice_start":20,"slice_len":4,"done":true}

## Closed 12 Nov 2026 - evidence

- Slice 1 (rows 1-10) in one fetch batch: `tmp/_tmp_u2_msg_fetch.out.txt` - 10 `FETCH ... match-cache http=200` lines (one `CORRECTED` leg), `fetched=10 of 10 bad=0 no-page=0`, `VERDICT fail=0`.
- Slice 2 (rows 11-20) in one fetch batch: `tmp/_tmp_u2_msg_fetch_s2.out.txt` - 10 `match-cache` at HTTP 200 with 5 `CORRECTED` legs, `fetched=10 of 10 bad=0 no-page=0`, `VERDICT fail=0`.
- Slice 3 (rows 21-24) in one fetch batch: `tmp/_tmp_u2_msg_fetch_s3.out.txt` - 4 `match-cache` at HTTP 200 with 1 `CORRECTED` leg, `fetched=4 of 4 bad=0 no-page=0`, `VERDICT fail=0`.
- 24 verdict lines appended to `tmp/_tmp_u2_decisions.txt` (the `message.cpp` block) and written into the table by `tmp/_tmp_u2_patch.ps1`; every row carries its in-tree caller count in `notes`, and all 24 docQuote columns hold the page's verbatim Return value text (`filled rows with no docQuote = 0`).
- The 404 fallback leg of the slice contract is load-bearing for this TU: 8 of 24 rows have no page under the plain `nf-winuser-<stem>` slug and were fetched at the suffixed ANSI twin (`...exa`, `peekmessagea`, `postmessagea`, `postthreadmessagea`, `registerwindowmessagea`, `sendmessagecallbacka`, `sendmessagetimeouta`, `sendnotifymessagea`), each printed as a `CORRECTED` line. This is the opposite of the misc leaf, where the leg never fired.
- Instrument fix recorded here: the per-slice report tag is `[Math]::Floor($skip / $take) + 1`, not the `[int]` rounding the misc instrument used - the misc leaf's third batch wrote `_tmp_u2_misc_fetch_s8.out.txt` (20/3 rounds to 7) and was renamed to `_tmp_u2_misc_fetch_s3.out.txt` after the fact.
- `PostQuitMessage` (pubRetType void, sdkRetType VOID, 3 in-tree callers) is the leaf's `void` row; its docQuote is the page's verbatim one-word Return value section `None`.
- Four class shapes appear here that no earlier leaf used: `TCheckFailureValue< long, -1 >` (the `BroadcastSystemMessage` pair - success is a positive recipient count, -1 is "unable to broadcast", and **zero is a legal answer** for a `BSF_QUERY` denial, so the check must key on -1 alone and never on nonzero-ness), `TCheckFailureValue< BOOL, -1 >` (`GetMessage` - zero is the legal `WM_QUIT` loop terminator and the page's own recommended test is `if (bRet == -1)`, so `BoolCheck` / `CheckNonZero` would report a legal `WM_QUIT` as a failure; the base is the published `BOOL`, which the preprocessed message TU shows as `typedef int BOOL` at `_tmp_msg_pp_final_s23.txt:145773`, so -1 survives the comparison), `TRangeCheck< UINT, 0xC000, 0xFFFF >` (`RegisterWindowMessage` - the page states the success range as bounds, so the range class is the tighter of the two that fit and `CheckNonZero< UINT >` is the weaker alternative; first use of `TRangeCheck` in the tree), and `CheckNonZero< LRESULT >` (`SendMessageTimeout` - the published `LRESULT` row type rules `BoolCheck` out by type).
- Deviation from this leaf's `## Context`, which predicted the unchecked set as `PostQuitMessage`, `SendMessageTimeout` and the answer-BOOL rows: measured, `PostQuitMessage` is `void` rather than unchecked, and `SendMessageTimeout` is **checked** - its page reserves zero for both failure states ("If the function fails or times out, the return value is 0") and adds its own reading rule ("If the function returns 0, and GetLastError returns ERROR_SUCCESS, then treat it as a generic failure"), so zero is never a legal answer. The `SendMessage` twin stays unchecked on the opposite reading: "The return value specifies the result of the message processing; it depends on the message sent." - the window procedure's own answer, with no failure value named. The pair is recorded in both reasons.
- The answer-shaped set this leaf actually books unchecked (13): `DispatchMessage` (the window procedure's own answer, "generally is ignored"), `GetInputState`, `GetQueueStatus`, `PeekMessage` (the queue-state question - zero answers an empty queue), `InSendMessage`, `InSendMessageEx` (`ISMEX_NOSEND` 0x00000000 is the legal answer for a window procedure handling its own message), `ReplyMessage` (the Remarks make zero the ordinary outcome: "If the message was not sent through SendMessage or if the message was sent by the same thread, ReplyMessage has no effect"), `TranslateMessage` (a message that posts no character message answers zero legitimately), `GetMessagePos`, `GetMessageTime`, `GetMessageExtraInfo`, `SetMessageExtraInfo` (the previous value, 0 on a thread's first call).
- 12 of 24 rows carry nonzero in-tree callers (1 each, `PostQuitMessage` 3), matching this leaf's `## Context`; every one of them carries its caller count in `notes` as the audit-before-landing marker. The 12 zero-caller rows are booked change-safe.
- No code change is proposed here; only the verdict triple. `message.cpp` currently has **0** live `CheckReturn` sites (the leaf's code census), so all 10 `checked` rows would be the first checks in this TU.

