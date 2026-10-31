# QOR Purpose Summaries per Library

Derived from the consolidated src/**/purpose.md (docs/agent/_tmp_purpose_all_out.md) + repo_map.md.
One line per QOR lib. "—" = no dedicated purpose.md; intent inferred from subsystem purpose.md.

## src/qor (ubiquitous core)
- qor_module        Base static lib linked into EVERY QOR module; enables static-library chaining + the module stitching system (module tag).
- qor_current       Abstractions of current thread + current process. Impl provided per-platform (src/platform/os/<os>/). Linked into all QOR executables; needed even single-threaded (Flyers bind data to current thread).
- qor_reporting     Forwarding reporting proxy with fallback so low-level libs can report errors without depending on qor_error.
- qor_text          String classes for UTF-8/UTF-16/UCS4 + code-page encodings. qor::String == UTF8String. Ops implemented once on AbstractString via iterators.
- qor_textshaping   Text layout/shaping (display). Part of the text subsystem (UTF->UCS4 code points for glyph lookup).
- qor_unicode       Unicode/UTF handling within the text subsystem.
- qor_error         Raise errors of varying severity + handle them. Builds on issue/log via Flyers.
- qor_interception  General function-level interception framework for AOP; powerful/fundamental, current code only lightly uses it.
- qor_issue         Extensible base classes + infra for issues; raised/handled via Flyer; each Issue records where/when; What is derived-type determined.
- qor_log           Log raising + default log handlers; logs have a level, handlers filter by level.
- qor_tef           Type-erased functions (port of magic_func); qor_pp_make_function -> callable type-erased fn object; ~2x call perf over std::function.
- qor_injection     Dependency injection (memory subsystem).
- qor_threadmemory  Thread-aware memory subsystem (src/qor/memory/thread).
- qor_property      Generic properties + collections, convertible to/from structs by reflection; variant-based (property map/vector/value); JSON-convertible. Experimental.
- (header-only, D6) assert, datastructures, frozen, factory, instance, mock, profiling, reference, test — see subsystem purposes below; no ctags index, index on demand.
  - assert/assert       Convenient assert functionality for testing/debugging.
  - datastructures      Header-only data structures used everywhere; std lib only deps.
    - frozen            Constexpr data structures (error->message maps, translation tables in code).
  - factory             Object construction factories (memory subsystem).
  - instance            Instance management (memory subsystem).
  - mock                Mock object support (replace real counterparts in tests to limit scenario complexity).
  - profiling           Simple aggregated function timing to find where time is spent in a call tree.
  - reference           Smart references = combined unique+shared; backend (locking vs not) chosen by class traits.
  - test                Unit testing: test cases, suites, reporting. Based on YAFFUT. header test.h, ns qor::test.
- qor_reflection    Reflection to iterate aggregate members type-safely (names/types/sizes); built on PFR; expect switch to C++26 reflection.

## src/framework (structural app parts)
- qor_application   Application thing (Features/Roles/Options/Workflows). Aggregation header app.h: include before deriving/using qor::Application.
- qor_dynamiclibrary Dynamic library loading support.
- qor_host          Host-process singleton; once-per-project, low-level; CENTER of the module stitching system tying QOR modules together.
- qor_optionparser  Command line options parsing.
- qor_role          Roles (real apps built from Roles + Features).
- qor_workflow      Reusable state-machine outline; extend-only to perform any linear or stack-based state chart.
- qor_pipeline      Data pipeline framework (Sink/Source). Chain source->sink. Pull: sink calls source; Push: source calls sink. Inline filters = buffers. Buffers in units w/ reader+writer, request/ack read+write heads, semicircular.
- qor_resources     Async general resource manager: discovery, unique ID, classification, parsing -> validated in-memory objects; offloaded to background task. Use ResourceHub Feature + Paths/FileIndexes/Claimer.
  - qor_image       Standard in-memory, device-independent image (pixmap/bitmap, multiple pixel formats, many formats R/W).
