
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_LAYOUT_SIZECONSTRAINER
#define QOR_PP_H_FRAMEWORK_UI_LAYOUT_SIZECONSTRAINER

#include <type_traits>
#include "src/qor/memory/reference/newref.h"
#include "arrangeable.h"

namespace qor{ namespace ui{ namespace layout {

    enum WidthOrHeight { WIDTH, HEIGHT };
    enum Constraint { LESS_THAN, EQUAL, GREATER_THAN };

    template< typename T > requires std::is_base_of_v<IArrangeable, T>
    class SizeConstrainer : public T    
    {
    public:

        SizeConstrainer(WidthOrHeight direction, Constraint constraint, int value) :
            m_direction(direction),
            m_constraint(constraint),
            m_value(value)
        { }
        
        virtual ~SizeConstrainer() = default;

        virtual void ComputeRequirement() override
        {
            T::ComputeRequirement();

            auto& value = m_direction == WIDTH ? T::m_requirement.min_x : T::m_requirement.min_y;

            switch (m_constraint) 
            {
            case LESS_THAN:
                value = std::min(value, m_value);
                break;
            case EQUAL:
                value = m_value;
                break;
            case GREATER_THAN:
                value = std::max(value, m_value);
                break;
            }
        }

        virtual void SetBox(Box box) override
        {
            if (m_direction == WIDTH) 
            {
                switch (m_constraint) 
                {
                    case LESS_THAN:
                    case EQUAL:
                    box.x_max = std::min(box.x_min + m_value + 1, box.x_max);
                    break;
                    case GREATER_THAN:
                    break;
                }
            } 
            else 
            {
                switch (m_constraint) 
                {
                    case LESS_THAN:
                    case EQUAL:
                    box.y_max = std::min(box.y_min + m_value + 1, box.y_max);
                    break;
                    case GREATER_THAN:
                    break;
                }
            }            

            T::SetBox(box);
        }

    private:

        WidthOrHeight m_direction;
        Constraint m_constraint;
        int m_value;
    };
}}}//qor::ui::layout

#endif//QOR_PP_H_FRAMEWORK_UI_LAYOUT_SIZECONSTRAINER