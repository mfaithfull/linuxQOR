// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_REQUIREMENT
#define QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_REQUIREMENT

#include "box.h"

namespace qor{ namespace ui{ namespace layout { 

    class qor_pp_module_interface(QOR_DESKTOPUI) Node;

    //Requirement is a structure that defines the layout requirements for a Node
    //It specifies the minimum size required to fully draw the element

    struct qor_pp_module_interface(QOR_DESKTOPUI) Requirement 
    {
        // The required size to fully draw the element.
        int min_x = 0;
        int min_y = 0;

        // How much flexibility is given to the component.
        int flex_grow_x = 0;
        int flex_grow_y = 0;
        int flex_shrink_x = 0;
        int flex_shrink_y = 0;

        // Focus management to support the frame/focus/select element.
        /*
        struct Focused 
        {
            bool enabled = false;
            Box box;
            Node* node = nullptr;
            //Screen::Cursor::Shape cursor_shape = Screen::Cursor::Shape::Hidden;

            // Internal for interactions with components.
            bool component_active = false;

            // Return whether this requirement should be preferred over the other.
            bool Prefer(const Focused& other) const 
            {
                if (!other.enabled) 
                {
                    return false;
                }
                if (!enabled) 
                {
                    return true;
                }

                return other.component_active && !component_active;
            }
        };
        
        Focused focused;*/
    };

}}}//qor::ui:layout

#endif//QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_REQUIREMENT

