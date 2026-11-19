---
Type: WikiPage
PageType: module
Title: "qor_optionparser"
Target: qor_optionparser
Status: "retired"
LastVerified: 2026-11-19
Claims: 6
Summary: "Removed module: nothing in src, test or examples names it, and the only anchors that still re-resolve are the folder that never configured it and the commit that deleted it."
Sources:
  - refresh-20261119-1
  - refresh-20261119-2
  - refresh-20261119-3
  - refresh-20261119-4
  - refresh-20261119-5
Anchors:
  - { kind: in-tree, path: src/framework/app/CMakeLists.txt, symbol: add_subdirectory, verified: 2026-11-19 }
  - { kind: in-tree, path: docs/agent/purposes.md, anchor: commit:dadb425d0c62bb2d53e1d5f2452afa523b3c10bd, verified: 2026-11-19 }
Judges:
  - cmake --build cmakebuild --target qor_optionparser --parallel 8
---

# qor_optionparser

Retired page: the module was deleted wholesale by commit `dadb425d` ("Removed obselete option parser
and cleaned up remaining references to it."), stamped in the Retirement block below. What the tree
still holds of it, present tense:

- The folder is gone: `src/framework/app/optionparser` does not exist, and 0 files under `src`,
  `test` and `examples` name `optionparser` at all. The five in-tree anchors this page once carried
  for the folder, its `CMakeLists.txt`, `getter.h`, `ioptionable.h` and `context.h` re-resolve now
  only through the deletion commit that removed those files.
- The symbols it exported are gone with it: 0 occurrences of `qor_optionparser`, `OptionGetter`,
  `IOptionable` or the gate token `QOR_OPTIONPARSER` remain under `src`, `test` and `examples` -
  no include, comment, `set(target` line or class marker survives anywhere they were once counted.
- The four example headers that included it anyway - `examples/advanced/httpserver/httpserverapp.h`,
  `examples/experimental/eventer/main.cpp`, `examples/experimental/echoserver/echoserverapp.h` and
  `examples/experimental/resourced/rdapp.h` - were stripped by the same commit: all four still exist,
  and none of them mentions option parsing now.
- The build shape around the deletion is unchanged: `src/framework/app/CMakeLists.txt` is still the
  five `add_subdirectory` lines - `application dynamiclibrary host role workflow` - that never
  configured the folder. The deletion removed code no build compiled, not a configuration.
- Its purpose line no longer stands: the quote this page once opened with ("Command line options
  parsing.") is gone from `docs/agent/purposes.md`, and that file's head is the deletion commit
  itself - what remains of the quote is the line `dadb425d` removed there.

## Failure modes

- The judge can never go green, and there is nothing left to green:
  `cmake --build cmakebuild --target qor_optionparser --parallel 8` fails with
  `MSBUILD : error MSB1009: Project file does not exist.` The old failure was a declared target no
  parent configured; after `dadb425d` no `CMakeLists.txt` declares the target at all, so the fix
  that was once one `add_subdirectory` line does not exist.

## See also

[[qor_application]] [[qor_host]] [[qor_textshaping]]

<!-- Retirement stamp, written by tools/wiki-retire/wiki-retire.ps1. Do not delete this -->
<!-- block: reason / evidence / commit is what makes a move auditable (F11). -->
## Retirement

- reason: obselete module has been deleted
- evidence: qor_optionparser deleted.
- commit: dadb425d0c62bb2d53e1d5f2452afa523b3c10bd
- from: modules/qor_optionparser.md
