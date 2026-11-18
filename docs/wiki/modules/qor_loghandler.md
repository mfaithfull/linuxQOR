---
Type: WikiPage
PageType: module
Title: "qor_loghandler"
Target: qor_loghandler
Status: current
LastVerified: 2026-11-18
Claims: 24
Summary: "An in-thread log handler: filter by level, format one line, emit it as a signal that qor_logaggregator receives. The forward path is real; the fluent WriteToFileSystem() and WriteToPipeline() setters are comment-only no-ops with no backing member, which is what FEATURES.md means by 'Stable, Incomplete'."
Anchors:
  - { kind: in-tree, path: src/components/qor/loghandler, symbol: qor_loghandler, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/qor/loghandler/CMakeLists.txt, symbol: qor_loghandler, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/qor/loghandler/loghandler.h, symbol: LogHandler, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/qor/loghandler/loghandler.cpp, symbol: LogHandler::Handle, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/qor/loghandler/loghandler.cpp, symbol: LogHandler::WriteToFileSystem, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/qor/loghandler/qor_loghandler.cpp, symbol: ThisModule, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_loghandler --parallel 8
---

# qor_loghandler

Judge GREEN: `cmake --build cmakebuild --target qor_loghandler
--parallel 8` exits 0 and links
`cmakebuild/src/components/qor/loghandler/Debug/qor_loghandler.dll`,
no warning or error line from this target.
`src/components/qor/CMakeLists.txt:2` adds the folder, next to
`qor_logaggregator`, which is this library's reason to exist: the
handler forwards, the aggregator receives (`qor_logaggregator.md`).

- **Folder:** 3 sources + build file, 199 physical lines -
  `CMakeLists.txt` 14, `qor_loghandler.cpp` 14, `loghandler.h` 53,
  `loghandler.cpp` 118.
- **What it is:** `class LogHandler : public qor::IssueHandler<log::Log>,
  public SignalBase` (`loghandler.h:17`) - one base that receives log
  issues, one that emits them; namespace `qor::components` (`:15`).
- **The whole flow is `Handle`** (`loghandler.cpp:90-111`): drop
  anything below `m_level` (`:92-95`), format one string with
  `MessageText` (`:98`), emit it if `m_forward` (`:100-103`), write it
  to `std::cout` if `m_writeToStandardOutput` (`:105-108`), return
  `true`.
- **Defaults:** the delegating ctor `LogHandler() : LogHandler(log::Level::Debug)`
  (`loghandler.cpp:19`) and the init list
  `m_forward(true), m_writeToStandardOutput(false)` (`:14-15`) - so by
  default this handler says nothing on stdout and only forwards.
- **The signal is the product:** `qor_pp_signal_func forward(std::string
  logMessage)` (`loghandler.cpp:113-116`) is one line,
  `qor_pp_emit(forward, logMessage);`, declared at `loghandler.h:41`
  with the comment "don't call this outgoing interface directly". The
  examples connect that signal to `&LogReceiver::ReceiveLog`
  (`examples/advanced/httpserver/httpserverapp.cpp:189`).
- **Two of the three sinks are no-ops:** `WriteToFileSystem()`
  (`loghandler.cpp:66-70`) and `WriteToPipeline()` (`:72-76`) are a
  comment and `return *this;` - "Look for a LogFileWriter to write to
  the filesystem for us" and "LogPipelineWriter". There is no
  `m_writeToFileSystem` or `m_writeToPipeline` member in the class
  (`loghandler.h:47-49` declares exactly `m_forward`,
  `m_writeToStandardOutput`, `m_level`), so the call cannot even be
  remembered. `WriteToStandardOutput(bool)` (`:60-64`) and `Forward(bool)`
  (`:54-58`) are the only two that set anything; all four return
  `*this` and chain.
- Same three names as its sibling, only one implemented: the aggregator's
  matching trio (`logreceiver.h:36-38`) has three backing flags
  (`:47-49`) and a real filesystem sink - and its pipeline branch is a
  `//TODO:` (`logreceiver.cpp:131`). Here two of the three do not even
  record a flag; both halves of the pair advertise an API that writes
  nothing.
- `FEATURES.md:88` rates it **"Stable, Incomplete"** - "Basic in thread
  Log handling" - against `qor_logaggregator`'s plain "Stable" at
  `FEATURES.md:64`; the file's own legend at `FEATURES.md:40` defines
  `Incomplete` as "what's there may be working and stable but due to
  the amount of work needed it's likely to change", which is what the
  two no-op setters above are.
- The formatted line prints a raw counter: `MessageText`
  (`loghandler.cpp:48-52`) returns
  `"{0}: \"{1}\" \nLogged from {2} at time {3}:"` with
  `when.time_since_epoch().count()` (`:50`) - nanoseconds since epoch
  of a high-resolution clock, not a wall-clock time, and an embedded
  `\n` in the middle of the entry that `Handle` then ends with
  `std::endl` (`:107`).
- The level name is an unchecked index: `log::LevelNames[(int)log.what().GetLevel()]`
  (`loghandler.cpp:97`) indexes the `constexpr std::string_view
  LevelNames[]` table (`src/qor/flyers/log/level.h:20`, enum at
  `:11`) with no bounds check between the level filter at `:92` and the
  subscript at `:97`.
- It is a near copy of qor_log's own handler: the same statement,
  `const std::string_view logLevel = log::LevelNames[(int)log.what().GetLevel()];`,
  sits at `src/qor/flyers/log/defaultloghandler.cpp:103`; what this
  folder adds to that body is the forward signal
  (`loghandler.cpp:100-102,113-116`) and a private `GetWhereClause`
  (`loghandler.h:45`).
- Four protected virtuals make the format subclassable: `WhereText`
  (`loghandler.h:33`), `InstanceText` (`:34`), `InExceptionText`
  (`:35`), `MessageText` (`:36-37`); the `where` string falls back to
  the literal `"location unknown"` when a record carries no
  location (`loghandler.cpp:80-87`).
- **Export style differs from its sibling:** 14
  `qor_pp_module_interface(QOR_LOGHANDLER)` decorations are spelled
  member by member (`loghandler.h:21-45`), including the private
  `GetWhereClause` (`:45`), while the class declaration at `:17`
  carries none; `logaggregator.h:47` decorates the class itself.
  `dumpbin /exports` on the built dll prints 14 exports.
- Guard macro is stale by one region: `QOR_PP_H_COMPONENTS_FRAMEWORK_LOGHANDLER`
  (`loghandler.h:4,5,53`) - 3 hits tree-wide, unique, but it says
  `FRAMEWORK` for a folder at `src/components/qor/loghandler`.
- Parameter-name drift between the two halves of one ctor: the header
  declares `LogHandler(log::Level legLevel)` (`loghandler.h:22`), the
  definition spells `LogHandler(log::Level logLevel)`
  (`loghandler.cpp:13`).
- **Link line vs index, clean:** `CMakeLists.txt:14` names 6 libraries
  - `qor_module` `qor_host` `qor_current` `qor_issue` `qor_signals`
  `qor_log` - and `docs/agent/repo_map.md:119` plus
  `docs/agent/deps.dot:139` list the same 6.
- `docs/agent/purposes.md:98` names the three real dependencies in
  parentheses: "Log handler (issue/signals/log)."
- **Consumers: 23 build files** outside the folder link it - 15 under
  `examples/`, 6 under `test/`, and 2 under `src/`: `qor_logaggregator`
  itself (`src/components/qor/logaggregator/CMakeLists.txt:14`) and
  `src/components/io/network/server/CMakeLists.txt:14`.
- No test folder is dedicated to it: no `add_subdirectory(log...)`
  exists anywhere under `test/` (grep over every `test/` CMakeLists),
  so its behaviour is exercised only through the consumers that link
  it.
- The bootstrap is the same 14 lines as `qor_uri`'s:
  `qor_loghandler.cpp:12` names the module
  "Querysoft Open Runtime: Log Handler Module"; it declares no
  `qor_pp_module_requires`, and unlike `qor_logaggregator.cpp:36` it
  registers no `TypeRegEntry` - the handler is handed to a role by
  hand, not looked up as a feature.

## Failure modes

- **A fluent no-op is worse than no method:** `WriteToStandardOutput()`
  and `Forward()` chain, so
  `handler.WriteToFileSystem().WriteToStandardOutput()` compiles,
  chains, and silently drops the first request (`loghandler.cpp:66-76`)
  - there is no flag, no writer, no diagnostic, and the call returns
  `*this` as if it had succeeded.
- The two no-op setters also mean the level and forward state are the
  only state a subclass can read: `m_level` and the two bools are
  `private` (`loghandler.h:43-49`), so a subclass overriding `WhereText`
  cannot ask whether file output was requested - because nothing could
  ever have requested it.
- `Handle` returns `true` for a record it forwarded but never wrote
  (`loghandler.cpp:100-110`): with the defaults
  (`m_forward(true)`, stdout off) a caller that treats the return as
  "logged" is counting on a receiver being connected, and nothing here
  connects one.
- Level filtering is a signed enum comparison with no upper guard:
  `if(m_level > log.what().GetLevel())` (`:92`) followed by the
  unchecked `LevelNames[(int)...]` subscript (`:97`) - the two lines
  together are the whole input validation on the level, and the
  default `Level::Debug` ctor (`:19`) puts the threshold at the
  lowest, most chatty setting.

## See also

[[qor_logaggregator]] [[qor_log]] [[qor_signals]] [[qor_issue]]
[[qor_netserver]] [[qor_module]] [[qor_host]] [[qor_current]]
