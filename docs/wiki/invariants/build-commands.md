---
Type: WikiPage
PageType: invariant
Title: "build-commands"
Status: current
LastVerified: 2026-11-18
Claims: 5
Summary: "One QOR lib is the unit of verification and only the touched lib rebuilds - stated with build/-j4, run with cmakebuild/--parallel 8; the rule's own text is stale against the tree it governs."
Anchors:
  - { kind: in-tree, path: docs/agent/verification.md, symbol: incremental, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_signals --parallel 8
---

# build-commands

The assertion, from `docs/agent/verification.md` section "Build commands (incremental)": the unit of
verification is **one QOR lib** (one `set(target qor_X)`), the rebuild is **incremental and only the
touched lib**, and one entry per verification run is recorded newest-first. If it fails, a verification
run silently covers less (or more) than the increment it names, and the Results log entries stop
corresponding to any reproducible build.

- As written the rule names `cmake -S . -B build` and `--build build --target qor_<NAME> -- -j4`.
  As the tree runs it the configured directory is `cmakebuild` and every module page judges with
  `cmake --build cmakebuild --target qor_<NAME> --parallel 8` - the file's own Results entries
  quote `cmakebuild` (e.g. the 26 Oct api_shell entry names `tmp/build_shell2.log` and
  `cmakebuild/src/...`). The rule text governs a directory the repo no longer builds.
- The fork is what the page exists to carry: an instrument that executed the rule text would build a
  `build/` tree the wiki's 91 pages do not judge, and `wiki-census coverage` would go red for
  reasons no command in the rule produced.
- The judge run 18 Nov 2026: `cmake --build cmakebuild --target qor_signals --parallel 8` exit 0,
  `qor_signals.dll` relinked - the incremental rule holds today against a core lib with five
  framework dependencies rebuilt ahead of it.
- What is not falsified here: "only the touched lib" is a habit, not a mechanism - the target
  dependency graph pulls what it pulls; nothing in `tools/wiki-*` checks that a run stayed narrow.

Nothing about the per-run entries is a rule the wiki re-states: the Results log is evidence, not the
rule (the judge vocabulary is sections L1-15 only).

## See also

[[lint]] [[test]] [[green-is-ok-and-exit-zero]] - the other three rules of the same file.
Folder rule: `docs/wiki/invariants/purpose.md`.
