// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//YAML parsing test

#include "src/configuration/configuration.h"
#include "src/qor/tdd/tdd.h"

#include "src/framework/data/pipeline/podbuffer.h"
#include "src/components/protocols/yaml/parser/yamlparser.h"
#include "src/components/protocols/yaml/parser/nodes/mapping.h"
#include "src/components/data/pipeline/sinks/parsersink/parsersink.h"

qor_pp_test_case(yaml_mapping_roundtrip)
{
    // { "key" : "value" }
    const char32_t document[] = U"{ \"key\" : \"value\" }";

    qor::pipeline::PODBuffer<uint32_t> buffer(256);
    for(const char32_t* p = document; *p != 0; ++p)
    {
        buffer << static_cast<uint32_t>(*p);
    }

    qor::pipeline::components::ParserSink< qor::protocols::yaml::mapping > sink(buffer);

    auto result = sink.Parser().FinalParse();
    qor_pp_assert_that(result != -1);
    qor_pp_assert_that(!sink.Parser().IsInError());

    auto node = sink.Parser().PopNode();
    qor_pp_assert_that(node.IsNotNull());
    qor_pp_assert_that(node->GetToken() == static_cast<uint64_t>(qor::protocols::yaml::yamlToken::mapping));

    auto mappingObj = node.AsRef<qor::protocols::yaml::MappingNode>()->GetObject();
    qor_pp_assert_that(mappingObj->m_pairs.size() == 1);
    qor_pp_assert_that(mappingObj->m_pairs[0]->GetKey() == "key");
    qor_pp_assert_that(mappingObj->m_pairs[0]->GetValue().GetType() == qor::protocols::model::yaml::ValueType::string);
}
