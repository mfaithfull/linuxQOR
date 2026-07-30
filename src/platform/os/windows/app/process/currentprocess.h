// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_PROCESS
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_PROCESS

#include "src/qor/essentials/current/icurrentprocess.h"
#include "heap.h"

namespace qor{
    bool qor_pp_module_interface(QOR_WINDOWSPROCESS) ImplementsICurrentProcess();
}

namespace qor{ namespace win{

    class qor_pp_module_interface(QOR_WINDOWSPROCESS) CurrentProcess : public qor::ICurrentProcess
    {
    public:
        
        CurrentProcess();
        virtual ~CurrentProcess() noexcept;

        std::optional<std::vector<bool>> GetAffinity();
        bool SetAffinity(const std::vector<bool>& affinity);
        std::optional<qor::ICurrentProcess::Priority> GetPriority();
        bool SetPriority(const qor::ICurrentProcess::Priority priority);
        ref_of<ProcessHeap>::type GetHeap();
    };

}}//qor::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_PROCESS
