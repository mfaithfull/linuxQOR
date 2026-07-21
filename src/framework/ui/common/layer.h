// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_COMMON_LAYER
#define QOR_PP_H_FRAMEWORK_UI_COMMON_LAYER

#include "compound.h"
#include "renderable.h"

namespace qor { namespace ui { 

    //Render if it's visible otherwise skip
    class Layer : public Compound, public virtual IRenderable
    {
    public:

        Layer() = default;
        ~Layer() = default;
        
    };

}}//qor::ui

#endif//QOR_PP_H_FRAMEWORK_UI_COMMON_LAYER
