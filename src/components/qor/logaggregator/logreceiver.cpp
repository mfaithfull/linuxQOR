// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include <iostream>

#include "logreceiver.h"
#include "src/platform/platform.h"

using namespace qor::platform;
using namespace qor::io;

namespace qor { namespace components{

    LogReceiver::LogReceiver() : 
        m_writeToStandardOutput(true),
        m_writeToFileSystem(false),
        m_writeToPipeline(false),
        m_logRollLimit(100),
        m_logRollNumber(0),
        m_totalSizeLimit(100 * 1024 * 1024),
        m_fileSizeLimit(10 * 1024 * 1024)
    { }
    
    LogReceiver::~LogReceiver() noexcept = default;
    
    void LogReceiver::WriteToStandardOutput(bool write)
    {
        std::scoped_lock<std::mutex> lock(m_mutex);
        m_writeToStandardOutput = write;
    }

    void LogReceiver::WriteToFileSystem(filesystem::Path path, const std::string& fileNamePrefix)
    {
        auto filesystem = ThePlatform(qor_shared)->GetSubsystem<FileSystem>();

        filesystem::Index pathIndex(path,"");
        if(!pathIndex.Exists())
        {
            filesystem->MakeDir(path);
        }
        std::string strFileName;
        filesystem::Index index;
        size_t fileSize = 0;
        std::scoped_lock<std::mutex> lock(m_mutex);
        do
        {
            strFileName = std::format("{0}{1}.log",fileNamePrefix, m_logRollNumber);
            index.Set(path, strFileName);
        }while(index.Exists() && (++m_logRollNumber < m_logRollLimit) && ((fileSize += index.Size()) < m_totalSizeLimit));
     
        filesystem::Index logFileIndex(path, strFileName);

        m_refLogFile = filesystem->Open(logFileIndex, 
            OpenFor::ReadWrite, logFileIndex.Exists() ? WithFlags::Append :
            WithFlags::CreateNew | WithFlags::Exclusive
        );
        if(m_refLogFile.IsNotNull())
        {
            m_writeToFileSystem = true;
        }
    }

    void LogReceiver::WriteToPipeline(bool write)
    {
        std::scoped_lock<std::mutex> lock(m_mutex);
        m_writeToPipeline = write;
    }

    void LogReceiver::Stop()
    {
        {
            std::scoped_lock<std::mutex> lock(m_mutex);
            m_running = false;
        }
        m_alarm.notify_one();
        m_running = false;
    }

    void LogReceiver::queueSlot(PendingSlot data, ConnectionKind /*type*/)
    {
        //SlotBase* receiver = data.receiver();
        std::scoped_lock<std::mutex> lock(m_mutex);
        m_array.push_back(std::move(data));
        m_alarm.notify_one();
    }

    void LogReceiver::Listen()
    {
        constexpr std::chrono::milliseconds long_sleep_time(250);
        m_running = true;
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

            m_alarm.wait_for(lock, long_sleep_time);
        }
    }

    void LogReceiver::ReceiveLog(std::string logEntry)
    {
        if(m_writeToStandardOutput)
        {
            std::cout << logEntry;// << std::endl;
        }

        if(m_writeToFileSystem)
        {
            m_refLogFile->Write((unsigned char*)(logEntry.data()), logEntry.size());
        }

        if(m_writeToPipeline)
        {
            //TODO:
        }
    }

}}//qor::components
	
