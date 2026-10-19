# Active Context

**Last updated:** 19 Oct 2026 (memory bank initialized from `STATUS.md`
29/07/2026, `CHANGES.md`, `FEATURES.md`, git log at `6b65785a`).

## Current focus (STATUS.md, 29/07/2026)
- **UI design** is still in progress; things "are coalescing slowly around a
  reasonable compromise".
- The **parser** has had a lot of work and now reads most Unicode text
  encodings.
- The **JSON parser** needs upgrading to be layered over Unicode text before it
  can be used to load UI scene templates.
- Trying out **Pipes in a platform-agnostic form**: sockets on Linux, files on
  Windows (see branch `PipeConnectorExperiment`).
- Planning to post to r/cpp Show and Tell.

## July 2026 priorities
- Portable modules are now **versioned**, identifying those ready for **0.5**.
- **HTTP** is the next major element to be cleaned up and completed — needs a
  couple of systematic refactors and generator infrastructure.
- A **version 0.5 interim release plan** is assembled with a long list of things
  to do before and after.
- **Scrubbing the module export interfaces** is under way — a pretty major piece
  of work that won't be complete soon. Base libraries are now clean and clean
  examples can be built; **anything using pipeline components won't** — that's
  the next target.
- June/July quality push: builds with all the pedantry turned on are possible;
  many `/qor` libraries are clean even with everything cranked up.

## Recent commits (`main`, newest first, at time of init)
- `6b65785a` DIO test MPL namespace fixes
- `fefab2aa` experimental mixins updates
- `04f8167f`, `d46e3e55` non-MSVC fixes
- `3b6a626a` **kvasir-based meta programming library**
- `6b1f253a` Signal tree tests + JSON test data
- `9fedc59f` experimental **signal tree** implementation
- `bb704b47` experimental `uri` and `jip` stuff
- `b2ea83c4` low-level `const` and `noexcept` improvements
- `95a791b8` Windows fixes · `8553df26` fixes · `3f2c5128` clang support docs
- `bb6510e0` fixes

## Active branches
`main` (HEAD, = `origin/main`), `Contract`, `DomainInterfaceObjects`,
`FileSystemRefactor`, `PipeConnectorExperiment`, `day1`, `module_support`.

## Next steps (explicit in STATUS.md / CONTRIBUTING.md)
1. HTTP: systematic refactors + generator infrastructure; complete response
   generation.
2. Pipelines: "still need a lot of work"; add pipeline components.
3. Parser edge cases to bottom out: tiny buffer sizes, parse completion.
4. Generator framework for writing text-based files, plus one for portable
   binary formats.
5. Unicode: more code pages, Unicode database → usable data structures,
   normalisation, Bidi; text shaping (hardest).
6. Serialization over pipelines → font and image loading, web server responses.
7. Multiple test executables: separate operational tests (e.g. thread pool
   performance) from fast loop-unrolled unit tests — needs CMake work.
8. Structural and control-flow modules for the framework, and initial components
   for customizing memory management.
9. Remaining export-interface scrubbing (pipeline-component consumers).
10. UI: assemble scene template loading once JSON sits over Unicode.

## Active decisions & considerations
- **UI**: single scene template driving both TUI and GUI; the whole UI subset is
  treated as experimental; Windows/Linux UI bits work on their own terms but
  integration into portable code is largely missing.
- **Pipes**: abstract platform-agnostic pipe over sockets (Linux) vs files
  (Windows).
- **MPL**: new kvasir-based meta programming library alongside `qor/essentials/mpl`.
- **qor_error + strings**: integration works but requires **double linking under
  GCC** — unfortunate and maybe resolvable; otherwise the error-raising interface
  moves into another module.
- Experimental code lives in `qor::exp::` and by 1.0 should graduate to
  `qor::components::…` (`qor::io::components`, `qor::io::network::components`,
  `qor::pipeline::components`, `qor::data::components`) or into main `qor` if
  fundamental.
- Serialization format for UI scenes is a live design question (JSON vs a
  human-readable format).

## Learnings & project insights
- The organizational structure is holding at large scale: ~66 cross-platform
  libraries, ~2,750 source files, 206 CMake targets in `src/`, with
  dependencies and sizes not out of control — "this kind of proves it works at
  least at this scale".
- Pedantic builds (`-Wall -Wextra -Wpedantic`, hidden visibility) are now
  achievable for most `/qor` libraries; some (e.g. `qor_mock`) probably never
  will be, given what they do.
- "Nothing scales forever"; the structure is judged good for anything one person
  can linearly write in a year or two.
- Style of QOR code is now clear; suggestions of extensions, specializations and
  improvements to qor modules are welcome.
- Build-time expectations: full build < 30 min on a reasonable PC (~26 min on
  GitHub); clean build of the incomplete HTTPServer example ~38 s.

## Working preferences observed
- Follow `docs/skills/QORconventions.md` exactly (license header, `configuration.h`
  first include, `QOR_PP_H_*` guards, `qor_pp_module_interface` qualifiers).
- New folder ⇒ add `purpose.md` (and `status.md` for volatile state).
- Keep the "rule of 7" in mind when creating folders/namespaces.
- Prefer `new_ref<T>()` smart references over raw `new`/`delete`/`unique_ptr`.
