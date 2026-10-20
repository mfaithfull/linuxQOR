# System Patterns

## Layered source tree (LAYOUT.md, docs/structure.md, src/*/purpose.md)
Dependency direction is strict: higher layers use lower layers; lower layers
never depend on higher ones. `src/qor` never depends on `src/framework` or
`src/components`.

| Layer | Contents | Rules |
|---|---|---|
| `src/configuration` | Root header included in every translation unit | `configuration.h` must be the **first include** in every `.cpp`/`.h` that uses QOR headers; `compiler.h` `#error`s otherwise |
| `src/macros` | Preprocessor library (Boost.Preprocessor port) | every macro is `qor_pp_*` |
| `src/platform` | `os/` (linux, windows), `compiler/`, `architecture/` | anything touching an OS API, calling convention, or arch feature lives here; implementation targets named per OS: `winqor_*`, `linqor_*` |
| `src/qor` | Universal facilities: essentials, memory, module, flyers, function, property, reflection, tdd | **no OS/arch/compiler specifics**; depends only on other qor modules + the standard library. "If you might use it in thousands of functions, it belongs here" |
| `src/framework` | Structural application elements: `app`, `data`, `event`, `io`, `parallel`, `ui` | general and usually singular — an application that cannot be that type of application without it has it here |
| `src/components` | Optional QOR modules built on the framework, mirroring where they'd sit as main modules | main QOR classes never depend on components; components may depend on any qor module and other components; experimental code in namespace `qor::exp::` |

**Rule of 7**: ~≤7 subfolders per folder, ≤7 files per folder, 7 classes per
namespace, 7 namespaces per namespace — not strictly adhered to where there is
no basis for subdivision. Every folder contains a `purpose.md`; frequently
changing development state goes in a separate `status.md`. **When adding a new
folder, add both.**

## Module system (docs/module system.md)
Self-aware registration chain built on static-construction singletons:
- **Library** — every QOR static library exposes a singleton `Library` object
  that registers itself with the containing Module on static construction.
- **Module** — every loadable library (and executable) exposes a singleton
  `Module` object that registers libraries, can iterate over them, and
  registers itself with the executable module on construction.
- **Host** — every top-level executable exposes a singleton `Host` object that
  registers loaded Modules, iterates them, and acts as the **type registry for
  dependency injection**.
- Every executable must link `qor_module` and provide a `ThisModule`
  implementation, so everything used gets initialized.
- `src/framework/host` is the library that makes a top-level executable a QOR
  executable.
- Convention: file-static registration objects in an anonymous namespace, e.g.
  `static qor::Library _qor_library(...)`, `static qor::TypeRegistry _theTypeRegistry;`,
  accessed by global functions `ThePlatform()`, `TheTypeRegistry()`, `ThisModule()`.

## Link-time dependency order (src/qor/dependency.md)
Primary qor modules form an explicit chain (each lists its dependencies):
```
qor_module
  → qor_host           : qor_module
  → qor_injection      : qor_module, qor_host
  → qor_reporting      : qor_module, qor_host
  → qor_sync           : qor_module, qor_host
  → qor_text           : qor_module, qor_host, qor_reporting
  → qor_current        : qor_module, qor_host, qor_injection
  → qor_interception   : qor_module, qor_host, qor_current
  → qor_issue          : qor_module, qor_host, qor_current
  → qor_property       : qor_module, qor_host, qor_reporting, qor_current
  → qor_error          : qor_module, qor_host, qor_reporting, qor_current,
                         qor_text, qor_interception, qor_issue
  → qor_thread         : qor_module, qor_host, qor_current, qor_sync, qor_injection
  → qor_signals        : qor_module, qor_host, qor_thread
  → qor_log            : qor_module, qor_host, qor_current, qor_interception,
                         qor_issue, qor_error, qor_signals
  → qor_threadmemory   : qor_module, qor_host, qor_thread, qor_sync
```
`test/CMakeLists.txt` is the canonical example of OS-specific link ordering
(`linqor_*` vs `winqor_*` target sets, plus `uring` on Linux).
**Note:** `qor_error` integration with text/string classes currently forces
*double linking under GCC* (see progress.md known issues).

## Key design patterns in use
- **Trait-based smart references** (`docs/smart_references.md`,
  `src/qor/memory/reference/newref.h`): construction decomposed into pluggable
  stages, each overridable per type via trait specialization:
  `new_ref<T>(args...)` → `instancer_of<T>::type::Instance(...)` (new? recycled?
  singleton?) → `factory_of<T>::type::Construct(...)` (internal vs external/DI
  factory) → `source_of<T>::type` (raw memory pool / arena / OS heap) →
  `allocator_of<T>::type` (layout, debug info, alignment). Result is a stack
  object of `ref_of<T>::type` needing no matching `delete`; arrays use
  `new_array_ref(count, args...)`. Traits are `<noun>_of<T>` with a nested
  `typedef ... type`. Policy-based, integrated memory management.
