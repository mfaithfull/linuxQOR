An error is an issue but not all issues are errors

The are all sorts of issues we may want to raise from code. Fatal errors, less serious errors, warnings etc are on a severity axis but there may be other axes and other purposes

qor_issue provies a general purpose issue template and allows for the collection of contextual information about when and where the issue was raised. The details of what the issue is are left to be added by extension.

qor_error extends qor_issue with severity based error categories and flyer based handlers. Errors can be ignored or escalated.

note        lowest level, can't be escalted
warning     nag level but can be escalted
continuable a real error but execution can carry on, iff it's handled
serious     a serious error, execution may be able to continue but you may prefer a quick exit. Probably best to save what can be saved and quit
fatal       Unresolvable, will terminate even if 'handled' successfully.

You raise an error by calling the (all lower case) free function corresponding to the level 
e.g.

serious ("Something has gone seriously wrong with {0}.", parameter);

These functions use std::format so the rules and extensions for putting parameters into the message
are those of std::format.

In order to see an Error, on the console for example, something has to handle it and write it out.
If no handler is found for an error it will be escalated, eventually be thrown as an exception
and then cause your program to terminate. This will happen for Warning or above. Notes will just be 
ignored if there is no handler.

Error handlers are Flyers, See flyer.md for how they work but usage is simple. You just make one

e.g. 

DefaultErrorHandler errHandler;

and while that object exists your errors will recieve the default handling it provides.

At any time you can make a another error handler

{
    MySpecialistErrorHandler tempErrHandler;

    and it will override the previous one while it exists
}

When it goes out of scope you'll be back to the DefaultErorHandler from before.
All handlers are per thread so when you launch a new thread you'll need to make
a handler on that thread for errors on that thread to be caught.


Logs are also implemented as issues with their own scale

Debug           any ammount of noisy diagnostic detail about what's going on
Inform(ative)   telling you about things that are done, normal flow, completions, opens, closes
Important       things that are significant, unexpected or off the happy path, missing resources, handled exceptions, failures
Impact(ful)     things that will change the flow of control and functional outcomes, exceptions, serious errors
Imperative      things that will cause a program to start or stop including Fatal errors

You raise a log by calling the (all lower case) free function corresponding to the level 

e.g.

inform("That thing that was executing, called {0}, has completed successfully. The result was {0}", Name, Result);

Log handler work just like error handlers and there's a 

DefaultLogHandler

to get you started. This can write to std::cout but it will also raise signals that can optionally be connected
to a log aggregator which collects logs from multiple threads and synchronises dealing with them.
If you don't connect it the signals won't go anywhere and the local handler will still work.

Overriding the DefaultLogHandler enables you to customise the content and format of logs.

