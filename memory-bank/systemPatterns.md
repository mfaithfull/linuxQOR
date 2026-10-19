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
  RFC 5234 parser framework, JSON parser, parser/serializer.
- **constexpr data structures** (`qor_datastructures`): compile-time map, set,
  string; rcu list (CopperSpice-derived).
- **Type erased functions** (`tef`), singleton/`SingletonInstanceHolder`,
  object context (Boost.Any-inspired), contract, factories, observers.

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
