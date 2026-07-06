// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_ASYNCIOSERVICE
#define QOR_PP_H_FRAMEWORK_ASYNCIOSERVICE

#include <atomic>
#include <coroutine>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/instance/singleton.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/factory/externalfactory.h"
#include "src/qor/memory/reference/newref.h"
#include "src/framework/app/role/ifeature.h"
#include "src/framework/io/network/address.h"
#include "src/framework/parallel/thread/threadpool.h"
#include "asynciotypes.h"
#include "asyncioeventprocessor.h"
#include "asyncioinitiator.h"
#include "asynciocontext.h"

namespace qor{ bool qor_pp_module_interface(QOR_ASYNCIOSERVICE) ImplementsAsyncIOService();}

namespace qor { namespace io{ namespace async{

    class qor_pp_module_interface(QOR_ASYNCIOSERVICE) AsyncIOService : public app::IFeature
    {
    public:

        AsyncIOService();
        qor_pp_noinline virtual ~AsyncIOService() noexcept = default;

		AsyncIOService(AsyncIOService&& other) = delete;
		AsyncIOService(const AsyncIOService& other) = delete;
		AsyncIOService& operator=(AsyncIOService&& other) = delete;
		AsyncIOService& operator=(const AsyncIOService& other) = delete;

        virtual void Setup();
        virtual void Shutdown();

        ref_of<AsyncIOContext::Session>::type GetSession();

    private:

        ref_of<thread::ThreadPool>::type m_threadPool;
        unsigned m_contextCount;
        ref_of<AsyncIOContext>::type* m_contextArray;
        size_t m_contextIndex;

    };
    }} //io::async

    qor_pp_declare_instancer_of(io::async::AsyncIOService, SingletonInstancer);
    qor_pp_declare_factory_of(io::async::AsyncIOService, ExternalFactory);
    constexpr GUID AsyncIOServiceGUID = {0x6201abca, 0xf405, 0x4709, {0xa9, 0x86, 0x26, 0x82, 0xeb, 0x66, 0xfd, 0xc6}};
    qor_pp_declare_guid_of(io::async::AsyncIOService,AsyncIOServiceGUID);

}//qor

#endif//QOR_PP_H_FRAMEWORK_ASYNCIOSERVICE
