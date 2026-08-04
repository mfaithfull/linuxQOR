A non comprehensive list of QOR features.

 - Almost everything is optional. You have full control. It's just C++ and you use whatever you need.
 - Everything works together out of the box. 
 - Cross platform, not just multi-platform. OS and compiler dependencies are factored out. Easy to port.
 - BSD, MIT and Boost licensed.
 - Almost everything can be overriden or replaced at need where configuration doesn't cut it.
 - Easy Modular system. A pattern and libraries for building 'perfect' C++ DLLs with controlled dependencies and APIs, No need for header only.
 - Adding a new module is 5-10 minutes work.
 - Role and Feature based Application framework with argument parsing and full customization.
 - TDD tool set. Assertion, Test and Mocking frameworks that work together.
 - constexpr data structures, map, set, string
 - State machine based Workflows.
 - Type erased functions
 - Class registry. An in process ORB for registering implementations of specific interfaces.
 - Policy based smart pointer and integrated memory management system. no more new and delete.
 - Thread Pool and integrated async task system
 - Modular data pipelines. Plug together sources, filters and sinks to achieve whatever you need.
 - Extensible error handling system using the flyer pattern.
 - Logging system using the flyer pattern.
 - Pre-C++26 refelection using pfr. 
 - Asynchronous task library
 - Asynchronous IO service
 - Signals and Slots, multithread and native C++ no pre-compiler.
 - Delegates
 - Partial localization support. Classes to support working with many types of Unicode data.
 - Filesystem support over std::filesystem
 - Spin up a network server with a protocol plugin in a few lines

*Coming eventually (or soon if everyone pitches in): Run a TUI or GUI from the same easy to use scene template.
The Wayland layer, X Windows layer and Windows GDI layers are written, the layout libary works, The terminal code is mostly ported from FTXUI
The JSON parser is written. Some assembly is still required.*

Below is the list of modules and what state of development they're in

Everything should of course build and does on GitHub's Ubuntu-latest as of 27/07/2026

Stable components are usable now, tested and known to work although not certified perfect of course.
Working components are known to have worked for some definition of working but have insufficient testing/usage to trust them as yet.
anything Incomplete is just that. What's there may be working and stable but due to the amount of work needed it's likely to change.
Experimental components may disappear altogether, don't use them except to experiment.
The entire UI subset of libraries is currently considered experimental because the UI design is still being iterated.
    Much of the Windows and Linux specific UI stuff works on it's own terms but the integration into the portable code is largely missing.

