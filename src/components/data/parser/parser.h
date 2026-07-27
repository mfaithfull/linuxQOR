// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_PARSER_PARSER
#define QOR_PP_H_DATA_PARSER_PARSER

#include <stack>
#include <iostream>

#include "src/framework/app/workflow/workflow.h"
#include "context.h"
#include "result.h"
#include "node.h"

namespace qor { namespace data {

    class qor_pp_module_interface(QOR_PARSER) Parser : public Workflow
    {
    public:

        Parser();
        Parser(const Parser&) = delete;
        Parser& operator = (const Parser&) = delete;
        Parser(ref_of<class parser::Context>::type context);
        ~Parser();
        parser::Context* GetContext() const;
        void SetContext(ref_of<class parser::Context>::type context);
        void PushNode(ref_of<parser::Node>::type node);
        ref_of<parser::Node>::type PopNode();
        int Parse();
        int FinalParse();
        bool IsFinal();
        bool IsInError();
        void Diagnostic();

    private:

        void Drain();
        void InnerParse();
        int SafeParse();

        bool m_inError{false};
        bool m_final{false};
        ref_of<class parser::Context>::type m_context;
        std::stack<ref_of<parser::Node>::type> m_nodes;
    };

}}//qor::data

#endif//QOR_PP_H_DATA_PARSER_PARSER