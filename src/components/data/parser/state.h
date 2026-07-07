// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PARSER_STATE
#define QOR_PP_H_COMPONENTS_PARSER_STATE

#include <iostream>
#include "src/platform/compiler/compiler.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/framework/app/workflow/workflow.h"
#include "result.h"
#include "node.h"
#include "tokens.h"

namespace qor { namespace components { namespace parser {

    class Parser;
    class Context;

    class qor_pp_module_interface(QOR_PARSER) ParserState : public workflow::State
    {
    public:

        ParserState(Parser* parser, uint64_t token = static_cast<uint64_t>(eToken::Lexical));
        virtual ~ParserState();
        virtual void Reset();
        uint64_t GetToken();
        class Parser* GetParser();
        class Context* GetContext();

        Result m_result;

    protected:

        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

        class Workflow* Workflow();
        uint64_t m_token{0};
    };

    class qor_pp_module_interface(QOR_PARSER) AcceptAll : public ParserState
    {
    public:

        AcceptAll(Parser* parser);
        virtual ~AcceptAll();
    };


}}}//qor::components::parser

#endif//QOR_PP_H_COMPONENTS_PARSER_STATE