# Session 27 - processthread.cpp (closed 3 Nov 2026)

5 wrappers - the whole `user32.h:255 //Process and Thread functions` section (`:256`-`:260`), header order, all static,
all defined here (`:81`, `:90`, `:99`, `:106`, `:115`): 0 U4, 0 U11, 0 U13, 0 U17, 0 U18. Source edits were **whitespace
only** - one TAB line (`:29`'s two tabs -> 8 spaces, the only TAB in the whole review queue), three stray spaces before
the closing `)` of an argument list (`:21`, `:46`, `:54`), and per the user's decision the TU's five `Call<>` sites
normalised to the folder's tight spelling (4 were spaced, 1 tight), which moves folder U14(a) **178/250 -> 174/254**.
The TU is 57 -> 122 lines (64-line contract block at `:16`-`:79`; the pre-edit line numbers quoted above for the
whitespace fixes are the 57-line file's).

**Bindings (D13).** 5/5 resolve and none needed changing: all five stems are single exports with no A/W family and **no
`winuser.h` macro at all**, so their bare `qor_pp_useswinapi` (`:84`, `:93`, `:102`, `:109`, `:118`) is the only spelling
that can bind them - `AttachThreadInput` F/0002E9B0, `GetGuiResources` 15F/0002EF50, `IsWow64Message` 24F/0004C4F0,
`UserHandleGrantAccess` 3D8/0002FEF0, `WaitForInputIdle` 3E9/0002B8F0. `tmp/_tmp_pt_class_s27.ps1` over the `/EP`
literals: 5/5 present in the export table, 0 macros, 0 P2, 0 P6. Folder census unchanged at **428 bindings / 1
unresolvable** (`window.cpp:124`); the wrong-stem scan has no `processthread.cpp` row (3 rows, all known-benign).

**Signatures.** 5/5 declaration-definition pairs parse, 0 hdr-vs-def and 0 def-vs-sdk parameter-name divergences, all
five `Call<>` template lists restate the definition's parameter types exactly, returns all agree (BOOL, DWORD, BOOL,
BOOL, DWORD at `winuser.h:4103`, `:12417`, `:3679`, `:14925`, `:4128`). This is only the second TU where the standing
`_tmp_sigcmp.ps1` could check the call sites at all: its call-site pattern read one source line, so every **wrapped**
call printed `CALL NOT PARSED` - fixed in session 27 to join to the terminating `;` (3 of this TU's 5 sites are wrapped).

**Return conventions (U2, recorded not changed).** 3 of the library's 4 `CheckReturn` sites are here:
`AttachThreadInput`/`UserHandleGrantAccess` (`BoolCheck`) and `GetGuiResources` (`CheckNonZero< DWORD>`, the kernel
layer's own spelling - `kconsole.cpp` uses it 5 times). `WaitForInputIdle` is deliberately unchecked and must stay so:
it returns `WAIT_OBJECT_0` (0) on success, `WAIT_TIMEOUT` on timeout and 0xFFFFFFFF on failure, so the non-zero check
this file uses two lines above would call its success a failure. `IsWow64Message` returns a state, not a verdict. G6
keeps the three live checks on every call, so a legitimate zero and a swallowed SEH fault stay indistinguishable (U7).

**U16.** 3 of 5 prototypes version-guarded, **5 of 5 `WINAPI_PARTITION_DESKTOP` only**: `GetGuiResources`
`#if(WINVER >= 0x0500)` (`winuser.h:12413`), `UserHandleGrantAccess` `#if(_WIN32_WINNT >= 0x0500)` (`:14920`),
`IsWow64Message` `#if(_WIN32_WINNT >= 0x0501)` (`:3675`, the item's second 0x0501 case). `AttachThreadInput` and
`WaitForInputIdle` sit in the Desktop sub-block `:4098`-`:4132` with no version guard of their own. Trap found:
`:4121`-`:4122` is an **empty** `#if (_WIN32_WINNT >= 0x602)` / `#endif` pair sitting between the two declarations, so a
backward guard scan without a depth counter reports 0x0602 for `WaitForInputIdle`.

**U19.** 1 LAYER / 0 INDIRECT / 0 BYPASS / 0 MEMBER over the 5 stems plus their A/W/T spellings. The only in-tree caller
of anything here is `ui/gui/view/handlers/messagehandler.cpp:246` (`User32::IsWow64Message()`), live code in a compiled
target (`ui/gui/CMakeLists.txt:60`); the other four entry points have no in-tree caller, which under D14 is published
surface, not a finding.

**Coverage measure (new instrument `_tmp_pt_coverage_s27.ps1`).** Of 1,018 user32 export names, **370** have no member
declaration in `user32.h` at all - 114 of them declared in the installed `winuser.h` (DPI-awareness, pointer-input,
touch/pen and display-config families) and 256 not declared there. This section contributes none. It also corrected a
claim this TU first wrote down: `SetThreadInputBlocked` (371) really is declared nowhere, but `DdeSetUserHandle` (9C)
*is* declared - at `user32.h:528`, as one of the 34 non-static `const` DDE members of U4's largest block.

**Gate.** `tmp/_tmp_cl_all_s27.log`: 38/38 at `/W4`, exit 0, 0 diagnostics, run 08:03:07 after the TU's last edit at
08:00:59. `/EP` pre 453,292 -> final 453,357 lines with the non-blank multiset identical except the 7 whitespace-touched
lines, and the 5 binding literals byte-identical. Citation verifier `tmp/_tmp_pt_cite_s27.ps1`: 28 citations in the
block, 0 unresolved, 5 def + 5 binding self-citations asserted to land on the right construct.
