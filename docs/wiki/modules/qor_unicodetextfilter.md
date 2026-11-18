---
Type: WikiPage
PageType: module
Title: "qor_unicodetextfilter"
Target: qor_unicodetextfilter
Status: current
LastVerified: 2026-11-18
Claims: 15
Summary: "The group's only type-changing filter and the region's only filter with configured consumers: a 636-line BOM-sniffing byte-to-UTF-32 state machine of 27 handlers whose judge prints a C4701 uninitialized-codePoint warning inside the UTF-8 path."
Anchors:
  - { kind: in-tree, path: src/components/data/pipeline/filters/unicodetextfilter, symbol: qor_unicodetextfilter, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/filters/unicodetextfilter/CMakeLists.txt, symbol: qor_unicodetextfilter, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/filters/unicodetextfilter/textfilter.h, symbol: UnicodeTextFilter, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/filters/unicodetextfilter/textfilter.cpp, symbol: UnicodeTextFilter::FirstByteHandler, verified: 2026-11-18 }
  - { kind: in-tree, path: test/components/formats/json/CMakeLists.txt, symbol: qor_unicodetextfilter, verified: 2026-11-18 }
  - { kind: in-tree, path: test/components/protocols/yaml/CMakeLists.txt, symbol: qor_unicodetextfilter, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_unicodetextfilter --parallel 8
---

# qor_unicodetextfilter

Configured at `src/components/data/pipeline/filters/CMakeLists.txt:5`; the folder is 4 files,
750 lines: 14-line `CMakeLists.txt`, 86-line `textfilter.h`, 636-line `textfilter.cpp`, and
`qor_unicodetextfilter.cpp` at 14 lines - a real `ThisModule()` bootstrap here, unlike the
linereader sibling's 0-byte one. It is the heaviest of the five filter folders by a wide margin,
and `FEATURES.md` carries no row for it: the table names the base64 filters (`:49`, `:50`),
jpeg (`:102`) and pipeline itself (`:69`), but never this target.

- `UnicodeTextFilter : public pipeline::InlineProcessor<uint32_t, byte>` (`textfilter.h:21`)
  takes bytes in and emits UTF-32 code points out - the only filter in this family that changes
  type; the ctor seeds 128/128 buffers plus `m_nextHandler = FirstByteHandler`
  (`textfilter.cpp:11-12`).
- The decoder is a byte state machine: 27 private `bool Handler()` members (`textfilter.h:39-65`)
  driven through a member function pointer `m_nextHandler` (`:67`); `FirstByteHandler`
  (`textfilter.cpp:112`) fans out on BOM prefix to UTF-32BE (`:124`), UTF-8 (`:129`), UTF-16BE
  (`:134`), the LE pair (`:139`), else ASCII (`:143`); `ErrorHandler` (`:421`) re-enters the
  stream at `ASCIIHandler` (`:432`) or `UTF8LeadByteHandler` (`:435`).
- `TextInputFormat` (`textfilter.h:11-19`) has 6 values and `SetInputFormat` (`:28`) forces one,
  but `Reset` pins `m_inputFormat = TextInputFormat::ascii` (`textfilter.cpp:74`) whatever was set.
- `EstimateOutputCount` (`textfilter.cpp:86-110`) sizes the write grant per `m_inputFormat`:
  utf8 `(inputCount * 3) >> 1` (`:98`), utf16 `>> 1` (`:102`), utf32 `>> 2` (`:106`), ascii as
  input, always `+ 1` (`:109`) - an over-estimate on every auto-detected path, because no
  supported encoding exceeds one code point per byte.
- `DecodeToUTF32` (`:55-60`) runs the handler chain until one returns false, then accounts:
  `WriteAcknowledge(m_writeCount)` or a count-less `WriteReject()` that drops the whole granted
  block (`:58`); `ReadAcknowledge(m_inputIndex)` or `ReadReject(m_inputSize)` (`:59`).
- The judge compiles both named `.cpp` (`CMakeLists.txt:3-4`) into a working dll with exactly one
  warning: `C4701: potentially uninitialized local variable 'codePoint' used` at
  `textfilter.cpp:413` - inside `UTF8FollowByteHandler`.
- It is the only filter in the region with configured, linked consumers: the
  `test/components/protocols/yaml` target (`CMakeLists.txt:15`, configured at
  `test/CMakeLists.txt:131`) and the `test/components/formats/json` target
  (`test/components/formats/json/CMakeLists.txt:17`, configured at `test/CMakeLists.txt:123`),
  whose `jsonreader.h:14` includes `textfilter.h` and holds two `UnicodeTextFilter` members
  (`:52`, `:85`).
- Link line is a quintet `qor_module qor_host qor_text qor_signals qor_pipeline`
  (`CMakeLists.txt:14`) - unlike the filter-family trio, it pulls `qor_host` and `qor_text`,
  matching `docs/agent/repo_map.md:89` and `docs/agent/deps.dot:110`.
- No unit test touches the filter directly: the pipeline test rows at `test/CMakeLists.txt:63-67`
  cover stdinsource/stdoutsink/ifstreamsource/ofstreamsink and the only filter test file
  (`test_httpfilter.cpp`, `:68`) is commented out - it is exercised only through the json and
  yaml targets.
- `docs/agent/purposes.md:90` groups it with the four filter siblings under "pipeline filters
  (inline buffer+function)" - the wrong shape for this one, which is an `InlineProcessor` with a
  27-state machine, and the folder carries no `purpose.md` of its own.

## Failure modes

- Grant-refusal spin: `Process` (`textfilter.cpp:28-31`) loops while `readTarget > 0` and both
  buffer capacities are positive, but if `ReadRequest` (`:39`) or `WriteRequest` (`:44`) returns
  null, `SetupState` (`:62-70`) zeroes `m_inputIndex` again and the loop body progresses by
  nothing - the reject logic lives in `DecodeToUTF32`, which a null grant never reaches.
- One corrupt byte recodes the remainder: `ErrorHandler` (`:421-435`) resumes mid-stream as
  ASCII or UTF-8, and the class exposes no error flag anywhere (`textfilter.h:69-81` is the full
  member list), so corrupted UTF-16/32 data past the bad byte is silently reinterpreted -
  `m_replacementCodePoint` `0xFFFD` (`:81`) is the only visible trace.
- The compiler corroborates the UTF-8 path: `C4701` at `:413` means MSVC found a path into
  `UTF8FollowByteHandler`'s use of `codePoint` with no prior write - an uninitialized code point
  can be acknowledged downstream.
- `Reset` discards an explicit format: `SetInputFormat` then any `Reset` re-pins to `ascii`
  (`:74`), so a caller forcing `leutf16` gets BOM auto-detection back after a reset.
- The sizing invariant is silent: the per-format divisors at `:98-106` are safe only while every
  supported encoding packs at most one code point per byte; nothing in the file asserts it, so a
  future expanding mode would under-allocate the granted block written through `m_space` (`:44`,
  `:47`) with no bounds check in sight.

## See also

[[qor_module]] [[qor_host]] [[qor_text]] [[qor_signals]] [[qor_pipeline]]

Not yet paged: its consumers `test/components/formats/json` and `test/components/protocols/yaml`
(test targets, not wiki units), the `pipeline::InlineProcessor` base it derives from
(`src/framework/data/pipeline/inlineprocessor.h`, in scope of `qor_pipeline`), and the sibling
sinks it feeds - `parsersink` links it only in test trees.
