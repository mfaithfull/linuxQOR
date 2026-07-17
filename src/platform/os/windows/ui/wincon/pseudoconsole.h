// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_UI_PSEUDO_CONSOLE
#define QOR_PP_H_OS_WINDOWS_UI_PSEUDO_CONSOLE

#include "src/platform/os/windows/common/structures.h"

namespace qor { namespace ui { namespace win {

	class qor_pp_module_interface(QOR_WINCONSOLE) PseudoConsole
	{
	public:
        
        PseudoConsole(void* handle);
        PseudoConsole(qor::platform::win::Coord size, void* hInput, void* hOutput, unsigned long flags);
        ~PseudoConsole();
        void* Handle() const;
        long Resize(platform::win::Coord size);

    private:

        void* m_handle;
    };

}}}//qor::ui::win

#endif//QOR_PP_H_OS_WINDOWS_UI_PSEUDO_CONSOLE
