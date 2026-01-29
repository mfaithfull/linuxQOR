
"Any problem in computer science can be solved with another layer of indirection", known as the 1st law of computing. The full quote is "Any problem in computer science can be solved with another layer of indirection, except of course for the problem of too many indirections" - Bjarne Stroustrup paraphrasing David Wheeler


"The zero-overhead principle is a C++ design principle that states:

You don't pay for what you don't use.
What you do use is just as efficient as what you could reasonably write by hand.
In general, this means that no feature should be added to C++ that would impose any overhead, whether in time or space, greater than a programmer would introduce without using the feature.

The only two features in the language that do not follow the zero-overhead principle are runtime type identification and exceptions, and are why most compilers include a switch to turn them off." - cpprefernce.com - https://en.cppreference.com/w/cpp/language/Zero-overhead_principle.html


I recently heard an experienced C++ developer say this was probably the only principle all C++ developers agree on.

Here then is the hole in our thinking. We look for zero overhead abstractions as if we were trying to extend the language and when we don't find one we just write linear code without considering abstractions at all. I'm interested in what happens when instead we trade some overhead. We choose to pay a price for some powerful abstractions and prefer that to avoiding them.

Yes we'll loose theoretical 'performance' we'd probably never have achieved and we'll pay a little for things we don't use all the time. However if we do it right the payoff will be that when things get large and difficult, as they always do, our toolkit won't run out of scale. Those larger spanners we paid for earlier will come into their own and we'll gain more than we lose. This is the theory.

The biggest and deepest problems in software engineering are all related to scale. Scaling out is difficult because humans find thinking in parallel difficult and scaling up because our tools and languages are only really designed with a limited range of scales in mind. Language developers are more likely to write Hello World a million times than write a million line project in their language. That's just the way it is.

For large scale-up projects C++ is already the best language. Millions of lines of code are not just possible but practical. Google even manage one or two orders of magnitude greater than that. If we want to go further, or make getting where Google are accessible to more people we need stronger foundations, not in the language, not in the standard library but on top of that. The some-overhead abstractions they can't or won't ship due to the zero-overhead principle. Without these we'll be forever stuck at the scale defined by the abstractions for which we can agree on a zero-overhead implementation.

There are of course few if any such things, or if there are, they're likely already in the language or the library. The overhead, for example, of 3 stack frames to call a std::function rather than the 1 I would ideally prefer is certainly not zero when my parser is running a thousand calls deep and chewing 28MB of stack space. Making me use platform specific APIs to manipulate the maximum stack size.

So we'll use the zero-overhead abstractions of the language and the library and add the some-cost infrastructure that we need to do real engineering. Comercially of course that's impossible because there's no profit in shipping imperfect abstractions. Fortunately we have the FOSS model so maybe, just maybe it's possible.

