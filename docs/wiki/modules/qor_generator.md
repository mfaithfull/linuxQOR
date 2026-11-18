---
Type: WikiPage
PageType: module
Title: "qor_generator"
Target: qor_generator
Status: current
LastVerified: 2026-11-18
Claims: 13
Summary: "The io-tree serializer engine, a near twin of qor_serialize pointed the other way: a qor_workflow that walks a node queue and writes octets into a caller buffer - the engine behind qor_generatorsource and the JSON generator - carrying qor_parser's compile token, two pump forks, and an index row that drops two of its seven links."
Anchors:
  - { kind: in-tree, path: src/components/io/generator, symbol: qor_generator, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/generator/CMakeLists.txt, symbol: qor_generator, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/generator/generator.h, symbol: Generator, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/generator/generator.cpp, symbol: Generator::Generate, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/generator/state.h, symbol: GeneratorState, verified: 2026-11-18 }
  - { kind: in-tree, path: test/components/formats/json/generator/test_jsongenerator.cpp, symbol: Generate, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_generator --parallel 8
---

# qor_generator

Configured at `src/components/io/CMakeLists.txt:1` beside its mirror; the folder is 10
files, 703 lines - `CMakeLists.txt` (14), `generator.h/cpp` (90/130), `context.h/cpp`
(68/46), `state.h/cpp` (72/112), `node.h` (86), `result.h` (49), and a 36-line
bootstrap; the dll builds the four `.cpp` files (`CMakeLists.txt:3`) and compiles
`node.h` and `result.h` nowhere. `FEATURES.md:105` carries it "Experimental";
`docs/agent/purposes.md:81` describes it as "Data generator source."

- That purpose row is backwards: nothing in this lib derives from `pipeline::Source` -
  the source lib is its leaf-1 sibling `qor_generatorsource` - while `qor_generator` is
  the workflow engine that one wraps: `generatorsource.h:71` calls
  `m_generator.Generate()` from its own `Generate` override.
- The shape mirrors `qor_serialize`: `class qor_pp_module_interface(QOR_GENERATOR)
  Generator : public Workflow` (`generator.h:40`) pumping `CurrentState()->Enter()`
  while incomplete and the context has room (`generator.cpp:38-41`), with a `Context`
  holding the same `pointer/position/size` triple (`context.h:60-62`) and the same
  unsigned-subtraction `HasSpace` (`:55`) - but pointed out instead of in, and
  `PutOctet`'s write is position-guarded (`context.cpp:38-40`), so this twin does not
  inherit the serializer's off-by-one.
- The input side is a node queue: `std::deque<ref_of<Node>> m_nodes` (`generator.h:85`),
  `QueueNode` skipping nulls (`:58-64`) and `DequeueNode` returning null when empty
  (`:66-75`); `Node` is a `uint64_t` token plus `ToString` defaulting to
  `"<anonymous node>"` (`node.h:35-55`), wrapped by `NodeAdapter<T>` (`:57-82`).
- The state model inverts the serializer's: `GeneratorState` (`state.h:40`) carries a
  `Result m_result` and a `Leave` lambda mapping the code to `Fail`/`Emit` (`state.cpp:41-55`)
  over empty virtual `Prepare`/`Emit`/`Fail` hooks (`:78-89`), and exactly one concrete
  state exists, `ASCIIChar`, which puts one octet and pops on success (`state.cpp:91-109`).
- The compile token belongs to another lib: `target_compile_definitions` defines
  `QOR_PARSER` (`CMakeLists.txt:8`) while every mark asks for `QOR_GENERATOR`
  (`generator.h:40`, `context.h:35`, `state.h:40,59`) - inert only because the macro
  tests argument emptiness, not the token (`src/configuration/configuration.h:33`) - and
  the copy-paste shows in a fossil namespace comment, `}//qor::data::parser` closing
  `qor::components::generator` (`context.cpp:29,46`).
- The index and graph understate it the same way as its twin: the link line names seven
  libraries (`qor_module qor_host qor_thread qor_sync qor_injection qor_workflow
  qor_pipeline`, `CMakeLists.txt:13-14`) while `docs/agent/repo_map.md:100` and
  `docs/agent/deps.dot:121` list five - the two dropped are again `qor_workflow` and
  `qor_pipeline`, the pair the engine is built on.
- And the same reverse orphan: `generator.cpp` includes the error flyer (`:31`) and fires
  `serious("No initial state set...")` from both pump entry points (`:57,92`), but
  `qor_error` appears in no link line, index or graph for this target.
- The pump exists twice: `Generate` delegates to `InnerGenerate` and speaks through
  `log::debug` (`generator.cpp:36-85`), while `Run` re-implements the identical loop
  inline with `std::cout` chatter (`:87-128`); both reset `m_complete`, both swallow
  `Serious`/`std::exception`/anything to `cerr`, and both return the half-written
  `m_result`.
- Its consumers are paged and configured: `qor_generatorsource` links it
  (`generatorsource/CMakeLists.txt:13` = `deps.dot:117`), `qor_jsondata` links it
  (`src/components/data/formats/json/CMakeLists.txt:42`), two JSON test executables link
  it (`test/components/formats/json/CMakeLists.txt:17,24` plus the `generator` subdir's
  own exe at `:18`), and the configured `httpserver` example links it
  (`examples/advanced/httpserver/CMakeLists.txt:20` via `examples/CMakeLists.txt:3`).
- The JSON generator suite is its real exercise: `test_jsongenerator.cpp` calls
  `.Generate()` 32 times across ~630 lines - the densest in-tree use of the engine -
  while `Run` has no call site anywhere in the configured tree.
- The result vocabulary is forked across the twins: this lib wraps its enum in a class
  `Result::eResultCode` (`result.h:32-45`) where the serializer uses bare
  `ResultCode` (`deserializer/state.h:37-43`) - identical enumerator lists, no shared
  header - and the whole license family is the 24-line MIT block "2008 - 2025" with the
  bootstrap naming "Querysoft Open Runtime: Generator Module" (`qor_generator.cpp:33`).
  The judge returned exit=0 on a dll already compiled the same morning (18 Nov 2026):
  1,055,744 bytes stamped 08:33, newer than every folder file (newest, `result.h`,
  15 Oct 2026).

## Failure modes

- The pumps trust the context pointer: both loops dereference `m_context->HasSpace()`
  with no null check (`generator.cpp:38,100`) while the default ctor leaves `m_context`
  unset (`generator.h:45`) - the empty-stack guard protects against no initial state
  (`:55-58,90-93`) but a context-less Generator faults on first Enter.
- The twin pumps have already drifted: `Generate` logs stack and queue sizes
  (`generator.cpp:62-63,82-83`) and prints nothing to stdout, while `Run` never calls
  `InnerGenerate`, prints stack size four times to `cout` (`:97,106,110,126`), and its
  exit line even has the words reversed - "Stack has N states on exit." versus
  "Stack on exit has N states." (`:126` vs `:82`) - proof the two copies are evolving
  separately.

## See also

[[qor_generatorsource]] [[qor_serialize]] [[qor_jsondata]] [[qor_workflow]]
[[qor_pipeline]] [[qor_module]] [[qor_host]] [[qor_thread]] [[qor_sync]]
[[qor_injection]] [[qor_error]]

Not yet paged: `qor_netclient` and `qor_httpclient` (group 3 leaves 6-7).
