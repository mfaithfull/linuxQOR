A home for Linux library wrappers

Anything that gets loaded in and needs a set of GetProcAddress calls to get the API should be wrapped here in a similar way to the Windows API layer with function statics so we only pay for looking up a function once if we use it and we only load a library if we use it.

This will probably used mostly to mitigate versioning with things like GL libraries
