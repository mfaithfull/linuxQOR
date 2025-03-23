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

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_MOCKREPOSITORY
#define QOR_PP_H_TESTMOCK_MOCKREPOSITORY

#include <iostream>
#include <vector>
#include <map>
#include <memory>

namespace qor{ namespace mock{

    class MockRepository
    {
    private:
        friend inline std::ostream& operator<<(std::ostream& os, const MockRepository& repo);
        std::vector<base_mock*> mocks;
#ifdef qor_pp_mock_cfuncsupport
        std::vector<std::unique_ptr<Replace>> staticReplaces;
#endif
        std::map<void (*)(), int> staticFuncMap;
    public:

        std::vector<std::unique_ptr<Call>> neverCalls;
        std::vector<std::unique_ptr<Call>> expectations;
        std::vector<std::unique_ptr<Call>> optionals;
        bool autoExpect;

    private:

        void addAutoExpectTo(Call* call)
        {
            if (autoExpect && expectations.size() > 0)
            {
                call->previousCalls.push_back(expectations.back().get());
            }
        }

        void addCall(Call* call, RegistrationType expect)
        {
            if (expect == Never) 
            {
                neverCalls.emplace_back(call);
            }
            else if (expect.minimum == expect.maximum)
            {
                addAutoExpectTo(call);
                expectations.emplace_back(call);
            }
            else
            {
                optionals.emplace_back(call);
            }
        }

    public:
#if (qor_pp_os_target == qor_pp_os_windows)
#ifdef qor_pp_mock_cfuncsupport
#define OnCallFunc(func) RegisterExpect_<__COUNTER__>(&func, qor::mock:: Any, #func, __FILE__, __LINE__)
#define ExpectCallFunc(func) RegisterExpect_<__COUNTER__>(&func, qor::mock:: Once, #func, __FILE__, __LINE__)
#define NeverCallFunc(func) RegisterExpect_<__COUNTER__>(&func, qor::mock:: Never, #func, __FILE__, __LINE__)
#define OnCallFuncOverload(func) RegisterExpect_<__COUNTER__>(func, qor::mock:: Any, #func, __FILE__, __LINE__)
#define ExpectCallFuncOverload(func) RegisterExpect_<__COUNTER__>(func, qor::mock:: Once, #func, __FILE__, __LINE__)
#define NeverCallFuncOverload(func) RegisterExpect_<__COUNTER__>(func, qor::mock:: Never, #func, __FILE__, __LINE__)
#endif
#define OnCall(obj, func) RegisterExpect_<__COUNTER__>(obj, &func, qor::mock:: Any, #func, __FILE__, __LINE__)
#define OnCalls(obj, func, minimum) RegisterExpect_<__COUNTER__>(obj, &func, qor::mock:: RegistrationType(minimum,(std::numeric_limits<unsigned>::max)()), #func, __FILE__, __LINE__)
#define ExpectCall(obj, func) RegisterExpect_<__COUNTER__>(obj, &func, qor::mock:: Once, #func, __FILE__, __LINE__)
#define ExpectCalls(obj, func, num) RegisterExpect_<__COUNTER__>(obj, &func, qor::mock:: RegistrationType(num,num), #func, __FILE__, __LINE__)
#define NeverCall(obj, func) RegisterExpect_<__COUNTER__>(obj, &func, qor::mock:: Never, #func, __FILE__, __LINE__)
#define OnCallOverload(obj, func) RegisterExpect_<__COUNTER__>(obj, func, qor::mock:: Any, #func, __FILE__, __LINE__)
#define ExpectCallOverload(obj, func) RegisterExpect_<__COUNTER__>(obj, func, qor::mock:: Once, #func, __FILE__, __LINE__)
#define NeverCallOverload(obj, func) RegisterExpect_<__COUNTER__>(obj, func, qor::mock:: Never, #func, __FILE__, __LINE__)
#define OnCallDestructor(obj) RegisterExpectDestructor<__COUNTER__>(obj, qor::mock:: Any, __FILE__, __LINE__)
#define ExpectCallDestructor(obj) RegisterExpectDestructor<__COUNTER__>(obj, qor::mock:: Once, __FILE__, __LINE__)
#define NeverCallDestructor(obj) RegisterExpectDestructor<__COUNTER__>(obj, qor::mock:: Never, __FILE__, __LINE__)
#else
#ifdef qor_pp_mock_cfuncsupport
#define OnCallFunc(func) RegisterExpect_<__LINE__>(&func, qor::mock:: Any, #func, __FILE__, __LINE__)
#define ExpectCallFunc(func) RegisterExpect_<__LINE__>(&func, qor::mock:: Once, #func, __FILE__, __LINE__)
#define NeverCallFunc(func) RegisterExpect_<__LINE__>(&func, qor::mock:: Never, #func, __FILE__, __LINE__)
#define OnCallFuncOverload(func) RegisterExpect_<__LINE__>(func, qor::mock:: Any, #func,  __FILE__, __LINE__)
#define ExpectCallFuncOverload(func) RegisterExpect_<__LINE__>(func, qor::mock:: Once, #func, __FILE__, __LINE__)
#define NeverCallFuncOverload(func) RegisterExpect_<__LINE__>(func, qor::mock:: Never, #func, __FILE__, __LINE__)
#endif
#define OnCall(obj, func) RegisterExpect_<__LINE__>(obj, &func, qor::mock:: Any, #func, __FILE__, __LINE__)
#define OnCalls(obj, func, minimum) RegisterExpect_<__LINE__>(obj, &func, qor::mock:: RegistrationType(minimum,(std::numeric_limits<unsigned>::max)()), #func, __FILE__, __LINE__)
#define ExpectCall(obj, func) RegisterExpect_<__LINE__>(obj, &func, qor::mock:: Once, #func, __FILE__, __LINE__)
#define ExpectCalls(obj, func, num) RegisterExpect_<__LINE__>(obj, &func, qor::mock:: RegistrationType(num,num), #func, __FILE__, __LINE__)
#define NeverCall(obj, func) RegisterExpect_<__LINE__>(obj, &func, qor::mock:: Never, #func, __FILE__, __LINE__)
#define OnCallOverload(obj, func) RegisterExpect_<__LINE__>(obj, func, qor::mock:: Any, #func, __FILE__, __LINE__)
#define ExpectCallOverload(obj, func) RegisterExpect_<__LINE__>(obj, func, qor::mock:: Once, #func, __FILE__, __LINE__)
#define NeverCallOverload(obj, func) RegisterExpect_<__LINE__>(obj, func, qor::mock:: Never, #func, __FILE__, __LINE__)
#define OnCallDestructor(obj) RegisterExpectDestructor<__LINE__>(obj, qor::mock:: Any, __FILE__, __LINE__)
#define ExpectCallDestructor(obj) RegisterExpectDestructor<__LINE__>(obj, qor::mock:: Once, __FILE__, __LINE__)
#define NeverCallDestructor(obj) RegisterExpectDestructor<__LINE__>(obj, qor::mock:: Never, __FILE__, __LINE__)
#endif

