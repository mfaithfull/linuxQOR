// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TESTFACTORY
#define QOR_PP_H_TESTFACTORY

#include <iostream>
#include <sstream>
#include "basicreporting.h"
#include "failure.h"

namespace qor{ namespace test
{
    std::string const version { "0.5 based on YAFFUT 1.1" };

    class Factory final
    {
    public:
		
    	static Factory& Instance()
		{
			static Factory instance;
			return instance;
		}

        void Register(TestCase acase)
        {
            acase.m_Index = m_registeredCount++;
            m_Tests.emplace_back(acase);
        }

        int main(int argc, const char* argv[])
        {
            argc--;	argv++; //Skip executable name, arg 0
            std::cout << "QOR Test.\n" << "Version " << version << std::endl << std::flush;
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
    };

}}//qor::test

#endif//QOR_PP_H_TESTFACTORY