- **Module interface qualifiers** (defined in `configuration.h`):
  `qor_pp_module_interface(QOR_<MODULE>)` on constructors, destructors and every
  method defined in a `.cpp` of a module other than the declaring one;
  inline/template methods in headers usually don't need it.
  `qor_pp_module_interface_gcc(QOR_<MODULE>)` for interfaces exported **only**
  under GCC/Clang (e.g. `Application`, `AppBuilder`). Interface-only headers
  implemented by a component use explicit `qor_pp_export`/`qor_pp_import` pairs
  guarded by `#ifdef QOR_<MODULE>` (`ImplementsIFileSystem()`), and consumers
  force the linker to pull the implementation with
  `qor_pp_module_requires(IFileSystem);`.
- **Flyer pattern** (`docs/articles/flyers.md`, `src/qor/flyers`): thread-local
  facilities — base `qor_issue` flyer, `qor_error` (error raising/handling),
  `qor_log`, `qor_reporting`. Extensible error handling and logging.
- **Interfaces**: `I` prefix (`IRole`, `IWorkflow`, `IFeature`, `IFileSystem`,
  `ISubsystem`), implementations drop the `I` (`Role`, `Workflow`, `FileSystem`).
  GUID-identified subsystems/registries: `constexpr GUID` constants named
  `<Type>GUID` (`FileSystemGUID`, `BMPHeaderModelGUID`).
- **Pipelines** (`qor_pipeline`, `src/components/pipeline`): "Anything to
  Anything, Push or Pull, with Filters". Sources → filters → sinks:
  `qor_datasource`, `qor_datasink`, `qor_stringsource`, `qor_stringsink`,
  `qor_linereader`, `qor_randomsource`, `qor_fileconnector`,
  `qor_socketconnector`, `qor_stdfstreamconnector`, `qor_stdifstreamsource`,
  `qor_stdofstreamsink`, `qor_stdinsource`, `qor_stdoutsink`,
  `qor_base64{encode,decode}filter`, `qor_parsersink`, `qor_generatorsource`,
  `qor_aggregatesource`, `qor_deserializersink`. Backbone of serialization work.

- **Application framework** (`src/framework/app`): Role/Feature based
  `Application`/`AppBuilder`, `qor_role` (customization, startup/shutdown
  control), `qor_optionparser` (command line options by callback injection),
  `argparse`, `qor_workflow` (state machine based workflows).
- **Events** (`src/framework/event`): signals & slots (CopperSpice-derived, no
  custom preprocessor, multithreaded), delegates (SAKryukov), observers +
  chainlink; experimental **signal tree** (`framework/parallel/signaltree`).
- **Parallelism** (`src/framework/parallel`): `qor_thread` (threads + thread
  pool, BS thread_pool adapted), `qor_task` (async tasks adapted from cppcoro —
  auto/latch/manual-reset events, mutex, generator, sequencer, barrier,
  cancellation token, shared task, when_all), `qor_sync` (synchronization zoo),
  `workitem`.
- **Async IO** (`src/framework/io`): `qor_asyncioservice` (IOCP on Windows,
  io_uring on Linux), `qor_sockets` (sync + async), `qor_netclient` /
  `qor_netserver` (plug in a protocol), filesystem over `std::filesystem` plus
  `qor_filesystem` provider; protocols in `src/components/protocols`
  (echo, http, http_old, jip, uri).
- **Interception / AOP** (`qor_interception`): call interceptors, function
  contexts, call contexts, parameter validation, function-context locks.
  Reflection is the basis of the AOP approach (pfr-derived `qor_reflection`).
- **Custom allocators** (`qor_threadmemory`, `src/components/threadmemory`):
  FastHeap, fast bucket, small-object page/bucket/heap, stack page, thread heap.
- **Property / data model** (`src/qor/property`, `src/components/data`):
  variant property types, tables, MDM, formats, pipeline data, resources
  (experimental).
- **Text & Unicode** (`src/qor/essentials/text`, `src/components/data/formats`):
  Copy-On-Write strings, UTF-8/UTF-16/UCS4, code pages, string functions,
  RFC 5234 parser framework, JSON parser, parser/serializer. A second parser
  instance now lives outside `formats`: the generated **YAML parser prototype**
  in `src/components/protocols/yaml` (`qor_yaml`) — same two-layer shape
  (general `qor::data::parser` framework + per-format lib), flow-subset only.
- **constexpr data structures** (`qor_datastructures`): compile-time map, set,
  string; rcu list (CopperSpice-derived).
