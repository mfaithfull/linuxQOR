
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_FOCUSABLE
#define QOR_PP_H_FRAMEWORK_UI_FOCUSABLE

#include "uiitem.h"

namespace qor{ namespace ui{
    
    class IFocusable : public virtual Item
    {
        IFocusable() = default;
        virtual ~IFocusable() = default;
        
        void Focus()
        {
            m_focused = true;
            OnFocus();                
        }

        virtual void OnFocus() {}

        void Unfocus()
        {
            m_focused = false;
            OnUnfocus();
        }

        virtual void OnUnfocus() {}

    private:
        bool m_focused{false};
    };

}}//qor::ui
#endif//QOR_PP_H_FRAMEWORK_UI_FOCUSABLE
