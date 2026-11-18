---
Type: WikiPage
PageType: module
Title: "qor_role"
Target: qor_role
Status: current
LastVerified: 2026-11-16
Claims: 10
Summary: "Role and Feature container - the base every application derives from; copyable, and a copy leaves its features pointing at the original."
Anchors:
  - { kind: in-tree, path: src/framework/app/role, symbol: qor_role, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/app/role/CMakeLists.txt, symbol: qor_role, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/app/role/role.h, symbol: Role, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/app/role/irole.h, symbol: IRole, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/app/role/ifeature.h, symbol: IFeature, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/app/role/getfeature.h, symbol: GetFeature, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_role --parallel 8
---

# qor_role

"Roles (real apps built from Roles + Features)." (`docs/agent/purposes.md`). Built SHARED from
`qor_role.cpp` and `role.cpp` at version 0.5.0 (`src/framework/app/role/CMakeLists.txt`).

- 53 of the 281 `CMakeLists.txt` under `src`, `test` and `examples` name it. Its link line is
  `qor_module qor_host qor_current qor_sync qor_platform qor_error`
  (`src/framework/app/role/CMakeLists.txt`), which `docs/agent/repo_map.md` copies token for token.
- The gate token `QOR_ROLE` is spelled the way the convention says and carries no `QOR_PP_` prefix; it
  has 1 hit in `src` - the export marker on `class Role : public IRole` (`src/framework/app/role/role.h`).
  `IRole` and `IFeature` are header-only interfaces and export nothing.
- `qor_role.cpp` defines `ThisModule()` returning a function-local `qor::Module` built with the default
  `doRegister` (`src/framework/app/role/qor_role.cpp`), so this target registers itself with
  [[qor_host]] through the same `TheHost()` call its own construction triggers - which is exactly what
  [[qor_optionparser]] does not do.
- `irole.h` reaches five `src/qor` headers, and two of them (`src/qor/memory/reference/reference.h`,
  `src/qor/essentials/objectcontext/anyobject.h`) live in folders that hold no `CMakeLists.txt` at all.
  That is why the link line names six targets where the includes suggest eight.
- The whole feature contract is four pure virtuals plus three templates. `IRole`'s templates build the
  key and the object at the caller's instantiation - `AddFeature<TFeature>()` calls
  `new_ref<TFeature>()` and keys it on `guid_of<TFeature>::guid()` (`src/framework/app/role/irole.h`) -
  and `Role` stores the result in `fifo_map<const GUID*, ref_of<IFeature>::type>`, keyed on the GUID
  **pointer** (`src/framework/app/role/role.h`).
- `IFeature` holds a raw `IRole* m_Role{ nullptr }` that only `friend class Role` may write
  (`src/framework/app/role/ifeature.h`), and exactly one line in the tree writes it:
  `feature->m_Role = this;` in `Role::AddFeature` (`src/framework/app/role/role.cpp`). A feature's
  route to its siblings depends on that one assignment - `src/components/qor/logaggregator/logaggregator.cpp`
  does `m_Role->GetFeature<thread::ThreadPool>()`.

## Failure modes

- `Role` is copyable and the copy does not re-point the back-reference. The copy constructor copies the
  map and nothing else - `Role::Role(const Role& src) : m_mapFeatures(src.m_mapFeatures){ }` - and
  `operator=` does the same assignment (`src/framework/app/role/role.cpp`), while `m_Role` is only ever
  written by `AddFeature`. Both are public and undeleted (`src/framework/app/role/role.h`), so a copied
  or assigned `Role` hands out features whose `m_Role` still names the original.
- `getfeature.h` states a rule the tree breaks. Its comment reads "The QOR itself can never include
  this header as it requires the defintion of any derived application class in use to already be
  present" (`src/framework/app/role/getfeature.h`), and it includes
  `src/framework/app/application/builder.h` - the header of the lib that links this one. Ten files
  include it: eight examples, and two that are QOR code, `src/framework/data/resources/manager.cpp`
  and `src/framework/data/resources/types/pathresource.cpp`. Those compile and link only because
  `qor_resources` names `qor_application` in its own link line
  (`src/framework/data/resources/CMakeLists.txt`) - and `docs/agent/repo_map.md` places
  `qor_resources` at layer 9 and `qor_application` at layer 10 of its build order, so the fence is
  prose in a header comment and nothing else.
- `IFeature(const IFeature&&) = delete;` is not a move constructor (`src/framework/app/role/ifeature.h`):
  a move constructor takes `IFeature&&`, so this line deletes an overload nothing can select. The
  non-copyable, non-movable type the author wanted is already what the previous line produces - the
  deleted copy constructor suppresses the implicit move - so the line reads as intent and does nothing.
- The include guard is `QOR_PP_H_ROLE_FEATURE_INTERFFACE` - `INTERFFACE`
  (`src/framework/app/role/ifeature.h`) - where the folder's other three guards follow the
  `QOR_PP_H_`-plus-name convention of `memory-bank/codingConventions.md` cleanly. The folder also
  carries no `purpose.md`, so the repo's per-folder rule has nothing to say here either.

## See also

[[qor_application]] [[qor_host]] [[qor_optionparser]] [[qor_module]]
