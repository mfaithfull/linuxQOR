# QOR Coding Conventions

Authoritative source: `docs/skills/QORconventions.md`. This file is a quick reference. Where the codebase is inconsistent, the authoritative document wins.

## Placement

See `memory-bank/systemPatterns.md` for the layering rules. In short: `src/configuration` first include, `src/macros` preprocessor library, `src/platform` OS/compiler/arch specifics, `src/qor` universal facilities, `src/framework` structural application elements, `src/components` optional modules. Every new folder gets `purpose.md`; volatile state gets `status.md`.

## Files and headers

- License header first in every file:
  ```cpp
  // Copyright Querysoft Limited 2008 - Present
  // SPDX-License-Identifier: BSL-1.0
  ```
  Older files may carry full BSL-1.0 text; third-party-derived code keeps original attribution after the QOR header.
- Include guards: `QOR_PP_H_` plus the source-root-relative path under `src/`, upper-cased with `_` separators. Closing `#endif` comment has no space, e.g. `#endif//QOR_PP_H_MEMORY`.
- Include order: `"src/configuration/configuration.h"` first in every `.cpp`, and before other QOR headers in `.h` files using `qor_pp_*`; then standard library headers; then other QOR headers by source-root-relative path; then local headers.
- Formatting: 4-space indent unless the file being edited uses tabs; control-flow braces on same line; function definitions put signature on one line and `{` on next; small accessors can be inline in headers; namespace closing comment names the full namespace.

## Naming

| Item | Convention | Examples |
|---|---|---|
| Namespaces | lower case, mirror folder path | `qor`, `qor::io::filesystem`, `qor::components::protocols::http` |
| Classes/structs | PascalCase | `UTF8String`, `MockRepository`, `SingletonInstanceHolder` |
| Interfaces | `I` prefix; implementations drop it | `IRole`/`Role`, `IFileSystem`/`FileSystem` |
| Methods | PascalCase, verb-first | `GetName()`, `SetRole()`, `AddSubsystem()` |
| Boolean queries | `Is*` / `Has*` | `IsEmpty()`, `HasWhen()` |
| Members | `m_` + PascalCase/camelCase | `m_Name`, `m_buffer` |
| File-static globals | `_` prefix, often anonymous namespace | `static qor::Library _qor_library(...)` |
| Global accessors | `The*` / `This*` | `ThePlatform()`, `ThisModule()` |

## Module interface

- Export APIs with `qor_pp_module_interface` or `qor_pp_module_interface_gcc(QOR_X)` where GCC/Clang-specific export handling is needed.
- Interface-only headers are implemented by components using import/export pairing.
- Consumers force a specific implementation with `qor_pp_module_requires(IName)`.
- A QOR lib target normally uses a `QOR_<MODULE>` PRIVATE compile definition and a file-static `Library` singleton.

## Memory management

- QOR-style code does not use raw `new`/`delete`, `unique_ptr`, or `shared_ptr`.
- Create objects with `new_ref<T>(args...)`; arrays use `new_array_ref(count, args...)`.
- Construction stages: `instancer_of<T>::type::Instance` -> `factory_of<T>::type::Construct` -> `source_of<T>::type` -> `allocator_of<T>::type`.
- Store references as members using declared ref traits such as `qor_pp_declare_ref_of`.

## Tests

- Add tests in the area mirroring `src/`; file names `test_<thing>.cpp`; camelCase test case names.
- Register new test files in the owning test CMakeLists. Do not use ctest: no `enable_testing()` or `add_test()`.
- Green test run = executable output `[OK](n/n)` plus exit 0, run from its own build directory.
- Every assertion must chain a proposition: `qor_pp_assert_that(expr).isTrue();`.
- Assert on observable API state, not internals or log output.
- Test-first discipline: research first, write one test first, see RED, then implement the smallest piece that makes it GREEN. Report honestly when RED was not observed.

## Working preferences

- Follow `docs/skills/QORconventions.md` exactly.
- Prefer `new_ref<T>()` over raw ownership primitives.
- Prefer standard-library facilities such as `std::to_chars` when they meet format requirements.
- Match local file style when editing legacy code.
