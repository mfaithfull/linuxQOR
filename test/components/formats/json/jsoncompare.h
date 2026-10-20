// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEST_JSON_JSONCOMPARE
#define QOR_PP_H_TEST_JSON_JSONCOMPARE

//Compares two decoded JSON model trees for structural and value equality. This
//is the roundtrip check: a model is generated to JSON text by the write half
//(jsonwriter.h), that text is parsed back into a second model by the read half
//(jsonreader.h), and the two models are compared here.
//
//The comparison uses decoded values. The model stores string text that has
//already been decoded from any JSON escapes, so the escapes themselves are not
//retained and cannot be compared; it is the decoded text that must be equal.
//Numbers are stored as doubles and the generator writes them with std::to_chars,
//which is round-trip exact, so an exact double comparison is correct. Containers
//are compared in their stored order, which is the order the parser populates
//from the file and the generator writes back out.

#include "src/components/data/formats/json/model/value.h"
#include "src/components/data/formats/json/model/object.h"
#include "src/components/data/formats/json/model/array.h"
#include "src/components/data/formats/json/model/string.h"
#include "src/components/data/formats/json/model/number.h"
#include "src/components/data/formats/json/model/member.h"
#include <cstddef>

namespace qor {

    //The three model kinds compare through these. They are mutually recursive:
    //a value reaches a concrete container through the polymorphic handle and
    //hands that container to its element/member comparison.

    bool CompareValue(qor::data::model::json::Value* left, qor::data::model::json::Value* right);
    bool CompareObject(qor::data::model::json::Object* left, qor::data::model::json::Object* right);
    bool CompareArray(qor::data::model::json::Array* left, qor::data::model::json::Array* right);

    inline bool CompareValue(qor::data::model::json::Value* left, qor::data::model::json::Value* right)
    {
        using namespace qor::data::model::json;

        if(left == nullptr || right == nullptr)
        {
            return left == right;
        }

        const ValueType leftType = left->GetType();
        if(leftType != right->GetType())
        {
            return false;
        }

        switch(leftType)
        {
            case ValueType::_null:
            case ValueType::_true:
            case ValueType::_false:
            {
                //These carry no payload; a matching type is enough.
                return true;
            }
            case ValueType::number:
            {
                //Reach the concrete Number through the polymorphic value handle,
                //exactly as the generator value state does before it writes one.
                Number* leftNumber = dynamic_cast<Number*>(left->GetValue().operator->());
                Number* rightNumber = dynamic_cast<Number*>(right->GetValue().operator->());
                if(leftNumber == nullptr || rightNumber == nullptr)
                {
                    return false;
                }
                return leftNumber->GetValue() == rightNumber->GetValue();
            }
            case ValueType::string:
            {
                qor::data::model::json::String* leftString = dynamic_cast<qor::data::model::json::String*>(left->GetValue().operator->());
                qor::data::model::json::String* rightString = dynamic_cast<qor::data::model::json::String*>(right->GetValue().operator->());
                if(leftString == nullptr || rightString == nullptr)
                {
                    return false;
                }
                return leftString->GetValue() == rightString->GetValue();
            }
            case ValueType::array:
            {
                Array* leftArray = dynamic_cast<Array*>(left->GetValue().operator->());
                Array* rightArray = dynamic_cast<Array*>(right->GetValue().operator->());
                if(leftArray == nullptr || rightArray == nullptr)
                {
                    return false;
                }
                return CompareArray(leftArray, rightArray);
            }
            case ValueType::object:
            {
                Object* leftObject = dynamic_cast<Object*>(left->GetValue().operator->());
                Object* rightObject = dynamic_cast<Object*>(right->GetValue().operator->());
                if(leftObject == nullptr || rightObject == nullptr)
                {
                    return false;
                }
                return CompareObject(leftObject, rightObject);
            }
            default:
            {
                return false;
            }
        }
    }

    inline bool CompareObject(qor::data::model::json::Object* left, qor::data::model::json::Object* right)
    {
        using namespace qor::data::model::json;

        if(left == nullptr || right == nullptr)
        {
            return left == right;
        }

        if(left->m_members.size() != right->m_members.size())
        {
            return false;
        }

        for(size_t index = 0; index < left->m_members.size(); ++index)
        {
            //A member is stored as a smart reference; operator-> gives the Member.
            Member* leftMember = left->m_members[index].operator->();
            Member* rightMember = right->m_members[index].operator->();
            if(leftMember == nullptr || rightMember == nullptr)
            {
                return false;
            }
            if(leftMember->GetName() != rightMember->GetName())
            {
                return false;
            }
            if(!CompareValue(&leftMember->GetValue(), &rightMember->GetValue()))
            {
                return false;
            }
        }

        return true;
    }

    inline bool CompareArray(qor::data::model::json::Array* left, qor::data::model::json::Array* right)
    {
        using namespace qor::data::model::json;

        if(left == nullptr || right == nullptr)
        {
            return left == right;
        }

        if(left->m_values.size() != right->m_values.size())
        {
            return false;
        }

        for(size_t index = 0; index < left->m_values.size(); ++index)
        {
            Value* leftElement = left->m_values[index].operator->();
            Value* rightElement = right->m_values[index].operator->();
            if(!CompareValue(leftElement, rightElement))
            {
                return false;
            }
        }

        return true;
    }

    //The single entry point. It picks the comparison for the top-level kind and
    //mirrors how the readers hand back an Object, an Array or a Value.
    inline bool CompareModel(qor::data::model::json::Value* left, qor::data::model::json::Value* right)
    {
        return CompareValue(left, right);
    }

    inline bool CompareModel(qor::data::model::json::Object* left, qor::data::model::json::Object* right)
    {
        return CompareObject(left, right);
    }

    inline bool CompareModel(qor::data::model::json::Array* left, qor::data::model::json::Array* right)
    {
        return CompareArray(left, right);
    }

}//qor

#endif//QOR_PP_H_TEST_JSON_JSONCOMPARE