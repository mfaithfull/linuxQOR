A new month and much progress. Interception is now working based on the use of Function Context objects and Flyers.

The rough plan from here is:

multicast delegates
Get the Type registry and Factory by ID working
Add DIWrapper wrapping up DI.

Pooled objects, refs and instancer
Singleton objects, refs and instancer
Intrusive Refs for COM and similar

Error and other Issue raising and handling

Thread Memory
Thread Pool
Signals

Reflection...


Multicast delgates and most of DI are done. ExternalFactory registrations with additional constructor parameters need more work if they're required.

Research into pfr indicates that it's what we need for reflection and can be extracted from Boost without issues.

Singletons are up and threaded singletons as well, yay modern C++.
Pools will require some kind of plugin strategy for scaling/sizing and blocking requests. not there yet.

Added a very basic issue module

Signals added without custom memory managemetn integration. std allocators and deleters are weird anyway and their use here seems to be inconsistent. I can't find any way to map std:unique_ptr and std::shared_ptr internal allocations into our memory mangement without mismatched deletes that at best have slicing problems.

Added pfr, the reflection library from Boost in as a raw port. Namespace, messages etc to be fixed but the tests pass so refactoring is now plausible.

Refactored pfr into qor_reflection

At this point the bulk of the core qor is written. There are a lot of extension points. 
We could add:
Intrusive references
Pooled instancing
Threaded memory (This is in progress)
External Factory support for constructor parameters
Serialization on top of qor_reflection (This is a definite)
Support for I18n and formatting of internal messages (This is also definitely required)

However to build actual applications, even simple tools, we need Framework modules for workflows, pipelines, roles, features and actual applications so these are more likely to be next

Simple Pipelines and even simpler workflows are now implemented. We'll need compound pipelines and compound workflows sooner or later and parallel execution as well. That will come when it's got a motivating use case.

Roles and the beggining on Features added as well as their Application singleton container.

Added a component for a command line option parser. Still very rough atm and more complicated than I'd like it to be. However we need to be able to pass parameters into simple programs to make them useful and this is the traditional way to do it.

Added the first System module, FileSystem which will have separate implementations for Linux and Windows. I don't consider std::filesystem to be complete or good enough on its own yet.
The pimpl pattern used here will be typical. The system module defines an interface, IFileSystem which is implemented by the platform specific library. The system module uses that platform specific implementation through an internal smart reference.

Got diverted into async IO. Adding a zoo of new synchronisation objects to support coroutines from Lewis Baker's cppcoro 

Performant Sync and Async file IO commonised across Windows and Linux looks possible now with liburing on Linux and a portable technique for handling async tasks using coroutines
However the Filesystem modules are a major undertaking. This is going to take some time to get even approximately right because everything is OS specific and also has to be common so needs to be be developed 2.5 times and the .5 is the hardest due to needing a common abstraction that doesn't loose information compared with what it's abstracting.


Took an opportunity back on Windows catching things up to start bringing in the thread memory stuff. There are now 3 thread specific memory sources to allocate from. 
The basic Thread Heap which for now is just the normal heap which is already thread specific to an extent on Linux. The Windows variant isn't implemented yet but will be completely thread isolated.
The 'Fast' heap which is a hybrid of an arena allocator and a stack. This is best for large up front allocations and chunky objects with stack like lifetime profiles. Asset loading , Sessions, profiles...
The 'Small Object' heap which is also a hybrid stack, arena and bitmap allocator. This is best for flyweights, strings, short lived data models and small (< a few KBytes) objects with unpredicatable and overlapping lifetimes.

New Source classes are provided so a simple 

namespace qor{ 
    qor_pp_declare_source_of(MyType, SmallObjectSource) 
}

is enough to get you a thread locally allocated MyType. This works with the singleton and default instancers, existing factories and of course flyers are already thread local and stack based so they're unaffected.

What about the reference to an object. That too has a heap allocated part as well as the stack allocated object that you interact with. The heap allocated part can now also be made thread local, avoiding the global heap and expensive locks altogether.

namespace qor{
    qor_pp_declare_source_of(typename ref_of<MyType>::type, SmallObjectSource) 
}

will tell the smart reference system to allocate the shared heap based element of a reference to MyType from the SmallObjectSource which is backed by the Fast Heap which is in turned backed by the Thread Heap.
Long running processes could run, memory stable as far as the OS is concerned, for hours never hitting the kernel or a mutex for memory allocation or release.
This is of course not free. We trade space for performance with the SmallObject heap being ~ 65% dense and Fast Heap varying from much lower to higher depending on the distribution curve of the size of your objects.
If you need to look after every byte then these are probably not the allocators you're looking for. If you need what they provide they're available as optional components.


Polished the Application a little and hooked it up the the singleton for the OS wrapper.
remember to provide The##### functions for genuinely global singletons so tha they are owned by the single module that declares them. use TheSystem() to access the System object from anywhere.


I'd forgotted what pain thread local singletons are on MSVC. Got them working there again although not without changing how they work to a less elegant solution under GCC. 
That remains to be tested.

Threadpool memory cleanup improved so threads in global pools are not left hanging around when the pool is done with. Wouldn't happen under 'normal' usage but who's ever seen 'normal' usage in the real world.

Prepping for expansion towards terminal support. The awkward Windows Console will come first as it's the lowest uncommon denominator. Then probably XTerm as the next most primitive.
The goal is to support a common port of Public Domain Curses, Qurses which will in turn act as a minimal fallback rendering target for a portable UI system.

