This file will provide the features available in the QOR.

For now it will list the modules and what state of development they're in

Everything should of course build. Remarkably hard to test locally as CMake will only build the dependencies of one target at a time.
However GitHub builds build absolutely everything so CI/CD is currently going in along with fixes for all the bits that didn't build
because they're not int use by anything so that we can get a passing CI/CD Ver 0.5 on GitHub.

Stable components are usable now, tested and known to work although not certified perfect of course.
Working components are known to have worked for some definition of working but have insufficient testing/usage to trust them as yet.
anything Incomplete is just that. What's there may be working and stable but due to the amount of work needed it's likely to change.
Experimental components may disappear altogether, don't use them except to experiment.
The entire UI subset of libraries is currently considered experimental because the UI design is still being iterated.
    Much of the Windows and Linux specific UI stuff works on it's own terms but the integration into the portable code is largely missing.

qor_application (shared library)            Stable          Application framework
qor_asyncioservice (shared library)         Stable          Asynchronous IO
qor_base64decodefilter (shared library)     Stable          Base64 decode filter for Pipeline
qor_base64encodefilter (shared library)     Stable          Base64 encode filter for Pipeline
qor_current (shared library)                Stable          Current Process and Thread, Thread Context, Thread Local infrastructure
qor_datasink (shared library)               Stable          Binary and structured data sink for Pipeline
qor_datasource (shared library)             Stable          Binary and structured data source for Pipeline
qor_echo (shared library)                   Stable          Echo protocol
qor_error (shared library)                  Stable          Error raising and handling Flyer
qor_fileconnector (shared library)          Stable          File Connector for Pipeline
qor_filesystem (shared library)             Stable          File System provider
qor_host (shared library)                   Stable          Module System Host and Type Registry
qor_injection (shared library)              Stable          Dependency Injection and Type Identity
qor_interception (shared library)           Stable          AOP Interception, Function Context, Call Context, Parameter Validation
qor_issue (shared library)                  Stable          Base Flyer library for Errors and Logs
qor_linereader (shared library)             Stable          A Line At A Time reader for Pipeline
qor_log (shared library)                    Stable          Logging, rasing, handling and forwarding logs
qor_logaggregator (shared library)          Stable          Multi threading log aggregator and handler
qor_module (static library)                 Stable          Module system base library
qor_netclient (shared library)              Stable          Network Client Module, Plug in a protocol to use it.
qor_netserver (shared library)              Stable          Network Server Module, Plug in a protocol to use it.
qor_optionparser (shared library)           Stable          Command Line Options by callback injection
qor_pipeline (shared library)               Stable          Pipeline construction library. Anything to Anything, Push or Pull, with Filters
qor_platform (shared library)               Stable          Platform abstraction module
qor_randomsource (shared library)           Stable          Noise Source for Pipeline
qor_role (shared library)                   Stable          Roles and Features for Application, Customization, Startup/Shutdown control
qor_signals (shared library)                Stable          Signals and Slots from CopperSpice. No custom preprocessor
qor_socketconnector (shared library)        Stable          Socket Connector for Pipeline
qor_sockets (shared library)                Stable          Portable Sockets, Sync and Async
qor_stdfstreamconnector (shared library)    Stable          std::fstream Connector for Pipeline
qor_stdifstreamconnector (shared library)   Stable          std::ifstream Connector for Pipeline
qor_stdifstreamsource (shared library)      Stable          std::ifstream Source for Pipeline
qor_stdinsource (shared library)            Stable          std::cin Source for Pipeline
qor_stdofstreamsink (shared library)        Stable          std::ofstream Sink for Pipeline
qor_stdoutsink (shared library)             Stable          std::cout Sink for Pipeline
qor_stringsink (shared library)             Stable          String Sink for Pipeline
qor_text (shared library)                   Stable          Strings, CoW, String functions, UTF-n and Code Pages
qor_thread (shared library)                 Stable          Threads, Thread Pool
qor_workflow (shared library)               Stable          State Machine
qor_compiler (shared library)               Stable, Incomplete      Compiler abstraction
qor_arch (shared library)                   Stable, Incomplete      Architecture abstraction
qor_loghandler (shared library)             Stable, Incomplete      Basic in thread Log handling
qor_sync (shared library)                   Stable, Changes pending, naming inconsistency. It's a zoo right now
qor_task (shared library)                   Stable, Changes pending, namespace, naming
qor_parser (shared library)                 Semi stable. Will be refactored for Unicode support
qor_parsersink (shared library)             Semi stable
qor_reporting (shared library)              Working, Incomplete
qor_threadmemory (shared library)           Working
qor_stringsource (shared library)           Working
qor_dynamiclibrary (shared library)         Working
qor_http (shared library)                   Incomplete - Request Parser works, Response Generator mostly complete
qor_image (shared library)                  Incomplete
qor_ttf (shared library)                    Incomplete
qor_unicode (shared library)                Incomplete
qor_json (shared library)                   Incomplete - JSON parsing works, builds a minimally simple RO object model. No writing
qor_jpegdecodefilter (shared library)       Experimental, Incomplete
qor_deserializersink (shared library)       Experimental
qor_events (shared library)                 Experimental
qor_generator (shared library)              Experimental
qor_generatorsource (shared library)        Experimental
qor_aggregatesource (shared library)        Experimental
qor_canvas (shared library)                 Experimental UI
qor_console (shared library)                Experimental UI
qor_egl (shared library)                    Experimental UI
qor_layout (shared library)                 Experimental UI
qor_opengles (shared library)               Experimental UI
qor_tui (shared library)                    Experimental UI
qor_ui (shared library)                     Experimental UI
qor_uiselector (shared library)             Experimental UI
qor_widgets (shared library)                Experimental UI
qor_renderer (shared library)               Experimental UI
qor_property (shared library)               Experimental
qor_resources (shared library)              Experimental, Incomplete
qor_textshaping (shared library)            Experimental
qor_serialize (shared library)              Experimental

