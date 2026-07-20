// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_LAYOUT_ALIGNRIGHT
#define QOR_PP_H_FRAMEWORK_UI_LAYOUT_ALIGNRIGHT

#include "arranger.h"
#include "../contained.h"

namespace qor{ namespace ui{ namespace layout { 

    class AlignRight : public Arranger
    {
    public:
        
        AlignRight(ref_of<Contained>::type contained);
        virtual ~AlignRight();
    
        virtual void ComputeRequirement() override;
        virtual void SetBox(Box box) override;

    };

    class AlignLeft : public Arranger
    {
    public:
        
        AlignLeft(ref_of<Contained>::type contained);
        virtual ~AlignLeft();
    
        virtual void ComputeRequirement() override;
        virtual void SetBox(Box box) override;
    };

    class AlignTop : public Arranger
    {
    public:
        
        AlignTop(ref_of<Contained>::type contained);
        virtual ~AlignTop();
    
        virtual void ComputeRequirement() override;
        virtual void SetBox(Box box) override;
    };

    class AlignBottom : public Arranger
    {
    public:
        
        AlignBottom(ref_of<Contained>::type contained);
        virtual ~AlignBottom();
    
        virtual void ComputeRequirement() override;
        virtual void SetBox(Box box) override;
    };

    class HCenter : public Arranger
    {
    public:
        
        HCenter(ref_of<Contained>::type contained);
        virtual ~HCenter();
    
        virtual void ComputeRequirement() override;
        virtual void SetBox(Box box) override;

    };

    class VCenter : public Arranger
    {
    public:
        
        VCenter(ref_of<Contained>::type contained);
        virtual ~VCenter();
    
        virtual void ComputeRequirement() override;
        virtual void SetBox(Box box) override;

    };

    class Center : public Arranger
    {
    public:
        
        Center(ref_of<Contained>::type contained);
        virtual ~Center();
    
        virtual void ComputeRequirement() override;
        virtual void SetBox(Box box) override;

    };

}}}//qor::ui:layout

#endif//QOR_PP_H_FRAMEWORK_UI_LAYOUT_ALIGNRIGHT
