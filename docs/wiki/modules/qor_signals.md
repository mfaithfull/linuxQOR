---
Type: WikiPage
PageType: module
Title: "qor_signals"
Target: qor_signals
Status: current
LastVerified: 2026-11-19
Claims: 11
Summary: "Vendored CsSignal in-process pub/sub; the SignalBase class marker now tests its own QOR_SIGNALS token (commit 154e547dba), and an AutoConnection queues only for the one receiver that overrides compareThreads()."
Sources:
  - hook-154e547dba-qor_signals
Anchors:
  - { kind: in-tree, path: src/framework/event/signals, symbol: qor_signals, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/event/signals/CMakeLists.txt, symbol: qor_signals, verified: 2026-11-19 }
  - { kind: in-tree, path: src/framework/event/signals/signalbase.h, symbol: SignalBase, verified: 2026-11-19 }
  - { kind: in-tree, path: src/framework/event/signals/signalbase.h, anchor: commit:154e547dba7027165d3124a743f5ea1372846468, verified: 2026-11-19 }
  - { kind: in-tree, path: src/framework/event/signals/slotbase.h, symbol: SlotBase, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/event/signals/do_signal.h, symbol: activate, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/event/signals/signalbase.cpp, symbol: beingDestroyed, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_signals --parallel 8
---

# qor_signals

"Signals-and-slots publish/subscribe for in-process messaging, with/without threads; lower coupling,
runtime plug-together." (`docs/agent/purposes.md:44`). Built SHARED from `qor_signals.cpp slotbase.cpp
signalbase.cpp pendingslot.cpp` at version 0.5.0 (`src/framework/event/signals/CMakeLists.txt`).

- The target is a vendored library: 11 of its 12 source files open with the CsSignal notice
  (Geller/Sermersheim 2016-2021, BSD 2-Clause) and 22 `CsSignal` hits cover every file except
  `qor_signals.cpp`, the module stamp. It sits under `src/` rebranded - guards `QOR_PP_H_*`, macros
  `qor_pp_signal*` - while `3rdparty/` holds exactly two vendored libraries (angle, glad). Upstream
  trait names survived the rebrand in 3 files: `detail::cs_check_connect_args` (do_signal.h:188,
  internal.h:34), `cs_testConnect_SenderSignal` (do_signal.h:250), `cs_unpack_method_args` (bento.h).
- 41 of the 281 `CMakeLists.txt` under `src`, `test` and `examples` name it, and its link line
  `qor_module qor_host qor_thread` is copied token for token by `docs/agent/repo_map.md:59` - the
  index also lists `qor_log` as depending on it (L42) and stacks it before `qor_pipeline` in 25
  component rows (L86-L111). But no file in the folder includes anything from host or thread: the
  folder's thread primitives are two `qor_pp_thread_local` statics (signalbase.cpp:80,
  slotbase.cpp:73) and `qor_module` is reached once, by `qor_signals.cpp`.
- Its `CMakeLists.txt:4` is one of only 5 in 281 that hard-code the target name into
  `add_library(qor_signals SHARED ...)` instead of `${target}` (the other four: qor_aggregatesource,
  qor_stringsource, qor_error, qor_log).
- `QOR_SIGNALS` has 13 hits in 4 files (signalbase.h, slotbase.h, pendingslot.h, detail/internal.h):
  9 in `signalbase.h` - the `SignalBase` class marker at `:74` plus 8 member markers
  (`:79,83-85,90-91,107,109`) - 1 in `slotbase.h:42`, 2 in `pendingslot.h:32,41`, 1 in
  `detail/internal.h:29`. Every class-level marker in the folder is `qor_pp_module_interface_gcc`,
  which `src/configuration/configuration.h:43` defines as nothing on non-GCC compilers - on this MSVC
  build (the judge runs MSBuild) the class markers are no-ops and the export direction rests entirely
  on the 8 member markers in `signalbase.h`.
- `SlotBase` is exercised by the test suite: `framework/test_signals.cpp` is compiled into QORTest
  (`test/CMakeLists.txt:34`) and `qor_signals` is in its link line (`test/CMakeLists.txt:89`). The two
  framework consumers inherit the base classes: `ResourceManager : public SlotBase` and
  `Resource : public SignalBase` (`src/framework/data/resources/manager.h:23`, `resource.h:26` -
  [[qor_resources]]).
- `SignalBase`'s class marker tests its own token - `class qor_pp_module_interface_gcc(QOR_SIGNALS)
  SignalBase` (`signalbase.h:74`) - it read `QOR_APPLICATION` until commit `154e547dba` replaced it.
  `QOR_APPLICATION` now survives in 3 files of `src` (`application.h`, `builder.h`, and
  `src/framework/parallel/thread/thread.h:17` marking `Thread`), and is defined PRIVATE by
  `src/framework/app/application/CMakeLists.txt` - `qor_thread` never defines it for its own class.

## Failure modes

- Queued connections queue for one receiver only. The base `SlotBase::compareThreads()` is
  `return true;` (`slotbase.cpp:79-82`), so `receiverInSameThread` at `do_signal.h:69` is true unless
  a receiver overrides it, and exactly one does: `LogReceiver::compareThreads()` returns `false`
  (`logreceiver.cpp:89-92`, `logreceiver.h:43`, commit `154e547dba`), so an `AutoConnection` to the
  log receiver takes the queueing branch (`do_signal.h:77-89`) and its `Listen()` drain runs the slot.
  Every other receiver keeps the base answer, and the base `queueSlot` is `data();` under the comment
  "calls the slot immediately" (`slotbase.cpp:84-88`), so an explicit `QueuedConnection` to a receiver
  that did not override it also runs on the emitting thread, paying the clone-and-heap-copy of
  `do_signal.h:80-85` for nothing. `queueSlot` overrides live in 3 places the folder does not own -
  `logreceiver.cpp:82`, `manager.cpp:18`, `test_signals.cpp:103` - and only the first also overrides
  `compareThreads`, which makes the purpose line's "with/without threads" a promise each consumer
  keeps on this library's behalf, one consumer at a time.
- The `BlockingQueuedConnection` branch feeds that hazard. It builds its `TeaCup_Data` with the
  copy-everything flag left false (`do_signal.h:93-95`, the comment reads "passing false indicates the
  data will not be copied"), so the pending slot references arguments living in `activate()`'s stack
  frame; the call does not block anywhere, and the design is only safe because the base `queueSlot`
  runs it inline before `activate()` returns. Any override that actually deferred a PendingSlot built
  this way would execute against a dead frame, and the `ConnectionKind` argument that would tell the
  override so is discarded by the base (`slotbase.cpp:84`).
- `SignalBase`'s DLL direction is decided by its own token, since commit `154e547dba`. The class
  marker (`signalbase.h:74`) tests emptiness on `QOR_SIGNALS` - the token stamped by this target's own
  `CMakeLists.txt:9` - (`configuration.h:33` routes it: empty goes import, otherwise export); until
  that commit it tested `QOR_APPLICATION`, a token this target never defines, which flipped the marker
  to export-side in exactly the builds compiling `qor_application`. The marker stays inert on the
  judged build only because `qor_pp_module_interface_gcc` is a no-op under MSVC.
- The vendored files carry a second licence in their SPDX line. Every one of the 11 CsSignal files
  stamps `// SPDX-License-Identifier: BSL-1.0` (line 2) directly above its own BSD 2-Clause notice, and
  the root `LICENSE` file, which does credit CsSignal at L81, files it under a generic "Portions
  licensed under the following license:" heading - the BSD text it prints at L54 ("3 clause BSD
  licesnse", sic) is the block belonging to "Copyright 2021 Sean Nash". So the machine-readable field
  of these files claims Boost-1.0, their prose claims BSD-2, and the only block the LICENSE labels BSD
  belongs to neither.
- `activate()`'s bookkeeping kept upstream's dead machinery. `sender.m_activateBusy--;` at
  `do_signal.h:151` sits in a `try` whose `catch (std::exception&)` throws
  `invalid_argument("activate(): Failed to obtain sender lock")` - a `--` on an `int` cannot throw -
  while the two early returns above it (`do_signal.h:121`, `do_signal.h:139`, the sender-destroyed
  paths) skip the decrement entirely, leaving `m_activateBusy` one above true for an object whose
  destructor is running. The upstream note on the lock below (`do_signal.h:108`,
  `do_signal.h:130`: "// should be a read lock") was carried over unedited.

## See also

[[qor_resources]] [[qor_pipeline]] [[qor_log]] [[qor_events]] [[qor_module]]

