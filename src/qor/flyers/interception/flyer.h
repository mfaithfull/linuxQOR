// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FLYER
#define QOR_PP_H_FLYER

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/essentials/objectcontext/typedany.h"

namespace qor {

    template< class T, class baseT >
    class Flyer : public baseT
    {
    public:

        Flyer() : baseT() { }
        virtual ~Flyer() = default;
        Flyer(const Flyer& src) = delete;
        Flyer& operator = (const Flyer& src) = delete;

        bool Push()
        {
            typename ref_of< T >::type instance( dynamic_cast<T*>(this) );
            const GUID* luid = guid_of<T>::guid();
            TypedAny< T > wrapper(instance);

            m_anyPrevious = CurrentThread::GetCurrent().Context().GetFlyerMap().Configure( luid, wrapper);

            if(!m_anyPrevious.IsNull())
			{
                m_Previous = reinterpret_cast<const TypedAnyPointer< baseT >*>(m_anyPrevious.Ptr())->operator baseT *();
            }
            return true;
        }

		bool Pop()
		{
        	CurrentThread::GetCurrent().Context().GetFlyerMap().Unconfigure(guid_of<T>::guid(), m_anyPrevious);
			return true;
		}

    protected:

        typedef baseT base_type;
        AnyObject m_anyPrevious;
        baseT* m_Previous{nullptr};
    };

}//qor

#endif//QOR_PP_H_FLYER
//Review 2027/07/07