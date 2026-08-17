// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_JSON_PARSER_FALSE
#define QOR_PP_H_DATA_JSON_PARSER_FALSE

#include "../../tokens.h"

namespace qor { namespace data { namespace parser { namespace json {

    //false = %x66.61.6c.73.65   ; false
    class qor_pp_module_interface(QOR_JSON) _false : public Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Specific<uint32_t> > > > >
    {
    public: 
        _false(Parser* parser);
        virtual ~_false();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

    private:

        Specific<uint32_t> m_f;
        Specific<uint32_t> m_a;
        Specific<uint32_t> m_l;
        Specific<uint32_t> m_s;
        Specific<uint32_t> m_e;
        Sequence_t< Specific<uint32_t>, Specific<uint32_t> > m_se;
        Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Specific<uint32_t> > > m_lse;
        Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Specific<uint32_t> > > > m_alse;
        
    };

}}}}//qor::data::parser::json

#endif//QOR_PP_H_DATA_JSON_PARSER_FALSE
