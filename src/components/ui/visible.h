
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_UI_SELECTABLE
#define QOR_PP_H_COMPONENTS_UI_SELECTABLE

namespace qor{ namespace ui{
    
    class IVisible
    {
        IVisible() = default;
        virtual ~IVisible() = default;
        
        void Show()
        {
            m_visible = true;
            OnShow();                
        }

        virtual void OnShow() {}

        void Hide()
        {
            m_visible = false;
            OnHide();
        }

        virtual void OnHide() {}

        bool IsVisible()
        {
            return m_visible;
        }
        
    private:
        bool m_visible{false};
    };

}}//qor::ui
#endif//QOR_PP_H_COMPONENTS_UI_SELECTABLE
