---
Type: Index
Description: "Index of reusable tools. Format is specified in docs/agent/TOOL-FORMAT.md. File: paths here are relative to memory-bank/; the File: paths inside a tool doc's Scripts: are relative to the repo root. Scripts live in the tracked tools/ tree, never in tmp/ (tmp/ is gitignored at .gitignore:41 and cleaned per task)."
index:
  - UseWhen: "sanity check that tool loading works at all"
    Scope: "memory-bank tooling"
    File: "tools/sanity/sanity.md"
  - UseWhen: "fetch a Learn API page for a slice of table rows and get each page's Return value section verbatim, with the row's types and caller count"
    Scope: "docs/learn.microsoft.com pages against tmp/<prefix>endpoint_table.csv - verdict authorship input, decides nothing"
    File: "tools/doc-fetch/tool.md"
---
