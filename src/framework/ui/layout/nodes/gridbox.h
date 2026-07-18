// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_GRIDBOX
#define QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_GRIDBOX

#include<vector>

#include "functional/layout.h"
#include "../node.h"
#include "detail/boxhelper.h"

namespace qor{ namespace ui{ namespace layout { 

    // Accumulate the values of a list U[n] into v[n]. So that:
    // V[0] = 0;
    // V[n+1] = v[n] + U[n]
    // return the sum of U[n].
    int Integrate(std::vector<int>& elements) 
    {
        int accu = 0;
        for (auto& i : elements) 
        {
            const int old_accu = accu;
            accu += i;
            i = old_accu;
        }
        return accu;
    }

    class qor_pp_module_interface(QOR_DESKTOPUI) GridBox : public Node 
    {
    public:
        explicit GridBox(std::vector<Elements> lines);
        void ComputeRequirement() override;
        void SetBox(Box box) override;

        int x_size = 0;
        int y_size = 0;
        std::vector<Elements> lines_;
    };

    //A container displaying a grid of elements.
    //lines A list of lines, each line being a list of elements.
    /// auto cell = [](const char* t) { return text(t) | border; };
    /// auto document = gridbox({
    ///   {cell("north-west") , cell("north")  , cell("north-east")} ,
    ///   {cell("west")       , cell("center") , cell("east")}       ,
    ///   {cell("south-west") , cell("south")  , cell("south-east")} ,
    /// });
    /// ╭──────────╮╭──────╮╭──────────╮
    /// │north-west││north ││north-east│
    /// ╰──────────╯╰──────╯╰──────────╯
    /// ╭──────────╮╭──────╮╭──────────╮
    /// │west      ││center││east      │
    /// ╰──────────╯╰──────╯╰──────────╯
    /// ╭──────────╮╭──────╮╭──────────╮
    /// │south-west││south ││south-east│
    /// ╰──────────╯╰──────╯╰──────────╯    

}}}//qor::ui::layout

#endif//QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_GRIDBOX

