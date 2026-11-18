---
Type: Plan
Title: "Invariant pages"
Parent: "../plan.md"
Level: 3
Items:
  - Title: Probe the rule sections and judges
    Status: Atomic
    Content:
  - Title: Write the build-commands invariant page
    Status: Atomic
    Content:
  - Title: Write the lint invariant page
    Status: Atomic
    Content:
  - Title: Write the test invariant page
    Status: Atomic
    Content:
  - Title: Write the green-rule invariant page
    Status: Atomic
    Content:
  - Title: Rebuild the index and close the pass
    Status: Atomic
    Content:
---

# Invariant pages

## Goal

Four pages, one per rule section of `docs/agent/verification.md` L1-15 - each states the repo's
verification rule as an assertion and carries the command that would show it violated, so the wiki
can be made red against the rules the build already runs by.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: CLOSED 18 Nov 2026` - all 6 `Atomic` leaves landed: probe, `build-commands`, `lint`,
  `test`, `green-is-ok-and-exit-zero`, index close. Per-leaf stops printed `  invariants = 1..4`
  with `illegal_fields = 0` and `dead = 0` throughout.
- **Close line, 18 Nov 2026** (reports `tmp/_tmp_iv_*.txt`): `wiki-index build` ->
  `index_rows = 95 of 95`, `unindexed = 0`, `groups = 3` (gotcha, invariant, module), index.md 110
  lines; `wiki-census pages` -> `pages = 95`, `  invariants = 4`, `strays = 0`;
  `wiki-lint schema` -> `illegal_fields = 0`; `wiki-ref-check anchors` -> `anchors = 461 of 461`,
  `dead = 0`, `unpinned = 0`; `wiki-lint provenance` -> `violations = 2` (unchanged pre-existing
  pair); `wiki-lint freshness` -> `judges = 95 evidenced = 3 undated = 92 unstamped = 0 stale = 0`
  - the wiki's first **dated** pages (`test`/`green` via the exe artifact, `lint` via the .cpp);
  each `VERDICT fail=0`.
- **Findings carried out** - (1) the lint page's judge is **red by configuration**: no
  `compile_commands.json` in `build/` or `cmakebuild/`, clang-tidy exits -1; stated on the page per
  the `qor_httpclient` precedent, not hidden. (2) the `## Test` section's own example exe
  (`QORTestMIME.exe`) is not configured - only 3 `QORTest*.exe` exist on disk (measured), so the
  green ladder's real coverage is 3 binaries; the `test` page carries the list. (3) artifact
  extraction is punctuation-fragile: a judge ending `; $LASTEXITCODE` folds `;` into the path token,
  `Test-Path` fails, and the page silently lands `undated` (`green` measured unevidenced with it,
  evidenced without) - same family as the gotchas' right-count-empty-content signature; the green
  judge is now the bare exe run and the exit-code check is prose. (4) the rule file's head
  `2026-10-26 54116800` and the `build` vs `cmakebuild` / `-j4` vs `--parallel 8` fork are recorded
  on the `build-commands` page.
- **Probe slot** (nine values, three per id, measured 18 Nov 2026, all symbols resolved by
  `Test-WikiSymbolInFile` over `docs/agent/verification.md`, head `2026-10-26 54116800`):
  - `build-commands` (rule L6-8) - symbol `incremental`; judge
    `cmake --build cmakebuild --target qor_signals --parallel 8` (head `cmake --build`; ran 18 Nov
    2026, exit 0, `qor_signals.dll` relinked); cost now: the rule text names `build` and `-j4` but
    the live configured tree is `cmakebuild` and all 91 module pages judge with `--parallel 8` -
    the rule's own text is stale against the tree it governs.
  - `lint` (rule L10-11) - symbol `clang-tidy`; judge
    `clang-tidy -p cmakebuild src/qor/essentials/current/qor_current.cpp` (head `clang-tidy`; ran
    18 Nov 2026, **exit -1**: `Could not auto-detect compilation database`, no `compile_commands.json`
    in `build/` or `cmakebuild/`); cost now: the rule cannot currently be judged on this tree at
    all - the judge is red for the tree's generator, not for the code.
  - `test` (rule L13-14) - symbol `Test`; judge
    `& build\test\uri\Debug\QORTestURI.exe` (head `test-exe`; ran 18 Nov 2026, `[OK](1/1)`, exit 0);
    cost now: the rule's own example `build\test\framework\data\mime\Debug\QORTestMIME.exe` is **not
    configured** - only 3 `QORTest*.exe` exist on disk (`jsongenerator`, `yaml`, `uri`, measured
    18 Nov 2026), so the green ladder runs over 3 binaries, not the whole test tree.
  - `green-is-ok-and-exit-zero` (rule L15, the green definition under `## Test`, not its own `##`)
    - symbol `green`; judge the same exe run plus `$LASTEXITCODE`; cost now: the two-condition rule
    (`[OK](n/n)` AND exit 0) is the only definition of green in the repo and no instrument checks
    the pair - `wiki-lint freshness` reads the stamp, never the exit code.

## Context

Inherited at creation - re-read rather than trusted, the tree has moved since:

- **Parent goal**: pages that carry a falsification handle rather than a description.
- **Parent locked decisions**: the judge vocabulary is the four rule sections (L6/L10/L13/L15),
  **not** the 129 Results-log bullets below L17; two catalogs that never sync.
- **Sibling plan titles**: `GotchaPages` (closed 18 Nov 2026 - its leaf order, close line and
  instrument invocations are the worked example for this plan), `DecisionPages` (`Refined`).
