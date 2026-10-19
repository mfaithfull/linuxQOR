\# QOR C++ Coding Conventions



Conventions extracted from analysis of the existing C++20 code in `src/`, plus the design

documents in `docs/` and `LAYOUT.md`. Use this as the reference when writing or reviewing

QOR code. Where the codebase itself is inconsistent, the dominant pattern is documented here.



\---



\## 1. Layering — where code belongs



The source tree is strictly layered (see `LAYOUT.md`). Placement decisions matter more than

most style details:



| Folder | Contents | Rules |

|---|---|---|

| `src/configuration/` | Root header included by every translation unit | `configuration.h` must be the \*\*first include\*\* in every .cpp/.h that uses QOR headers |

| `src/macros/` | Preprocessor library (port of Boost.Preprocessor) | All macros take the form `qor\_pp\_...` |

| `src/platform/` | OS / compiler / architecture specifics | Anything touching an API, calling convention or arch feature lives here. Sub-implementations are named by OS: `lin\*`, `win\*` |

| `src/qor/` | Universal facilities (memory, module, flyers, essentials, tdd...) | \*\*Must have no OS, architecture or compiler specifics.\*\* Depends only on other qor modules + the standard library. "If you might use it in thousands of functions, it belongs here" |

| `src/framework/` | Structural application elements (app, data, event, io, parallel, ui) | General and usually singular: Roles, Workflows, Pipelines, Signals, Threads... |

| `src/components/` | Optional modules built on the framework | Main QOR classes never depend on components. Arranged to mirror where they'd sit if they were main modules (`components/data`, `components/io`, `components/ui`, ...). Experimental code goes in namespace `qor::exp::` until it graduates |



Every folder contains a `purpose.md` explaining its contents; frequently-changing

development state goes in a separate `status.md`. \*\*When adding a new folder, add both.\*\*



The project follows the "rule of 7" (roughly ≤7 subfolders per folder, ≤7 files per folder)

where there is a rational basis for subdivision.



\---



\## 2. File and header conventions



\### License header — mandatory, first thing in every file



```cpp

// Copyright Querysoft Limited 2008 - Present

// SPDX-License-Identifier: BSL-1.0

```



Older files (and the `src/macros/` port) carry the full BSL-1.0 text instead of the SPDX line.

New files use the two-line form above. Third-party-derived code keeps its original attribution

comment \*after\* the QOR header (e.g. CsSignal in `signals/signal.h`, assertcc in

`tdd/assert/`, SAKryukov delegates).



\### Include guards



Form: `QOR\_PP\_H\_` + the file's path under `src/` with separators as `\_`, all upper case:



```cpp

\#ifndef QOR\_PP\_H\_MEMORY            // src/qor/memory/memory.h

\#define QOR\_PP\_H\_MEMORY

...

\#endif//QOR\_PP\_H\_MEMORY            // closing #endif gets a comment, no space after #endif

```



Examples seen in the tree: `QOR\_PP\_H\_PROPERTY\_PROPERTYVALUE`,

`QOR\_PP\_H\_COMPONENTS\_DATA\_MDM\_TABLE`, `QOR\_PP\_H\_SIGNALS\_SIGNAL`.



\### Include order (observed dominant pattern)



