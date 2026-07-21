
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_HOVERABLE
#define QOR_PP_H_FRAMEWORK_UI_HOVERABLE

#include "uiitem.h"

namespace qor{ namespace ui{
    
    class IHoverable : public virtual Item
    {
    public:
        IHoverable() = default;
        virtual ~IHoverable() = default;
        
        void Enter()
        {
            m_hovered = true;
            OnHover();                
        }

        virtual void OnHover() {}

        void Leave()
        {
            m_hovered = false;
            OnEndHover();
        }

        virtual void OnEndHover() {}

        bool IsHovered() const
        {
            return m_hovered;
        }

    private:
        bool m_hovered{false};
    };

}}//qor::ui

#endif//QOR_PP_H_FRAMEWORK_UI_HOVERABLE
