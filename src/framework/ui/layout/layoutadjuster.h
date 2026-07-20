
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_LAYOUT_ADJUSTER
#define QOR_PP_H_FRAMEWORK_UI_LAYOUT_ADJUSTER

#include <type_traits>
#include "src/qor/memory/reference/newref.h"
#include "arrangeable.h"

namespace qor{ namespace ui{ namespace layout {

    template< typename T > requires std::is_base_of_v<IArrangeable, T>
    class Adjuster : public T    
    {
    public:

        Adjuster() = default;
        virtual ~Adjuster() = default;

        virtual void ComputeRequirement() override
        {
            T::ComputeRequirement();
        }

        virtual void SetBox(Box box) override
        {
            T::SetBox(box);
        }

    };
}}}//qor::ui::layout

#endif//QOR_PP_H_FRAMEWORK_UI_LAYOUT_ADJUSTER