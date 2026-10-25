# RCU Glossary (QOR)

The canonical language for this workspace. Every lesson, reference and learning record uses
these words exactly. Definitions are tight and, where possible, anchored to a line in
`src/qor/essentials/datastructures/`.

## The contract

**Read-copy update**:
A synchronization scheme where readers traverse pointer-linked data with no lock at all, and
one writer at a time copies-then-unlinks, deferring the free until no reader can still see
what was removed.
_Avoid_: read-write lock, shared_ptr sharing, lock-free list

**Register / unregister**:
A reader announcing and then withdrawing its presence in the list's zombie list; the RCU
read_lock / read_unlock pair. This is a *declaration of intent*, not a lock.
_Avoid_: acquire or release a mutex

**Handle**:
The RAII object handed back by `lock_read` or `lock_write`; the only way to reach guarded data
(`rcuguarded.h:40-41`).
_Avoid_: smart pointer, proxy, lock guard

**Lazy registration**:
A handle registers with the list on its first dereference, not at construction; an unused
handle registers nothing (`rcuguarded.h:74-81`, 122-129).
_Avoid_: eager locking

**Write mutex**:
The `std::mutex` stored inside the list (`rculist.h:156`) that serializes writers against
each other. Readers never touch it.
_Avoid_: the lock around reads

## The list

**Zombie list**:
The list's lock-free stack of retired nodes *and* active reader-guards, newest at the head
(`rculist.h:154`, pushed at 191-194 and 623-626).
_Avoid_: free list, garbage list, deferred list

**Retire**:
Unlink a node from the live chain and hand it to the zombie list, leaving its memory valid
for readers that still point at it (`rculist.h:589-631`).
_Avoid_: delete, free, destroy

**Guard (rcu_guard)**:
The object stored inside the list that a handle borrows in order to register and unregister
itself (`rculist.h:65-67`, 165-180).
_Avoid_: std::lock_guard, RAII lock

**Reclaim scan**:
At unregister, walk from this guard toward older registrations and free everything older than
the newest still-active reader (`rculist.h:206-245`).
_Avoid_: garbage collection, cleanup pass

**Live chain / retired node**:
A node reachable from `m_head` by following `next` (live); a node unlinked from that chain
but still held by the zombie list (retired).
_Avoid_: valid/invalid, used/unused

## In this repo

**SharedList<T>**:
`rcu_guarded<rcu_list<T>>` (`rculist.h:635`) - an RCU-linked list whose only access route is
a handle.
_Avoid_: shared list of shared_ptrs

**Torn read**:
A read that observes a half-updated node - a pointer updated but its payload not, or a node
freed while still being walked. RCU's whole point is that this cannot happen.
_Avoid_: partial update, inconsistent read
