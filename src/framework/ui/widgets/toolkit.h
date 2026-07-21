// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_COMMON_TOOLKIT
#define QOR_PP_H_FRAMEWORK_UI_COMMON_TOOLKIT

#include "src/qor/memory/reference/newref.h"

#include "../common/uiitem.h"

//Base Toolkit. Factory for widgets and parts

namespace qor { namespace ui { 

    class Toolkit
    {
    public:

        Toolkit() = default;
        ~Toolkit() = default;

        /*
        Create an appropriate top level container for the UI, Usually a Window
        Size and Position will be externally imposed and may change depending on the UI
        The Scene will decide if you can have more than 1. The toolkit should be fairly ignorant factory
        It is allowed to know about the Renderer for things like Style options so that it can include
        parts conditionally
        It will also need to know about the widgets it's creating. Mapping names to types
        This should basically work like the Windows GUI Factory with a bunch of pre-registered types
        for the widgets in the toolkit. For the GDI Toolkit these would literally be registered Window 
        classes.
        */
        virtual ref_of<Item>::type CreateTopLevelContainer()
        {
            return ref_of<Item>::type();
        }
    };

}}//qor::ui

#endif//QOR_PP_H_FRAMEWORK_UI_COMMON_TOOLKIT
