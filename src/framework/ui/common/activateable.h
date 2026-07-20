
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_ACTIVATABLE
#define QOR_PP_H_FRAMEWORK_UI_ACTIVATABLE

#include "uiitem.h"

namespace qor{ namespace ui{
    
    class IActivatable : public virtual Item
    {
    public:
        IActivatable() = default;
        virtual ~IActivatable() = default;
        
        void Activate()
        {
            if(!m_activation)
            {
                m_activation = true;
                //TODO: bubble activation either up or down or maybe both
                OnActivate();                
            }
        }

        virtual void OnActivate(){ }

        void Deactivate()
        {
            if(m_activation)
            {
                m_activation = false;
                //TODO: bubble deactivation either up or down or maybe both
                OnDeactivate();
            }
        }

        virtual void OnDeactivate() {}

        bool IsActivated() const
        {
            return m_activation;
        }

    private:
        bool m_activation{false};
    };

}}//qor::ui

#endif//QOR_PP_H_FRAMEWORK_UI_ACTIVATABLE
