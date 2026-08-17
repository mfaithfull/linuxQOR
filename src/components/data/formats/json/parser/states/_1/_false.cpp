// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/log.h"
#include "src/qor/flyers/log/debug.h"
#include "_false.h"
#include "../../nodes/_false.h"

namespace qor { namespace data { namespace parser { namespace json {

    _false::_false(Parser* parser) : 
        Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Specific<uint32_t> > > > >(
            parser, &m_f, &m_alse, static_cast<uint64_t>(jsonToken::_false)), 
        m_f(parser, 0x66), 
        m_a(parser, 0x61), 
        m_l(parser, 0x6c), 
        m_s(parser, 0x73),
        m_e(parser, 0x65),
        m_se(parser, &m_s, &m_e),
        m_lse(parser, &m_l, &m_se),
        m_alse(parser, &m_a, &m_lse)
    { }

    _false::~_false() = default;

    void _false::Prepare()
    {
        //log::debug("Looking for a false...");
    }

    void _false::Emit()
    {        
        //log::debug("Emitting a false.");
        GetParser()->PushNode(new_ref<_FalseNode>());
    }

    void _false::Fail()
    {
        //log::debug("...Didn't find a false.");
    }

}}}}//qor::data::parser::json
