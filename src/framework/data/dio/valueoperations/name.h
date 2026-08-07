// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_NAME
#define QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_NAME

#include <vector>

namespace qor{ namespace dio{

    template<typename T>
    struct Name
    {
        Name(T& t){ }

        void SetName(const std::string& name)
        {
            m_name = name;
        }    

        std::string GetName() const
        {
            return m_name;
        }

    private:
        std::string m_name;
    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_WRITE
