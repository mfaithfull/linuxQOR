Conventions for Windows specific code in the QOR

All Windows specific code belongs in:
 - src/platform/os/windows for Windows library code
 - examples/experimental/osspecific/windows for Windows examples

Windows libraries are in 2 layers
 - src/platform/os/windows/api_layer Windows API QOR libraries
 - The rest of src/platform/os/windows/** Windows Implementation libraries

Windows API QOR libraries are:
 - One stateless library per Windows system DLL
 - Allowed to call the underlying Windows API
 - Not unit tested because calling system APIs is not safe
 - Rely on very simple, heavily defensive, code and error handling
 - Assume that the underlying API won't crash if it is available and will throw if it fails
 - The underlying Windows API is documented at https://learn.microsoft.com/en-us/windows/win32/api/
 - API calls are made through the templated functions in src/platform/os/windows/api_layer/library.h

Windows Implementation libraries are:
 - Libraries that provide Windows specific implementations of portable interfaces from the framework or components libraries
 - Libraries that provide Windows specific functionality for other Windows Implementation libraries
 - Not allowed to call underlying Windows API directly. Must call through Windows API QOR libraries
 - Usually Provide stateful classes
