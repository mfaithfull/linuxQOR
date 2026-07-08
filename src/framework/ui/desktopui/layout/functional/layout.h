// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI_FUNCTIONAL_LAYOUT
#define QOR_PP_H_FRAMEWORK_DESKTOPUI_FUNCTIONAL_LAYOUT

#include <memory>
#include <vector>
#include <string>
#include <functional>

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/reference/newref.h"

#include "flexboxconfig.h"
#include "../box.h"

namespace qor{ namespace ui{ namespace layout { 

    enum WidthOrHeight { WIDTH, HEIGHT };
    enum Constraint { LESS_THAN, EQUAL, GREATER_THAN };

    class qor_pp_module_interface(QOR_DESKTOPUI) Node;

    using Element = ref_of<Node>::type;
    using Elements = std::vector<Element>;
    using Decorator = std::function<Element(Element)>;

    qor_pp_module_interface(QOR_DESKTOPUI) Element emptyElement();
    qor_pp_module_interface(QOR_DESKTOPUI) Element operator|(Element, Decorator);
    qor_pp_module_interface(QOR_DESKTOPUI) Element& operator|=(Element&, Decorator);
    qor_pp_module_interface(QOR_DESKTOPUI) Elements operator|(Elements, Decorator);
    qor_pp_module_interface(QOR_DESKTOPUI) Decorator operator|(Decorator, Decorator);
    qor_pp_module_interface(QOR_DESKTOPUI) Element nothing(Element element);
    qor_pp_module_interface(QOR_DESKTOPUI) Decorator operator|(Decorator a, Decorator b);
    qor_pp_module_interface(QOR_DESKTOPUI) Elements operator|(Elements elements, Decorator decorator);
    qor_pp_module_interface(QOR_DESKTOPUI) Element operator|(Element element, Decorator decorator);
    qor_pp_module_interface(QOR_DESKTOPUI) Element& operator|=(Element& e, Decorator d);
    qor_pp_module_interface(QOR_DESKTOPUI) Element hcenter(Element child);
    qor_pp_module_interface(QOR_DESKTOPUI) Element vcenter(Element child);
    qor_pp_module_interface(QOR_DESKTOPUI) Element center(Element child);
    qor_pp_module_interface(QOR_DESKTOPUI) Element align_right(Element child);
    //flexbox
    qor_pp_module_interface(QOR_DESKTOPUI) Element flexbox(Elements children, FlexboxConfig config);
    qor_pp_module_interface(QOR_DESKTOPUI) Element hflow(Elements children);
    qor_pp_module_interface(QOR_DESKTOPUI) Element vflow(Elements children);
    //flex
    qor_pp_module_interface(QOR_DESKTOPUI) Element filler();
    qor_pp_module_interface(QOR_DESKTOPUI) Element flex(Element child);
    qor_pp_module_interface(QOR_DESKTOPUI) Element xflex(Element child);
    qor_pp_module_interface(QOR_DESKTOPUI) Element yflex(Element child);
    qor_pp_module_interface(QOR_DESKTOPUI) Element flex_grow(Element child);
    qor_pp_module_interface(QOR_DESKTOPUI) Element xflex_grow(Element child);
    qor_pp_module_interface(QOR_DESKTOPUI) Element yflex_grow(Element child);
    qor_pp_module_interface(QOR_DESKTOPUI) Element flex_shrink(Element child);
    qor_pp_module_interface(QOR_DESKTOPUI) Element xflex_shrink(Element child);
    qor_pp_module_interface(QOR_DESKTOPUI) Element yflex_shrink(Element child);
    qor_pp_module_interface(QOR_DESKTOPUI) Element notflex(Element child);
    //dbox
    qor_pp_module_interface(QOR_DESKTOPUI) Element dbox(Elements children_);
    //gridbox
    qor_pp_module_interface(QOR_DESKTOPUI) Element gridbox(std::vector<Elements> lines);
    //hbox
    qor_pp_module_interface(QOR_DESKTOPUI) Element hbox(Elements children);
    //reflect
    qor_pp_module_interface(QOR_DESKTOPUI) Decorator reflect(Box& box);
    //size
    qor_pp_module_interface(QOR_DESKTOPUI) Decorator size(WidthOrHeight direction, Constraint constraint, int value);
    //vbox
    qor_pp_module_interface(QOR_DESKTOPUI) Element vbox(Elements children);
}}}//qor::ui::layout


#endif//QOR_PP_H_FRAMEWORK_DESKTOPUI_FUNCTIONAL_LAYOUT

