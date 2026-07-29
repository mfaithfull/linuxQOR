#Flyers - Dependency Injection on the Fly

This article describes the Flyer software pattern and it's particular implementation in the QOR Framework.

###Intro

We'd like to write Single Responsibility code. After All Uncle Bob says we should :-;
He's right but no matter how we try there are certain kinds things that show up all over our code, across classes and functions. Things there is no reasonable way to abstract out or invert dependencies except to inject them into each class. Error handling and logging are obvious examples.

I'm sure you've injected a Serilog logger into your REST API handler or something equivalent. I know I have. That's the best we can do isn't it? Rather than take a dependency on a specific logger we take a dependency on a logger interface and that interface goes into the signature of our constructor

```
class Processor
{
    Processor(ILogger logger...) : m_logger(logger)
    {...}
};
```

That's OK for logging but what if we want profiling and error handling and translation services and perhaps serialization or messages servies?
Pretty soon we've got a hedgehog constructor dependent on half a dozen interface types. This is the current state of the art in C derived languages and we generally accept it as the best we can do. 

###How this works

Here's a C++ function which takes no parameters and returns nothing. So how is it able to do anything useful?

```
void Processor::Process()
{
    data = m_request.DecodeWhatWeNeed();
    result = m_complexCalculation.DoItQuickly();
    m_response.Encode(result);
}
```

Obviously because it's a member function. It has implicit access to *this*.

The above function is equivalent to:

```
void Processor_Process(Processor* this)
{
    data = this->m_request.DecodeWhatWeNeed();
    result = this->m_complexCalculation.DoItQuickly();
    this->m_response.Encode(result);
}
```

It's easy to forget that every member function compiles to the equivalent of a function taking a pointer as it's first parameter.
This single hidden variable trick is what gives us objects in C++ and C# and probably a dozen other languages by now.
inheritance, vtables, even multiple inheritance. All possible with a single hidden this pointer.

Now imagine for a moment that we had complete arbitrary control of this mechanism.
We could add extra hidden variables at will.

```
void Processor_Process(Processor* this, Logger* thisLogger, Error* thisErrorHandler, Translator* thisTranslator, Serializer* thisSerializer)
{
    ...
}
```

and we could get all this with just:

```
void Processor::Process()
{
    ...
}
```

That would be amazing but it would also mean 4 x sizeof(pointer) extra bytes on the stack for every call and what if we don't need thisSerializer in most of our functions?
We only want to pay for what we use even if that means paying a little more when we do use it.

###Here come the Flyers

Can we do it?

Not directly with automatically passed hidden variables. That would require heavily modifying the compiler and then we wouldn't be using the same language anymore.
However what if instead of putting these pointers into every function we put them into the thread of execution instead and then something like this 
would work.

```
void Processor::Process()
{
    SendMessage(CurrentThread().Translator().Translate("Message to customer."));
    ...
    if(Error)
    {
        CurrentThread().ErrorHandler().HandleError(Error);
    }
}
```

It's a little more work and a little more verbose than magic hidden variables but we only pay for the services we use and the Processor constructor no longer has to explicitly depend on all the interfaces we might need. Processor becoming default constructible has all sorts of benefits in C++ but mostly it's just cleaner and simpler.

```
Processor::Processor(){}
```

To do this we obviously need a mechanism to retrieve the CurrentThread() and a way to attach all sorts of things to it. We could use Thread Local Storage directly to implement these Flyers but that's not as portable as we might want so some supporting infrastructure will be required.

There's one more important aspect to the Flyer pattern. 
What if I already have a general Error handling Flyer available on my thread but for some specific code I want a special error handler?
For example in a Windows only subclass I want a handler that can deal with Windows specific errors? There are a few of those after all.
Flyers address this by stacking.

```
WindowsProcessor::Process()
{
    Win32ErrorHandler win32ErrorHandler;
    //Win32 specific handling code
    ...
}
```

This puts a Win32ErrorHandler on the function stack but because Win32ErrorHandler is a Flyer derived from ErrorHandler it registers itself as the current ErrorHandler Flyer with the Thread, replacing the previous ErrorHandler until it goes out of scope and then putting the old one back.

Before -

Thread:
Flyers: ErrorHandler->BaseErrorHandler

When WindowsProcessor::Process is called 

```
WindowsProcessor::Process(){...
Win32ErrorHandler win32ErrorHandler;
```

Thread:
Flyers: ErrorHandler->Win32ErrorHandler

```
...}
```

After -

Thread:
Flyers: ErrorHandler->BaseErrorHandler

This makes Flyers incredibly easy to make and to replace for a given scope. You just create one on the stack with the lifetime that you need.
Getting them from the CurrentThread is done by type and is a bit more involved

```
AnyObject context = CurrentThread::GetCurrent().Context().GetFlyerMap().Lookup(guid_of< T >::guid());
FlyerRef< T > pCurrent(context.operator T*());
return pCurrent;
```

fortunately that's all wrapped in a factory so all you actually have to do is

```
auto ptrErrorHandler = new_ref< ErrorHandler >();
if(ptrErrorHandler)
{
    ptrErrorHandler->DealWithTheError(...
```

Rather than allocating a new ErrorHandler on the heap as it might at first appear. This just returns the current ErrorHandler previsously injected into the Thread.

To create an entirely new kind of Flyer:

First we need a simple base so that others can derive from our Flyer and the replacement mechanism will work.

```
    class MyBaseFlyerType;
    constexpr GUID MyBaseFlyerTypeGUID = {0x12345678, ...};
    qor_pp_declare_guid_of(MyBaseFlyerType,BaseFlyerTypeGUID);
    qor_pp_declare_ref_of(MyBaseFlyerType,FlyerRef);
    
    class MyBaseFlyerType
    {
    public:
        MyBaseFlyerType() = default;
		virtual ~MyBaseFlyerType() = default;      
    };
```    

We use a QOR type trait here to attach a GUID to the MyBaseFlyerType. The registation and lookup mechanism uses this to identify the type of Flyer.

```        
    class MyFlyer : public Flyer< MyFlyer, MyBaseFlyerType >
    {
    public:

        MyFlyer()
        {
            Flyer< MyFlyer, MyBaseFlyerType >::Push();
        }

		virtual ~MyFlyer()
        {
            Flyer< MyFlyer, MyBaseFlyerType >::Pop();
        }

        virtual bool HandleWhateverThisFlyerIsDesignedFor(...)
        {
            ...
        }        
    };    
```

This is suffcient to make MyFlyer a Flyer type which will stack with anything derived from MyBaseFlyerType.

Did we do away with our dependency on loggers and error handlers entirely?
No. We are still using them so we still have header and link time dependency on those mechanisms. However we're still only dependent
on the interface types unless we want to specialize services and we no longer have to inject them into our constructors and store pointers to them in each class instance.

We only pay at runtime for the services we need, when we actually need them. We can replace them with more specialised variants as easily as 
making a local variable. We can retrieve the pointers we used to inject and store with a new_ref< T >(...) which is ubiquitous within the QOR.
The mechanism is inherently thread safe because, by design, it never steps outside the current thread. 
All Flyers are cleaned up automatically, they live on the stack and they never need to be heap allocated.

The QOR framework provides the infrastructure to make Flyers work and uses them for error handling, logging and more.
Dependency injection in C++ just grew up.

https://github.com/mfaithfull/linuxQOR

