// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TESTDETAIL_HASCOUTSUPPORT
#define QOR_PP_H_TESTDETAIL_HASCOUTSUPPORT

namespace qor { namespace test { namespace detail {

    typedef char yes;
    typedef char(&no)[2];
    struct any_t
    {
        template<class T> any_t(const T&);
    };

    no operator << (const any_t&, const any_t&);
    no operator >> (const any_t&, const any_t&);

    template <class T> yes check(T const&);

    no check(no);

    template<typename streamtype, typename T>
    struct has_cout_support
    {
        static streamtype& stream;
        static T& x;
        static const bool value = sizeof(check(stream << x)) == sizeof(yes);
    };
    
}}}//qor::test::detail

#endif//QOR_PP_H_TESTDETAIL_HASCOUTSUPPORT
