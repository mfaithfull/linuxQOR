---
Type: WikiPage
PageType: concept
Title: "cs-libguarded"
Status: current
LastVerified: 2026-11-18
Claims: 4
Summary: "The cs_libguarded domain: CopperSpice's header-only, C++20, BSD 2-clause concurrency-guard library (copyright 2015-2018 Barbara Geller and Ansel Sermersheim) - the upstream both of this repo's guarded-header copies derive from, and the reference point for reasoning about what the code should do versus what this copy does."
Judges:
  - none - this page describes a domain, not this tree; it is not falsifiable against a build (see docs/agent/decisions/D56.md)
---

# cs-libguarded - the upstream domain

**This page describes a domain, not this tree; it is not falsifiable against a build.**

The resource named by `docs/learning/RCU/RESOURCES.md` (the GitHub `copperspice/cs_libguarded`
row) is authority for:

- the library's **shape**: header-only, C++20, BSD 2-clause licensed;
- its **provenance**: copyright 2015-2018 Barbara Geller / Ansel Sermersheim, published by
  CopperSpice;
- the **derivation fact** this tree observes: both QOR guarded headers carry the libguarded file
  header ("This file is part of libguarded") - the copies are descendants of this upstream, not
  independent implementations;
- its **use as a reference point**: when reasoning about the copies, the upstream defines what
  the code *should* do; differences are then defects or deliberate local choices, not ambiguity.

What this page is **not**: a specification of this repo's copies, and not an upstream changelog -
it records the domain the RESOURCES row names, and upstream facts beyond that row need the
resource itself.

## See also

[[rcu]] - the synchronization domain the guarded types serve; [[libguarded-contract]] - the
upstream's intended member-level contract; [[copperspice-forum]] - the upstream's community.