Windows:

winqor_api_winsock2 (shared library)        Stable                  Socket API
winqor_api_asyncioservice (shared library)  Stable                  AsyncIO, IOCompletionPorts, Cancellation, Co-routine based
winqor_common (shared library)              Stable                  Common Windows stuff like Handles
winqor_exebootstrap (static library)        Stable                  Executable Bootstrap library for Windows Processes (May be redundant)
winqor_filesystem (shared library)          Stable                  Windows File System
winqor_process (shared library)             Stable                  Windows Current Process control
winqor_sharedbootstrap (static library)     Stable                  Bootstrap library for DLLS (May be redundant)
winqor_sockets (shared library)             Stable                  Windows Sockets
winqor_thread (shared library)              Stable                  Windows Threads
winqor_api_kernel (shared library)          Stable, Incomplete      Kernel32 API
winqor_api_user (shared library)            Stable, Incomplete      User32 API
winqor_dynamiclibrary (shared library)      Working                 DLL loading and runtime linking
winqor_api_ole32 (shared library)           Working, Incomplete     OLE32 API
winqor_api_shcore (shared library)          Working, Incomplete     ShCore API
winqor_api_shell (shared library)           Working, Incomplete     Shell API
winqor_com (shared library)                 Working, Incomplete     COM support
winqor_api_xinput (shared library)          Incomplete              XInput API
winqor_api_uxtheme (shared library)         Experimental UI         Theme API
winqor_api_opengl (shared library)          Experimental UI         OpenGL API
winqor_api_comctl32 (shared library)        Experimental UI
winqor_api_direct2d (shared library)        Experimental UI
winqor_api_dwm (shared library)             Experimental UI
winqor_api_gdi (shared library)             Experimental UI
winqor_console (shared library)             Experimental UI
winqor_desktopui (shared library)           Experimental UI
winqor_egl (shared library)                 Experimental UI
winqor_gdi (shared library)                 Experimental UI
winqor_gl (shared library)                  Experimental UI
winqor_gui (shared library)                 Experimental UI
winqor_opengles (shared library)            Experimental UI
winqor_termscreen (shared library)          Experimental UI

Linux:

linqor_filesystem (shared library)          Stable                  Linux File System
linqor_process (shared library)             Stable                  Linux Current Process control
linqor_sockets (shared library)             Stable                  Linux Sockets
linqor_thread (shared library)              Stable                  Linux Threads
linqor_asyncioservice (shared library)      Stable, Incomplete - Needs Cancallation adding
linqor_dynamiclibrary (shared library)      Working                 DSO loading and runtime linking
linqor_console (shared library)             Experimental UI         Linux terminal interaction
linqor_egl (shared library)                 Experimental UI
linqor_opengles (shared library)            Experimental UI
linqor_termscreen (shared library)          Experimental UI
linqor_waylandclient (shared library)       Experimental UI
linqor_waylandegl (shared library)          Experimental UI
linqor_wlpresentationtime (shared library)  Experimental UI
linqor_wlviewporter (shared library)        Experimental UI
linqor_x (shared library)                   Experimental UI
linqor_xdgshell (shared library)            Experimental UI
linqor_xegl (shared library)                Experimental UI

Sample Apps:

basic (executable)                          Stable Sample App               Hello world
client (executable)                         Stable Sample App               Echo client
httpserver (executable)                     Stable Sample App, Incomplete   HTTP Server
options (executable)                        Stable Sample App, Incomplete   Command line Options
outline (executable)                        Stable Sample App               Self version reporting
parallel (executable)                       Stable Sample App               Threads and Log Aggregation
plumbing (executable)                       Stable Sample App               Pipelines
work (executable)                           Stable Sample App               Workflows
roller (executable)                         Stable Sample App               Roles
server (executable)                         Stable Sample App               Echo Server
button (executable)                         Experimental UI
dialogs (executable)                        Experimental UI
editbox (executable)                        Experimental UI
expconsole (executable)                     Experimental UI
mainwindow (executable)                     Experimental UI
menu (executable)                           Experimental UI
minimal (executable)                        Experimental UI

Tests:

QOROpTests (executable)                     Stable Test App
QORTermTests (executable)                   Stable Test App
QORTest (executable)                        Stable Test App
QORTestFileSystem (executable)              Stable Test App
QORTestPipelines (executable)               Stable Test App
