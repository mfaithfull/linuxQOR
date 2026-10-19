# Product Context

## Why this project exists
The QOR addresses the long-standing problem of the `utilities`/`utils` folder in
application source trees: code that is critical to an application, meets no
application requirement by itself, receives almost no attention, and is not
organised at all. Over the years frameworks and standard libraries absorbed
those utilities, and the framework layer keeps taking on more work. `KEY_CONCEPTS.md`
argues it is time to re-examine the application-over-framework layering with two
guiding ideas:

- **Separation of Concerns** → portability, adaptability, reduced coupling to
  dependencies (single-responsibility at the architectural scale).
- **Self Aware Software** → make the software do to itself what a computer can
  do well: test itself, build itself, report on its activity, scale itself,
  manage its own memory, port itself, connect itself, examine itself
  (reflection → serialization, AOP).

"None of these are things you want to have to write in each application. All of
these belong in the framework. The framework should facilitate SOC and SAC."

## Problems it solves (user motivation, FAQ.md)
- Write your application **once**, run it **everywhere**, **forever**, at full
  speed.
- Read, understand, and change if necessary **all** the source in your product.
- When a completely new hardware platform, OS, or toolchain appears, be there
  first with your existing code.
- Get advanced C++ techniques (reflection, smart references, DI, pipelines,
  interception, async IO, thread pools, signals/slots) without writing them
  yourself.
- Do as much or as little memory and object lifetime management as you want.
- Replace just the parts you want without re-writing or duplicating thousands of
  lines of code.

## How it should work (intended usage flow)
1. Read a little about the organization of the QOR and decide how you want to
   use it.
2. Configure it.
3. Build it with your chosen compiler for your chosen OS and hardware (CMake).
4. Write your application against the QOR SDK headers, e.g. `sdk/app.h`.
5. Link your application against the QOR runtime you built in step 3.

You do **not** have to use the application framework to use the other libraries.

## User experience goals (FEATURES.md)
- Almost everything is optional — full control; "it's just C++".
- Everything works together out of the box.
- Almost everything can be overridden or replaced where configuration doesn't
  cut it.
- Easy modular system: patterns and libraries for building "perfect" C++ DLLs
  with controlled dependencies and APIs; no need for header-only.
- Adding a new module is 5–10 minutes of work.
- Role and Feature based application framework with argument parsing and full
  customization.
- TDD tool set: assertion, test and mocking frameworks that work together.
- constexpr data structures (map, set, string); state machine based workflows;
  type erased functions; class registry (in-process ORB for registering
  implementations of specific interfaces).
- Policy-based smart pointer + integrated memory management: no more `new` and
  `delete`.
- Thread pool and integrated async task system; asynchronous IO service;
  asynchronous task library.
- Modular data pipelines: plug together sources, filters and sinks.
- Extensible error handling and logging using the flyer pattern.
- Reflection (pre-C++26, using pfr); signals & slots (multi-thread, native C++,
  no pre-processor); delegates.
- Partial localization support: classes for many types of Unicode data;
  filesystem support over `std::filesystem`.
- Spin up a network server with a protocol plugin in a few lines.

## In-progress product outcome
Run a **TUI or GUI from the same easy-to-use scene template**. The Wayland
layer, X Windows layer and Windows GDI layers are written, the layout library
works, the terminal code is mostly ported from FTXUI, the JSON parser is
written — assembly is still required. Community feedback (r/cpp) says a GUI
builder with a better-than-JSON human-readable serialization format is needed to
compete with Qt.

## Openness & collaboration
The project is "way more than one crazy person can handle". Contributions are
open: GitHub/vcpkg expertise, UI design, profiling, data structures,
optimisation, Mac, Android, WASM, Vulkan. Enumerated starter contributions in
`CONTRIBUTING.md`: add a CodePage under
`src/qor/essentials/text/codepage/codepages` (+ a CodePage-specific String type
and UTF-8 conversion tests), add unit/operational tests, Unicode database →
data structures, normalisation, Bidi algorithm, text shaping (HarfBuzz
OpenType shaper), FastHeap vs new/delete drag-race harness, Mac OS support
(clang-llvm first, FileSystem next), widget set for OpenGL/Vulkan/MoltenVK, a
YAML parser (built on the JSON parser base, UTF-8 first), database engines,
Kafka pipeline components.
