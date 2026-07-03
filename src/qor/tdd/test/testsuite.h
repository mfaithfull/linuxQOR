// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TESTSUITE
#define QOR_PP_H_TESTSUITE

#include <string>
#include <limits>
#include "failure.h"

namespace qor{ namespace test{

    template <typename Suite, typename Case = void>
    struct Test : public virtual Suite{ };

    template <typename Expected, typename Actual>
    inline void equal(const Expected& e, const Actual& a, const char* at = "", const char* expr = "")
    {
        if (e != a)
        {
            throw failure(e, a, at, expr);
        }
    }

    inline void equal(const char* e, const char* a, const char* at = "", const char* expr = "")
    {
        if (std::string(e) != std::string(a))
        {
            throw failure(e, a, at, expr);
        }
    }

    inline void equal(double e, double a, const char* at = "", const char* expr = "")
    {
        double max = std::abs(std::max<double>(e, a));
        max = max < 1.0 ? 1.0 : max;
        if (std::abs(e - a) > std::numeric_limits<double>::epsilon() * max)
        {
            throw failure(e, a, at, expr);
        }
    }

    inline void check(bool b, const char* at = "", const char* expr = "")
    {
        if (!b)
        {
            throw failure(at, expr);
        }
    }

    template <typename Expected, typename Actual>
    inline void unequal(const Expected& e, const Actual& a, const char* at = "", const char* expr = "")
    {
        if (e == a)
        {
            throw failure(e, a, at, expr);
        }
    }

    inline void unequal(const char* e, const char* a, const char* at = "", const char* expr = "")
    {
        if (std::string(e) == std::string(a))
        {
            throw failure(e, a, at, expr);
        }
    }

    inline void unequal(double e, double a, const char* at = "", const char* expr = "")
    {
        double max = std::abs(std::max(e, a));
        max = max < 1.0 ? 1.0 : max;
        if( std::abs(e -a) <= std::numeric_limits<double>::epsilon() * max)
        {
            throw failure(e, a, at, expr);
        }
    }

    template <typename T>
    void fail(const T& expr, const char* at = "")
    {
        std::ostringstream os;
        os << expr;
        throw failure(at,os.str().c_str());
    }
}}//qor::test

#endif//QOR_PP_H_TESTSUITE
