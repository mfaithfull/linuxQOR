// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license

#pragma once

#include <string>
#include <vector>
#include <sstream>

#include "prettyprint.h"
#include "../../test/failure.h"

namespace qor{ namespace assert { namespace util {

    class FailMessage 
    {
    private:

        std::string d_file;
        int d_line;
        std::stringstream d_buffer;
        bool d_failOnError;

        FailMessage() : d_file(""), d_line(), d_buffer(), d_failOnError(true) {}

    public:

        static FailMessage create() 
        { 
            return FailMessage(); 
        }

        FailMessage& file(std::string file) 
        {
            d_file = file;
            return *this;
        }

        FailMessage& line(int line) 
        {
            d_line = line;
            return *this;
        }

        template<typename T>
        FailMessage& fact(const std::string& key, const T& value) 
        {
            d_buffer << key << ": " << value << "\n";
            return *this;
        }

        FailMessage& fact(const std::string& key, const wchar_t& value) 
        {
            d_buffer << key << ": " << static_cast<char>(value) << "\n";
            return *this;
        }
        
        FailMessage& fact(const std::string& key, const char32_t& value) 
        {
            d_buffer << key << ": " << static_cast<char>(value) << "\n";
            return *this;
        }

        FailMessage& fact(const std::string& key, const char16_t& value) 
        {
            d_buffer << key << ": " << static_cast<char>(value) << "\n";
            return *this;
        }

        FailMessage& fact(const std::string& key, const char8_t& value) 
        {
            d_buffer << key << ": " << static_cast<char>(value) << "\n";
            return *this;
        }

        template<typename T, typename std::enable_if<std::is_same<T, bool>::value>::type* = nullptr>
        FailMessage& fact(const std::string& key, bool value) 
        {
            d_buffer << key << ": " << (value? "true" : "false")<< "\n";
            return *this;
        }

        FailMessage& fact(const std::string& key) 
        {
            d_buffer << key << "\n";
            return *this;
        }

        FailMessage& expected(std::string msg) 
        {
            fact(msg);
            return *this;
        }

        template <typename T>
        FailMessage& fact(std::string msg, const T begin, const T end) 
        {
            d_buffer << msg << " [";
            for (auto iter = begin; iter != end; iter++) {
                d_buffer << " " << *iter;
            }
            d_buffer << " ]";
            return *this;
        }

        FailMessage& failOnError(bool failOnError) 
        {
            d_failOnError = failOnError;
            return *this;
        }

        ~FailMessage() noexcept(false)
        {
            throw qor::test::failure(d_file.c_str(), d_buffer.str().c_str());
        }
    };
    
}}}//qor::assert::util
