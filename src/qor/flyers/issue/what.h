// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ISSUE_WHAT
#define QOR_PP_H_ISSUE_WHAT

#include <string>

//A basic What class that just wraps a std::string

namespace qor{ 

    class qor_pp_module_interface(QOR_ISSUE) What
    {
    public:

        What(const std::string& what) : m_content(what){ }
        virtual ~What() noexcept = default;

        const std::string& Content() const;

    private:

        std::string m_content;
    };

}//qor

#endif//QOR_PP_H_ISSUE_WHAT
