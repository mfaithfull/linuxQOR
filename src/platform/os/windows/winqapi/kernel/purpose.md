Intends to provide access to the kernel32.dll API as was. Now remapped to many libraries.

This is persistently available in Windows as all Windows executables are implicitly linked (effectively static linking) to kernel32.dll which is now a shim for a subset of the windows user space kernel interface.

