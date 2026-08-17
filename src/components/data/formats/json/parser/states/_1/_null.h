// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_NULL
#define QOR_PP_H_DATA_JSON_PARSER_NULL

#include "../../tokens.h"

namespace qor { namespace data { namespace parser { namespace json {

    //null  = %x6e.75.6c.6c      ; null    
    class qor_pp_module_interface(QOR_JSON) _null : public Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Specific<uint32_t> > > >
    {
    public: 
        _null(Parser* parser);
        virtual ~_null();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

    private:
    
        Specific<uint32_t> m_n;
        Specific<uint32_t> m_u;
        Specific<uint32_t> m_l;
        Specific<uint32_t> m_ll;        

        Sequence_t< Specific<uint32_t>, Specific<uint32_t> > m_sll;
        Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Specific<uint32_t> > > m_ull;
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_NULL
