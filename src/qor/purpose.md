The src/qor folder is for libraries and modules that will be used all over the rest of the QOR
These are facilities built on top of the standard library to assist in writing code rather than to assist in implementing applications directly.

These libraries must be universal, having no operating system, architecture or compiler specifics. They depend only on one another and the standard library.

If you might use something in thousands of functions in your application, it belongs here.
