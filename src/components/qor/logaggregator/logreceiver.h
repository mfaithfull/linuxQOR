// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_LOGRECEIVER
#define QOR_PP_H_COMPONENTS_LOGRECEIVER
 
#include <atomic>
#include <coroutine>
#include "src/qor/flyers/error/error.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/instance/singleton.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/factory/externalfactory.h"
#include "src/qor/memory/reference/newref.h"
#include "src/framework/app/role/ifeature.h"
#include "src/framework/parallel/thread/threadpool.h"
#include "src/framework/event/signals/signal.h"
#include "src/components/qor/loghandler/loghandler.h"
#include "src/framework/io/filesystem/filesystem.h"
#include "src/framework/io/filesystem/path.h"

namespace qor { namespace components{
  
    class qor_pp_module_interface(QOR_LOGAGGREGATOR) LogReceiver : public SlotBase
    {
    public:

        LogReceiver();
        virtual ~LogReceiver() noexcept;

        void Stop();
        void ReceiveLog(std::string logEntry);
        void Listen();

        void WriteToStandardOutput(bool write = true);
        void WriteToFileSystem(io::filesystem::Path path, const std::string& fileNamePrefix);
        void WriteToPipeline(bool write = true);

    private:        

        virtual void queueSlot(PendingSlot data, ConnectionKind type) override;
        std::deque<PendingSlot> m_array;
        std::mutex m_mutex;
        std::condition_variable m_alarm;      
        bool m_running;
        bool m_writeToStandardOutput;
        bool m_writeToFileSystem;
        bool m_writeToPipeline;

        unsigned int m_logRollLimit;
        unsigned int m_logRollNumber;
        size_t m_totalSizeLimit;
        size_t m_fileSizeLimit;
        qor::ref_of<qor::io::File>::type m_refLogFile;
    };
}}//qor::components

#endif//QOR_PP_H_COMPONENTS_LOGRECEIVER
