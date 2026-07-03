// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_THREAD_CURRENT_INTERFACE
#define QOR_PP_H_FRAMEWORK_THREAD_CURRENT_INTERFACE

#include <optional>
#include <vector>

#include "src/qor/memory/instance/singleton.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/factory/externalfactory.h"
#include "src/qor/memory/instance/threadsingleton.h"

namespace qor
{
    //Base interface for platform specific class representing the current thread of execution
    class ICurrentThread
    {
    public:

        enum class Priority
        {
            idle = 0,
            lowest,
            below_normal,
            normal,
            above_normal,
            highest,
            realtime
        };    

        ICurrentThread() = default;
        virtual ~ICurrentThread() noexcept = default;

        virtual bool SetPriority(ICurrentThread::Priority /*priority*/)
        {
            return false;
        }

        virtual std::optional<ICurrentThread::Priority> GetPriority() const
        {
            return Priority::normal;
        }

        virtual bool SetName(const std::string& /*name*/)
        {
            return false;
        }

        virtual std::optional<std::string> GetName()
        {
            return std::nullopt;
        }

        virtual bool SetAffinity(const std::vector<bool>& /*affinity*/)
        {
            return false;
        }
        
        virtual std::optional<std::vector<bool>> GetAffinity()
        {
            return std::nullopt;
        }
    };
    
    qor_pp_declare_instancer_of(ICurrentThread, ThreadSingletonInstancer);    
    qor_pp_declare_factory_of(ICurrentThread, ExternalFactory);
    constexpr GUID ICurrentThreadGUID = {0x6d4deb2e, 0x05d5, 0x40b0, { 0xbc, 0x25, 0x9d, 0xa9, 0x3d, 0x55, 0xa1, 0xd6}};
    qor_pp_declare_guid_of(ICurrentThread,ICurrentThreadGUID);

}//qor

#endif//QOR_PP_H_FRAMEWORK_THREAD_CURRENT_PROCESS_INTERFACE


