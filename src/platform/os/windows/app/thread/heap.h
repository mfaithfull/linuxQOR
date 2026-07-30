// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_APP_THREAD_HEAP
#define QOR_PP_H_OS_WINDOWS_APP_THREAD_HEAP

#include "src/platform/compiler/compiler.h"
#include "src/platform/os/windows/common/handles/handle.h"

namespace qor{ namespace win{

    class qor_pp_module_interface(QOR_WINDOWSTHREAD) ThreadHeap
    {
    public:
        
        struct Entry
        {
            void* lpData;
            unsigned long cbData;
            byte cbOverhead;
            byte iRegionIndex;
            unsigned short wFlags;
            union {
                struct {
                    void* hMem;
                    unsigned long dwReserved[ 3 ];
                } Block;
                struct {
                    unsigned long dwCommittedSize;
                    unsigned long dwUnCommittedSize;
                    void* lpFirstBlock;
                    void* lpLastBlock;
                } Region;
            };
        };

        enum class HeapInformation
        {
            CompatibilityInformation = 0,
            EnableTerminationOnCorruption = 1,
            OptimizeResources = 3,
            Tag = 7
        };

        ThreadHeap();
        virtual ~ThreadHeap() noexcept;
       
		byte* Alloc(unsigned long flags, size_t bytes) const;
		size_t Compact(unsigned long flags);
		bool Free(unsigned long flags, byte* mem) const;
		bool Lock();
		bool QueryInformation(HeapInformation informationClass, void* information, size_t informationLength, size_t& ReturnLength);
		byte* ReAlloc(unsigned long flags, byte* mem, size_t bytes);
		bool SetInformation(HeapInformation informationClass, void* information, size_t informationLength);
		size_t Size(unsigned long flags, const byte* mem);
		bool Unlock();
		bool Validate(unsigned long flags, const byte* mem);
		bool Walk(Entry* entry);

    private:

        platform::win::Handle m_handle;
    };

}}//qor::win

#endif//QOR_PP_H_OS_WINDOWS_APP_PROCESS_HEAP
