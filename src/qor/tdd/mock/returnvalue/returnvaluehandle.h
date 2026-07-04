// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_RETURNVALUEHANDLE
#define QOR_PP_H_TESTMOCK_RETURNVALUEHANDLE

namespace qor{ namespace mock{
    
    template <typename T>
    class ReturnValueHandle
    {
    public:

        std::unique_ptr<ReturnValueWrapper<T>> wrapper;

        void operator=(ReturnValueWrapper<T>* newValue)
        {
            wrapper.reset(newValue);
        }

        T value() 
        {
            return wrapper->value();
        }

        bool set() 
        {
            return wrapper != nullptr;
        }
    };

    template <>
    class ReturnValueHandle<void>
    {
    public:

        void value(){}

        bool set() 
        { 
            return true; 
        }
    };

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_RETURNVALUEHANDLE
