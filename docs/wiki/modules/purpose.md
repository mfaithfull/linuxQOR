# Purpose - docs/wiki/modules

One page per **QOR lib build target** - the unit of wiki work (§13 Q2 answer: target granularity is
commit granularity). File name is the target: `qor_pipeline.md` for `set(target qor_pipeline)`.

- The count is the target set, not a feeling: **91 `set(target qor_` declarations / 90 distinct
  targets** across 208 `src/**/CMakeLists.txt` (measured 14 Nov 2026; `qor_http` is declared twice).
  `tools/wiki-census targets` reprints it; `coverage` says which targets still have no page.
- A page is about the target: what it is for, what breaks if it changes, and the invariants its own
  sources enforce. Symbol detail lives in the page unless the symbol earned its own - §5.3.
- `Judges:` are that target's commands - `cmake --build cmakebuild --target qor_<NAME> --parallel 8`
  plus its test binary - so a page goes red exactly when its library does.
- Front matter fields, page size budget and the claim contract: `docs/wiki/schema.md`.
- Not here: a page per file, per class or per symbol (that is `.ua/knowledge-graph.json`'s job, at
  higher quality and zero cost); subsystem essays, which are `docs/wiki/` root pages.
