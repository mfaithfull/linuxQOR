provides type erased functions. A port of magic_func
use qor_pp_make_function to turn a function pointer into a type erased, callable, function object
Claims 2x call time performance over std::function which seems to be the case.

TODO: The allocator needs routing through our memory sourcing system.
