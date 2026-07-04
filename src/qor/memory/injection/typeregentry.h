// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TYPEREGENTRY
#define QOR_PP_H_TYPEREGENTRY

#include "typeregistry.h"
#include "src/qor/memory/reference/reference.h"

namespace qor {

    template<class I>
    class IndirectFactory   //This is what gets registered in the TypeRegistry but the real type may be one of the derived types below
    {
    public:

        virtual ref_of<I>::type Construct(size_t count = 1) const
        {
            return InternalFactory<I>::Construct(count);
        }

        virtual void Destruct( I* i, size_t count = 1) const
        {
            InternalFactory<I>::Destruct(i, count);
        }

        virtual ~IndirectFactory() = default;
    };

    template<class I, typename... _p>
    class IndirectFactorywithParams : public IndirectFactory<I>
    {
    public:
        virtual ref_of<I>::type Construct(size_t count, _p... p1) const
        {
            return InternalFactory<I>::template Construct<_p...>(count, p1...);
        }

        virtual void Destruct( I* i, size_t count = 1) const
        {
            InternalFactory<I>::Destruct(i, count);
        }

        virtual ~IndirectFactorywithParams() = default;
    };


    //T is the type we're registering for. I is the real (derived) implementation type
    //new_ref<T>(...) will return ref_of<I>::type holding a new I(...)
    template<class T, class I = T, typename... _p>
    class TypeRegEntryWithParams : public IndirectFactorywithParams<I, _p...>
    {
    public:

        inline TypeRegEntryWithParams()
        {
            TheTypeRegistry()->Register( *(guid_of<I>::guid()), TypedAny< IndirectFactory<I> >(dynamic_cast< IndirectFactory<I>* >(this)));
        }

        virtual ~TypeRegEntryWithParams()
        {
            TheTypeRegistry()->Unregister( *(guid_of<I>::guid()));
        }

        virtual ref_of<I>::type Construct(size_t count, _p... p1) const
        {
            return InternalFactory<T>::template Construct<_p...>(count, p1...).template AsRef<I>();
        }

        virtual ref_of<I>::type Construct(size_t count = 1) const
        {
            return InternalFactory<T>::Construct(count).template AsRef<I>();
        }

        virtual void Destruct( I* i, size_t count = 1) const
        {
            auto t = dynamic_cast<T*>(i);
            if(t != nullptr)
            {
                InternalFactory<T>::Destruct(t, count);
            }
            else
            {
                InternalFactory<I>::Destruct(i, count);
            }
        }
    };

    template<class T, class I = T>
    class TypeRegEntry : public IndirectFactory<I>
    {
    public:

        inline TypeRegEntry()
        {
            TheTypeRegistry()->Register(*(guid_of<I>::guid()), TypedAny< IndirectFactory<I> >(dynamic_cast< IndirectFactory<I>* >(this)));
        }

        virtual ~TypeRegEntry()
        {
            TheTypeRegistry()->Unregister(*(guid_of<I>::guid()));
        }

        virtual ref_of<I>::type Construct(size_t count = 1) const
        {
            return InternalFactory<T>::Construct(count).template AsRef<I>();
        }

        virtual void Destruct(I* i, size_t count = 1) const
        {
            InternalFactory<T>::Destruct(dynamic_cast<T*>(i), count);
        }
    };

}//qor

#endif//QOR_PP_H_TYPEREGENTRY
