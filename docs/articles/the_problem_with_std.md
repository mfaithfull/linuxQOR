# What's the problem with the C++ standard library?

In short the problem with the C++ standard library is developer expectations of what it should be and what it should provide.
The standard library is not a runtime. The standard library is not a framework. It's a set of low level, zero overhead, abstractions that are the 'best known' solutions to common problems that can be solved with low level, zero overhead, abstractions.
The zero overhead part is important and is part of the design/specification/concept of C++ itself as expressed by it's author, Bjarne Stroustrup. This makes the using the standard library guilt free because of it's zero runtime overhead.

As a result if something can't be a zero overhead abstraction, e.g. logging, then it isn't in the standard library. The file system and formatting building blocks to build logging are there but logging is never zero overhead. The very process literally is overhead and so it will probably never be provided by the standard library. Added to this that there will never be agreement on a 'best known' way to do it and the chances receed further. Similarly for error handling with the exception of exceptions which are a whole controversial topic in themselves.
These restrictions and the historical difficulties and risks associated with extending a large integrated standard are why the C++ standard library remains a low level library that provides fine grained components.

There's nothing wrong with this, of course, it's by design. Unless your background is in another language, e.g. VB or C#, where the enormous runtime provides almost every facility you can think of. APIs with hundreds or even thousands of entry points. The skill set in these languages is largely learning what's in the standard libraries and how to use it because almost everything you'll ever need is already there. From a Web Server with secure sockets to a UI toolkit and a JSON parser.
If your background is in any language where the standard runtime provides these things then you'll be shocked at how little the C++ standard library provides. On the other hand if your background is in C you'll be constantly finding whole tracts of your code that be replaced wholesale with std:: classes you no longer have to write yourself.

The real problem then with the C++ standard library is not what it is, it's expecting it to provide the building blocks for your application. Unless you're building something really small and simple, it won't. You'll need 3rd party libraries for every substantial aspect of your substantial application, even relatively small stuff like logging.

There are other issues like the essentially unreadable library source code which need to be dealt with ahead of extending it to provide more things but I'll leave those to the experts.

