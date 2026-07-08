// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUEST_QUERY
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUEST_QUERY

#include <string>
#include <map>

namespace qor { namespace components { namespace protocols { namespace http {

    inline std::string EncodeQueryComponent(const std::string &component, bool space_as_plus) 
    {
        std::string result;
        result.reserve(component.size() * 3);

        for (size_t i = 0; i < component.size(); i++) 
        {
            auto c = static_cast<unsigned char>(component[i]);
            
            if (std::isalnum(c) || c == '-' || c == '.' || c == '_' || c == '~') // Unreserved characters per RFC 3986
            {
                result += static_cast<char>(c);
            }
            else if (c == ' ') // Space handling
            {
                if (space_as_plus) 
                {
                    result += '+';
                } 
                else 
                {
                    result += "%20";
                }
            }            
            else if (c == '+') // Plus sign handling
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
            else if (c == '!' || c == '$' || c == '\'' || c == '(' || c == ')' || c == '*' || c == ',' || c == ';') // Query-safe sub-delimiters (excluding & and = which are query delimiters)
            {
                result += static_cast<char>(c);
            }            
            else if (c == ':' || c == '@') // Colon and @ are allowed in query
            {
                result += static_cast<char>(c);
            }            
            else if (c == '/') // Forward slash is allowed in query values
            {
                result += static_cast<char>(c);
            }            
            else if (c == '?') // Question mark is allowed in query values (after first ?)
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

    inline std::string DecodeQueryComponent(const std::string &component, bool plus_as_space = true) 
    {
        std::string result;
        result.reserve(component.size());

        for (size_t i = 0; i < component.size(); i++) 
        {
            if (component[i] == '%' && i + 2 < component.size()) 
            {
                std::string hex = component.substr(i + 1, 2);
                char *end;
                unsigned long value = std::strtoul(hex.c_str(), &end, 16);
                if (end == hex.c_str() + 2) 
                {
                    result += static_cast<char>(value);
                    i += 2;
                } 
                else 
                {
                    result += component[i];
                }
                } 
                else if (component[i] == '+' && plus_as_space) 
                {
                    result += ' '; // + becomes space in form-urlencoded
                } else {
                result += component[i];
            }
        }
        return result;
    }

}}}}//qor::components::protocols::http

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUEST_QUERY