---
Type: WikiPage
PageType: module
Title: "qor_filesystem"
Target: qor_filesystem
Status: current
LastVerified: 2026-11-17
Claims: 9
Summary: "std::filesystem facade over an injected platform pimpl; its path properties are process-wide statics that a Windows build fills with POSIX defaults."
Anchors:
  - { kind: in-tree, path: src/framework/io/filesystem, symbol: qor_filesystem, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/io/filesystem/CMakeLists.txt, symbol: qor_filesystem, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/io/filesystem/filesystem.h, symbol: FileSystem, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/io/filesystem/filesystem.cpp, symbol: RootIndicator, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/io/filesystem/path.h, symbol: Path, verified: 2026-11-17 }
  - { kind: in-tree, path: test/platform/test_filesystem.cpp, symbol: canGetRootFolder, verified: 2026-11-17 }
Judges:
  - cmake --build cmakebuild --target qor_filesystem --parallel 8
---

# qor_filesystem

"File/filesystem access via platform layer modules + std::filesystem."
(`docs/agent/purposes.md:48`; the folder's own `purpose.md` is one line). Built SHARED from ten
translation units - `qor_filesystem.cpp filesystem.cpp path.cpp root.cpp fileindex.cpp folder.cpp
stat.cpp file/file.cpp file/filereader.cpp file/filewriter.cpp pipe/pipe.cpp` - at version 0.5.0
(`src/framework/io/filesystem/CMakeLists.txt:4`).

- 46 of the 281 `CMakeLists.txt` under `src`, `test` and `examples` name it. QORTest names it twice -
  once per OS link line, each with its sibling (`test/CMakeLists.txt:90`, `:93`, `:101`, `:104`) - and
  it has its own test executable, `QORTestFileSystem` (`test/filesystem/CMakeLists.txt:1`), while
  `platform/test_filesystem.cpp` is compiled into QORTest (`test/CMakeLists.txt:37`). The link line
  `qor_module qor_host qor_sync qor_thread qor_platform qor_error` (`CMakeLists.txt:17`) is copied
  token for token by `docs/agent/repo_map.md:61`, which places it at layer 7 of the build order (L138).
- The pimpl is a linker-pull, not a vtable hand-off: `FileSystem : public ISubsystem` holds
  `ref_of<IFileSystem>::type m_pimpl` (`filesystem.h:24`, `:58`), filled by `new_ref<IFileSystem>()`
  (`filesystem.cpp:15`) - an `ExternalFactory` lookup that only resolves if an OS sibling is linked.
  The hook is `ImplementsIFileSystem()`, declared in the two-directional form
  `#ifdef QOR_FILESYSTEM bool qor_pp_import ... #else ... qor_pp_export` (`filesystem.h:15-19`) - this
  target defines the token PRIVATE (`CMakeLists.txt:12`) so it reads import and everyone else export -
  and defined by the sibling as `bool qor_pp_module_interface(QOR_WINDOWSFILESYSTEM)
  ImplementsIFileSystem() { return true; }` under the comment "Implement this trivial function so the
  linker will pull in this library" (`src/platform/os/windows/io/filesystem/winqor_filesystem.cpp:36-37`).
  `ImplementsFile()` works the same way (`file/ifile.h:19-23`).
- `QOR_FILESYSTEM` has 52 hits in 10 headers; `file/filereader.h` alone carries 31 of them, one per
  `Read` overload. The sibling defines its own gate instead (`QOR_WINDOWSFILESYSTEM`,
  `src/platform/os/windows/io/filesystem/CMakeLists.txt:9`).
- The façade is thin over the standard library on both ends: `Path` wraps
  `std::filesystem::path m_path` with conversion operators in both directions
  (`path.h:50-51`, `:63`), and `Index` wraps `std::filesystem::directory_entry m_dirent`
  (`fileindex.h:48`) - `fileindex.cpp` is fifteen straight `std::filesystem::` calls
  (`:74` `is_block_file` through `:114` `file_size`).
- `Root` shares names with two of its own member types, C++-classically: `class Path Path() const;`,
  `class Folder Folder() const;`, `class Path m_path;` (`root.h:24-30`) - the `class` keyword is what
  elides the member-function name that would otherwise hide the type. The tests exercise it bare:
  `Folder rootFolder(m_fileSystem->GetRoot().Path());` runs in both suites
  (`test/platform/test_filesystem.cpp:62`, `test/filesystem/smoke.cpp:65`), and the case is named
  `canGetRootFolder`.

## Failure modes

- The five path properties are statics with a fill-once owner. `s_separator`, `s_selfIndicator`,
  `s_parentIndicator`, `s_rootIndicator`, `s_maxElementLength` are defined in `path.cpp:10-14` and
  assigned nowhere in `Path` itself - the only writes in the tree are `FileSystem::Setup()`
  (`filesystem.cpp:24-28`, a file outside `path.cpp`'s include of `path.h`'s authors). A `Path` built
  before any `FileSystem::Setup()` gets `Separator()` = `""` and `MaxElementLength()` = 0
  (`path.cpp:51-69`), and `Root::Setup()` - called on the line straight after the fills
  (`filesystem.cpp:30`) - sets the root from `Path().RootIndicator()` (`root.cpp:18`), so if the
  `if(m_pimpl.IsNotNull())` guard above it skipped the fills (`filesystem.cpp:22`), the root folder is
  the empty string. `canGetRootFolder` asserts exactly that non-empty (`test/platform/test_filesystem.cpp:62-69`)
  and passes - because `QORTestFileSystem` links `winqor_filesystem` (`test/filesystem/CMakeLists.txt:23`)
  - which means the guard only ever trips in binaries that skip the sibling, silently.
- Windows gets POSIX defaults because nobody overrides them. `IFileSystem` declares the five as virtual
  *with bodies*: `PathSeparator() { return "/"; }`, `RootIndicator() { return "/"; }`,
  `MaxElementLength() { return 256; }` (`ifilesystem.h:59-63`). The Windows sibling implements
  `filesystem.cpp / file.cpp / pipe.cpp` (`src/platform/os/windows/io/filesystem/CMakeLists.txt:4`)
  and 0 of its 6 files mention any of the five names - so `FileSystem::Setup()` on Windows faithfully
  fills the process-wide statics with `/` and `256`.
- `Shutdown()` is `{}` and the pointer it guards is not guarded later
  (`filesystem.h:34`, `filesystem.cpp:33`). `Setup()` checks `m_pimpl.IsNotNull()`, but `Create`
  (`filesystem.cpp:54`), `Open` (`:116`) and `ApplicationLogPath` (`:168`) dereference unguarded, and
  the injected base answers the verbs with silent empties rather than failures:
  `virtual ref_of<File>::type Create(...) const { ref_of<File>::type noresult; return noresult;}`
  (`ifilesystem.h:64`). With no sibling the whole module degrades to null files and empty indicators;
  the async sibling of this design at least `fatal`s (`src/framework/io/async/initiator.cpp`).
- `docs/agent/repo_map.md:62` describes this target's dependencies as `(portable/os libs only)` - the
  one such parenthetical among the 26 framework rows - and it is false on its face two leaves on:
  `qor_sockets`' link line is `set(portable_libraries qor_module qor_host qor_current qor_sync
  qor_platform qor_issue qor_interception qor_asyncioservice)`
  (`src/framework/io/network/CMakeLists.txt:14-15`), naming `qor_host` and a framework lib. The
  index's only claim-style cell in the slice is the wrong one.

## See also

[[qor_sockets]] [[qor_asyncioservice]] [[qor_platform]] [[qor_image]] [[qor_module]]

