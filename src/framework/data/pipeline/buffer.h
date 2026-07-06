// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_BUFFER
#define QOR_PP_H_PIPELINE_BUFFER

#include "src/platform/compiler/compiler.h"

namespace qor{ namespace pipeline{

    //This provides most of the logic for simple buffer management without any actual allocation
    //Derive a class from it to add actual memory to the buffer in unit sizes.
    //As this is intended for a pipeline it presumes one writer and one reader that follow one
    //another through the buffer.
    //Both Writes and reads are three step, they reserve space, then attempt to read or write it
    //Then acknowledge how much they have read or written.
    //This allows for failure and unreliable device interaction without courrupting the buffer
    class qor_pp_module_interface(QOR_PIPELINE) Buffer
    {
    public:

        Buffer(size_t unitSize = 1, size_t itemCount = 0);
        Buffer(const Buffer& src);
        virtual ~Buffer();
        Buffer& operator = (const Buffer& src);

        virtual size_t WriteCapacity() const;
        virtual size_t ReadCapacity() const;
        virtual byte* WriteRequest(size_t& itemCount);
        virtual byte* ReadRequest(size_t& itemCount);
        virtual void SetCapacity(size_t itemCount);
        virtual void Reset(size_t itemCount = 0);
        size_t Capacity() const;
        virtual size_t ReadAcknowledge(size_t& itemCount);
        virtual size_t ReadReject(size_t& itemCount);
        virtual size_t WriteAcknowledge(size_t& itemCount);
        size_t GetUnitSize() const;

    protected:

        void SetUnitSize(size_t unitSize);

        size_t m_allocationCount;   //Tracks number of items of m_unitSize we have space for
        size_t m_unitSize;
        size_t m_readBegin;
        size_t m_readEnd;
        size_t m_writeBegin;
        size_t m_writeEnd;

    };

}}//qor::pipeline

#endif//QOR_PP_H_PIPELINE_BUFFER
