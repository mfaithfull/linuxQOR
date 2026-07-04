// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TYPEIDENTITY
#define QOR_PP_H_TYPEIDENTITY

#include <typeinfo>
#include <typeindex>

#include "src/platform/compiler/compiler.h"
#include "src/qor/essentials/datastructures/guid.h"

namespace qor{

    template<typename T>
    struct id_of
    {
#if(qor_pp_compiler == qor_pp_compiler_clang)
        static std::type_index id()
#else
        static constexpr std::type_index id()
#endif
        {
            return std::type_index(typeid(T));
        }
    };

    template<typename T>
    struct name_of
    {
        static std::string name()
        {
            return compiler::demangle<T>();
        }
    };

    template<typename T>
    struct guid_of
    {
        static const GUID* guid()
        {
            return &null_guid;
        }
    };

//Preprocessor macro shorthand for declaring a guid_of template specialisation
#   define qor_pp_declare_guid_of(_CLASS,_GUID)\
    template<> struct guid_of< _CLASS >\
    {\
        static const GUID* guid()\
        {\
            return &_GUID;\
        }\
    };

//Example: 
/*
constexpr FeatureGUID = {0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0}};
qor_pp_declare_guid_of(Feature, FeatureGUID);
*/

}//qor

#endif//QOR_PP_H_TYPEIDENTITY
