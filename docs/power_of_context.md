A large part of self aware code is having the code be able to reflect on the context in which it's executing.
What process is this in?
What thread is this on?
What module is this line of code in?
What class is this line of code in?
What object instance is the member function being called on?
Is this being executed as part of handling an exception?

All these facts and more should be available at any point in the code to branch on or to report on
