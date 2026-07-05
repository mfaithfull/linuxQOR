// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_THREAD
#define QOR_PP_H_FRAMEWORK_THREAD

#include <chrono>
#include <thread>
#include <type_traits>

#include "src/platform/compiler/compiler.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/framework/event/delegate/delegate.h"

namespace qor{

    class Thread
    {
    public:

		typedef std::jthread std_thread_t;
		typedef std::jthread::native_handle_type native_handle_type;
		typedef std::jthread::id id;

        qor_pp_module_interface(QOR_THREAD) Thread();
		Thread(const Thread & src) = delete;
		qor_pp_module_interface(QOR_THREAD) Thread(Thread&& other) noexcept;
		qor_pp_module_interface(QOR_THREAD) virtual ~Thread();
		Thread&	operator=(const Thread&) = delete;
		qor_pp_module_interface(QOR_THREAD) Thread&	operator=(Thread&& other) noexcept;
		qor_pp_module_interface(QOR_THREAD) void swap(Thread& other) noexcept;
	
		template<typename _Callable, typename... _Args,	typename = std::enable_if_t<!std::is_same_v<std::remove_cvref_t<_Callable>,Thread>>>
	 	explicit Thread(_Callable&& __f, _Args&&... __args) : 
			m_std_thread(std::forward<_Callable>(__f), std::forward<_Args>(__args)...), 
			m_callback(m_std_thread.get_stop_token(), Delegate<void(void)>::Create<Thread, &Thread::CleanUp>(this) ) 
		{ }
		
		qor_pp_module_interface(QOR_THREAD) std::thread::id GetID();
		qor_pp_module_interface(QOR_THREAD) void Detach();
		qor_pp_module_interface(QOR_THREAD) std::stop_source GetStopSource();
		qor_pp_module_interface(QOR_THREAD) std::stop_token GetStopToken();
		qor_pp_module_interface(QOR_THREAD) void Join();
		qor_pp_module_interface(QOR_THREAD) bool Joinable();
		qor_pp_module_interface(QOR_THREAD) bool RequestStop();
		qor_pp_module_interface(QOR_THREAD) std::jthread& stdThread();
		qor_pp_module_interface(QOR_THREAD) virtual void Run();
		
    private:	

		qor_pp_module_interface(QOR_THREAD) void Setup();
		qor_pp_module_interface(QOR_THREAD) void CleanUp();
		
		CurrentThread* m_pCurrent;
        std::jthread m_std_thread;
		std::stop_callback< Delegate<void(void)> > m_callback;
    };

}//qor

#endif//QOR_PP_H_FRAMEWORK_THREAD
