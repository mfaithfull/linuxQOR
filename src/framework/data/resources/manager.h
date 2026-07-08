// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_RESOURCES_MANAGER
#define QOR_PP_H_FRAMEWORK_RESOURCES_MANAGER

#include <string>
#include <map>
#include <functional>
#include <deque>

#include "src/qor/memory/reference/reference.h"
#include "src/framework/io/filesystem/path.h"
#include "src/framework/io/filesystem/fileindex.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/framework/parallel/thread/threadpool.h"
#include "src/framework/event/signals/slotbase.h"
#include "resource.h"

namespace qor{ namespace framework{

	//Asynchronous resource manager base class
    class qor_pp_module_interface(QOR_RESOURCES) ResourceManager : public SlotBase
    {
    public:
    
        ResourceManager();
        virtual ~ResourceManager() = default;

        //This is the interface seen and used by resources----------------------------------------------------//|
        void ReceiveResourceUpdate(Resource* res, ResourceStatus status);                                     //|
        //This is the interface to be implemented by an actual resource manager----------------------------------.
        virtual void AddPath(const io::filesystem::Path& path, Resource* batchKey = nullptr){}                //|.
        virtual void AddFile(const io::filesystem::Index& file, Resource* batchKey = nullptr){}               //|.
        virtual void AddJSON(const io::filesystem::Index& file, Resource* batchKey = nullptr){}               //|.
        virtual void AddFont(const io::filesystem::Index& file, Resource* batchKey = nullptr){}               //|.
        virtual void BeginBatch(Resource* batchKey){}                                                         //|.
        virtual void EndBatch(Resource* batchKey){}											                  //|.

    protected:

        void Start();        
        void Stop();        
                
        virtual void OnNamed(Resource* res);                                                                  // .
        virtual void OnLocated(Resource* res);                                                                // .
        virtual void OnClaimed(Resource* res);                                  							  // .
        virtual void OnAvailable(Resource* res);                               								  // .
        virtual void OnRemoved(Resource* res);                                								  // .

        ref_of<thread::ThreadPool>::type m_threadPool;

    private:        

        int Run();
        virtual void queueSlot(PendingSlot data, ConnectionKind type) override;
        
        std::deque<PendingSlot> m_array;
        std::mutex m_mutex;
        std::condition_variable m_alarm;
        bool m_running;   

    };
}}//qor::framework

#endif//QOR_PP_H_FRAMEWORK_RESOURCES_MANAGER
