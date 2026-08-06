// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DIO_COLLECTIONADAPTERS_TOSTRINGCOLLECTION
#define QOR_PP_H_FRAMEWORK_DIO_COLLECTIONADAPTERS_TOSTRINGCOLLECTION

#include <string>

namespace qor{ namespace dio{

    template<typename range_t>
    struct ToStringCollection
    {
        ToStringCollection(range_t& r) : m_(r)
        { }

        std::string toString() const 
        { 
            std::string result;
            size_t index = 0;
            if constexpr( std::is_same_v< typename range_t::value_type, std::string > )
            {
                for( typename range_t::value_type t : m_ )
                {
                    if(index++ != 0)
                    {
                        result.append(", ");
                    }
                    result.append(t);
                }
            }
            else
            {
                for( typename range_t::value_type t : m_ )
                {
                    if(index++ != 0)
                    {
                        result.append(", ");
                    }
                    result.append(std::to_string(t));
                }
            }
            return result;
        }

    private:
        range_t& m_;
    };

}}//qor::dio

#endif//QOR_PP_H_FRAMEWORK_DIO_COLLECTIONADAPTERS_TOSTRINGCOLLECTION


