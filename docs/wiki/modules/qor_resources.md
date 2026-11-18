---
Type: WikiPage
PageType: module
Title: "qor_resources"
Target: qor_resources
Status: current
LastVerified: 2026-11-16
Claims: 11
Summary: "Async resource manager behind the ResourceHub Feature; widest link line in the framework, and its Font path parses a font and throws the object away."
Anchors:
  - { kind: in-tree, path: src/framework/data/resources, symbol: qor_resources, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/data/resources/CMakeLists.txt, symbol: qor_resources, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/data/resources/resourcehub.h, symbol: ResourceHub, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/data/resources/qor_resources.cpp, symbol: ThisModule, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/data/resources/types/font/reader.h, symbol: TTFReader, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/data/resources/types/font/fontresource.cpp, symbol: Font::Claim, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_resources --parallel 8
---

# qor_resources

"Async general resource manager: discovery, unique ID, classification, parsing -> validated in-memory
objects; offloaded to background task. Use ResourceHub Feature + Paths/FileIndexes/Claimer."
(`docs/agent/purposes.md`). Built SHARED from ten translation units at version 0.0.2
(`src/framework/data/resources/CMakeLists.txt`).

- Its link line names 17 `qor_` targets - the widest in `src/framework` and the second widest anywhere
  in `src`, measured over the 167 `target_link_libraries(${target} ...)` lines in the 208
  `CMakeLists.txt` under `src` (only `src/platform/os/windows/ui/gui` names more, at 19).
  `docs/agent/repo_map.md` lists 6 of those 17:
  `qor_module qor_thread qor_sync qor_platform qor_error qor_filesystem`. Absent from the row:
  `qor_application qor_signals qor_log qor_pipeline qor_fileconnector qor_jsondata qor_parsersink
  qor_parser qor_workflow qor_deserializersink qor_serialize`.
- Almost nothing consumes it: one `CMakeLists.txt` links it, `examples/experimental/resourced`
  (two targets). Grepping the name finds a second file,
  `src/platform/os/linux/ui/x/xresources/CMakeLists.txt`, which is `linqor_resources` - a substring
  collision, not a consumer.
- The purposes line's "ResourceHub Feature" is literal C++: `ResourceHub : public app::IFeature, public
  ResourceManager` (`src/framework/data/resources/resourcehub.h`) - the feature contract is
  [[qor_role]]'s `app::IFeature`. The other two bases come from elsewhere: `ResourceManager` derives
  from `SlotBase` and `manager.h` includes `src/framework/event/signals/slotbase.h`; `Resource` derives
  from `SignalBase` and `resource.h` includes `src/framework/event/signals/signal.h`. `qor_signals` is
  one of the 11 names the repo_map row omits.
- `qor_pp_module_will_provide(QOR_RESOURCES, ResourceHub)` (`resourcehub.h`) is 1 of 16 uses of that
  macro in 12 files, and `src/configuration/configuration.h` expands it to
  `namespace qor{ bool qor_pp_module_interface(_M) qor_pp_cat(Implements,_X)(); }` - the same
  declaration the header hand-writes eight lines above it behind
  `#ifndef QOR_PP_IMPLEMENTS_RESOURCEHUB`. `resourcehub.cpp` is the one file that `#define`s that macro.
- Every URI it mints carries a placeholder: `"font://<ip-address>/" + name`, `"json://<ip-address>/..."`,
  `"file://<ip-address>/..."` (`types/font/fontresource.cpp`, `types/json/jsonresource.cpp`,
  `types/fileresource.cpp`) - 3 hits for the literal `<ip-address>` in all of `src`, and all 3 are here.
- `purpose.md` is 182 words, 4th longest of the 28 under `src/framework`, and it still reads as a
  design note rather than a contract: "Writing custom per resource code is tiresome and error prone"
  (`src/framework/data/resources/purpose.md`).
## Failure modes

- `Font::GetObject()` cannot return an object. `fontresource.h` declares
  `ref_of<TTFObject>::type m_object;` and `fontresource.cpp` returns it from `GetObject()`; across the
  two files those are the only mentions of the member - nothing assigns it. `Font::Claim()` calls the
  reader and drops the result into `auto result` without reading it again
  (`src/framework/data/resources/types/font/fontresource.cpp`), under its own comment "Parse the file
  fully and extract the TTFObject(s) from it."
- The producer is broken too, so the call site is not the only fix.
  `TTFReader::operator()` computes `auto ttfState = sink.GetObject();` and then
  `return ref_of<TTFObject>::type();` (`src/framework/data/resources/types/font/reader.h`) - an empty
  ref on the only path. The JSON sibling shows what was meant: `JSON::Locate()` does
  `m_object = reader(...)` and gates on `if(m_object.IsNotNull())`
  (`src/framework/data/resources/types/json/jsonresource.cpp`).
- Nothing tests the reader this folder ships. The one test named for it,
  `test/pipeline/testttfreader.cpp` (built into target `QORTestPipelines`, which is configured),
  includes `src/components/data/formats/ttf/reader.h` and exercises `qor::res::ttf::Reader` - a
  different class in `src/components` - and its fixture path is an absolute path on the `F:` drive
  naming `cmakebuild/test/data`, so it cannot run from a checkout that is not on that drive.
- QOR code includes the header that says QOR may not. `src/framework/app/role/getfeature.h` opens
  "The QOR itself can never include this header as it requires the defintion of any derived application
  class in use to already be present", and two files here include it (`manager.cpp`,
  `types/pathresource.cpp`); the latter calls `GetFeature<thread::ThreadPool>()`, which walks
  `AppBuilder().TheApplication()->GetRole()`. That also breaks the index's own build order:
  `docs/agent/repo_map.md` puts `qor_resources` at layer 9 and `qor_application` at layer 10 while this
  link line names `qor_application`.
- `src/framework/data/resources/types/font/reader.cpp` is named by 0 of the 282 `CMakeLists.txt` in the
  tree, so it is compiled by no target - harmless today, since the file is 234 bytes whose entire body
  is an empty `namespace qor{ namespace framework{ namespace res {` pair, but it means the header it
  includes is checked by nothing.

## See also

[[qor_role]] [[qor_pipeline]] [[qor_error]] [[qor_log]] [[qor_module]]

