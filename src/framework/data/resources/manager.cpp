// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "resourcehub.h"
#include "src/qor/flyers/error/error.h"
#include "src/framework/event/signals/signal.h"
#include "src/framework/parallel/thread/threadpool.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/framework/app/role/getfeature.h"

namespace qor{ namespace framework{

    ResourceManager::ResourceManager() : m_running(false)
    {
    }

    void ResourceManager::queueSlot(PendingSlot data, ConnectionKind type)
    {
        SlotBase* receiver = data.receiver();
        std::lock_guard<std::mutex> lock(m_mutex);
        m_array.push_back(std::move(data));
        m_alarm.notify_one();
    }

    void ResourceManager::Start()
    {
        if(m_threadPool.IsNotNull())
        {
            m_running = true;
            m_threadPool->PostTask(
                [this]()->void
                {
                    qor::CurrentThread::GetCurrent().SetName("Resource Manager");
                    Run();
                }
            );
        }
        else
        {
            serious("ResourceManager requires the ThreadPool Feature.");
        }
    }

    int ResourceManager::Run()
    {                
        try{
            while (m_running) 
            {
                std::unique_lock<std::mutex> lock(m_mutex);

                if (!m_array.empty()) 
                {
                    auto data = std::move(m_array.front());
                    m_array.pop_front();
                    lock.unlock();                
                    data(); // call the slot
                    continue;
                }
                else if (!m_running) 
                {
                    break;
                }

                m_alarm.wait(lock);
            }
        }
        catch(std::exception e)
        {
            std::string s(e.what());
            serious("Resource Manager stopped due to exception {0}.", s);
        }
        return m_running ? 1 : 0;
    }

    void ResourceManager::ReceiveResourceUpdate(Resource* res, ResourceStatus status)
    {
        //Inform subscribers based on status and URI.
        switch(status)
        {
        case Named:
            OnNamed(res);
            break;
        case Located:
            OnLocated(res);
            break;
        case Claimed:
            OnClaimed(res);
            break;
        case Available:
            OnAvailable(res);
            break;
        case Removed:
            OnRemoved(res);
            break;
        default:
            break;
        }
    }

    void ResourceManager::Stop()
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_running = false;
        }
        m_alarm.notify_one();
        m_running = false;
    }

    void ResourceManager::OnNamed(Resource* res){}
    void ResourceManager::OnLocated(Resource* res){}
    void ResourceManager::OnClaimed(Resource* res){}
    void ResourceManager::OnAvailable(Resource* res){}
    void ResourceManager::OnRemoved(Resource* res){}

}}//qor::framework