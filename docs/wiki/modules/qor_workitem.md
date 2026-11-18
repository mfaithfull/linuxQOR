---
Type: WikiPage
PageType: module
Title: "qor_workitem"
Target: qor_workitem
Status: current
LastVerified: 2026-11-18
Claims: 9
Summary: "Lock-free work-contract groups over the untargeted signaltree headers; the normative CONCURRENCY.md is cited twice and exists nowhere, and schedule() is the one verb that trusts its own vtable."
Anchors:
  - { kind: in-tree, path: src/framework/parallel/workitem, symbol: qor_workitem, verified: 2026-11-18 }
  - { kind: in-tree, path: src/framework/parallel/workitem/CMakeLists.txt, symbol: qor_workitem, verified: 2026-11-18 }
  - { kind: in-tree, path: src/framework/parallel/workitem/itemgroup.h, symbol: workitem_group, verified: 2026-11-18 }
  - { kind: in-tree, path: src/framework/parallel/workitem/this.h, symbol: this_contract, verified: 2026-11-18 }
  - { kind: in-tree, path: src/framework/parallel/workitem/this.cpp, symbol: GetCurrentWorkItem, verified: 2026-11-18 }
  - { kind: in-tree, path: src/framework/parallel/signaltree, symbol: qor, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_workitem --parallel 8
---

# qor_workitem

"(parallel) work item support." (`docs/agent/purposes.md:53` - five words, the thinnest line an
implemented target carries in that file). Built SHARED from two translation units at version 0.5.0
(`src/framework/parallel/workitem/CMakeLists.txt:4`): `qor_workitem.cpp` is the module stamp and
`this.cpp` the TLS definition, so the 39 KB of headers in the folder - `itemgroup.h` alone 23,558
bytes - compile into whoever includes them.

- 2 of the 281 `CMakeLists.txt` under `src`, `test` and `examples` name it: its own and
  `test/framework/parallel/workitem/CMakeLists.txt:12` (`QORTestWorkItem`, configured at
  `test/CMakeLists.txt:130`). Nothing in `src` or `examples` links it. The link line
  `qor_module qor_host` (`CMakeLists.txt:14`) is copied token for token by
  `docs/agent/repo_map.md:66`; 0 files in the folder mention `Host` - the sibling is needed to
  resolve `TheHost()`, which `qor::Module`'s constructor calls on this target's own `ThisModule()`.
- The engine sits on a second library that is not a target. `src/framework/parallel/signaltree/` is
  10 headers with no `CMakeLists.txt` - absent from `src/framework/parallel/CMakeLists.txt`, which
  lists `task thread sync workitem`, and absent from the 90 - and four of workitem's headers include
  five of its headers (`id.h:12`, `item.h:14`, `sharedstate.h:13-14`, `itemgroup.h:24`).
  `workitem_id` is an explicit wrapper over its `signal_id`: "the value is the signal index within
  that group's signal_set so conversion is free" (`id.h:16-17`), with `to_signal_id()` at `id.h:35`.
  signaltree still gets its own suite - `QORTestSignalTreeSet`, five test files, configured at
  `test/CMakeLists.txt:129` - and that test's link line (`test/.../signaltree/CMakeLists.txt:16`)
  names no workitem: the header-only library is judged directly while shipping no target.
- The ABA design is the stated model: the per-contract state word packs three flag bits
  (`schedule_flag/execute_flag/release_flag`, `sharedstate.h:51-53`) above a generation counter
  (`generation_mask/generation_increment`, `:57-58`); mutation runs through
  `compare_exchange_weak(..., acq_rel, acquire)` (`:110`) and a stale generation "is silently
  ignored" (`item.h:145-146`, checked at `sharedstate.h:117`); the group's reference count is
  `memory_order_relaxed` (`:83`). `workitem_group` is a template on
  `signals_per_subtree = 512` and `synchronization_mode` (`itemgroup.h:45`) - the default matches
  `default_workitem_group_capacity = 512ull` (`itemgroup.h:30`).
- What crosses the DLL is three symbols, not the engine: `workitem_id` (`id.h:18`) and `workitem`
  (`item.h:20`) carry `qor_pp_module_interface(QOR_WORKITEM)` - the token's only 2 hits in all of
  `src` - and `GetCurrentWorkItem()` carries plain `qor_pp_export` (`this.h:16`, `this.cpp:11`).
  `workitem_group` has no marker at all: it is non_copyable, non_movable (`itemgroup.h:46`) and
  header-only, so every consumer instantiates its own; the .dll exists to stamp the registry and own
  the TLS.
- The TLS mechanism is deliberately function-mediated: `GetCurrentWorkItem()` returns
  `this_contract*&` to the single `thread_local` member defined once in `this.cpp:9`, so contracts
  installed from any module share one stack; `this_contract`'s constructor links itself to the
  previous (`this.h:30`, `:36`) and its destructor restores it (`:41`) - the "opaque pointer plus
  thunks" design its comment describes (`this.h:18-20`), reaching the group through
  `void* group_` and two function pointers (`this.h:56-59`) to stay off the group's template
  parameters. The folder's files are named `this.h`/`this.cpp` after a C++ keyword, and
  `CMakeLists.txt:4` reads `add_library(${target} SHARED qor_workitem.cpp this.cpp)`.

## Failure modes

- The concurrency spec this page's claims rest on is not in the tree. `itemgroup.h:40-44` names it
  as the authority twice over - "The concurrency model -- thread-safety of each call, the state-word
  invariants, the ABA/generation design, and the one memory-ordering dependency on signal tree -- is
  written up in this library's CONCURRENCY.md" - and `test/framework/parallel/workitem/test_main.cpp:5`
  sends its reader to `../library/workitem/CONCURRENCY.md`, a path whose first segment does not exist.
  The tree holds 0 files named `CONCURRENCY*` and no `library/workitem` directory; what survives in
  the repo for a lock-free engine is header comments, the five-word index line, and no `purpose.md`
  in the folder. Two callers were pointed at a document that was never committed.
- `schedule()` is the one verb with no guard. The empty handle is the designed state -
  `operations const* operations_{};` (`item.h:101`), and the move-assignment zeroes both the state and
  the function table (`std::exchange(other.sharedState_, nullptr)` /
  `std::exchange(other.operations_, nullptr)`, `item.h:128-129`) - and `release()` (`:156-157`),
  `reset()` (`:164-166`) and `is_valid()` (`:174-175`) all test `sharedState_` before use. `schedule()`
  does not: `(*operations_)[schedule_operation](sharedState_, id_, generation_);` (`item.h:150`) calls
  straight through the table pointer, so scheduling a default-constructed or moved-from handle is a
  call through null - while the comment above it (`item.h:145-146`) promises stale handles "silently
  ignored", which is true only of a *generation* mismatch, evaluated inside a live `shared_state`.
  The suite that exists hammers exactly this ground - "stale-handle safety, handle-outlives-group"
  (`test_main.cpp:3-5`) - but only the guarded verbs can meet a handle in that state today.
- The TLS verbs are the guarded half and their neighbours are not. `this_contract::schedule()`,
  `release()` and `get_id()` dereference `GetCurrentWorkItem()` with no check
  (`this.h:44`, `:46`, `:48`), all three `noexcept`, while `is_executing()` (`this.h:50`) exists
  precisely because the TLS pointer is null off-contract - a caller has to know to ask first, and
  work that escapes its contract scope (a detached thread, a deferred callback holding the static)
  gets a null dereference with no assert in the path.
- The judge prints a warning on every clean build of the target's one real translation unit:
  `this.h(61,5): warning C4324: 'qor::par::this_contract': structure was padded due to alignment
  specifier` - `alignas(64)` is carried twice (`this.h:14` forward, `:21` definition) and MSVC says so
  at the closing brace every time `this.cpp` compiles. Green, but the only compile-time noise in the
  leaf, and it lands on the file that owns the TLS.

## See also

[[qor_sync]] [[qor_thread]] [[qor_task]] [[qor_host]] [[qor_module]]

