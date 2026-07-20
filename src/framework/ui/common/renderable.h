
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_RENDERABLE
#define QOR_PP_H_FRAMEWORK_UI_RENDERABLE

#include "../uiitem.h"
#include "../compound.h"
#include "renderer.h"

namespace qor{ namespace ui{
    
    class IRenderable : public virtual Item
    {
        IRenderable() = default;
        virtual ~IRenderable() = default;
        
        virtual void Render(Renderer* renderer)
        {
            if(Item::Is<Compound>())
            {
                Compound* compound = dynamic_cast<Compound*>(this);
                compound->ApplyIf<IRenderable>([renderer](IRenderable* child)
                {
                    child->Render(renderer);
                });
            }
        }
    };

}}//qor::ui
#endif//QOR_PP_H_FRAMEWORK_UI_RENDERABLE
