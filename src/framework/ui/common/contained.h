
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_UI_CONTAINED
#define QOR_PP_H_UI_CONTAINED

#include <memory>
#include <vector>
#include <cassert>

#include "src/qor/memory/reference/newref.h"
#include "uiitem.h"

namespace qor{ namespace ui {

    class Compound;

    class qor_pp_module_interface(QOR_UICOMMON) Contained : public virtual Item
    {
    public:

        Contained();
        virtual ~Contained();

        constexpr bool Is(const Contained*) const;
        Compound* Parent() const;
        void SetParent(Compound*);
        int Index() const;
        void Detach();

    private:

        Compound* m_parent{nullptr};
    };

}}//qor::ui

#endif//QOR_PP_H_UI_COMPOUND