- **Folder purpose already exists**: `docs/wiki/invariants/purpose.md` (landed by the scaffolding
  pass). Its L13 asserts "tools/wiki-lint rejects a judge it cannot find there" - **measured false**:
  `Test-WikiJudgeInVocabulary` (`tools/wiki-census/wiki-shape.ps1`:533) is called by no shipped
  instrument. Recorded, not edited: it is an artifact of a closed item, and fixing the assertion is
  a governance action, not a leaf in this pass.

## Decisions

- **Why this `Atomic` item was cut (18 Nov 2026)** - the same three gate rows GotchaPages failed:
  four page writes plus four anchor lookups plus four judge lookups plus instrument passes (kinds
  past 1:1), against the tree's own 16 Nov 2026 rule that a page-bearing leaf writes **exactly one
  page**; and the item's stated inputs were wrong - the canonical test exe does not exist and the
  probe was never run. Cut from `Atomic` to 6 `Atomic`: probe, one leaf per page, index close.
- **No `pin:` on these anchors** - the parent's item prose said "with its `pin:` at the current
  commit"; `Resolve-WikiAnchor` requires pins only for `out-of-tree`, and `docs/agent/verification.md`
  is in-tree (`wiki-ref-check anchors` counts `unpinned` from out-of-tree only). The head
  `2026-10-26 54116800` goes in the slot, not the page - the same F11 reasoning as
  `GotchaPages`' third decision.
- **A red judge stays on the page and is named** - `qor_httpclient` is the precedent: the lint
  page's judge fails today for a tree reason (no compilation database), and the page says so;
  hiding the failure inside a page that claims a working lint rule would make the wiki assert a
  run nobody can reproduce.
- **The stale rule text is the wiki's to carry** - `build` vs `cmakebuild`, `-j4` vs `--parallel 8`,
  the MIME example that is not configured: these are the fork the invariant pages exist to name,
  so each page states the rule **as the file says it** and **as the tree runs it**.

## Probe the rule sections and judges

Input verified: `docs/agent/verification.md` exists (529 lines, rules at L6/L10/L13/L15); both
configured trees exist.

Done 18 Nov 2026 by `tmp/_tmp_iv_probe.ps1`: four `symbol:` strings resolved (not invented) against
the file; four judges taken from the rule sections, then each **run** and its result recorded in the
probe slot above; heads mapped through `Get-WikiJudgeHead` (`cmake --build`, `clang-tidy`,
`test-exe`).

Stop condition printed: `symbol_anchors_core = 5 of 5` (five symbols tested, four used) plus the
per-judge run results in the slot.

## Write the build-commands invariant page

Input verified: probe slot row 1; `qor_signals` judge ran green 18 Nov 2026.

One page: `docs/wiki/invariants/build-commands.md` - `Type: WikiPage`, `PageType: invariant`,
`Status: current`, `LastVerified: 2026-11-18`, anchor
`{ kind: in-tree, path: docs/agent/verification.md, symbol: incremental, verified: 2026-11-18 }`,
`Judges:` the slot's cmake line; body asserts the one-lib-increment rule, states the as-written /
as-run fork, and points at the file without restating the Results log.

Stop condition: `wiki-census pages` prints `  invariants = 1`; `wiki-lint schema`
`illegal_fields = 0`; `wiki-ref-check anchors` `dead = 0`, `unpinned = 0`.

## Write the lint invariant page

Input verified: probe slot row 2; the judge's exit -1 and its exact error text measured 18 Nov 2026.

One page: `docs/wiki/invariants/lint.md`, same shape, symbol `clang-tidy`; body carries the
unjudgeable-today finding head-and-shoulders - the command, the exit, the missing
`compile_commands.json` in both configured trees - because an invariant page that hid a red judge
would be asserting a rule it cannot falsify.

Stop condition: `  invariants = 2`, `illegal_fields = 0`, `dead = 0`.

## Write the test invariant page

Input verified: probe slot row 3; `QORTestURI.exe` ran green 18 Nov 2026.

One page: `docs/wiki/invariants/test.md`, same shape, symbol `Test`; body asserts the run-the-exe
rule, names the three configured exes against the rule's MIME example, and states that no instrument
enforces "the QOR does not use ctest" - freshness of the test tree is `wiki-census` and disk, not a
checker.

Stop condition: `  invariants = 3`, `illegal_fields = 0`, `dead = 0`.

## Write the green-rule invariant page

Input verified: probe slot row 4; the `[OK](1/1)` + exit 0 pair measured 18 Nov 2026.

One page: `docs/wiki/invariants/green-is-ok-and-exit-zero.md`, same shape, symbol `green`; body
asserts the two-condition definition, names that it is the only definition of green in the repo and
that the pair is checked by no shipped instrument - which is why the judge is the run itself.

Stop condition: `  invariants = 4` - the census against the four enumerated rule sections - with
`illegal_fields = 0`, `strays = 0`, `dead = 0`.

## Rebuild the index and close the pass

Input verified: `  invariants = 4` printed by the leaf above.

One generator run and two slot writes: `wiki-index build` (generated file, op 12 - never edited);
expected print `index_rows = 95 of 95` with `groups = 3` (`gotcha`, `invariant`, `module`); this
plan's close line goes into this slot and the parent's, and the `freshness` line is re-read because
the `test`/`green` judges name a file that exists, so `evidenced` must move off 0 - the first
dated pages in the wiki.

Stop condition: `wiki-index build` prints `index_rows = 95 of 95` with `unindexed = 0`, and
`wiki-census pages` prints `pages = 95` with `  invariants = 4`.