        template <typename A, class B, typename C>
        void Member(A* mck, C B::* member)
        {
            C A::* realMember = (C A::*)member;
            C* realRealMember = &(mck->*realMember);
            mock<A>* realMock = (mock<A> *)mck;
            realMock->members.emplace_back(new MemberWrap<C>(realRealMember));
        }

        template <int X, typename Z2>
        TCall<void>& RegisterExpectDestructor(Z2* mck, RegistrationType expect, const char* fileName, unsigned long lineNo);

#ifdef qor_pp_mock_cfuncsupport
        template <int X, typename Y, typename... Args>
        TCall<Y, Args...>& RegisterExpect_(Y(*func)(Args...), RegistrationType expect, const char* functionName, const char* fileName, unsigned long lineNo);

#if (qor_pp_os_target == qor_pp_os_windows) && !defined(qor_pp_os_64bit)
        template <int X, typename Y, typename... Args>
        TCall<Y, Args...>& RegisterExpect_(Y(__stdcall* func)(Args...), RegistrationType expect, const char* functionName, const char* fileName, unsigned long lineNo);
#endif
#endif

        template <int X, typename Z2, typename Y, typename Z, typename... Args>
        TCall<Y, Args...>& RegisterExpect_(Z2* mck, Y(Z::* func)(Args...), RegistrationType expect, const char* functionName, const char* fileName, unsigned long lineNo);
        
        
        template <int X, typename Z2, typename Y, typename Z, typename... Args>
        TCall<Y, Args...>& RegisterExpect_(Z2* mck, Y(Z::* func)(Args...) volatile, RegistrationType expect, const char* functionName, const char* fileName, unsigned long lineNo)
        {
            return RegisterExpect_<X>(mck, (Y(Z::*)(Args...))(func), expect, functionName, fileName, lineNo);
        }

        
        template <int X, typename Z2, typename Y, typename Z, typename... Args>
        TCall<Y, Args...>& RegisterExpect_(Z2* mck, Y(Z::* func)(Args...) const volatile, RegistrationType expect, const char* functionName, const char* fileName, unsigned long lineNo)
        {
            return RegisterExpect_<X>(mck, (Y(Z::*)(Args...))(func), expect, functionName, fileName, lineNo);
        }

        
        template <int X, typename Z2, typename Y, typename Z, typename... Args>
        TCall<Y, Args...>& RegisterExpect_(Z2* mck, Y(Z::* func)(Args...) const, RegistrationType expect, const char* functionName, const char* fileName, unsigned long lineNo)
        {
            return RegisterExpect_<X>(mck, (Y(Z::*)(Args...))(func), expect, functionName, fileName, lineNo);
        }

#if (qor_pp_os_target == qor_pp_os_windows) && !defined(qor_pp_os_64bit)
        // COM only support - you can duplicate this for cdecl and fastcall if you want to, but those are not as common as COM.
        template <int X, typename Z2, typename Y, typename Z, typename... Args>
        TCall<Y, Args...>& RegisterExpect_(Z2* mck, Y(__stdcall Z::* func)(Args...), RegistrationType expect, const char* functionName, const char* fileName, unsigned long lineNo);
        
        
        template <int X, typename Z2, typename Y, typename Z, typename... Args>
        TCall<Y, Args...>& RegisterExpect_(Z2* mck, Y(__stdcall Z::* func)(Args...) volatile, RegistrationType expect, const char* functionName, const char* fileName, unsigned long lineNo)
        {
            return RegisterExpect_<X>(mck, (Y(__stdcall Z::*)(Args...))(func), expect, functionName, fileName, lineNo);
        }

        
        template <int X, typename Z2, typename Y, typename Z, typename... Args>
        TCall<Y, Args...>& RegisterExpect_(Z2* mck, Y(__stdcall Z::* func)(Args...) const volatile, RegistrationType expect, const char* functionName, const char* fileName, unsigned long lineNo)
        {
            return RegisterExpect_<X>(mck, (Y(__stdcall Z::*)(Args...))(func), expect, functionName, fileName, lineNo);
        }

        
        template <int X, typename Z2, typename Y, typename Z, typename... Args>
        TCall<Y, Args...>& RegisterExpect_(Z2* mck, Y(__stdcall Z::* func)(Args...) const, RegistrationType expect, const char* functionName, const char* fileName, unsigned long lineNo)
        {
            return RegisterExpect_<X>(mck, (Y(__stdcall Z::*)(Args...))(func), expect, functionName, fileName, lineNo);
        }
#endif

