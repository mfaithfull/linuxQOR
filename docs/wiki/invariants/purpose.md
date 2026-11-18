# Purpose - docs/wiki/invariants

The repo's rules written as **assertions** - one file per rule, each stating what must hold, what
breaks if it does not, the line that enforces it, and the command that would show it failing. The
code analogue of "name the structural claim and the position it argues against" (`docs/WikiDesign.md`
§2(d)): an invariant with no judge is a preference.

- Sources, in order of authority: the code itself, then `docs/agent/verification.md`'s rule sections
  (the judge vocabulary - a page may not invent a second notion of green), then
  `docs/skills/QORconventions.md`, then `memory-bank/gotchas/` as evidence of what breaking the rule
  already cost.
- Every page carries `Judges:` drawn from that vocabulary: a build target, a test binary, a
  `dumpbin` call, a `clang-tidy` run. `tools/wiki-lint` rejects a judge it cannot find there.
- Naming is the rule's shape, not its number: `configuration-first-include.md`, not `rule-7.md`, so
  the file name survives the day the list is reordered.
- Not here: prose about how a subsystem works (`modules/`), a restatement of a D-entry (link the
  D-entry, `docs/agent/decisions/`), or a wish about how the code should be (`docs/future/TODO.md`).
