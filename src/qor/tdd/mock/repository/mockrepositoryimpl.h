// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_MOCKREPOSITORYIMPL
#define QOR_PP_H_TESTMOCK_MOCKREPOSITORYIMPL

namespace qor{ namespace mock{

    template <typename Z>
    void MockRepository::BasicRegisterExpect(mock<Z>* zMock, int baseOffset, int funcIndex, void (base_mock::* func)(), int X)
    {
        if (funcIndex > qor_pp_mock_virt_func_limit) MockRepoInstanceHolder<0>::reporter->InvalidFuncIndex(funcIndex, *this);
        if ((unsigned int)baseOffset * sizeof(void*) + sizeof(void*) - 1 > sizeof(Z)) MockRepoInstanceHolder<0>::reporter->InvalidBaseOffset(baseOffset, *this);
        if (zMock->funcMap.find(std::make_pair(baseOffset, funcIndex)) == zMock->funcMap.end())
        {
            if (zMock->funcTables.find(baseOffset) == zMock->funcTables.end())
            {
                typedef void (*funcptr)();
                funcptr* funcTable = new funcptr[qor_pp_mock_virt_func_limit + 4] + 2;
                memcpy(funcTable, zMock->notimplementedfuncs, sizeof(funcptr) * qor_pp_mock_virt_func_limit);

                ((void**)funcTable)[-1] = zMock->rttiinfo.get();
                ((size_t*)funcTable)[-2] = baseOffset * sizeof(void*);

                ((void**)funcTable)[qor_pp_mock_virt_func_limit] = zMock;
                zMock->funcTables[baseOffset] = funcTable;
                ((void**)zMock)[baseOffset] = funcTable;
            }
            zMock->funcMap[std::make_pair(baseOffset, funcIndex)] = X + 1;
            zMock->funcTables[baseOffset][funcIndex] = getNonvirtualMemberFunctionAddress<void (*)()>(func);
        }
    }

    template <int X, typename Z2>
    TCall<void>& MockRepository::RegisterExpectDestructor(Z2* mck, RegistrationType expect, const char* fileName, unsigned long lineNo)
    {
        func_index idx;
        ((Z2*)&idx)->~Z2();
        int funcIndex = idx.lci * function_stride + function_base;
        void (mock<Z2>:: * member)(int);
        member = &mock<Z2>::template mockedDestructor<X>;
        BasicRegisterExpect(reinterpret_cast<mock<Z2> *>(mck),
            0, funcIndex,
            reinterpret_cast<void (base_mock::*)()>(member), X);
#ifdef qor_pp_compiler_extra_destructor
        BasicRegisterExpect(reinterpret_cast<mock<Z2> *>(mck),
            0, funcIndex + 1,
            reinterpret_cast<void (base_mock::*)()>(member), X);
#endif
        TCall<void>* call = new TCall<void>(Once, reinterpret_cast<base_mock*>(mck), std::pair<int, int>(0, funcIndex), lineNo, "destructor", fileName);
        addCall(call, expect);
        return *call;
    }

#if (qor_pp_os_target == qor_pp_os_windows) && !defined(qor_pp_os_64bit)

    // Support for COM, see declarations
    template <int X, typename Z2, typename Y, typename Z, typename... Args>
    TCall<Y, Args...>& MockRepository::RegisterExpect_(Z2* mck, Y(__stdcall Z::* func)(Args...), RegistrationType expect, const char* funcName, const char* fileName, unsigned long lineNo)
    {
        std::pair<int, int> funcIndex = virtual_index((Y(__stdcall Z2::*)(Args...))func);
        Y(__stdcall mockFuncs<Z2, Y>:: * mfp)(Args...);
        mfp = &mockFuncs<Z2, Y>::template stdcallexpectation<X, Args...>;
        BasicRegisterExpect(reinterpret_cast<mock<Z2> *>(mck),
            funcIndex.first,
            funcIndex.second,
            reinterpret_cast<void (base_mock::*)()>(mfp), X);
        TCall<Y, Args...>* call = new TCall<Y, Args...>(expect, reinterpret_cast<base_mock*>(mck), funcIndex, lineNo, funcName, fileName);

        addCall(call, expect);
        return *call;
    }
#endif

#ifdef qor_pp_mock_cfuncsupport

