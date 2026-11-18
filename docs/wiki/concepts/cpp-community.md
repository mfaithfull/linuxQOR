---
Type: WikiPage
PageType: concept
Title: "cpp-community"
Status: current
LastVerified: 2026-11-18
Claims: 3
Summary: "The mainstream-practice domain: r/cpp is where an RCU design gets reviewed against mainstream C++ concurrency practice - community review is context that sharpens reasoning, not a build verdict: nothing on the wiki becomes true because a community agrees with it."
Judges:
  - none - this page describes a domain, not this tree; it is not falsifiable against a build (see docs/agent/decisions/D56.md)
---

# cpp-community - the mainstream-practice domain

**This page describes a domain, not this tree; it is not falsifiable against a build.**

The resource named by `docs/learning/RCU/RESOURCES.md` (the r/cpp row) is the place for:

- **reviewing an RCU design against mainstream C++ concurrency practice** - grace-period
  handling, reclamation timing, guard idioms: the broad community's practice is a yardstick for
  whether a design is conventional, novel, or wrong in a well-known way;
- the **status of that review**: context. A community consensus can name a domain term, point at
  a known defect class, or show the conventional shape - it cannot make a claim about this tree
  red or green; only the compilers and the instruments can.

What this page is **not**: a source of authority for repo behavior, and not a place the wiki
quotes decisions from. A design settled here lands as a decision entry with its own artifact.

## See also

[[rcu]] - the design under review; [[libguarded-contract]] - the contract a mainstream review
weighs; [[copperspice-forum]] - the narrow upstream community.
