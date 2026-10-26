# Skill: Creating a QOR Parser Module

Learned by analysis of the `qor_jsondata` module at
`src/components/data/formats/json`. This is a **recursive-descent / state-machine
parser** built on the general `qor::data::parser` framework
(`src/components/data/parser`). The framework turns ABNF grammar productions
directly into a few lines of C++ each.

## 0. The two layers

1. **General framework** — `src/components/data/parser` (header-only, no CMakeLists.txt
   per D6). Provides `Parser`, `ParserState`, `Node`, `NodeAdapter`, `Context`,
   `Result`, `tokens.h`, and the **combinator states** in `states/`:
   `Sequence`, `Sequence_t`, `Optional`, `ZeroOrMore`, `OneOrMore`, `AnyOneOf`,
   `AnyOneOfSet`, `OneOfARange`, `Specific`, `Deferred`, `rfc5234.h`.
   Purpose (from `src/components/data/parser/purpose.md`): "Provides a general
   parser state machine... every grammar production gets its own class, closely
   matching the ABNF."

2. **Concrete parser module** — one `set(target qor_Xdata)` library per format
   (e.g. `qor_jsondata`). It defines the format's **tokens**, **model** (the
   in-memory data structure), **nodes** (NodeAdapter wrappers over model types),
   and **states** (one class per grammar production).

## 1. Module skeleton (mirror json)

Directory layout for a new format `foo`:

```
src/components/data/formats/foo/
  CMakeLists.txt          # set(target qor_foo) + add_library SHARED ...
  qor_foo.cpp             # module registration (ThisModule)
  object.h                # (optional) top-level convenience header
  parser/
    tokens.h              # enum class fooToken : uint64_t + name map
    jsonparser.h          # umbrella include of every state header
    helpers.h             # StandardPrepare/StandardFail/TrivialEmit/SimpleEmit
    states/
      _1/ ... _4/         # one .h/.cpp per production, grouped by nesting depth
    nodes/                # one NodeAdapter<model::...> header per production
  model/                  # the in-memory value types (Array, Object, Value, ...)
```

### CMakeLists.txt (from json)
```cmake
set(target qor_jsondata)
add_library(${target} SHARED qor_json.cpp
  parser/states/_1/array.cpp
  ... one .cpp per non-header-only production ...)
include("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")
target_compile_definitions(${target} PRIVATE QOR_JSON
  qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
  qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
  qor_pp_ver_patch=${PROJECT_VERSION_PATCH})
target_link_libraries(${target} qor_module qor_host qor_current qor_sync qor_injection
  qor_workflow qor_pipeline qor_parser qor_issue qor_error qor_log)
```
Key link deps: **`qor_parser`** (the framework), `qor_pipeline` (buffer context),
`qor_workflow` (fastflow), plus the usual core. Register the dir with
`add_subdirectory(foo)` in `src/components/data/formats/CMakeLists.txt`.

### qor_foo.cpp (module registration — copy verbatim, rename)
```cpp
#include "src/configuration/configuration.h"
#ifdef NDEBUG
#include <buildnumber.h>
#endif
#include "src/qor/module/module.h"
qor::Module& ThisModule(void) {
    static qor::Module QORModule("Querysoft Open Runtime: FOO Module", qor_pp_module_ver_string);
    return QORModule;
}
```

## 2. The core model: ParserState + combinators

`ParserState` (`src/components/data/parser/state.h`) derives from `fastflow::Step`
and has virtual `Prepare()`, `Emit()`, `Fail()` plus a `Result m_result`
(`SUCCESS` / `FAILURE` / `MORE_DATA`). The `Parser` class drives a `Fastflow`
workflow, keeps a **node stack** (`PushNode`/`PopNode`/`TopNode`) and a
`stateCache` keyed by token.

**Combinators** (`src/components/data/parser/states/`) are the reusable building
blocks. The ABNF→C++ translation rules (from `purpose.md`):

| ABNF | C++ combinator |
|------|----------------|
| `A B` (concatenation) | `Sequence(parser, A, B, token)` |
| `[ X ]` (optional) | `Optional(parser, X)` |
| `*( X )` (zero or more) | `ZeroOrMore(parser, X)` |
| `+( X )` (one or more) | `OneOrMore(parser, X)` |
| exact literal byte/codepoint | `Specific(parser, (uint32_t)0x7B)` |
| range / set | `OneOfARange`, `AnyOneOf`, `AnyOneOfSet` |

`Sequence` (runtime, heap refs) vs `Sequence_t` (templated, stack members) —
`Sequence_t` is used for leaf productions like `begin_array`/`begin_object`.

**Only `Specific`/`Range` matches advance the input (lexing).** Everything else
combines already-read symbols into higher constructs (parsing). This is the
lex/parse split.

## 3. Writing a production class

### Composite (non-terminal) — e.g. `array`
Header declares the class deriving from `Sequence`; the .cpp builds the tree of
sub-states in the ctor and implements `Prepare/Emit/Fail`.

