// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

/*
	Copyright (C) 2017 by Sergey A Kryukov: derived work
	http://www.SAKryukov.org
	http://www.codeproject.com/Members/SAKryukov

	Based on original work by Sergey Ryazanov:
	"The Impossibly Fast C++ Delegates", 18 Jul 2005
	https://www.codeproject.com/articles/11015/the-impossibly-fast-c-delegates

	MIT license:
	http://en.wikipedia.org/wiki/MIT_License

	Original publication: https://www.codeproject.com/Articles/1170503/The-Impossibly-Fast-Cplusplus-Delegates-Fixed

*/

#ifndef QOR_PP_H_MULTICASTDELEGATE
#define QOR_PP_H_MULTICASTDELEGATE

#include <list>
#include <functional>
#include "delegate.h"
#include "src/qor/memory/memory.h"

namespace qor {

	template<typename ret_t, typename ...params_t>
	class MulticastDelegate<ret_t(params_t...)> final : private detail::DelegateHolder<ret_t(params_t...)> 
	{
	public:

		MulticastDelegate() = default;

		virtual ~MulticastDelegate()
		{
			for (auto& element : invocationList)
			{
                allocator_of<decltype(element)>::type::Free(element);
			}
			invocationList.clear();
		}
		
		bool IsNull() const 
		{ 
			return invocationList.size() < 1; 
		}

		bool operator ==(void* ptr) const 
		{
			return (ptr == nullptr) && this->IsNull();
		}
		
		bool operator !=(void* ptr) const 
		{
			return (ptr != nullptr) || (!this->IsNull());
		}

		size_t size() const 
		{ 
			return invocationList.size(); 
		}

		MulticastDelegate& operator =(const MulticastDelegate&) = delete;
		MulticastDelegate(const MulticastDelegate&) = delete;

		bool operator ==(const MulticastDelegate& another) const 
		{
			if (invocationList.size() != another.invocationList.size())
			{
				return false;
			}
			auto anotherIt = another.invocationList.begin();			
			for (auto it = invocationList.begin(); it != invocationList.end(); ++it)
			{
				if (**it != **anotherIt) return false;
			}
			return true;
		}

		bool operator !=(const MulticastDelegate& another) const 
		{ 
			return !(*this == another); 
		}

		bool operator ==(const Delegate<ret_t(params_t...)>& another) const 
		{
			if (IsNull() && another.IsNull())
			{
				return true;
			}

			if (another.IsNull() || (size() != 1))
			{
				return false;
			}

			return (another.invocation == **invocationList.begin());
		}

		bool operator !=(const Delegate<ret_t(params_t...)>& another) const 
		{ 
			return !(*this == another); 
		}

		MulticastDelegate& operator +=(const MulticastDelegate& another) 
		{
			for (auto& item : another.invocationList) // clone, not copy; flattens hierarchy:
			{
				this->invocationList.push_back(
                    allocator_of<typename detail::DelegateHolder<ret_t(params_t...)>::InvocationElement>::type::Allocate((size_t)1, item->object, item->stub));
			}
			return *this;
		}
	
		template <typename lambda_t> // template instantiation is not needed, will be deduced/inferred:
		MulticastDelegate& operator +=(const lambda_t & lambda) 
		{
			Delegate<ret_t(params_t...)> d = Delegate<ret_t(params_t...)>::template Create<lambda_t>(lambda);
			return *this += d;
		}

		MulticastDelegate& operator +=(const Delegate<ret_t(params_t...)>& another) 
		{
			if (!another.IsNull())
			{
				this->invocationList.push_back(
                    allocator_of<typename detail::DelegateHolder<ret_t(params_t...)>::InvocationElement>::type::template Allocate<typename detail::DelegateHolder<ret_t(params_t...)>::InvocationElement>((size_t)1, another.m_Invocation.object, another.m_Invocation.stub)
				);
			}
			return *this;
		}

		// will work even if ret_t is void, return values are ignored:
		// (for handling return values, see operator(..., handler))
		void operator()(params_t... arg) const 
		{
			for (auto& item : invocationList)
			{
				(*(item->stub))(item->object, arg...);
			}
		}

		template<typename handler_t>
		void operator()(params_t... arg, handler_t handler) const 
		{
			size_t index = 0;
			for (auto& item : invocationList) 
			{
				ret_t value = (*(item->stub))(item->object, arg...);
				handler(index, &value);
				++index;
			}
		}

		void operator()(params_t... arg, Delegate<void(size_t, ret_t*)> handler) const 
		{
			operator() < decltype(handler) > (arg..., handler);
		}

		void operator()(params_t... arg, std::function<void(size_t, ret_t*)> handler) const 
		{
			operator() < decltype(handler) > (arg..., handler);
		}

	private:

		std::list<typename detail::DelegateHolder<ret_t(params_t...)>::InvocationElement *> invocationList;

	};

}//qor

#endif//QOR_PP_H_MULTICASTDELEGATE
