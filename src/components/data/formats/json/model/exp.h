// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_MODEL_EXP
#define QOR_PP_H_DATA_JSON_MODEL_EXP

namespace qor { namespace data{ namespace model{ namespace json {

    class Exp
    {
    public:

        Exp() : m_value(0){ }
        virtual ~Exp() = default;

        int GetValue(){ return m_value; }
        void SetValue(int value){ m_value = value;}
        
    private:

        int m_value;        

    };

}}}}//qor::data::model::json

#endif//QOR_PP_H_DATA_JSON_MODEL_EXP
