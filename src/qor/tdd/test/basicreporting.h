// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TESTBASICREPORTING
#define QOR_PP_H_TESTBASICREPORTING

#include <list>
#include "reporting.h"

namespace qor { namespace test
{
    class BasicReporting : public Reporting
    {
		private:

			std::list<std::string> m_Failures;
            size_t m_fail{ 0 }, m_pass{ 0 };
            clock_t m_start{ 0 }, m_end{ 0 };
			
			void ReportSummary()
			{
				const size_t size = m_pass + m_fail;
				std::cout << std::endl;
				std::cout << "[TOTAL](" << size << '/' << size << ")" << std::endl;
				std::cout << "[OK](" << m_pass << '/' << size << ")" << std::endl;
				if (m_fail > 0)
				{
					std::cout << "[FAIL](" << m_fail << '/' << size << ")" << std::endl;
				}
				double duration = ((double)(m_end - m_start)) / CLOCKS_PER_SEC;
				std::cout << "[DURATION] " << duration << " seconds" << std::endl;
			}
			
			void ReportFailures()
			{
				if (m_Failures.size() > 0)
				{
					std::cout << "[ALL FAILURES]" << std::endl;
					std::list<std::string>::iterator it;
					for (it = m_Failures.begin(); it != m_Failures.end(); ++it)
					{
						std::cout << *it << std::endl;
					}
				}
			}
        
        public:

        BasicReporting() : m_fail(0), m_pass(0){}

        int FailCount()
        {
            return static_cast<int>(m_fail);
        }

        void TestRunStarting()
        {
            m_start = clock();
        }

        void TestCaseStarting(const TestCase& info)
        {
            std::cout << std::endl << info.m_At << info.m_Name << std::flush;
        }

        void TestCaseEnded(const TestCase& info)
        {
            if(info.m_Passed)
            {
                ++m_pass;
                //std::cout << " [OK]";                
            }
            else
            {                
                ++m_fail;
                std::cout << std::endl << info.m_At << info.m_Name << std::flush;
                std::cout << " [FAIL]";
                std::cout << ", #" << info.m_Index << std::flush;
            }        
        }

        void TestRunEnded()
        {
            m_end = clock();
            ReportSummary();
            ReportFailures();
        }

        void LogFailure(const std::string s)
        {
            m_Failures.emplace_back(s);
        }

    };

}}//qor::test

#endif//QOR_PP_H_TESTBASICREPORTING
