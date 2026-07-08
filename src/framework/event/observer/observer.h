// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OBSERVER_OBSERVER
#define QOR_PP_H_OBSERVER_OBSERVER

#include "proxyobserver.h"

namespace qor{ 

    template< class Observed >
    class Observable
    {
    public:

        Observable( Observed& target ) : m_Observed( target ), m_first(nullptr) {}

        Observable( const Observable& src) : m_Observed( src.m_Observed )
        {
            *this = src;
        }

        Observable& operator = (const Observable& src )
        {
            if( &src != this )
            {
                m_first = src.m_first;
            }
            return *this;
        }

        ~Observable() noexcept = default;

        void Update()
        {
            AbstractObserver< Observed >* it = dynamic_cast< AbstractObserver< Observed >* >( m_first );
            while( nullptr != it )
            {
                it->Update( m_Observed );
                it = dynamic_cast< AbstractObserver< Observed >* >(it->Next());
            }
        }

        void AddObserver( detail::ChainLink* _new )
        {
            detail::ChainLink::AddBefore( m_first, _new );
        }

        void RemoveObserver( detail::ChainLink* old )
        {
            detail::ChainLink::Remove( m_first, old );
        }

    private:

        Observed& m_Observed;
        detail::ChainLink* m_first;
    };

}//qor

#endif//QOR_PP_H_OBSERVER_OBSERVER
