// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_PRINTARG
#define QOR_PP_H_TESTMOCK_PRINTARG

namespace qor{ namespace mock{

    template <typename T>
    struct printArg
    {
        static inline void print(std::ostream& os, T arg, bool withComma)
        {
            if (withComma)
            {
                os << ",";
            }
            os << arg;
        }

        static inline void print(std::wostream& os, T arg, bool withComma)
        {
            if (withComma)
            {
                os << L",";
            }
            os << arg;
        }
    };

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_PRINTARG
