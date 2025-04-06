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

#ifndef QOR_PP_H_REF
#define QOR_PP_H_REF

#include <atomic>
#include "src/qor/sync/sync.h"
#include "src/qor/instance/instance.h"
#include "src/qor/memory/memory.h"

namespace qor{

	//A counted reference class for managed objects
	template< class T > class Ref;
	template< typename T> void internal_del_ref(T* p);
		
	namespace detail
	{
        template< typename R, bool bValue >
        struct AllocateOnlyConcreteTypesFunctor
        {				
            template< typename... _p >
            static R* Allocate(size_t count, _p&&... pq)
            {
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
			SharedRef(size_t count, _p&&... p1) : m_p(nullptr), m_ulRefCount(0), m_Section()
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

            R* AllocateConcreteType(size_t count)
            {
                return allocator_of<R>::type::template Allocate<R>(count);
            }
    

			//The same as above but for default constructed target types
			SharedRef(size_t count) : m_p(nullptr), m_ulRefCount(0), m_Section()
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
			
			SharedRef() : m_p(nullptr), m_ulRefCount(0), m_Section()
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
					throw new std::logic_error("An object still exists when its owning reference is destroyed.");
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
				unsigned long ulResult = ++m_ulRefCount;
				Unlock();
				return ulResult;
			}
			
			unsigned long Release(void)
			{
				Lock();
				unsigned long ulResult = m_ulRefCount > 0 ? --m_ulRefCount : 0;
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
				source_of< R >::type::Free(reinterpret_cast<byte*>(const_cast<SharedRef< R >*>(this)), sizeof(SharedRef< R >));
			}

			//Never call this unless you know the real object has gone for good.
			void Reset(void)
			{
				m_p = nullptr;				
			}
			
			void Lock() const
			{
				m_Section.Acquire();
			}
			
			void Unlock() const
			{
				m_Section.Release();
			}

			template< class TDerived >
			bool Configure()
			{
				bool bResult = false;
				if (m_ulRefCount == 1)
				{
					instancer_of<R>::type::template Release<R>(m_p, 1);
					m_p = nullptr;
					m_p = allocator_of<TDerived>::type::Allocate(1);
					if (m_p != nullptr)
					{
						int** ppBack = ((int**)m_p) - 1;
						*ppBack = (int*)(this);
						bResult = true;
					}
				}
				return bResult;
			}
			
			template< class TDerived, typename... _p >
			bool Configure( _p&& ... p1)
			{
				bool bResult = false;
				if (m_ulRefCount == 1)
				{
					instancer_of<R>::type::template Release<R>(m_p, 1);
					m_p = nullptr;
					m_p = allocator_of<TDerived>::type::Allocate(1, std::forward<_p>(p1)...);
					if (m_p != nullptr)
					{
						int** ppBack = ((int**)m_p) - 1;
						*ppBack = (int*)(this);
						bResult = true;
					}
				}
				return bResult;
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
					m_ulRefCount = 0;
					int** ppBack = ((int**)m_p) - 1;
					*ppBack = (int*)(this);
				}
			}

		protected:

			mutable R* m_p;
			mutable std::atomic< unsigned long > m_ulRefCount;
			mutable typename sync_of< R >::type m_Section;
		};

	}//detail
	
	//The outer public reference class is an augmented pointer to the inner shared reference
	template< class T > 
	class Ref final
	{
	public:

		typedef detail::SharedRef<T> _tInternalRef;

		constexpr Ref() : m_p(nullptr) {}

		Ref(const detail::SharedRef<T>* pt) : m_p(pt)
		{
			if (m_p != nullptr)
			{
				m_p->AddRef();
			}
		}

		Ref(const Ref< T >& Src) : m_p(Src.m_p)
		{
			if (m_p != nullptr)
			{
				m_p->AddRef();
			}
		}

		Ref(Ref< T >&& Src) noexcept
		{
			m_p = Src.m_p;
			Src.m_p = nullptr;
		}

		Ref& operator = (const Ref< T >& Src)
		{
			if (this != &Src)
			{
				Dispose();
				m_p = Src.m_p;
				if (m_p != nullptr)
				{
					m_p->AddRef();
				}
			}
			return *this;
		}

		Ref& operator = (Ref< T >&& Src) noexcept
		{
			if (this != &Src)
			{
				Dispose();
				m_p = Src.m_p;
				if (m_p != nullptr)
				{
					m_p->AddRef();
				}
			}
			return *this;
		}

		//Two references are equal if they refer to the same object
		bool operator == (const Ref< T >& Cmp) const
		{
			return (m_p == Cmp.m_p);
		}

		//The overloaded operators double dereference through the inner pointer to provide access to the referenced object
		operator T* (void) const
		{
			return m_p ? m_p->ptr() : nullptr;
		}

		T& operator() (void) const
		{
			if (m_p == nullptr)
			{
				throw std::logic_error("Null reference exception: A reference must refer to an object in order to be used.");
			}
			return *(m_p->ptr());
		}

		T* operator -> () const
		{
			if (m_p == nullptr)
			{
				throw std::logic_error("Null reference exception: A reference must refer to an object in order to be used.");
			}
			return m_p->ptr();
		}

		~Ref()
		{
			Dispose();
		}

		void Dispose(void)
		{
			if (m_p)
			{
				(const_cast<detail::SharedRef<T>*>(m_p))->Release();
			}
			m_p = nullptr;
		}

		Ref<T>& DeOwn()
		{
			if (m_p != nullptr)
			{
				(const_cast<detail::SharedRef<T>*>(m_p))->Release();
			}
			m_p = nullptr;
			return *this;
		}

		Ref<T>& Reset()
		{
			if (m_p != nullptr)
			{
				(const_cast<detail::SharedRef<T>*>(m_p))->Reset();
			}
			m_p = nullptr;
			return *this;
		}

		void Attach(T* pt)
		{
			if( m_p->ptr() == nullptr)
			{
				m_p->Attach(pt);
			}
		}

		template< class TDerived >
		TDerived* As(void)
		{
			return m_p ? dynamic_cast< TDerived* >(m_p->ptr()) : nullptr;
		}

		template< class TDerived >
		Ref< TDerived > AsRef(void)
		{
			if (m_p && ( dynamic_cast<TDerived*>(m_p->ptr()) != nullptr ))
			{
				return Ref< TDerived >(reinterpret_cast<const detail::SharedRef<TDerived>*>(m_p));
			}
			return Ref< TDerived >();
		}

		bool IsNull(void) const
		{
			return m_p == nullptr || (m_p->IsNull());
		}

		bool IsNotNull(void) const
		{
			return m_p != nullptr && (!m_p->IsNull());
		}

		bool IsOwner(void) const
		{
			return (m_p && m_p->m_ulRefCount == 1) ? true : false;
		}

		template< class TDerived >
		bool Configure()
		{
			return const_cast<detail::SharedRef<T>*>(m_p)->Configure();
		}

		template< class TDerived, typename... _p >
		bool Configure(_p&&... p1)
		{
			return const_cast<detail::SharedRef<T>*>(m_p)->Configure(std::forward<_p>(p1)...);
		}

		void Lock() const
		{
			if (m_p != nullptr)
			{
				m_p->Lock();
			}
		}

		void Unlock() const
		{
			if (m_p != nullptr)
			{
				m_p->Unlock();
			}
		}

	protected:

		//A pointer to the shared reference, behaves like a pointer to the object itself.
		const detail::SharedRef<T>* m_p;
	};

}//qor

#endif//QOR_PP_H_REF
