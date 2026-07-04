// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_DONTCARE
#define QOR_PP_H_TESTMOCK_DONTCARE

#include <iostream>

namespace qor{ namespace mock {

    class DontCare
    {
        static DontCare& instance();
    };

    static DontCare qor_pp_mock_dontcare_name;

    inline DontCare& DontCare::instance()
    {
        return qor_pp_mock_dontcare_name;
    }

    inline std::ostream& operator << (std::ostream& os, DontCare const&)
    {
        os << "_";
        return os;
    }

    template <typename T>
    static inline bool operator==(const DontCare&, const T&)
    {
        return true;
    }

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_DONTCARE
