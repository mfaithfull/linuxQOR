// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/memory/reference/newref.h"

#include "align.h"
#include "hbox.h"
#include "vbox.h"
#include "flex.h"

namespace qor{ namespace ui{ namespace layout {

    AlignRight::AlignRight(ref_of<Contained>::type contained)
    {
        auto hbox = new_ref<HBox>();
        hbox->Add(new_ref<Spacer>());
        hbox->Add(contained);
        Add(hbox);
    }

    AlignRight::~AlignRight() = default;

    AlignLeft::AlignLeft(ref_of<Contained>::type contained)
    {
        auto hbox = new_ref<HBox>();
        hbox->Add(contained);
        hbox->Add(new_ref<Spacer>());
        Add(hbox);
    }

    AlignLeft::~AlignLeft() = default;


    AlignTop::AlignTop(ref_of<Contained>::type contained)
    {
        auto vbox = new_ref<VBox>();
        vbox->Add(contained);
        vbox->Add(new_ref<Spacer>());
        Add(vbox);
    }

    AlignTop::~AlignTop() = default;


    AlignBottom::AlignBottom(ref_of<Contained>::type contained)
    {
        auto vbox = new_ref<VBox>();
        vbox->Add(new_ref<Spacer>());
        vbox->Add(contained);
        Add(vbox);
    }

    AlignBottom::~AlignBottom() = default;



    HCenter::HCenter(ref_of<Contained>::type contained)
    {
        auto hbox = new_ref<HBox>();
        hbox->Add(new_ref<Spacer>());
        hbox->Add(contained);
        hbox->Add(new_ref<Spacer>());
        Add(hbox);
    }

    HCenter::~HCenter() = default;

    VCenter::VCenter(ref_of<Contained>::type contained)
    {
        auto vbox = new_ref<VBox>();
        vbox->Add(new_ref<Spacer>());
        vbox->Add(contained);
        vbox->Add(new_ref<Spacer>());
        Add(vbox);
    }

    VCenter::~VCenter() = default;

    Center::Center(ref_of<Contained>::type contained)
    {
        Add(new_ref<HCenter>(new_ref<VCenter>(contained)));
    }

    Center::~Center() = default;

}}}//qor::ui::layout
