# System Patterns

## Layered source tree

Dependency direction is strict: higher layers use lower layers; lower layers never depend on higher ones. `src/qor` never depends on `src/framework` or `src/components`.

| Layer | Contents | Rule |
|---|---|---|
| `src/configuration` | root header | `configuration.h` must be first include in QOR TUs |
| `src/macros` | preprocessor library | every macro is `qor_pp_*` |
| `src/platform` | OS, compiler, architecture | OS APIs, calling conventions, and arch features live here; targets are `winqor_*`, `linqor_*` |
| `src/qor` | universal facilities | no OS/arch/compiler specifics; depends only on qor modules and standard library |
| `src/framework` | structural application elements | general and usually singular elements needed for a kind of application |
| `src/components` | optional modules built on framework | main QOR classes never depend on components; experimental code is `qor::exp::` |

Rule of 7: about 7 subfolders/files/classes/namespaces per container. Every new folder needs `purpose.md`; volatile development state needs `status.md`.

## Module system

Static-construction singleton registration chain:
- `Library` registers itself with its containing `Module`.
- `Module` registers libraries and registers itself with the executable `Host`.
- `Host` registers loaded modules and acts as the DI type registry.
- Every executable links `qor_module` and provides a `ThisModule` implementation.
- `src/framework/host` makes a top-level executable a QOR executable.
- File-static registration objects are usually anonymous-namespace statics; global accessors are `The*` / `This*`.

See `src/qor/dependency.md` for the explicit primary qor dependency chain.

## Critical implementation paths

1. `configuration.h` as first include - build breaks otherwise.
2. `qor_module` plus `ThisModule()` in every executable and module.
3. Library -> Module -> Host static-init ordering matters; `qor_host` provides the DI type registry.
4. `cmake/flags.cmake` is included by every `CMakeLists.txt`; it is the single compiler-flag place.
5. Windows post-build copies runtime DLL dependencies next to executables; required for tests/examples.
6. Adding a module: copy an existing module CMakeLists, add its `QOR_<MODULE>` compile definition, export API with `qor_pp_module_interface`, add `Library` singleton, add `purpose.md`.
7. OS-specific modules split interface and implementation (`qor_filesystem` interface plus `winqor_filesystem` / `linqor_filesystem` implementation, forced by `qor_pp_module_requires`).

## Pipeline filter pattern

A pipeline element is usually its own shared library. A filter derives from `qor::pipeline::Filter`, which is both `Sink` and `Source`; constructor initialization order matters: `Sink(source), Source(sink)`.

- Filter hooks: `ReadFilter(size_t& unitsProcessed, size_t unitsToProcess)` for pull, `WriteFilter(size_t& unitsProcessed, size_t unitsToProcess)` for push. Returning false stops the pump.
- Working pipeline mode is Push. Set `pipeline.SetFlowMode(Push)` and ensure every element has a parent pipeline via `SetParent`.
- Pull mode is not a reliable base-framework path for filters; the base zero-unit path breaks.
- A filter that only inspects must not consume source data. The buffer protocol is `ReadRequest(n)` reserve, `ReadAcknowledge(n)` commit, `ReadReject(n)` release reservation.
- A recognising filter classifies pre-encoded octets before downstream transforms.
- The module contract is the registered type name; choosing a downstream sink from that name is application glue, not module responsibility.
- `Reset()` must clear recognition state and re-arm the filter.
- Internal pass-through helpers stay file-local in the module `.cpp` unless they must cross the module boundary.

## Parser/generator symmetry

- Reading: `ParserSink` wraps `Parser : Fastflow`; parser states are `fastflow::Step`; `Prepare()` pushes a `NodeAdapter<T>`, `Emit()` fills an empty model.
- Writing: `GeneratorSource` wraps `Generator : Workflow`; generator states are `workflow::State`; `Prepare()` reads a populated model, `Emit()` writes octets through `Context::PutOctet()`.
- Parser and generator share token enums, node adapters, and model types. Generator node adapters carry already-populated model objects.
- JSON model ownership is ref-counted, not borrowed. `NodeAdapter<T>` holds a `ref_of<T>::type` and returns owning objects by value.
- JSON model shape: `Object` has a public member vector; `Array` has a public value vector; most other state is read through getters. `String` stores decoded text, so original escapes are not recoverable from the model.
- Empty containers need explicit generator state branches. The JSON object state advances per member and closes at `position == count * 2`, so `count == 0` needs its own branch.

## Windows api_layer: Win32 entry-point binding

Each `src/platform/os/windows/api_layer/<lib>` module wraps one system DLL with a stateless idiom:
1. `qor_pp_useswinapi(<dllbase>, Name)` binds `Name` at call time from the already loaded image using `GetModuleHandle` + `GetProcAddress`; no CMake link against the system DLL is needed, and the built library should not import it.
2. `Library::Call<RET, Args...>(pFunc, args...)` applies the exception filter around the call and passes SDK argument types verbatim. `Library::voidCall` is the void form.
3. `Win32ErrorHandler _;` runs the exception filter at call sites. `CheckReturn<HRESULT>::TType` / `CheckReturn<BOOL>::TType` raise a `Win32Error` only where zero/false means failure; handle/count/pointer families return raw.

Header-inline variant (D11): if the SDK header defines a `FORCEINLINE` API shim rather than a DLL export, use `qor_pp_usesinlineapi(Name)` plus `Library::inlineCall` / `Library::voidInlineCall`. Call with explicit template arguments and `::` global SDK names, e.g. `Library::voidInlineCall< ::PTP_CALLBACK_ENVIRON >( ::InitializeThreadpoolEnvironment, pcbe )`. Do not bind header-inline entry points through `qor_pp_useswinapi` / `GetProcAddress`.

Audit consequence: an api_layer library that imports its intended system DLL means binding is wrong. A wrapper/header/implementation/export mismatch means either the wrapper set or the audit is wrong; compare both directions.
