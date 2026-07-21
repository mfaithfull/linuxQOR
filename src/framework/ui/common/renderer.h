// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_COMMON_RENDERER
#define QOR_PP_H_FRAMEWORK_UI_COMMON_RENDERER

#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/reference/newref.h"
//#include "scene.h"

//Base renderer
namespace qor { namespace ui { 

    class Scene;

    class Renderer
    {
    public:

        Renderer() = default;
        virtual ~Renderer() = default;

        void Frame(Scene* scene)
        {
            //Work out how much space there is
            //Layout the scene in the space
            //Render it
            //Handle interaction and events/sizing, Ctrl+C, signals etc
        };
    };

}}//qor::ui

#endif//QOR_PP_H_FRAMEWORK_UI_COMMON_RENDERER
