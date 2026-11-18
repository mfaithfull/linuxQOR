---
Type: WikiPage
PageType: concept
Title: "copperspice-forum"
Status: current
LastVerified: 2026-11-18
Claims: 3
Summary: "The upstream-community domain: the CopperSpice forum is where questions specific to cs_libguarded - the upstream of this repo's exact guarded-header copies - belong, and a forum answer is community guidance, not specification: it can inform but never settles what the code does."
Judges:
  - none - this page describes a domain, not this tree; it is not falsifiable against a build (see docs/agent/decisions/D56.md)
---

# copperspice-forum - the upstream-community domain

**This page describes a domain, not this tree; it is not falsifiable against a build.**

The resource named by `docs/learning/RCU/RESOURCES.md` (the CopperSpice forum row) is the place
for:

- **cs_libguarded-specific questions** - the upstream of this repo's exact copies, so a question
  about intended behavior, a suspected upstream defect, or a contract reading lands in the
  community that maintains it;
- the **status of an answer there**: community guidance. It can sharpen a reading of the contract
  ([[libguarded-contract]]) or expose an upstream intent the docs do not state; it **never**
  settles what this tree's code does - only its build and its pages hold that.

What this page is **not**: a log of forum threads, and not a source the wiki quotes as authority
for repo behavior. If a forum answer ever changes a repo claim, the change lands on the claim's
own page with that page's judge.

## See also

[[cs-libguarded]] - the library the forum serves; [[cpp-community]] - the broader review ground.
