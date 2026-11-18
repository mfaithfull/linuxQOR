# WikiDesign - an LLM-maintained wiki for code understanding

Date: 14 Nov 2026. Scope: a design, not yet an implementation. This file instantiates
[Andrej Karpathy's LLM Wiki idea file](https://gist.github.com/karpathy/442a6bf555914893e9891c11519de94f)
("LLM Wiki - example of an 'idea file'",
[HN 47640875](https://news.ycombinator.com/item?id=47640875), 296 points / 95 comments)
for this repository: a 250,000-line C++20 tree where the material to be ingested is
**code that churns**, not articles that sit still.

Note on the path: the request said `doc/WikiDesign`; this repo has no `doc/` folder and keeps
all durable prose under `docs/` (`docs/structure.md`, `docs/future/`, `docs/skills/`). The
design therefore lands at `docs/WikiDesign.md`. If a literal `doc/` is wanted, move the file -
nothing in this design depends on its location.

The document's job is to answer three questions:

1. How to prompt for it - a paste-ready idea file ([§10](#10-the-prompt-paste-ready)).
2. Which wiki operations belong in skills, which in tools, which in hooks ([§8](#8-skills-vs-tools-vs-hooks)).
3. How maintenance is automated, and how it fails when it is not ([§9](#9-maintenance-automation), [§12](#12-failure-modes-and-guards)).

The five open decisions were settled in §13 on 14 Nov 2026; what those answers lock in is rolled out
in [§13.1](#131-what-those-five-answers-lock-in), and the paths throughout this document already
reflect them (`docs/raw/`, `docs/wiki/`, `docs/wiki/schema.md`, target-granularity, tracked,
ingest-commit cadence).

---

## 1. The pattern, as stated

Karpathy's gist states three things and leaves the rest abstract ("This document is
intentionally abstract... The exact directory structure, the schema conventions, the page
formats, the tooling - all of that will depend on your domain").

**The core difference from RAG.** RAG rediscovers knowledge per query: nothing accumulates.
The wiki instead is "a persistent, compounding artifact" - "the cross-references are already
there. The contradictions have already been flagged. The synthesis already reflects
everything you've read." Knowledge is "compiled once and then *kept current*, not re-derived
on every query."

**Three layers** (his words: Raw sources / The wiki / The schema):

| Layer | Karpathy's rule | Who owns it |
|---|---|---|
| Raw sources | "your curated collection of source documents... These are immutable - the LLM reads from them but never modifies them. This is your source of truth." | human curates |
| Wiki | "a directory of LLM-generated markdown files... The LLM owns this layer entirely." | LLM writes, human reads |
| Schema | "a document (e.g. CLAUDE.md...) that tells the LLM how the wiki is structured, what the conventions are, and what workflows to follow... the key configuration file" | human + LLM co-evolve |

**Three operations**: Ingest (drop a source; the LLM reads it, writes a summary page, updates
the index, updates entity/concept pages across the wiki, appends to the log - "a single source
might touch 10-15 wiki pages"), Query (search, read, synthesise with citations; "good answers
can be filed back into the wiki as new pages"), Lint (health-check for "contradictions between
pages, stale claims that newer sources have superseded, orphan pages with no inbound links,
important concepts mentioned but lacking their own page, missing cross-references, data gaps").

**Two navigation files**: `index.md` is content-oriented ("a catalog of everything in the wiki -
each page listed with a link, a one-line summary") and `log.md` is chronological, append-only,
with a consistent entry prefix so it greps. At ~100 sources / hundreds of pages the index alone
suffices; past that, add real search ([qmd](https://github.com/tobi/qmd) - BM25 + vectors +
LLM re-rank, on-device, CLI and MCP).

**Why it works** (his words): "the tedious part... is the bookkeeping. Humans abandon wikis
because the maintenance burden grows faster than the value. LLMs don't get bored, don't forget
to update a cross-reference, and can touch 15 files in one pass." Related in spirit to Vannevar
Bush's Memex (1945) - the part Bush could not solve was "who does the maintenance".

## 2. What changes when the sources are code

Four properties of code invalidate the article-shaped assumptions. Each changes a rule below;
the deltas are load-bearing, not cosmetic.

**(a) Code is already the source of truth, and it is mutable.** Copying it into `raw/` creates
a second copy of the truth that drifts silently. Karpathy's raw layer is immutable *because it
is a copy*. Here the source is not a copy - it is the thing itself, versioned by git. So `raw/`
holds **references, never copies** (§4). This repo has learned the same lesson twice:
`docs/agent/TOOL-FORMAT.md` records that `tmp/` is gitignored (`.gitignore:41`) and cleaned per
task - measured 13 Nov 2026 at 3,878 files / 1.07 GB, 782 of them `.ps1` - and that "Scripts left in
`tmp/` are therefore neither versioned, nor findable, nor durable" plus "Reports go to `tmp/`
(disposable). Scripts never do.", because the 566 comment runs that cited
`tmp/_tmp_u2_endpoint_table.csv` became the only surviving copy of that reasoning once `tmp/`
was emptied. A reference that cannot be re-resolved is the same bug one layer up.

**(b) A query about code has a checkable answer.** "What does this say?" becomes "where is this
defined, what calls it, what breaks if I change it, is this still true?" - each with a mechanical
judge: the compiler, the linker, `dumpbin /exports`, ctags, a test's `[OK](n/n)` plus exit 0
(`docs/agent/verification.md`, decision D3 "compiler-as-judge"). Wiki claims must therefore
carry a **falsification handle**, not only a citation.

**(c) The corpus churns under the wiki.** A page about code has a validity interval, not a mood.
Staleness must be machine-detected (anchors + fingerprints + commit compare), never LLM-guessed.
The `understand-anything` plugin already installed in this workspace solves the hard half of that,
and its shape is the one to copy: a `meta.json` commit baseline, structural fingerprints, a plan
whose actions are `SKIP / PARTIAL_UPDATE / ARCHITECTURE_UPDATE / FULL_UPDATE`, and a cost rule
quoting its own auto-update prompt - "deletion-only, ignored-only, generated-artifact-only, and
cosmetic-only updates dispatch no LLM agent".

**(e) Ingestion consumes its reference.** Karpathy's raw layer is a *stock*: sources sit in it for
ever, and after ingestion the copy is archived so the reading is preserved. Ours is a *queue*. A
reference here is a pointer into the thing that already exists, so preserving it preserves nothing -
ingest deletes the entry once its anchor has been carried into a page, and **`docs/raw/` is empty
when the wiki is current** (§4.4). That inversion is what retires the archive step, and it moves the
archive from the sources to the pages: a superseded *page* still has content worth keeping, so it
still archives (`docs/wiki/archive/`, per `TOOL-FORMAT.md`); a superseded *reference* does not.
The rule is only safe because the page carries the anchor it consumed - deleting a reference no
page cites is data loss, and it is a counted failure, not a policy hope (F12, §12).

**(d) Ungoverned compilation averages.** Hand a compiler 300 files with no governance and it
returns "consensus" - encyclopedic summaries that have lost the reading ([Jonadas, "Beyond
Karpathy's LLM-Wiki"](https://www.jonadas.com/writing/essays/beyond-karpathys-llm-wiki)); the
sharpest HN objection, from `qaadika`, is that "there's nothing 'personal' about a knowledge base
you filled by asking AI questions" - the bookkeeping the LLM automates is "exactly where genuine
understanding forms". For code the same failure appears as *plausible prose about code*: a page
that reads well but does not match the tree. The counterweight is not "do the bookkeeping by hand"
- it is governance that makes a claim refutable. The code analogue of Jonadas' friction-point
protocol ("extract the central structural claim; name the position it argues against; link to at
least two existing nodes; never summarise neutrally") is: **name the invariant, name what breaks
it, cite the line that enforces it, name the build or test that would show it failing.**

## 3. Layers

```
docs/raw/    references only, awaiting ingestion - a queue of pointers into the repo, the web,
             git history and other documents. Emptied by ingest; empty means nothing is un-compiled
docs/wiki/   LLM-written prose about the code; every claim carries an anchor + a falsification
             handle. The consumed references live here, as frontmatter, forever
docs/wiki/schema.md  the conventions + workflow file the agent re-reads every session (the governance)
```

The three layers are unchanged from Karpathy; what changed is the *content* of `raw/` (pointers,
not documents), the *contract* of `wiki/` (claims are refutable), and the *lifecycle* of `raw/`
(a queue that ingest consumes, not a stock that ingest archives - §2(e), §4.4). Two extra layers exist in
this tree and must not be duplicated - they are cited, never restated:

| Existing thing | Role | Wiki's relation to it |
|---|---|---|
| `docs/agent/` (`DECISIONS.md`, `verification.md`, `repo_map.md`, `ctags/`, `deps.dot`) | mutable agent state + deterministic indexes | `raw/` references them; wiki pages link the D-entry, they do not restate it (`docs/future/PLAN-FORMAT.md` already mandates "linked to `docs/agent/DECISIONS.md` D-entries rather than restated") |
| `memory-bank/` (`activeContext.md`, `gotchas/`, `tools/`) | the session-state substrate and the tool index | `raw/` references `gotchas.md` entries as *evidence of what breaks*; never a page of its own |

The wiki holds the understanding, invariants, decisions, traps, and the tour that
makes a 250k-line tree readable.

## 4. `docs/raw/` - references only, awaiting ingestion

The hard rule, stated once and enforced by an instrument:

> **`docs/raw/` contains pointers and annotations. It contains no copied prose, no pasted code,
> no screenshots, no summaries.** The only text in the queue is (i) an anchor that resolves,
> (ii) an annotation of at most three lines saying what the reference is good for, and
> (iii) a verification stamp. Entries are deleted when ingested (§4.4); nothing is archived here.

This is the shape this repo already uses for reference sets: `docs/learning/RCU/RESOURCES.md`
lists each source with a one-line "Use for:" and a "Fetched and verified 2026-10-24" stamp, and
the `teach` skill's `RESOURCES-FORMAT.md` pins the rules "Annotate every entry. A bare link is
useless in three months", "Prune ruthlessly. Better five sharp sources than thirty mediocre
ones", and "Surface gaps explicitly. If no good resource exists for an area the mission needs,
write a `## Gaps` section listing what is missing. This drives future search." Adopt all three;
add the fourth rule this repo needs and `teach` does not have: **an anchor must be re-resolvable
after the fact.** The repo already enforces the adjacent half of that rule - `docs/agent/TOOL-FORMAT.md`
says "Scripts left in `tmp/` are therefore neither versioned, nor findable, nor durable" - and the
wiki's anchors are the same kind of pointer, one layer up.

### 4.1 Layout

```
docs/raw/
  purpose.md              # what this folder is; repo rule: every new folder needs one
  pending.md              # the queue: one entry per reference awaiting ingestion
```

One queue file, not one file per reference: a single `pending.md` keeps the queue greppable (the
pattern's own claim is that catalog-style indexes work to hundreds of entries) and makes the whole
manifest one instrument's input. A reference that needs prose longer than three lines belongs in
`docs/wiki/` as a page that *cites* the reference. The file is named for what it holds: it is a
list of work, and it is emptied by the op that does the work (§4.4).

### 4.2 Entry format

```yaml
- id: sdk-user32-ntddi
  kind: in-tree            # in-tree | out-of-tree | event | artifact
  anchor: "src/platform/os/windows/api_layer/kernel/kernel32.h#_WIN32_WINNT"
  pin: "SDK 10.0.26100.0; _WIN32_WINNT=0x0A00; NTDDI_VERSION=0x0A000010 (D21, kernel32.h:22-30)"
  use_for: "which exports exist at all; the authority for a wrapper's signature"
  verified: 2026-11-13
  judge: "cl /EP of the TU - audit bindings from preprocessed output, never source text (D13)"
  notes: "shrink macros (NOGDI/NOUSER/...) are NOT pinned and still decide the surface"
  added_by: human          # human | commit-hook | gapfill | session
  closes: ""               # the gaps.md row this reference would close, if any
```

`kind` decides what the re-resolution instrument does with it:

| kind | anchor form | re-resolution | example |
|---|---|---|---|
| `in-tree` | `path`, `path:line-line`, `path#symbol`, `commit:sha`, `D<n>`, CMake target id | re-open the file; re-grep the symbol; `git show sha` | `src/qor/dependency.md`, `docs/agent/DECISIONS.md#D13`, `git:99c8c20c` |
| `out-of-tree` | URL | HEAD + content fingerprint; record fetch date | MS Learn API page, DeepWiki, a paper |
| `event` | session id, PR number, chat thread, `docs/future/<tree>/plan.md` | re-read the artifact; it is append-only, so it never drifts | this session's decisions |
| `artifact` | generated file: `.ua/knowledge-graph.json`, `docs/agent/ctags/*.tags`, `deps.dot`, `docs/agent/repo_map.md` | re-generate; compare fingerprint | the graph's `gitCommitHash` field |

### 4.3 Anchor rules (the anti-rot contract)

These four rules are the answer to "how do references survive a decade of churn".

1. **Prefer named anchors to numeric ones.** `kernel32.h#SetTerminatingWindow` beats
   `kernel32.h:412`. A line number is a *derived* anchor: it may be recorded, but it may not be
   the only anchor, because every insert above it invalidates it silently.
2. **Every anchor carries a verification stamp and a judge.** "Fetched and verified 2026-10-24"
   is the repo's existing convention; extend it with the command that would re-establish the
   claim, so a stale reference is *detectable*, not plausible.
3. **No scratch paths.** `tmp/` is gitignored and deleted per task. An anchor into `tmp/` is a
   contract violation, and this repo has already been bitten: the stale-`tmp/` citation was the
   proximate cause of the `SourceCommentShadowTree` debt recorded in `docs/future/SourceCommentShadowTree/plan.md`
   citing per-session files such as `memory-bank/activeContext.md` carries the same risk
4. **Version-pin what the reference implies.** A citation to an SDK header is only reproducible
   with the SDK triple; that is why decision D21 pins `_WIN32_WINNT`/`WINVER`/`NTDDI_VERSION` in
   `kernel32.h`. Any `out-of-tree` reference whose answer depends on a toolchain version carries
   that version in `pin:`.

**Enforcement** is a tool, not a policy note: `tools/wiki-ref-check` walks every entry, re-resolves
each anchor, and prints one line per kind plus a verdict in this repo's house style -
`in_tree=312 resolved=305 rotted=7  out_tree=44 live=41 moved=3  tmp_refs=0  VERDICT fail=1`.
A design rule that is not printed by an instrument is a rule nobody follows (§8, §9).

### 4.4 What ingestion does to a reference: it consumes it

Karpathy's raw layer is a *stock* - sources accumulate in it, and after ingestion a copy is moved to
an archive so the reading is preserved. That step exists in his design because his raw entry **is**
the reading: the prose is in the layer, so deleting it would delete the evidence. Ours is a *queue*,
because a raw entry is a pointer into the thing that already exists and is already versioned. There
is no reading in the layer to preserve, so there is nothing to archive.

```
proposed   a human drops a reference, gapfill proposes candidates, or the post-commit hook
           enqueues the targets a commit touched
  -> pending     the entry sits in docs/raw/pending.md; the wiki has not read it
  -> ingested    wiki-ingest reads through the anchor and writes claims that carry that anchor
  -> consumed    the entry is DELETED from pending.md. Nothing moves. Nothing is renamed.
```

Four consequences, each load-bearing:

1. **`docs/raw/` being empty is the invariant, not tidiness.** A non-empty queue names exactly what
   the wiki has not yet compiled; an empty one is the state the wiki may claim to be in. This is the
   one place the design can say "up to date" without an opinion, and it is the reason the folder
   exists at all: it is a to-do list whose items are references, not a library of them.
2. **There is no archive for references, and none is wanted.** Nothing is destroyed at consumption
   because the entry's anchor, `pin:` and `verified:` stamp are copied *into the page that consumed
   them* - the page is the archive. The archive rule therefore applies to pages only
   (`docs/wiki/archive/`, §7 `retire`): a superseded page still holds prose worth reading, a
   superseded reference held only a pointer.
3. **Re-verification does not re-open the queue.** A page already carries the anchors it was built
   from, so `refresh` and `wiki-reconcile` re-resolve them against the current tree with no raw
   entry in sight. Entries in `docs/raw/` are only ever *new* knowledge. Keeping re-verification out
   of the queue is what keeps the queue bounded: it cannot be refilled by work that has already been
   done.
4. **Provenance survives the deletion twice over.** The entry's `id` is named in the append-only
   `docs/wiki/log.md` line for the ingest (`ingest-ref | sdk-user32-ntddi -> 6 pages`), and git's
   history of `docs/raw/pending.md` holds the deleted text itself - both are free, per §13's answer
   that the wiki is tracked.

The consume step gets its own instrument line, because it is the one step in the design that can
lose information:

```
pending=0 consumed=7 carried=7 uncarried=0 VERDICT fail=0
```

`uncarried>0` says an ingest deleted a reference that no page cites - the same data loss the stale
`tmp/_tmp_u2_endpoint_table.csv` citation caused at this repo's larger scale, caught at the moment it
happens instead of a month later. `wiki-ingest` must print the pages it wrote *before* deleting the
entry, so the human can see the carry; the deletion is the last thing the op does.

## 5. `docs/wiki/` - the page taxonomy and the claim contract

```
docs/wiki/
  purpose.md              # what this folder is
  schema.md               # the governance - the one file every op re-reads (was schema/)
  index.md                # content catalog, generated (§6)
  log.md                  # append-only operation log, one prefix per op
  meta.md                 # baseline: <sha>, graphHash: <sha>, lastDrain: <verdict>
  gaps.md                 # open questions and missing coverage - drives future ingest
  architecture.md         # the layering, in prose the graph cannot carry
  dependencies.md         # what depends on what, and why the edge direction is load-bearing
  tour.md                 # the onboarding path: 7 stops, each one page
  archive/                # superseded PAGES only - references are consumed, not archived (§4.4)
  modules/<target>.md     # one page per QOR lib target (measured 14 Nov 2026: 91 `set(target qor_` declarations / 90 distinct)
  symbols/<name>.md       # only symbols that earned a page - see the significance rule
  concepts/<name>.md      # DI, smart references, pipeline filters, Flyers, macro resolution
  invariants/<name>.md    # a claim that must hold + its falsification handle
  decisions/D<n>.md       # a pointer page into docs/agent/decisions/D<n>.md, never a restatement
  gotchas/<name>.md       # a pointer page into memory-bank/gotchas/, plus the code it now costs
  questions/<date>-<slug>.md   # filed answers (Karpathy: "good answers can be filed back")
```

Two repo rules shape this: **the Rule of 7** ("about 7 subfolders/files/classes/namespaces per
container") caps the folder fan-out above at seven *category* kinds - `archive/` is a lifecycle
folder, not a category, and if the count strains the rule it may live in the existing `docs/archive/`
instead - and **every new folder needs `purpose.md`** (`memory-bank/systemPatterns.md`).

### 5.1 Frontmatter = machine state; body = prose

Same discipline the plan trees already enforce (`docs/future/PLAN-FORMAT.md`: "YAML front matter
carries machine state only. The markdown body carries the prose"). It is also what makes lint
deterministic instead of an LLM reading every page.

```yaml
---
Type: WikiPage
PageType: module            # module | symbol | concept | invariant | decision | gotcha | question
Title: "qor_pipeline - Sink/Source filters"
Anchors:                    # what this page is *about*; see §4.3
  - {kind: in-tree, path: src/framework/data/pipeline, symbol: qor::pipeline::Filter, verified: 2026-11-08}
Sources:                    # ids of references this page consumed - the entries themselves were
  - cmake-pipeline-target   # deleted from docs/raw/pending.md when they were carried here; these
  - learn-pipeline-doc      # ids are the archive, and wiki-ref-check re-resolves what they point at
GraphNodes:                 # ids into .ua/knowledge-graph.json; the wiki sits above the graph
  - "file:src/framework/data/pipeline/filter.h"
Status: current             # current | stale | superseded | draft
SupersededBy: ""
Claims: 12
LastVerified: 2026-11-08
Judges:                     # how this page is falsified, as commands
  - "cmake --build cmakebuild --target qor_pipeline --parallel 8"
  - "tools/wiki-invariant-check --page ./docs/wiki/invariants/configuration-first.md"
---
```

### 5.2 The claim contract

Every non-navigational sentence in a `wiki/` body that asserts something about the code must
carry an inline anchor marker, and every `invariant` page must carry a judge:

```md
A QOR TU that omits `configuration.h` as its first include fails to build
(`src/configuration/configuration.h:33`, judge: `cmake --build cmakebuild --target qor_<NAME>`
exits non-zero with `C2065` on the omitted include - verified 2026-11-08).
```

Three consequences, and each is a lint:

- **No anchor, no claim.** A sentence about code with no anchor is a *gossip* claim; lint counts
  them (`claims=12 anchored=11 unanchored=1 VERDICT fail=1`) and the unanchored count is the page's
  debt. This is the guard against failure (d) above: ungoverned compilation produces plausible
  unanchored prose.
- **An anchor that no longer resolves makes the page stale**, not silently wrong: `Status: stale`,
  and `gaps.md` gains a row.
- **A contradiction between two pages is only interesting if both sides are anchored.** Karpathy's
  lint looks for "contradictions between pages"; here it looks for *contradictions between anchors*,
  which is decidable - two pages citing the same symbol with different claims, or a page claiming a
  symbol the graph says does not exist.

### 5.3 Page-significance rule (the god-node guard)

Vault Operator's health check flags "God nodes: hub notes with far more connections than they can
usefully organize" (threshold fixed at fifty connections). The code analogue: **a symbol earns its
own page only when it is the subject of at least three claims or is a hub in the graph above the
median in-degree.** Everything else stays a section inside its module page. A wiki with a page per
symbol in a 250k-line tree is a graph, not a wiki - it duplicates `.ua/knowledge-graph.json` at
higher cost and lower quality.

## 6. Navigation files

`index.md` is content-oriented (page + one-line summary, grouped by `PageType`), `log.md` is
append-only with the prefix convention Karpathy recommends so it greps -
`## [2026-11-14] ingest-ref | sdk-user32-ntddi -> 6 pages touched`. `gaps.md` is this design's addition to Karpathy's two navigation files: where open questions
and missing coverage live, and the *input* to the next ingest. It exists because the wiki must be
able to admit it does not know. A page that fills a gap with plausible prose is the failure mode
this design exists to prevent, and `gaps.md` is where the deficit is named instead of papered over
- the same discipline as the "Surface gaps explicitly" rule in `teach/RESOURCES-FORMAT.md`.

`index.md` is generated, not hand-maintained. Karpathy has the LLM update it on every ingest; at
this repo's scale (33 QOR libraries, hundreds of pages) a hand or LLM edit of the catalog is one more
place to drift. `tools/wiki-index` regenerates it from frontmatter and prints
`pages=214 indexed=214 unindexed=0 VERDICT fail=0`. The LLM writes the one-line summaries into
frontmatter (`Summary:`), the tool writes the catalog.

## 7. Operations catalogue

Every op has one owner (§8 decides *which* owner) and one stop condition printed by an instrument,
because `docs/future/PLAN-FORMAT.md` already requires it: "every `Atomic` item states, in its
section, the one check that closes it - an instrument line or a re-read that prints a count."

| # | Op | Trigger | In | Out | Kind | Cost | Gate |
|---|---|---|---|---|---|---|---|
| 1 | **seed** | explicit, once | repo tree, `docs/agent/repo_map.md`, `.ua/knowledge-graph.json` | `docs/wiki/` skeleton + first module pages for the highest-churn libs | LLM over deterministic structure | one-time, ~$0.50-1.00/project (measured in the working implementation, §14 refs) | human reviews `gaps.md` before accepting |
| 2 | **ingest-ref** | a human adds a `docs/raw/` entry | one reference id | 1-15 pages touched, `index.md`, `log.md`, then the entry is **consumed** | LLM | small | one source at a time while the human reads the diffs (Karpathy's stated preference: "I prefer to ingest sources one at a time and stay involved"); the deletion is the op's last act, after the carry is printed |
| 3 | **ingest-commit** | post-commit hook, `SessionStart` staleness gate | the **targets** touched by `git diff` since `docs/wiki/meta.md` baseline (§13.2) | queue entries for those targets, then pages for them | hybrid: deterministic plan, LLM only on the delta | $0.05-0.15 per iteration (measured, same writeup) | `SKIP` when the diff is cosmetic/generated/deletion-only - zero tokens; never enqueue a diff that touches only `docs/wiki/` or `docs/raw/` |
| 4 | **ingest-session** | session close (the `retro`/`handoff` moment) | the session's decisions, dead ends, the plan cursor | `decisions/D<n>.md` pointers, `gaps.md` rows, `log.md` | LLM | small | only what is significant; the page set is "what I would need to get back to where we are" (the `nimbalyst-wiki` criterion) |
| 5 | **ingest-question** | a good query, or `research` | a question, web hits | new `docs/raw/` entries + a `questions/` page | LLM | medium | the answer must be anchored before it is filed |
| 6 | **query** | every session that asks | `index.md` → pages → anchors | an answer with citations | LLM | small | never answer from memory when an anchor exists; "Knowledge for explainers should be drawn from [the resources set], not from parametric guesses" (`teach/RESOURCES-FORMAT.md`) |
| 7 | **file-answer** | the answer was worth keeping | an answer | a `questions/` page + `log.md` | LLM | tiny | "good answers can be filed back into the wiki... they shouldn't disappear into chat history" |
| 8 | **reconcile** | every session start; CI | page anchors vs live tree/graph, commit baseline | `Status: stale` flips, `gaps.md` rows, rot counts | deterministic | free | no LLM may mark staleness |
| 9 | **lint** | weekly, or after a batch ingest | all frontmatter + wikilinks + anchors + graph | findings grouped by check type | deterministic scan + LLM triage | scan free; triage $0.30-0.50/project weekly (measured, same writeup) | mechanical fixes auto-applicable; judgement fixes need the human |
| 10 | **gapfill** | `gaps.md` row | the row, `research` | candidate `docs/raw/` entries (queued, unverified) | LLM | medium | candidates enter `docs/raw/` as *unverified*; ingest is what promotes them and what consumes them |
| 11 | **tour** | explicit, or on `ARCHITECTURE_UPDATE` | merged graph, previous layers | `tour.md` (≤7 stops) | hybrid | medium | tour prose is re-stated only when the plan says the architecture changed |
| 12 | **index** | every ingest, every lint | frontmatter | `index.md` | deterministic | free | regenerable in one pass; hand edits are a contract violation |
| 13 | **log** | every op | op, subject, counts | one `log.md` line | deterministic | free | `claimed = N of N` re-sums the ops against the pages touched |
| 14 | **retire** | a page is superseded | the page, its successor | `Status: superseded`, move to `docs/wiki/archive/` | LLM+tool | tiny | never delete - `TOOL-FORMAT.md` already mandates "superseded moves to archive, it is not deleted, so provenance survives". This is the *page* rule; references have no archive (§4.4) |

Every ingest op (2-5, 10) ends the same way: it consumes the entry that justified it. `pending=N`
is therefore a count every ingest prints and lint reads, and it is the queue's own health signal -
a `pending` count that only ever grows is maintenance that has silently stopped working (F8).

## 8. Skills vs tools vs hooks

The decision rule, written to match this repo's own vocabulary (`docs/agent/TOOL-FORMAT.md`: a
**Tool** is "a reusable script the agent runs to do a task", living in tracked `tools/<slug>/` with
its doc at `memory-bank/tools/<slug>/tool.md`, indexed by `memory-bank/tools.md`):

| Owner | Choose it when | Test |
|---|---|---|
| **Tool** (`tools/<slug>/`) | the operation is deterministic, re-runnable, and its output is a count | can it be written without an LLM call? does it print a verdict? |
| **Skill** (`docs/skills/<slug>.md` + the installed skill set) | the operation needs judgement over prose or code, and the human may want to interrupt | does the step change if you re-run it on the same input? |
| **Hook** (agent hook / git hook / CI) | the operation must fire without being asked, and must be cheap and fail-closed | would the agent reliably remember to do it? if not, hook it |
| **Neither - leave it human** | the operation is the thinking itself | the synthesis, the direction, the decision to retire a page |

One rule from `TOOL-FORMAT.md` carries over unchanged and is the whole cost of getting this wrong:
**one capability, one instrument.** "Do not clone a script and change two constants - parameterise
the two constants." That repo lesson (782 `.ps1` files in `tmp/`, eight `_slug_fetch` clones, the
`[int](skip/take)+1` slice-tag bug reproduced across clones) applies to skills too: do not write a
`wiki-research` skill when `understand-knowledge` already reads a Karpathy-pattern wiki.

### 8.1 Mapping the 14 ops onto instruments that already exist here

The `understand-anything` plugin is installed in this workspace (`~/.agents/skills/understand*`,
graph already at `.ua/knowledge-graph.json`) and its `understand-knowledge` skill's own description
is "Analyze a **Karpathy-pattern LLM wiki** knowledge base and generate an interactive knowledge
graph with entity extraction, implicit relationships, and topic clustering" - it detects
`index.md` + wikilinks + `raw/` + schema. The pattern is already instrumented here. Reuse it.

| Op | Owner | Instrument |
|---|---|---|
| seed | **skill, existing** | `/understand` (produces `.ua/knowledge-graph.json`, 7 phases, batched, worktree-redirect aware) then `/understand-onboard` for the tour; `wiki/` prose is written *from* that graph, never instead of it |
| ingest-commit | **skill, existing** | `/understand-diff` for the human-facing read ("what changed, affected components, and risks"); the *automatic* half is the plugin's own `hooks/post-tool-use-auto-update.mjs` + `prepare-incremental.mjs` plan - do not write a second diff-to-pages path |
| ingest-ref | **new skill** `wiki-ingest` | the only genuinely new semantic op: read one `raw/` reference, integrate into 1-15 pages, update index/log, respect the claim contract |
| ingest-session | **existing skills** | `retro` (close a session) + `handoff` (attach the cursor) - the wiki page set is their output shape; no new instrument |
| ingest-question / gapfill | **existing skill** | `research` for the web half; `wiki-ingest` for integrating what it returns |
| query / explain | **existing skills** | `/understand-chat`, `/understand-explain` (both already read the graph and cite nodes); add one rule in the schema file: answers cite `wiki/` pages when they exist |
| file-answer | **new skill** `wiki-file` | tiny; could equally be a stanza of `wiki-ingest` - keep it there until it earns its own name |
| reconcile | **new tool** `tools/wiki-reconcile` | deterministic: page anchors vs tree, `LastVerified` vs commit, graph `gitCommitHash` vs HEAD |
| lint | **new tool** `tools/wiki-lint` (scan) **+ existing skill** `triage` (findings) | the scan is SQL-over-frontmatter in Vault Operator's words - zero tokens; `triage` already owns "turn a pile of findings into a queue with owners" |
| index / log | **new tool** `tools/wiki-index` | regenerates `index.md`, prints `pages=N indexed=N unindexed=0` |
| tour | **existing** | `/understand` tour-builder phase; `tour.md` is a render of `tour[]`, not a second prose artifact |
| retire | **new tool** `tools/wiki-retire` | moves to `docs/wiki/archive/`, rewrites inbound links, prints the diff count |

Net new instruments: **six tools** (`wiki-census`, `wiki-ref-check`, `wiki-index`, `wiki-reconcile`,
`wiki-lint`, `wiki-retire`), **two skills** (`wiki-ingest`, `wiki-file`), one schema file.
`wiki-invariant-check` is not built - an invariant page's `Judges:` are already a build, a test
binary or a `dumpbin` call, so that capability would re-implement the compiler. Everything else is
already installed. That is the design's economy: the article-shaped pattern needs six new scripts;
reusing this environment's plugin needs six instruments and two skills for a domain it cannot serve.

### 8.2 What belongs in a skill, concretely

A skill file is the *governance for a semantic op* - the machine-checkable half lives in the tools,
the judgement half lives in the skill. `wiki-ingest` is roughly: the protocol (read the reference
via its anchor; name what it changes; touch only pages whose anchors overlap; every new claim gets
an anchor + verified stamp; append `log.md`; add `gaps.md` rows for what the reference does not
answer; **delete the queue entry from `docs/raw/pending.md` only after its anchor has been carried
into a page** - the consume step, §4.4), plus the four prohibitions that stop the docile-compiler
failure: **no unanchored claim about code; no restating a D-entry - link it; no copying a reference
into a page - cite its id; no consuming a reference before it is carried.**
Skills are markdown, so they are versioned, diffable, and reviewable like the wiki itself -
the HN thread's own observation, "Skills and memory are .md files on your system", is why this
works without inventing a plugin format.

## 9. Maintenance automation

Five mechanisms, in ascending eagerness. The order is load-bearing: each level is cheap enough to
run more often, and the expensive ones run only when the cheap one says so.

**(1) Staleness gate (free, every session start).** Never let an agent decide it is stale. Copy
the plugin's three-part test, which is subtle in a way worth keeping verbatim: compare the graph's
`gitCommitHash` against HEAD, but **"a hash mismatch alone is not stale when the project diff is
empty"** - `understand-diff` resolves the recorded commit, then runs
`git diff --name-only "$GRAPH_COMMIT" HEAD -- .` plus the working-tree and untracked lists, and
requires the `-- .` pathspec "because commits that only touch a sibling monorepo project must not
make this graph stale". The wiki's baseline is `docs/wiki/meta.md` (`baseline: <sha>`,
`graphHash: <sha>`). A `SessionStart` hook prints the same style nudge the plugin prints:
`[wiki] 12 pages stale over 47 changed files. Verdict fail=1.`

**(2) Deterministic reconcile (free, every session start / commit).** `tools/wiki-reconcile`
re-resolves anchors, flips `Status: stale`, opens `gaps.md` rows, and prints
`pages=214 anchors=1891 resolved=1884 rotted=7 stale=12 VERDICT fail=1`. Nothing is re-written by
LLM at this level; a page becomes *flagged*, not *rewritten*.

**(3) Post-commit ingest (cheap, bounded).** The hook fires; it does not free-fall. Four properties
the design requires of it, each chosen because a cheaper version was already seen to fail:

- **Queue, do not act.** The commit writes the changed-source set into a queue; ingest drains it.
  Acting inline on the commit path couples code review to LLM latency and makes a bad page
  invisible until it is read.
- **Quarantine failures.** A record whose read or validation step fails moves to a quarantine list
  with its reason; it is never silently dropped, and it never blocks the rest of the drain.
- **Bounded retries, fail-closed drain.** Re-run while the queue is non-empty; a failed batch leaves
  the circuit open, so the next session sees it. Only a clean final batch clears the baseline - which
  is the same invariant the `understand-anything` finalizer enforces ("a graph-save failure must
  never advance the fingerprint or commit baseline", "never update `meta.json` after a failed
  graph/agent phase").
- **Budget ceiling.** `--max-budget-usd` on every automated call - the working implementation's own
  safety net, quoted: "the --max-budget-usd flag serves as your safety net during every automated
  call. If something goes wrong, the process will stop once the limit is reached instead of
  uncontrollably consuming funds." Measured cost of this op: $0.05-0.15 per iteration.
- "deletion-only, ignored-only, generated-artifact-only, and cosmetic-only updates dispatch no
LLM agent."

**(3b) One queue, three producers.** `docs/raw/pending.md` *is* the queue, and §13's answers make it
the only one: a human drops a reference, `gapfill` proposes candidates, and the post-commit hook
enqueues the **targets** a commit touched. All three write the same entry shape, with `added_by:`
naming which; `wiki-ingest` is the only consumer. Two consequences of unifying them:

- **The anti-loop guard is mandatory, not optional.** The hook must ignore a diff that touches only
  `docs/wiki/` or `docs/raw/` - otherwise the ingest's own commit re-enqueues itself forever, which
  is F9 as a loop rather than a cost. This guard exists only because wiki output is tracked (§13.3);
  it is the one place a settled decision creates a hard requirement.
- **`pending=N` sits next to the staleness count in the SessionStart nudge**, so "the wiki is behind"
  and "the wiki is unverified" are two visible facts instead of one feeling:
  `[wiki] pending=3 pages_stale=12 over 47 changed files. Verdict fail=1.`

The invariant the design claims, and which nothing else may claim:

```
pending=0            everything known has been compiled into pages
reconcile fail=0     what is compiled still matches the tree
both green           the wiki is current - the only state worth asserting
```

Neither half implies the other. An empty queue with seven rotted anchors is a wiki that is compiled
and lying; a full queue with clean anchors is a wiki that is honest and behind. Only the pair is a
verdict, and that pair is why `docs/raw/` is allowed to be empty at all.

**(4) Lint cadence (weekly, or after a batch ingest).** Vault Operator's rhythm is the honest one
and it is worth keeping as the default: the scan "runs once automatically when the vault opens;
beyond that there is no schedule... most users only run it when something prompts them: after
importing a batch of notes, after reorganizing folders, when search results start feeling patchy."
So: scan on demand + at ingest; schedule nothing except the weekly triage. Findings arrive
**grouped by check type, each with a severity marker and what's wrong and where**, and each gets
three actions - `Repair` (mechanical, applied with an undo), `Discuss` (needs judgement),
`Dismiss` (filtered out of future scans, not deleted; "the modal footer shows a count of everything
you've dismissed", restorable). Adopt all three states; a finding with no `Dismiss` state is a
finding that reappears forever.

The lint checks, transcribed from Vault Operator's health check (each is a query over the graph,
zero LLM tokens) into their code equivalents:

| Check | Code equivalent |
|---|---|
| orphaned notes (zero inbound wikilinks) | a page no other page or `index.md` links to - invisible to navigation |
| broken links | wikilink/markdown link to a page that no longer exists (rename debt) |
| missing backlinks | page links out, target never links back |
| god nodes | a page above the in-degree threshold - it has become a dumping ground |
| weak clusters | pages semantically close but unlinked - the connection you meant to make |
| inconsistent tags | `PageType`/`Status` vocabulary drift, e.g. `stale` vs `Stale` |
| category mismatches | frontmatter `PageType` disagrees with the folder it lives in |
| stale clusters | freshness below threshold - where a refresh pays off |
| source concentration | a page whose anchors all come from one file/domain - it inherits that blind spot |
| **code-specific additions** | unanchored claims; a symbol claimed by a page that the graph says does not exist; two pages asserting different claims for the same anchor; a page whose `Judges:` command no longer exists; an anchor into `tmp/` |

**(5) CI gate (never LLM).** `.github/workflows/wiki-maintenance.yml` beside the existing
`cmake-multi-platform.yml`, running only the deterministic tools and failing on rot: `wiki-reconcile`
+ `wiki-lint` + `wiki-index` with `VERDICT fail=N` as the exit code. The wiki then has the same
notion of green as the code does (`docs/agent/verification.md`: green is a printed count plus exit
0), and a rotting wiki turns the CI red the way a broken build does.

**Never automate:** the synthesis, the decision to retire a page, and any *judgement* fix. The
governing pattern is Vault Operator's fail-closed rule - "write operations need your approval,
unless you have selected auto-approve for that category... for mechanical, low-risk repairs
(like reformatting a malformed wikilink) you can let the agent apply them without asking.
Anything ambiguous still goes through Discuss." Auto-approve exactly: `index.md` regeneration,
backlink insertion, link rewrite on move, `Status: stale` flips. Human-gated: every prose edit,
every merge of two pages, every split of a god node.

**Undo is git.** The wiki is markdown in the repo, so the checkpoint system Vault Operator builds
out of isomorphic-git and shadow repos is already free here: "the wiki is just a git repo of
markdown files. You get version history, branching, and collaboration for free." Keep `docs/wiki/` and
`docs/raw/` **tracked** (unlike `nimbalyst`, which keeps its wiki out of git by default); the point of
tracking is that wiki drift becomes reviewable in the same `git diff` as code drift. §13 settles it
to Yes, and that answer creates two requirements stated elsewhere: the `wiki:` commit prefix (§13.1)
and the hook's exclusion of `docs/wiki/` and `docs/raw/` from its diff (§9(3b)) - without the second,
the first makes the maintenance path loop rather than run.

## 10. The prompt (paste-ready)

This is the deliverable for "how best to prompt you". Three parts, and the order matters: the
idea file first (it communicates the pattern), the instantiation second (the domain-specific
deltas), the *anti*-instructions third (what to refuse). Karpathy's own instruction is "The right
way to use this is to share it with your LLM agent and work together to instantiate a version that
fits your needs" - so the prompt's job is to make the agent *negotiate*, not to pre-decide the tree.

```text
You are to become the maintainer of a wiki whose sources are this repository's code.

Read this design file first: docs/WikiDesign.md. It instantiates Karpathy's LLM Wiki pattern
(<https://gist.github.com/karpathy/442a6bf555914893e9891c11519de94f>) for a codebase. Then
propose the instantiation. Do not start writing pages until the proposal is approved.

Three layers:
- docs/raw/  references only, awaiting ingestion. One queue file, pending.md, one entry per
         reference: id, kind (in-tree | out-of-tree | event | artifact), anchor, pin, use_for,
         verified, judge, added_by. NEVER copy a reference into docs/raw/ or into a wiki page.
         Cite the id. The queue is emptied by ingest, not by an archive step.
- docs/wiki/  the prose. LLM-owned. Every page: YAML frontmatter (machine state) + markdown body.
         Every claim about code carries an anchor and a verified stamp. Every invariant page
         carries a judge: the command that would show it failing.
- docs/wiki/schema.md  the governance. One file the agent re-reads every session. Co-evolve it; log
         every change to it in docs/wiki/log.md.

Hard rules, in priority order:
1. No unanchored claim about code. An unanchored sentence is a defect; count it, do not hide it.
2. Never restate a decision - link the D-entry in docs/agent/DECISIONS.md.
3. Never restate structure the graph already holds - cite .ua/knowledge-graph.json node ids.
   The wiki adds only what the graph cannot hold: invariants, decisions, traps, the tour.
4. Prefer named anchors (path#symbol, commit:sha, D<n>) to line numbers. A line number is a
   derived anchor and may not be the only anchor.
5. No anchor may point into tmp/. It is gitignored and deleted per task.
6. Staleness is decided by tools/wiki-reconcile, never by me. A page becomes flagged stale,
   not rewritten, at that level.
7. One capability, one instrument. Before writing any script or skill, read
   memory-bank/tools.md and the installed skill set. If a capability exists, refine it.
8. Ingest one reference at a time while I read the diffs. Batch only when I say so.
9. Propose, then act: show the pages you will touch and the anchors you will add before you
   write them. Mechanical fixes (index regeneration, backlinks, stale flips) may apply
   unasked. Any prose edit, page merge, or god-node split needs my approval.
10. Ingest consumes its reference: carry the anchor into the pages you wrote, print the carry,
    THEN delete the entry from docs/raw/pending.md. Never delete before the carry. The only
    state I may claim is pending=0 with reconcile fail=0 - "the wiki is current".

Operations I can ask for, and their owners (docs/WikiDesign.md §7-8):
seed / ingest-ref / ingest-commit / ingest-session / ingest-question / query / file-answer /
reconcile / lint / gapfill / tour / index / log / retire.

Before designing, re-read these and inherit their decisions rather than inventing parallels:
- docs/future/PLAN-FORMAT.md  (front matter = machine state, body = prose; "every `Atomic` item
  states, in its section, the one check that closes it - an instrument line or a re-read that
  prints a count. An item without a stated stop condition is not a step")
- docs/agent/TOOL-FORMAT.md   (the tool doc format; the one-instrument rule; the archive rule is inverted for the wiki - deletion + git history keeps it manageable and safe)
- memory-bank/gotchas/        (each is evidence for an invariant page)
- docs/learning/RCU/RESOURCES.md  (the reference-catalog style to copy, including Gaps)
- the understand-* skills     (structure, diff, chat, explain, knowledge-graph - already installed)

Two things to refuse on sight:
- A page that reads well but whose anchors do not resolve. Plausible prose about code is the
  failure this design exists to prevent.
- A wiki that mirrors the tree. A page per symbol is a graph, not a wiki; the graph already
  exists at .ua/. Apply the page-significance rule before creating a page.

Cost ceiling per automated call: <name the budget flag and value>. Print, at the end of every op,
one line in this repo's house style: counts plus VERDICT fail=N, and name the inputs it read.
```

Two prompt variants worth keeping:

- **Short form** (a new session, one line): `wiki: ingest <ref-id> per docs/WikiDesign.md §4-5;
  print the touch-set and anchors before writing; then run tools/wiki-reconcile and report VERDICT.`
  The schema file carries the governance, so the prompt only has to name the op and the object.
- **Anti-drift form** for the maintenance ops, which is the only place a prompt must be *strict*:
  `Run tools/wiki-reconcile. Do not read docs/wiki/ pages. Do not edit prose. Report rot counts and
  open gaps.md rows for each. VERDICT fail=N closes it.`

## 11. Relationship to what already exists here

The wiki must not become a fourth place where the same thing is written. The repo already has
three, and the design's rule for each is one line: **reference it, never fork it.**

| Artifact | Owner | Wiki's relation | Anti-pattern |
|---|---|---|---|
| `docs/agent/repo_map.md`, `deps.dot`, `docs/agent/ctags/*.tags` | generated, deterministic | `docs/wiki/architecture.md` and `docs/wiki/dependencies.md` link them and add the *why* the generator cannot state | restating the dependency list in prose |
| `memory-bank/ActiveContext.md`, plan-tree `plan.md` cursors | mutable session state - the durable record is the D-entries and the register CLOSE lines | `docs/wiki/` is the record; `docs/raw/` may reference a plan item as an `event` anchor | letting a wiki page become the cursor |
| `docs/agent/DECISIONS.md` + `decisions/D<n>.md` | the durable why | `docs/wiki/decisions/D<n>.md` is a pointer page: title, one line, link | duplicating the decision body |
| `memory-bank/gotchas/*` | what has cost time before | each becomes an `invariants/` or `gotchas/` page whose judge is the failing command | re-deriving the trap from memory |
| `memory-bank/tools.md` + `tools/<slug>/` | the tool index | the four new wiki tools register there per `TOOL-FORMAT.md` (`Type: Tool`, `Scripts: File/Usage/LastRun/Output`) | a wiki script outside the index |
| `docs/learning/<topic>/` (`MISSION.md`, `RESOURCES.md`) | teaches a **human** what the repo does not contain; `docs/wiki/` tells an **LLM** what the repo does (§13.4) | supplies anchors on request: a learning resource becomes a `docs/raw/` entry only when a wiki claim would otherwise be unanchored | keeping the two catalogs in sync; importing a curriculum into the wiki; giving learning pages claim contracts |

One genuine tension to name rather than resolve silently: `docs/agent/repo_map.md` already says
"Generated by agent. Do not edit by hand; regenerate from CMakeLists.txt scan." So there are two
prose-about-code surfaces - `docs/agent/` (agent state + generated indexes, imperative, per-pass)
and `docs/wiki/` (declarative understanding, permanent). Keep them separate by role, not by format:
`docs/agent/` answers *what is happening now*; `docs/wiki/` answers *what is true about the system and
what it means*. A reader who cannot tell which one to open is reading a design that has not made
that split, and that is the failure below.

§13's answer to decision 4 names a third surface and separates all three by audience, which is the
sharper cut: `docs/learning/` is for a human meeting something the repo does not contain (it may
carry a mission, a curriculum, prose that is aspirational); `docs/wiki/` is for an LLM citing this
repo (every claim refutable); `docs/agent/` is the agent's own working state. Two of the three carry
claim contracts and one deliberately does not - an aspirational learning page has no judge to run
against this tree, and giving it one would turn a teaching document into a claim about code. The
split is therefore one-directional: learning supplies anchors to the wiki, and the wiki never feeds
back into the learning track.

## 12. Failure modes and guards

Each row states the failure, the guard that makes it visible, and the instrument line that
closes it. Ordered by what actually bit this repo, not by what is theoretically worst.

| # | Failure | Guard | Instrument line |
|---|---|---|---|
| F1 | **Anchor rot** - the page cites a symbol that no longer exists, so the prose is unfalsifiable | `wiki-reconcile` re-resolves every anchor on every session start | `anchors=1891 resolved=1884 rotted=7 VERDICT fail=1` |
| F2 | **Scratch-path citation** - an anchor into `tmp/`, which is gitignored and deleted per task | `wiki-ref-check` counts any anchor whose path starts `tmp/`; hard fail | `tmp_refs=0 VERDICT fail=0` (this repo already lost 566 comment runs' reasoning this way) |
| F3 | **Unanchored claim** - plausible prose about code, the docile-compiler failure | claim-contract lint counts sentences about code that carry no anchor | `claims=12 anchored=11 unanchored=1 VERDICT fail=1` |
| F4 | **Duplication drift** - the wiki restates a D-entry or the dependency list, then one copy goes stale | lint flags pages whose body overlaps a linked artifact above a similarity threshold; the fix is always "link it" | `restatements=2 VERDICT fail=1` |
| F5 | **Mirror-of-the-tree** - a page per symbol; the wiki has become a worse copy of the graph | page-significance rule (§5.3) gates page creation; god-node check gates hubs | `pages=214 orphans=6 over_connected=2 VERDICT fail=1` |
| F6 | **Staleness by assumption** - the agent declares a page fresh because it feels fresh | only `wiki-reconcile` may set `Status`; commit-baseline + project diff, never the hash alone | `baseline=66118c30 head=99c8c20c project_diff=47 stale=12 VERDICT fail=1` |
| F7 | **Contradiction left standing** - two pages assert different claims for the same anchor | lint groups claims by anchor, flags more than one distinct assertion | `anchor_conflicts=3 VERDICT fail=1` |
| F8 | **Hook silence** - the auto-maintenance path fails and nobody learns it | queue + quarantine + fail-closed drain; CI gate fails on rot; the hook prints its verdict | `queued=0 quarantined=0 circuit_open=false VERDICT fail=0` |
| F9 | **Cost runaway** - a loop feeds itself (lint opens gaps, gapfill ingests, ingest triggers lint) | per-op budget ceiling; a drain that cannot re-enter itself; ingest never schedules lint | `budget_usd=0.40 spent=0.31 VERDICT fail=0` |
| F10 | **Governance drift** - the schema file and the skills disagree, so the same op runs two ways | every schema edit is logged in `docs/wiki/log.md` with its date; skills cite the schema section they implement | `schema_version=7 skills_current=6/7 VERDICT fail=1` |
| F11 | **Silent authority loss** - an `out-of-tree` reference's answer depends on an unpinned version | every `out-of-tree` entry carries `pin:` and `verified:`; a missing pin is a hard fail | `out_tree=44 pinned=41 unpinned=3 VERDICT fail=1` (this is D21: the SDK triple, plus the unpinned shrink macros that still decide the surface) |
| F12 | **Consumed before carried** - an ingest deletes a queue entry that no page cites, so the reference and everything it justified are gone with no record of what was lost | `wiki-ingest` prints the carry before deleting; `wiki-ref-check` counts pending ids against ids cited in any page's `Sources:`/`Anchors:`; an uncarried deletion is a hard fail, and the entry is recoverable from git's history of `docs/raw/pending.md` | `pending=0 consumed=7 carried=7 uncarried=0 VERDICT fail=0` |

F11 is the quietest and worth a worked example: `user32.h` at SDK 10.0.26100.0 exposes 598
declarations / 597 entry points only with `_WIN32_WINNT=0x0A00`, `NTDDI_VERSION=0x0A000010`
(`NTDDI_WIN11_GE`), pinned at `kernel32.h:22-30` by decision D21, and the shrink macros D21 records
as unsettled at its line 63 (`NOGDI`, `NOUSER`, `NOSYSPARAMSINFO`) left alone. Quote the anchor
without the pin and the citation cannot be re-derived - the count changes with the pin, and the page
still reads perfectly. A wiki that records claims without recording the conditions that make them
reproducible is not a wiki; it is a transcript.

## 13. The five decisions - settled 14 Nov 2026

Five, because each changes the shape of the artifacts, not just their content. Everything else in
this design is settled enough to build against. The questions and your answers are kept verbatim
below; §13.1 is what they commit.

1. **Wiki location.** `wiki/` at the repo root beside `raw/`, or `docs/wiki/` inside the existing
   prose tree? The repo's convention favours `docs/wiki/` (all durable prose lives under `docs/`);
   the pattern favours a root `wiki/` so the three layers read as one unit. Preference, not design.
   ANSWER: docs/wiki/
2. **Granularity of the ingest unit.** Karpathy drops a whole document. Here the unit is file / TU /
   target / subsystem. The repo's own churn unit is the **target** - `docs/agent/verification.md`
   sets "Unit of verification = one QOR lib (one `set(target qor_X)`)". Measured 14 Nov 2026: 91
   `set(target qor_` occurrences across 208 `src/` CMakeLists.txt, which `docs/agent/repo_map.md`
   groups into 33 QOR libraries - a workable page count either way. Adopt target-granularity.
   ANSWER: target granularity is commit/change granularity. Ingest what has changed keeps the wiki in step with the code
3. **Whether `wiki/` is git-tracked.** Recommendation: yes, tracked, so wiki drift shows in the same
   `git diff` as code drift and the archive rule keeps provenance. The counter-case is a wiki that
   churns faster than it is reviewed, which makes commit noise outweigh value.
   ANSWER: Yes
4. **Whether the wiki duplicates the `docs/learning/RCU/` track.** Recommendation: no - that track
   stays a `raw/` reference set; the wiki absorbs it only where RCU becomes a QOR concern.
   ANSWER: No - docs/learning is for teaching the human user new things docs/wiki is for informing the LLM about the repo
5. **The cadence the human actually wants.** Two settings, not a dial: ingest-commit on (the hook
   drains after each commit) or off (the wiki updates only at session close). Everything else in §9
   is cadence-free: session-start reconcile, on-demand lint scan, weekly triage.
   ANSWER: ingest-commit. Aligns with Granularity

### 13.1 What those five answers lock in

Each answer is a decision about shape, so each renames or retires something below. None of them is
cosmetic; the list is what a first pass must respect.

1. **`docs/wiki/`** renames every path in this document: the three layers become
   `docs/raw/pending.md`, `docs/wiki/`, `docs/wiki/schema.md`. Two consequences worth noticing
   rather than inheriting silently. First, `docs/wiki/` becomes a sibling of the existing
   `docs/{agent,archive,articles,future,learning,skills,ui}`, so the repo's "all durable prose under
   `docs/`" convention holds and the layers stay adjacent. Second, **the schema stops being a folder
   and becomes one file** - the repo rule "every new folder needs `purpose.md`" would otherwise
   demand a folder whose only member is the schema. Third, `schema/` no longer sits beside `raw/`,
   so the "three layers read as one unit" argument is carried by naming (`docs/raw`, `docs/wiki`)
   instead of by adjacency. Verified today: `.gitignore` holds only `/.ua` and `/tmp`, so nothing
   under `docs/` is ignored and answer 3 costs nothing.
2. **Target granularity = commit granularity** makes the ingest unit the QOR lib target, which is
   already this repo's unit of verification (`docs/agent/verification.md`: one QOR lib, one
   `set(target qor_X)`), and whose judge is already house style:
   `cmake --build cmakebuild --target qor_<NAME> --parallel 8`. The roll-out is a deterministic map
   the design can now depend on: `git diff --name-only baseline HEAD` → the set of targets → the set
   of pages, where file→target comes from the graph's module grouping or a CMakeLists scan. Three
   things follow. (i) The queue's items are targets, not files, so one commit touching four files in
   one lib enqueues one unit. (ii) The page count is bounded by the measured target set - **91
   `set(target qor_` declarations / 90 distinct targets**, of which 88 appear in `repo_map.md`'s
   Library index while that header still claims 33 (stale prose; the instrument is the authority,
   and `docs/agent/repo_map.md`'s count is corrected in §14's census item). A first pass seeding the
   17 core libs (14 `src/qor` + 3 `src/platform`) sits far under Karpathy's ~100-source threshold,
   so **`index.md` alone suffices and the qmd rung stays unbuilt**; full 90-page coverage is where
   that threshold is met, not undercut. (iii) A page's `Judges:` are its target's build commands,
   which means a wiki page can be made red by the same command that makes a library red, and there
   is no wiki-specific notion of green left to invent.
3. **Tracked** means wiki maintenance produces commits, and the repo's own commit rules then apply
   to them: one logical commit per coherent change, no drive-by edits, commit record = objective +
   pre-existing defect + change. Roll-out: a drain's output is committed as **one `wiki:`-prefixed
   commit carrying that drain's verdict line in its body**, at the drain's granularity, never folded
   into the code commit that triggered it (that commit already exists by the time the drain runs).
   The sharp consequence is the loop guard in §9(3b): `wiki:` commits are not code churn, so the
   staleness gate and the post-commit hook must exclude `docs/wiki/` and `docs/raw/` from their
   diffs - without that exclusion the ingest's own commit re-enqueues itself forever, and the design
   has a runaway instead of a cost. The same exclusion is what keeps `pending=0` reachable.
4. **Learning ≠ wiki, by audience** resolves the §11 tension in favour of two catalogs that never
   sync. `docs/learning/` teaches a human things the repo does not contain; `docs/wiki/` states
   falsifiable things about the repo. Roll-out: the learning track is a *supplier of anchors*,
   consulted when a wiki claim would otherwise be unanchored, and never a catalog to keep in step;
   learning pages never carry `Anchors:`, `Judges:` or `LastVerified`. The asymmetry is deliberate
   and is enforced by the claim contract, not by convention: an aspirational page about RCU internals
   is not a claim about this tree and must not be falsified as one.
5. **ingest-commit on**, aligned with granularity as the answer says, makes the drain the default
   maintenance path: §9's levels 1-3 always run, levels 4-5 stay on demand. Combined with answer 2
   this is what turns "empty `docs/raw/`" into a routine steady state rather than an aspiration -
   every commit enqueues its targets, the drain consumes them, `pending=0` is normal. It also means
   the queue has three producers and one consumer, which is why entries carry `added_by:`.

The three invariants the five answers produce together, and which the build must print rather than
assert:

```
pending=0            docs/raw/ is empty  -  everything known has been compiled
reconcile fail=0     what is compiled still matches the tree
pages_for(targets)   every changed target has a page, and that page's judge is its build
```

The first two are the wiki's only claimable state (§9). The third is what answer 2 buys: page
coverage is checkable against the diff, so "the wiki is behind" is a named set of targets and not a
feeling.

## 14. First pass if this is built

An item tree per `docs/future/PLAN-FORMAT.md` - machine state in front matter, prose in the body,
each Atomic item naming the instrument line that closes it. Deliberately sequenced so the
deterministic half exists *before* the prose half, because prose with no instrument to judge it is
the failure this design exists to prevent.

| # | Item | Kind | Closes with |
|---|---|---|---|
| 1 | Write the governance file `docs/wiki/schema.md`: the three layers, the claim contract, the op list, the consume rule, the prohibitions | Atomic | a `schema_version=1` line in `docs/wiki/log.md` |
| 2 | `tools/wiki-ref-check` - re-resolve every anchor a page carries and every entry still in the queue; count uncarried consumptions | Atomic | `pending=N resolved=N rotted=0 tmp_refs=0 uncarried=0 VERDICT fail=0` |
| 3 | `tools/wiki-index` - regenerate `docs/wiki/index.md` from frontmatter | Atomic | `pages=N indexed=N unindexed=0 VERDICT fail=0` |
| 4 | `tools/wiki-reconcile` - anchors vs tree, baseline vs HEAD, graph hash vs HEAD, and diff → targets → page coverage | Atomic | `pending=N anchors=N resolved=N stale=0 uncovered_targets=0 VERDICT fail=0` |
| 5 | Seed `docs/raw/pending.md` from references that already exist: `docs/agent/*`, `memory-bank/gotchas/*`, the SDK pins | Atomic | item 2's line, all green |
| 6 | The two skeletons: `docs/raw/` (`purpose.md`, `pending.md`) and `docs/wiki/` (`purpose.md`, `schema.md`, `index.md`, `log.md`, `meta.md`, `gaps.md`, seven category folders) | Atomic | every folder carries a `purpose.md` (repo rule) |
| 7 | Seed the graph and read it: `/understand`, then `/understand-onboard` | Atomic | `.ua/knowledge-graph.json` `gitCommitHash` == HEAD |
| 8 | Write one page per QOR lib target for the highest-churn libs (start with `docs/future/TODO.md` item 13 and the U2 register) | Compound, 5 Atomic children by target | each page's `Judges:` build line green + claim lint `unanchored=0` |
| 9 | Write the invariant pages out of `memory-bank/gotchas/*` - one page per gotcha, judge = the failing command | Compound, 4 Atomic children | each page's judge reproduces the recorded failure |
| 10 | `docs/wiki/architecture.md` + `docs/wiki/dependencies.md` as pointer pages into `deps.dot` / `repo_map.md` | Atomic | lint `restatements=0` |
| 11 | `tools/wiki-lint` - the ten checks of §9 plus F12's consume check, deterministic, zero tokens | Atomic | `VERDICT fail=0` on a clean wiki **and** one injected anchor rot and one injected uncarried consumption each fire |
| 12 | The `wiki-ingest` skill, including its consume step (carry, print, delete) | Atomic | one reference ingested end-to-end: touch-set printed before writing, entry deleted last, item 2's line green after, `carried=1 uncarried=0` |
| 13 | The staleness gate + the post-commit hook: diff → targets → queue, excluding `docs/wiki/` and `docs/raw/` | Atomic | a cosmetic-only commit spends zero tokens and prints `queued=0 pending=0 VERDICT fail=0`; a `wiki:`-prefixed commit enqueues nothing |
| 14 | The CI gate workflow | Atomic | workflow exits non-zero on injected anchor rot, zero on a clean tree |
| 15 | Register the six tools in `memory-bank/tools.md` per `TOOL-FORMAT.md` | Atomic | each entry has `Type: Tool` and `Scripts: File/Usage/Output` |

This table is the design's shape. The charted, gate-checked decomposition of it lives at
[`docs/future/BuildLLMWiki/plan.md`](../future/BuildLLMWiki/plan.md) (14 Nov 2026): **16 plans,
44 `Atomic` leaves, tree terminal** (`plans=16 levels=0,1,2,3 atomic=44 refined=15 planning=0
VERDICT fail=0`), with measured unit counts on every item and every leaf marked `Atomic` only
where the gate's size, input and enumeration tests pass - which is why the tree's leaf count
exceeds this table's 15 items.

Item 11 deliberately injects its own failures: an instrument that has never seen a red case is
untested, and this repo's whole habit is that a gate is only trusted once it has been seen to fail
(`docs/agent/verification.md` records the same discipline for builds and tests). Two red cases are
injected rather than one, because the consume check and the anchor check fail for different reasons
and only the pair proves the queue's contract.

## 15. What was read, and what it changed

| Source | What it changed in this design |
|---|---|
| [Karpathy's gist](https://gist.github.com/karpathy/442a6bf555914893e9891c11519de94f) (raw text read in full) | the three layers, three ops, two nav files; and the assumption §4 inverts - the raw layer is immutable *because it is a copy*, which is what makes §4.4's consume-and-delete safe here and would be destructive there |
| [HN 47640875](https://news.ycombinator.com/item?id=47640875) | the objections that became F3 (ungoverned compilation) and F5 (a wiki that mirrors the graph instead of reasoning over it) |
| [Hacker Noon: self-maintaining knowledge base for 6 projects](https://hackernoon.com/how-i-built-a-self-maintaining-knowledge-base-for-6-projects-using-claude-code-and-karpathys-llm-wiki) | the only published measured costs ($0.50-1.00 setup, $0.05-0.15 per commit ingest, $0.30-0.50 weekly lint, $10-20/month over six projects), the `--max-budget-usd` ceiling, and QMD as the search layer past ~100 sources |
| [Vault Operator health check](https://pssah4.github.io/vault-operator/guides/vault-health) | the ten lint checks, the Repair/Discuss/Dismiss tristate, the "no schedule beyond the open-the-vault scan" cadence, and auto-approve restricted to mechanical repairs |
| the `understand-anything` plugin installed in this workspace (`~/.understand-anything-plugin`, graph at `.ua/knowledge-graph.json`) | the incremental machinery worth copying outright - commit baseline, structural fingerprints, `SKIP/PARTIAL_UPDATE/ARCHITECTURE_UPDATE/FULL_UPDATE`, the zero-token cost rule, the "hash mismatch alone is not stale" test, and `understand-knowledge`, which already parses a Karpathy-pattern wiki |
| [Jonadas: Beyond Karpathy's LLM-Wiki](https://www.jonadas.com/writing/essays/beyond-karpathys-llm-wiki) | governance as the counterweight to ungoverned compilation - the friction-point protocol behind §2(d) and the claim contract in §5.2 |
| [Nimbalyst Wiki](https://github.com/nimbalyst/nimbalyst-wiki) | the page-selection criterion ("what I would need to get back to where we are") and the counter-example of keeping a wiki out of git, which this design argues against for this repo |
| this repo: `docs/future/PLAN-FORMAT.md`, `docs/agent/TOOL-FORMAT.md`, `docs/agent/verification.md`, `docs/agent/decisions/D21.md`, `docs/agent/repo_map.md`, `docs/agent/purposes.md`, `memory-bank/*`, `docs/learning/RCU/RESOURCES.md`, `docs/agent/skill.md` | the conventions inherited rather than invented: front matter = machine state, one tool per capability, green = printed count + exit 0, the SDK pin that makes a citation reproducible, the `tmp/` citation loss that became F2, and the "generated, do not edit by hand" precedent that forced the §11 role split |

**Not adopted, and why:** the gist's Obsidian Web Clipper, Marp slides, Dataview frontmatter tables,
and image-download tricks - the sources here are code, not clipped articles. Also not adopted:
**the archive step for the raw layer**, which his design needs because a raw entry is the reading
and ours needs because a raw entry is a pointer - the consumed anchor is carried into the page that
used it, so the page is the archive and the queue is allowed to be empty (§4.4); the page-level
archive rule survives unchanged, since a superseded page still holds prose worth reading. The one
tip that survives is "the wiki is just a git repo of markdown files. You get version history,
branching, and collaboration for free", which is what makes Undo, provenance of a deleted queue
entry, and the `wiki:` commit convention free in §9 and §13.1. Also not adopted: a page per symbol,
a per-session wiki, and an LLM that decides freshness - each is a failure mode (F5, F6) rather than
a preference.

## 16. Next step

The five decisions are settled (§13), so this design is buildable and the file's remaining job is to
be re-read, not re-written. Two actions, in order:

- **Build §14 as a plan tree** - `/implement-spec` or `/to-tickets` over the 15-item table, items 1-4
  before item 5. The tree is already Atomic-shaped and every close is a printed verdict line.
- **Update the memory bank**, because this design changes the repo's shape: `docs/raw/` and
  `docs/wiki/` are new folders, six tools register in `memory-bank/tools.md` per `TOOL-FORMAT.md`,
  and §13's answers plus §13.1's consequences belong in `memory-bank/activeContext.md` as the current
  focus.

Two things that must not be fudged during the build: no item may report done except by its own
instrument line (`docs/future/PLAN-FORMAT.md`'s rule), and the only wiki state worth claiming is
`pending=0` with `reconcile fail=0` (§9). Everything else the wiki may say is a count, not a mood.
