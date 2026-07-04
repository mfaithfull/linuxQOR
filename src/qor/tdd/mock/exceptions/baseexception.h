// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_BASEEXCEPTION
#define QOR_PP_H_TESTMOCK_BASEEXCEPTION

#include <exception>
#include <string>

#ifndef qor_pp_mock_baseexception
#define qor_pp_mock_baseexception std::exception
#endif

#define qor_pp_mock_raiseexception(e)   { /*qor_pp_compiler_debugbreak(e);*/ if(std::uncaught_exceptions() > 0) latentException = [=, &repo]{ throw e; }; else throw e; }

namespace qor{ namespace mock{

    class BaseException : public qor_pp_mock_baseexception
    {
    public:
        ~BaseException() throw() {}
        const char* what() const throw() { return txt.c_str(); }
    protected:
        std::string txt;
    };    

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_BASEEXCEPTION