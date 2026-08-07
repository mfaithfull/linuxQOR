// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_STRUCTUREADAPTER
#define QOR_PP_H_FRAMEWORK_DIO_STRUCTUREADAPTER

namespace qor{ namespace dio{

    //Each of the Args should be a ValueAdapter
    template <typename... Args>
    struct StructureAdapter : public std::tuple<Args...>
    {
        using storage_type = std::tuple<Args...>;

        StructureAdapter(Args&&... args) : std::tuple<Args...>{std::forward<Args>(args)...}
        { }

        constexpr auto field_count() 
        {
            return std::tuple_size<std::tuple<Args...>>::value;
        }
        
        template<class _Callable>
        void for_each_field(_Callable&& fn)
        {
            storage_type& storage = *this;
            std::apply([&fn,this](const auto&... args){iterate(fn, args...);}, storage);
        }

    private:

        template<class _Callable>
        void iterate(_Callable&& fn, const auto&... args)
        {
            size_t index = 0;
            (fn(index, args), ...);
        }

    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_STRUCTUREADAPTER