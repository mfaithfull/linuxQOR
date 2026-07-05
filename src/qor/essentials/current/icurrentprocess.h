// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_THREAD_CURRENT_PROCESS_INTERFACE
#define QOR_PP_H_FRAMEWORK_THREAD_CURRENT_PROCESS_INTERFACE

#include <optional>
#include <vector>

#include "src/qor/memory/instance/singleton.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/factory/externalfactory.h"

namespace qor
{
    //Base interface for platform specific classes representing the containing process
    class ICurrentProcess
    {
    public:

        enum class Priority
        {
            idle = 1,
            below_normal,
            normal,
            above_normal,
            high,
            realtime
        };

        inline ICurrentProcess() = default;
        inline virtual ~ICurrentProcess() noexcept = default;

        virtual std::optional<std::vector<bool>> GetAffinity()
        {
            return std::nullopt;
        }

        virtual bool SetAffinity(const std::vector<bool>& /*affinity*/)
        {
            return false;
        }

        virtual std::optional<Priority> GetPriority()
        {
            return std::nullopt;
        }

        virtual bool SetPriority(const Priority /*priority*/)
        {
            return false;
        }
    };
    
    qor_pp_declare_instancer_of(ICurrentProcess, SingletonInstancer);
    qor_pp_declare_factory_of(ICurrentProcess, ExternalFactory);
    constexpr GUID ICurrentProcessGUID = {0x57fd8b55, 0xa793, 0x4510, { 0xab, 0x66, 0xf7, 0x5a, 0x00, 0xb6, 0x13, 0x23}};
    qor_pp_declare_guid_of(ICurrentProcess,ICurrentProcessGUID);

}//qor

#endif//QOR_PP_H_FRAMEWORK_THREAD_CURRENT_PROCESS_INTERFACE