1\. `"src/configuration/configuration.h"` — first, in every .cpp (and required before any

&#x20;  other QOR header in .h files that use `qor\_pp\_\*` macros; `compiler.h` will `#error` otherwise)

2\. Standard library headers (`<vector>`, `<string>`, `<cstdint>`...)

3\. Other QOR headers, using \*\*source-root-relative paths\*\*: `"src/qor/memory/reference/reference.h"`

4\. Local (same-folder) headers last: `"propertyvaluebase.h"`, `"library.h"`



\### Formatting



\- 4-space indentation (some legacy files use tabs — match the file you are editing).

\- Control-flow braces on the same line: `if(x)\\n{` ... observed throughout.

\- Function definitions: return type and name on one line, `{` starts the next line:

&#x20; ```cpp

&#x20; Severity ErrorBase::GetSeverity() const 

&#x20; { 

&#x20;     return m\_what.IsNotNull() ? m\_what->GetSeverity() : Severity::Fatal\_Error; 

&#x20; }

&#x20; ```

\- Small accessors are written inline in headers on a single line:

&#x20; `inline const char\* Name() const { return m\_Name; }`

\- Namespace closing comment names the full namespace, nested closings stacked:

&#x20; `}}//qor::workflow`, `}}}//qor::assert::subject`, `}}}}//qor::components::data::mdm`.



\---



\## 3. Naming conventions



| Item | Convention | Examples from the tree |

|---|---|---|

| Namespaces | lower case, mirror folder path under `src/` | `qor`, `qor::platform`, `qor::app`, `qor::io::filesystem`, `qor::components::protocols::http` |

| Classes / structs | PascalCase | `UTF8String`, `MockRepository`, `SingletonInstanceHolder` |

| Interfaces | `I` prefix, implementations drop it | `IRole`/`Role`, `IWorkflow`/`Workflow`, `IFileSystem`/`FileSystem`, `IFeature`, `ISubsystem` |

| Methods / functions | PascalCase, verb-first | `GetName()`, `SetRole()`, `AddSubsystem()`, `GetFeature<T>()`, `PumpAll()` |

| Boolean queries | `Is\*` / `Has\*` prefix | `IsEmpty()`, `IsNull()`, `HasWhen()`, `Exists()`, `IsComplete()` |

| Member variables | `m\_` + PascalCase (or camelCase in newer code) | `m\_Name`, `m\_RefCount`, `m\_buffer`, `m\_cachedLength` |

| Free/global objects | `\_` prefix, file-static | `static qor::Library \_qor\_library(...)`, `static qor::TypeRegistry \_theTypeRegistry;` (often inside an anonymous namespace) |

| Global accessor functions | `The\*` / `This\*` | `ThePlatform()`, `TheTypeRegistry()`, `ThisModule()` |

| Macros | all lower case, `qor\_pp\_` prefix; parameters `\_CAPS` | `qor\_pp\_test\_case(\_CASE)`, `qor\_pp\_declare\_ref\_of(\_CLASS,\_REF)` |

| Type traits | `<noun>\_of<T>` template structs with a nested `typedef ... type` | `ref\_of<T>`, `guid\_of<T>`, `factory\_of<T>`, `instancer\_of<T>`, `allocator\_of<T>`, `source\_of<T>`, `sync\_of<T>` |

| Test files | `test\_<thing>.cpp` in `/test/<area>/` mirroring the src area | `test\_utf8string.cpp`, `test\_singleton.cpp`, `test\_mock.cpp` |

| Test cases | camelCase sentence, usually starting with `can\*` or stating the expected behaviour | `canInstanceUTF8String`, `outOfRangeMutableBufferAccessThrows`, `copyConstructorCopiesMutableBuffer` |

| CMake targets | main modules: `qor\_<module>`; platform impls: `<os>qor\_\*` / `linqor\_\*`; components keep descriptive names | `qor\_error`, `qor\_property`, `winqor\_sockets`, `linqor\_filesystem`, `qor\_jsondata` |

| Module interface defines | `QOR\_<MODULE>` upper snake, passed as PRIVATE compile definition | `QOR\_PROPERTY`, `QOR\_ERROR`, `QOR\_FILESYSTEM`, `QOR\_WINTERMSCREEN` |

| GUID constants | `<Type>GUID`, `constexpr GUID` | `FileSystemGUID`, `BMPHeaderModelGUID` |



\---



\## 4. Namespace and module-interface pattern



Classes that are exported from a shared library carry the module-interface qualifier, which

resolves to `qor\_pp\_export` in the defining module's translation units and `qor\_pp\_import`

everywhere else (see `configuration.h`):



```cpp

class qor\_pp\_module\_interface(QOR\_PLATFORM) Platform final

{

public:

&#x20;   qor\_pp\_module\_interface(QOR\_PLATFORM) Platform();     

&#x20;   qor\_pp\_module\_interface(QOR\_PLATFORM) \~Platform();

&#x20;   qor\_pp\_module\_interface(QOR\_PLATFORM) void Setup();

&#x20;   ...

};

```



Rules observed:



\- Apply `qor\_pp\_module\_interface(<QOR\_X>)` to constructors, destructors and every method that

&#x20; is defined in a .cpp of another module than the one declaring it. Pure inline / template

&#x20; methods in headers usually don't need it.

\- Use `qor\_pp\_module\_interface\_gcc(<QOR\_X>)` for classes whose interface must \*only\* be

&#x20; exported under GCC/Clang (MSVC uses `/EXPORT` differently) — e.g. `Application`, `AppBuilder`.

\- Interface-only headers that a \*component\* implements use the explicit import/export pair:

&#x20; ```cpp

&#x20; #ifdef QOR\_FILESYSTEM    

&#x20;     bool qor\_pp\_import ImplementsIFileSystem();

&#x20; #else

&#x20;     bool qor\_pp\_export ImplementsIFileSystem();   //implementation library exports this

&#x20; #endif

&#x20; ```

\- Consumers force the linker to pull in a specific implementation with:

&#x20; `qor\_pp\_module\_requires(IFileSystem);` (declares `bool requiresIFileSystem()` which calls

&#x20; the exported `ImplementsIFileSystem()`).



\---



\## 5. Memory management — smart references, never raw new/delete



The QOR replaces `new`/`delete`, `unique\_ptr` and `shared\_ptr` with \*\*trait-based smart

references\*\* (see `docs/smart\_references.md`). Construction is decomposed into pluggable

stages, each overridable per type via a trait specialisation:



```

new\_ref<T>(args...) 

&#x20; → instancer\_of<T>::type::Instance(...)     // new? recycled? singleton?

&#x20;   → factory\_of<T>::type::Construct(...)    // internal vs external (DI) factory

&#x20;     → source\_of<T>::type                   // raw memory pool / arena / OS heap

&#x20;       → allocator\_of<T>::type              // layout, debug info, alignment

```



Conventions:



\- \*\*Create objects with `new\_ref<T>(args...)`\*\* (`src/qor/memory/reference/newref.h`).

&#x20; The result is a stack object of type `ref\_of<T>::type`; it needs no matching delete.

&#x20; Arrays use `new\_array\_ref(count, args...)`.

\- Store references as members: `ref

