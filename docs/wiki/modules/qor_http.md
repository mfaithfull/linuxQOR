---
Type: WikiPage
PageType: module
Title: "qor_http"
Target: qor_http
Status: current
LastVerified: 2026-11-18
Claims: 24
Summary: "The largest leaf so far and the only one whose judge build is red: HTTPSink's ctor calls new_ref on BufferContext<uint32_t> with no arguments (sink.cpp:11) and BufferContext has only a Buffer*-taking ctor (buffercontext.h:18), so the target does not compile (C2512, exit 1, no dll). 141 files, 6 of 46 TUs compiled, a Hello-World server hard-coded in the filter, and the duplicate http_old declaration resolved by D5 - built once, declared twice."
Anchors:
  - { kind: in-tree, path: src/components/protocols/http, symbol: qor_http, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/protocols/http/CMakeLists.txt, symbol: qor_http, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/protocols/CMakeLists.txt, symbol: http, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/protocols/http/protocol.cpp, symbol: HTTPProtocol::GetReasonPhrase, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/protocols/http/filter.cpp, symbol: HTTPFilter::Filter, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/protocols/http/client/responsesink/sink.cpp, symbol: HTTPSink::HTTPSink, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_http --parallel 8
---

# qor_http

Configured at `protocols/CMakeLists.txt:4`, judge-RED on 18 Nov 2026:
`cmake --build cmakebuild --target qor_http --parallel 8` exits 1 -
C2512 in `sink.cpp:11` - and no dll exists under
`cmakebuild/src/components/protocols/http/Debug/`. The largest folder
of the six leaves: 141 files, 10,431 physical lines, 46 `.cpp` and 94
`.h`. `repo_map.md:114`, `deps.dot:135`, `deps.svg:2306-2332`,
`purposes.md:94` and `FEATURES.md:97` index it - and `FEATURES.md:97`
is the only leaf row that says "Incomplete".

- **Declared twice, built once (D5):** `set(target qor_http)` appears
  in BOTH `http/CMakeLists.txt:1` and `http_old/CMakeLists.txt:1`;
  `wiki-census targets` prints `declared_twice = [qor_http]`. Only
  `protocols/CMakeLists.txt:1-4` decides - it adds `http`, never
  `http_old`, and no CMakeLists anywhere references `http_old`. This
  page is the one place a reader looks for why two folders name one
  target: `purposes.md:94` records `http_old` as redundant ref code,
  not built, excluded from the repo map and ctags.
- Of 46 `.cpp` files the build compiles six (`CMakeLists.txt:4-9`);
  41 source lines are commented out (`:10-50`); `sink.cpp` is named
  twice - live at `:8`, commented again at `:47`.
- Judge evidence: exit 1 on 18 Nov 2026; first error C2512
  `BufferContext<uint32_t>: no appropriate default constructor`
  instantiating `sink.cpp:11`; 5 of 6 TUs compile with warnings, the
  sixth is fatal.
- One filter for both directions: `GetNewRequestFilter` AND
  `GetNewResponseFilter` each return `new_ref<HTTPFilter>()`
  (`protocol.cpp:21-29`) - echo splits service/response filters, http
  does not; `Filter` is virtual at `inlinefilter.h:99`.
- The filter's buffer size defaults to zero:
  `HTTPFilter(size_t itemCount = 0)` (`filter.h:19`) and the protocol
  factories use the default (`protocol.cpp:23,28`) - echo hard-codes
  512, http hard-codes nothing.
- `GetRequestBufferSize`/`GetResponseBufferSize` return 16384
  (`protocol.cpp:31-39`); neither name exists in
  `src/framework/data/pipeline` (grep) - http-local additions to the
  base Protocol, like echo's `GetMaxEchoSize`.
- The built-in server is a demo hard-coded in the filter: `GET` gets
  200 + `text/plain` + "Hello, World!", every other method 405 +
  "Method Not Allowed" (`filter.cpp:37-48`).
- Dead path rewrite: `if(request->GetPath() == "/") request->SetPath
  ("/index.html")` (`filter.cpp:31-34`) - nothing on the response path
  reads `GetPath()` afterwards.
- `GetReasonPhrase` is a 40-case switch with the typos shipped in it:
  "Non-Authoritive Information" (`protocol.cpp:50`), "HTTP Version not
  supoprted" (`:84`); default returns "Unsupported extension-code"
  (`:85`).
- Model objects: `HTTPRequest` (`request.h:21-218`) carries
  method/path/headers/trailers/body; `Headers` is a case-insensitive
  `unordered_multimap` (`headers.h:13`) - the only case-folded
  container in these six targets.
- `HTTPResponse` defaults to version 1.1 and status 404
  (`response.h:18`); `SetStatus` rejects out-of-range values with a
  `warning` flyer and keeps the old code (`:34-44`); the object holds
  TWO payload strings - `SetValue/GetValue` (`:24-32`) and
  `SetBody/GetBody` (`:76-84`).
- `GetURI()` returns `m_path` with `//TODO: URI object ?`
  (`request.h:48-51`) - a second TODO repeats at `:201`.
- `HTTPRequest`'s private block carries client machinery the server
  never uses - content receiver, upload/download progress,
  `m_maxRedirectCount`, `m_startTime` (`request.h:208-216`) - and the
  ctor definition lives in `request.cpp:11`, a commented-out TU
  (`CMakeLists.txt:50`).
- Client end pieces are real pipeline parts: `HTTPSource :
  pipeline::Source` (`source.h:17`) and `HTTPSink : pipeline::Sink`
  (`sink.h:17`); both request and response bodies are generated by
  Workflows - `HTTPRequestGenerator : Workflow`
  (`requestgenerator.h:16`) and `response::Generator : Workflow`
  (`responsegenerator.h:15-17`, namespace `response` inside
  namespace `http`).
- `HTTPSink` never feeds its parser: the only data line is commented -
  `//m_context->SetData(data, bytesToWrite);` (`sink.cpp:60`) - so
  `Write` parses an empty context and returns `GetPosition()`
  (`:61-67`); the compiler confirms, C4100 on `data` and
  `bytesToWrite` at `sink.cpp:58`.
- Parser contexts are typed `uint32_t`, not `byte` (`filter.cpp:58`,
  `sink.cpp:11`) - the compiler flags the resulting loss in the
  parser-state instantiations, C4244 `uint32_t` to `char`
  (`responseparser.h:47`, `unreservedtoken.h:48`).
- Orphan: `server/httpservicefilter.h` is 0 bytes and referenced
  nowhere in the live tree; the class name `HttpServiceFilter` exists
  only in `http_old/httpservicefilter.h:32` - migration debris.
- Index drift, same shape as echo: the link line names six libraries
  (`CMakeLists.txt:61-62`, ending `qor_parser`) while `repo_map.md:114`,
  `deps.dot:135` and the `deps.svg:2306-2332` edges all show five -
  `qor_parser` missing; `add_module.md:42` documents the five-lib core.
- Consumers, counted: `advanced/httpserver/CMakeLists.txt:20` links
  `qor_http qor_parser` and runs `NetworkServer(5080, HTTPProtocol)`
  (`httpserverapp.cpp:81`); `experimental/curlike/CMakeLists.txt:16`
  holds `HTTPSource m_source; HTTPSink m_sink;` members
  (`httpclient.h:183-184`); `experimental/requestread/CMakeLists.txt:15`
  links it; `qor_httpclient` has an edge to it (`deps.svg:2012`);
  `test/operational/CMakeLists.txt:33` links it.
- The test situation, read to the boundary: `test/CMakeLists.txt:132`
  adds the http test dir, but `QORTestHTTPRequestGen` does not link
  qor_http - `qor_libraries` names nineteen libraries and `qor_http`
  is not among them (`test/.../http/CMakeLists.txt:12-14`) - and its
  TU includes no http header at all: 30 lines of app harness titled
  "Test HTTP Requst Gen" (`test_httprequestgen.cpp:19`). The one real
  filter test is commented out (`test/CMakeLists.txt:68`).
- Bootstrap: 14 lines, static Module "Querysoft Open Runtime: HTTP
  Protocol Module" (`qor_http.cpp:10-14`).

## Failure modes

- The target does not compile: `HTTPSink::HTTPSink()` runs
  `m_context(new_ref<pipeline::BufferContext<uint32_t>>())`
  (`sink.cpp:11`) but `BufferContext` declares exactly one ctor,
  `BufferContext(const Buffer*)` (`buffercontext.h:18`) - C2512,
  exit 1, no dll. Every consumer that links `qor_http` (`httpserver`,
  `curlike`, `requestread`, the operational test) therefore ships
  against a stale or missing binary.
- The sink's data path is dead even in the source: with
  `SetData` commented out (`sink.cpp:60`), `Write` runs the parser
  over a context that never receives bytes, returns
  `m_context->GetPosition()` (`:67`) - permanently zero - and
  `GetResponse()` (`:16-19`) hands back the null `m_response`
  forever.
- The filter answers every connection from one template: a parse
  failure returns with only a `//TODO: return a Bad Request Response`
  comment (`filter.cpp:25-29`) and `writeCount` is never assigned on
  that path, while a successful parse always produces the same
  "Hello, World!" body (`filter.cpp:37-48`) - no path, host or body
  from the request reaches the response.

## See also

[[qor_echo]] [[qor_jip]] [[qor_parser]] [[qor_pipeline]] [[qor_uri]]
[[qor_httpclient]] [[qor_netserver]] [[qor_module]]
