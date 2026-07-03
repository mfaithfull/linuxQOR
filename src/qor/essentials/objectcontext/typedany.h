// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TYPEDANY
#define QOR_PP_H_TYPEDANY

#include "src/qor/essentials/objectcontext/anyobject.h"

namespace qor{

    //a type aware container for a pointer to anything
    template< class T >
    class TypedAny : public AnyObject
    {
    public:

        TypedAny(const T* pt) : AnyObject(pt){}

        virtual ~TypedAny() = default;

        template< typename U > constexpr bool IsTypeTarget(const U*) const { return false; }

        constexpr bool IsTypeTarget(const T*) const { return true; }

        operator T* () const { return (dynamic_cast< TypedAnyPointer< T >* >(m_p))->operator->(); }

    };

}//qor

#endif//QOR_PP_H_TYPEDANY
