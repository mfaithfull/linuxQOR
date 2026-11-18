---
Type: WikiPage
PageType: module
Title: "qor_arch"
Target: qor_arch
Status: current
LastVerified: 2026-11-16
Claims: 5
Summary: "Architecture of built QOR (endianness, word size, arch-dependent facts factored in)."
Anchors:
  - { kind: in-tree, path: src/platform/architecture, symbol: qor_arch, verified: 2026-11-16 }
  - { kind: in-tree, path: src/platform/architecture/CMakeLists.txt, symbol: qor_arch, verified: 2026-11-16 }
  - { kind: in-tree, path: src/platform/architecture/cpu.h, symbol: TheCPU, verified: 2026-11-16 }
  - { kind: in-tree, path: src/platform/os/windows/app/sharedbootstrap/CMakeLists.txt, symbol: winqor_sharedbootstrap, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_arch --parallel 8
---

# qor_arch

Architecture of the *built* QOR - endianness, word size, arch-dependent facts factored in
(`docs/agent/purposes.md`). The compiled library is one class; the headers beside it are what the
tree actually consumes.

- The exported surface is `qor::arch::CPU`, a singleton with a single member, `ByteOrder()`, plus the
  accessor `TheCPU()` (`src/platform/architecture/cpu.h#TheCPU`). Two translation units, built SHARED:
  `cpu.cpp` and the module identity TU `qor_architecture.cpp`
  (`src/platform/architecture/CMakeLists.txt`).
- The headers are the product: `src/configuration/configuration.h` includes
  `../platform/architecture/architectures.h` before it can name an architecture and
  `../platform/architecture/detectarchitecture.h` to default the target to the host, so every TU in
  the build reads this folder - and neither include pulls the library in.
- Its link line is OS-dependent in a way a base library should not be: the Windows branch adds
  `winqor_sharedbootstrap` to `qor_module qor_host qor_thread`, the Linux branch does not
  (`src/platform/architecture/CMakeLists.txt`). That object library lives three levels down in the OS
  tree (`src/platform/os/windows/app/sharedbootstrap/CMakeLists.txt`), so the dependency runs from the
  base layer into the Windows layer.

## Failure modes

- Nothing calls it. `TheCPU(` has exactly two matches in `src`, `test` and `examples`: its own
  definition (`src/platform/architecture/cpu.cpp`) and its own declaration
  (`src/platform/architecture/cpu.h`). Fifteen `CMakeLists.txt` files still link it - fourteen test
  suites (`test/tef/CMakeLists.txt`, `test/pipeline/CMakeLists.txt`, …) plus
  `examples/experimental/modeltest` - so a build failure here breaks fifteen targets that never read
  the answer.
- Its private definition is `QOR_PP_ARCH`, a token that appears in no source file; the module macro in
  the sources spells `QOR_ARCH` (3 sites). The mismatch is harmless only because the macro ignores its
  argument's identity (`src/configuration/configuration.h#qor_pp_module_interface`), and it makes
  grepping the build flags for this library's name come up empty.
