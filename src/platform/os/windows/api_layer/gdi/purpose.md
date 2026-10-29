# winqor_api_gdi - the gdi32.dll API wrapper

Intends to provide access to gdi32.dll The Windows GDI API: one stateless QOR wrapper per documented entry
point, no argument rewriting, no QOR-side semantics. The wrapper layer keeps the SDK parameter types verbatim so callers see exactly what the Windows API sees.

gdi32.dll is a normal (not implicitly linked) Windows DLL: the module binds its entry points the same way `winqor_api_kernel` does - `qor_pp_useswinapi(shell32, Name)` resolves the symbol from the image loaded in the process image at call time (`GetModuleHandle` + `GetProcAddress`), so there is no import library and no build-time dependency on gdi32.dll. `winqor_api_kernel` is a link dependency of this library because `Win32ErrorHandler`
(the call-site exception filter every wrapper runs under) lives there.
