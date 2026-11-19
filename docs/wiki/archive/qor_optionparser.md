---
Type: WikiPage
PageType: module
Title: "qor_optionparser"
Target: qor_optionparser
Status: "retired"
LastVerified: 2026-11-19
Claims: 10
Summary: "Command-line option parsing; declared but never configured, nothing links it, and no class in the tree implements its interface."
Anchors:
  - { kind: in-tree, path: src/framework/app/optionparser, symbol: qor_optionparser, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/app/optionparser/CMakeLists.txt, symbol: qor_optionparser, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/app/optionparser/getter.h, symbol: OptionGetter, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/app/optionparser/ioptionable.h, symbol: IOptionable, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/app/optionparser/context.h, symbol: Context, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/app/CMakeLists.txt, symbol: add_subdirectory, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_optionparser --parallel 8
---

# qor_optionparser

"Command line options parsing." (`docs/agent/purposes.md`). Declared SHARED from one translation unit,
`getter.cpp`, at version 0.5.0 (`src/framework/app/optionparser/CMakeLists.txt`).

- The folder is not configured. `src/framework/app/CMakeLists.txt` is five `add_subdirectory` lines -
  `application dynamiclibrary host role workflow` - and `optionparser` is not one of them, so
  0 files matching `*optionparser*` exist under `cmakebuild/.cmake/api/v1/reply` where `qor_host` has
  four. This is the [[qor_textshaping]] shape, one folder down.
- Nothing links it: of the 281 `CMakeLists.txt` under `src`, `test` and `examples` exactly one names
  `qor_optionparser`, and that is its own (the `set(target` line and the gate-token line). Four example
  headers include it anyway - `examples/advanced/httpserver/httpserverapp.h`,
  `examples/experimental/eventer/main.cpp` (both `getter.h`) and
  `examples/experimental/echoserver/echoserverapp.h`, `examples/experimental/resourced/rdapp.h` (both
  `ioptionable.h`).
- Its link line is `qor_module qor_thread` (`src/framework/app/optionparser/CMakeLists.txt`), which the
  `docs/agent/repo_map.md` row copies exactly - but no file in the folder mentions `thread` at all; the
  only occurrence in the folder is that same `target_link_libraries` line.
- The gate token `QOR_OPTIONPARSER` has 1 hit in all of `src`: the class marker on `OptionGetter`
  (`src/framework/app/optionparser/getter.h`). The other types it exports nothing - `Option`,
  `Context`, `IOptionable` - are header-only.
- Eight headers and one `.cpp`, and no `qor_optionparser.cpp`: 0 occurrences of `ThisModule` in the
  folder, so the target registers no `qor::Module` and would never appear in the registry that
  [[qor_host]] owns, even if it were configured and linked.
- The tree's spelling is `interpretter` - 37 hits against 3 for `interpreter` across `src`, `test` and
  `examples` - and it is load-bearing in file names (`shortoption_interpretter.h`,
  `longoption_interpretter.h`) and in the two member names in `getter.h`, which disagree with each
  other: `shortOptionInterpretter` against `longOptioninterpretter` (lower-case `i`).

## Failure modes

- The judge cannot go green because the target does not exist in the build tree.
  `cmake --build cmakebuild --target qor_optionparser --parallel 8` prints
  `MSBUILD : error MSB1009: Project file does not exist.` / `Switch: qor_optionparser.vcxproj` and
  exits 1. The fix is one `add_subdirectory(optionparser)` line in `src/framework/app/CMakeLists.txt`;
  until then every claim below this one is about code that no build compiles.
- `context.h` is not self-contained. `Context` holds `enum Ordering m_Ordering;`
  (`src/framework/app/optionparser/context.h`), and `Ordering` is declared at
  `src/framework/app/optionparser/shortoption_interpretter.h` - a header `context.h` does not include,
  since its only `#include` is `option.h`. It compiles today only because `getter.h` happens to include
  `shortoption_interpretter.h` before `context.h`, and `getter.h` is the only file in the tree that
  includes this `context.h`, so nothing has ever included it on its own.
- The exported class has no consumer. All 19 mentions of `OptionGetter` across `src`, `test` and
  `examples` are in `getter.h` and `getter.cpp`, and of the 10 mentions of `IOptionable` the 4 outside
  the folder are two `#include` lines and two comments - no class anywhere derives from it. The
  interface's comment says "Implement this interface to be able to get command line options passed to
  your application" (`src/framework/app/optionparser/ioptionable.h`); nothing does.
- The folder has no `purpose.md`, and `src/framework/app/CMakeLists.txt` does not descend into it, so
  neither the repo's per-folder rule nor the build would notice it drifting further. Counted honestly,
  this is the tree's normal state rather than one folder's fault: 46 of the 76 directories under
  `src/framework` carry no `purpose.md` - it was 49 before commit `25c080074a` added three in
  `src/framework/app`.

## See also

[[qor_application]] [[qor_host]] [[qor_textshaping]]

<!-- Retirement stamp, written by tools/wiki-retire/wiki-retire.ps1. Do not delete this -->
<!-- block: reason / evidence / commit is what makes a move auditable (F11). -->
## Retirement

- reason: obselete module has been deleted
- evidence: qor_optionparser deleted.
- commit: dadb425d0c62bb2d53e1d5f2452afa523b3c10bd
- from: modules/qor_optionparser.md
