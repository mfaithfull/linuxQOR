// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_INSTANCE_POOL
#define QOR_PP_H_INSTANCE_POOL

#include <stddef.h>
#include <list>
#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/factory/factory.h"
#include "src/framework/parallel/sync/recursive_mutex.h"
#include "src/framework/parallel/sync/lock.h"

//The pool instancer gives out one of a limited pool of reusable resources

namespace qor{

    //Trait to determine the pool size for a type
    template<typename T>
    struct pool_size_of
    {
        static constexpr size_t size = 2;
    };

    namespace detail 
    {
        template <class T, size_t poolSize = pool_size_of<T>::size >
        class PoolInstanceHolder final
        {
        public:
            PoolInstanceHolder() : maxCount(poolSize), m_instanceCount(0) {}

            ~PoolInstanceHolder()
            {
                Lock lock(m);
                Drain();
            }

            typename ref_of<T>::type Instance()
            {
                Lock lock(m);
                typename ref_of<T>::type instance;
                if(m_FreeList.size() > 0)
                {
                    auto it = m_FreeList.begin();
                    instance = factory_of<T>::type::ConstructFromExisting(*it);
                    m_FreeList.erase(m_FreeList.begin());
                }
                else
                {
                    if(m_instanceCount < maxCount)
                    {                        
                        instance = factory_of<T>::type::Construct();
                        ++m_instanceCount;
                    }
                    else
                    {
                        //TODO: At this point we need a plug in policy to determine what to do as the pool is empty
                        //Do we give out existing in use instances or do we throw an error.
                        //TOOD: For now use qor_reporting to raise an error
                    }
                }
                return instance;
            }

            template<typename... _p >
            typename ref_of<T>::type Instance(size_t count, _p&&... p1)
            {
                Lock lock(m);
                typename ref_of<T>::type instance;
                if(m_FreeList.size() > 0)
                {
                    auto it = m_FreeList.begin();
                    instance = factory_of<T>::type::ConstructFromExisting(*it);
                    m_FreeList.erase(m_FreeList.begin());
                    instance->Inflate();
                }
                else
                {
                    if(m_instanceCount < maxCount)
                    {                        
                        instance = factory_of<T>::type::Construct(count, std::forward<_p>(p1)...);
                        m_instanceCount += count;
                    }
                    else
                    {
                        //TODO: At this point we need a plug in policy to determine what to do as the pool is empty
                        //Do we give out existing in use instances or do we throw an error.
                    }
                }
                return instance;
            }
               
            void Release(T* instance)
            {
                Lock lock(m);
                instance->Deflate();
                m_FreeList.push_back(instance);
            }

            void SetSize(size_t max)
            {
                maxCount = max;
            }

            size_t GetSize() const
            {
                return maxCount;
            }

            void Drain()
            {                
                Lock lock(m);
                for(T* t : m_FreeList)
                {
                    factory_of<T>::type::Destruct(t, 1);
                }
                m_FreeList.erase(m_FreeList.begin(), m_FreeList.end());
                m_instanceCount = 0;
            }

        private:
            size_t maxCount;
            std::list<T*> m_FreeList;
            size_t m_instanceCount;
            RecursiveMutex m;
        };

    }//detail
    
	class PoolInstancer final
	{
	public:

		template< class T >
		static inline void Release(T* t, size_t /*count*/ = 1)
		{
			Holder<T>().Release(t);
		}

		template< class T >
		static inline auto Instance(size_t /*count*/ = 1)
		{
            return Holder<T>().Instance();
		}

        template< class T, typename... _p >
		static inline auto Instance(size_t count, _p&&... p1)
		{
			return Holder<T>().Instance(count, std::forward<_p>(p1)...);
		}

        template< class T >
        static inline void SetPoolSize(size_t size)
        {
            return Holder<T>().SetSize(size);
        }

        template< class T >
        static inline size_t GetPoolSize()
        {
            return Holder<T>().GetSize();
        }

        template< class T >
        static inline void Drain()
        {
            return Holder<T>().Drain();
        }
		
	private:

        template< class T>
        static detail::PoolInstanceHolder<T, pool_size_of<T>::size >& Holder()
        {
            static detail::PoolInstanceHolder<T> holder;
            return holder;
        }

        PoolInstancer() = delete;
		~PoolInstancer() = delete;
	};

}//qor

#endif//QOR_PP_H_INSTANCE_SINGLETON
