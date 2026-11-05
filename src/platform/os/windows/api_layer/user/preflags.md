# Pre-flags for the next TU - session 37, `winqor_api_user.cpp` (the module glue)

Measured 5 Nov 2026 (session 36), shape re-read from the source at the protocol-v3 rewrite.

**Write-once.** The closing session writes this file wholesale; the next session reads it once at boot and edits it
never. There is no previous version to diff against here - it is in git. Do not carry anything forward from it.

## What this TU is (measured from `winqor_api_user.cpp`, 14 lines)

- 5 `#include`: `src/configuration/configuration.h` `:4`, `<buildnumber.h>` behind `#ifdef NDEBUG` `:5`-`:7`,
  `src/qor/module/module.h` `:8`. **No** `user32.h`, **no** `../library.h`, **no** `../returncheck.h`.
- One definition: `qor::Module& ThisModule(void)` `:10`, whose body is a function-local
  `static qor::Module QORModule("Querysoft Open Runtime: Windows API User Module", qor_pp_module_ver_string)` `:12`
  and `return QORModule;` `:13`. No namespace block.
- 0 `User32::` definitions, 0 bindings, 0 `qor_pp_fcontext`, 0 `Library::Call`, 0 `CheckReturn`, 0 `Win32ErrorHandler`.

## Consequence for the method

Steps 1-5b and ids **U4 / U11 / U13 / U17 / U18 / U19 / U25 / U26** have nothing to bite on: there is no wrapper, no
declaration in `user32.h`, no SDK entry point and no callback in this TU. What is left to measure:

- **The module-registration idiom** - `ThisModule()` and its function-local `static qor::Module` - against the sibling
  `api_layer/kernel` and `api_layer/shell` glue and against the other `ThisModule()` definitions in `src`: signature,
  function-local-static shape, and the `qor_pp_module_ver_string` argument.
- **`<buildnumber.h>` behind `#ifdef NDEBUG` (`:5`-`:7`)**: nothing in the body names a build number, so this is a
  **T7**-class dead include under a build guard - check what the sibling glue does before recording it.
- **Hygiene**: `:12` indents with a tab where the rest of the folder indents 4 spaces, and the TU has no
  `qor::win::api` namespace block. Both are U14-class convention notes and **U14 is frozen (D15)** - record only what a
  decision asks for, book no tally.
- **Gate unchanged**: this TU is one of the 38 in the folder-wide compile command in `status.md`.

## Instruments NOT to run here

The export-table capture, the `/EP` binding digest, the wrong-stem scan, the LAYER/BYPASS scan and the callback
`is_convertible` probe have no input in this TU - 0 bindings, 0 SDK calls, 0 callbacks. Running them is what produced the
stale pre-flag block this file replaces (a `winqor_api_user.cpp` heading over `windowprocedure.cpp` measurements; the
retired text and its git locator are in `archive/protocol-v1-2026-11-05.md`).
