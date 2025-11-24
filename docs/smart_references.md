
The QOR uses trait based smart references. You'll see a lot of 
auto ref = new_ref<T>()
and variations.

A smart reference is similar to a std::unique_ptr or std::shared_ptr but it can fill the roles of both and a little more
The QOR breaks the process of constructing an object into plugable stages and the result is a reference which knows how to manage the lifetime of your object and grant access to it.

Stages of getting a smart reference to a T

First the Instancer for T decides whether you need a new T or perhaps it can recycle one from a pool or just give you the one true T if it's a singleton.
Lets say it decides you need a new T then it invokes the Factory for T to produce one
The Factory knows what type of reference you want so in the default case it starts by making the internal part of the smart reference. This private part is the shared part for when you need your smart reference to behave like a std::shared_ptr
The factory puts the shared part of the reference in the same kind of memory where your object will live by using the Source for T
The Source is a raw memory pool from the OS or a memory arena or by default it just uses old fashioned new & delete to manage memory
The inner reference invokes the Allocator for T to make and initialize an actual instance of T
The Allocator may adding debugging information to the allocation and space for a back pointer to the shared part of the reference. It gets this memory as a single block from the Source for T 
At this point the T constructor is called with any parameters passed to new_ref<T>(...)
The shared part of the reference is now complete and it constructs an outer reference, a pointer to the inner reference wrapped in an object, and returns it.
The returned smart reference lives on the stack and can be passed around through function calls and duplicated as many times as required.
Every time a copy is made of the outer reference it increases the count held by the inner reference.
Every time an outer reference goes out of scope and is automatically cleaned up it decreases the count held by the inner reference
When the last outer reference expires the count goes to 0 and the inner reference hands the T back to the factory that produced it to be recycled or destroyed.
The Factory may hand the destruction off to the Allocator, which is in charge of the item memory layout so it knows how much it can free.
The Allocator hands that memory back to the Source it got it from to be returned to a pool or to the operating system by delete.

Unless you do something very unusual (in which case you're on your own) you never have to clean up after a smart reference.
There is no need for a matching delete or free with new_ref because smart references are always on the stack even though the referenced item is on the heap.

Breaking down allocation into these stages also means we can separate the concerns of object lifetime, memory management and memory performance. 
By customizing the Source, Allocator, Factory and Instancer associated with a type we can generate smart references with a variety of characteristics suitable for a lot of different objects.
However we can use them all in the same way and with the same level of safety.
