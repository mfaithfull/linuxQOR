// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "test_common.h"

int FreeFunction(bool& called, bool&& value) 
{
    called = std::move(value);
    return 42;
}

int Sum(int x, int y) 
{
    return x + y;
}

Object::Object(int id) : id_(id) 
{ }

Object::Object(const Object& object) : id_(object.id_) 
{ }

Object::Object(const volatile Object& object) : id_(object.id_) 
{ }

int Object::Sum(int x, int y) 
{
    return id_ + x + y;
}

int Object::Function(bool& called, bool&& value) 
{
    called = std::move(value);
    return id_;
}

int Object::ConstFunction(bool& called, bool&& value) const 
{
    called = std::move(value);
    return id_;
}

int Object::VolatileFunction(bool& called, bool&& value) volatile 
{
    called = std::move(value);
    return id_;
}

int Object::ConstVolatileFunction(bool& called, bool&& value) const volatile 
{
    called = std::move(value);
    return id_;
}

int Object::VirtualFunction(bool& called, bool&& value, bool* derived) 
{
    called = std::move(value);
    *derived = false;
    return id_;
}

char Object::ConvertibleFunction(const std::string& str,
                                 const unsigned int index) 
{
    return str[index];
}

int Object::Overloaded(CVQualification& cv) 
{
    cv = CVQualification::kNonQualified;
    return id_;
}

int Object::Overloaded(CVQualification& cv) const 
{
    cv = CVQualification::kConstQualified;
    return id_;
}

int Object::Overloaded(CVQualification& cv) volatile 
{
    cv = CVQualification::kVolatileQualified;
    return id_;
}

int Object::Overloaded(CVQualification& cv) const volatile 
{
    cv = CVQualification::kConstVolatileQualified;
    return id_;
}

DerivedObject::DerivedObject(int id) : Object(id) 
{ }

int DerivedObject::VirtualFunction(bool& called, bool&& value, bool* derived) 
{
    called = std::move(value);
    *derived = true;
    return id_;
}

OverloadedCallable::OverloadedCallable(int id) : id_(id) 
{ }

OverloadedCallable::OverloadedCallable(const OverloadedCallable& callable) : id_(callable.id_) 
{ }

OverloadedCallable::OverloadedCallable( const volatile OverloadedCallable& callable) : id_(callable.id_) 
{ }

int OverloadedCallable::operator ()(CVQualification& cv) 
{
    cv = CVQualification::kNonQualified;
    return id_;
}

int OverloadedCallable::operator ()(CVQualification& cv) const 
{
    cv = CVQualification::kConstQualified;
    return id_;
}

int OverloadedCallable::operator ()(CVQualification& cv) volatile 
{
    cv = CVQualification::kVolatileQualified;
    return id_;
}

int OverloadedCallable::operator ()(CVQualification& cv) const volatile 
{
    cv = CVQualification::kConstVolatileQualified;
    return id_;
}
