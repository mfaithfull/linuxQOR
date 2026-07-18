
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_UI_ITEM
#define QOR_PP_H_COMPONENTS_UI_ITEM

#include <string>
#include "visible.h"

namespace qor{ namespace ui{
    
    class Item : public IVisible
    {
        Item() = default;
        virtual ~Item() = default;
        
        std::string& Name()
        {
            return m_name;
        }

    private:

        std::string m_name;
    };

}}//qor::ui
#endif//QOR_PP_H_COMPONENTS_UI_ITEM
