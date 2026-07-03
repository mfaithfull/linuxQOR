The essential libraries that make everythig else possible
qor_current gives us thread and process context
qor_datastructures, qor_iterators, qor_objectcontext, provide utility classes
qor_reporting provides a forwarding reporting proxy, with fallback, 
    so that low level libraries can report errors without depending on qor_error
qor_text handles the vexed matter of text. The most low level thing (code is text) and yet 
    needing high level algorithmic treatment, Unicode, UTF, CodePages and even Layout and Shaping. 
    In principle we can't even report errors without text. In practice this is mostly high level 
    classes wrapped over the standard library and iterators.
