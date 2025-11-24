The QOR makes building applications easier and that means different which will likely initially seem harder.

To ease the learning curve we need some good tutorial material.

Examples:

basic:

outline - a QOR Hello World example that also shows how the QOR module system manages static and dynamic libraries
options - Add command line parameters to your QOR Application
roller - Give your application a Role and Features
work - Add workflow state machines to manage the flow of your application
plumbing - Connect to files and the world with QOR Pipelines for IO and data processing
parallel - Launch and use background services and custom parallel tasks

A target for the QOR is to bridge the gap between the fine grained tools the standard library provides and the macro structures from which real world applications are constructed.

Like every framework out there we provide an Application class
Unlike most however you're not required to derive from it to make it useful. You can of course.
The QOR provides the Feature concept for the features that make up an Application and the Role concept as a container for those Features.
You plug your Features into the Role and set your Application to have that Role and you're ready to go.
But go where? This in another point where many frameworks fail, providing no structured flow control.
The QOR provides Workflows, a flexible type of state machine. Like everything else they're optional.

A Workflow is a stack of States. Each State has 4 events that can happen to it.
Enter       The State was entered (or re-entered). This will happen in a loop until the State stack changes
Suspend     The State is about to be suspended because another State has been pushed onto the top of the stack
Resume      The State is back on top of the stack after another state was popped.
Leave.      The State is about to be popped from the stack. Last chance to do anything in this state.

You configure what happens for each each state when it receives these events. By default they do nothing
so you only override what you need. In most cases this means just the Enter function.

Most of what constitutes doing things in software isn't, as many believe, running fancy algorithms to calculate clever things with billions of math operations. Even when that's most of what your program does at runtime, it will likely form a very small part of the code and the only way you know it worked, or can make any use of it, is by doing IO.
Actual Input and Output to the real world is where the value is realised. Believe it or not there are entire languages (Looking at you Miranda) where you can't even do IO or where it's a distant afterthought.
The C++ standard library solves this problem with iostreams and they great, except they're not. They're pretty opaque, have limited extensibility and are viral when it comes to integrating with your code. Requiring the internals of classes to be changed to work with them.
You can't, for example use an iostream to read from a socket (Boost asio provides it) and you can't insert gzip compression or base64 encoding into a std::ostream (At least not that I've ever seen although boost::asio can do anything in principle. I just can't work out how to get it make tea for me). 
The QOR solves these problems with a flexible Pipeline library which allows you to connect a pipeline from anything that produces data to anything that consumes it. Put any kind of transformations you like in between and pump data through the pipeline on demand or on availability.

When it comes to the actual IO part of IO at the ends of the pipes the QOR provides async IO similar to Boost asio but it's easy to use.

Modern software is almost always parallel and any framework needs to provide tasks and a thread pool. The QOR does this based on other open source projects and fully integrated with the other features.

The QOR takes inspiration from Qt's signals and slots mechanism but derives it's implementation from CopperSpice so no pre-compiler or language extensions are required. You can safely raise signals to a listening thread without blocking.

This is all just the qor::framework namespace and that's a large bag of powertools and prefabs to build with.