        template <typename Z>
        void BasicRegisterExpect(mock<Z>* zMock, int baseOffset, int funcIndex, void (base_mock::* func)(), int X);

#ifdef qor_pp_mock_cfuncsupport
        
        int BasicStaticRegisterExpect(void (*func)(), void (*fp)(), int X)
        {
            if (staticFuncMap.find(func) == staticFuncMap.end())
            {
                staticFuncMap[func] = X;
                staticReplaces.emplace_back(new Replace(func, fp));
            }
            return staticFuncMap[func];
        }
#endif
        
        const char* funcName(base_mock* mock, std::pair<int, int> funcno)
        {
            for (auto& i : expectations)
                if (i->mock == mock &&
                    i->funcIndex == funcno)
                    return i->funcName;

            for (auto& i : optionals)
                if (i->mock == mock &&
                    i->funcIndex == funcno)
                    return i->funcName;

            for (auto& i : neverCalls)
                if (i->mock == mock &&
                    i->funcIndex == funcno)
                    return i->funcName;

            return nullptr;
        }

        template <typename Y, typename... Args>
        Y DoExpectation(base_mock* mock, std::pair<int, int> funcno, std::tuple<Args...>& tuple);

        
        template <typename... Args>
        inline void DoVoidExpectation(base_mock* mock, std::pair<int, int> funcno, std::tuple<Args...>& tuple)
        {
            for (auto& c : reverse_order(expectations)) 
            {
                if (!c->applies(mock, funcno)) continue;

                TCall<void, Args...>* tc = static_cast<TCall<void, Args...>*>(c.get());
                if (!tc->isSatisfied() && tc->matches(tuple)) {
                    tc->handle(tuple);
                    return;
                }
            }
            for (auto& c : reverse_order(neverCalls)) 
            {
                if (!c->applies(mock, funcno)) continue;

                TCall<void, Args...>* tc = static_cast<TCall<void, Args...>*>(c.get());
                if (tc->matches(tuple)) {
                    tc->handle(tuple);
                    return;
                }
            }
            for (auto& c : reverse_order(optionals)) 
            {
                if (!c->applies(mock, funcno)) continue;

                TCall<void, Args...>* tc = static_cast<TCall<void, Args...>*>(c.get());
                if (tc->matches(tuple)) {
                    tc->handle(tuple);
                    return;
                }
            }
            std::stringstream args;
            printTuple(args, tuple);
            MockRepoInstanceHolder<0>::reporter->NoExpectationMatches(*this, args.str(), funcName(mock, funcno));
            // We reported it, but we can return here since this is always a void expectation. 
        }

        
        MockRepository(Reporter* reporter = detail::GetDefaultReporter()) : autoExpect(qor_pp_mock_default_autoexpect)
        {
            MockRepoInstanceHolder<0>::instance = this;
            MockRepoInstanceHolder<0>::reporter = reporter;
            reporter->TestStarted();
        }

        
        ~MockRepository() noexcept(false)
        {
            if (!std::uncaught_exceptions())
            {
                try
                {
                    VerifyAll();
                }
                catch (...)
                {
                    reset();
                    for (auto& i : mocks)
                        i->destroy();
#ifdef qor_pp_mock_cfuncsupport
                    staticReplaces.clear();
#endif
                    Reporter* reporter = MockRepoInstanceHolder<0>::reporter;
                    MockRepoInstanceHolder<0>::instance = nullptr;
                    MockRepoInstanceHolder<0>::reporter = nullptr;
                    reporter->TestFinished();
                    throw;
                }
            }

            reset();
            for (auto& i : mocks)
                i->destroy();
#ifdef qor_pp_mock_cfuncsupport
            staticReplaces.clear();
#endif
            Reporter* reporter = MockRepoInstanceHolder<0>::reporter;
            MockRepoInstanceHolder<0>::instance = nullptr;
            MockRepoInstanceHolder<0>::reporter = nullptr;
            reporter->TestFinished();
        }

        
        void reset()
        {
            expectations.clear();
            neverCalls.clear();
            optionals.clear();
        }

        
        void VerifyAll()
        {
            for (auto& i : expectations)
            {
                if (!i->isSatisfied())
                {
                    MockRepoInstanceHolder<0>::reporter->CallMissing(*i, *this);
                }
            }
        }

        void VerifyPartial(base_mock* obj)
        {
            for (auto& i : expectations)
            {
                if (i->mock == (base_mock*)obj &&
                    !i->isSatisfied())
                {
                    MockRepoInstanceHolder<0>::reporter->CallMissing(*i, *this);
                }
            }
        }

        template <typename base> base* Mock();
    };

    inline std::ostream& operator<<(std::ostream& os, const MockRepository& repo)
    {
        if (repo.expectations.size())
        {
            os << "Expectations set:" << std::endl;
            for (auto& exp : repo.expectations)
                os << *exp;
            os << std::endl;
        }

        if (repo.neverCalls.size())
        {
            os << "Functions explicitly expected to not be called:" << std::endl;
            for (auto& nc : repo.neverCalls)
                os << *nc;
            os << std::endl;
        }

        if (repo.optionals.size())
        {
            os << "Optional results set up:" << std::endl;
            for (auto& opt : repo.optionals)
                os << *opt;
            os << std::endl;
        }
        return os;
    }

}}//qor::ext

#endif//QOR_PP_H_TESTMOCK_MOCKREPOSITORY
