
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_UI_COMPOUND
#define QOR_PP_H_UI_COMPOUND

#include <memory>
#include <vector>
#include <cassert>

#include "src/qor/memory/reference/newref.h"

namespace qor{ namespace ui {

    class Compound
    {
    public:
        
        Compound* Parent() const
        {
            return m_parent;
        }

        ref_of<Compound>::type ChildAt(size_t i)
        {
            assert(i < ChildCount());
            return m_children[i];
        }

        size_t ChildCount() const
        {
            return m_children.size();
        }

        int Index() const
        {
            if (m_parent == nullptr) 
            {
                return -1;
            }
            int index = 0;
            for(ref_of<Compound>::type child : m_parent->m_children) 
            {
                if (child == this) 
                {
                    return index;
                }
                index++;
            }
            return -1;  // Not reached.
        }

        void Add(ref_of<Compound>::type child)
        {
            child->Detach();
            child->m_parent = this;
            m_children.push_back(child);
        }

        void Detach()
        {
            if (m_parent == nullptr) 
            {
                return;
            }
            auto it = std::find_if(std::begin(m_parent->m_children), std::end(m_parent->m_children), [this](const ref_of<Compound>::type& that) 
                {
                    return this == that;
                }
            );
            Compound* parent = m_parent;
            m_parent = nullptr;
            parent->m_children.erase(it);
        }

        void DetachAllChildren()
        {
            while (!m_children.empty()) 
            {
                m_children[0]->Detach();
            }
        }

        template< class T >
        void ApplyIf( void(*f)(typename ref_of<T>::type) )
        {
            for(ref_of<Compound>::type child : m_children) 
            {
                auto t = child.AsRef<T>();
                if(t.IsNotNull())
                {
                    f(t);
                }
            }
        }

    protected:

        std::vector< ref_of<Compound>::type > m_children;

    private:

        Compound* m_parent{nullptr};        
    };

}}//qor::ui

#endif//QOR_PP_H_UI_COMPOUND

