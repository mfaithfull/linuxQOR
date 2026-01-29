The QOR Application class is a singleton built by the AppBuilder class.

Never create one directly yourself. Always use AppBuilder.Build() to make the Application
and AppBuilder.TheApplication() to access it. Even if you've overrriden it (A separate topic)

The Application only has two properties a Name and a Role
You set the name when you create it with the AppBuilder and the Role with SetRole

The Role is a simple container for Features and these are where the useful stuff happens

When you call Run, or it's variants, on your Application, 
the Role will call Startup on each of the Features you've added before the main Runable is 
executed.
When execution completes the Role will call Shutdown on each of the Features in the reverse
order of Startup calls.

This simple structure gives a way to pull together separately developed Features and integrate them
even when there are dependencies between them.
It aims to support scaling your application to almost any size and also be reasonably easy to use for
small projects.

Application
    Role
        Feature
        Feature
        ...

All of these are also customisation points if you need them to be. Custom Features being the easiest and
most obviously necessary.






