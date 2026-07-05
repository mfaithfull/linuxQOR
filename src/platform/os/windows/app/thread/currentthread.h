// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_CURRENTTHREAD
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_CURRENTTHREAD

#include "src/qor/essentials/current/icurrentthread.h"

namespace qor
{    
    bool qor_pp_module_interface(QOR_WINDOWSTHREAD) ImplementsICurrentThread();
}

namespace qor{ namespace win{

    class qor_pp_module_interface(QOR_WINDOWSTHREAD) CurrentThread : public qor::ICurrentThread
    {
    public:
        qor_pp_noinline CurrentThread() = default;
        qor_pp_noinline virtual ~CurrentThread() noexcept = default;

        virtual bool SetPriority(ICurrentThread::Priority priority);
        virtual std::optional<ICurrentThread::Priority> GetPriority() const;
        virtual bool SetName(const std::string& name);
        virtual std::optional<std::string> GetName();
        virtual bool SetAffinity(const std::vector<bool>& affinity);
        virtual std::optional<std::vector<bool>> GetAffinity();
    };

}}//qor::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_CURRENTTHREAD
