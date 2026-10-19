# QOR Coding Conventions (condensed)

**Authoritative source:** `docs/skills/QORconventions.md` — read it before
writing or reviewing QOR code. This file is the quick-reference summary
extracted from it plus the codebase. Where the codebase is inconsistent, the
dominant pattern in the authoritative doc wins.

## Placement (rule of 7, purpose.md, status.md)
Code placement decisions matter more than style details.
- `src/configuration` — root header, first include in every TU.
- `src/macros` — preprocessor library; macros `qor_pp_*`.
- `src/platform` — OS/compiler/architecture specifics only (`win*`/`lin*`).
- `src/qor` — universal, no OS/arch/compiler specifics; depends only on qor
  modules + std lib. "If you might use it in thousands of functions, it belongs
  here."
- `src/framework` — general, usually singular structural elements (Roles,
  Workflows, Pipelines, Signals, Threads).
- `src/components` — optional modules; never depended on by main QOR classes;
  hierarchy mirrors where they'd sit as main modules; experimental code in
  `qor::exp::`.
- **Every new folder needs a `purpose.md`; volatile development state goes in
  `status.md`.**

## Files & headers
- **License header first in every file** (mandatory):
  ```cpp
  // Copyright Querysoft Limited 2008 - Present
  // SPDX-License-Identifier: BSL-1.0
  ```
  Older files (and the `src/macros` port) carry full BSL-1.0 text.
  Third-party-derived code keeps its original attribution comment **after** the
  QOR header (CsSignal, assertcc, SAKryukov delegates).
- **Include guard**: `QOR_PP_H_` + file path under `src/` upper-cased with `_`
  separators. Closing `#endif` has a trailing comment with no space:
  `#endif//QOR_PP_H_MEMORY`.
  Examples: `QOR_PP_H_PROPERTY_PROPERTYVALUE`,
  `QOR_PP_H_COMPONENTS_DATA_MDM_TABLE`, `QOR_PP_H_SIGNALS_SIGNAL`.
- **Include order**: (1) `"src/configuration/configuration.h"` — first in every
  `.cpp`, and required before any other QOR header in `.h` files using
  `qor_pp_*` (otherwise `compiler.h` `#error`s); (2) standard library headers;
  (3) other QOR headers by **source-root-relative path**
  (`"src/qor/memory/reference/reference.h"`); (4) local (same-folder) headers
  last.
- **Formatting**: 4-space indent (some legacy files use tabs — match the file
  being edited); control-flow braces on the same line (`if(x)` then `{`);
  function definitions put return type + name on one line and `{` on the next;
  small accessors inline on one line in headers; namespace closing comment
  names the full namespace (`}}//qor::workflow`,
  `}}}//qor::assert::subject`).

## Naming
| Item | Convention | Examples |
|---|---|---|
| Namespaces | lower case, mirror folder path under `src/` | `qor`, `qor::platform`, `qor::app`, `qor::io::filesystem`, `qor::components::protocols::http` |
| Classes / structs | PascalCase | `UTF8String`, `MockRepository`, `SingletonInstanceHolder` |
| Interfaces | `I` prefix; implementations drop it | `IRole`/`Role`, `IWorkflow`/`Workflow`, `IFileSystem`/`FileSystem`, `IFeature`, `ISubsystem` |
| Methods | PascalCase, verb-first | `GetName()`, `SetRole()`, `AddSubsystem()`, `GetFeature<T>()`, `PumpAll()` |
| Boolean queries | `Is*` / `Has*` | `IsEmpty()`, `IsNull()`, `HasWhen()`, `IsComplete()` |
| Members | `m_` + PascalCase (camelCase in newer code) | `m_Name`, `m_RefCount`, `m_buffer`, `m_cachedLength` |
| File-static globals | `_` prefix, often anonymous namespace | `static qor::Library _qor_library(...)`, `static qor::TypeRegistry _theTypeRegistry;` |
| Global accessors | `The*` / `This*` | `ThePlatform()`, `TheTypeRegistry()`, `ThisModule()` |
| Macros | all lower case `qor_pp_`, params `_CAPS` | `qor_pp_test_case(_CASE)`, `qor_pp_declare_ref_of(_CLASS,_REF)` |
| Type traits | `<noun>_of<T>` with nested `typedef ... type` | `ref_of`, `guid_of`, `factory_of`, `instancer_of`, `allocator_of`, `source_of`, `sync_of` |
| Test files | `test_<thing>.cpp` in `/test/<area>/` mirroring src | `test_utf8string.cpp`, `test_singleton.cpp` |
| Test cases | camelCase sentence | `canInstanceUTF8String`, `outOfRangeMutableBufferAccessThrows` |
| CMake targets | `qor_<module>`; platform impls `winqor_*`/`linqor_*` | `qor_error`, `qor_property`, `winqor_sockets`, `linqor_filesystem`, `qor_jsondata` |
| Module interface defines | `QOR_<MODULE>` upper snake, PRIVATE compile definition | `QOR_PROPERTY`, `QOR_ERROR`, `QOR_FILESYSTEM` |
| GUID constants | `<Type>GUID`, `constexpr GUID` | `FileSystemGUID`, `BMPHeaderModelGUID` |

