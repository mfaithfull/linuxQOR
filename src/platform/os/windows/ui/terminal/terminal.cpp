// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "terminal.h"

namespace qor{ namespace ui{ namespace tui { namespace win{

    std::uint8_t Terminal::RegisterHyperlink(std::string_view link) 
    {
        for (std::size_t i = 0; i < hyperlinks_.size(); ++i) 
        {
            if (hyperlinks_[i] == link) 
            {
                return i;
            }
        }
        if (hyperlinks_.size() == std::numeric_limits<std::uint8_t>::max()) 
        {
            return 0;
        }
        hyperlinks_.push_back(std::string(link));
        return hyperlinks_.size() - 1;
    }

    const std::string& Terminal::Hyperlink(std::uint8_t id) const 
    {
        if (id >= hyperlinks_.size()) 
        {
            return hyperlinks_[0];
        }
        return hyperlinks_[id];
    }

}}}}//qor::ui::tui::win