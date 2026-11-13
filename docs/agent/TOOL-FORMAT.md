# Tool format

A **Tool** is a reusable script (or set of scripts) that the agent runs to do a
task. Tools live in the code tree under `tools/<tool-slug>/`; their docs live
under `memory-bank/tools/<tool-slug>/tool.md`. `memory-bank/tools.md` indexes
the docs.

## Why this shape exists

`tmp/` is gitignored (`.gitignore:41` `/tmp`) and is cleaned per task. Measured
13 Nov 2026: `tmp/` held **3,878 files**, **782** of them `.ps1` scripts,
**155** of those from one pass. Scripts left in `tmp/` are therefore neither
versioned, nor findable, nor durable. A tool that is copied out of `tmp/` is
not a tool.

## Directory layout

```
tools/<tool-slug>/<script>.<ext>          # the script - tracked, persistent
memory-bank/tools/<tool-slug>/tool.md     # its doc - indexed by tools.md
```

`tools.md` `File:` and `tool.md` `File:` paths are resolved as written: `File:`
in `tools.md` is relative to `memory-bank/`, `File:` in a `tool.md` script entry
is relative to the repo root. Both are stated explicitly in `tools.md`'s
`Description` so the two never have to be inferred.

## `memory-bank/tools.md` (Type: Index)

```yaml
---
Type: Index
Description: "Index of reusable tools. File: paths are relative to memory-bank/. Script File: paths inside a tool doc are relative to the repo root."
index:
  - UseWhen: "what you want to do, phrased generally"
    Scope: "the area of the repo the tool serves, or 'repo-wide' / 'external'"
    File: "tools/<tool-slug>/tool.md"
---
```

- `UseWhen` is the retrieval key. Write it **generally** ("fetch a Learn API
  page and extract its Return value section"), never per task ("misc slice 2 of
  the U2 pass"). A per-task entry recreates the debris it was meant to avoid.
- `Scope` names what the tool touches so a tool is not applied to the wrong
  tree.
- Keep the key `index:` lower-case, matching `memory-bank/gotchas.md`.

## `memory-bank/tools/<slug>/tool.md` (Type: Tool)

```yaml
---
Type: Tool
Scripts:
  - File: tools/<slug>/<script>.ps1
    Usage: "the arguments, in order, with what each one changes"
    LastRun: <ISO date>
    Output: "where the report lands"
---

# <slug>

## What it does
## Contract
## Provenance
```

Required sections and what each must record:

- **What it does** — the capability, in general terms, so the `UseWhen` lookup
  has something to match.
- **Contract** — required args and defaults, plus the **resolved paths the run
  writes**. A script with a baked absolute path writes into the previous task's
  file and prints its own numbers, which looks like success. Any path that is
  not an argument is a contract violation.
- **Provenance** — where the script came from, and **the bugs it fixes**.
  Promoting an instrument out of `tmp/` is a review, not a copy: a bug present
  in three copies must be fixed once, in the promoted copy, and named here.
- `LastRun` advances only on a run that finished with the report's own verdict
  line at `fail=0`.

## Rules

- One tool per capability. Do not clone a script and change two constants -
  parameterise the two constants. The clone families in `tmp/` (`<slug>_fetch`
  × 8, `<x>_callers` × 10, `gate_*` × 5) are exactly what this format forbids.
- Reports go to `tmp/` (disposable). Scripts never do.
- A tool whose capability is superseded moves to `memory-bank/tools/archive/`,
  it is not deleted, so provenance survives.
- Harvesting `tmp/` means promoting the **general** form of a family, not the
  family's members. Registering 782 entries is as unreliable as the listing it
  replaces.
