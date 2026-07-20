// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_LAYOUT_BOX
#define QOR_PP_H_FRAMEWORK_UI_LAYOUT_BOX

namespace qor{ namespace ui { namespace layout {

    struct qor_pp_module_interface(QOR_UILAYOUT) Box
    {
    public:

        int x_min = 0;
        int x_max = 0;
        int y_min = 0;
        int y_max = 0;

        static auto Intersection(Box a, Box b) -> Box;
        static auto Union(Box a, Box b) -> Box;
        void Shift(int x, int y);
        bool Contain(int x, int y) const;
        bool IsEmpty() const;
        bool operator==(const Box& other) const;
        bool operator!=(const Box& other) const;
    };

}}}//qor::ui::layout

#endif//QOR_PP_H_FRAMEWORK_UI_LAYOUT_BOX

