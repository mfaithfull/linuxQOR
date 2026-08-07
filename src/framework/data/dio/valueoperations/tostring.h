// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_TOSTRING
#define QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_TOSTRING

#include <string>

namespace qor{ namespace dio{

    template<typename T>
    struct ToString
    {
        ToString(T& t) : m_(t)
        { }

        std::string toString() const { return "missing template override for ToString<T>."; }

    private:
        T& m_;
    };

    template<>
    struct ToString<unsigned int>
    {
        ToString(unsigned int& t) : m_(t)
        { }

        std::string toString() const 
        {
            return std::to_string(m_);
        }

    private:
        unsigned int& m_;
    };

    template<>
    struct ToString<std::string>
    {
        ToString(std::string& t) : m_(t)
        { }

        std::string toString() const 
        {
            return m_;
        }

    private:
        std::string& m_;
    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_TOSTRING


