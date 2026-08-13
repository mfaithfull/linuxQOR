// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_ABSTRACT_CONTEXT
#define QOR_PP_H_DATA_ABSTRACT_CONTEXT

namespace qor{ namespace data{

    class AbstractDataContext
    {
    public:
                
        virtual bool RejectItem() = 0;
        virtual bool ConsumeItem() = 0;
        virtual size_t GetPosition() = 0;
        virtual bool HasData() = 0;
        virtual bool HasSpace() = 0;
        virtual void Reset() = 0;
     };

}}//qor::data

#endif//QOR_PP_H_DATA_ABSTRACT_CONTEXT
