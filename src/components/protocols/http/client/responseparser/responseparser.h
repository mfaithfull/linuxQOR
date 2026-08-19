// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_RESPONSEPARSER
#define QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_RESPONSEPARSER

#include <map>

#include "src/components/data/parser/state.h"
#include "src/components/data/parser/states/rfc5234.h"
#include "src/components/data/parser/tokens.h"
#include "src/components/data/parser/parser.h"
#include "src/components/data/parser/states/oneormore.h"
#include "src/components/data/parser/nodes/char.h"

namespace qor { namespace components { namespace protocols { namespace http { namespace response {

    enum class httpResponseToken : uint64_t
    {
        responseChar = static_cast<uint64_t>(qor::data::parser::eToken::Max) + 1ull,
        response,
        status_line,
        specificString,
        version,
        status_code,
        reason_phrase,
    };

    static const std::map<const httpResponseToken, const std::string> httpResponseTokenNames = {{
        {httpResponseToken::responseChar, "responseChar"},
        {httpResponseToken::response, "response"},
    }};

    class responseChar : public qor::data::parser::OneOfARange_t<uint32_t>
    {
    public: responseChar(qor::data::Parser* parser) :
                qor::data::parser::OneOfARange_t<uint32_t>(parser,
                0x00,
                0xFF,
                static_cast<uint64_t>(httpResponseToken::responseChar))
            {}
        virtual ~responseChar() = default;
        virtual void Emit()
        {
            uint32_t charValue = m_result.first;
            GetParser()->PushNode(
                qor::new_ref<qor::data::parser::CharNode>(
                    charValue,static_cast<uint64_t>(httpResponseToken::responseChar)
                ).template AsRef<qor::data::parser::Node>()
            );
        }
    };

    class Initial : public qor::data::parser::OneOrMore
    {
    public: Initial(qor::data::Parser* parser) :
                data::parser::OneOrMore( parser,
                    new_ref<responseChar>(parser),
                    static_cast<uint64_t>(httpResponseToken::response))
            {}

        virtual ~Initial() = default;

        virtual void Prepare();
        virtual void Emit();
        virtual void Fail()
        {
            auto node = GetParser()->PopNode();
            if(node.IsNotNull() && node->GetToken() != m_token)
            {
                GetParser()->PushNode(node);
            }
        }
    };

    class SpecificString : public data::parser::OneOrMore
    {
    public: SpecificString(data::Parser* parser, const std::string& /*specific*/) : data::parser::OneOrMore(parser,
                new_ref<data::parser::CHAR<uint32_t>>(parser),
                static_cast<uint64_t>(httpResponseToken::specificString)
            )
            {}

        virtual ~SpecificString() = default;
        virtual void Prepare()
        {

        }

        virtual void Emit()
        {

        }

        virtual void Fail()
        {

        }

    };

    class VersionParser : public data::parser::Sequence
    {
    public: VersionParser( data::Parser* parser) : 
                data::parser::Sequence( parser,
                    new_ref<SpecificString>(parser, "HTTP"),
                    new_ref<data::parser::Sequence>(parser,
                        new_ref<data::parser::Specific<uint32_t>>(parser, '/'),
                        new_ref<data::parser::Sequence>(parser,
                            new_ref<data::parser::DIGIT<uint32_t>>(parser),
                            new_ref<data::parser::Sequence>(parser,
                                new_ref<data::parser::Specific<uint32_t>>(parser, '.'),
                                new_ref<data::parser::DIGIT<uint32_t>>(parser)
                            )
                        )
                    ),
                    static_cast<uint64_t>(httpResponseToken::version)
                )
            {}

        virtual ~VersionParser() = default;
        virtual void Prepare()
        {

        }

        virtual void Emit()
        {

        }
        
        virtual void Fail()
        {

        }
    };

    class ReasonPhraseParser : public data::parser::OneOrMore
    {
    public: ReasonPhraseParser(data::Parser* parser) :
                data::parser::OneOrMore( parser,
                    new_ref<data::parser::ALPHA<uint32_t>>(parser),
                    static_cast<uint64_t>(httpResponseToken::reason_phrase)
                )
            {}
        virtual ~ReasonPhraseParser() = default;
        virtual void Prepare()
        {

        }

        virtual void Emit()
        {

        }
        
        virtual void Fail()
        {

        }
    };

    class StatusCodeParser : public data::parser::Sequence
    {
    public: StatusCodeParser(data::Parser* parser) :
                data::parser::Sequence( parser,
                    new_ref<data::parser::DIGIT<uint32_t>>(parser),
                    new_ref<data::parser::Sequence>(parser,
                        new_ref<data::parser::DIGIT<uint32_t>>(parser),
                        new_ref<data::parser::DIGIT<uint32_t>>(parser)
                    ),
                    static_cast<uint64_t>(httpResponseToken::status_code)
                )
            {}
        virtual ~StatusCodeParser() = default;
        virtual void Prepare()
        {

        }

        virtual void Emit()
        {

        }
        
        virtual void Fail()
        {

        }
    };
    
    class StatusLineParser : public data::parser::Sequence
    {
    public: StatusLineParser(data::Parser* parser) : 
                data::parser::Sequence( parser, 
                    new_ref<VersionParser>(parser),
                    new_ref<data::parser::Sequence>(parser,
                        new_ref<data::parser::SP<uint32_t>>(parser),
                        new_ref<data::parser::Sequence>(parser,
                            new_ref<StatusCodeParser>(parser),
                            new_ref<data::parser::Sequence>(parser,
                                new_ref<data::parser::SP<uint32_t>>(parser),
                                new_ref<data::parser::Sequence>(parser,
                                    new_ref<ReasonPhraseParser>(parser),
                                    new_ref<data::parser::CRLF<uint32_t>>(parser)
                                )
                            )
                        )                        
                    ),
                    static_cast<uint64_t>(httpResponseToken::status_line)
                )
            {}

        virtual ~StatusLineParser() = default;
        virtual void Prepare()
        {

        }

        virtual void Emit()
        {

        }
        
        virtual void Fail()
        {

        }
    };

}}}}}//qor::components::protocols::http::response

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_HTTP_RESPONSEPARSER