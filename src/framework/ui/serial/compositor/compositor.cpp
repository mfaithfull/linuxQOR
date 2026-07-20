// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/essentials/objectcontext/anyobject.h"
#include "compositor.h"

namespace qor { namespace ui { namespace sercon {

    Compositor::Compositor() = default;

    Compositor::~Compositor() = default;

    void Compositor::Compose(ref_of<ConsoleViewModel>::type model, ref_of<ConsoleView>::type view)
    {
        for(auto widget: model()())
        {
            Compose(widget->GetPosition(), widget->GetSize(), widget->View(), view);
        }
    }

    void Compositor::Compose(Position position, Size size, ref_of<ConsoleView>::type viewSource, ref_of<ConsoleView>::type viewTarget)
    {
        viewTarget->GetBuffer()->CopyFrom(viewSource->GetBuffer(), position, size);
    }

}}}//qor::ui::sercon
