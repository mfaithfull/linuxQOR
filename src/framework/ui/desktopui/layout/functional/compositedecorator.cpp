// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <utility>  // for move

#include "../nodes/vbox.h"
#include "../nodes/hbox.h"

namespace qor{ namespace ui{ namespace layout {

	qor_pp_module_interface(QOR_DESKTOPUI) Element hcenter(Element child)
	{
		return hbox(filler(), std::move(child), filler());
	}

	qor_pp_module_interface(QOR_DESKTOPUI) Element vcenter(Element child) 
	{
		return vbox(filler(), std::move(child), filler());
	}

	qor_pp_module_interface(QOR_DESKTOPUI) Element center(Element child)
	{
		return hcenter(vcenter(std::move(child)));
	}

	qor_pp_module_interface(QOR_DESKTOPUI) Element align_right(Element child) 
	{
		return hbox(filler(), std::move(child));
	}

}}}//qor::ui::layout
