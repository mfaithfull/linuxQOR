// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_BASEMOCK
#define QOR_PP_H_TESTMOCK_BASEMOCK

namespace qor { namespace mock {

    class base_mock
    {
    public:

        void destroy()
        {
            unwriteVft();
            delete this;
        }

        virtual ~base_mock(){}

        void* rewriteVft(void* newVf)
        {
            void* oldVf = *(void**)this;
            *(void**)this = newVf;
            return oldVf;
        }

        void unwriteVft()
        {
            *(void**)this = (*(void***)this)[qor_pp_mock_virt_func_limit + 1];
        }

    };
    
}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_BASEMOCK
