// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_PORT
#define QOR_PP_H_FRAMEWORK_DIO_PORT

namespace qor{ namespace dio{

    //Bind a port to the fields of a domain storage class instance to modify that instance
    //throught the bound interface.

    template<class interface_t, class storage, typename field_enum>
    struct Port : public interface_t
    {                
        template <typename... Args>
        Port(storage& src, Args&&... args) : m_(src), interface_t(args...)
        { }
    
        //accessor for fields
        template<field_enum f>
        auto& at() { return std::get<static_cast<int>(f)>(*this); }

        template<field_enum f>
        struct type_of_field
        {
            using type = std::invoke_result_t< decltype(&Port<interface_t, storage, field_enum>::template at<f>), Port<interface_t, storage, field_enum> >;
        };

    private:

        storage& m_;
    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_PORT
