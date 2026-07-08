// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OBSERVER_PROXY
#define QOR_PP_H_OBSERVER_PROXY

#include "abstractobserver.h"

namespace qor{ 

    template< class Observed, class Impl >
    class ProxyObserver : public AbstractObserver< Observed >
    {
    public:

        ProxyObserver(Impl& impl) : AbstractObserver< Observed >(), m_impl(impl) {}

        ProxyObserver(const ProxyObserver& src) : AbstractObserver< Observed >(src), m_impl(src.m_impl) {}
        virtual ~ProxyObserver() noexcept = default;

        ProxyObserver& operator = (const ProxyObserver& src)
        {
            AbstractObserver< Observed >::operator=(src);
            m_impl = src.m_impl;
            return *this;
        }

        virtual void Update(Observed& target)
        {
            m_impl.Update(target);
        }

    protected:

        Impl& m_impl;

    };

}//qor

#endif//QOR_PP_H_OBSERVER_PROXY
