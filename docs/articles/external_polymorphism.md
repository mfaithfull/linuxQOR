
External Polymorphism - Ways of getting the same extensibility and composability of types you get with 'normal' inheritance based polymorphism but without virtual functions or thunks and their associated costs. Without changing the types that you're extending and composing or imposing heavy requirements on them.

The idea has been around for at least thirty years and there have been lots of C++ implementations over the years. Mostly they started out recreating vtables outside the target classes in an effort to implement type erasure. These days type erasure is common place but, useful as it is, it is only one form of external polymorphism.

I've recently been porting a std::function alternative library [magic_func](https://github.com/leandro-gracia-gil/magic_func) which uses type erasure over function pointers and also looking at [Ilya Korolev's Contracts library](https://github.com/antako76/Contract) which puts what could maybe be external polymorphism back into the types being adapted. That gives you access to all the internals of the target type and uses the encapsulation of the target type to encapsulate the contract machinery as well. 
Can we do some of what Contracts is doing without modifying the target classes and at the same time do type safe external polymorphism rather than fully generic type erasure.?

Using the principles of code sfumato we'll see if we can gradually build something useful starting very simple. 
Let's say we have some kind of business object...

```C++
namespace domain
{
    struct datatype
    {        
        unsigned int accno;
        float ammount;
        std::string name;
        std::vector<std::string> log;
    };  
}
```

Everything is public. Everything is a value type. This is just plain data.

There are two ways we could bind something external to a member of 
```domain::datatype```in C++. Either by pointer or by reference. 
Pointers are easy to type erase, they're all the same size and they don't have to be initialised. The downside is that because they don't have to be initialised, they **do** have to be null checked and because they are stored and have size they can't easily be elided by the optimiser. 
References are somewhat more slippery to deal with but we'll give them try. Mostly because in the past I've always opted for pointers and I've never been super happy with the result.

```C++
template< typename T >
struct ValueAdapter
{
    ValueAdapter(T& t) : m_(t)
	{ }        
private:
	T& m_;
};
```

Not bad as a first attempt. We can construct a ```ValueAdapter(x)``` on any value of type T and we have an object entirely external to T which can operate on x.
To make ```ValueAdapter``` useful though we need to extend it with the operations we want to perform on our captured T&.

When I first started messing around with external polymorphism it was extremely difficult to do. In 2003 tuples were a PhD thesis by a Finnish researcher and I may have been the first ever person to see 
```tuple::get<0>(t)``` compile under under a Microsoft compiler and execute correctly on Windows. It needed compiler specific template and macro hackery to make the port work (barely) with a maximum of 5 elements and it wasn't practical.
These days many extraordinary things are possible in modern C++. We'll come back to tuples. Lets try to improve ```ValueAdapter```.

```C++
template <typename T, typename... Bases>
struct ValueAdapter : public Bases...
{
    using value_type = T;
    ValueAdapter(T& t) : Bases(std::forward<T&>(t))...
    { }        
};
```

A second attempt at ```ValueAdapter``` lets us provide it with a type for the value and a list of types to inherit from, each of which gets constructed with the T&, passed to ```ValueAdapter```. We no longer need to hold a reference in this root object as each operation that needs one will hold onto one itself.

You might notice the Curiously Recuring Template Pattern (CRTP) hiding here in parameter pack form.

To make use of this we'll need to write a set of operations that can be composed into a ```ValueAdapter```. Starting with a simple Read to get the value.

```C++
template<typename T>
struct Read
{    
    Read(T& t) : m_(t){ }

    T Get() const {return m_;}
    
private:
    T& m_;
};
```

Here's an equally simple Write to set the value.

```C++
template<typename T>
struct Write
{
    Write(T& t) : m_(t){ }

    void Set(const T t){ m_ = t; }    
    
private:

    T& m_;
};
```

These operations are so trivial and specific in what they do that they might even be considered bug free, maybe.

Now we can make an actual ```ValueAdapter``` type to eventually attach to ```unsigned int datatype::accno``` :

```C++
using AccNoAdapter = ValueAdapter< unsigned int, Read< unsigned int >, Write< unsigned int > >;
```

If we left out the Write we'd have a read only value and if we left out the Read we'd have a write only value. Both things we might want at some point. A practical library would certainly provide ```ReadOnlyValueAdapter``` as a type and many other operations we could plug in so we might end up with something like:

```C++
using AccNoAdapter = ReadOnlyValueAdapter< unsigned int, StreamOut< unsigned int >, ToString< unsigned int >, Name< unsigned int > >
```

This is fine but a bit tedious to type with the repetition of ```unsigned int```, the type of the underlying thing we're binding to.
It should be possible to just say we want the ```ReadOnlyValueAdapter```
type with an ```unsigned int```
and a list of operations, that also get the ```unsigned int``` automatically.

```C++
template<
template<class...> class P, class T, template<class> class... L
>
struct adapter_apply
{
    using type = P<T, L<T>... >;
};
```

With this beauty using template templates and pack expansion it is. We can now say:

```C++
using AccNoAdapter = adapter_apply< ReadOnlyValueAdapter, unsigned int, StreamOut, ToString, Name >::type;
```

```AccNoAdapter``` becomes a ```ReadOnlyValueAdapter``` templated on ```unsigned int``` and the expanded list of types, each templated on ```unsigned int``` exactly as we did before by hand.

That takes care or attaching almost anything we can think of to a single value type.
```C++
unsigned int accno;
AccNoAdapter accnoAdapter(accno);
```
attaches reading, writing, naming and streaming operations to ```accno``` so we can do things like.
```C++
accnoAdapter.SetName("Bob");
accnoAdapter.Set(163);
std::cout << accnoAdapter;
```

Here are a couple more ```ValueAdapter``` types we need to build to address the members of domain::datatype

```C++
using AmmountAdapter = adapter_apply< ValueAdapter, float, Read, EnhancedNumericWrite, StreamIO, Name >::type;

using LogAdapter = adapter_apply< CollectionAdapter, std::vector<std::string>, ReadCollection, Append, StreamOutCollection >::type;
```

We've painted the first layer of external polymorphism, Value Adapters.

Now we'd like to adapt entire objects at a time. In practice what we usually want is to provide specific interfaces over those objects that meet the requirements of something external to our domain.

```C++
template <typename... Args>
struct InterfaceAdapter : public std::tuple<Args...>
{
    using storage_type = std::tuple<Args...>;

    InterfaceAdapter(Args&&... args) : 
	    std::tuple<Args...>{std::forward<Args>(args)...}
    { }
    //...
```

This is where ```tuples``` come in handy. Under the hood they are derived from all their arguments recursively and come with helpful and reliable standard library machinery like iteration and element lookup.
Otherwise the pattern is similar to what we did with ```ValueAdapter``` except that now the composed base classes are the ```ValueAdapter``` types we built up for the individual members.

```C++
using PaymentInterface = InterfaceAdapter< AccNoAdapter, AmmountAdapter, LogAdapter >;
```

These interface types are a kind of two dimensional class. A tuple of ```ValueAdapters```, one for each member being referenced, with each ```ValueAdapter``` being a collection of operations attached to that member.

We need to bind this interface type to the underlying storage type ```domain::datatype``` that we started with. For this we'll need a binding adapter

```C++
template<class interface_t, class storage, typename field_enum>
struct Adapter : public interface_t, protected storage
{            
    template <typename... Args>
    Adapter(storage& src, Args&&... args) : 
	    storage(src), interface_t(args...)
    { }

    //accessor for fields
    template<field_enum f>
    auto& at() { return std::get<static_cast<int>(f)>(*this); }

    template<field_enum f>
    struct type_of_field
    {
        using type = std::invoke_result_t<
	        decltype(
		        &Adapter<interface_t, storage, 
		        field_enum>::template at<f>
		    ), 
		    Adapter<interface_t, storage,
		    field_enum> >;
        };
    };
```

In order to map the elements from our ```ValueAdapter``` tuple to members in ```domain::datatype``` we also need to provide the adapter with an ```enum class```

```C++
enum class paymentfields
{
    account,
    ammount,
    log
};
```

This just gives names to constants 0, 1 & 2 so that we can say things like
```C++
payment.at<paymentfields::account>().SetName("Account");
``` 

We'll get to that. First we need to combine our interface and enumeration types together to form a specific adapter type.

```C++
template<class storage>
struct PaymentAdapter : public Adapter<PaymentInterface, storage, paymentfields>
{
    PaymentAdapter(storage& src) : Adapter<PaymentInterface, storage, paymentfields>(src,
        storage::accno,//the position in this list
        storage::ammount,//must match the enum value
        storage::log)//of the associated ValueAdapter
    { }
};
```

```storage``` is still a template parameter here but we know it must have at least the named fields, in any order, for this template to be instantiated.
It's important to note that because ```Adapter``` inherits from ```storage``` ```
PaymentAdapter``` is an instance of ```storage```. The underlying struct will be copied when bound to ```PaymentAdapter``` and it will operate on the copy.
This is fine for outgoing adapters that read ```storage``` and provide an interface for something external but if we want to be able to modify the original storage type instance through the interface then we need a ```
Port``` rather than an ```Adapter```.

```C++
template<class interface_t, class storage, typename field_enum>
struct Port : public interface_t
{                
    template <typename... Args>
    Port(storage& src, Args&&... args) : 
	    m_(src), interface_t(args...)
    { }

    //accessor for fields
    template<field_enum f>
    auto& at() { return std::get<static_cast<int>(f)>(*this); }

    template<field_enum f>
    struct type_of_field
    {
        using type = std::invoke_result_t<
	        decltype(&Port<interface_t, storage,
		        field_enum>::template at<f>),
		    Port<interface_t, storage, field_enum> >;
    };

private:

    storage& m_;
};
```

A Port over the ```PaymentInterface``` and the same ```paymentfields``` enumeration gives us:

```C++
template<class storage>
struct PaymentPort : public Port<PaymentInterface, storage, paymentfields>
{
    PaymentPort(storage& src) : 
	    Port<PaymentInterface, storage, paymentfields>(src,
	    src.accno,//The refenced fields from the src
        src.ammount,//are bound to the ValueAdapters
        src.log)//in the order of the paymentfields enum
    { }
};
```

With all the type infrastructure we need in hand we can now do:

```C++
using adapter_t = PaymentAdapter<domain::datatype>;

domain::datatype data{ 20015678, 56.8f, "name" };
adapter_t payment(data);                                   
```

```payment``` is now a ```PaymentInterface``` bound to a copy of data. We can use the ```paymentfields enum``` to access the ```ValueAdapters``` and directly call the bolted on operations as if they were functions on members of ```domain::datatype```.

```C++
payment.at<paymentfields::account>().SetName("Account");
auto a = payment.at<paymentfields::account>().Get();

payment.at<paymentfields::ammount>().SetName("Payment Ammount");
payment.at<paymentfields::ammount>().SetMinimum(0.01f);
payment.at<paymentfields::ammount>().SetMaximum(10000.0f);

payment.at<paymentfields::ammount>().SetOnChange(qor_pp_make_function(&OnPaymentAmmountChange));

payment.at<paymentfields::ammount>().Set(103.8f);           
```

There's more though. I didn't show you the full implementation of 
```InterfaceAdapter``` above. It enables us to do things like applying a lambda template to each of the fields on an interface

```C++
payment.for_each_field([](size_t& index, const auto& x)     
{                        
    if constexpr(is_streamoutable<decltype(x)>())
    {
        if (index++ > 0) std::cout << ", ";
        if constexpr(is_named<decltype(x)>())
        {
            std::cout << x.GetName() << ": ";
        }
        x.operator << (std::cout);
    }
});
```

This iterates over the tuple of ```ValueAdapters``` on the ```PaymentInterface``` calling the lambda with each one. The lambda has to be a template because each ```ValueAdapter``` has a different type.
The ```is_streamoutable``` and ```is_named``` functions check whether the field being processed has particular operations attached to it. These are ```constexpr``` functions so the parts of the lambda that won't work never even make it into the binary.
In this way we can filter fields for ones that can be printed or read or written or whatever we need.

```C++
template<typename x>
constexpr bool is_named()
{
    using x_type = std::remove_cvref_t<x>;
    if constexpr(is_valueadapter<x>())
    {
        using v_type = typename x_type::value_type;
        return std::is_base_of_v<Name<v_type>, x_type>;
    }
	/*More stuff to handle Collections*/
    return false;
}
```

To use a ```Port``` that can modify our original object we wrap a ```PaymentPort``` around our ```domain::datatype```.

```C++
using port_t = PaymentPort<domain::datatype>;
port_t pay(data);

pay.at<paymentfields::ammount>().Set(34.67f);
pay.at<paymentfields::log>().push_back("Set ammount to 34.67");
```

```pay``` holds a reference to ```data``` so these lines modify the contents of ```data``` directly.

The types of individual fields can be retrieved with 
```interface_t::type_of_field< enum_index >::type``` 
as seen here where we check if the account field is convertible into a string before attempting to write it out. ```is_stringable``` works just like ```is_named```. It checks if the type is derived from the ```ToString<>``` operation.

```C++
if constexpr(is_stringable<
	adapter_t::type_of_field<paymentfields::account>::type >())
{
   std::cout << 
	   payment.at<paymentfields::account>().toString();
}
```

That's it. There are undoubtedly wrinkles. Every implementation of external polymorphism I've come across has them but with this small library of templates we can build almost any functionality over a set of plain data objects without virtual functions and without modifying them.
There are no vtables. We don't even have to build artificial ones ourselves. The equivalent is the lookup done by ```std::get< i >( T )``` to get a reference into our tuple of ```ValueAdapters```.
We can have multiple specialist interfaces over the same type without any cross dependency or additional cost and we can add unlimited meta data, validation, etc to those interfaces. We can introspect the operations available on a data item and branch on the result with no runtime cost. Every custom operation is available for reuse on any interface without even having to derive from it.

When templates were still a non standard feature, liable to crash your compiler, and tuples were academic research we dreamed of being able to do things like this. What do you dream of being able to do that's just out of reach with C++ today?

Source code is available on the DomainInterfaceObjects branch of the [QOR](https://github.com/mfaithfull/linuxQOR/tree/DomainInterfaceObjects)If you think it's of value let me know and I'll pull it into main.