**Portable**

 - qor_application (shared library)            Stable          Application framework
 - qor_asyncioservice (shared library)         Stable          Asynchronous IO
 - qor_base64decodefilter (shared library)     Stable          Base64 decode filter for Pipeline
 - qor_base64encodefilter (shared library)     Stable          Base64 encode filter for Pipeline
 - qor_current (shared library)                Stable          Current Process and Thread, Thread Context, Thread Local infrastructure
 - qor_datasink (shared library)               Stable          Binary and structured data sink for Pipeline
 - qor_datasource (shared library)             Stable          Binary and structured data source for Pipeline
 - qor_echo (shared library)                   Stable          Echo protocol
 - qor_error (shared library)                  Stable          Error raising and handling Flyer
 - qor_fileconnector (shared library)          Stable          File Connector for Pipeline
 - qor_filesystem (shared library)             Stable          File System provider
 - qor_host (shared library)                   Stable          Module System Host and Type Registry
 - qor_injection (shared library)              Stable          Dependency Injection and Type Identity
 - qor_interception (shared library)           Stable          AOP Interception, Function Context, Call Context, Parameter Validation
 - qor_issue (shared library)                  Stable          Base Flyer library for Errors and Logs
 - qor_linereader (shared library)             Stable          A Line At A Time reader for Pipeline
 - qor_log (shared library)                    Stable          Logging, rasing, handling and forwarding logs
 - qor_logaggregator (shared library)          Stable          Multi threading log aggregator and handler
 - qor_module (static library)                 Stable          Module system base library
 - qor_netclient (shared library)              Stable          Network Client Module, Plug in a protocol to use it.
 - qor_netserver (shared library)              Stable          Network Server Module, Plug in a protocol to use it.
 - qor_optionparser (shared library)           Stable          Command Line Options by callback injection
 - qor_pipeline (shared library)               Stable          Pipeline construction library. Anything to Anything, Push or Pull, with Filters
 - qor_platform (shared library)               Stable          Platform abstraction module
 - qor_randomsource (shared library)           Stable          Noise Source for Pipeline
 - qor_role (shared library)                   Stable          Roles and Features for Application, Customization, Startup/Shutdown control
 - qor_signals (shared library)                Stable          Signals and Slots from CopperSpice. No custom preprocessor
 - qor_socketconnector (shared library)        Stable          Socket Connector for Pipeline
 - qor_sockets (shared library)                Stable          Portable Sockets, Sync and Async
 - qor_stdfstreamconnector (shared library)    Stable          std::fstream Connector for Pipeline
 - qor_stdifstreamconnector (shared library)   Stable          std::ifstream Connector for Pipeline
 - qor_stdifstreamsource (shared library)      Stable          std::ifstream Source for Pipeline
 - qor_stdinsource (shared library)            Stable          std::cin Source for Pipeline
 - qor_stdofstreamsink (shared library)        Stable          std::ofstream Sink for Pipeline
 - qor_stdoutsink (shared library)             Stable          std::cout Sink for Pipeline
 - qor_stringsink (shared library)             Stable          String Sink for Pipeline
 - qor_text (shared library)                   Stable          Strings, CoW, String functions, UTF-n and Code Pages
 - qor_thread (shared library)                 Stable          Threads, Thread Pool
 - qor_workflow (shared library)               Stable          State Machine
 - qor_compiler (shared library)               Stable, Incomplete      Compiler abstraction
 - qor_arch (shared library)                   Stable, Incomplete      Architecture abstraction
 - qor_loghandler (shared library)             Stable, Incomplete      Basic in thread Log handling
 - qor_sync (shared library)                   Stable, Changes pending, naming inconsistency. It's a zoo right now
 - qor_task (shared library)                   Stable, Changes pending, namespace, naming
 - qor_parser (shared library)                 Semi stable. Will be refactored for Unicode support
 - qor_parsersink (shared library)             Semi stable
 - qor_reporting (shared library)              Working, Incomplete
 - qor_threadmemory (shared library)           Working
 - qor_stringsource (shared library)           Working
 - qor_dynamiclibrary (shared library)         Working
 - qor_http (shared library)                   Incomplete - Request Parser works, Response Generator mostly complete
 - qor_image (shared library)                  Incomplete
 - qor_ttf (shared library)                    Incomplete
 - qor_unicode (shared library)                Incomplete
 - qor_json (shared library)                   Incomplete - JSON parsing works, builds a minimally simple RO object model. No writing
 - qor_jpegdecodefilter (shared library)       Experimental, Incomplete
 - qor_deserializersink (shared library)       Experimental
 - qor_events (shared library)                 Experimental
 - qor_generator (shared library)              Experimental
 - qor_generatorsource (shared library)        Experimental
 - qor_aggregatesource (shared library)        Experimental
 - qor_canvas (shared library)                 Experimental UI
 - qor_console (shared library)                Experimental UI
 - qor_egl (shared library)                    Experimental UI
 - qor_layout (shared library)                 Experimental UI
 - qor_opengles (shared library)               Experimental UI
 - qor_tui (shared library)                    Experimental UI
 - qor_ui (shared library)                     Experimental UI
 - qor_uiselector (shared library)             Experimental UI
 - qor_widgets (shared library)                Experimental UI
 - qor_renderer (shared library)               Experimental UI
 - qor_property (shared library)               Experimental
 - qor_resources (shared library)              Experimental, Incomplete
 - qor_textshaping (shared library)            Experimental
 - qor_serialize (shared library)              Experimental


