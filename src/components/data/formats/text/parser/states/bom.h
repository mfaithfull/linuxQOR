// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_TEXT_PARSER_BOM
#define QOR_PP_H_DATA_TEXT_PARSER_BOM

#include "../tokens.h"
#include "src/components/data/parser/states/anyoneofset.h"

namespace qor { namespace data { namespace parser { namespace text {
    
    class qor_pp_module_interface(QOR_TEXTFORMAT) bom : public AnyOneOfSet
    {
    public: 
        bom(Parser* parser);
        virtual ~bom();

        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    };

}}}}//qor::data::parser::text

#endif//QOR_PP_H_DATA_TEXT_PARSER_BOM
