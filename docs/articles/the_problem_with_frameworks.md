# What's the problem with frameworks?

In principle a great framework should be the answer to most of the problems we've discussed with application development in C++. In practice there is really one large problem. Writing frameworks is hard. Writing a great framework is really, really hard.

Lets take the example of Qt, a massive and commercially successful C++ framework. It's The foundation of KDE on Linux and used by countless development teams to create Windows Linux and Mac applications. It has almost everything. It does almost everything. And yet when Nokia bought it and tried to port it to their new mobile operating system, the project cost many millions and nearly broke them and their development team.
Nokia never really recovered, for other unrelated reasons, and Qt itself was re-open sourced and then re-commercialized again before being significantly further developed.
What went wrong? In simple terms, the Nokia team didn't recognize the difference between a Multi Platform Framework and a Cross Platform Framework. Qt was always intended, from the ground up, to work on Window and Unix (Mac/Linux). It was Multi Platform. It was never designed to be portable to anything else. It wasn't designed to have platforms added to it. This may seem like a subtle distinction but it's the critical difference that cost Nokia millions and left them with a half working, late, product that missed the smart phone revolution they should have been equipped to lead.

...some code...
#ifdef WIN32
...do the windows thing...
#else
...do the UNIX thing...
#endif

may seem entirely innocent but when it occurs 90,000 times in your 2,000,000 line source base and three out of five of those need a new 
#elif(NEW_OS)
...do the new thing...
#else
One out five needs the WIN32 to be used for NEW_OS and one needs a third banch of the Linux/Mac code within the UNIX block. Excepting the cases where currently common code needs an entirely new #ifdef block because the new OS is mobile specific and does things differently. You're screwed. File sizes explode. Tracking what does and doesn't need changing, and how, is practically impossible. The entire, already baked in, approach of varying the code with preprocessor blocks doesn't scale.

That Qt, a good and successful project, can have this kind of design flaw baked in just goes to show how hard writing a great platform really is.

A great frame would be designed to be portable between platforms. It would have all the coherence and self dependency of Qt while allowing basic things such as memory allocation strategies to swapped out without breaking. It would provide the high level and mid level concepts, constructs and components needed to build applications without locking you in to one particular way or one particular kind of application. It would be broad while allowing you to go deep in specialist areas. It wouldn't become specialized (like Juice) so that it becomes a framework for a particular kind of application but any kind of application would be easier to write with it than without it. It would integrate with all the exsting technologies you might want to use, from Open Telemetry to Open GL, without creating a hard dependency on any of them. It would for sure give you widgets but it wouldn't just be a UI tookit. It would work just as well on the Web and doing micro services.

We can imagine such a framework and the beauty of C++ is that if we can imagine it, we can build it. No limitations but the hardware, right?
We should probably do that.
