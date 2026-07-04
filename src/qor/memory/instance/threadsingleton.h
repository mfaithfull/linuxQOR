// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_INSTANCE_THREADSINGLETON
#define QOR_PP_H_INSTANCE_THREADSINGLETON

#include <stddef.h>

#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/factory/factory.h"
#include "src/framework/parallel/sync/recursive_mutex.h"
#include "src/framework/parallel/sync/lock.h"

//The thread singleton instancer gives out the per thread instance each time unless it it destroyed by there being no more references to it. Then it gives a new one

namespace qor{ namespace detail {

        template <class T> class ThreadInstanceHolder;
        
        template<typename T>
        ThreadInstanceHolder<T>* theThreadInstanceHolder();
       
        template <class T>    
        class ThreadInstanceHolder final
        {
        public:
            constexpr ThreadInstanceHolder() : initialised(false) { }

            ~ThreadInstanceHolder() noexcept
            {
                if(initialised)
                {
                    theRef.Dispose();
                    initialised = false;
                }
            }

            typename ref_of<T>::type Instance()
            {
                if (!initialised)
                {
                    theRef = factory_of<T>::type::Construct();
                    initialised = true;
                }
                return theRef;
            }

            template< typename... _p >
            inline auto Instance(size_t uiCount, _p&&... p1)
            {
                if (!initialised)
                {
                    theRef = factory_of<T>::type::Construct(uiCount, std::forward<_p>(p1)...);
                    initialised = true;
                }
                return theRef;
            }

            void Release()
            {
                if(initialised)
                {
                    factory_of<T>::type::Destruct(theRef);
                    initialised = false;
                }
            }

        public:
            ref_of<T>::type theRef;
            bool initialised;
        };
        
    }//detail
    
	class ThreadSingletonInstancer final
	{
	public:

		template< class T >
		static inline void Release(T* /*t*/, size_t /*count = 1*/)
		{
            detail::theThreadInstanceHolder<T>()->Release();
		}

		template< class T >
		static inline auto Instance(size_t /*count = 1*/)
		{
            return detail::theThreadInstanceHolder<T>()->Instance();
		}

        template< class T, typename... _p >
		static inline auto Instance(size_t uiCount, _p&&... p1)
		{
			return detail::theThreadInstanceHolder<T>()->Instance(uiCount, std::forward<_p>(p1)...);
		}
		
	private:

        ThreadSingletonInstancer() = delete;
		~ThreadSingletonInstancer() = delete;
	};    

}//qor

#endif//QOR_PP_H_INSTANCE_SINGLETON
