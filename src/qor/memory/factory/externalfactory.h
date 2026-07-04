// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EXTERNALFACTORY
#define QOR_PP_H_EXTERNALFACTORY

#include "src/qor/memory/injection/typeregistry.h"
#include "src/qor/memory/injection/typeregentry.h"

namespace qor
{
    //Gets the indirect factory for the implementation of T, from the type registry, and defers to it.

    template< class T >
    class ExternalFactory final 
    {
    private:

        ExternalFactory() = delete;
		~ExternalFactory() = delete;

    public:

        typedef IndirectFactory<T>* reg_entry_ptr_t;

        static void Destruct(T* t, size_t count = 1)
        {
            AnyObject registration = TheTypeRegistry()->GetFactory(*(guid_of<T>::guid()));
            IndirectFactory<T>* factory = registration;
			if(factory)
			{
                factory->Destruct(t, count);				                
			}
			else
			{
                return;
                //Singleton's with external factories have to be leaked here on process shutdown as they're no longer even 
                //registered with the TypeRegistry which itself may have gone before they are destroyed.                
			}
        }
        
        static typename ref_of<T>::type Construct(size_t count = 1)
        {
            AnyObject registration = TheTypeRegistry()->GetFactory(*(guid_of<T>::guid()));
            IndirectFactory<T>* factory = registration;
            if(factory)
            {
                return factory->Construct(count).template AsRef<T>();
            }
            else
            {
                throw std::logic_error("ExternalFactory cannot Construct an unregistered type.");
            }            
        }

		template< typename... _p >
		static typename ref_of<T>::type Construct(size_t count, _p&&... p1)
		{
			AnyObject registration = TheTypeRegistry()->GetFactory(*(guid_of<T>::guid()));
            IndirectFactory<T>* baseFactory = registration;
			if(baseFactory)
			{
                IndirectFactorywithParams<T, _p...>* factory = dynamic_cast<IndirectFactorywithParams<T, _p...>*>(baseFactory);
                if(factory)
                {
                    return factory->Construct(count, p1...).template AsRef<T>();
                }
                else
                {
                    //TODO: Raise a warning through qor_reporting
                    //throw std::logic_error("ExternalFactory couldn't find a factory with mataching parameters");
                    factory = reinterpret_cast<IndirectFactorywithParams<T, _p...>*>(baseFactory);
                    return factory->Construct(count, p1...).template AsRef<T>();
                }				
			}
			else
			{
                throw std::logic_error("ExternalFactory cannot Construct an unregistered type.");
			}
		}

    };
}//qor

#endif//QOR_PP_H_EXTERNALFACTORY