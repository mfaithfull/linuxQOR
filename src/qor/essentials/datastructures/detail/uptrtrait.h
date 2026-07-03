// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

/***********************************************************************
*
* Copyright (c) 2015-2018 Barbara Geller
* Copyright (c) 2015-2018 Ansel Sermersheim
* All rights reserved.
*
* This file is part of libguarded
*
* libguarded is free software, released under the BSD 2-Clause license.
* For license details refer to LICENSE provided with this project.
*
***********************************************************************/

namespace detail
{
    // allocator-aware deleter for unique_ptr
    template <typename Alloc>
    class deallocator
    {
        using allocator_type   = Alloc;
        using allocator_traits = std::allocator_traits<allocator_type>;
        using pointer          = typename allocator_traits::pointer;

        allocator_type alloc;

    public:

        explicit deallocator(const allocator_type &alloc) noexcept
            : alloc(alloc){ }

        void operator()(pointer p) 
        {
            if (p != nullptr) {
                allocator_traits::destroy(alloc, p);
                allocator_traits::deallocate(alloc, p, 1);
            }
        }
    };

    // unique_ptr counterpart for std::allocate_shared()
    template <typename T, typename Alloc, typename... Args>
    std::unique_ptr<T, deallocator<Alloc>> allocate_unique(Alloc &alloc, Args &&... args)
    {
        using allocator_traits = std::allocator_traits<Alloc>;
        auto p = allocator_traits::allocate(alloc, 1);
        try 
        {
            allocator_traits::construct(alloc, p, std::forward<Args>(args)...);
            return {p, deallocator<Alloc>{alloc}};

        } 
        catch (...) 
        {
            allocator_traits::deallocate(alloc, p, 1);
            throw;
        }
    }

}//detail
