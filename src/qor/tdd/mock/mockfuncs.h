// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_MOCKFUNCS
#define QOR_PP_H_TESTMOCK_MOCKFUNCS

namespace qor{ namespace mock{
        
    template <typename Z, typename Y>
    class mockFuncs : public mock<Z>
    {
    private:
        mockFuncs();
    public:

        template <int X, typename... Args>
        Y expectation(Args... args)
        {
            std::tuple<Args...> argT(args...);
            mock<Z>* realMock = mock<Z>::getRealThis();
            MockRepository* myRepo = realMock->repo;
            if (realMock->isZombie) 
            {
                std::stringstream argstr;
                printTuple(argstr, argT);
                MockRepoInstanceHolder<0>::reporter->FunctionCallToZombie(*myRepo, argstr.str());
            }
            return myRepo->template DoExpectation<Y>(realMock, realMock->translateX(X), argT);
        }

        template <int X, typename... Args>
        static Y static_expectation(Args... args)
        {
            std::tuple<Args...> argT(args...);
            return MockRepoInstanceHolder<0>::instance->template DoExpectation<Y>(nullptr, std::pair<int, int>(0, X), argT);
        }

#if (qor_pp_os_target == qor_pp_os_windows)
        
        template <int X, typename... Args>
        Y __stdcall stdcallexpectation(Args... args)
        {
            std::tuple<Args...> argT(args...);
            mock<Z>* realMock = mock<Z>::getRealThis();
            MockRepository* myRepo = realMock->repo;
            if (realMock->isZombie) 
            {
                std::stringstream argstr;
                printTuple(argstr, argT);
                MockRepoInstanceHolder<0>::reporter->FunctionCallToZombie(*myRepo, argstr.str());
            }
            return myRepo->template DoExpectation<Y>(realMock, realMock->translateX(X), argT);
        }

#if (qor_pp_os_target == qor_pp_os_windows) && !defined(qor_pp_os_64bit)
        
        template <int X, typename... Args>
        static Y __stdcall static_stdcallexpectation(Args... args)
        {
            std::tuple<Args...> argT(args...);
            return MockRepoInstanceHolder<0>::instance->template DoExpectation<Y>(nullptr, std::pair<int, int>(0, X), argT);
        }
#endif
#endif
    };

   
    template <typename Z>
    class mockFuncs<Z, void> : public mock<Z>
    {
    private:
        mockFuncs();
    public:
        
        template <int X, typename... Args>
        void expectation(Args... args)
        {
            std::tuple<Args...> argT(args...);
            mock<Z>* realMock = mock<Z>::getRealThis();
            MockRepository* myRepo = realMock->repo;
            if (realMock->isZombie) 
            {
                std::stringstream argstr;
                printTuple(argstr, argT);
                MockRepoInstanceHolder<0>::reporter->FunctionCallToZombie(*myRepo, argstr.str());
            }
            myRepo->DoVoidExpectation(realMock, realMock->translateX(X), argT);
        }
        
        template <int X, typename... Args>
        static void static_expectation(Args... args)
        {
            std::tuple<Args...> argT(args...);
            MockRepoInstanceHolder<0>::instance->DoVoidExpectation(nullptr, std::pair<int, int>(0, X), argT);
        }

#if (qor_pp_os_target == qor_pp_os_windows)
        
        template <int X, typename... Args>
        void __stdcall stdcallexpectation(Args... args)
        {
            std::tuple<Args...> argT(args...);
            mock<Z>* realMock = mock<Z>::getRealThis();
            MockRepository* myRepo = realMock->repo;
            if (realMock->isZombie) 
            {
                std::stringstream argstr;
                printTuple(argstr, argT);
                MockRepoInstanceHolder<0>::reporter->FunctionCallToZombie(*myRepo, argstr.str());
            }
            myRepo->DoVoidExpectation(this, mock<Z>::translateX(X), argT);
        }

#if (qor_pp_os_target == qor_pp_os_windows) && !defined(qor_pp_os_64bit)
        
        template <int X, typename... Args>
        static void __stdcall static_stdcallexpectation(Args... args)
        {
            std::tuple<Args...> argT(args...);
            return MockRepoInstanceHolder<0>::instance->DoVoidExpectation(nullptr, std::pair<int, int>(0, X), argT);
        }
#endif
#endif
    };

    }//mock
}//qor

#endif//QOR_PP_H_TESTMOCK_MOCKFUNCS
