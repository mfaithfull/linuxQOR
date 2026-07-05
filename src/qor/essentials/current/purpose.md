Provides abstractions of the current thread and current process. 
The implementations have to be provided by the platform you're targetting, 
i.e. /src/platform/os/<target_os>/
needs to contain a working implementation of ICurrentThread and ICurrentProcess

We need the Current Thread concept even for a single threaded applications because
we tie data to the Current thread for Flyers and even single threaded app can have
affinity and thread priority on multi threaded platforms.

qor_current needs to be linked into all QOR executables.
