// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_WRITE
#define QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_WRITE

#include <vector>

#include "src/qor/function/tef.h"

namespace qor{ namespace dio{

    template<typename T>
    struct Write
    {
        Write(T& t) : m_(t)
        { }

        void Set(const T t)
        {
            m_ = t;
        }    

    private:
        T& m_;
    };

    template<typename T>
    struct EnhancedNumericWrite
    {
        EnhancedNumericWrite(T& t) : m_(t)
        { }

        void Set(const T t)
        {
            if(m_hasMin && t < m_min)
            {
                throw std::range_error("value too small");
            }
            if(m_hasMax && t > m_max)
            {
                throw std::range_error("value too large");
            }
            m_ = t;
            if(m_hasOnChange)
            {
                m_OnChange(t);
            }
        }    

        void SetMinimum(const T t)
        {
            m_min = t;
            m_hasMin = true;
        }

        void SetMaximum(const T t)
        {
            m_max = t;
            m_hasMax = true;
        }

        void SetOnChange(const tef::Function<void(T)>& onChange)
        {
            m_OnChange = onChange;
            m_hasOnChange = true;
        }

        void ClearMinimum()
        {
            m_hasMin = false;
        }

        void ClearMaximum()
        {
            m_hasMax = false;
        }

        void ClearOnChange()
        {
            m_hasOnChange = false;
        }

    private:
        T& m_;
        bool m_hasMin{false};
        bool m_hasMax{false};
        bool m_hasOnChange{false};
        T m_min;
        T m_max;
        tef::Function<void(T)> m_OnChange;
    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_VALUEADAPTERS_WRITE
