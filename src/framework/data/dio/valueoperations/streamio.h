// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_STREAMIO
#define QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_STREAMIO

#include "streamin.h"
#include "streamout.h"

namespace qor{ namespace dio{

    template<typename T>
    struct StreamIO : public StreamOut<T>, public StreamIn<T>
    {
        StreamIO(T& t) : StreamOut<T>(t), StreamIn<T>(t)
        { }
    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_STREAMIO
