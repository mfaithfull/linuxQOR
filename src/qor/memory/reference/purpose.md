qor_reference intends to provide smart reference implementations.
Smart references are in short a combined unique_ptr and shared_ptr
the client side is the same and which backend is used (locking or no locking) is determined by the trais of the class being referenced

Reference is a terrible overloaded term but then unique pointers aren't unique and shared pointers aren't shared so we're in good company.
