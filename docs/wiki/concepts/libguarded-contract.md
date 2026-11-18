---
Type: WikiPage
PageType: concept
Title: "libguarded-contract"
Status: current
LastVerified: 2026-11-18
Claims: 4
Summary: "The intended upstream contract domain: the CsLibGuarded documentation (Doxygen, v2.0.1) defines rcu_guarded and the wrapped type's rcu_read_lock / rcu_read_unlock / rcu_write_lock / rcu_write_unlock members - the contract as upstream intends it, which is the yardstick the repo's copies are measured against when they differ."
Judges:
  - none - this page describes a domain, not this tree; it is not falsifiable against a build (see docs/agent/decisions/D56.md)
---

# libguarded-contract - the upstream's intended contract domain

**This page describes a domain, not this tree; it is not falsifiable against a build.**

The resource named by `docs/learning/RCU/RESOURCES.md` (the CsLibGuarded Doxygen v2.0.1 row) is
authority for the **intended contract** of:

- `rcu_guarded<T>` - the guard type that owns a value and mediates concurrent access to it;
- the wrapped type's **lock member quartet** - `rcu_read_lock` / `rcu_read_unlock` for the
  reader side and `rcu_write_lock` / `rcu_write_unlock` for the update side, as the upstream
  defines their acquisition, scope and pairing;
- the version the contract was read at (**v2.0.1**) - a contract statement is always dated to a
  documentation version, because upstream may move.

Use the contract the way a spec is used: the copies in this tree either honor it, deviate
deliberately (and say so in their own pages), or carry a defect. A deviation found by build
evidence belongs on a module or gotcha page with a real judge, not here.

What this page is **not**: the contract of this repo's copies - the copies' behavior is what its
compilers and module pages hold to; this page states what upstream says the names mean.

## See also

[[cs-libguarded]] - the library the contract belongs to; [[rcu]] - the domain the contract
encodes; [[cpp-community]] - where the contract gets reviewed against mainstream practice.
