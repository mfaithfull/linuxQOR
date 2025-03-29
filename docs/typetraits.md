The QOR makes extensive use of traits to attach information to types

For example these traits are used for memory management

qor::instancer_of<T>::type is the class used to determine whether you get a new T or a recycled one and where from
qor::factory_of<T>:: type is the factory used to provide and recycle instances
qor::allocator_of<T>::type is the class used by the instancer to allocate instances of T 
qor::source_of<T>::type is the class used to source the memory for the allocator
qor::ref_of<T>::type is the smart reference associated with T
qor::sync_of<T>::type is the synchonisation object type used to protect concurrent access to instances of T

For a more detailed explanation of how these work together see smart_references.md