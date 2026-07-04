// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_ZOMBIEMOCKEXCEPTION
#define QOR_PP_H_TESTMOCK_ZOMBIEMOCKEXCEPTION

#include <sstream>

namespace qor{ namespace mock{
    
    class ZombieMockException : public BaseException 
    {
    public:
        ZombieMockException(MockRepository& repo)
        {
            std::stringstream text;
            text << "Function called on mock that has already been destroyed!" << std::endl;
            text << repo;

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

#endif//QOR_PP_H_TESTMOCK_ZOMBIEMOCKEXCEPTION