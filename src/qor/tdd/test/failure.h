// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TESTFAILURE
#define QOR_PP_H_TESTFAILURE

#include <sstream>
#include "detail/hascoutsupport.h"
#include "detail/extrainformation.h"

namespace qor { namespace test {
    
    class failure
    {
    private:

        std::string m_failure;

    public:

        template <typename Expected, typename Actual>
        failure(const Expected& e, const Actual& a, const char* at = "", const char* expr = "")
        {
            std::ostringstream os;
            os << at << expr;
            detail::extra_information< Expected, Actual, detail::has_cout_support< std::ostream, Expected >::value >::print(os, e, a);
            m_failure = os.str();
        }

        failure(const char* at = "", const char* expr = "")
        {
            std::ostringstream os;
            os << at << expr;
            m_failure = os.str();
        }

        virtual ~failure() noexcept = default;

        virtual const char* what() const noexcept 
        { 
            return m_failure.c_str(); 
        }

    };
}}//qor::test

#endif//QOR_PP_H_TESTFAILURE