---
Type: WikiPage
PageType: module
Title: "qor_echo"
Target: qor_echo
Status: current
LastVerified: 2026-11-18
Claims: 25
Summary: "The full-model echo protocol: every byte is a valid token and end-of-data is the message boundary, so message splits follow buffer scheduling, not protocol. Both filters discard FinalParse's result (C4189 on both), the 1024-byte advertised cap is never consulted by the library's own 512-byte filters, and all three indexes miss the sixth linked library qor_parser."
Anchors:
  - { kind: in-tree, path: src/components/protocols/echo, symbol: qor_echo, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/protocols/echo/CMakeLists.txt, symbol: qor_echo, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/protocols/echo/echoprotocol.h, symbol: EchoProtocol, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/protocols/echo/echoservicefilter.cpp, symbol: EchoServiceFilter::WriteAcknowledge, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/protocols/echo/echoresponsefilter.cpp, symbol: EchoResponseFilter::HandleResponse, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/protocols/echo/echorequestparser.cpp, symbol: request::Emit, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_echo --parallel 8
---

# qor_echo

Configured at `protocols/CMakeLists.txt:1`, judge-green on 18 Nov 2026
(exit 0, all 6 declared TUs compiled fresh, 19 dependency artifacts
linked, 1,235,456-byte dll stamped 13:31). `repo_map.md:113`,
`deps.dot:134`, `deps.svg:2276-2302` and `FEATURES.md:54` index it;
`purposes.md:93` bills it as the "full model protocol" - the echo here
is the byte pipe, not RFC 862.

- Folder: 16 files, 714 physical lines - 6 compiled TUs (308 lines),
  9 headers (390 lines), `CMakeLists.txt` (16); no `purpose.md`.
- One protocol class, two filter halves: `EchoProtocol : public
  qor::pipeline::Protocol` (`echoprotocol.h:16`), server-side
  `EchoServiceFilter`, client-side `EchoResponseFilter`.
- The base virtuals are live: `GetNewRequestFilter`/
  `GetNewResponseFilter` are virtual at `protocol.h:41,46`,
  `GetAddressFamily` at `:26`; the protocol's factories return a fresh
  `EchoServiceFilter` / `EchoResponseFilter` (`echoprotocol.cpp:20-28`).
- `GetAddressFamily` hard-wires IPv4: returns
  `eAddressFamily::AF_INet` (`echoprotocol.cpp:15-18`).
- `InlineFilter::WriteAcknowledge` is virtual at `inlinefilter.h:82`;
  the service filter's override is marked `override`
  (`echoservicefilter.h:23`) but the response filter's identical
  signature carries no `override` (`echoresponsefilter.h:23`).
- No grammar at all: `requestChar` and `responseChar` are
  `OneOfAByteRange(0x00, 0xFF)` (`echorequestparser.h:30-37`,
  `echoresponseparser.h:31-38`) - every byte is a valid token; the
  grammar is "one or more of anything".
- End of data is end of message: both char states' `Fail()` rewrite
  `MORE_DATA` to `SUCCESS` and call `SetComplete()`, comment "we're
  simply out of data. That's fine for an echo. Echo what we have"
  (`echorequestparser.h:48-57`, mirrored `echoresponseparser.h:50-59`).
- Token enums start at `eToken::Max + 1` (`echorequestparser.h:19-23`,
  `echoresponseparser.h:20-24`); the token-name maps are `static const
  std::map` in headers (`:25-28` / `:26-29`) - a private copy per
  including TU.
- `Emit` pops CharNodes LIFO into a vector, then rebuilds the string
  through `rbegin()/rend()` (`echorequestparser.cpp:22-56,52`;
  `echoresponseparser.cpp:61`) - two reversals to undo the stack.
- Emit's error path reports but does not stop: unexpected token node
  prints to `std::cout` and fires `serious("Request parsing failed.")`
  then keeps popping (`echorequestparser.cpp:36-45`; response side
  `:45-54`) - with an all-bytes grammar only a foreign parser's node
  can get there.
- The server answer is the request: `HandleRequest` memcpys the parsed
  request value straight into `m_sinkBuffer` (`echoservicefilter.cpp:
  44-58`) - no framing is added in either direction.
- Re-arm ritual: each filter installs a fresh parser step in its ctor
  (`echoservicefilter.cpp:19-24`, `echoresponsefilter.cpp:18-22`) and
  again after every handled message (`:56-57` / `:58-59`).
- The parse result is thrown away on both sides: `int parseResult =
  m_...Parser.FinalParse();` is never read - the compiler says so, C4189
  at `echoservicefilter.cpp:32` and `echoresponsefilter.cpp:30` -
  completion is `IsComplete()` alone.
- `WriteAcknowledge` returns `m_sourceBuffer.WriteCapacity()`
  (`echoservicefilter.cpp:41`, `echoresponsefilter.cpp:38`) - the
  source buffer's capacity, not the sink's.
- `quit` is the library's only semantics, and it is client-side: exact
  match `output == "quit"` fires `continuable("user wants to quit")`
  (`echoresponsefilter.cpp:45-48`); the service filter has no
  equivalent (`echoservicefilter.cpp`).
- Payload objects are 36-line string holders with virtual dtors
  (`echorequest.h:12-32`, `echoresponse.h:12-32`), wrapped in
  `NodeAdapter`-derived `RequestNode`/`ResponseNode` tagged with the
  request/response token (`requestnode.h:16-22`, `responsenode.h:16-22`).
- Export asymmetry: the parser state `response` is exported
  (`echoresponseparser.h:62`) while its mirror `request` is not
  (`echorequestparser.h:60`); protocol and both filters carry the
  export macro.
- Guard style drift: 8 headers use `QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_*`
  but `echoresponse.h:4` uses the `QOR_PP_H_SRC_COMPONENTS_...` form;
  all 9 guards verified unique tree-wide (27 hits, all in-folder).
- Index drift: the link line names six libraries (`CMakeLists.txt:15-16`
  ends in `qor_parser`) while `repo_map.md:113`, `deps.dot:134` and
  `deps.svg:2276-2302` all show five - the missing `qor_parser` edge is
  real: both filters instantiate `data::Parser`
  (`echoservicefilter.cpp:19`, `echoresponsefilter.cpp:18`);
  `add_module.md:42` documents the five-lib core "uri/echo/http use".
- Consumers, exactly two examples: `client/CMakeLists.txt:14` links it
  and sizes its request buffer from `GetMaxEchoSize()`
  (`workflow.cpp:23-25`); `server/CMakeLists.txt:16` links it and runs
  `NetworkServer(12345, EchoProtocol)` (`main.cpp:37`). The
  experimental `echoclient` does not link this library - it rolls its
  own pipeline.
- `GetMaxEchoSize` is an echo-local addition: no `GetMaxEchoSize`
  exists in `src/framework/data/pipeline` (grep) - the base Protocol
  carries no size cap.
- Bootstrap: 14 lines, static `qor::Module` "Querysoft Open Runtime:
  Echo Protocol Module" (`qor_echo.cpp:10-14`), `buildnumber.h` only
  under `NDEBUG` (`:5-7`).

## Failure modes

- Message boundaries are scheduling, not protocol: every byte is a
  valid token and running out of data completes the parse
  (`echorequestparser.h:48-57`), so any write that arrives in more
  than one filter-buffer flush becomes multiple "requests" - the
  server echoes each fragment back as its own message and the client
  parses them apart, silently, with no error or reassembly.
- The 1024-byte cap is advertised but never enforced: `maxEchoSize =
  1024` (`echoprotocol.h:13`) is served by `GetMaxEchoSize()` and the
  client example sizes itself by it (`workflow.cpp:24-25`), yet both
  filters hard-code 512-byte buffers (`echoservicefilter.cpp:19`,
  `echoresponsefilter.cpp:18`) and never consult the cap - a request
  between 513 and 1024 bytes is legal per the advertised limit and
  guaranteed to fragment per the actual one.
- `quit` ends nothing by itself: the check is exact-string equality
  (`echoresponsefilter.cpp:45`), so a trailing CR or LF echoed back
  defeats it forever; and when it does match, the only effect is one
  `continuable` flyer (`:47`) - the library stops nothing, and the
  service filter (`echoservicefilter.cpp`) has no quit path at all.

## See also

[[qor_pipeline]] [[qor_parser]] [[qor_netclient]] [[qor_netserver]]
[[qor_jip]] [[qor_http]] [[qor_stdinsource]] [[qor_module]]
