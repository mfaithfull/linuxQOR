A flyer is a per thread object that stacks instances using the function call stack.
The instance at the top of the stack is always available.
imagine a per thread singleton that you can swap out at any time simply by creating one on the stack and which one is current unwinds with the stack and the objects themselves.

This is great for interceptors, error handler, loggers and anything that you might pass into every function but you don't want to put it in all those function signatures.

Another way to think of this is making classes n-dimensional.
A class is a set of functions that all take an implicit this pointer as their first parameter
With that one single additional automatically passed parameter we do OO
With Flyers you can have as many additional automatically passed parameters as you want
OO becomes an n-dimensional space, enabling higher order programming.
