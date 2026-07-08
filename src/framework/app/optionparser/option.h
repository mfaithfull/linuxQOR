// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OPTIONPARSER_OPTION
#define QOR_PP_H_OPTIONPARSER_OPTION

namespace qor{ namespace app{

    struct Option
    {
        static constexpr int no_argument = 0;
        static constexpr int required_argument = 1;
        static constexpr int optional_argument = 2;

        const char* name;
        int has_arg;
        int* flag;
        int val;
    };

}}//qor::app

#endif//QOR_PP_H_OPTIONPARSER_OPTION
