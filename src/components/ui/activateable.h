
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_UI_ACTIVATABLE
#define QOR_PP_H_COMPONENTS_UI_ACTIVATABLE

namespace qor{ namespace ui{
    
    class IActivatable
    {
        IActivatable() = default;
        virtual ~IActivatable() = default;
        
        void Activate()
        {
            if(!m_activation)
            {
                m_activation = true;
                OnActivate();                
            }
        }

        virtual void OnActivate() {}

        void Deactivate()
        {
            if(m_activation)
            {
                m_activation = false;
                OnDeactivate();
            }
        }

        virtual void OnDeactivate() {}

    private:
        bool m_activation{false};
    };

}}//qor::ui
#endif//QOR_PP_H_COMPONENTS_UI_ACTIVATABLE
