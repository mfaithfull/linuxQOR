# Project Brief — QOR (Querysoft Open Runtime)

**Workspace:** `c:\RAID0\Develop\linuxQOR` — the **linuxQOR** repository
(migrated in Oct 2026 from `f:\Develop\linuxQOR`; any hard-coded
`F:/Develop/...` path in sources/CMake is stale).
https://github.com/mfaithfull/linuxQOR. Author: Matthew Faithfull
(mfaithfull *at* btopenworld.com).

## What it is
A free, open-source, **cross-platform, modular C++20 application framework**.
The QOR is intended to be *Self Aware Software* with very strong *Separation Of
Concerns* (see `KEY_CONCEPTS.md`). If it can be done in C++ or C, that's the
default way to do it.

CMake project declaration:
`project(QOR VERSION 0.5.0 DESCRIPTION "Querysoft Open Runtime" LANGUAGES C CXX)`

License: **BSL-1.0** (Boost Software License) for QOR code; MIT/BSD where
inherited from upstream forks (see `LICENSE`, `CREDITS.md`).

Approximate scale (re-measured 19 Oct 2026 at HEAD `93668d3d`): ~2,810 source
files (`.h/.hpp/.cpp/.inl`) in `src/`, 207 `CMakeLists.txt` under `src/`, **66**
cross-platform libraries per `STATUS.md`.

## Core requirements / goals
1. **Separation of Concerns** — drive out unnecessary coupling at all levels;
   be able to change the behaviour of already-written code without breaking it.
   Portability and adaptability follow from it.
2. **Self Aware Code** — anything the computer can do well, we make it do: unit
   testing/TDD, building, logging/tracing/metrics/debug info, scaling (thread
   pools), memory management, porting, connecting (modules, signals, delegates,
   RPC), reflection and serialization. Reflection is the basis of the AOP
   approach used in the QOR.
3. **Testable** — TDD is the development process. Extensive testing is essential
   because multiple compilers and target platforms are supported.
4. **Easy to extend** — adding a new QOR module is 5–10 minutes of work.
5. **Flexible** — use what you need; the rest must not bother you with breaking
   changes. Most libraries are usable without `qor::app::Application`, Roles or
   Features: link `qor_module` and provide a `ThisModule` implementation, then
   use only the modules' actual dependencies.
6. **Portable** — cross platform, not just multi-platform. OS, compiler and
   processor-architecture dependencies are factored out into `src/platform`, so
   porting is easy.
7. **Reliability over peak performance** — "The QOR may not give you the highest
   possible performance implementation, however you'll get a reliable working
   implementation sooner than anything more optimised."

## Scope (top-level tree, LAYOUT.md)
- `src/qor` — ubiquitous low-level libraries: `essentials` (current,
  datastructures, iterators, mpl, objectcontext, reporting, text, traits),
  `memory`, `module`, `flyers`, `function`, `property`, `reflection`, `tdd`
- `src/framework` — structural application elements: `app`, `data`, `event`,
  `io`, `parallel`, `ui`
- `src/components` — optional modules built on the framework: `data` (formats,
  mdm, parser, pipeline, res…), `io` (deserializer, generator, network,
  pipeline), `protocols` (echo, http, http_old, jip, uri), `ui` (canvas,
  console, egl, layout, opengles, renderer, terminal, widgets), `qor`
- `src/platform` — `os` (linux, windows), `compiler`, `architecture`
- `src/macros` — preprocessor library (`qor_pp_*`)
- `src/configuration` — headers included in every translation unit
- `sdk/` — headers generated for external applications (`app.h`, `qor.h`,
  `framework.h`, `essentials.h`, `memory.h`, `error.h`, `log.h`, `tdd.h`, …)
- `test/` — unit + operational tests; `examples/` — demonstrating & validating
  aspects of the QOR; `3rdparty/` (glad, Windows); `docs/`; `cmake/`

## Non-goals & explicit positions
- Not the highest-performance implementation.
- **C++ language modules are not used (yet).** "QOR module" means a runtime
  loadable, dynamic library (`.dll` on Windows, `.so` on Linux) with QOR
  registration extras. Never confuse the two terminologies.
- Macros remain in heavy use; pre-compiled headers are used heavily (speed and
  future C++ module preparation).
- The C++ flavour is deliberately opinionated ("and therefore almost always
  wrong"); C++20 is required.
- Contributions are welcome, but only once the intent/style/direction is
  comprehensible to the contributor (`CONTRIBUTING.md`, `STATUS.md`).

## Source of truth for status
`STATUS.md` (latest entry 29/07/2026 — **trails the repo**, which moved on
through Oct 2026), `CHANGES.md` (nothing shipped yet; 0.5 is the first
potentially releasable milestone), `FEATURES.md` (per-module maturity table).
Milestones: working toward **0.5** interim release; **1.0** is the eventual
target. For the newest state trust `git log`, the parallel agent-state files
`docs/agent/TODO.md`, `docs/agent/STATE.MD`, `docs/agent/DECISIONS.md`, and this
`memory-bank/` (last updated 20 Oct 2026 — JSON generator **pipeline integration**
verified at `[OK](36/36)` in
`QORTestJSONGenerator`, with the parse -> generate -> parse **roundtrip** verified
green in `QORTestJSON` (process exit code 0, `test/data/json_roundtrip_*.json`
written and compared equal through `jsoncompare.h`)).
Also 22 Oct 2026: the framework-layer data module `src/framework/data/mime` (lib `qor_mime`) — a MIME-type-recognition
**pipeline filter** (ten popular MIME types + application/octet-stream fallback, test-first plan
`memory-bank/qor_mime_plan.md`) — has **plan steps 0-14 DONE and verified green** (`QORTestMIME.exe`
`[TOTAL](15/15) [OK](15/15)` exit 0, every step RED-then-GREEN except step 4 verified after the fact and
step 14 GREEN on its FIRST run): scaffold constructs, octets pass through untouched, the leading octets
captured into the 16-byte signature window, **ALL 10 of the 10 target types recognised** (PNG, JPEG, GIF, PDF, ZIP, XML, HTML, JSON,
JavaScript, plain text) via fixed-order guards in `Inspect()`, **plus the application/octet-stream
fallback proven** — a stream pumped in full always ends with a type and a non-empty name — **and recognition
proven across pumped chunk boundaries** (the 8-byte PNG magic split across three 3-byte pumped chunks still
classifies `MIMEType::PNG`, with NO implementation change: the window accumulates per chunk). Next plan step:
15 `mime_recognizer_resets_between_streams` (`Reset()` reuse — already implemented, the case verifies it),
then 16-17 (chaining with `qor_base64encodefilter`, `mime_type_name_drives_a_sink` optional scope).


## Key project documents
`README.md` · `KEY_CONCEPTS.md` · `LAYOUT.md` · `BUILDING.md` · `FEATURES.md` ·
`STATUS.md` · `CHANGES.md` · `FAQ.md` · `CONTRIBUTING.md` · `CREDITS.md` ·
`docs/` (tutorial, structure, module system, linking, globalobjects,
levels_of_abstraction, power_of_context, smart_references, sfumato, typetraits,
compilers, articles) · `docs/skills/QORconventions.md` (authoritative coding
conventions) · `.clinerules/memory-bank.md` (memory-bank rules for this
workspace).
