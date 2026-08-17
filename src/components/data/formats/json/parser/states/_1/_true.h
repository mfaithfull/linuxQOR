// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_TRUE
#define QOR_PP_H_DATA_JSON_PARSER_TRUE

#include "../../tokens.h"

namespace qor { namespace data { namespace parser { namespace json {

    //true  = %x74.72.75.65      ; true
    class qor_pp_module_interface(QOR_JSON) _true : public Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Specific<uint32_t> > > >
    {
    public: 
        _true(Parser* parser);
        virtual ~_true();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

    private:

        Specific<uint32_t> m_t;
        Specific<uint32_t> m_r;
        Specific<uint32_t> m_u;
        Specific<uint32_t> m_e;

        Sequence_t< Specific<uint32_t>, Specific<uint32_t> > m_ue;
        Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Specific<uint32_t> > > m_rue;

    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_TRUE
