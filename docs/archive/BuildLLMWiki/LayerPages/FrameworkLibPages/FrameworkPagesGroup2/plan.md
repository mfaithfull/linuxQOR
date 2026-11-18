---
Type: Plan
Title: "Framework pages group 2"
Parent: "../plan.md"
Level: 3
Items:
  - Title: Write the qor_resources page
    Status: Atomic
    Content:
  - Title: Write the qor_image page
    Status: Atomic
    Content:
  - Title: Write the qor_events page
    Status: Atomic
    Content:
  - Title: Write the qor_signals page
    Status: Atomic
    Content:
  - Title: Write the qor_asyncioservice page
    Status: Atomic
    Content:
  - Title: Write the qor_filesystem page
    Status: Atomic
    Content:
  - Title: Write the qor_sockets page
    Status: Atomic
    Content:
---

# Framework pages group 2

## Goal

Seven steps of **one page each**: `qor_resources`, `qor_image`, `qor_events`, `qor_signals`,
`qor_asyncioservice`, `qor_filesystem`, `qor_sockets`.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- **PLAN CLOSED 17 Nov 2026.** `Cursor: closed - all 7 leaves landed`. All seven pages are on disk
  (`qor_resources` 11, `qor_image` 13, `qor_events` 10, `qor_signals` 11, `qor_asyncioservice` 10,
  `qor_filesystem` 9, `qor_sockets` 9 claims), all judges green, and the close line printed
  `coverage = 31 of 90`, `uncovered_targets = 59`, `dangling_module_pages = 0`, `illegal_fields = 0`
  (`pages = 31 of 37`), `dead = 0` (`anchors = 118 of 118`), each `VERDICT fail=0`.
  `FrameworkPagesGroup3` opens on `coverage = 31 of 90`.
- **The truncation defect is not a one-off.** Three of seven pages landed with the same wound -
  `qor_asyncioservice` (leaf 5, found and repaired), `qor_filesystem` and `qor_sockets` (leaf 6-7,
  found while closing: a claim bullet cut mid-sentence, `## Failure modes` inserted over its tail, the
  orphaned remainder parked after `## See also`). It is the writer's long insert landing on a boundary
  line, and none of the three instruments sees it: the front matter is legal, the anchors resolve, the
  body is well-formed markdown. Read the body of every page this plan writes, top to bottom, before
  its stop pair runs. `qor_image` was caught the same way on 16 Nov.
- **Both instrument facts below paid for themselves.** The qualified-symbol rule bit again on
  `sockets.cpp#Sockets::CreateSocket` (dead - the out-of-line definition is `Sockets::CreateSocket`,
  so the `:` defeats the lookbehind); the anchor is now `sockets.cpp#Sockets`. And counting every bullet
  caught `qor_filesystem` at `Claims: 11` over 9 bullets, plus four seeded pages in the same survey
  (`qor_current` 4/5, `qor_error` 5/6, `qor_log` 5/6, `qor_threadmemory` 5/7) that this plan does not
  own - `qor_dynamiclibrary` was its own and is fixed to 7.
- **17 Nov 2026 re-verify:** `qor_asyncioservice.md` had landed truncated (claim 5 cut mid-sentence,
  `## Failure modes` heading dropped inside it, its tail orphaned after `See also`). Repaired against
  `src/framework/io/async/context.h:124-178`, judge re-run green, instruments re-green at the same
  numbers. A truncated write is invisible to all three instruments - only reading the body finds it.
- Nothing here besides leaves 1-5 is landed: neither `qor_filesystem` nor `qor_sockets` has a page
  (`wiki-census coverage` prints the set).
- **Two instrument facts worth re-using.** `Test-WikiSymbolInFile`
  (`tools/wiki-census/wiki-shape.ps1:380`) resolves a symbol with
  `(?<![A-Za-z0-9_:])symbol`, so an anchor whose symbol only ever appears as `Class::Member` in a
  `.cpp` is **dead** - the out-of-line definition is preceded by `:`. Pick a symbol that appears bare
  in the file (leaf 2: `image.cpp#SetSize` was dead, `image.cpp#Free` resolves because `SetSize` calls
  `Free()` unqualified). And `Claims:` counts **every** top-level `- ` bullet in the body, Failure
  modes included - the seeded pages (`qor_module.md` 4, `qor_application.md` 6) set that precedent, and
  `wiki-lint schema` does not check the number, so it is on the writer to count.

## Context

Inherited from the parent (`../plan.md`, Level 2) and grandparent (`../../plan.md`, Level 1): one
page per build target in `docs/wiki/modules/`; `Judges:` is that target's own build command; the
target list was verified once by `Reconcile the target list`. **`../plan.md` §Decisions holds the
page contract - shape, read budget, judge rule, stop pair - and binds every leaf here.** Sibling
titles: `FrameworkPagesGroup1`, `FrameworkPagesGroup3`, `FrameworkPagesGroup4`.

## Decisions

- **`qor_image` is a target of its own inside `qor_resources`' folder** -
  `src/framework/data/resources/image/CMakeLists.txt` declares it, so the two pages name two
  different CMakeLists. A leaf that reads its own `CMakeLists.txt` cannot confuse them; a leaf that
  reads the folder cannot either, which is why the read budget names the file.

## Write the qor_resources page

**CLOSED 16 Nov 2026.** `docs/wiki/modules/qor_resources.md`, `Claims: 11` (6 claims + 5 failure
modes), judge `cmake --build cmakebuild --target qor_resources --parallel 8` (green). Its link line is
the widest in `src/framework` (17 `qor_` targets) and `docs/agent/repo_map.md` lists 6 of them;
`Font::GetObject()` returns a member nothing assigns.