**Windows:**

 - winqor_api_winsock2 (shared library)        Stable                  Socket API
 - winqor_api_asyncioservice (shared library)  Stable                  AsyncIO, IOCompletionPorts, Cancellation, Co-routine based
 - winqor_common (shared library)              Stable                  Common Windows stuff like Handles
 - winqor_exebootstrap (static library)        Stable                  Executable Bootstrap library for Windows Processes (May be redundant)
 - winqor_filesystem (shared library)          Stable                  Windows File System
 - winqor_process (shared library)             Stable                  Windows Current Process control
 - winqor_sharedbootstrap (static library)     Stable                  Bootstrap library for DLLS (May be redundant)
 - winqor_sockets (shared library)             Stable                  Windows Sockets
 - winqor_thread (shared library)              Stable                  Windows Threads
 - winqor_api_kernel (shared library)          Stable, Incomplete      Kernel32 API
 - winqor_api_user (shared library)            Stable, Incomplete      User32 API
 - winqor_dynamiclibrary (shared library)      Working                 DLL loading and runtime linking
 - winqor_api_ole32 (shared library)           Working, Incomplete     OLE32 API
 - winqor_api_shcore (shared library)          Working, Incomplete     ShCore API
 - winqor_api_shell (shared library)           Working, Incomplete     Shell API
 - winqor_com (shared library)                 Working, Incomplete     COM support
 - winqor_api_xinput (shared library)          Incomplete              XInput API
 - winqor_api_uxtheme (shared library)         Experimental UI         Theme API
 - winqor_api_opengl (shared library)          Experimental UI         OpenGL API
 - winqor_api_comctl32 (shared library)        Experimental UI
 - winqor_api_direct2d (shared library)        Experimental UI
 - winqor_api_dwm (shared library)             Experimental UI
 - winqor_api_gdi (shared library)             Experimental UI
 - winqor_console (shared library)             Experimental UI
 - winqor_desktopui (shared library)           Experimental UI
 - winqor_egl (shared library)                 Experimental UI
 - winqor_gdi (shared library)                 Experimental UI
 - winqor_gl (shared library)                  Experimental UI
 - winqor_gui (shared library)                 Experimental UI
 - winqor_opengles (shared library)            Experimental UI
 - winqor_termscreen (shared library)          Experimental UI

**Linux:**

 - linqor_filesystem (shared library)          Stable                  Linux File System
 - linqor_process (shared library)             Stable                  Linux Current Process control
 - linqor_sockets (shared library)             Stable                  Linux Sockets
 - linqor_thread (shared library)              Stable                  Linux Threads
 - linqor_asyncioservice (shared library)      Stable, Incomplete - Needs Cancallation adding
 - linqor_dynamiclibrary (shared library)      Working                 DSO loading and runtime linking
 - linqor_console (shared library)             Experimental UI         Linux terminal interaction
 - linqor_egl (shared library)                 Experimental UI
 - linqor_opengles (shared library)            Experimental UI
 - linqor_termscreen (shared library)          Experimental UI
 - linqor_waylandclient (shared library)       Experimental UI
 - linqor_waylandegl (shared library)          Experimental UI
 - linqor_wlpresentationtime (shared library)  Experimental UI
 - linqor_wlviewporter (shared library)        Experimental UI
 - linqor_x (shared library)                   Experimental UI
 - linqor_xdgshell (shared library)            Experimental UI
 - linqor_xegl (shared library)                Experimental UI

**Sample Apps:**

 - helloworld (executable)                     Stable Sample App               Hello world
 - client (executable)                         Stable Sample App               Echo client
 - httpserver (executable)                     Stable Sample App, Incomplete   HTTP Server
 - options (executable)                        Stable Sample App               Command line Options
 - outline (executable)                        Stable Sample App               Self version reporting
 - parallel (executable)                       Stable Sample App               Threads and Log Aggregation
 - plumbing (executable)                       Stable Sample App               Pipelines
 - work (executable)                           Stable Sample App               Workflows
 - roller (executable)                         Stable Sample App               Roles
 - server (executable)                         Stable Sample App               Echo Server
 - button (executable)                         Experimental UI Windows only
 - dialogs (executable)                        Experimental UI Windows only
 - editbox (executable)                        Experimental UI Windows only
 - expconsole (executable)                     Experimental UI
 - mainwindow (executable)                     Experimental UI Windows only
 - menu (executable)                           Experimental UI Windows only
 - minimal (executable)                        Experimental UI Windows only

**Tests:**

 - QOROpTests (executable)                     Operations tests
 - QORTermTests (executable)                   Terminal tests
 - QORTest (executable)                        Unit tests
 - QORTestFileSystem (executable)              FileSystem operational tests
 - QORTestPipelines (executable)               Pipeline operational tests
