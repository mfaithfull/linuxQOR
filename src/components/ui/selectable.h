
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_UI_SELECTABLE
#define QOR_PP_H_COMPONENTS_UI_SELECTABLE

namespace qor{ namespace ui{
    
    class ISelectable
    {
        ISelectable() = default;
        virtual ~ISelectable() = default;
        
        void Select()
        {
            m_selected = true;
            OnSelected();                
        }

        virtual void OnSelected() {}

        void Deselect()
        {
            m_selected = false;
            OnDeselected();
        }

        virtual void OnDeselected() {}

    private:
        bool m_selected{false};
    };

}}//qor::ui
#endif//QOR_PP_H_COMPONENTS_UI_SELECTABLE
