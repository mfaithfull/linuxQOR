// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_LAYOUT_ALIGNRIGHT
#define QOR_PP_H_FRAMEWORK_UI_LAYOUT_ALIGNRIGHT

#include "arranger.h"
#include "../common/contained.h"

namespace qor{ namespace ui{ namespace layout { 

    class qor_pp_module_interface(QOR_UILAYOUT) AlignRight : public Arranger
    {
    public:
        
        AlignRight(ref_of<Contained>::type contained);
        virtual ~AlignRight();
    };

    class qor_pp_module_interface(QOR_UILAYOUT) AlignLeft : public Arranger
    {
    public:
        
        AlignLeft(ref_of<Contained>::type contained);
        virtual ~AlignLeft();
    };

    class qor_pp_module_interface(QOR_UILAYOUT) AlignTop : public Arranger
    {
    public:
        
        AlignTop(ref_of<Contained>::type contained);
        virtual ~AlignTop();
    };

    class qor_pp_module_interface(QOR_UILAYOUT) AlignBottom : public Arranger
    {
    public:
        
        AlignBottom(ref_of<Contained>::type contained);
        virtual ~AlignBottom();
    };

    class qor_pp_module_interface(QOR_UILAYOUT) HCenter : public Arranger
    {
    public:
        
        HCenter(ref_of<Contained>::type contained);
        virtual ~HCenter();
    };

    class qor_pp_module_interface(QOR_UILAYOUT) VCenter : public Arranger
    {
    public:
        
        VCenter(ref_of<Contained>::type contained);
        virtual ~VCenter();
    };

    class qor_pp_module_interface(QOR_UILAYOUT) Center : public Arranger
    {
    public:
        
        Center(ref_of<Contained>::type contained);
        virtual ~Center();
    };

}}}//qor::ui:layout

#endif//QOR_PP_H_FRAMEWORK_UI_LAYOUT_ALIGNRIGHT
