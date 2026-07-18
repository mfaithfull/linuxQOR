// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_DETAIL_FLEX
#define QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_DETAIL_FLEX

#include "src/configuration/configuration.h"

#include "../node.h"
#include "../nodes/vbox.h"
#include "../nodes/hbox.h"
#include "../nodes/detail/anyargs.h"
#include "../nodes/flexnode.h"

namespace qor{ namespace ui{ namespace layout {
    
	//An element that will take expand proportionally to the space left in a container.
	qor_pp_module_interface(QOR_DESKTOPUI) Element filler()
	{
		return new_ref<Flex>(function_flex);
	}

	//Make a child element to expand proportionally to the space left in a container.

	///   hbox({
	///     text("left") | border ,
	///     text("middle") | border | flex,
	///     text("right") | border,
	///   });
	///
	/// ┌────┐┌─────────────────────────────────────────────────────────┐┌─────┐
	/// │left││middle                                                   ││right│
	/// └────┘└─────────────────────────────────────────────────────────┘└─────┘

	qor_pp_module_interface(QOR_DESKTOPUI) Element flex(Element child)
	{
		return new_ref<Flex>(function_flex, std::move(child));
	}

	//Expand/Minimize if possible/needed on the X axis.
	qor_pp_module_interface(QOR_DESKTOPUI) Element xflex(Element child) 
	{
		return new_ref<Flex>(function_xflex, std::move(child));
	}

	//Expand/Minimize if possible/needed on the Y axis.
	qor_pp_module_interface(QOR_DESKTOPUI) Element yflex(Element child)
	{
		return new_ref<Flex>(function_yflex, std::move(child));
	}

	//Expand if possible.
	qor_pp_module_interface(QOR_DESKTOPUI) Element flex_grow(Element child)
	{
		return new_ref<Flex>(function_flex_grow, std::move(child));
	}

	//Expand if possible on the X axis.
	qor_pp_module_interface(QOR_DESKTOPUI) Element xflex_grow(Element child)
	{
		return new_ref<Flex>(function_xflex_grow, std::move(child));
	}

	//Expand if possible on the Y axis.
	qor_pp_module_interface(QOR_DESKTOPUI) Element yflex_grow(Element child)
	{
		return new_ref<Flex>(function_yflex_grow, std::move(child));
	}

	//Minimize.
	qor_pp_module_interface(QOR_DESKTOPUI) Element flex_shrink(Element child)
	{
		return new_ref<Flex>(function_flex_shrink, std::move(child));
	}

	//Minimize on the X axis.
	qor_pp_module_interface(QOR_DESKTOPUI) Element xflex_shrink(Element child) 
	{
		return new_ref<Flex>(function_xflex_shrink, std::move(child));
	}

	//Minimize on the Y axis.
	qor_pp_module_interface(QOR_DESKTOPUI) Element yflex_shrink(Element child)
	{
		return new_ref<Flex>(function_yflex_shrink, std::move(child));
	}

	//Make the element not flexible.
	qor_pp_module_interface(QOR_DESKTOPUI) Element notflex(Element child)
	{
		return new_ref<Flex>(function_not_flex, std::move(child));
	}

}}}//qor::ui::layout

#endif//QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_DETAIL_FLEX

