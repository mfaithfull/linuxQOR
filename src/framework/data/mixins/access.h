// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_MIXINS_ACCESS
#define QOR_PP_H_FRAMEWORK_MIXINS_ACCESS

namespace qor{ namespace mix{

    template< typename T >
    struct access {
        auto& get_data() {
            return static_cast<T*>(this)->data;
        }
    };

    template<typename T>
    struct protect : protected T {};

    //protect<access<composition<Ts...>>>
}}//qor::mix
#endif//QOR_PP_H_FRAMEWORK_MIXINS_ACCESS