// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_INTERCEPTION_PARAMETER_CHECKS_NOCHECK
#define QOR_PP_H_INTERCEPTION_PARAMETER_CHECKS_NOCHECK

namespace qor {

    class qor_pp_module_interface(QOR_INTERCEPTION) NoCheck
    {
    public:
        
        template< typename tchk >
        static void Check(tchk /*t*/) { }
    };
    
}//qor

#endif//QOR_PP_H_INTERCEPTION_PARAMETER_CHECKS_NOCHECK
