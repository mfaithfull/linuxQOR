// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OBSERVER_CHAINLINK
#define QOR_PP_H_OBSERVER_CHAINLINK

namespace qor{ namespace detail {

    //An entirely weak reference, dynamic allocation free, doubly linked list
    //NOTE: Do not use this across threads
    class ChainLink
    {
    public:

		ChainLink() noexcept{ }

		ChainLink(const ChainLink& src) noexcept
		{
			m_prev = src.m_prev;
			m_next = src.m_next;
		}

		virtual ~ChainLink() noexcept = default;

		ChainLink& operator = (const ChainLink& src) noexcept
		{
			if (&src != this)
			{
				m_prev = src.m_prev;
				m_next = src.m_next;
			}
			return *this;
		}

		ChainLink*& Prev(void)
		{
			return m_prev;
		}

		ChainLink*& Next(void)
		{
			return m_next;
		}

		static void AddBefore(ChainLink* &first, ChainLink* _new)
		{
			if (_new)
			{
				if (first != nullptr)
				{
					first->Prev() = _new;
					_new->Next() = first;
				}

				first = _new;
			}
		}

		static void Remove(ChainLink* &first, ChainLink* old)
		{
			if (first == old)
			{
				first = old->Next();
			}

			if (old)
			{
				if (old->Prev())
				{
					old->Prev()->Next() = old->Next();                    
				}

				if (old->Next())
				{
					old->Next()->Prev() = old->Prev();
                    
				}
				old->Next() = nullptr;
				old->Prev() = nullptr;
			}
		}

		template< class TDerived >
		static void Walk(ChainLink* first, void(*func)(TDerived*))
		{
			while (first != nullptr)
			{
				TDerived* derived = dynamic_cast<TDerived*>(first);
				if (derived)
				{
					(func)(derived);
				}
				first = first->Next();
			}
		}

    protected:

		ChainLink* m_prev{nullptr};
		ChainLink* m_next{nullptr};

    };
}}//qor::detail

#endif//QOR_PP_H_OBSERVER_CHAINLINK
