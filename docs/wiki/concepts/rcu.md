---
Type: WikiPage
PageType: concept
Title: "rcu"
Status: current
LastVerified: 2026-11-18
Claims: 4
Summary: "The read-copy-update domain: a synchronization scheme whose canonical five-step contract lets many readers traverse a shared structure with no lock on the read side, while an updater makes a new copy, publishes it atomically, waits for pre-existing readers to finish, then retires the old one - and whose defining rule is that updaters cannot block readers or force them to retry."
Judges:
  - none - this page describes a domain, not this tree; it is not falsifiable against a build (see docs/agent/decisions/D56.md)
---

# rcu - the read-copy-update domain

**This page describes a domain, not this tree; it is not falsifiable against a build.**

The resource named by `docs/learning/RCU/RESOURCES.md` (the Wikipedia "Read-copy-update" row,
fetched and verified 2026-10-24) is authority for:

- the **canonical five-step contract**: readers traverse a pointer-protected structure without
  acquiring a lock; an updater allocates a replacement copy, applies its change to the copy,
  publishes the new pointer with an atomic store, waits for a grace period during which no
  pre-existing reader remains, then retires (deallocates) the old version;
- the rule that **updaters cannot block readers or force them to retry** - the read side never
  spins, sleeps or repeats; blocking and waiting live entirely on the update side;
- the **retire/deallocate phase** as a separate, required step: freeing a structure while a
  pre-existing reader can still hold a reference is the domain's classic defect, so reclamation
  is deferred past the grace period;
- RCU's **place against reader-writer locking**: readers trade "no writer has finished" for
  "the snapshot I started on is still alive", which makes reads cheap and updates the expensive,
  coordinated side.

What this page is **not**: a claim about any header in this tree. The repo's RCU-shaped code
(`rcuguarded.h`, `rculist.h` and friends) is described and judged by its own module and gotcha
pages; this page only names the domain those pages reason in.

## See also

[[cs-libguarded]] - the upstream this repo's copies derive from; [[libguarded-contract]] - the
upstream's intended member-level contract.