## Module-interface pattern
Exported classes use `qor_pp_module_interface(QOR_<MODULE>)`:
```cpp
class qor_pp_module_interface(QOR_PLATFORM) Platform final
{
public:
    qor_pp_module_interface(QOR_PLATFORM) Platform();
    qor_pp_module_interface(QOR_PLATFORM) ~Platform();
    qor_pp_module_interface(QOR_PLATFORM) void Setup();
};
```
- Apply to ctors, dtors and every method defined in a `.cpp` belonging to
  **another** module; pure inline/template methods in headers usually don't
  need it.
- `qor_pp_module_interface_gcc(QOR_X)` when the interface must be exported
  **only** under GCC/Clang (MSVC handles `/EXPORT` differently) — e.g.
  `Application`, `AppBuilder`.
- Interface-only headers implemented by a component:
  ```cpp
  #ifdef QOR_FILESYSTEM
      bool qor_pp_import ImplementsIFileSystem();
  #else
      bool qor_pp_export ImplementsIFileSystem();   // implementation exports
  #endif
  ```
- Consumers force a specific implementation into the linker with
  `qor_pp_module_requires(IFileSystem);`.

## Memory management
- **No raw `new`/`delete`, no `unique_ptr`/`shared_ptr`** in QOR-style code.
  Create objects with `new_ref<T>(args...)`
  (`src/qor/memory/reference/newref.h`); the result is `ref_of<T>::type` on the
  stack, no matching delete. Arrays: `new_array_ref(count, args...)`.
- Construction stages, each overridable per type by trait specialization:
  `instancer_of<T>::type::Instance` → `factory_of<T>::type::Construct` →
  `source_of<T>::type` → `allocator_of<T>::type`.
- Store references as members using declared ref traits
  (`qor_pp_declare_ref_of`).

## Tests
- Add tests in the area mirroring `src/`; file name `test_<thing>.cpp`;
  camelCase test case names.
- Register the new file in the owning `test/CMakeLists.txt` — the main `QORTest`
  source list, or the sub-directory's own CMakeLists when its own executable
  exists.
- `qor_assert` (fluent assertions), `qor_test` (Yaffut-derived), `qor_mock`
  (HippoMocks-derived) are used together.
- Verify order: build `examples/basic/helloworld`, then `QORTest`.

## Working preferences
- Follow `docs/skills/QORconventions.md` exactly (license header,
  `configuration.h` first include, `QOR_PP_H_*` guards,
  `qor_pp_module_interface` qualifiers).
- New folder ⇒ add `purpose.md` (and `status.md` for volatile state).
- Keep the "rule of 7" in mind when creating folders/namespaces.
- Prefer `new_ref<T>()` smart references over raw `new`/`delete`/`unique_ptr`.
- Match the conventions of the file being edited (legacy files use tabs).
