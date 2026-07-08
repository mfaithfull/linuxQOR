// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_MODEL_INTEGER
#define QOR_PP_H_COMPONENTS_JSON_MODEL_INTEGER

namespace qor { namespace components{ namespace model{ namespace json {

    class Integer
    {
    public:

        Integer() : m_value(0){ }
        virtual ~Integer() = default;

        int GetValue(){ return m_value; }
        void SetValue(int value){ m_value = value;}
        
    private:

        int m_value;        
    };

}}}}//qor::components::model::json

#endif//QOR_PP_H_COMPONENTS_JSON_MODEL_INTEGER
