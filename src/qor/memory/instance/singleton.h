// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_INSTANCE_SINGLETON
#define QOR_PP_H_INSTANCE_SINGLETON

#include <stddef.h>

#include "src/platform/compiler/compiler.h"
#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/factory/factory.h"
#include "src/framework/parallel/sync/recursive_mutex.h"
#include "src/framework/parallel/sync/lock.h"

//The singleton instancer gives out the same instance each time unless it is destroyed by there being no more references to it. Then it gives a new one

#define qor_pp_declare_singleton(_CLASS)\
namespace qor{\
template<> struct instancer_of< _CLASS >\
{\
    typedef SingletonInstancer type;\
};\
template<> struct sync_of< _CLASS >\
{\
    typedef RecursiveMutex type;\
};\
}//qor

namespace qor{ namespace detail {

    template <class T>    
    class SingletonInstanceHolder
    {
    public:

        SingletonInstanceHolder() : initialised(false), redirected(false) {}

        virtual ~SingletonInstanceHolder()
        {
            if(redirected)
            {
                redirect->~SingletonInstanceHolder();
            }
            else
            {
                Lock lock(m);
                if(initialised)
                {
                    theRef.Dispose();
                    initialised = false;
                }
            }
        }

        virtual typename ref_of<T>::type Instance()
        {
            if(redirected)
            {
                return redirect->Instance();
            }
            else
            {
                Lock lock(m);
                if(!initialised)
                {                
                    theRef = factory_of<T>::type::Construct(count);                    
                    initialised = true;
                }
                else if(theRef.IsNull())
                {
                    theRef.Attach(factory_of<T>::type::Construct(count));
                }
                return theRef;
            }
        }
        
        template<typename... _p >
        typename ref_of<T>::type Instance(size_t, _p&&... p1)
        {
            if(redirected)
            {
                return redirect->Instance(1,std::forward<_p>(p1)...);
            }
            else
            {
                Lock lock(m);
                if( !initialised )
                {                                    
                    theRef = factory_of<T>::type::Construct(count, std::forward<_p>(p1)...);
                    initialised = true;
                }
                else if(theRef.IsNull())
                {
                    theRef.Attach(factory_of<T>::type::Construct(count, std::forward<_p>(p1)...));
                }
                return theRef;
            }
        }

        virtual void Release()
        {
            if(redirected)
            {
                redirect->Release();
            }
            else
            {
                Lock lock(m);
                if(initialised)
                {
                    factory_of<T>::type::Destruct(theRef);
                    theRef.Reset();
                }
            }
        }

        template<class D>
        void Redirect()
        {
            static SingletonInstanceHolder<D> holder;
            redirect = reinterpret_cast< SingletonInstanceHolder<T>* >(&holder);
            redirected = true;
        }

        void AutoRedirect(ref_of<T>::type ref)
        {
            Lock lock(m);
            if(!initialised)
            {
                initialised = true;
                theRef = ref;
            }
        }

    private:

        static constexpr size_t count = 1;
        ref_of<T>::type theRef;
        bool initialised;
        bool redirected;
        RecursiveMutex m;
        SingletonInstanceHolder<T>* redirect{ nullptr };
    };

    }//detail
        
	class SingletonInstancer final
	{
	public:

		template< class T >
		static inline void Release(T* /*t*/, size_t /*count = ignored_count*/)
		{
			Holder<T>().Release();
		}

		template< class T >
		static inline auto Instance(size_t /*count = ignored_count*/)
		{
            return Holder<T>().Instance();
		}

        template< class T, typename... _p >
		static inline auto Instance(size_t count, _p&&... p1)
		{
			return Holder<T>().Instance(count, std::forward<_p>(p1)...);
		}
		
        static constexpr size_t ignored_count = 1;

        template<class T, class D>
        static inline void Redirect()
        {
            Holder<T>().template Redirect<D>();
        }

        template< class T >
        static inline void AutoRedirect(ref_of<T>::type ref)
        {
            Holder<T>().AutoRedirect(ref);
        }

	private:

        template< class T>
        qor_pp_noinline static detail::SingletonInstanceHolder<T>& Holder()
        {
            static detail::SingletonInstanceHolder<T> holder;
            return holder;
        }

        SingletonInstancer() = delete;
		~SingletonInstancer() = delete;
	};

    template<class B, class D>
    class SingletonRedirector
    {
    public:
        SingletonRedirector()
        {
            SingletonInstancer::Redirect<B, D>();
        }
    };
}//qor

#endif//QOR_PP_H_INSTANCE_SINGLETON
