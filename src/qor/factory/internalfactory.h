// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef QOR_PP_H_INTERNALFACTORY
#define QOR_PP_H_INTERNALFACTORY

//The purpose of a factory is create and dispose of instances of a type.

#include "src/qor/memory/memory.h"
#include "src/qor/reference/reference.h"

namespace qor
{
    template< class T, class Tref >
    struct factoryFunctor
    {
        static void Destruct(size_t count, T* pt)
        {
            allocator_of<T>::type::Free(pt);
        }

        static Tref Construct(size_t count)
        {
            T* pt = allocator_of<T>::type::Allocate(count);
            return Tref(pt, true, count);
        }

        template< typename... _p >
        static Tref Construct(size_t count, _p... p1)
        {
            T* pt = allocator_of<T>::type::Allocate(count, p1...);
            return Tref(pt, true, count);
        }
    };

    template< class T >
    struct factoryFunctor< T, Ref< T > >
    {
        static void Destruct(size_t count, T* pt)
        {
            allocator_of<T>::type::Free(pt);
        }

        static Ref< T > Construct(size_t count)
        {
            void* pMem = source_of< detail::SharedRef< T > >::type::Source(sizeof(detail::SharedRef< T >));
            detail::SharedRef< T >* pShared = new(pMem) detail::SharedRef< T >(count);
            return pShared->_Ref();
        }

        template< typename... _p >
        static Ref< T > Construct(size_t count, _p&&... p1)
        {
            return (
                new(source_of< detail::SharedRef< T > >::type::Source(sizeof(detail::SharedRef< T >)))
                detail::SharedRef< T >(count, std::forward<_p>(p1)...)
            )->_Ref();
        }
    };

    template< class T >
    struct factoryFunctor< T, FlyerRef< T > >
    {
        static void Destruct(size_t count, T* pt)
        {
            framework::CurrentThread::GetCurrent().Context().GetFlyerMap().Unconfigure(
                guid_of<T>::guid(), ObjectContext<T>(pt->m_pPrevious)
            );
        }

        static FlyerRef< T > Construct(size_t)
        {
            ObjectContextBase context = framework::CurrentThread::GetCurrent().Context().GetFlyerMap().Lookup(guid_of<T>::guid());            
            if (!context.IsNull())
            {
                FlyerRef< T > pCurrent(context.operator T*());
                return pCurrent;
            }
            return FlyerRef< T >();
        }
    };


    template< class T >
    class InternalFactory final
    {
    private:

        InternalFactory() = delete;
		~InternalFactory() = delete;

    public:

        static void Destruct(T* pt, size_t count = 1)
        {
            factoryFunctor< T, typename ref_of<T>::type >::Destruct(count, pt);
        }
        
        static ref_of<T>::type Construct(size_t count = 1)
        {
            return factoryFunctor< T, typename ref_of<T>::type >::Construct(count);
        }

        template< typename... _p >
        static ref_of<T>::type Construct(size_t count, _p&&... p1)
        {
			return factoryFunctor< T, typename ref_of<T>::type >::template Construct<_p...>(count, std::forward<_p>(p1)...);
        }

    };
}//qor

#endif//QOR_PP_H_INTERNALFACTORY