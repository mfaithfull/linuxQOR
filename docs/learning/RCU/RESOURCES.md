# RCU Resources

## Knowledge

- [Wikipedia: Read-copy-update](https://en.wikipedia.org/wiki/Read-copy-update)
  Use for: the canonical five-step contract, the rule that updaters "cannot block readers or
  force them to retry", the retire/deallocate phase, and RCU's place against reader-writer
  locking. Fetched and verified 2026-10-24.

- [GitHub: copperspice/cs_libguarded](https://github.com/copperspice/cs_libguarded)
  Use for: the upstream this code derives from. Both QOR headers carry the libguarded file
  header ("This file is part of libguarded", copyright 2015-2018 Barbara Geller / Ansel
  Sermersheim). Header-only, C++20, BSD 2-clause. Use when reasoning about what the code
  *should* do versus what this copy does.

- [CsLibGuarded documentation (Doxygen, v2.0.1)](https://www.copperspice.com/docs/cs_libguarded/index.html)
  Use for: the intended contract for `rcu_guarded` and the wrapped type's
  `rcu_read_lock` / `rcu_read_unlock` / `rcu_write_lock` / `rcu_write_unlock` members.

- Local: `src/qor/essentials/datastructures/rcuguarded.h` (168 lines), `rculist.h` (639 lines),
  `detail/uptrtrait.h`
  Use for: the primary object of study. Lessons cite these line numbers.

- Local: `src/framework/event/signals/signalbase.{h,cpp}`, `slotbase.h`
  Use for: how `SharedList<T>` is used in anger - `lock_read()` + range-for
  (signalbase.cpp:128-131), `lock_write()` + `push_back` (signalbase.cpp:109-116).

- Local: `src/qor/essentials/datastructures/purpose.md`, `status.md`, `BUILDING.md`
  Use for: scope (header-only, std only), claimed status ("the base libraries are now clean and
  some clean examples can be built", status.md:24 - a claim the `operator--` defect contradicts),
  and which compilers the project targets.

## Wisdom (Communities)

- [CopperSpice forum](https://forum.copperspice.com)
  Use for: questions specific to cs_libguarded, the upstream of this exact code.
- [r/cpp](https://www.reddit.com/r/cpp/)
  Use for: reviewing an RCU design against mainstream C++ concurrency practice.
- Local: build `QORTest` or a scratch TU with `cl.exe` / `clang++.exe`
  Use for: real feedback - compile the header before reasoning about it.

## Gaps

- No verified source yet for production C++ RCU outside this repo (Meta Folly `rcu`, Abseil
  RCU, `rcupdtr`): fetches 404'd on 2026-10-24. Fill before any lesson on alternatives.
- No repo-internal doc on RCU: memory-bank mentions it once
  (`systemPatterns.md:136` - "string; rcu list (CopperSpice-derived)"), and `docs/` has none.
