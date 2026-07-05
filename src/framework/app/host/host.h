// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_HOST
#define QOR_PP_H_FRAMEWORK_HOST

#include "src/qor/module/module.h"

namespace qor{

    qor_pp_module_interface(QOR_PP_HOST)Module* TheHost();

    class qor_pp_module_interface(QOR_PP_HOST) Host final : public qor::Module
    {

    public:

        Host();
        virtual ~Host();

        static Module& Instance();

        virtual void RegisterModule(Module* module);
        virtual void UnregisterModule(Module* module);

    };

}//qor

#endif//QOR_PP_H_FRAMEWORK_HOST
