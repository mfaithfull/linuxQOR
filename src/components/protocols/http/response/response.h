// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_RESPONSE
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_RESPONSE

#include <string>
#include "../headers/headers.h"
#include "../version.h"
#include "src/qor/flyers/error/error.h"

namespace qor { namespace components { namespace protocols { namespace http {

    class HTTPResponse
    {
    public:

        HTTPResponse() : m_version{1,1}, m_statusCode{404}
        {
        }

        virtual ~HTTPResponse() = default;

        void SetValue(const std::string& value)
        {
            m_response = value;
        }

        const std::string& GetValue() const
        {
            return m_response;
        }

        void SetStatus(const unsigned int status)
        {
            if(status >= 100 && status < 600)
            {
                m_statusCode = status;
            }
            else
            {
                warning("Invalid Status Code not set. Value must be > 99 and < 600");
            }
        }

        unsigned int GetStatus()
        {
            return m_statusCode;
        }

        void SetVersion(const HTTPVersion& version)
        {
            m_version = version;
        }

        HTTPVersion GetVersion()
        {
            return m_version;
        }

        Headers& GetHeaders()
        {
            return m_headers;
        }

        void AddHeader(const std::string& key, const std::string& value)
        {
            m_headers.emplace(key,value);
        }

        void SetHeaders(const Headers& headers)
        {
            m_headers = headers;
        }

        void SetBody(const std::string& body)
        {
            m_body = body;
        }

        std::string GetBody()
        {
            return m_body;
        }

    private:

        std::string m_response;

        Headers m_headers;
        HTTPVersion m_version;
        unsigned int m_statusCode;
        std::string m_body;

    };

}}}}//qor::components::protocols::http

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_RESPONSE