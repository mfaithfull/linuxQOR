// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ERROR_DEFAULTHANDLER
#define QOR_PP_H_ERROR_DEFAULTHANDLER

#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/error/handler.h"

namespace qor{ 

    class qor_pp_module_interface(QOR_ERROR) DefaultErrorHandler : public qor::IssueHandler<qor::Error>
    {
    public:
        
        virtual bool Handle(const qor::Error& error);
    };

}//qor

#endif//QOR_PP_H_ERROR_DEFAULTHANDLER