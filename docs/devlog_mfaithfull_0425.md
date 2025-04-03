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
