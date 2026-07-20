
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_RENDERABLE
#define QOR_PP_H_FRAMEWORK_UI_RENDERABLE

#include "../common/uiitem.h"
#include "../common/compound.h"
#include "renderer.h"

namespace qor{ namespace ui{
    
    class IRenderable : public virtual Item
    {
    public:
        IRenderable() = default;
        virtual ~IRenderable() = default;
        
        virtual void Render(Renderer* renderer)
        {
            auto compound = Item::As<Compound>();
            if(compound)
            {                
                compound->ApplyIf<IRenderable>([renderer](IRenderable* child)
                {
                    child->Render(renderer);
                });
            }
        }
    };

}}//qor::ui
#endif//QOR_PP_H_FRAMEWORK_UI_RENDERABLE
