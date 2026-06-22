QOR Thread module
Provides a platform agnostic interface to manual thread management ultimately layered over C++ standard library jthreads.
Also provides a platform agnostic Thread Pool for simplified thread and task management.
It is recommend to use the Thread Pool feature for multi threaded applications even if you intend to do custom thread management alongside it because other QOR features like asyncio and log aggregation depend on the Thread Pool feature.
It's fine to use both the Thread Pool and manual thread management as long as you don't pass or share anything directly between pool and non pool threads.
QOR Applications don't have to be multi threaded but they do have to be thread aware and therefore required a multi threaded OS and threading support. Linking the qor_thread library is required for example to support error handling and logging because they are per-thread even in a single threaded application that doesn't use the Thread Pool feature.

