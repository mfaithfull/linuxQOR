// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_PARSER_PARSER
#define QOR_PP_H_DATA_PARSER_PARSER

#include <stack>
#include <iostream>

#include "src/framework/app/workflow/fastflow.h"
#include "context.h"
#include "result.h"
#include "node.h"

namespace qor { namespace data {

    class qor_pp_module_interface(QOR_PARSER) Parser : public Fastflow
    {
    public:

        Parser();
        Parser(const Parser&) = delete;
        Parser& operator = (const Parser&) = delete;
        Parser(data::AbstractDataContext* context);
        ~Parser();
        data::AbstractDataContext* GetContext() const;
        void SetContext(data::AbstractDataContext* context);
        void PushNode(ref_of<parser::Node>::type node);
        ref_of<parser::Node>::type PopNode();
        ref_of<parser::Node>::type& TopNode();
        int Parse();
        int FinalParse();
        bool IsFinal();
        bool IsInError();
        void Diagnostic();

        std::vector< ref_of<fastflow::Step>::type >& GetStateCache(uint64_t token);
    private:

        void Drain();
        void InnerParse();
        int SafeParse();

        void DumpTokenStack();

        bool m_inError{false};
        bool m_final{false};
        data::AbstractDataContext* m_context{nullptr};
        std::stack<ref_of<parser::Node>::type> m_nodes;
        std::unordered_map< uint64_t, std::vector< ref_of<fastflow::Step>::type > > m_stateCache;
    };

}}//qor::data

#endif//QOR_PP_H_DATA_PARSER_PARSER