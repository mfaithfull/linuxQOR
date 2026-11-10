# Purpose - docs/future

Holds **plan trees**: requests charted by the `deep-planning` skill and not yet implemented. One folder per request. The index in each folder is `plan.md`; its YAML front matter carries the machine state (`Type: Plan`, `Title:`, `Parent:`, `Level:`, `Items:` with `Title`/`Status`/`Content`) and its body carries the prose (`## Goal`, `## Context`, `## Decisions`, one `## <Item Title>` per item).

- A folder is a plan; `plan.md` is its index; `Content:` points at a child folder `plan.md`, `Parent:` points back. Pointers are relative to the file holding them.
- Statuses: `Planning`, `Drafted`, `Refined`, `Atomic`, `Deferred`. A tree is done when every item in its root plan is terminal.
- Trees stay here until implemented. Retire finished work to `docs/archive/` and record locked choices as D-entries in `docs/agent/DECISIONS.md`.
- Every tree registers in the queue at `docs/agent/TODO.md` with a `[~]` entry linking its root `plan.md`.
- Format and gate are defined once in `PLAN-FORMAT.md`; this file does not restate them.