- qor_events        Memory-stable publish/subscribe events queueing (prototype).
- qor_signals       Signals-and-slots publish/subscribe for in-process messaging, with/without threads; lower coupling, runtime plug-together.
- qor_delegate      (header-only) single+multicast delegates from fastest codeproject.com impl. delegate.h / multicastdelegate.h.
- qor_observer      (header-only) observer pattern pieces; purely additive, proxy avoids touching inheritance.
- qor_asyncioservice Async IO via platform impls; apps initiate, background IO tasks on thread pool wait.
- qor_filesystem    File/filesystem access via platform layer + std::filesystem.
- qor_sockets       Network sockets + addressing via platform layer modules.
- qor_sync          Curated std sync primitives w/ coherent interfaces (choice of primitive transparent to caller); scoped lock as "the one true lock" + scoped key class to invert.
- qor_task          Async tasks: run, schedule, cancel, wait.
- qor_thread        Platform-agnostic manual thread mgmt over std jthread; + Thread Pool feature (recommended even alongside custom threads; asyncio/log-aggregation depend on it).
- qor_workitem      (parallel) work item support.
- qor_uicommon      Common UI base (layout/nav shared across UIs).
- qor_desktopui     Desktop integration (system menus, icons, launch, registration, minimize-to-bar).
- qor_gl            GL (OpenGL) support base for graphical UI.
- qor_uilayout      Base layout node + flexbox/vbox/hbox/grid/radial/textflow algorithms; pure logic (only needs extents); shared across all UIs incl. nav.
- qor_uiselector    Runtime selection among available UIs.
- qor_sercon        Serial UI: interaction over a bidirectional pipe (output = titles in hierarchy; input = keyboard); same backend models as graphical UI, minimal presentation. ns qor::ui::con.
- qor_terminal      Full TUI (FTXUI-derived but deviates for UI/gfx compat); ncurses-style look/feel. ns qor::ui::tty:: {Display,Renderer,Toolkit,SurfaceBuffer,TopLevelContainer}.
- qor_uiwidgets     Conceptual widgets = state + actions (e.g. PushButton caption+push->action, transient pushed state); rendering separate.
  (graphical subdirs toolkit/windows are design notes; no separate lib target)

## src/platform (arch/compiler/os base)
- qor_platform      Umbrella of arch+os+compiler; the base the rest of QOR stands on.
- qor_arch          Architecture of built QOR (endianness, word size, arch-dependent facts factored in).
- qor_compiler      Source compiler traits (quirks/specifics per compiler/version; demangling, vtable layout).
- (os adapters; not QOR libs)
  - linux: app (current thread/process, dyn lib load), io (files/sockets/async via io_uring), ui (X/Wayland experimental); ui/composition (X/Wayland), renderapis (EGL/GL/GLES/Vulkan), toolkits (GTK/FLTK/Qt/EFL).
  - windows: api_layer (NT/Kernel/User/Avanced/Winsock2/Process/GDI/COM/Theme/GDI/...), app/exebootstrap (Win exe endpoint+support), app/sharedbootstrap (DLL entry point; work on process/thread entry/exit), common (win types/defs), io (files/sockets/async over completion ports), ui (wincon=Windows Console; gui controllers WindowController+Button..; renderapis WGL/EGL/GLES/GL/DC/DX/Vulkan; toolkits Native/Win++/Qt/GTK).
- qor_sharedbootstrap (win/lin) static lib linked into every QOR dynamic lib -> DLL entry point.
- qor_exebootstrap (win) static lib linked into QOR Windows executables (Windows-specific bootstrap).

