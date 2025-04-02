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

#ifndef QOR_PP_H_EXTERNALFACTORY
#define QOR_PP_H_EXTERNALFACTORY

#include "src/qor/injection/typeregistry.h"
#include "src/qor/injection/typeregentry.h"

namespace qor
{
    //Gets the internal factory for the implementation of T, from the type registry, and defers to it.
    template< class T >
    class ExternalFactory final 
    {
    private:

        ExternalFactory() = delete;
		~ExternalFactory() = delete;

    public:

        typedef IndirectFactory<T>* reg_entry_ptr_t;

        static void Destruct(T* pt, size_t count = 1)
        {
            ObjectContextBase Registration = TheTypeRegistry()->GetFactory(*(guid_of<T>::guid()));
			if (Registration.IsNull())
			{
				throw std::logic_error("Template ID Factory cannot Destruct an unregistered class.");
			}
			else
			{
                IndirectFactory<T>* pFactory = Registration;
				return pFactory->Destruct(pt,count);
			}
        }
        
        static typename ref_of<T>::type Construct(size_t count = 1)
        {
            ObjectContextBase Registration = TheTypeRegistry()->GetFactory(*(guid_of<T>::guid()));
            if(Registration.IsNull())
            {
                throw std::logic_error("Template ID Factory cannot Construct an unregistered class.");
            }
            else
            {
                IndirectFactory<T>* pFactory = Registration;
                return pFactory->Construct(count).template AsRef<T>();
            }            
        }

		template< typename... _p >
		static typename ref_of<T>::type Construct(size_t count, _p&&... p1)
		{
			ObjectContextBase Registration = TheTypeRegistry()->GetFactory(*(guid_of<T>::guid()));
			if (Registration.IsNull())
			{
				throw std::logic_error("Template ID Factory cannot Construct an unregistered class.");
			}
			else
			{
                IndirectFactory<T>* pFactory = Registration;
                return pFactory->Construct(count, p1...);
			}
		}

    };
}//qor

#endif//QOR_PP_H_EXTERNALFACTORY