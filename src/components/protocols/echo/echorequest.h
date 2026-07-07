// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EXAMPLES_ECHOSERVER_ECHOREQUEST
#define QOR_PP_H_EXAMPLES_ECHOSERVER_ECHOREQUEST

#include <string>

namespace qor { namespace components { namespace protocols { namespace echo {
    
    //Server side request object
    class EchoRequest
    {
    public:

        EchoRequest() = default;
        virtual ~EchoRequest() = default;

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

}}}}//qor::components::protocols::echo

#endif//QOR_PP_H_EXAMPLES_ECHOSERVER_ECHOREQUEST