Input verified then: `FrameworkPagesGroup1` closed on `coverage = 24 of 90`.

Stop condition printed: `coverage = 25 of 90`, `uncovered_targets = 65`, `illegal_fields = 0`,
`dead = 0`.

## Write the qor_image page

**CLOSED 16 Nov 2026.** `docs/wiki/modules/qor_image.md`, `Claims: 13` (7 claims + 6 failure modes),
judge `cmake --build cmakebuild --target qor_image --parallel 8` (green). The strongest claim:
`m_width`/`m_height` have 0 assignments in the folder, so nothing ever sizes an image, and
`RGBPixel`'s default template argument `rgb8` exists nowhere in the tree - which is why the one
consumer (`examples/experimental/images/images.cpp:114`, `Image output;`) is commented out of the
build (`examples/experimental/CMakeLists.txt:9`).

Input verified then: `coverage = 25 of 90` printed by the leaf above.

Stop condition printed: `coverage = 26 of 90`, `uncovered_targets = 64`, `illegal_fields = 0`,
`dead = 0`.

## Write the qor_events page

**CLOSED 16 Nov 2026.** `docs/wiki/modules/qor_events.md`, `Claims: 10` (6 claims + 4 failure modes),
judge `cmake --build cmakebuild --target qor_events --parallel 8` (green). The strongest pair:
`QOR_EVENTS` has 0 hits in `src` so the SHARED target exports no class (which is why its one consumer,
`examples/experimental/eventer/main.cpp`, links nothing and is itself named by no
`add_subdirectory`); and `PODQueue`'s capacity overrides are declared without the base's `const`, so
they hide rather than override and the wrap-around guard never runs through a base reference.

Input verified then: `coverage = 26 of 90` printed by the leaf above.

Stop condition printed: `coverage = 27 of 90`, `uncovered_targets = 63`, `illegal_fields = 0`,
`dead = 0`.

## Write the qor_signals page

**CLOSED 16 Nov 2026.** `docs/wiki/modules/qor_signals.md`, `Claims: 11` (6 claims + 5 failure modes),
judge `cmake --build cmakebuild --target qor_signals --parallel 8` (green). The target is vendored
CsSignal rebranded in place; `SignalBase`'s class marker tests another target's `QOR_APPLICATION`;
`compareThreads()` is `return true` with 0 overrides, so queued connections run inline on the emitting
thread; and the 11 vendored files stamp `SPDX-License-Identifier: BSL-1.0` above their own BSD-2 notice.

Input verified then: `coverage = 27 of 90` printed by the leaf above.

Stop condition printed: `coverage = 28 of 90`, `uncovered_targets = 62`, `illegal_fields = 0`
(`pages = 28 of 34`), `dead = 0` (`anchors = 100 of 100`).

## Write the qor_asyncioservice page

**CLOSED 16 Nov 2026.** `docs/wiki/modules/qor_asyncioservice.md`, `Claims: 10` (5 claims + 5 failure
modes), judge `cmake --build cmakebuild --target qor_asyncioservice --parallel 8` (green). The
exported `ImplementsAsyncIOService()` has no definition anywhere; `m_contextCount` is never
initialised; the OS siblings `winqor_/linqor_asyncioservice` register the real implementations and
appear nowhere in `repo_map.md`.

Input verified then: `coverage = 28 of 90` printed by the leaf above.

Stop condition printed: `coverage = 29 of 90`, `uncovered_targets = 61`, `illegal_fields = 0`
(`pages = 29 of 35`), `dead = 0` (`anchors = 106 of 106`).

## Write the qor_filesystem page

**CLOSED 17 Nov 2026.** `docs/wiki/modules/qor_filesystem.md`, `Claims: 9` (5 claims + 4 failure
modes), judge `cmake --build cmakebuild --target qor_filesystem --parallel 8` (green). The five path
properties are process-wide statics written only by `FileSystem::Setup()`
(`filesystem.cpp:24-28`), and `IFileSystem`'s defaults for them (`ifilesystem.h:59-63`) are POSIX -
0 of the 6 Windows sibling files overrides any, so a Windows build fills `/` and `256`. The pimpl
arrives by linker-pull (`ImplementsIFileSystem()`, `filesystem.h:15-19`), the same seam as
[[qor_dynamiclibrary]]. Repaired after landing: see the truncation note in the resume slot.

Input verified then: `coverage = 29 of 90` printed by the leaf above.

Stop condition printed: `coverage = 30 of 90`, `uncovered_targets = 60`, `illegal_fields = 0`,
`dead = 0`.

## Write the qor_sockets page

**CLOSED 17 Nov 2026.** `docs/wiki/modules/qor_sockets.md`, `Claims: 9` (5 claims + 4 failure modes),
judge `cmake --build cmakebuild --target qor_sockets --parallel 8` (green). The portable half refuses:
`Sockets::CreateSocket` returns an empty ref (`sockets.cpp:21-25`) and `ImplementsSockets()` is defined
only by the OS twins; the twelve addressing `.cpp/.h` pairs sit in a commented-out source list
(`CMakeLists.txt:5`) while `ipendpoint.h`'s two out-of-line members live in the unbuilt
`ipendpoint.cpp`; and `repo_map.md:62` writes this row's dependencies as `(portable/os libs only)`
while its own link line names `qor_host` and `qor_asyncioservice` - which the index places at layer 8,
after this target's 7. Repaired after landing: same truncation note.

Input verified then: `coverage = 30 of 90` printed by the leaf above.

Stop condition printed: `coverage = 31 of 90`, `uncovered_targets = 59`, `illegal_fields = 0`
(`pages = 31 of 37`), `dead = 0` (`anchors = 118 of 118`) - this plan's close line.

