// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_INTERFACE
#define QOR_PP_H_FRAMEWORK_DIO_INTERFACE

namespace qor{ namespace dio{

    //Each of the Args should be a FunctionAdapter
    template<class T, typename... Args>
    struct Interface : public std::tuple<Args...>
    {
        Interface(T& t, Args&... args) : std::tuple<Args...>{std::forward<Args>(args)...}
        { }
        
    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_INTERFACE