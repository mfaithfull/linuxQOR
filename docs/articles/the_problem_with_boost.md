# What's the problem with Boost?

www.boost.org

The Boost libraries are the canonincal C++ library collection from which many of the standard library extensions come.
Boost has become an upstream experimental development library for standard C++.
It's huge, written by a loose collective of geniuses and, while containing all sorts of brilliance, has enormous practical problems for building real world applications. Not the least of which is it's arcane build system.

While going a little higher level than the C++ standard library, Boost remains mostly a collection of libraries at the same low level. There are huge gaps in what it provides if you're looking for a framework to build applications on. No application level constructs, no main loop and historically no examples that could ever be more than toys or dev tools.
These aren't really problems with Boost though, any more than they are with the standard library. Just problems it doesn't solve so we still have to. The real problem with Boost in practice is it's tangled mess of dependencies and duplication of concepts.

A few years ago, when I used it in commercial applications, there were no less than five smart pointer implementations within Boost and no way to control which ones you were actually using due to those tangled dependencies. Boost was originally concieved as a header only library set, where you either took all of it or none of it. Despite having grown well beyond where that makes sense, it still retains the legacy of that design. Runtime dependencies between built binaries are documented now and can be tracked but header dependencies between libraries are such that if you depend on any of it you're going to be depending on a lot of things you don't need and don't want. Updates to header libraries you didn't think you were even using can break your code in ways so arcane you'll need to hire a consultant to even explain it.

If you're going to build on Boost then it's best to go all in on it. Treat it as a framework. Assume that version updates will require you to change an unconstrained ammount of your code to get a build and use everything boost:: you can because you're probably depending on it anyway. This is a viable strategy if the tradeoffs make sense for your situation.

Boost is a fantastic academic exercise in how to write C++ libraries, how to solve hard algorithmic problems, and the current state of the art. If you're an ordinary human, who's brain doesn't require a wheelbarrow to cart it around in, don't expect to understand how it's doing what it's doing. Just expect it to perform.

What it doesn't always do so well is close the gap between the low level building blocks of the standard library and the higher level components we need to build applications. Where it tries to do that, for example Boost::asio for async IO, then it locks you in to whatever memory allocation Asio is using, whatever threading model Asio is using and whatever hooks it provides for logging. Forking it to change these things is going to be way more work than rolling your own async IO from scratch. It's very good and if the buy-in makes sense for you, use it. However there will be a lot of people for whom the buy-in doesn't make sense.

The problem with Boost then is that you need to treat it as a framework but it doesn't provide everything a framework should. Neither is it as integrated or coherent as a framework should be. The more standalone libraries duplicate a lot of abstractions they could be reusing and the less standalone libraries have deep dependencies that are near impossible to control. Every library varies so you have to suck it and see.
Extending Boost to live up to what a coherent framework should be would require a mind capable of deeply understanding several dozen Boost libraries all written by different, genius level, developers. Good luck with that.

