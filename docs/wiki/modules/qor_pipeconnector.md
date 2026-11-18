---
Type: WikiPage
PageType: module
Title: "qor_pipeconnector"
Target: qor_pipeconnector
Status: current
LastVerified: 2026-11-18
Claims: 16
Summary: "The mirror of the clients ghosts: fully configured and judge-green - four TUs compile into a real dll - but functionally a stub: ConnectToName ignores its argument and returns false so the connector can never connect, Configure is declared and never defined, m_pipe is never assigned, and nothing in the repo links the target at all."
Anchors:
  - { kind: in-tree, path: src/components/io/pipeline/connectors/pipeconnector, symbol: qor_pipeconnector, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/connectors/pipeconnector/CMakeLists.txt, symbol: qor_pipeconnector, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/connectors/pipeconnector/pipeconnector.h, symbol: PipeConnector, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/connectors/pipeconnector/pipesource.h, symbol: PipeSource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/connectors/pipeconnector/pipesink.h, symbol: PipeSink, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/connectors/CMakeLists.txt, symbol: add_subdirectory, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_pipeconnector --parallel 8
---

# qor_pipeconnector

The inverse failure of the clients ghosts: the build system reaches it -
`connectors/CMakeLists.txt:3` adds the folder, the judge exited 0 on 18 Nov
2026 with all four declared TUs compiling fresh into a 1,043,968-byte dll
stamped 12:38, 11 `qor_pipeconnector*` artifacts in `cmakebuild` - and the
class is still inert: its connect path is a return-false stub, and no target
in `src`, `examples` or `test` links it. `docs/agent/repo_map.md:106` and
`deps.dot:127` index it, `purposes.md:87` calls it "Generic pipe connector.",
`FEATURES.md` has no row.

- `Connect()` can never report true: the public `Connect()` delegates to
  `ConnectToName(m_name)` (`pipeconnector.cpp:40-47`), and
  `ConnectToName(const std::string& /*name*/)` discards its argument and
  unconditionally `return false` (`:49-52`) - the flag `m_connected` is set
  false forever, whatever name is configured.
- `Configure(const std::string &name)` is declared (`pipeconnector.h:28`)
  but defined in no TU of the folder (grep `Configure` across the folder's
  `.cpp` files: 0 hits) - a member the class advertises but cannot link to,
  and `m_name` is therefore never settable from outside the constructor's
  `m_name = ""` (`pipeconnector.cpp:15`).
- The pipe itself is never created: `m_pipe` is assigned nowhere in the
  folder, so `Pipe()` returns a default (empty) reference
  (`pipeconnector.cpp:60-63`); `Disconnect()`'s only body is a commented-out
  `//m_pipe->` and a flag reset (`:54-58`).
- Because `IsConnected()` can only be false, both pump paths are
  unreachable-by-design: `ReadBytes` and `WriteBytes` gate on
  `m_Connector->IsConnected()` (`pipesource.cpp:18`, `pipesink.cpp:18`),
  so every attempt takes the failure branch - the flyer
  `continuable("Attempting to read from closed pipe.")` (`pipesource.cpp:29`)
  or `OnWriteError(0,0)` (`pipesink.cpp:24`).
- The export marking carries a copy-paste fossil: `Pipe()` is marked
  `qor_pp_module_interface(QOR_FILECONNECTOR)` (`pipeconnector.h:29`) - the
  *file* connector's macro - while the compile definition for this target is
  `QOR_PIPECONNECTOR` (`CMakeLists.txt:9`).
- The shape is the family template unchanged: `PipeConnector :
  pipeline::Plug` (`pipeconnector.h:16`) in `qor::io::components` (`:14`),
  with `PipeSource : pipeline::iosource<PipeConnector>` (`pipesource.h:12`)
  and `PipeSink : pipeline::iosink<PipeConnector>` (`pipesink.h:12`); the
  constructor wires both elements with `SetPlug(this)`
  (`pipeconnector.cpp:16-19`) - the same construction as `FileConnector`
  two folders left.
- The element pair is a rename of the fileconnector pair: the pump bodies
  differ from `filesource.cpp`/`filesink.cpp` only in the noun
  ("closed pipe" `:29`) and the pipe call (`Pipe()->Read(space,
  bytesToRead, -1)` `pipesource.cpp:20` keeps the same literal -1 timeout
  argument).
- Index vs link: the link line names five libraries
  (`CMakeLists.txt:14-15`: `qor_module qor_signals qor_pipeline
  qor_filesystem qor_sockets`) while `deps.dot:127` and `repo_map.md:106`
  show three - `qor_filesystem` and `qor_sockets` invisible; unlike the
  fileconnector page's dead edge, both invisible edges are genuinely
  included here (`pipeconnector.h:8-10` pulls `io/network/socket.h`,
  `sockets.h` and `filesystem/pipe/pipe.h`) - while `qor_signals`, in both
  link and graph, is used by no file in the folder.
- The consumer grep is the finding: `qor_pipeconnector` appears in exactly
  two CMake files in the tree - its own `CMakeLists.txt` and the parent's
  `add_subdirectory` line - no example, no test, no library links the dll.
- `qor_error` is again off the link line while `pipesource.cpp:7` includes
  the flyer header and emits `continuable` (`:29`) - the flyer resolves only
  transitively through `qor_pipeline`, the same asymmetry the fileconnector
  page records.
- The fossil closer repeats: `pipesource.h:25` closes `//qor::io:components`
  - one colon short, copied with the rest of the filesource skeleton.
- All eight files carry the 2-line SPDX BSL-1.0 header ("2008 - Present");
  the 15-line bootstrap names "Querysoft Open Runtime: Pipe Connector
  Module" (`qor_pipeconnector.cpp:12`).
- Folder: 8 files, 259 physical lines - declared TUs 143 (bootstrap 15,
  `pipeconnector.cpp` 65, `pipesink.cpp` 29, `pipesource.cpp` 34, all in
  `add_library`, `CMakeLists.txt:3-4`), headers 101 (48+27+26).
- Judge evidence: `cmake --build cmakebuild --target qor_pipeconnector
  --parallel 8` exit 0, 18 Nov 2026 - green build over a stub: the
  instruments can certify that it compiles, not that it works, which is
  exactly the gap a `Judges:` list is honest about.

## Failure modes

- A configured-but-inert connector fails at the pump, not at connect:
  `Connect()` returns false silently (`pipeconnector.cpp:44` via
  `:49-52`) with no flyer on that path, and a pipeline built on it surfaces
  the loss only when pumping - `continuable` for a source
  (`pipesource.cpp:29`), silent `OnWriteError(0, 0)` for a sink
  (`pipesink.cpp:24`).
- Anyone who does link this dll and calls `Configure` gets a link error,
  not a compile error: the declaration exists (`pipeconnector.h:28`) so the
  call compiles, while no definition exists anywhere - the two stub halves
  (declared configure, never-defined connect) fail at different times, and
  only the second one is loud.

## See also

[[qor_fileconnector]] [[qor_pipeline]] [[qor_signals]] [[qor_filesystem]]
[[qor_sockets]] [[qor_module]] [[qor_error]]

Not yet paged: `qor_socketconnector`, `qor_stdfstreamconnector` (this
group - the two remaining siblings named by the parent's
`add_subdirectory` lines).
