// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_READONLY
#define QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_READONLY

#include "read.h"

namespace qor{ namespace dio{

    template <typename T, typename... Bases>
    struct ReadOnlyValueAdapter : public Read<T>, public Bases...
    {
        using value_type = T;
        ReadOnlyValueAdapter(T& t) : Read<T>(t), Bases(std::forward<T&>(t))...
        { }
    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_READONLY
