// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OBSERVER_ABSTRACT
#define QOR_PP_H_OBSERVER_ABSTRACT

#include "detail/chainlink.h"

namespace qor{ 

    template< class Observed >
    class AbstractObserver : public detail::ChainLink
    {
    public:

        AbstractObserver() : ChainLink() {}

        AbstractObserver(const AbstractObserver& src) : ChainLink(src) {}
        virtual ~AbstractObserver() noexcept = default;
        
        AbstractObserver& operator = (const AbstractObserver& src)
        {
            ChainLink::operator= (src);
            return *this;
        }

        virtual void Update(Observed& target) = 0;
    };

}//qor

#endif//QOR_PP_H_OBSERVER_ABSTRACT
