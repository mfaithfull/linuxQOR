// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_READWITE
#define QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_READWITE

#include "read.h"
#include "write.h"

namespace qor{ namespace dio{

    template<typename T, typename... Bases>
    struct ReadWriteValueAdapter : public Read<T>, public Write<T>, public Bases...
    {
        using value_type = T;
        ReadWriteValueAdapter(T& t) : Read<T>(t), Write<T>(t), Bases(std::forward<T&>(t))...
        { }
    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_READWITE
