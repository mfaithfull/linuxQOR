This static library shgould be linked into every Windows dynamic library of the QOR to provide the DLL entry point
This will need some work if we want to dynamically load QOR Modules on Windows.
Gives us the oportunity to do work on Process and Thread Entry and exit of the module
Might be used to instantiate a basic Window specific error handler on every thread that enters a Windows specific module
Not sure if that would work but might be worth a try.