    template <int X, typename Y, typename... Args>
    TCall<Y, Args...>& MockRepository::RegisterExpect_(Y(*func)(Args...), RegistrationType expect, const char* funcName, const char* fileName, unsigned long lineNo)
    {
        Y(*fp)(Args...);
        fp = &mockFuncs<char, Y>::template static_expectation<X, Args...>;
        int index = BasicStaticRegisterExpect(reinterpret_cast<void (*)()>(func), reinterpret_cast<void (*)()>(fp), X);
        TCall<Y, Args...>* call = new TCall<Y, Args...>(expect, nullptr, std::pair<int, int>(0, index), lineNo, funcName, fileName);
        addCall(call, expect);
        return *call;
    }

#if (qor_pp_os_target == qor_pp_os_windows) && !defined(qor_pp_os_64bit)

    template <int X, typename Y, typename... Args>
    TCall<Y, Args...>& MockRepository::RegisterExpect_(Y(__stdcall* func)(Args...), RegistrationType expect, const char* funcName, const char* fileName, unsigned long lineNo)
    {
        Y(__stdcall * fp)(Args...);
        fp = &mockFuncs<char, Y>::template static_stdcallexpectation<X, Args...>;
        int index = BasicStaticRegisterExpect(reinterpret_cast<void (*)()>(func), reinterpret_cast<void (*)()>(fp), X);
        TCall<Y, Args...>* call = new TCall<Y, Args...>(expect, nullptr, std::pair<int, int>(0, index), lineNo, funcName, fileName);
        addCall(call, expect);
        return *call;
    }
#endif
#endif

    template <int X, typename Z2, typename Y, typename Z, typename... Args>
    TCall<Y, Args...>& MockRepository::RegisterExpect_(Z2* mck, Y(Z::* func)(Args...), RegistrationType expect, const char* functionName, const char* fileName, unsigned long lineNo)
    {
        std::pair<int, int> funcIndex = virtual_index((Y(Z2::*)(Args...))func);
        Y(mockFuncs<Z2, Y>:: * mfp)(Args...);
        mfp = &mockFuncs<Z2, Y>::template expectation<X, Args...>;
        BasicRegisterExpect(reinterpret_cast<mock<Z2> *>(mck),
            funcIndex.first, funcIndex.second,
            reinterpret_cast<void (base_mock::*)()>(mfp), X);
        TCall<Y, Args...>* call = new TCall<Y, Args...>(expect, reinterpret_cast<base_mock*>(mck), funcIndex, lineNo, functionName, fileName);

        addCall(call, expect);
        return *call;
    }

    template <typename Y, typename... Args>
    Y MockRepository::DoExpectation(base_mock* mock, std::pair<int, int> funcno, std::tuple<Args...>& tuple)
    {
        for (auto& c : reverse_order(expectations))
        {
            if (!c->applies(mock, funcno)) continue;

            TCall<Y, Args...>* tc = static_cast<TCall<Y, Args...>*>(c.get());
            if (!tc->isSatisfied() && tc->matches(tuple)) {
                return tc->handle(tuple);
            }
        }
        for (auto& c : reverse_order(neverCalls))
        {
            if (!c->applies(mock, funcno)) continue;

            TCall<Y, Args...>* tc = static_cast<TCall<Y, Args...>*>(c.get());
            if (tc->matches(tuple)) {
                return tc->handle(tuple);
            }
        }
        for (auto& c : reverse_order(optionals))
        {
            if (!c->applies(mock, funcno)) continue;

            TCall<Y, Args...>* tc = static_cast<TCall<Y, Args...>*>(c.get());
            if (tc->matches(tuple)) {
                return tc->handle(tuple);
            }
        }
        std::stringstream args;
        printTuple(args, tuple);
        MockRepoInstanceHolder<0>::reporter->NoExpectationMatches(*this, args.str(), funcName(mock, funcno));
        // If this did not throw an exception or somehow got me out of here, crash.
        std::terminate();
    }

    template <typename base>
    base* MockRepository::Mock() 
    {
        mock<base>* m = new mock<base>(this);
        mocks.push_back(m);
        return reinterpret_cast<base*>(m);
    }

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_MOCKREPOSITORYIMPL
