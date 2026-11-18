---
Type: WikiPage
PageType: module
Title: "qor_parser"
Target: qor_parser
Status: current
LastVerified: 2026-11-18
Claims: 14
Summary: "The grammar framework 18 other CMakeLists link - 9 exported state classes, a template half (combinators plus 15 RFC5234 productions) and 17 tokens; it has no test target of its own, one of its 11 .cpp compiles nothing, and the judge linked nothing new."
Anchors:
  - { kind: in-tree, path: src/components/data/parser, symbol: qor_parser, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/parser/CMakeLists.txt, symbol: qor_parser, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/parser/parser.h, symbol: Parser, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/parser/state.h, symbol: ParserState, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/parser/states/rfc5234.h, symbol: ALPHA, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/parser/states/sequence.h, symbol: Sequence_t, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_parser --parallel 8
---

# qor_parser

"General parser state machine; derived from qor_workflow w/ Parse/FinalParse"
(`docs/agent/purposes.md:77`), and the folder explains itself at length: "The Parser class is
derived from qor::Workflow but doesn't use the standard Run function. It adds Parse and FinalParse
instead" (`src/components/data/parser/purpose.md:2-3`). The folder is 34 files - 11 `.cpp`
(903 lines), 19 `.h` (1,357), three `purpose.md` (107 + 48 + 2) and a 22-line `CMakeLists.txt`;
it builds SHARED.

- The widest fan-in in this group so far: 19 of the tree's 281 `CMakeLists.txt` name the target -
  its own (`:1`, `:3`), 10 in `src` (`qor_jsondata` `json/CMakeLists.txt:42`, `qor_resources`
  `framework/data/resources/CMakeLists.txt:22`, the sinks `parsersink:13` and
  `deserializersink:13`, and `protocols/{yaml:21, uri:14, jip:16, http:62, echo:16, http_old:16}`),
  two test executables (`test/components/formats/json/CMakeLists.txt:17` and `:24`,
  `test/components/protocols/yaml/CMakeLists.txt:15`) and six example executables
  (`examples/advanced/httpserver/CMakeLists.txt:20` plus five under `examples/experimental` and
  `examples/intermediate`). The index rows agree with the build: `repo_map.md:84` and
  `deps.dot:105` print all 7 libraries on the link line (`CMakeLists.txt:21-22`).
- `add_library` (`:3-11`) names 11 `.cpp`, the folder holds exactly 11 and
  `cmakebuild/src/components/data/parser` holds 11 `.obj` - but one of them compiles nothing:
  `states/specific.cpp:10-12` is an empty `qor::data::parser` namespace, because `Specific` is a
  template (`states/specific.h:13`) and this TU instantiates none.
- Nine exported classes at 10 marker sites: `Parser` is marked twice (forward declaration
  `state.h:19`, definition `parser.h:17`), `ParserState` (`state.h:23`) and the combinators
  `Sequence` (`states/sequence.h:12`), `OneOrMore` (`oneormore.h:12`), `ZeroOrMore`
  (`zeroormore.h:12`), `Optional` (`optional.h:12`), `AnyOneOf` (`anyoneof.h:12`),
  `AnyOneOfSet` (`anyoneofset.h:13`), `OneOfAByteRange` (`oneofarange.h:12`); `QOR_PARSER` is
  defined PRIVATE by this target alone (`CMakeLists.txt:16`).
- The template half carries no marker and no `.cpp`: the twins `Sequence_t` (`sequence.h:29`),
  `AnyOneOf_t` (`anyoneof.h:27`), `OneOfARange_t` (`oneofarange.h:25`), `ZeroOrMore_t`
  (`zeroormore.h:28`), `Specific` (`specific.h:13`), `deferred` (`deferred.h:12`) and `AcceptAll`
  (`state.h:47-48`), plus the 15 RFC 5234 core productions in `states/rfc5234.h` - `CR` (`:20`)
  through `ALPHA` (`:269`) - which the `json` grammar states derive from.
- The token space is 17 names with a sentinel: `enum class eToken` (`tokens.h:13-33`) fixes only
  `Lexical = 1` (`:15`), `Max` ends the list (`:32`), and the `tokenNames` map (`:35-53`) has 17
  rows; `qor_jsondata` numbers its own `jsonToken` enum from `eToken::Max + 1ull`
  (`src/components/data/formats/json/parser/tokens.h:21`).
- `Parser` is a workflow object: it derives from `Fastflow` (`parser.h:17`,
  `src/framework/app/workflow/fastflow.h` included at `:10`), exposes `Parse`/`FinalParse`
  (`:31-32`) instead of `Run`, keeps a node stack (`m_nodes`, `:49`) and a per-token state cache
  (`m_stateCache`, `:50`; `GetStateCache` `:37`). `ParserState` is a `fastflow::Step`
  (`state.h:23`) with `Prepare`/`Emit`/`Fail` (`:38-40`).
- The folder is the only component folder here with its own `purpose.md` (107 lines): "every
  grammar production gets it's own class, closely matching the ABNF" (`:20-21`), and a production
  census - "JSON has about 30 productions, YAML 1.2.2 has 211, Powershell > 300" (`:107`).
  Three agent docs call it framework ground: the `purposes.md:77` row,
  `docs/agent/parsers.md:59` "`qor_parser` (the framework)" and the core-dep rule
  `docs/agent/add_module.md:42`.
- The judge build links nothing new: exit 0, a 20-line log of msbuild up-to-date lines only, no
  `cl.exe`; `qor_parser.dll` (1,324,544 bytes) is stamped 18 Nov 2026 08:33:09 from this
  session's earlier `qor_jsondata` build, which pulls this library, while `.exp` is dated
  16 Nov and `.lib` 24 Oct, and the newest source (`states/specific.h`) is 22 Oct 2026.

## Failure modes

- The target has no test target of its own and the judge only compiles: correctness is exercised
  only through consumers' tests (`test/components/formats/json/CMakeLists.txt:17`,
  `test/components/protocols/yaml/CMakeLists.txt:15`), and no test executable is one of the 90
  covered module targets - so a green `qor_parser` page says the framework builds, never that a
  grammar parses.
