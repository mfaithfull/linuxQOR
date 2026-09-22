// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_JIP_RESPONSE
#define QOR_PP_H_COMPONENTS_PROTOCOLS_JIP_RESPONSE

#include <string>

//TODO: This also gets replaced with a JSON Object

namespace qor { namespace components { namespace protocols { namespace jip {
    
    //Response parsed from data returned to client
    class JipResponse
    {
    public:

        JipResponse() = default;
        virtual ~JipResponse() = default;

        void SetValue(const std::string& value)
        {
            m_response = value;
        }

        const std::string& GetValue() const
        {
            return m_response;
        }
        
    private:

        std::string m_response;
    };

}}}}//qor::components::protocols::jip

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_JIP_RESPONSE