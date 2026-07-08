// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_MODEL_VALUEMODEL
#define QOR_PP_H_COMPONENTS_JSON_MODEL_VALUEMODEL

namespace qor { namespace components{ namespace model{ namespace json {

    //value = false / null / true / object / array / number / string
    enum class qor_pp_module_interface(QOR_JSON) ValueType
    {
        _false,
        _null,
        _true,
        number,
        string,
        array,
        object
    };

    class ValueModel
    {
    public:        

        ValueModel(){ }
        virtual ~ValueModel() = default;

        virtual ValueType GetType() = 0;
    };

}}}}//qor::components::model::json

#endif//QOR_PP_H_COMPONENTS_JSON_MODEL_VALUEMODEL
