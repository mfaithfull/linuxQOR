// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <regex>
#include <unordered_set>
#include "request.h"

namespace qor { namespace components { namespace protocols { namespace http {

    HTTPRequest::HTTPRequest()
    {
        m_version.major = 1;
        m_version.minor = 1;
    }

    std::string HTTPRequest::AppendQueryParams(const std::string &path, const qor::components::protocols::http::Params &params) 
    {
        std::string path_with_query = path;
        thread_local const std::regex re("[^?]+\\?.*");
        auto delm = std::regex_match(path, re) ? '&' : '?';
        path_with_query += delm + qor::components::protocols::http::HTTPRequest::ParamsToQueryStr(params);
        return path_with_query;
    }

    std::string HTTPRequest::ParamsToQueryStr(const qor::components::protocols::http::Params &params)
    {
        std::string query;

        for (auto it = params.begin(); it != params.end(); ++it) 
        {
            if (it != params.begin()) { query += "&"; }
            query += qor::components::protocols::http::HTTPRequest::EncodeQueryComponent(it->first);
            query += "=";
            query += qor::components::protocols::http::HTTPRequest::EncodeQueryComponent(it->second);
        }
        return query;
    }

    std::string HTTPRequest::EncodeQueryComponent(const std::string &component, bool space_as_plus) 
    {
        std::string result;
        result.reserve(component.size() * 3);

        for (size_t i = 0; i < component.size(); i++) 
        {
            auto c = static_cast<unsigned char>(component[i]);

            // Unreserved characters per RFC 3986
            if (std::isalnum(c) || c == '-' || c == '.' || c == '_' || c == '~') 
            {
                result += static_cast<char>(c);
            }
            // Space handling
            else if (c == ' ') 
            {
                if (space_as_plus) 
                {
                    result += '+';
                } 
                else {
                    result += "%20";
                }
            }
            // Plus sign handling
            else if (c == '+') 
            {
                if (space_as_plus) 
                {
                    result += "%2B";
                } 
                else 
                {
                    result += static_cast<char>(c);
                }
            }
            // Query-safe sub-delimiters (excluding & and = which are query delimiters)
            else if (c == '!' || c == '$' || c == '\'' || c == '(' || c == ')' || c == '*' || c == ',' || c == ';') 
            {
                result += static_cast<char>(c);
            }
            // Colon and @ are allowed in query
            else if (c == ':' || c == '@') 
            {
                result += static_cast<char>(c);
            }
            // Forward slash is allowed in query values
            else if (c == '/') 
            {
                result += static_cast<char>(c);
            }
            // Question mark is allowed in query values (after first ?)
            else if (c == '?') 
            {
                result += static_cast<char>(c);
            } 
            else 
            {
                result += '%';
                char hex[3];
                snprintf(hex, sizeof(hex), "%02X", c);
                result.append(hex, 2);
            }
        }
        return result;
    }
}}}} //qor::components::protocols::http