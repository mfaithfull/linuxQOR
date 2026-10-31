# Notes

Teaching preferences and working notes. Check back here before each lesson.

## User's stated posture
- Wants RCU **from first principles** (invariants, zombie list, non-blocking reads) *before*
  application work or repair work. Teach the model, then map it onto this repo.
- No preference stated about communities; both forum.copperspice.com and r/cpp are offered.

## How this workspace is being run
- One concept per lesson. Lesson 0001 = the contract plus the zombie list (the two things
  the model needs before anything else).
- Every claim carries a line citation into `rcuguarded.h` / `rculist.h`, or a RESOURCES.md link.
  Claims with neither are marked as inference, not fact.
- Quiz rules held from the skill: options equal in word count within a question, correct
  position varies across questions, no formatting clues.

## For a later lesson (do not spoil in 0001)
Confirmed by reading the header, not assumed:
- `clear()` (rculist.h:86) and the four `insert()` overloads (rculist.h:88-93) are **declared
  but never defined** in this header.
- `iterator::operator--` (rculist.h:301) and `const_iterator::operator--` (rculist.h:375)
  use `m_current->prev.load()`, but `node` (rculist.h:121-126) has no `prev` member - it is
  named `back`. These bodies only fail when instantiated.
- `node::deleted` (rculist.h:124) is a plain `bool`, not atomic. It is read and written only
  inside `erase()` under the write mutex (rculist.h:591, 593), so it is writer-only - do not
  describe it as a data race without checking that claim.
- `emplace()` (rculist.h:96) is declared and never defined; only `emplace_front` (522) and
  `emplace_back` (565) have bodies. `pop_front`/`pop_back` do not appear in this header at all.
- `reverse_iterator`, `const_reverse_iterator`, `end_reverse_iterator` are declared
  (rculist.h:60, 61, 63) and never defined anywhere in the header.
- `status.md:24` says "the base libraries are now clean and some clean examples can be built",
  which the `operator--` finding above contradicts for this header.

## Workspace state (update as lessons ship)
- 2026-10-24 - MISSION.md, RESOURCES.md, GLOSSARY.md, learning-records/0001,
  reference/rcu-contract.html and lessons/0001-rcu-the-contract.html written. Every line
  citation in both documents re-checked against the sources the same day; the lesson's quiz
  JS parses and runs, and its option word-counts were fixed to be equal per question.
- Next: lesson 0002 on the defects once the contract is demonstrated; then a second spaced
  quiz session against the reference sheet alone.
