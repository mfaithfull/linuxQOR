// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_COMMON_SCENE
#define QOR_PP_H_FRAMEWORK_UI_COMMON_SCENE

#include "compound.h"
#include "renderable.h"

//Base Scene. The retained heirarchy of Items. 

namespace qor { namespace ui { 

    class Scene : public Compound, public virtual IRenderable
    {
    public:

        Scene() = default;
        ~Scene() = default;

    };

}}//qor::ui

#endif//QOR_PP_H_FRAMEWORK_UI_COMMON_SCENE
