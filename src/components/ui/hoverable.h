
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_UI_HOVERABLE
#define QOR_PP_H_COMPONENTS_UI_HOVERABLE

namespace qor{ namespace ui{
    
    class IHoverable
    {
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

    private:
        bool m_hovered{false};
    };

}}//qor::ui
#endif//QOR_PP_H_COMPONENTS_UI_HOVERABLE
