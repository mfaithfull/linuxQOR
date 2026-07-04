// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_MEMBERWRAPPER
#define QOR_PP_H_TESTMOCK_MEMBERWRAPPER

namespace qor{ namespace mock{
    
    template <typename A>
    class MemberWrap : public TypeDestructable
    {
    private:
        A* member;

    public:
        
        MemberWrap(A* mem) : member(mem)
        {
            new (member) A();
        }

        virtual ~MemberWrap()
        {
            member->~A();
        }
    };

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_MEMBERWRAPPER
