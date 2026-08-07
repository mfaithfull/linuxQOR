// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_READ
#define QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_READ

namespace qor{ namespace dio{

    template<typename T>
    struct Read
    {    
        Read(T& t) : m_(t)
        { }

        T Get() const {return m_;}

    private:
        T& m_;
    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_READ
