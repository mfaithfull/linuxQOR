// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_MODEL_CODEPOINT
#define QOR_PP_H_DATA_JSON_MODEL_CODEPOINT

namespace qor { namespace data{ namespace model{ namespace json {

    class CodePoint
    {
    public:

        CodePoint() : m_value(0){ }
        virtual ~CodePoint() = default;

        int GetValue(){ return m_value; }
        void SetValue(int value){ m_value = value;}
        
    private:

        char32_t m_value;
    };

}}}}//qor::data::model::json

#endif//QOR_PP_H_DATA_JSON_MODEL_CODEPOINT
