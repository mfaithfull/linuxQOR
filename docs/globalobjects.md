
The topic of global objects (singletons) can be controversial. Developers are encouraged not to use globals as a pretty hard rule, which is really an attempt to achieve two things:

1.Originally globals were discouraged to promote object oriented design, i.e. modularity. Limiting the scope and extent of variables so that code is easier to reason about and side effects can be limited, as can the blast radius when things go wrong.

2.Later this discouragement was strengthened because singletons are inherently not thread safe and when made safe they can undermine performance. It's easy to end up with an n-thread design performing slower than a single thread design as all the threads queue up to grap the same global singleton lock and hold it while they work.

This advice is sound but it leads to architectural blind spots. In the real world there are things global to your program, things you only ever want one of and things you need to be able to access from anywhere in your code. These are sometimes the same things and sometimes not. Removing singletons altogether from our thinking blinds us to possibilities that a more nuanced approach opens up. 

We often find ourselves needing or dealing with Singletons at the initial stages and top level of an application. Precisely when we're getting started both the striong advise and usually our framework of choice, lets us down by not providing a safe and clear way to do what we need. So we bodge a (temporary) solution and hope for the best, building in instabilities before we've even written any application logic.

Singletons are powerful and dangerous so treat them with care and manage them properly. Don't hide the ones you can't avoid in a dark corner and cross your fingers that they'll never come out to bite you. Put them front and centre where they usually belong. Make them safe and do everything possible to keep them limited and performant.

Some singletons provider by the QOR

Platform
ICurrentProcess
IFileSystem
AsyncAIOService
LogAggregatorService
Application

The global module registry and TypeRegistry are also singletons but you never have to create them
or interact with them directly. The outline example shows runtime, readonly, usage of the 
global module registry which is normally only used at static initialisation time.

These are things we only want one of as they have to coordinate globally to do their job or they 
are things we only want one of because they represent a resource that there really is only one of.

Creating and cleaning up singletons
QOR singletons are created using a trait factory pattern. We declare a type to be a singleton by overriding the instancer_of trait

qor_pp_declare_instancer_of(platform::IFileSystem, SingletonInstancer);

which expands to:

template<> struct instancer_of< platform::IFileSystem >{ typedef SingletonInstancer type;};

or in other words make the dependent type 
instancer_of<IFileSystem>::type 
to be SingletonInstancer

This ensures that 

instancer_of<IFileSystem>::type::Instance();

calls

SingletonInstancer::Instance<IFilesystem>();

This is called when we request an instance of IFileSystem by 

new_ref< IFileSystem >();

This then uses a function static 

SingletonInstanceHolder<IFileSystem> holder;

to access the unique instance manager for IFileSystem
It's this internal object that creates the IFileSystem 
implementation the first time it's called and returns the same one
on every subsequent call.
The SingletonInstanceHolder<IFileSystem> is cleaned up at static 
destruction (When the app closes) and will delete the IFileSystem
instance if it's still alive at that point.

All we have to do to make use of this is to call

auto fileSystem = new_ref<IFileSystem>();

No different for a Singleton or any other type.
This is part of abstracting requesting an object from what creates, 
where it comes from or even what it really is.
When we say new_ref<IFileSystem>(); we are saying,
give me something that implements IFileSystem. The rest
is negotiable.

However in order to use a Singleton safely it must be locked.
To achieve this transparently while keeping the time for
which the lock is held to a minimum the QOR provides a
temporary Access object through which we make calls on
Sigletons.

auto logPath = fileSystem(qor_shared).ApplicationLogPath();

This expands to:

auto logPath = fileSystem()().ApplicationLogPath();

which is roughly equivalent to:

{
    Access temp = fileSystem();             //Access locks the fileSystem shared reference here
    logPath = temp->ApplicationLogPath();
}                                           //Access auto destructs here, unlocking the fileSystem shared reference

Note that the lock is only held as long as the call is in progress and using the
fileSystem(qor_shared)... 
form we never get a name for, or any visbility of, the lock so there's no way to accidentally hold on to it for
longer than necessary.
In this case qor::filesystem is designed to be thread safe anyway so this isn't strictly necessary
however if we were accessing any part of the Application singleton, The Role or a Feature (built in or custom)
then this would be required.

auto logAggregator = AppBuilder().TheApplication(qor_shared)->
    GetRole(qor_shared)->
    GetFeature<LogAggregatorService>().AsRef<LogAggregatorService>();

The logAggregator is still a singular shared resource so we need to continue to use locked access on the returned reference

logAggregator(qor_shared).Receiver().WriteToStandardOutput(true);

The only references/pointers from inside shared objects that are safe to retain are Ref<T> instances
returned by the QOR.
For example is we retained the 

LogReceiver& receiver = logAggregator(qor_shared).Receiver();

Then this would be dangerous to use as we can't lock a raw C++ reference.

If we get a shared reference to the Role from inside the Application singlteon:

ref_of<IRole>::type role = AppBuilder().TheApplication(qor_shared)->GetRole();

This is safe to use as we can lock the shared reference to the Role

role(qor_shared)->GetFeature<....

and while we hold a reference to the Role (locked or unlocked) it can't be destroyed
even by the owning Application itself.
We pay for this by having to lock both the Application and the Role when we access the Role
through the Application because another thread might already hold a reference to the Role

auto logAggregator = AppBuilder().TheApplication(qor_shared)->GetRole(qor_shared)->GetFeature<LogAggregatorService>();



