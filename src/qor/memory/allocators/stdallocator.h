// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_STDALLOCATOR
#define QOR_PP_H_STDALLOCATOR

#include <memory>
#include "../memory.h"
#include "stddeleter.h"

namespace qor{

	template <class _Ty>
	class qor_std_allocator 
	{
    public:
        static_assert(!std::is_const_v<_Ty>, "The C++ Standard forbids containers of const elements "
            "because allocator<const T> is ill-formed.");
        static_assert(!std::is_function_v<_Ty>, "The C++ Standard forbids allocators for function elements "
            "because of [allocator.requirements].");
        static_assert(!std::is_reference_v<_Ty>, "The C++ Standard forbids allocators for reference elements "
            "because of [allocator.requirements].");

        using _From_primary = qor_std_allocator;
        using value_type = _Ty;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using propagate_on_container_move_assignment = std::true_type;
        using is_always_equal = std::true_type;

        constexpr qor_std_allocator() noexcept {}

        constexpr qor_std_allocator(const qor_std_allocator&) noexcept = default;

        template <class _Other>
        constexpr qor_std_allocator(const qor_std_allocator<_Other>&) noexcept {}

        constexpr ~qor_std_allocator() = default;
        constexpr qor_std_allocator& operator=(const qor_std_allocator&) = default;

        constexpr void deallocate(_Ty* const _Ptr, const size_t _Count) 
        {
            _STL_ASSERT(_Ptr != nullptr || _Count == 0, "null pointer cannot point to a block of non-zero size");
            // no overflow check on the following multiply; we assume _Allocate did that check
            allocator_of<_Ty>::type::Free(_Ptr);//<_Ty>
            //_Deallocate<_New_alignof<_Ty>>(_Ptr, sizeof(_Ty) * _Count);
        }

        [[nodiscard]] constexpr qor_pp_allocator _Ty* allocate(/*_CRT_GUARDOVERFLOW*/ const size_t _Count) 
        {
            static_assert(sizeof(value_type) > 0, "value_type must be complete before calling allocate.");
            return static_cast<_Ty*>(
                allocator_of<_Ty>::type::Allocate(sizeof(_Ty), _Count)//<_Ty>
                //_Allocate<_New_alignof<_Ty>>(_Get_size_of_n<sizeof(_Ty)>(_Count))
                );
        }

#if _HAS_CXX23
        _NODISCARD_RAW_PTR_ALLOC constexpr std::allocation_result<_Ty*> allocate_at_least( _CRT_GUARDOVERFLOW const size_t _Count) 
        {
            return { allocate(_Count), _Count };
        }
#endif // _HAS_CXX23

        //static constexpr size_t _Minimum_asan_allocation_alignment = _Asan_granularity;
	};

	struct qor_std_dbgInfo
	{
		size_t Count;
		size_t Size;
		uint32_t uiLine;
		const char* szFile;
		void* pBackRef;
	};

	template <class _Ty, class... _Types, std::enable_if_t<!std::is_array_v<_Ty>, int> = 0>
	std::unique_ptr<_Ty, qor_std_deleter<_Ty>> make_qor_unique(_Types&&... _Args) 
	{
		
		_Ty* pResult = allocator_of<_Ty>::type::template Allocate<_Ty>((size_t)1, std::forward<_Types>(_Args)...);
		return std::unique_ptr< _Ty, qor_std_deleter<_Ty > >(pResult);
	}

	template <class _Ty, std::enable_if_t<std::is_array_v<_Ty>&& std::extent_v<_Ty> == 0, int> = 0>
	std::unique_ptr<_Ty, qor_std_deleter<_Ty>> make_qor_unique(const size_t _Size)
	{
		using _Elem = std::remove_extent_t<_Ty>;
		return std::unique_ptr<_Ty, qor_std_deleter<_Ty>>(source_of<_Ty>::type::Source(_Size * sizeof(_Ty)));//<_Elem>
	}

	template <class _Ty, class... _Types, std::enable_if_t<std::extent_v<_Ty> != 0, int> = 0>
	void make_qor_unique(_Types&&...) = delete;

}//qor

#endif//QOR_PP_H_STDALLOCATOR
