---
Type: WikiPage
PageType: module
Title: "qor_compiler"
Target: qor_compiler
Status: current
LastVerified: 2026-11-16
Claims: 5
Summary: "Source compiler traits (quirks/specifics per compiler/version; demangling, vtable layout)."
Anchors:
  - { kind: in-tree, path: src/platform/compiler, symbol: qor_compiler, verified: 2026-11-16 }
  - { kind: in-tree, path: src/platform/compiler/CMakeLists.txt, symbol: qor_compiler, verified: 2026-11-16 }
  - { kind: in-tree, path: src/platform/compiler/compiler.h, symbol: TheCompiler, verified: 2026-11-16 }
  - { kind: in-tree, path: src/platform/compiler/detectcompiler.h, symbol: qor_pp_compiler_root_folder, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_compiler --parallel 8
---

# qor_compiler

Source compiler traits - quirks and specifics per compiler and version, demangling, vtable layout
(`docs/agent/purposes.md`). As with `qor_arch`, the weight is in the headers: the compiled library is
one string.

- One translation unit, and it is the only core library built **STATIC**: line 4 reads
  `add_library(${target} compiler.cpp)` with no `SHARED` keyword, and `BUILD_SHARED_LIBS` is set
  nowhere in the tree, so CMake's default applies. `cmakebuild/src/platform/compiler/Debug/` holds
  `qor_compiler.lib` and no `.dll`, where `qor_platform` and `qor_arch` each produce both
  (`src/platform/compiler/CMakeLists.txt`).
- What the library contributes is `TheCompiler()`, returning the address of a file-static `Compiler`
  (`src/platform/compiler/compiler.cpp`), and `Compiler::Name()`, returning `qor_pp_compiler_name`.
  `Compiler` adds nothing else: it derives from `CompilerBase`, a typedef the *detected*
  compiler-version header provides - `typedef MSVCCompiler21 CompilerBase;`
  (`src/platform/compiler/msvc/vs2022/msvc21.h`).
- The headers do the work the purposes line describes: `detectcompiler.h` picks `qor_pp_compiler` and
  `qor_pp_compiler_root_folder` from the toolchain macros, Boost-style
  (`src/platform/compiler/detectcompiler.h`), and `src/configuration/configuration.h` includes
  `../platform/compiler/detecttoolchain.h` before it can configure anything else - so this folder
  decides the build of every TU in the tree while contributing one linked symbol.

## Failure modes

- Its header is the enforcer of the tree's include-order rule: `#ifndef qor_pp_compiler_include` →
  `#error Every translation unit that uses QOR headers must first include 'src/configuration/configuration.h'`
  (`src/platform/compiler/compiler.h`). A TU that reaches `compiler.h` by any other route fails here,
  and the message is the only documentation of that rule at the point it bites.
- 67 `CMakeLists.txt` files name it in a link line - mostly examples, e.g.
  `examples/intermediate/terminal/CMakeLists.txt` - yet `TheCompiler(` has three matches, all in its own
  two files. The judge is honest and weak: an up-to-date build prints nothing in under a second, and a
  green run says the one TU compiles, not that any consumer's `CompilerBase` still resolves.
