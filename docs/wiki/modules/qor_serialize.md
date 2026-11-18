---
Type: WikiPage
PageType: module
Title: "qor_serialize"
Target: qor_serialize
Status: current
LastVerified: 2026-11-18
Claims: 15
Summary: "The io-tree serialization engine (folder io/deserializer, namespace serial): a qor_workflow state machine that parses an octet stream one byte per state-Enter and drives TTF font parsing in qor_resources - with an off-by-one written into both byte-walk paths and two of its six links missing from the index and graph."
Anchors:
  - { kind: in-tree, path: src/components/io/deserializer, symbol: qor_serialize, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/deserializer/CMakeLists.txt, symbol: qor_serialize, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/deserializer/deserializer.h, symbol: Deserializer, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/deserializer/state.cpp, symbol: DeserializerState::DeserializerState, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/deserializer/deserializer.cpp, symbol: Deserializer::Read, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/deserializer/structstate.h, symbol: StructureDeserializer, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_serialize --parallel 8
---

# qor_serialize

Configured at `src/components/io/CMakeLists.txt:1`; the folder is 9 files, 738 lines -
the biggest lib in the group by far - of which the dll builds only 4 TUs
(`CMakeLists.txt:3`: bootstrap, `context.cpp`, `deserializer.cpp`, `state.cpp`) and the
four headers, 254 of them in `structstate.h`, compile nowhere inside the target.
`FEATURES.md:121` carries it as "Experimental" and `docs/agent/purposes.md:80` calls it
"serialization support" under `(io/deserializer)`.

- The lib carries five names: target `qor_serialize` (`CMakeLists.txt:1`), folder
  `src/components/io/deserializer`, namespace `qor::components::serial`
  (`deserializer.h:33`), compile token `QOR_SERIAL` (`CMakeLists.txt:8`), and bootstrap
  module string "Querysoft Open Runtime: Deserializer Module" (`qor_serialize.cpp:33`).
- The index and graph understate it: the link line names six libraries - `qor_module
  qor_thread qor_sync qor_injection qor_workflow qor_pipeline` (`CMakeLists.txt:13-14`) -
  while `docs/agent/repo_map.md:99` and `docs/agent/deps.dot:120` list only the first
  four, dropping the two the code actually leans on most.
- The reverse orphan: `deserializer.cpp` includes `src/qor/flyers/error/error.h` (`:28`)
  and fires `serious("No initial state set for Deserializer.")` (`:39`), but
  `qor_error` appears in no link line, index or graph for this target.
- The core is a workflow, not a reader: `class qor_pp_module_interface(QOR_SERIAL)
  Deserializer : public Workflow` (`deserializer.h:37`), with the header comment "A
  workflow where each state becomes the effective sink of a pipeline" (`:35`);
  `Deserializer::Read` (`deserializer.cpp:34-78`) resets `m_complete` (`:36`), pumps
  `CurrentState()->Enter()` while the workflow is incomplete and the context has unread
  octets (`:47-54`), and returns the workflow's `m_result` after swallowing every
  exception class to `cerr` (`:63-74`).
- The byte cursor is three ints: `Context` holds `m_octetStream/m_position/m_size`
  (`context.h:63-65`); `GetOctet` guards with `m_position < m_size` (`context.cpp:44-48`)
  and `ConsumeOctet` increments unconditionally (`:38`), so the only thing keeping
  `HasUnreadData`'s unsigned subtraction (`context.h:58`) from wrapping is the
  discipline of callers.
- Primitive decoding is a one-byte-per-call state: the `DeserializerState` ctor seeds
  `m_index` at 0 or `m_size - 1` depending on source-vs-host endianness
  (`state.cpp:35-38`) and the `Enter` lambda writes one octet per call at
  `m_data[m_index]`, walking forward or backward and popping the state with `SUCCESS`
  at the far end (`:40-77`).
- The forward walk stops one byte late: the host-endian branch tests `m_index ==
  m_size` only after writing `m_data[m_index]` (`state.cpp:45,50`), so for a size-N
  object the Nth call writes `m_data[N-1]` and advances to N, and the (N+1)th call
  writes `m_data[N]` - one past the buffer - and only then declares `SUCCESS`; the
  reversed branch terminates correctly at index 0 (`:62`).
- The defect is forked, not shared: `PrimitiveState<T>` (`structstate.h:34-111`) derives
  from `workflow::State` - not from `DeserializerState` - and re-implements the same
  `Enter` body including the same `m_index == m_size` test (`:56`), re-declaring the
  five members and all three `dynamic_cast` accessors; `StructState` copies the three
  accessors a second time (`:184-199`) and the terminal specialization a third
  (`:226-241`).
- Struct parsing rides the reflection library: `StructState<struct_T,I,N>::Enter` calls
  `StateSelector<I, fieldCount>::Select` over `reflection::detail::tie_as_tuple`
  (`structstate.h:162-167`), and each `Select` pushes exactly one state - its tail call
  into `Select(I+1)` passes `recurse = false` (`:141`) only for the callee to return
  false on that flag (`:121-124`), so the apparent recursion is vestigial and the
  field stepping happens in `Resume`, which `SetState`s `StructState<struct_T, I+1, N>`
  (`:175-178`); `StructureDeserializer<t>::base` is the alias consumers use (`:246-250`).
- `StructState` carries dead plumbing: `m_data` is assigned from the object pointer
  (`:159`) but read by no lambda in either specialization, and `m_index` is declared
  (`:203`) never initialized and never touched.
- The structstate layer compiles only in consumers: the 254-line header builds no TU in
  this dll, and its templates instantiate in exactly one place in-tree -
  `src/framework/data/resources/types/font/reader/ttfinitial.h:11` - the TTF font
  parser, whose states derive from `StructureDeserializer<T>::base`
  (`ttfinitial.cpp:43-48`) and whose `DesignatorState` constructs `DeserializerState`
  directly with big-endian `uint32_t` (`designator.cpp:10`).
- The two consumers use it unevenly: `qor_deserializersink` includes `deserializer.h`
  and `context.h` (`deserializersink.h:30-31`) and holds a
  `qor::components::serial::Deserializer` member (`:56`) but its link line
  (`CMakeLists.txt:13`) omits `qor_serialize` - the template surface pushes the symbol
  requirement out to whoever instantiates it; `qor_resources` links the lib outright
  (`src/framework/data/resources/CMakeLists.txt:23`).
- No test links or names it: grep for `qor_serialize`/`deserializer` across `test/`
  finds nothing, and the whole license family is the 24-line MIT block "2008 - 2025"
  (`deserializer.h:1-23` style) with a 35-line bootstrap of the same era - like
  generatorsource and randomsource, unlike stringsource's 2-line SPDX. The judge linked
  it exit=0 into a 1,019,392-byte dll stamped 11:33, newer than every file in the
  folder (newest, `CMakeLists.txt`, 24 Oct 2026).

## Failure modes

- Every host-endian primitive parse overruns by one: `PrimitiveState<T>` maps
  `m_data` onto `sizeof(T)` bytes (`structstate.h:41-42`) yet its `Enter` writes
  `m_data[m_size]` before succeeding (`:51,56`), so each little-endian field both
  scribbles one byte into its successor and consumes one extra octet from the stream,
  silently shifting every later field in the struct.
- The pump is loud and lossy: each `Read` prints "Stack on entry has N states." to
  stdout unconditionally (`deserializer.cpp:44`, again `:60,76`), and after catching
  `Serious`, `std::exception` or anything else to `cerr` (`:63-74`) it returns the
  half-written `m_result` with no error channel - a caller cannot tell a completed
  parse from one that died in an `Enter`.

## See also

[[qor_deserializersink]] [[qor_resources]] [[qor_workflow]] [[qor_pipeline]]
[[qor_module]] [[qor_thread]] [[qor_sync]] [[qor_injection]] [[qor_error]]

Not yet paged: `qor_generator` (group 3 leaf 5), `qor_netclient` and `qor_httpclient`
(group 3 leaves 6-7).