## src/components (leaf apps; depend on others, not depended on)
- qor_jsondata      JSON data handling (formats/json).
- qor_ttf           TrueType font reader (experimental TTF reader; formats/ttf).
- qor_parser        General parser state machine; derived from qor_workflow w/ Parse/FinalParse; RFC5234 token/states/productions; attach to qor::Pipeline via Context + ParserSink. Each grammar production = own class matching ABNF; Specific/Range match moves parser forward (lexing), rest combine tokens (parsing). Good for JSON/BSON/HTTP/SQL/HTML/XML/SMTP/Kafka (not C++ most-vexing-parse).
  - parser/states   Parser state classes = grammar (ABNF) productions.
  - parser/nodes    Parser node classes = AST (initial parse output); Emit builds a model by popping/consuming stack nodes; Prepare speculatively pushes a node; Fail pops+discards.
- qor_serialize     (io/deserializer) serialization support.
- qor_generator     Data generator source.
- qor_netclient     NetworkClient: connect + send/receive flow (intermediate server/client example).
- qor_httpclient    HTTP client (io/network/clients/http).
- qor_socketclient  Socket client.
- qor_netserver     NetworkServer background service (production-ready: Thread Pool + Async IO + Workflows, protocol-agnostic, per-connection logging; serves Echo Protocol on a port).
- qor_fileconnector File pipeline connector (Sink/Source/Connector over files).
- qor_pipeconnector Generic pipe connector.
- qor_socketconnector Socket<->Pipeline bridge; Connector implements socket interface + full network connector (addr/port/flags). SocketReader/SocketWriter pipelines DEPRECATED/may be removed.
- qor_stdfstreamconnector / qor_stdofstreamsink / qor_stdoutsink / qor_stdifstreamsource / qor_stdinsource  std-fstream/console pipeline parts (see io/pipeline connectors/sinks/sources).
- qor_base64decodefilter / qor_base64encodefilter / qor_jpegdecodefilter / qor_linereader / qor_unicodetextfilter  pipeline filters (inline buffer+function).
- qor_datasink / qor_deserializersink / qor_parsersink / qor_stringsink  pipeline sinks.
- qor_aggregatesource / qor_datasource / qor_generatorsource / qor_randomsource / qor_stringsource  pipeline sources.
- qor_echo          Echo Protocol (full model protocol; bounces what you send; req+resp "parsed").
- qor_http          HTTP Protocol (protocols/http; the LIVE one — http_old is redundant ref code, NOT built, per D5).
- qor_jip           JIP protocol.
- qor_uri           URI handling.
- qor_logaggregator Log Aggregator (thread pool + signals + filesystem + role).
- qor_loghandler    Log handler (issue/signals/log).
- qor_canvas        Canvas drawing component.
- qor_console       (components/ui/console + framework ui/console) minimal std io console, serial-link capable, NO TTY specialization.
- qor_egl / qor_opengles EGL / OpenGLES render component.
- qor_layout        Layout component (role/app/error).
- qor_renderer      Renderer (workflow+signals).
- qor_tui           TUI component (role/app/workflow/signals/error).
- qor_widgets       Widgets component.

## Top-level src purposes (context)
- src/qor           Universal libs (no OS/arch/compiler specifics; only std lib + each other). "If used in thousands of functions, belongs here."
- src/framework     Structural app parts + high-level flow control (app/data/event/io/parallel/ui).
- src/components    Optional QOR modules on top of others; arranged like qor:: subsystems; experimental in qor::exp::; migrate to qor::components/* by v1.0.
- src/platform      Collects target arch + target os + source compiler; base of QOR. OS/compiler/arch-specific MUST be here.
- src/configuration Primary qor header -> PCH; first include in all TUs (#include "src/configuration/configuration.h").
- src/macros        QOR preprocessor lib (Boost PP port; qor_pp_*).
- sdk               Aggregation headers incl. app.h (everything needed before deriving/using qor::Application).
- examples/basic    helloworld, outline (self-analysing app), roller (Roles+Features), work (workflow state machines), options (CLI opts), plumbing (Pipelines IO/transcode), parallel (background services + parallel tasks via Signals).
- examples/intermediate server+client over Echo Protocol (NetworkServer + NetworkClient).