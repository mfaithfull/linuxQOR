// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EXAMPLES_ECHOSERVER_ECHORESPONSE
#define QOR_PP_H_EXAMPLES_ECHOSERVER_ECHORESPONSE

#include <string>

namespace qor { namespace components { namespace protocols { namespace echo {
    
    //Response parsed from data returned to client
    class EchoResponse
    {
    public:

        EchoResponse() = default;
        virtual ~EchoResponse() = default;

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

}}}}//qor::components::protocols::echo

#endif//QOR_PP_H_EXAMPLES_ECHOSERVER_ECHORESPONSE