// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_MODEL_VALUEMODEL
#define QOR_PP_H_PROTOCOLS_YAML_MODEL_VALUEMODEL

namespace qor { namespace protocols { namespace model { namespace yaml {

    //value = scalar / sequence / mapping
    enum class ValueType
    {
        _null,
        _true,
        _false,
        number,
        string,
        sequence,
        mapping
    };

    class ValueModel
    {
    public:

        virtual ~ValueModel() = default;

        virtual ValueType GetType()
        {
            return ValueType::_null;
        }
    };

}}}}//qor::protocols::model::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_MODEL_VALUEMODEL