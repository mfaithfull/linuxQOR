Provides custom iterator framework in a header only library.
Derived from https://github.com/navyenzo/blIteratorAPI
These are useful if you have a custom container and want it to have an stl style iterator attached from the outside
They are used exclusively for custom UTF-8 and UTF-16 string iterators within the QOR.
They work for that but may not for anything else. They are made public here but Use At Your Own Risk.
Circular iterators in particular are untested and judging by the bugs in the original source for linear iterators...
