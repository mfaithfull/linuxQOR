// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_COMSUPPORT
#define QOR_PP_H_TESTMOCK_COMSUPPORT

#if (qor_pp_os_target == qor_pp_os_windows)

#   include <winerror.h>
#   include <guiddef.h>
#   include <Unknwnbase.h>

namespace qor{ namespace mock{

    template <typename T>
    void AddComExpectations(MockRepository& mocks, T* m)
    {
        mocks.OnCall(m, T::AddRef)
            .Return(1);
        mocks.OnCall(m, T::Release)
            .Return(1);
        mocks.OnCallOverload(m, (long(__stdcall T::*)(const _GUID&, void**)) & T::QueryInterface)
            .With(__uuidof(T), Out((void**)m))
            .Return(S_OK);

        mocks.OnCallOverload(m, (long(__stdcall T::*)(const IID&, void**)) & T::QueryInterface)
            .With(__uuidof(IUnknown), Out((void**)m))
            .Return(S_OK);

    }

    template <typename T1, typename T2>
    void ConnectComInterfaces(MockRepository& mocks, T1* m1, T2* m2)
    {
        //from T1 to T2
        mocks.OnCallOverload(m1, (long(__stdcall T1::*)(const _GUID&, void**)) & T1::QueryInterface)
            .With(__uuidof(T2), Out((void**)m2))
            .Return(S_OK);
        //from T2 to T1
        mocks.OnCallOverload(m2, (long(__stdcall T2::*)(const _GUID&, void**)) & T2::QueryInterface)
            .With(__uuidof(T1), Out((void**)m1))
            .Return(S_OK);

        AddComExpectations(mocks, m1);
        AddComExpectations(mocks, m2);

        //no support for interface hierarchies
        //no Base IUnknown -> do it yourself if you really need that special case
    }

}}//qor::mock

#endif

#endif//QOR_PP_H_TESTMOCK_COMSUPPORT
