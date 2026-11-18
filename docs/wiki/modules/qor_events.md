---
Type: WikiPage
PageType: module
Title: "qor_events"
Target: qor_events
Status: current
LastVerified: 2026-11-16
Claims: 10
Summary: "Ring-buffer event queue, self-declared prototype; it exports no class, is linked by nothing, and its own index calls three of its four dependencies nothing it includes."
Anchors:
  - { kind: in-tree, path: src/framework/event/events, symbol: qor_events, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/event/events/CMakeLists.txt, symbol: qor_events, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/event/events/eventqueue.h, symbol: PODQueue, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/event/events/eventqueue.h, symbol: AbstractQueue, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/event/events/eventsource.h, symbol: Source, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/event/events/eventsink.h, symbol: Sink, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_events --parallel 8
---

# qor_events

"Memory-stable publish/subscribe events queueing (prototype)." (`docs/agent/purposes.md`). Built
SHARED from `qor_events.cpp eventqueue.cpp` at version 0.0.2
(`src/framework/event/events/CMakeLists.txt`).

- Its link line is `qor_module qor_thread qor_sync qor_interception`
  (`src/framework/event/events/CMakeLists.txt`) and `docs/agent/repo_map.md:58` copies it token for
  token - one of the few exact matches in that index. But 13 `#include` lines exist across the six
  source files in the folder and none names thread, sync or interception; the only QOR
  headers it reaches are `src/platform/compiler/compiler.h` (`eventqueue.h`),
  `src/qor/module/module.h` (`qor_events.cpp`) and one header from a folder that is not a target at
  all. Three of the four dependencies are asserted by a CMakeLists no include contradicts, because no
  include looks.
- Nothing links it: of the 281 `CMakeLists.txt` under `src`, `test` and `examples`, exactly 1 names
  `qor_events` - its own. Its judge is green. The only file in the tree that uses it,
  `examples/experimental/eventer/main.cpp` (lines 21-22 include `eventsource.h` and `eventsink.h`,
  lines 96-106 build a queue, a `Source`, a `Sink` and call them), is unreachable: 1 occurrence of the
  string `eventer` exists in any CMakeLists under `examples` - its own `set(target eventer)` - so no
  `add_subdirectory` names it, and `eventer.vcxproj` has 0 matches under `cmakebuild`. It would not
  link this target even if it were configured: `qor_events` is absent from both branches of that
  example's `target_link_libraries`.
- The token is defined and never used. `QOR_EVENTS` has **0 hits in `src`**: line 9 of its
  `CMakeLists.txt` defines it and no file consumes it, so no class in the folder carries
  `qor_pp_module_interface` and the SHARED target exports no C++ class. That is also why the example
  above links nothing and still works - `eventqueue.cpp` is 9 lines whose body is an empty
  `namespace qor{ namespace events{ }}` (`src/framework/event/events/eventqueue.cpp`),
  `qor_events.cpp` contributes only `ThisModule()`, and `AbstractQueue`, `PODQueue`, `Sink` and
  `Source` are all templates or inline. Meanwhile `eventqueue.h` includes the header that provides the
  export macros (`src/platform/compiler/compiler.h`) and applies none.
- `Source` reaches `src/framework/event/delegate/bindmember.h` (`eventsource.h`), and
  `src/framework/event/delegate/` is five headers plus a `purpose.md` with **no `CMakeLists.txt`** - a
  folder the repo's per-folder rule knows about and no target builds. It is compiled anyway:
  `src/framework/data/resources/claimer.h:8` includes the same header, so `bindMemberFunction` is
  duplicated into `qor_resources` and anything else that reaches for it.
- The purpose file is 12 words: "Intends to provide memory stable publish subcribe events queueing /
  Currently a prototype" (`src/framework/event/events/purpose.md`, `subcribe` as written), and the
  `docs/agent/purposes.md` row restates it as what the target does, keeping "(prototype)" as the only
  hedge. The subscribe half is absent: `category` is written at `eventsource.h`, copied at
  `eventsink.h`, and read by nothing - there is no subscriber list, no filter, no dispatch.
- `typedef PODQueue<struct event, size_t> EventQueue;` (`src/framework/event/events/eventqueue.h`)
  names a queue of a struct that is not POD: `struct event` holds `std::function<void(void)> release`
  (`src/framework/event/events/event.h`), which is neither trivially copyable nor standard-layout, and
  `Sink::GetMessage()` returns `struct event` **by value** (`src/framework/event/events/eventsink.h`),
  so every message copies a `std::function` - the one thing the file's own comment
  ("The one and only bit of memory management;", `eventqueue.h`) claims is managed once.

## Failure modes

- The two capacity functions that matter do not override anything. `AbstractQueue` declares
  `virtual size_type WriteCapacity() const` and `virtual size_type ReadCapacity() const`
  (`src/framework/event/events/eventqueue.h`); `PODQueue` declares both again **without the `const`**
  (`src/framework/event/events/eventqueue.h`), which is a different signature, and 0 occurrences of
  `override` appear in the file so nothing says so. The base's virtual slots therefore still resolve to
  the base bodies - the ones with no wrap-around guard - while `PODQueue::ReadCapacity` carries the
  guard (`if(result > this->m_allocationCount) { result = 0; }`) precisely because
  `m_write - (m_read + m_beingWritten)` on `std::atomic<size_type>` is enormous once a read has raced
  ahead of a write. A caller holding the abstraction this file defines, `AbstractQueue<size_t>&`, gets
  the unguarded number and no diagnostic.
- Three unbounded spin loops, no wait and no error path:
  `while(eventData == nullptr) { eventData = m_stock.WriteBegin(); }` and
  `while(eventMessage == nullptr) { eventMessage = m_queue->WriteBegin(); }`
  (`src/framework/event/events/eventsource.h`), and
  `while(evt == nullptr) { evt = m_queue->ReadBegin(); }`
  (`src/framework/event/events/eventsink.h`). Both `Begin` functions also return `nullptr` while
  `m_beingWritten > 0` / `m_beingRead > 0` (`src/framework/event/events/eventqueue.h`), and nothing in
  the folder owns that flag: no RAII guard, no owner id, no timeout. Two `Source`s or two `Sink`s over
  one `EventQueue` - which the constructors invite, both take a bare `EventQueue*` - therefore hang
  each other, and any early return between a `ReadBegin` and its `ReadEnd` leaves `m_beingRead`
  non-zero for the life of the queue.
- `Source::Release` dereferences a pointer its own API can return as null:
  `event_t* eventData = m_stock.ReadBegin(); eventData->Reset();`
  (`src/framework/event/events/eventsource.h`) - `ReadBegin()` returns `nullptr` when the stock is
  empty or a read is already in flight (`src/framework/event/events/eventqueue.h`). The `Sink` copies
  the token out with the message (`e.release = evt->release;`,
  `src/framework/event/events/eventsink.h`) and the token is
  `bindMemberFunction(&Source<event_type>::Release, this)` (`src/framework/event/events/eventsource.h`) -
  a raw `this` with nothing keeping the `Source` alive while the queue is still holding slots that
  name it. The only call of that token anywhere is `e.release();` at
  `examples/experimental/eventer/main.cpp:106`, in the example no build configures.
- The derived constructor silently depends on the base constructor's virtual call failing.
  `AbstractQueue(size_type unitSize, size_type itemCount)` ends with `SetCapacity(itemCount)`
  (`src/framework/event/events/eventqueue.h`), and a virtual called from a constructor resolves to the
  constructor's own class - `AbstractQueue::SetCapacity`, which only zeroes the counters and never
  touches `m_pAllocation`. The allocation happens only because `PODQueue`'s constructor calls
  `SetCapacity(itemCount)` a second time in its body (`src/framework/event/events/eventqueue.h`),
  which looks like a redundant line and is the only thing separating `m_allocationCount > 0` with
  `m_pAllocation == nullptr` - a state `AddressOf` reports as a valid-looking `nullptr + index`
  (`event_t* pResult = nullptr; … pResult = m_pAllocation + (index % this->m_allocationCount);`).

## See also

[[qor_pipeline]] [[qor_image]] [[qor_resources]] [[qor_module]]

