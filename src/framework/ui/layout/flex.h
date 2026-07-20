
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_UI_LAYOUT_FLEX
#define QOR_PP_H_UI_LAYOUT_FLEX

#include <type_traits>
#include "leaf.h"
#include "layoutadjuster.h"

namespace qor{ namespace ui { namespace layout {

    class Spacer : public Leaf
    {
    public:

        Spacer() = default;
        virtual ~Spacer() = default;

        virtual void ComputeRequirement() override
        {
            IArrangeable::m_requirement.flex_grow_x = 1;
			IArrangeable::m_requirement.flex_grow_y = 1;
			IArrangeable::m_requirement.flex_shrink_x = 1;
			IArrangeable::m_requirement.flex_shrink_y = 1;
        }
    };

    template< typename T > requires std::is_base_of_v<IArrangeable, T>
    class Flex : public Adjuster<T>
    {
    public:

        Flex() = default;
        virtual ~Flex() = default;

        virtual void ComputeRequirement() override 
        {
            T::ComputeRequirment();
            IArrangeable::m_requirement.flex_grow_x = 1;
			IArrangeable::m_requirement.flex_grow_y = 1;
			IArrangeable::m_requirement.flex_shrink_x = 1;
			IArrangeable::m_requirement.flex_shrink_y = 1;
        }
    };

    template< typename T > requires std::is_base_of_v<IArrangeable, T>
    class XFlex : public Adjuster<T>
    {
    public:

        XFlex() = default;
        virtual ~XFlex() = default;

        virtual void ComputeRequirement() override 
        {
            T::ComputeRequirment();
            IArrangeable::m_requirement.flex_grow_x = 1;
			IArrangeable::m_requirement.flex_shrink_x = 1;
        }
    };

    template< typename T > requires std::is_base_of_v<IArrangeable, T>
    class YFlex : public Adjuster<T>
    {
    public:

        YFlex() = default;
        virtual ~YFlex() = default;

        virtual void ComputeRequirement() override 
        {
            T::ComputeRequirment();
            IArrangeable::m_requirement.flex_grow_y = 1;
			IArrangeable::m_requirement.flex_shrink_y = 1;
        }
    };

    template< typename T > requires std::is_base_of_v<IArrangeable, T>
    class Grow : public Adjuster<T>
    {
    public:

        Grow() = default;
        virtual ~Grow() = default;

        virtual void ComputeRequirement() override 
        {
            T::ComputeRequirment();
            IArrangeable::m_requirement.flex_grow_x = 1;
			IArrangeable::m_requirement.flex_grow_y = 1;
        }
    };

    template< typename T > requires std::is_base_of_v<IArrangeable, T>
    class XGrow : public Adjuster<T>
    {
    public:

        XGrow() = default;
        virtual ~XGrow() = default;

        virtual void ComputeRequirement() override 
        {
            T::ComputeRequirment();
            IArrangeable::m_requirement.flex_grow_x = 1;			
        }
    };

    template< typename T > requires std::is_base_of_v<IArrangeable, T>
    class YGrow : public Adjuster<T>
    {
    public:

        YGrow() = default;
        virtual ~YGrow() = default;

        virtual void ComputeRequirement() override 
        {
            T::ComputeRequirment();
            IArrangeable::m_requirement.flex_grow_y = 1;
        }
    };

    template< typename T > requires std::is_base_of_v<IArrangeable, T>
    class Shrink : public Adjuster<T>
    {
    public:

        Shrink() = default;
        virtual ~Shrink() = default;

        virtual void ComputeRequirement() override 
        {
            T::ComputeRequirment();
            IArrangeable::m_requirement.flex_shrink_x = 1;
			IArrangeable::m_requirement.flex_shrink_y = 1;
        }
    };

    template< typename T > requires std::is_base_of_v<IArrangeable, T>
    class XShrink : public Adjuster<T>
    {
    public:

        XShrink() = default;
        virtual ~XShrink() = default;

        virtual void ComputeRequirement() override 
        {
            T::ComputeRequirment();
            IArrangeable::m_requirement.flex_shrink_x = 1;			
        }
    };

    template< typename T > requires std::is_base_of_v<IArrangeable, T>
    class YShrink : public Adjuster<T>
    {
    public:

        YShrink() = default;
        virtual ~YShrink() = default;

        virtual void ComputeRequirement() override 
        {
            T::ComputeRequirment();
            IArrangeable::m_requirement.flex_shrink_y = 1;
        }
    };

    template< typename T > requires std::is_base_of_v<IArrangeable, T>
    class NotFlex : public Adjuster<T>
    {
    public:

        NotFlex() = default;
        virtual ~NotFlex() = default;

        virtual void ComputeRequirement() override 
        {
            T::ComputeRequirment();
			IArrangeable::m_requirement.flex_grow_x = 0;
			IArrangeable::m_requirement.flex_grow_y = 0;
			IArrangeable::m_requirement.flex_shrink_x = 0;
			IArrangeable::m_requirement.flex_shrink_y = 0;
        }
    };

}}}//qor::ui::layout

#endif//QOR_PP_H_UI_LAYOUT_FLEX

