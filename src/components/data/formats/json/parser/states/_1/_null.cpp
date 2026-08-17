// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/log.h"
#include "src/qor/flyers/log/debug.h"
#include "_null.h"
#include "../../nodes/_null.h"

namespace qor { namespace data { namespace parser { namespace json {

    _null::_null(Parser* parser) :
        Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Specific<uint32_t> > > >(
            parser, &m_n, &m_ull, static_cast<uint64_t>(jsonToken::_null)),
        m_n(parser, 0x6e), 
        m_u(parser, 0x75), 
        m_l(parser, 0x6c), 
        m_ll(parser, 0x6c),         
        m_sll(parser, &m_l, &m_ll),
        m_ull(parser, &m_u, &m_sll)
    { }

    _null::~_null() = default;

    void _null::Prepare()
    {
        //log::debug("Looking for a null...");
    }

    void _null::Emit()
    {        
        log::debug("Emitting a null.");
        GetParser()->PushNode(new_ref<_NullNode>());
    }

    void _null::Fail()
    {
        //log::debug("...Didn't find a null.");
    }

}}}}//qor::data::parser::json
