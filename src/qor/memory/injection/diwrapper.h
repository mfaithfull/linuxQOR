// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_INJECTION_DIWRAPPER
#define QOR_PP_H_INJECTION_DIWRAPPER

#include "src/qor/memory/reference/reference.h"

#define qor_pp_did(INTERFACE, PARAM) DIWrapper<INTERFACE> PARAM = DIWrapper<INTERFACE>()
#define qor_pp_dii(INTERFACE, PARAM) DIWrapper<INTERFACE> PARAM

namespace qor{

	template<typename Interface>
	class DIWrapper final
	{
	public:

		template< typename... _p >
		DIWrapper(_p&&... p1)
		{
			m_instance = new_ref<Interface>(std::forward<_p&&>(p1)...);
		}

		DIWrapper(typename ref_of<Interface>::type p1)
		{
			m_instance = p1;
		}

		~DIWrapper() = default;

		operator typename ref_of<Interface>::type() const
		{
			return m_instance;
		}

		typename ref_of<Interface>::type operator() (void) const
		{
			return m_instance;
		}

	private:

		typename ref_of<Interface>::type m_instance;
	};

}//qor

#endif//QOR_PP_H_INJECTION_DIWRAPPER
