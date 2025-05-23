// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "src/configuration/configuration.h"

#include <errno.h>
#include <optional>
#include <vector>
#include <format>
#include "src/qor/error/error.h"
#include "currentprocess.h"

#include <windows.h>
#include <processthreadsapi.h>

namespace qor {
    bool qor_pp_module_interface(QOR_WINDOWSPROCESS) ImplementsICurrentProcess() //Implement this trivial function so the linker will pull in this library to fulfil the ImplementsIFileSystem requirement. 
    {
        return true;
    }
}

namespace qor{ namespace nsWindows{ namespace framework{

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

    std::optional<qor::framework::ICurrentProcess::Priority> CurrentProcess::GetPriority()
    {
        const DWORD OSPriority = GetPriorityClass(GetCurrentProcess());
        if (OSPriority == 0)
        {
            return std::nullopt;
        }

        switch(OSPriority)
        {
            case IDLE_PRIORITY_CLASS:
            return qor::framework::ICurrentProcess::Priority::idle;
            case BELOW_NORMAL_PRIORITY_CLASS:
            return qor::framework::ICurrentProcess::Priority::below_normal;
            case NORMAL_PRIORITY_CLASS:
            return qor::framework::ICurrentProcess::Priority::normal;
            case ABOVE_NORMAL_PRIORITY_CLASS:
            return qor::framework::ICurrentProcess::Priority::above_normal;
            case HIGH_PRIORITY_CLASS:
            return qor::framework::ICurrentProcess::Priority::high;
            case REALTIME_PRIORITY_CLASS:
            return qor::framework::ICurrentProcess::Priority::realtime;
        }

        return qor::framework::ICurrentProcess::Priority::normal;
    }

    bool CurrentProcess::SetPriority(const qor::framework::ICurrentProcess::Priority priority)
    {
        DWORD OSPriority = NORMAL_PRIORITY_CLASS;
        switch(priority)
        {
            case qor::framework::ICurrentProcess::Priority::idle:
            OSPriority = IDLE_PRIORITY_CLASS;
            break;
            case qor::framework::ICurrentProcess::Priority::below_normal:
            OSPriority = BELOW_NORMAL_PRIORITY_CLASS;
            break;
            case qor::framework::ICurrentProcess::Priority::normal:
            OSPriority = IDLE_PRIORITY_CLASS;
            break;
            case qor::framework::ICurrentProcess::Priority::above_normal:
            OSPriority = ABOVE_NORMAL_PRIORITY_CLASS;
            break;
            case qor::framework::ICurrentProcess::Priority::high:
            OSPriority = HIGH_PRIORITY_CLASS;
            break;
            case qor::framework::ICurrentProcess::Priority::realtime:
            OSPriority = REALTIME_PRIORITY_CLASS;
            break;
        }

        return SetPriorityClass(GetCurrentProcess(), OSPriority) != 0;
    }

}}}//qor::nsWindows::framework
