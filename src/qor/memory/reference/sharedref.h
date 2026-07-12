// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_SHAREDREF
#define QOR_PP_H_SHAREDREF

#define qor_shared )(
#define qor_unlocked ).Unlock()(

namespace qor{

	template< class T > class Ref;
	template< typename T> void internal_del_ref(T* p);

	namespace detail
	{
        template< typename R, bool bValue >
        struct AllocateOnlyConcreteTypesFunctor
        {
            template< typename... _p >
            static R* Allocate(size_t /*count*/, _p&&... /*pq*/)
            {
				//TODO: Raise an error through qor_reporting
                return nullptr;
            }
        };

        template< typename R >
        struct AllocateOnlyConcreteTypesFunctor<R, false>
        {
            template< typename... _p >
            static R* Allocate(size_t count, _p&&... p1)
            {
                return allocator_of<R>::type::template Allocate<R>(count, std::forward<_p>(p1)...);
            }
        };

		template< typename Lock_t >
		struct LockIsRealInternal
		{
			typedef std::true_type type;
		};

		template<>
		struct LockIsRealInternal<NullSection>
		{
			typedef std::false_type type;
		};


		//This is the owning shared reference class which has the pointer to the object
		//It also holds the reference count
		template< class R >
		class SharedRef final
		{
			friend class ::qor::Ref< R >;

		private:

			typedef typename std::is_abstract<R> is_abstract;

		public:

			template< typename... _p >
			SharedRef(size_t count, _p&&... p1) : m_p(nullptr), m_RefCount(0), m_Section()
			{
				//This is where the underlying raw object gets allocated iff its type is constructable
				m_p = AllocateOnlyConcreteTypesFunctor<R, is_abstract::value>::template Allocate<_p...>(count, std::forward<_p>(p1)...);

				if (m_p != nullptr)
				{
					//Insert a link to this into the new objects allocated space so that from the object this shared reference can be found
					int** ppBack = ((int**)m_p) - 1;
					*ppBack = (int*)(this);
				}
				else
				{
					throw new std::logic_error("Cannot make an instance of an abstract type.");
				}
			}

			SharedRef(R* pr) : m_p(pr), m_RefCount(0), m_Section()
			{
				if(m_p != nullptr)
				{
					int** ppBack = ((int**)m_p) - 1;
					*ppBack = (int*)(this);
				}
			}

            R* AllocateConcreteType(size_t count)
            {
                return allocator_of<R>::type::template Allocate<R>(count);
            }


			//The same as above but for default constructed target types
			SharedRef(size_t count) : m_p(nullptr), m_RefCount(0), m_Section()
			{
                if(!is_abstract::value)
                {
    				m_p = AllocateConcreteType(count);
                }

				if (m_p != nullptr)
				{
					int** ppBack = ((int**)m_p) - 1;
					*ppBack = (int*)(this);
				}
				else
				{
					throw new std::logic_error("Cannot make an instance of an abstract type.");
				}
			}

			SharedRef() : m_p(nullptr), m_RefCount(0), m_Section()
			{
			}

			SharedRef(const SharedRef< R >& Src) = delete;
			SharedRef& operator = (const SharedRef< R >& Src) = delete;

			//Construct and return an outer non shared reference around this inner shared reference
			Ref< R > _Ref(void) const
			{
				return Ref< R >(this);
			}

			virtual ~SharedRef() noexcept(false)
			{
				if (m_p)
				{
					throw new std::logic_error("An object still exists when its owning reference has been destroyed.");
				}
				m_p = nullptr;
			}

			bool IsNull(void) const
			{
				return m_p == nullptr;
			}

			unsigned long AddRef(void) const
			{
				Lock();
				unsigned long ulResult = ++m_RefCount;
				Unlock();
				return ulResult;
			}

			unsigned long Release(void)
			{
				Lock();
				unsigned long ulResult = m_RefCount > 0 ? --m_RefCount : 0;
				Unlock();

				if (ulResult == 0)
				{
					InternalRelease();
				}
				return ulResult;
			}

			//When the reference count hits zero
			void InternalRelease(void)
			{
				Lock();
				internal_del_ref<R>(m_p);
				m_p = nullptr;//drop our pointer to it
				Unlock();
				//Self delete this shared reference. This must be safe as no non shared references exist when the reference count is zero
				source_of< Ref< R > >::type::Free(reinterpret_cast<byte*>(const_cast<SharedRef< R >*>(this)), sizeof(SharedRef< R >));
			}

			//Never call this unless you know the real object has gone for good.
			void Reset(void)
			{
				m_p = nullptr;
			}

			bool LockIsReal() const
			{
				return LockIsRealInternal< typename sync_of<R>::type >::type::value;
			}

			void Lock() const
			{
				m_Section.Acquire();
			}

			void Unlock() const
			{
				m_Section.Release();
			}

			bool IsLocked() const
			{
				return m_Section.IsLocked();
			}

			template< class TDerived >
			bool Configure()
			{
				bool result = false;
				if (m_RefCount == 1)
				{
					instancer_of<R>::type::template Release<R>(m_p, 1);
					m_p = nullptr;
					m_p = allocator_of<TDerived>::type::Allocate(1);
					if (m_p != nullptr)
					{
						int** ppBack = ((int**)m_p) - 1;
						*ppBack = (int*)(this);
						result = true;
					}
				}
				return result;
			}

			template< class TDerived, typename... _p >
			bool Configure( _p&& ... p1)
			{
				bool result = false;
				if (m_RefCount == 1)
				{
					instancer_of<R>::type::template Release<R>(m_p, 1);
					m_p = nullptr;
					m_p = allocator_of<TDerived>::type::Allocate(1, std::forward<_p>(p1)...);
					if (m_p != nullptr)
					{
						int** ppBack = ((int**)m_p) - 1;
						*ppBack = (int*)(this);
						result = true;
					}
				}
				return result;
			}

		private:

			R* ptr(void) const
			{
				return m_p;
			}

			void Attach(R* pt) const
			{
				if (m_p == nullptr)
				{
					m_p = pt;
					m_RefCount = 0;
					int** ppBack = ((int**)m_p) - 1;
					*ppBack = (int*)(this);
				}
			}

		protected:

			mutable R* m_p;
			mutable std::atomic< unsigned long > m_RefCount;
			mutable typename sync_of< R >::type m_Section;
		};

	}//detail
}//qor

#endif//QOR_PP_H_SHAREDREF
