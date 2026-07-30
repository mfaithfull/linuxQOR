// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <errno.h>
#include <optional>
#include <vector>
#include <format>
#include "src/qor/flyers/error/error.h"
#include "currentprocess.h"

#include <windows.h>
#include <processthreadsapi.h>

//Implement this trivial function so the linker will pull in this library to fulfill the ImplementsICurrentProcess requirement. 
namespace qor { bool qor_pp_module_interface(QOR_WINDOWSPROCESS) ImplementsICurrentProcess() {return true;} }

namespace qor{ namespace win{

    CurrentProcess::CurrentProcess() = default;
    CurrentProcess::~CurrentProcess() noexcept = default;

    std::optional<std::vector<bool>> CurrentProcess::GetAffinity()
    {
        DWORD_PTR process_mask = 0;
        DWORD_PTR system_mask = 0;
        if (GetProcessAffinityMask(GetCurrentProcess(), &process_mask, &system_mask) == 0)
        {
            return std::nullopt;
        }
#ifdef __cpp_lib_int_pow2
        const std::size_t num_cpus = static_cast<std::size_t>(std::bit_width(system_mask));
#else
        std::size_t num_cpus = 0;
        if (system_mask != 0)
        {
            num_cpus = 1;
            while ((system_mask >>= 1U) != 0U)
                ++num_cpus;
        }
#endif
        std::vector<bool> affinity(num_cpus);
        for (std::size_t i = 0; i < num_cpus; ++i)
        {
            affinity[i] = ((process_mask & (1ULL << i)) != 0ULL);
        }
        return affinity;     
    }

    bool CurrentProcess::SetAffinity(const std::vector<bool>& affinity)
    {
        DWORD_PTR process_mask = 0;
        for (std::size_t i = 0; i < std::min<std::size_t>(affinity.size(), sizeof(DWORD_PTR) * 8); ++i)
            process_mask |= (affinity[i] ? (1ULL << i) : 0ULL);
        return SetProcessAffinityMask(GetCurrentProcess(), process_mask) != 0;
    }

    std::optional<qor::ICurrentProcess::Priority> CurrentProcess::GetPriority()
    {
        const DWORD OSPriority = GetPriorityClass(GetCurrentProcess());
        if (OSPriority == 0)
        {
            return std::nullopt;
        }

        switch(OSPriority)
        {
        case IDLE_PRIORITY_CLASS:
            return qor::ICurrentProcess::Priority::idle;
        case BELOW_NORMAL_PRIORITY_CLASS:
            return qor::ICurrentProcess::Priority::below_normal;
        case NORMAL_PRIORITY_CLASS:
            return qor::ICurrentProcess::Priority::normal;
        case ABOVE_NORMAL_PRIORITY_CLASS:
            return qor::ICurrentProcess::Priority::above_normal;
        case HIGH_PRIORITY_CLASS:
            return qor::ICurrentProcess::Priority::high;
        case REALTIME_PRIORITY_CLASS:
            return qor::ICurrentProcess::Priority::realtime;
        }

        return qor::ICurrentProcess::Priority::normal;
    }

    bool CurrentProcess::SetPriority(const qor::ICurrentProcess::Priority priority)
    {
        DWORD OSPriority = NORMAL_PRIORITY_CLASS;
        switch(priority)
        {
        case qor::ICurrentProcess::Priority::idle:
            OSPriority = IDLE_PRIORITY_CLASS;
            break;
        case qor::ICurrentProcess::Priority::below_normal:
            OSPriority = BELOW_NORMAL_PRIORITY_CLASS;
            break;
        case qor::ICurrentProcess::Priority::normal:
            OSPriority = IDLE_PRIORITY_CLASS;
            break;
        case qor::ICurrentProcess::Priority::above_normal:
            OSPriority = ABOVE_NORMAL_PRIORITY_CLASS;
            break;
        case qor::ICurrentProcess::Priority::high:
            OSPriority = HIGH_PRIORITY_CLASS;
            break;
        case qor::ICurrentProcess::Priority::realtime:
            OSPriority = REALTIME_PRIORITY_CLASS;
            break;
        }

        return SetPriorityClass(GetCurrentProcess(), OSPriority) != 0;
    }

    ref_of<ProcessHeap>::type CurrentProcess::GetHeap()
    {
        return new_ref<ProcessHeap>();
    }
    
}}//qor::win
