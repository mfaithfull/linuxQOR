---
Type: WikiPage
PageType: module
Title: "qor_text"
Target: qor_text
Status: current
LastVerified: 2026-11-16
Claims: 5
Summary: "String classes for UTF-8, UTF-16 and UCS4 plus code-page encodings; qor::String is UTF8String."
Anchors:
  - { kind: in-tree, path: src/qor/essentials/text, symbol: qor_text, verified: 2026-11-16 }
  - { kind: in-tree, path: src/qor/essentials/text/CMakeLists.txt, symbol: qor_text, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_text --parallel 8
---

# qor_text

String classes for UTF-8, UTF-16 and UCS4 plus the code-page encodings, with the operations
implemented once on AbstractString via iterators; `qor::String` is UTF8String
(`docs/agent/purposes.md`).

- Its QOR dependencies are `qor_module`, `qor_host` and `qor_reporting`
  (`src/qor/essentials/text/CMakeLists.txt`).
- Each code page is its own translation unit under `codepage/codepages` - ascii, utf8, utf16, utf32,
  isolatin1, isolatin2, iso8859-3, iso8859-4 - registered through `codepageregistry.cpp`
  (`src/qor/essentials/text/CMakeLists.txt`).
- The target is the parent of two subdirectories and the switch between them is one comment
  character wide (`src/qor/essentials/text/CMakeLists.txt`).

## Failure modes

- `add_subdirectory(shaping)` is commented out, so `qor_textshaping` is not configured by this tree
  and only `qor_unicode` is built (`src/qor/essentials/text/CMakeLists.txt`).
- The buildnumber rule, shared by every target that includes `cmake/updatebuildnumber.cmake`.
