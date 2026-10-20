// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cstddef>
#include "src/components/io/generator/generator.h"
#include "src/components/io/generator/state.h"
#include "src/components/io/generator/result.h"
#include "src/components/data/formats/json/model/object.h"
#include "src/components/data/formats/json/model/member.h"
#include "_object.h"
#include "_member.h"
#include "_comma.h"

namespace qor { namespace data { namespace generator { namespace json {

    //An empty object is just the two braces.
    _object::_object(qor::components::generator::Generator* generator) :
        qor::components::generator::GeneratorState(generator),
        m_object(nullptr),
        m_count(0),
        m_position(0)
    {
        Enter = [this]()->void
        {
            if(m_position == 0)
            {
                Workflow()->PushState(new_ref<qor::components::generator::ASCIIChar>(GetGenerator(), static_cast<byte>('{')));
            }
            else if(m_position == 1)
            {
                Workflow()->PushState(new_ref<qor::components::generator::ASCIIChar>(GetGenerator(), static_cast<byte>('}')));
            }
            else
            {
                m_result.code = qor::components::generator::Result::SUCCESS;
                Workflow()->PopState();
            }
        };

        Resume = [this]()->void
        {
            ++m_position;
        };
    }

    //A non-empty object writes the opening brace, then each member separated by
    //a comma, then the closing brace. The push sequence is:
    //   position 0            : opening brace
    //   position 2k + 1       : member k
    //   position 2k           : comma before member k (k >= 1)
    //   position 2 * count    : closing brace
    _object::_object(qor::components::generator::Generator* generator, qor::data::model::json::Object* object) :
        qor::components::generator::GeneratorState(generator),
        m_object(object),
        m_count(object->m_members.size()),
        m_position(0)
    {
        Enter = [this]()->void
        {
            if(m_position == 0)
            {
                Workflow()->PushState(new_ref<qor::components::generator::ASCIIChar>(GetGenerator(), static_cast<byte>('{')));
            }
            else if(m_position < (m_count * 2))
            {
                if((m_position & 1) != 0)
                {
                    //Write the member through the member state. The member is
                    //owned by the reference stored in the model, which stays
                    //alive for as long as the object does.
                    qor::data::model::json::Member* member = m_object->m_members[m_position / 2];
                    Workflow()->PushState(new_ref<qor::data::generator::json::_member>(GetGenerator(), member));
                }
                else
                {
                    Workflow()->PushState(new_ref<qor::data::generator::json::_comma>(GetGenerator()));
                }
            }
            else if(m_position == (m_count * 2))
            {
                Workflow()->PushState(new_ref<qor::components::generator::ASCIIChar>(GetGenerator(), static_cast<byte>('}')));
            }
            else
            {
                m_result.code = qor::components::generator::Result::SUCCESS;
                Workflow()->PopState();
            }
        };

        Resume = [this]()->void
        {
            ++m_position;
        };
    }

    _object::~_object() noexcept = default;

}}}}//qor::data::generator::json
