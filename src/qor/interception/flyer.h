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

#ifndef QOR_PP_H_FLYER
#define QOR_PP_H_FLYER

#include "src/framework/thread/thread.h"
#include "src/qor/objectcontext/typedany.h"

namespace qor {

    template< class T, class baseT >
    class Flyer : public baseT
    {
    public:

        Flyer() : m_pPrevious( nullptr ){}
        virtual ~Flyer() = default;

        bool Push()
        {
            typename ref_of< T >::type instance( dynamic_cast<T*>(this) );
            const GUID* luid = guid_of<T>::guid();
            TypedAny< T > wrapper(instance);

            AnyObject prev = framework::CurrentThread::GetCurrent().Context().GetFlyerMap().Configure( luid, wrapper);

            if(!prev.IsNull())
			{                
				m_pPrevious = prev;
            }
            return true;
        }

		bool Pop()
		{
            const GUID* luid = guid_of<T>::guid();
            TypedAny< T > wrapper(dynamic_cast<T*>(m_pPrevious));
        	framework::CurrentThread::GetCurrent().Context().GetFlyerMap().Unconfigure(luid, wrapper);
			return true;
		}

    protected:

        typedef baseT base_type;
        baseT* m_pPrevious;
    };

}//qor

#endif//QOR_PP_H_FLYER
