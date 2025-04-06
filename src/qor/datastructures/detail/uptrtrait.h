// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

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
        : alloc(alloc)
    {
    }

    void operator()(pointer p) {
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

    try {
        allocator_traits::construct(alloc, p, std::forward<Args>(args)...);
        return {p, deallocator<Alloc>{alloc}};

    } catch (...) {
        allocator_traits::deallocate(alloc, p, 1);
        throw;
    }
    }

}//detail
