# Key Concepts

Back in the bad old days it was very common in the source tree of a project to see a folder or a file called utilities, utility or utils. 
Have you ever seen that?
What was in the utilities folder?
A whole bunch of unrelated things usually which were needed in order to make the application work but weren't really specific to the application and didn't in and of themselves meet any of its requirements. 
Maybe your application needed a particular sort algorithm that the libraries on your system didn't provide or there was some hinky thing that you kept needing to do with the files your supplier was sending you to skip over their custom headers.
How important were those things?
Well, likely critical because otherwise no one would have written them given that they didn't by themselves meet any application requirement.
How much attention did the content of the utilities folder generally get?
Usually very little.
How organised was it?
Probably not at all.
Why don't we have utilities folders or files anymore?
Maybe your project still does. Where should that code be instead?
In the framework. Over the years frameworks and standard libraries have improved to include a lot of the utilities that we used to have to write ourselves.
So frameworks are important, they've become more and more important as they've taken on more work. Maybe its time to look again at the layering of application over framework that has evolved over the years. 


Building frameworks is not like building applications. There is no spec, no requirements other than to make it easier to build applications. There are just ideas and experience.
So what ideas should we use to guide us? Low level simple ideas for low level simple software.


# Separation of concerns

By separating concerns we get portability and adaptability. You can think of this as similar to the single responsibility principle. It's about reduced coupling to dependencies. It's about being able to change the behaviour of code we already wrote without breaking it.

Now if you think about that, that's an oxymoron. The definition of broken is that it doesn't behave according to the requirements, right?
So if we change the behaviour then it will, by definition, be broken. 
Unless of course the requirements have changed and we all know that never happens.

Truly separating concerns and controlling dependencies is one of the three hardest things in software along with concurrency and bootstrapping, maybe the hardest, so this not easy and is never going to be perfect. We should however go as hard as we can to drive out unnecessary coupling at all levels.


# Self aware software

By now we hopefully all understand the costs and benefits of unit testing and TDD. 
What is unit testing and other level of automated testing? 
What is happening when you run a unit test?
You're getting a software system to test itself. You don't do the actual testing, the software does it and then reports the pass or fail. In a way this is the most basic and also the most important aspect of self aware software. The software knows what it's supposed to do and not do and can assess it's own fitness.

Well that's good but is testing itself the only thing we make the software do to itself?

How about building itself?

We already use code to build code, not just compilers but make and CMake and the like. So OK we can get software to build software.

How about reporting on its activity?

Well that would be logging, tracing, metrics and all the meta data that spews out of modern systems. Also all the debugging information and facilities like breakpoints and exceptions. Good. Well some frameworks do a decent job, other less so.

How about scaling itself?

We have thread pools and microservices and kubernetes and 128 core CPUs with terabytes of RAM. Getting algorithms to manage that scaling is tricky but the hyper-scalers like AWS are certainly working on them and the millions of dollars that even tiny improvements are worth to them will drive that forward.
At the low level though, automatically spreading work across threads. That's a tricky problem and facilities for doing it safely and tuning it safely once it's done could probably be improved in most frameworks

How about something even simpler. Software managing it's own memory?

Well we have garbage collection. A terrible cop out but better than nothing. 
This is definitely an area where we need to do better.

How about software porting itself to different operating systems and architectures?#

Hmm. We could definitely do better there. Apple spent a fortune writing an emulator for PowerPC to run on x86 and just a few years later they've spent another fortune on an emulator for x86 to run on ARM and that's all within their own closed ecosystem on a single operating system they completely control.

How about software connecting itself together?
Libraries linking at compile time. Dynamic libraries wiring up interface at load time. Signals and delegates and internal lock free message queues connecting components at runtime. RPC and service busses and Kafka and internet protocols connecting outside the process boundary.

That stuff is getting pretty good. It's a bit of a mess with all these different technologies. Each having their own way to do everything from connecting and disconnecting to security and encryption but we have a lot of good options now for getting one piece of software to talk to another in a standardised way. This is the foundation of modern micro services, where we pay the price of spending most of our time and effort sending and receiving messages, to get the benefits of scalability, independent deployability and being able to dive up large projects across multiple teams without inevitable disaster. The probability is somewhat reduced anyway.

Just one more aspect of Self aware software. Back to the low level.
What have we not mentioned?

Reflection. Code that can literally examine itself and act on what it finds.
this is commonly used to automate serialization. So you don't have to write a read function and a write function for every type of object you want to be able to store in a file. You have functions for the built in types and reflection allows the code to reverse engineer the sequence of built in types from your aggregate type definitions, your struct and classes, and generate code to read and write whole objects at a time.
Serialization is just one of the things you can do with reflection. It's potentially useful for testing, debugging, tracing, monitoring, performance measurement and all kinds of introspecting things.
Reflection is the basis for Aspect Oriented Programming, something we'll definitely talk more about.

So what does this all add up to?
All these things we've just talked about can be encapsulated in the idea of Self aware code. Software that looks at itself and operates on itself in some way. We've been inventing and reinventing these things for years without realizing that they're all part of the same idea. Making the software more self aware.

All of these are things you probably want to use in your application but none of them do you want to have to write in each application. All these things belong in the framework. The framework should facilitate SOC and SAC.

Separation Of Concerns so that the application can adapt easily.
Self Aware Code so that anything the computer can do well, we make it do, not us. We can get on with delivering on the requirements.