- Inserting an `eToken` renumbers every downstream grammar: the enum fixes only `Lexical = 1`
  (`tokens.h:15`), so every later value - and the whole `jsonToken` range starting at `Max + 1`
  (`json/parser/tokens.h:21`) - shifts across the dll boundary; `Diagnostic` (`parser.h:35`) can
  name tokens only through the 17-row map (`tokens.h:35-53`) and prints nothing for grammar
  tokens.
- Marked runtime and template twin sit side by side - `Sequence` (`sequence.h:12`) and
  `Sequence_t` (`:29`) - grammars mix both (`json` states derive from the `_t` versions), and no
  instrument checks that the exported class and its twin still behave alike.
- `AcceptAll` ships with its own warning - "Don't use this in practice as it will consume all the
  rest of any data" (`state.h:51`) - and nothing enforces it; one miswired state silently drains
  the stream, and this target's compile-only judge cannot see it.
- `qor_parser` still links into the deprecated tree (`src/components/protocols/http_old` at
  `CMakeLists.txt:16`): removing any of the 9 exported classes has to keep that build alive, and
  no index row marks the consumer deprecated.

## See also

[[qor_module]] [[qor_host]] [[qor_thread]] [[qor_sync]] [[qor_injection]] [[qor_workflow]]
[[qor_pipeline]] [[qor_jsondata]]

Not yet paged: the parser's pipeline side - `qor_parsersink`
(`src/components/data/pipeline/sinks/parsersink`) and `qor_deserializersink` beside it - and the
protocol grammars that consume the framework, `qor_http`, `qor_uri`, `qor_jip`, `qor_echo` and
`qor_yaml` under `src/components/protocols`.
