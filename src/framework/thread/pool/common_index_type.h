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

#ifndef QOR_PP_H_FRAMEWORK_THREADPOOL_COMMON_IDX_TYPE
#define QOR_PP_H_FRAMEWORK_THREADPOOL_COMMON_IDX_TYPE

namespace qor { namespace framework{

    //A meta-programming template to determine the common type of two integer types. Unlike `std::common_type`, this template maintains correct signedness.
    //Enable A dummy parameter to enable SFINAE in specializations.
    template <typename T1, typename T2, typename Enable = void>
    struct common_index_type
    {
        using type = std::common_type_t<T1, T2>;        // Fallback to `std::common_type_t` if no specialization matches.
    };

    // The common type of two signed integers is the larger of the integers, with the same signedness.
    template <typename T1, typename T2>
    struct common_index_type<T1, T2, std::enable_if_t<std::is_signed_v<T1> && std::is_signed_v<T2>>>
    {
        using type = std::conditional_t<(sizeof(T1) >= sizeof(T2)), T1, T2>;
    };

    // The common type of two unsigned integers is the larger of the integers, with the same signedness.
    template <typename T1, typename T2>
    struct common_index_type<T1, T2, std::enable_if_t<std::is_unsigned_v<T1> && std::is_unsigned_v<T2>>>
    {
        using type = std::conditional_t<(sizeof(T1) >= sizeof(T2)), T1, T2>;
    };

    // The common type of a signed and an unsigned integer is a signed integer that can hold the full ranges of both integers.
    template <typename T1, typename T2>
    struct common_index_type<T1, T2, std::enable_if_t<(std::is_signed_v<T1> && std::is_unsigned_v<T2>) || (std::is_unsigned_v<T1> && std::is_signed_v<T2>)>>
    {
        using S = std::conditional_t<std::is_signed_v<T1>, T1, T2>;
        using U = std::conditional_t<std::is_unsigned_v<T1>, T1, T2>;
        static constexpr std::size_t larger_size = (sizeof(S) > sizeof(U)) ? sizeof(S) : sizeof(U);
        using type = std::conditional_t<larger_size <= 4,
            // If both integers are 32 bits or less, the common type should be a signed type that can hold both of them. If both are 8 bits, or the signed type is 16 bits and the unsigned type is 8 bits, the common type is `std::int16_t`. Otherwise, if both are 16 bits, or the signed type is 32 bits and the unsigned type is smaller, the common type is `std::int32_t`. Otherwise, if both are 32 bits or less, the common type is `std::int64_t`.
            std::conditional_t<larger_size == 1 || (sizeof(S) == 2 && sizeof(U) == 1), std::int16_t, std::conditional_t<larger_size == 2 || (sizeof(S) == 4 && sizeof(U) < 4), std::int32_t, std::int64_t>>,
            // If the unsigned integer is 64 bits, the common type should also be an unsigned 64-bit integer, that is, `std::uint64_t`. The reason is that the most common scenario where this might happen is where the indices go from 0 to `x` where `x` has been previously defined as `std::size_t`, e.g. the size of a vector. Note that this will fail if the first index is negative; in that case, the user must cast the indices explicitly to the desired common type. If the unsigned integer is not 64 bits, then the signed integer must be 64 bits, hence the common type is `std::int64_t`.
            std::conditional_t<sizeof(U) == 8, std::uint64_t, std::int64_t>>;
    };

    //A helper type alias to obtain the common type from the template `common_index_type`.
    template <typename T1, typename T2>
    using common_index_type_t = typename common_index_type<T1, T2>::type;

}}//qor::framework

#endif//QOR_PP_H_FRAMEWORK_THREADPOOL_COMMON_IDX_TYPE
