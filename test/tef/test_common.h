// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEST_TEF_COMMON
#define QOR_PP_H_TEST_TEF_COMMON

#include <string>
#include <utility>

// Different overload types by cv qualification.
enum class CVQualification {
    kUndefined = 0,
    kNonQualified,
    kConstQualified,
    kVolatileQualified,
    kConstVolatileQualified,
};

// Sample free function using lvalue and rvalue references.
int FreeFunction(bool& called, bool&& value);

// Another free function that returns the sum of two values.
int Sum(int x, int y);

// Sample object holding an integer value.
class Object {
public:
    // Sets the integer value returned by functions.
    explicit Object(int id);

    // Explicit copy constructors to allow storing volatile objects.
    // Implicit ones do not support volatile.
    explicit Object(const Object& object);
    explicit Object(const volatile Object& object);

    // Returns the sum of the provided arguments and the stored object id.
    int Sum(int x, int y);

    // Example member function using lvalue and rvalue references.
    // Sets in called the provided value and returns the object id.
    int Function(bool& called, bool&& value);

    // Example const member function using lvalue and rvalue references.
    // Sets in called the provided value and returns the object id.
    int ConstFunction(bool& called, bool&& value) const;

    // Example volatile member function using lvalue and rvalue references.
    // Sets in called the provided value and returns the object id.
    int VolatileFunction(bool& called, bool&& value) volatile;

    // Example const volatile member function using lvalue and rvalue references.
    // Sets in called the provided value and returns the object id.
    int ConstVolatileFunction(bool& called, bool&& value) const volatile;

    // Example virtual function using lvalue and rvalue references.
    // Sets in called the provided value and returns the object id.
    // Sets derived to false when called.
    virtual int VirtualFunction(bool& called, bool&& value, bool* derived);

    // Function used to test convertibility of argument and return types.
    // Returns the index-th character of a string.
    char ConvertibleFunction(const std::string& str, const unsigned int index);

    // Overloaded member function by cv qualifiers.
    int Overloaded(CVQualification& cv);
    int Overloaded(CVQualification& cv) const;
    int Overloaded(CVQualification& cv) volatile;
    int Overloaded(CVQualification& cv) const volatile;

protected:
    int id_;
};

// Override of the virtual function from the class above.
class DerivedObject : public Object 
{
public:
    // Sets the integer value returned by functions.
    explicit DerivedObject(int id);

    // Example virtual function using lvalue and rvalue references.
    // Sets in called the provided value and returns the object id.
    // Sets derived to true when called.
    int VirtualFunction(bool& called, bool&& value, bool* derived) override;
};

// Callable object that overloads its operator () with cv qualifiers.
class OverloadedCallable {
public:
    // Sets the integer value returned by functions.
    explicit OverloadedCallable(int id);

    // Explicit copy constructors to allow storing volatile objects.
    // Implicit ones do not support volatile.
    explicit OverloadedCallable(const OverloadedCallable& callable);
    explicit OverloadedCallable(const volatile OverloadedCallable& callable);

    // Overloads of operator ().
    int operator()(CVQualification& cv);
    int operator()(CVQualification& cv) const;
    int operator()(CVQualification& cv) volatile;
    int operator()(CVQualification& cv) const volatile;

private:
    int id_;
};

#endif//QOR_PP_H_TEST_TEF_COMMON
