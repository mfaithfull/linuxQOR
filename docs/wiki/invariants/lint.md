---
Type: WikiPage
PageType: invariant
Title: "lint"
Status: current
LastVerified: 2026-11-18
Claims: 5
Summary: "clang-tidy over the build tree is the lint rule - and as of 18 Nov 2026 it is unjudgeable on this tree: no compilation database exists in either configured directory, so the page carries its red judge openly."
Anchors:
  - { kind: in-tree, path: docs/agent/verification.md, symbol: clang-tidy, verified: 2026-11-18 }
Judges:
  - clang-tidy -p cmakebuild src/qor/essentials/current/qor_current.cpp
---

# lint

The assertion, from `docs/agent/verification.md` section "Lint": linting this repo means
`clang-tidy -p <build>` over named source files - one build-integrated analyzer run, not an
ad-hoc invocation. If it fails, style and bug-pattern claims in module pages ("0 warnings",
"C4100 sites") rest on a check nobody can re-run.

- The judge carried here is **red, and the page says so**: measured 18 Nov 2026,
  `clang-tidy -p cmakebuild src/qor/essentials/current/qor_current.cpp` exits **-1** with
  `Could not auto-detect compilation database` - there is no `compile_commands.json` in `build/`
  or `cmakebuild/`, and the Visual Studio generator the tree configures with does not emit one.
- So the rule as written cannot currently be exercised on this tree at all. The break is in the
  toolchain wiring, not in the sources - which is the difference this page names: the rule is not
  violated by code, it is unjudgeable by configuration.
- That distinction matters because the wiki already claims lint-shaped facts: module pages print
  warning counts (`0 warnings`, `3 C4100`) from **MSVC**, not from clang-tidy. The rule's analyzer
  and the evidence's analyzer are different programs, and no instrument connects them -
  `Test-WikiJudgeInVocabulary` exists but no shipped instrument calls it.
- What would make the judge green: a compilation database for the configured tree (a second Ninja
  configure, or an equivalent exported database in `cmakebuild/`). Naming the fix is not the
  rule's content, so it is not stamped anywhere; the day the database exists, the judge run is the
  re-verification.

## See also

[[build-commands]] [[test]] [[green-is-ok-and-exit-zero]] - the other rules of the same file.
The folder rule and why a page may carry a red judge: `docs/wiki/invariants/purpose.md`, and
the precedent page `docs/wiki/modules/qor_httpclient.md` (judges a never-configured target).
