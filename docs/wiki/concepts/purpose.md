# Purpose - docs/wiki/concepts

One page per **external** row of `docs/learning/RCU/RESOURCES.md` - the domain vocabulary the
wiki carries without pretending it is a claim about this tree (`docs/wiki/schema.md` section
"Pages"). The learning catalog stays in `docs/learning/`; a wiki page that copies its index has
forked the truth, and the two catalogs never sync.

- A concept page is `Type: WikiPage`, `PageType: concept`, and carries **no `Anchors:`** -
  there is no path, symbol or commit in this tree that can make it stale.
- `Judges:` is the D56 sentinel (`docs/agent/decisions/D56.md`): `none - this page describes a
  domain, not this tree; it is not falsifiable against a build`. The list is non-empty, so
  `wiki-lint schema` passes it, and `wiki-lint freshness` leaves these pages `undated` by design
  - a page nothing dates is a page nothing can stale-date.
- The count is the external-row count: `wiki-census pages` prints `  concepts = N` and the day it
  diverges from the external rows of the RESOURCES file is a finding. A `Local:` row is this tree
  and gets module/gotcha handles, not a concept page.
- Not here: claims about this tree (those are module/gotcha/invariant/decision pages),
  `docs/learning/` index content, or a domain fact the tree itself can falsify.