```cpp
// array = begin-array [ value *( value-separator value ) ] end-array
class qor_pp_module_interface(QOR_JSON) array final : public Sequence {
public:
    array(Parser* parser);
    virtual ~array() noexcept;
    virtual void Prepare(); virtual void Emit(); virtual void Fail();
private:
    Parser* m_parser;
};
```
```cpp
array::array(Parser* parser) :
    Sequence(parser,
        new_ref<begin_array>(parser),
        new_ref<Sequence>(parser,
            new_ref<Optional>(parser,
                new_ref<Sequence>(parser,
                    new_ref<value>(parser),
                    new_ref<ZeroOrMore>(parser,
                        new_ref<Sequence>(parser,
                            new_ref<value_separator>(parser),
                            new_ref<value>(parser))))),
            new_ref<end_array>(parser)),
        static_cast<uint64_t>(jsonToken::array)),
    m_parser(parser) {}
```
- `Prepare()` pushes a fresh model node (`new_ref<ArrayNode>().AsRef<Node>()`).
- `Emit()` pops child nodes off the stack until it finds its own token, collects
  them into the model, then pushes the node back.
- `Fail()` pops its own node if it's on top.

### Terminal / leaf — e.g. `begin_array`
Built from `Sequence_t` over `ws`, `Specific`, `ws`; emits nothing.
```cpp
// begin-array = ws %x5B ws  ; [
class begin_array final : public Sequence_t< ws, Sequence_t< Specific<uint32_t>, ws > > {
public:
    begin_array(Parser* parser) :
        Sequence_t<ws, Sequence_t<Specific<uint32_t>, ws>>(parser, &m_leadingws, &m_tail,
            static_cast<uint64_t>(jsonToken::begin_array)),
        m_leadingws(parser), m_trailingws(parser),
        m_bracket(parser, 0x5B), m_tail(parser, &m_bracket, &m_trailingws) {}
    virtual ~begin_array() noexcept = default;
    virtual void Emit(){};   // emit nothing
private:
    ws m_leadingws; ws m_trailingws;
    Specific<uint32_t> m_bracket;
    Sequence_t<Specific<uint32_t>, ws> m_tail;
};
```

## 4. Tokens, nodes, model

**tokens.h** — `enum class jsonToken : uint64_t` starting at
`static_cast<uint64_t>(eToken::Max) + 1ull` (so it doesn't collide with the
framework's `eToken`), one entry per production, plus a `std::map` of names for
diagnostics.

**nodes/** — thin `NodeAdapter<ModelType>` wrappers tagged with the token:
```cpp
class ArrayNode final : public NodeAdapter< model::json::Array > {
public:
    ArrayNode() : NodeAdapter<model::json::Array>(static_cast<uint64_t>(jsonToken::array)){};
    virtual ~ArrayNode() noexcept = default;
};
```

**model/** — the real data structures (`Array`, `Object`, `Value`, `ValueModel`,
`number`, `string`, ...). `Value` holds a `polymorphic<ValueModel>`; `Array`
derives from `ValueModel` and overrides `GetType()`.

**helpers.h** — reusable `StandardPrepare`, `StandardFail`, `TrivialEmit`,
`SimpleEmit` templates so simple productions don't each re-implement the pop-loop.

## 5. Feeding input

`Parser` attaches to a `Context` = `pipeline::BufferContext<uint32_t>`
(`CodePointContext`) or `<byte>` (`ByteContext`). The parser is designed to sit
on a `qor::Pipeline` via a `ParserSink`. `Specific::Enter` calls
`context->GetItem(data)`; on match it `ConsumeItem()`s, on mismatch `RejectItem()`s,
and returns `MORE_DATA` when input is exhausted (unless `IsFinal()`).

## 6. Checklist to add a new parser (e.g. YAML, BSON, HTTP)

1. `mkdir src/components/data/formats/<fmt>`; copy the json dir as scaffold.
2. Rename target `qor_<fmt>`; fix `add_library` source list to your productions.
3. Rewrite `tokens.h` enum + name map for your grammar.
4. Define `model/` types for your data model.
5. Write `nodes/` NodeAdapters over those model types.
6. For each ABNF production: write `states/_N/<prod>.h` (class decl) +
   `.cpp` (ctor tree + Prepare/Emit/Fail). Use the ABNF→combinator table.
7. Update `parser/<fmt>parser.h` umbrella include.
8. `add_subdirectory(<fmt>)` in `src/components/data/formats/CMakeLists.txt`.
9. Build: `cmake --build build --target qor_<fmt>`; record in `verification.md` (D3).
10. Add a test exe under `test/components/data/formats/<fmt>` (mirror the
   qor_yaml test pattern from `add_module.md`).

## 7. Notes / gotchas

- `qor_pp_module_interface(QOR_JSON)` marks the symbol as belonging to the module
  interface; use `QOR_<YOURFMT>` consistently.
- `final` on classes = no further derivation (used on every production class).
- Production classes are grouped into `states/_1`.._4` by **nesting depth** in the
  grammar (json: `_1` = top-level array/object/value-ish, `_4` = leaf-ish).
- The framework is header-only (D6: no CMakeLists.txt in `src/components/data/parser`).
- `purpose.md` production counts: JSON ~30, YAML 1.2.2 = 211, Powershell >300,
  C++ thousands — expect the file count to scale with grammar size.
- This design suits "relatively simple languages" (JSON, BSON, HTTP, SQL, XML,
  SMTP, Kafka). Not for ambiguous/hard grammars (C++ most-vexing-parse).