- **Type erased functions** (`tef`), singleton/`SingletonInstanceHolder`,
  object context (Boost.Any-inspired), contract, factories, observers.

## Parser ↔ generator symmetry (the pattern the JSON generator task follows)
Reading and writing are deliberately mirrored through the pipeline:

| Reading | Writing |
|---|---|
| `qor_parsersink` (`ParserSink` = pipeline **Sink** wrapping `Parser : Fastflow`) | `qor_generatorsource` (`GeneratorSource` = pipeline **Source** wrapping `Generator : Workflow`) |
| `ParserState : fastflow::Step`; `Prepare()` pushes a `NodeAdapter<T>`, `Emit()` fills the empty model | `GeneratorState : workflow::State`; `Prepare()` reads a populated model, `Emit()` writes octets via `Context::PutOctet()` |
| grammar composites `Sequence_t`/`Optional_t`/`ZeroOrMore_t`/`OneOrMore_t`/`AnyOneOf_t`/`Deferred_t` in `parser/states/_1`–`_4/` | literal output composites built from `ASCIIChar` (`src/components/io/generator/state.h`) in a matching `generator/states/` |
| `parser/tokens.h` token enum + `parser/nodes/` adapters + `model/` types | reuse the same token enum and `model/` types; `generator/nodes/` adapters carry already-populated model objects |
| flow mode `Push` (source pushes bytes into the sink) | flow mode `Pull` (sink pulls bytes from the source) |

Flows in use today: `FileConnector → UnicodeTextFilter → ParserSink` (JSON,
YAML, HTTP request parsing) and `GeneratorSource → … → FileConnector` (HTTP
request generation). `qor_generator` is now linked by `qor_jsondata` for the
JSON generator states; `qor_generatorsource` pipeline integration is still the
remaining next step.

JSON model shape (verified 19 Oct 2026 in `src/components/data/formats/json/model/`):
`Object` holds `std::vector<ref_of<Member>::type> m_members`; `Array` holds
`std::vector<ref_of<Value>::type> m_values`; `Member` stores `std::string m_name`
and `Value m_value` **by value** (`GetName()`, `GetValue() -> Value&`); `Value`
holds `polymorphic<ValueModel> m_valueModel` (`GetValue() -> polymorphic<ValueModel>&`,
`GetType() -> ValueType`); `String` holds a **decoded** `private std::string m_value`
(`GetValue()`, `SetValue()`, `Append(const char*, size_t)`); `Number` holds a
`private double m_value` (`GetValue()`, `SetValue()`). Only `m_members` and
`m_values` are public — everything else must be read through getters, and the
decoded `String` means escapes are not recoverable from the model.

JSON generator shape (updated 20 Oct 2026): model `Number` doubles are written
by `_number` through `std::to_chars`, which emits shortest round-trip decimal
text and therefore can include JSON-compatible exponent forms such as `1e+10`
or `1e-10`. Non-finite doubles are rejected before entering the number state.
Nested containers are supported by binding an inner `Object`, `Array`, `String`,
or `Number` model into a `Value` slot with
`polymorphic<qor::data::model::json::ValueModel>(inner)` and letting `_value`
dispatch again on `Value::GetType()`.

## Verified pipeline shapes (20 Oct 2026)

- **Read** (proven by JSON/YAML/HTTP parsing):
  `FileConnector(Source, buffer, …) → UnicodeTextFilter → ParserSink(buffer)`
  driven `.Connect().PumpAll()` in `Push` mode.
- **Write** (now proven by `QORTestJSONGenerator`, `[OK](36/36)`):
  `GeneratorSource<JSONState>(buffer, model) → FileConnector(Sink, file, buffer, OpenFor::WriteOnly, WithFlags::None)`
  also driven `.Connect().PumpAll()` in `Push` mode. `GeneratorSource` owns the
  output `qor::components::generator::Context` (the mirror of
  `ParserSink(buffer)` owning a `CodePointContext`), implements the pipeline
  `Source::ReadBytes` hook, and receives the model through a variadic
  `GeneratorSource(const pipeline::Buffer&, TArgs&&...)` ctor — the state stack
  survives across pulls, so output larger than the buffer still completes.
- **Platform subsystems are opt-in by the executable**: `ThePlatform(qor_shared)`
  starts with **no** subsystems, so anything that hooks a pipeline to a file must
  call `ThePlatform(qor_shared)->AddSubsystem<FileSystem>()` and declare
  `qor_pp_module_requires(IFileSystem)`. `qor_fileconnector` compiles and links
  without it and only fails its `FileSystem`/`IFile` module requirements **at
  run time**. Reference: `test/components/formats/json/generator/main.cpp`.

