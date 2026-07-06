// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>

#include "service.h"
#include "src/framework/parallel/sync/onscopeexit.h"
#include "src/framework/app/role/role.h"
#include "src/framework/parallel/thread/threadpool.h"

namespace qor { namespace io{ namespace async{

    Service::Service()
    {
        m_contextArray = nullptr;
        m_contextIndex = 0;
    }

    Service::~Service() noexcept = default;

    void Service::Setup()
    {
        m_threadPool = m_Role->GetFeature<thread::ThreadPool>();

        //Take all the Context(s) from the pool.
        m_contextCount = (unsigned int)PoolInstancer::GetPoolSize<Context>();
        m_contextArray = new ref_of<Context>::type [m_contextCount];
        for(unsigned context = 0; context < m_contextCount; context++)
        {
            m_contextArray[context] = new_ref<Context>(m_threadPool);
        }
    }

    void Service::Shutdown()
    {
        for(unsigned context = 0; context < m_contextCount; context++)
        {
            m_contextArray[context].Dispose();
        }
        delete [] m_contextArray;
        m_contextArray = nullptr;
    }

    ref_of<Context::Session>::type Service::GetSession()
    {
        return m_contextArray[m_contextIndex++ % m_contextCount]->GetSession();
    }

}}}//qor::io::async