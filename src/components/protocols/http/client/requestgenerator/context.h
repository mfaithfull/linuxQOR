// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTGENERATORCONTEXT
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTGENERATORCONTEXT

#include "src/platform/compiler/compiler.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/framework/data/pipeline/pipeline.h"

namespace qor { namespace components { namespace protocols { namespace http {

    class qor_pp_module_interface(QOR_HTTP) Context
    {
    public:

        inline Context()
        {}

        inline Context(byte* data, size_t itemCount) : m_space(data), m_position(0), m_size(itemCount)
        {}

        inline bool PutOctet(byte data)
        {
            if(m_position < m_size)
            {
                m_space[m_position++] = data;
                return true;
            }
            return false;
        }

        inline size_t GetPosition()
        {
            return m_position;
        }

        inline void SetSpace(byte* data, size_t itemCount)
        {
            m_space = data;
            m_position = 0;
            m_size = itemCount;
        }

        inline bool HasSpace()
        {
            return m_size - m_position > 0;
        }

    private:
    
        byte* m_space{nullptr};
        size_t m_position{0};
        size_t m_size{0};

    };

}}}}//qor::components::protocols::http

#endif //QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_REQUESTGENERATORCONTEXT
