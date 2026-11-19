---
Type: WikiPage
PageType: module
Title: "qor_workflow"
Target: qor_workflow
Status: current
LastVerified: 2026-11-19
Claims: 10
Summary: "Two state-machine engines in one target; the widest link line in the app layer, and it trades headers with qor_application over no link edge."
Sources:
  - hook-cbffd54d10-qor_workflow
Anchors:
  - { kind: in-tree, path: src/framework/app/workflow, symbol: qor_workflow, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/app/workflow/CMakeLists.txt, symbol: qor_workflow, verified: 2026-11-18 }
  - { kind: in-tree, path: src/framework/app/workflow/workflow.h, symbol: Workflow, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/app/workflow/fastflow.h, symbol: Fastflow, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/app/workflow/iworkflow.h, symbol: IWorkflow, verified: 2026-11-19 }
  - { kind: in-tree, path: src/framework/app/workflow/purpose.md, symbol: proivde, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/app/workflow/workflow.cpp, symbol: m_result, verified: 2026-11-19 }
  - { kind: in-tree, path: src/framework/app/workflow/iworkflow.h, anchor: commit:cbffd54d10152cc94089b76550cb691b5414c725, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_workflow --parallel 8
---

# qor_workflow

A reusable state-machine outline "which can be modified by extension only"
(`src/framework/app/workflow/purpose.md`). Built SHARED from `qor_workflow.cpp`, `workflow.cpp` and
`fastflow.cpp` at version 0.5.0 (`src/framework/app/workflow/CMakeLists.txt`).

- Nine QOR dependencies - `qor_module qor_host qor_current qor_issue qor_error qor_log qor_thread
  qor_tef qor_threadmemory` (`src/framework/app/workflow/CMakeLists.txt`) - the widest link line in the
  app layer, and `docs/agent/repo_map.md` copies it token for token. 31 `CMakeLists.txt` name the target.
- Two engines, one target: `qor::Workflow` keeps a `std::stack<ref_of<workflow::State>::type>` and
  `std::function` hooks (`workflow.h`), while `qor::Fastflow` keeps a
  `std::stack<fastflow::Step*, std::vector<fastflow::Step*>>`, `tef::Function` hooks and a `TEFAllocator`
  member (`fastflow.h`). Both derive from `workflow::IWorkflow`, and the only two `IWorkflow`
  inheritors in the tree are those two classes.
- `State` has no virtual of its own beyond the destructor. Its four members are `std::function`
  assigned in its constructor from the owning workflow - `Enter = std::bind(&Workflow::Enter, workflow);`
  and the same for Suspend/Resume/Leave (`src/framework/app/workflow/workflow.cpp`) - so a state cannot
  behave differently from its workflow; every difference is a `Workflow::Enter` override. That is the
  mechanism behind "modified by extension only".
- `QOR_WORKFLOW` has 43 hits in 2 files, annotating members rather than classes: each public member of
  both engines carries its own `qor_pp_module_interface(QOR_WORKFLOW)` and each class declaration
  carries `qor_pp_module_interface_gcc` (`workflow.h`, `fastflow.h`), against the single hit for
  [[qor_role]] and the single hit for [[qor_optionparser]].
- The purpose file is the only one of the four `src/framework/app` library folders here that exists,
  and it is not the source of the index line: it reads "intends to proivde a reusable state machine
  outline which can be modified by extension only to perfrom any linear or stack based state chart"
  (`src/framework/app/workflow/purpose.md`), two misspellings that
  `docs/agent/purposes.md` has already silently corrected to "Reusable state-machine outline;
  extend-only to perform any linear or stack-based state chart."
- `Run()` prints its failures to `std::cerr` in both engines - 3 lines in `workflow.cpp`, 3 in
  `fastflow.cpp`, 6 of the 33 `std::cerr` lines in `src` - while its own link line names `qor_log`, and
  `memory-bank/codingConventions.md` states "No printf/cout/cerr - use `qor_log`".

## Failure modes

- `iworkflow.h` includes `"../application/irunable.h"` (`src/framework/app/workflow/iworkflow.h`) - a
  relative path into a folder owned by a different target - and `qor_application`'s own public header
  includes the other way: `#include "src/framework/app/workflow/iworkflow.h"`
  (`src/framework/app/application/application.h`, whose interface uses `ref_of<workflow::IWorkflow>::type`
  in three member declarations). Neither link line names the other: `qor_workflow`'s is the nine
  targets above (`src/framework/app/workflow/CMakeLists.txt`) and `qor_application`'s is
  `qor_module qor_thread qor_sync qor_platform qor_role`
  (`src/framework/app/application/CMakeLists.txt`). `docs/agent/repo_map.md` puts them in the same
  numbered row 10, so the index shows a pair, not a cycle, and no build order can order them.
- `Workflow::operator=` copies all four members - `m_result` first, then `m_complete`,
  `m_StateStack` and `m_initialState` (`src/framework/app/workflow/workflow.cpp`) - so a copy
  carries the exit code. It did not until commit `cbffd54d` added `m_result = src.m_result;`;
  `Run()` ends with `return m_result;` and the copy constructor is
  `Workflow(const Workflow& src) : Workflow() { *this = src; }` (`workflow.cpp`), so the omission
  once reached copies too - the shape the `Role` copy on the [[qor_role]] page still has.
- An unextended `Workflow` is legal and does almost nothing. `Workflow` overrides both pure virtuals of
  `IWorkflow` (`workflow.h`), so it is instantiable despite the "extension only" purpose, and
  `Workflow::Enter() { m_complete = true; }` (`workflow.cpp`) combined with
  `while(!IsComplete()) { CurrentState()->Enter(); }` (`workflow.cpp`) means a bare workflow runs
  `Enter` once, drains the stack and returns `m_result` - 0.
- The include guard is `QOR_PP_H_WORKFLOW_INTERFACE` (`src/framework/app/workflow/iworkflow.h`),
  corrected from the `INTERFFACE` misspelling by commit `cbffd54d`. Three `INTERFFACE` hits remain
  in the tree, all `QOR_PP_H_ROLE_FEATURE_INTERFFACE` in `src/framework/app/role/ifeature.h`.

## See also

[[qor_application]] [[qor_role]] [[qor_module]] [[qor_log]]
