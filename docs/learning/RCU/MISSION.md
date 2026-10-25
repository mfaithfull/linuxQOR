# Mission: Read-Copy-Update (RCU), grounded in the QOR's own implementation

## Why
QOR shares pointer-linked collections across threads - the signal/slot connection lists
(`SharedList<ConnectStruct>` in `src/framework/event/signals/signalbase.h:105`,
`SharedList<const SignalBase*>` in `slotbase.h:61`) - and they are mutated while other
threads iterate them. You want the algorithm from first principles: the invariants that
make it safe, the zombie list that makes reclamation terminate, and why a reader never
blocks. With that model you can read, defend and extend
`src/qor/essentials/datastructures/rculist.h` instead of pattern-matching on it.

## Success looks like
- Recite the RCU contract (register / read-or-copy / unlink / retire / reclaim) from memory.
- Point at the exact lines in `rcuguarded.h` and `rculist.h` that implement each step.
- Explain why the reclaim scan walks from the newest registration toward older ones.
- Explain why `rcu_write_lock` is a one-line call to `rcu_read_lock` (rculist.h:250).
- Predict what a reader sees when a writer mutates mid-traversal: old or new, never torn.
- Name what this header declares but does not currently define.

## Constraints
- Windows only: MSVC `cl.exe` and `clang++.exe` are present; the header is header-only,
  standard library only (`purpose.md`: "No dependencies except the standard library").
- Learning is spaced over sessions in this workspace; do not front-load everything.
- Every claim in a lesson carries a line citation into this repo or a link in RESOURCES.md.

## Out of scope
- Linux kernel RCU machinery (`synchronize_rcu`, epoch counters, hazard pointers) - only as
  contrast for why this userspace variant needs no global sync point.
- The rest of QOR's data structures (frozen/*, fifomap, polymorphic) until RCU is solid.
