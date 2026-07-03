// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FUNCTIONPROFILER
#define QOR_PP_H_FUNCTIONPROFILER

#include <chrono>
#include "src/platform/compiler/compiler.h"
#include "src/qor/tdd/profiling/iprofilereceiver.h"

namespace qor {
    
	class FunctionProfiler final
	{
	public:

		FunctionProfiler(IProfileReceiver* receiver, bool enabled)  : m_profileEnabled(enabled), m_startTime(std::chrono::high_resolution_clock::now()),m_Receviver(receiver){ };
		
        ~FunctionProfiler()
        {
            if (m_profileEnabled && m_Receviver)
            {
                m_Receviver->Profile( std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_startTime), nullptr);
            }
        }    

	private:
		bool m_profileEnabled;
		std::chrono::high_resolution_clock::time_point m_startTime;
		IProfileReceiver* m_Receviver;
	};

}//qor

#endif//QOR_PP_H_FUNCTIONPROFILER
