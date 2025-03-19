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

#ifndef QOR_PP_H_TESTFACTORY
#define QOR_PP_H_TESTFACTORY

#include <iostream>
#include <sstream>
#include "basicreporting.h"
#include "failure.h"

namespace qor{ namespace test
{
    std::string const version { "0.1 based on YAFFUT 1.1" };


    class Factory final
    {
    private:

        std::list<TestCase> m_Tests;
        int m_registeredCount;

        Factory() : m_registeredCount(0) 
        {
        }

        static bool EqualsSuiteName(std::string const& name, std::string const& s)
        {
            return (s.find(name) != std::string::npos);
        }
        
    public:
		
    	static Factory& Instance()
		{
			static Factory instance;
			return instance;
		}

        int main(int argc, const char* argv[])
        {
            argc--;	argv++; //Skip executable name, arg 0
            std::cout << "QOR Test - Derived from YAFFUT.\n" << "Version " << version << std::endl << std::flush;
            BasicReporting reporter;
            ExecuteTests(argv,argc,reporter);
            return reporter.FailCount();
        }

        void ExecuteTests(const char** args, int argumentcount, Reporting& reporter)
        {
            reporter.TestRunStarting();
            argumentcount == 0 ? ExecuteAllTests(reporter) : ExecuteMatchingTests(args, argumentcount, reporter);
            reporter.TestRunEnded();
        }

        void ExecuteAllTests( Reporting& reporter)
        {
            struct PredicateAll
            {
                bool operator()(const TestCase&) {return true;}
            };
            PredicateAll matchAll;
            Run(matchAll, reporter);
        }

        void ExecuteMatchingTests(const char** args, int argumentcount, Reporting& reporter)
        {
            for (int i = 0; i < argumentcount; ++i)
            {
                int n;
                if ((std::istringstream(args[i]) >> n).fail())
                {
                    ExecuteMatchOnName(args[i], reporter);
                }
                else
                {
                    ExecuteMatchOnIndex(n, reporter);
                }
            }
        }

        void ExecuteMatchOnName(const char* name, Reporting& reporter)
        {
            struct PredicateName
            {
                std::string m_Name;

                PredicateName(std::string s) : m_Name(s) {}
                
                bool operator()(const TestCase& info)
                {
                    return (info.m_Name == m_Name || EqualsSuiteName(m_Name, info.m_Name));
                }
            };

            PredicateName pred(name);
            Run(pred, reporter);
        }

        void ExecuteMatchOnIndex(int index, Reporting& reporter)
        {
            struct PredicateIndex
            {
                int searchIndex;
                
                PredicateIndex(int index) :searchIndex(index) {}

                bool operator()(const TestCase& info)
                {
                    return (info.m_Index == searchIndex);
                }
            };

            PredicateIndex pred(index);
            Run(pred, reporter);
        }

        template <typename Predicate>
        void Run(Predicate& canRun, Reporting& reporter)
        {
            for (std::list<TestCase>::iterator it = m_Tests.begin(); it != m_Tests.end(); ++it)
            {
                TestCase info = *it;
                if (canRun(info))
                {
                    RunTestCase(info, reporter);
                }
            }
        }

        void RunTestCase(TestCase& info, Reporting& reporter)
        {
            try
            {
                reporter.TestCaseStarting(info);
                info.m_Func(); //executes the test 
                info.m_Passed = true;
            }
            catch (const qor::test::failure& f)
            {
                reporter.LogFailure(f.what());
            }
            catch (...)
            {
                reporter.LogFailure("unknown exception");
            }
            reporter.TestCaseEnded(info);
        }

    };

}}//qor::test

#endif//QOR_PP_H_TESTFACTORY