## Critical implementation paths
1. `configuration.h` as first include — build breaks otherwise.
2. `qor_module` + `ThisModule()` present in every executable and every module.
3. Library → Module → Host static-init registration chain (ordering matters;
   `qor_host` provides the DI type registry).
4. `cmake/flags.cmake` is included by every `CMakeLists.txt` — the single place
   compiler flags are defined.
5. Windows post-build step copies `$<TARGET_RUNTIME_DLLS:target>` next to the
   executable (`copy_if_different`); required for tests/examples to run.
6. Adding a module: copy an existing module's `CMakeLists.txt`, add the
   `QOR_<MODULE>` PRIVATE compile definition, export the API with
   `qor_pp_module_interface`, add the `Library` singleton, add `purpose.md`
   (5–10 minutes of work).
7. Interface/implementation split for OS-specific modules
   (`qor_filesystem` interface + `winqor_filesystem` / `linqor_filesystem`
   implementation, forced by `qor_pp_module_requires`).

## Naming & formatting conventions
Summarized in `memory-bank/codingConventions.md`; the authoritative version is
`docs/skills/QORconventions.md`. Consult both before writing code.

## JSON roundtrip defects and model ownership (verified 20 Oct 2026)

Empirical shape of the JSON read/write pair, established while getting the
`QORTestJSON` roundtrip green:

- **Model ownership is ref-counted, not borrowed.** `qor::components::generator::NodeAdapter<T>`
  holds a `ref_of<T>::type m_t` and `GetObject()` returns it **by value**, so
  `push_back((*it)->GetObject())` copies an owning reference and the model stays
  alive. Suspecting dangling model data in the generator wastes time; check state
  assembly/population first.
- **String text accumulates live during parsing.** `character::Emit` appends each
  decoded character **directly into the StringNode's model** while that node sits
  on the parser node stack, so the collection loop in `string::Emit` normally
  leaves its own `stringValue` empty. Any unconditional `SetValue(stringValue)`
  there wipes the accumulated text; it must be guarded with
  `if(!stringValue.empty())` (`parser/states/_4/string.cpp`).
- **Empty containers need their own branch in the generator.** The model-driven
  `_object(generator, object)` state advances one position per member
  (value + separator) and emits the closing brace at
  `m_position == (m_count * 2)`; with `m_count == 0` that test is
  `1 == 0` and never fires, so `{}` was emitted as `{`. Fixed with an explicit
  `m_count == 0` branch in `generator/states/_object.cpp`. The dedicated
  empty-object (`nullptr`) ctor path is separate and unchanged.
- Leaf scalars held directly in a `Value` (null, booleans, numbers) render
  correctly even when container/string content does not, so "well-formed scalars
  plus empty/malformed containers" points at these state-population defects
  rather than at an ownership problem.

## Pipeline filter module shape (qor_mime, 20 Oct 2026)

A pipeline element is its own SHARED lib. The filter contract (from
`src/framework/data/pipeline/filter.h` + `filters/base64encodefilter` + `filters/linereader`):

```
class qor_pp_module_interface_gcc(QOR_MIME) MIMERecognizerFilter : public qor::pipeline::Filter
{   ctor / dtor / operator=  — = default style (linereader) or explicit base-init (base64encodefilter)
    virtual bool ReadFilter (size_t& unitsProcessed, size_t unitsToProcess);   //push mode
    virtual bool WriteFilter(size_t& unitsProcessed, size_t unitsToProcess);   //pull mode
};
```
`qor::pipeline::Filter : Sink, Source` — ctor initialises `Sink(source), Source(sink)` (order
matters: the sink is built FROM the source argument). Returning false from a filter hook
stops the pump. A filter that only inspects must NOT ack/consume source data (the sink would
lose bytes).

Module registration: parent `CMakeLists.txt` `add_subdirectory(mime)`; module
`set(target qor_mime)` + `add_library(${target} SHARED qor_mime.cpp mime.cpp)` +
`target_compile_definitions(${target} PRIVATE QOR_MIME ...)` + `qor::Module& ThisModule()` singleton.
Windows needs at least one exported symbol for the import lib — `qor_pp_export void init_mime(void)`.

Mirror test target: `test/framework/data/mime` → `QORTestMIME` exe; registered in `test/CMakeLists.txt`;
links the module lib plus the pipeline libs it needs (`qor_pipeline`, and
`qor_stringsource`/`qor_stringsink` for pipeline pumping tests); POST_BUILD copies
`$<TARGET_RUNTIME_DLLS:${target}>` next to the exe; `enable_testing()` + `add_test(TestMIME ${target})`.
Run from the build folder (the local ctest shim does not list VS-generator tests — see
`memory-bank/qor_mime_plan.md` pitfalls).

