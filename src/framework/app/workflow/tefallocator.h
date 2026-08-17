// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_FASTFLOW_TEF_ALLOCATOR
#define QOR_PP_H_FRAMEWORK_FASTFLOW_TEF_ALLOCATOR

#include "src/qor/memory/reference/newref.h"
#include "src/qor/memory/instance/threadsingleton.h"
#include "src/qor/memory/thread/fastheap/fastheap.h"
#include "src/qor/memory/sources/fastsource.h"

namespace qor{

    class TEFAllocator 
    {
    private:

        ref_of<memory::FastHeap>::type m_fh;

    public:    

        TEFAllocator ()
        { 
             m_fh = new_ref<memory::FastHeap>();
        }

        void* Allocate(size_t size, size_t alignment) 
        {
            return m_fh->Allocate(size);
            /*
            // Count how many blocks this request needs.
            size_t required_blocks = (size + BlockSize - 1) / BlockSize;

            for (size_t block = 0; block < NumBlocks; ++block) 
            {
                // Find the first available set of contiguous blocks long enough for
                // our needs. Might not be enough after taking alignment into account.
                bool has_enough_contiguous_blocks = true;
                for (size_t i = 0; i < required_blocks; ++i) 
                {
                    if (blocks_used_.test(block + i)) 
                    {
                        has_enough_contiguous_blocks = false;
                        break;
                    }
                }

                if (!has_enough_contiguous_blocks)
                continue;

                // Take alignment into account, which might require additional blocks.
                uint8_t* start_ptr = buffer_.get() + block * BlockSize;
                size_t alignment_shift = (alignment - (reinterpret_cast<uintptr_t>(start_ptr) % alignment)) % alignment;
                start_ptr += alignment_shift;

                // Check if the required size puts us past the buffer size.
                uint8_t* end_ptr = start_ptr + size;
                size_t end_in_buffer = end_ptr - buffer_.get();
                if (end_in_buffer >= kBufferSize)
                return nullptr;

                // Check if any extra alignment requires more blocks, or leaves some empty
                // at the beginning.
                size_t start_block = (start_ptr - buffer_.get()) / BlockSize;
                size_t end_block = (end_in_buffer + BlockSize - 1) / BlockSize;
                for (size_t i = block + required_blocks; i < end_block; ++i) 
                {
                    if (blocks_used_.test(i)) 
                    {
                        has_enough_contiguous_blocks = false;
                        break;
                    }
                }

                // More blocks required by alignment, but none available.
                if (!has_enough_contiguous_blocks)
                continue;

                // Everything looks good, so mark blocks as used and return the pointer.
                for (size_t i = start_block; i < end_block; ++i)
                {
                    blocks_used_.set(i);
                }

                return start_ptr;
            }

            // No continuous set of blocks large enough available.
            return nullptr;
            */
        }

        bool Deallocate(void* address, size_t size, size_t alignment) 
        {
            m_fh->Free((byte*)address, size);
            return true;
        };
    };
}//qor

#endif//QOR_PP_H_FRAMEWORK_FASTFLOW_TEF_ALLOCATOR