# Mission fixed: first-principles RCU, not application or repair

Given four candidate goals, the user picked "reason about RCU from first principles: the
invariants, the zombie list, and why readers never block writers" - over using `SharedList`
in new code and over auditing the implementation's gaps. That sets the floor for what to
teach next: no application work and no bug-hunting until the model itself is demonstrated,
not just recited.

**Implications**
- Lesson 0001 must be theory-plus-code-map, not "here is how to use it".
- The audit findings already noted in NOTES.md (undeclared `insert`/`clear` definitions,
  `node::prev` vs `back` in `operator--`) are deliberately withheld from 0001; they predict
  a strong later session once the model holds.
- Evidence so far is *choice*, not competence. Nothing here proves the user can do RCU yet,
  so 0001 assumes no prior knowledge of the algorithm and starts from the contract.

**What 0001 was built to produce (shipped 2026-10-24)**
`lessons/0001-rcu-the-contract.html`: the contract mapped line-by-line onto
`rcuguarded.h`/`rculist.h`, the four invariants, the zombie list and its unregister scan,
then a seven-question click-to-answer quiz (one attempt each) and a typed seven-step ordering
task. Evidence bar for the next session: a score that holds when re-answering against the
reference sheet alone, and the typed sequence written without the glossary open. Reciting is
not proof of understanding - the per-question explanations are what separate storage from
fluency.
