// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_INSTANCE_DEFAULT
#define QOR_PP_H_INSTANCE_DEFAULT

#include <stddef.h>

#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/factory/factory.h"

//The purpose of an instancer is to determine whether to give out an existing instance
//a new instance or perhaps a recycled instance or to refuse the request.

namespace qor{

    //The default instancer gives out unconstrained freshly constructed instance references from the per type factory
	class DefaultInstancer final
	{
	public:

		template< class T >
		static inline void Release(T* t, size_t count = 1)
		{
			factory_of<T>::type::Destruct(t, count);
		}

		template< class T >
		static inline auto Instance(size_t count = 1)
		{
			return factory_of<T>::type::Construct(count);
		}
		
		template< class T, typename... _p >
		static inline auto Instance(size_t count, _p&&... p1)
		{
			return factory_of<T>::type::Construct(count, p1...);
		}

	private:

		DefaultInstancer() = delete;
		~DefaultInstancer() = delete;
	};

}//qor

#include "src/qor/memory/factory/internalfactory.h"
#include "src/qor/memory/factory/externalfactory.h"

#endif//QOR_PP_H_INSTANCE_DEFAULT
