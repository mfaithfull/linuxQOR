---
Type: Tool
Scripts:
  - File: tools/doc-fetch/doc-fetch.ps1
    Usage: "want slug take skip slice prefix dir - e.g. strings.cpp str 10 20 3 _tmp_u2_ c:\RAID0\Develop\linuxQOR\tmp ; want is the TU column value, slug names the report and the html cache, take/skip select the table rows in table order, slice is the explicit slice number used only to name the report; want/slug/prefix/dir are REQUIRED (no task-shaped defaults), take defaults 10, skip 0, slice 1"
    Output: "tmp/<prefix><slug>_fetch[_s<slice>].out.txt ; one <prefix><slug>_<stem>.html per fetched URL"
---

# doc-fetch

## What it does

For a slice of rows in the endpoint table, fetches each row's Learn page once
and prints that page's **Return value section verbatim**, alongside the row's
binding, published/SDK return types and in-tree caller count. It does not
decide anything: the verdict is authored from the printed section.

Three legs, each printed explicitly so a report is self-explaining:

- `FETCH` - page resolved, HTTP 200, section extracted; tagged
  `match-cache` / `differs-from-cache` against the probe cache.
- `CORRECTED` - the row's page lives under a suffixed ANSI/Unicode twin slug
  (`...exa`, `peekmessagea`, ...) and was fetched there.
- `NO-PAGE` / `FROM-CACHE` - the probe marked the row as having no page; the
  cached section is used if one exists, otherwise the row is booked with no
  docQuote and must be decided from the probe line.

## Contract

Positional args. `want` / `slug` / `prefix` / `dir` are **required** - a missing
one prints `USAGE`, `MISSING args=...`, `VERDICT fail=1` and exits `1` without
touching any file. Only the three numeric args may default:

| arg | required | default | what it changes |
|---|---|---|---|
| `want` | yes | - | the table's TU column |
| `slug` | yes | - | names the report and the html cache files |
| `take` | no | `10` | rows per slice - the per-item URL cap |
| `skip` | no | `0` | rows to skip, i.e. the leaf cursor's `slice_start` |
| `slice` | no | `1` | explicit slice number, **only** to name the report |
| `prefix` | yes | - | artifact prefix for table / probe / cache / report |
| `dir` | yes | - | where this task's artifacts live |

No path is baked into the script, and no default encodes a task: `prefix` and
`dir` name the task, so giving them a default silently writes into another
task's files and prints its own numbers, which looks like success. The run's first three report lines are
`CONTRACT`, `PATHS` and `ROWS` - the resolved argument values, the resolved
file paths, and the selected row range by stem. A report that does not name
its inputs is not this tool's output.

Exit condition: the last line is `VERDICT fail=0`, meaning every selected row
was either fetched at HTTP 200 or explicitly booked `NO-PAGE`. `bad` counts
rows that got neither, so a non-zero `bad` is a failed batch, not a partial
success.

## Provenance

Generalised 13 Nov 2026 from **eight** near-identical clones in `tmp/`
(`_tmp_u2_{tu,dialog,misc,msg,kb,str,wo,wb}_fetch.ps1`), ~98 lines each,
~780 lines of duplicate script. Only four things varied between them: `want`,
the report/html prefix, the header sentence, and the report path.

Bugs in the clones that this version fixes - each named here so the fix is not
re-found per copy:

- **The slice tag was derived, not passed.** The clones computed
  `[int]($skip / $take) + 1`, and `[int]` **rounds**: misc slice 3
  (`skip=20, take=3`) wrote `_tmp_u2_misc_fetch_s8.out.txt`, and the strings
  family left `_tmp_u2_str_fetch_s8.out.txt` on disk from the same arithmetic.
  Both files exist as measured evidence. `slice` is now an explicit argument.
- **Divergent clone bodies.** The clones had drifted apart - one probed with
  `Select-Object -Skip 3` against the probe's 4-line header, another used a
  `take + 0` division. A single body removes the drift class.

- **Task-shaped defaults.** The clones defaulted `want` to a TU and `dir` to an
  absolute `tmp/` path. A default that names one task is the same contract
  violation one level up: a later task that forgets the argument writes into the
  earlier task's artifacts and prints its own numbers, which looks like success.
  `want` / `slug` / `prefix` / `dir` are now required; only `take` (cap),
  `skip` (offset) and `slice` (report suffix) default. Measured both ways 13
  Nov 2026: `doc-fetch.ps1 strings.cpp str 3 20` printed
  `MISSING args=prefix dir`, `VERDICT fail=1`, exit `1`, and touched no file;
  the full invocation reproduced the historic
  `_tmp_u2_str_fetch_s3.out.txt` values (`bytes=52406 section_len=205
  match-cache` on `IsCharAlphaNumericA`), `VERDICT fail=0`.

Validated by re-running an already-fetched slice rather than trusting the
rewrite: `doc-fetch.ps1 strings.cpp str 3 20 <slice>` reproduced the historic
`_tmp_u2_str_fetch_s3.out.txt` rows byte-for-byte (`bytes=52406
section_len=205 match-cache` on `IsCharAlphaNumericA`, identical in both
reports), `VERDICT fail=0`.

Not yet promoted, same clone family, still in `tmp/`: the docref **probe**
(`_tmp_u2_docref_probe.ps1`, `_tmp_u2_docref_verify.ps1`, `_tmp_u2_href_probe.ps1`)
that this tool consumes. Merging probe and fetch into one leg is the open
next step - today each endpoint costs a probe request and a fetch request for
one fact.
