// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_ADAPTER
#define QOR_PP_H_FRAMEWORK_DIO_ADAPTER

namespace qor{ namespace dio{

    //Bind an Adapter to fields of an instance of a domain object to copy them and make them enumerable
    //The interface interface_t becomes available on over the bound fields. 
    //Can be used for outbound conversions and mappings. Fields can be writable but the original object
    //copied from is unaffected.
    template<class interface_t, class storage, typename field_enum>
    struct Adapter : public interface_t, protected storage
    {            
        //Bind to a copy of the domain object on construction.
        template <typename... Args>
        Adapter(storage& src, Args&&... args) : storage(src), interface_t(args...)
        { }
    
        //accessor for fields
        template<field_enum f>
        auto& at() { return std::get<static_cast<int>(f)>(*this); }

        template<field_enum f>
        struct type_of_field
        {
            using type = std::invoke_result_t< decltype(&Adapter<interface_t, storage, field_enum>::template at<f>), Adapter<interface_t, storage, field_enum> >;
        };
    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_ADAPTER
