// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_MODEL_FRACTION
#define QOR_PP_H_COMPONENTS_JSON_MODEL_FRACTION

namespace qor { namespace components{ namespace model{ namespace json {

    class Fraction
    {
    public:

        Fraction(){}
        virtual ~Fraction() = default;

        double GetValue() const { return m_value;}
        void SetValue(const double& value) { m_value = value;}
 
    private:

        double m_value{ 0.0 };
    };

}}}}//qor::components::model::json

#endif//QOR_PP_H_COMPONENTS_JSON_MODEL_FRACTION
