
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_UI_COMPOUND
#define QOR_PP_H_UI_COMPOUND

#include <memory>
#include <vector>
#include <cassert>

#include "src/qor/memory/reference/newref.h"
#include "uiitem.h"
#include "contained.h"

namespace qor{ namespace ui {

    class Compound : public virtual Item
    {
    public:

        ref_of<Contained>::type ChildAt(size_t i)
        {
            assert(i < ChildCount());
            return m_children[i];
        }

        size_t ChildCount() const
        {
            return m_children.size();
        }

        int IndexOf(const Contained* contained) const
        {
            int index = 0;
            for(ref_of<Contained>::type child : m_children) 
            {
                if (child == contained) 
                {
                    return index;
                }
                index++;
            }
            return -1;
        }

        void Add(ref_of<Contained>::type child)
        {
            if(child.IsNotNull())
            {
                child->Detach();
                child->SetParent(this);
                m_children.push_back(child);
            }
        }

        void Detach(Contained* contained)
        {
            auto it = std::find_if(std::begin(m_children), std::end(m_children), [contained](const ref_of<Contained>::type& that) 
                {
                    return contained == that;
                }
            );
            if(it != m_children.end())
            {
                m_children.erase(it);
            }
        }

        void DetachAllChildren()
        {
            while (!m_children.empty()) 
            {
                m_children[0]->Detach();
            }
        }

        template< class R, typename func >
        void ApplyIf( func&& apply )
        {
            for( typename ref_of<Contained>::type child : m_children) 
            {                
                R* t = child->As<R>();
                if(t)
                {
                    apply(t);
                }
            }
        }

    protected:

        std::vector< ref_of<Contained>::type > m_children;

    };

}}//qor::ui

#endif//QOR_PP_H_UI_COMPOUND

