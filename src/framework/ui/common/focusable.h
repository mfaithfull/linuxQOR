
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_FOCUSABLE
#define QOR_PP_H_FRAMEWORK_UI_FOCUSABLE

#include "uiitem.h"
#include "activateable.h"

namespace qor{ namespace ui{
    
    class IFocusable : public virtual Item
    {
    public:
        IFocusable() = default;
        virtual ~IFocusable() = default;
        
        void Focus()
        {
            if(Item::Is<IActivatable>())
            {
                Item::As<IActivatable>()->Activate();
            }
            m_focused = true;
            //TODO: bubble focus either up or down or maybe both
            OnFocus();                
        }

        virtual void OnFocus() {}

        void Unfocus()
        {
            m_focused = false;
            OnUnfocus();
        }

        virtual void OnUnfocus() {}

        bool IsFocused() const
        {
            return m_focused;
        }

    private:
        bool m_focused{false};
    };

}}//qor::ui
#endif//QOR_PP_H_FRAMEWORK_UI_FOCUSABLE
