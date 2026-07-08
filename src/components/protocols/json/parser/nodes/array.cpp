// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <sstream>
#include "array.h"

namespace qor { namespace components{ namespace parser{ namespace json {

    std::string ArrayNode::ToString() const
    {
        std::stringstream buffer;
        buffer << "[";
        if(!GetObject()->m_values.empty())
        {
            for(auto it = GetObject()->m_values.begin(), it2 = it+1; it2 != GetObject()->m_values.end(); ++it, ++it2)
            {
                //buffer << (*it)->ToString();
                buffer << ",";
            }
            //buffer << (*GetObject()->m_values.rbegin())->ToString();
        }
        buffer << "]";
        return buffer.str();
    }

}}}}//qor::components::parser::json
