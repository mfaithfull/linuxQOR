// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_JIP_REQUEST
#define QOR_PP_H_COMPONENTS_PROTOCOLS_JIP_REQUEST

#include <string>

//TODO: This gets replaced with the proper JSON Object

namespace qor { namespace components { namespace protocols { namespace jip {
    
    //Server side request object
    class JipRequest
    {
    public:

        JipRequest() = default;
        virtual ~JipRequest() = default;

        void SetValue(const std::string& value)
        {
            m_request = value;
        }

        const std::string& GetValue() const
        {
            return m_request;
        }
        
    private:

        std::string m_request;
    };

}}}}//qor::components::protocols::jip

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_JIP_REQUEST