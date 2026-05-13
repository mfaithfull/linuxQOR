# What's the problem with C++?

You've heard that C++ is hard. You've heard that C++ is unsafe. You've heard that C++ is powerful. All these things are true. 
C++ is likes rocks. Rocks are hard. Rocks are inherently unsafe, especially if dropped from a height. Rocks contain all sorts of powerful substances like uranium and lithium that can make you rich, or dead. Also we live on a big rock falling at 66,000 km/s towards an unshielded nuclear fusion reactor. We'd be lost without it, so perhaps unsafe is a matter of perspective.

Actually none of these things is why many developers have problems with C++. Many developers have problems with C++ because of what we're trying to do with it. Our day job is to build applications for human beings. Things that do business, interact with people and make it easier for them to part with money for goods and services, or goods and services for money. 
It's tempting to think that a powerful programming language like C++ would be designed to help us do that but it isn't. It's designed to talk to machines about numbers, that's all. That's all it does and all it will ever do, really, really fast.

The mistake we often make is to think that because C++ is so much larger, more complex and more 'modern' than C, that it's therefore a high level language. It really isn't. It's a hybrid language capable of expressing high level constructs but not actually providing them. The standard C++ library provides basic containers and algorithms but they are just that, basic. They aren't really the building blocks of applications. I'll go into the reasons for this another time.

Building applications directly out of standard library components is certainly possible and many people recommend it. After thirty years of experience I would recommend strongly against it, if you're building anything more substantial than a calculator or anything higher level than a single purpose library.
Yes. Use everything the standard library provides, where you'd otherwise have to reinvent it but don't assume these very small bricks are everything you need. There's a reason space Lego exists: Once you've used it you wouldn't want to go back to building everything out of purely rectangular bricks.

In summary if you find yourself doing 'problematic' things like memory management, trying to interrupt a thread, or dealing with compiler differences, while you're building an application, then you're doing it wrong. Solve those problems once and for all, package the solution and reuse it, so you can stop worrying about low and high level problems at the same time. This frees up your brain power to solve the high level, high value, problems.

C++ is of course the perfect language in which to solve those low level problems and then to reuse your solutions. The very broadness of the language that we trip over is exactly what makes it the greatest programming language, if we understand how to use it.

