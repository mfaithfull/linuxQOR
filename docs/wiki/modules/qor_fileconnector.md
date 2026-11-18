---
Type: WikiPage
PageType: module
Title: "qor_fileconnector"
Target: qor_fileconnector
Status: current
LastVerified: 2026-11-18
Claims: 17
Summary: "The most consumed connector and the only configured family member's reference page: a Plug that owns a FileSink and FileSource templated on itself, read by 18 CMake lines including two framework libs that invert the layer naming - with a dead qor_signals edge the graph repeats, a flyer emitted against a library off the link line, and a caller-owned file index stored as a bare pointer that Connect dereferences unchecked."
Anchors:
  - { kind: in-tree, path: src/components/io/pipeline/connectors/fileconnector, symbol: qor_fileconnector, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/connectors/fileconnector/CMakeLists.txt, symbol: qor_fileconnector, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/connectors/fileconnector/fileconnector.h, symbol: FileConnector, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/connectors/fileconnector/filesource.h, symbol: FileSource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/connectors/fileconnector/filesink.h, symbol: FileSink, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/connectors/CMakeLists.txt, symbol: add_subdirectory, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_fileconnector --parallel 8
---

# qor_fileconnector

The configured workhorse of the connector family: the parent
`connectors/CMakeLists.txt:1-4` adds all four connector subdirectories, and
this one carries 18 consumer lines across 13 example trees, 3 test trees and
2 framework libraries. Judge run 18 Nov 2026: green, fresh relink at 12:35
into a 1,057,280-byte dll newer than every folder file (newest,
`filesink.h`, 15 Oct 2026); 27 `qor_fileconnector*` artifacts in `cmakebuild`.
`FEATURES.md:56` "Stable - File Connector for Pipeline";
`docs/agent/purposes.md:86` "File pipeline connector (Sink/Source/Connector
over files)".

- The shape is the family template: `FileConnector : public pipeline::Plug`
  (`fileconnector.h:14`) in namespace `qor::io::components` (`:12`), and its
  elements are templated on it - `FileSource :
  pipeline::iosource<FileConnector>` (`filesource.h:12`), `FileSink :
  pipeline::iosink<FileConnector>` (`filesink.h:12`); every constructor
  allocates both elements and wires them to the connector with
  `SetPlug(this)` (`fileconnector.cpp:17-20,29-32`).
- Two index errors sit in one 4-library line: the link names four
  (`CMakeLists.txt:13-14`: `qor_module qor_signals qor_pipeline
  qor_filesystem`), the graph three (`deps.dot:126`: module, signals,
  pipeline) so the `qor_filesystem` edge is invisible - while
  `qor_signals`, present in both, is used by no file in the folder: no
  include, no symbol (grep of the folder finds only `OnWriteError`, a
  pipeline method). The index agrees with CMake, not with the code.
- `filesource.cpp:7` includes the flyer header and `ReadBytes` emits
  `continuable("Attempting to read from closed file.")` when the connector
  is down (`:29`) - yet `qor_error` is not on the link line
  (`CMakeLists.txt:13-14`); the judge run shows it built only transitively
  (through `qor_pipeline`).
- The file index is caller-owned and stored as a bare pointer: the
  non-default constructors take it by const reference and keep its address
  through a `const_cast` (`fileconnector.cpp:27`), `SetFile` does the same
  for a mutable reference (`:103-105`), and the default constructor leaves
  `m_pFileIndex{ nullptr }` (`fileconnector.h:41`) with `FileIndex()`
  dereferencing it unconditionally (`:108-111`).
- `Connect()` judges success by handle non-nullness - `m_connected =
  !m_File.IsNull()` - with the stronger `->IsOpen()` check commented out
  beside it (`fileconnector.cpp:61`); `Disconnect` disposes the file
  (`:68`) and the destructor disconnects when still connected (`:48-53`).
- The mode defaults fork: constructor parameters default to
  `WithFlags::CreateNew` (`fileconnector.h:19-21`) while the default
  constructor sets `m_Mode = WithFlags::None` (`fileconnector.cpp:14`) -
  two different meanings for "unspecified mode".
- Pumping is thin over `io::File`: `ReadBytes` calls
  `File()->Read(space, bytesToRead, -1)` (`filesource.cpp:20`, a literal
  -1 timeout argument) and treats a 0 return as `OnEndOfData()` (`:22-25`);
  `WriteBytes` calls `File()->Write` (`filesink.cpp:20`), and the
  disconnected branches are asymmetric - the source flyers (`:29`) while
  the sink just fires `OnWriteError(0, 0)` and returns 0 silently
  (`:24`).
- The async-connection path is a fossil in both files:
  `HandlePendingConnectionResult` is commented out in the header
  (`fileconnector.h:27`) and its 9-line body is commented out in the cpp
  (`fileconnector.cpp:72-81`).
- The layer naming inverts here: this `src/components` lib is linked by two
  `src/framework` libs - `src/framework/data/resources/image/CMakeLists.txt:15`
  (qor_image) and `src/framework/data/resources/CMakeLists.txt:22`
  (qor_resources) - the framework consuming the components layer, not the
  other way round.
- Its consumers are also the wiki's most cross-linked example set:
  `examples/basic/plumbing/CMakeLists.txt:22`, the two configured
  intermediate trees (`client:14`, `server:24`), and the test trees
  `test/pipeline/CMakeLists.txt:15`, `test/components/formats/json/
  CMakeLists.txt:16,23` - every page that pumps a file into a pipeline
  passes through this dll.
- `repo_map.md:105` mirrors `deps.dot:126` exactly (module, signals,
  pipeline) - the pair's shared blind spot is CMake's, not each other's.
- All eight files carry the 2-line SPDX BSL-1.0 header ("2008 - Present");
  the 15-line bootstrap names "Querysoft Open Runtime: File Connector
  Module" (`qor_fileconnector.cpp:12`).
- One closing-comment fossil: `filesource.h:25` closes with
  `//qor::io:components` - one colon short of the namespace it names.
- Folder: 8 files, 322 physical lines - declared TUs 200 (bootstrap 15,
  `fileconnector.cpp` 122, `filesink.cpp` 29, `filesource.cpp` 34, all four
  in `add_library`, `CMakeLists.txt:3`), headers 108 (55+27+26).
- Judge evidence: `cmake --build cmakebuild --target qor_fileconnector
  --parallel 8` exits 0 (18 Nov 2026), relinking only its own dll - every
  dependency dll was already current.

## Failure modes

- Default-construct then `Connect()` is a null dereference: `m_pFileIndex`
  starts null (`fileconnector.h:41`) and `Connect` passes `*m_pFileIndex`
  to the file constructor with no guard (`fileconnector.cpp:60`);
  `FileIndex()` (`:110`) is the same unguarded dereference - the empty
  connector is a crash waiting for either call.
- A disconnected sink is silent where a disconnected source speaks: the
  source flyers `continuable` on a closed connector (`filesource.cpp:29`)
  but the sink writes nothing, returns 0 and reports `OnWriteError(0, 0)`
  with zero context (`filesink.cpp:24`) - a pipeline that loses its file
  mid-run surfaces the loss only as stalled octet counts downstream.

## See also

[[qor_pipeline]] [[qor_signals]] [[qor_filesystem]] [[qor_module]]
[[qor_error]] [[qor_image]] [[qor_resources]] [[qor_jsondata]]
[[qor_linereader]]

Not yet paged: `qor_pipeconnector`, `qor_socketconnector`,
`qor_stdfstreamconnector` (this group - the three siblings the parent's
four `add_subdirectory` lines name alongside this one).
