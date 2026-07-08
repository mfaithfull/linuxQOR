// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <stdexcept>
#include <iostream>
#include "thread.h"

namespace qor{

	Thread::Thread() : m_std_thread(
		Delegate<void(void)>::Create<Thread, &Thread::Setup>(this)
	), m_callback(m_std_thread.get_stop_token(), Delegate<void(void)>::Create<Thread, &Thread::CleanUp>(this)){ }

	Thread::Thread(Thread&& other) noexcept : 
		m_std_thread(std::move(other.m_std_thread)), 
		m_callback(m_std_thread.get_stop_token(), Delegate<void(void)>::Create<Thread, &Thread::CleanUp>(this) )
	{
		m_pCurrent = other.m_pCurrent;
		other.m_pCurrent = nullptr;			
	}

	Thread::~Thread(){ }

	Thread&	Thread::operator=(Thread&& other) noexcept
	{
		other.swap(*this);
		return *this;
	}

	std::thread::id Thread::GetID()
	{
		return m_std_thread.get_id();
	}

    void Thread::Detach(void)
	{
		m_std_thread.detach();
	}

    std::stop_source Thread::GetStopSource(void)
	{		
		return m_std_thread.get_stop_source();
	}

    std::stop_token Thread::GetStopToken()
	{		
		return m_std_thread.get_stop_token();
	}

    void Thread::Join(void)
	{
		m_std_thread.join();				
	}

    bool Thread::Joinable(void)
	{
		return m_std_thread.joinable();		
	}

    bool Thread::RequestStop()
	{
		return m_std_thread.request_stop();
	}

	std::jthread& Thread::stdThread()
	{
		return m_std_thread;
	}

	void Thread::swap(Thread& other) noexcept
	{
		std::swap(m_std_thread, other.m_std_thread);
		m_pCurrent = other.m_pCurrent;
		other.m_pCurrent = nullptr;
	}

	void Thread::Run(){ }

	void Thread::Setup()
	{
		try
		{
			Run();			
		}
		/*
		catch(const Serious& s)
		{
			std::cerr << s.what().Content() << std::endl;
		}
		*/
		catch(const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
		catch(...)
		{
			std::cerr << "Unhandled exception." << std::endl;
		}
	}

	void Thread::CleanUp(){ }

}//qor
