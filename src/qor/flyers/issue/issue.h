// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ISSUE
#define QOR_PP_H_ISSUE

#include "when.h"
#include "where.h"
#include "what.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/reference/newref.h"

namespace qor{ 

    template< class WhatT >
    class Issue
    {
    public:

        Issue() = default;
        
        Issue(const Issue& src)
        {
            if(src.m_what.IsNotNull())
            {
                m_what = new_ref<WhatT>(src.m_what);
            }
            if(src.m_when.IsNotNull())
            {
                m_when = new_ref<When>(src.m_when);
            }
            if( src.m_where.IsNotNull() )
            {
                m_where = new_ref<Where>(src.m_where);
            }
        }

        virtual ~Issue() noexcept = default;

        Issue& operator = (const Issue& src)
        {
            if(&src != this)
            {
                if(src.m_what.IsNotNull())
                {
                    m_what = new_ref<WhatT>(src.m_what);
                }
                if(src.m_when.IsNotNull())
                {
                    m_when = new_ref<When>(src.m_when);
                }
                if( src.m_where.IsNotNull() )
                {
                    m_where = new_ref<Where>(src.m_where);
                }
            }
            return *this;
        }

        virtual const When& when(void) const
        {
            return *(m_when.template As<When>());
        }

        bool HasWhen() const
        {
            return m_when.IsNotNull();
        }

        virtual const Where& where(void) const
        {
            return *(m_where.template As<Where>());
        }

        bool HasWhere() const
        {
            return m_where.IsNotNull();
        }

        virtual const WhatT& what(void) const
        {
            return *(m_what.template As<WhatT>());
        }       

        bool HasWhat() const
        {
            return m_what.IsNotNull();
        }

        virtual void Handle(void) = 0;

        virtual void Resolve(bool bHandled) const
        {
            if(!bHandled)
            {
                Escalate();
            }
        }
        
        virtual void Catch()
        {
            Handle();
        }

        virtual void Escalate(void) const = 0;
        
        virtual void Ignore() const {}

    protected:

        ref_of<WhatT>::type m_what;
        ref_of<Where>::type m_where;
        ref_of<When>::type m_when;
    };

    template< class t_Issue, typename... _p >
    void issue(_p&&... p1)
    {
        new_ref<t_Issue>( std::forward<_p>(p1)... )->Handle();
    }

}//qor

#endif//QOR_PP_H_ISSUE
