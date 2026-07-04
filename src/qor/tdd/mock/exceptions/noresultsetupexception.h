// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_NORESULTSETUPEXCEPTION
#define QOR_PP_H_TESTMOCK_NORESULTSETUPEXCEPTION

#include <sstream>

#if(qor_pp_os_target == qor_pp_os_linux)
#   include <execinfo.h>
#endif

namespace qor{ namespace mock{

    class NoResultSetUpException : public BaseException 
    {
    public:
        NoResultSetUpException(MockRepository& repo, const std::string& args, const char* funcName)
        {
            std::stringstream text;
            text << "No result set up on call to " << funcName << args << std::endl << repo;

#if(qor_pp_os_target == qor_pp_os_linux)
            void* stacktrace[256];
            size_t size = backtrace(stacktrace, sizeof(stacktrace));
            if (size > 0)
            {
                text << "Stackdump:" << std::endl;
                char** symbols = backtrace_symbols(stacktrace, size);
                for (size_t i = 0; i < size; i = i + 1)
                {
                    text << symbols[i] << std::endl;
                }
                free(symbols);
            }
#endif

            txt = text.str();
        }
    };

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_NORESULTSETUPEXCEPTION
