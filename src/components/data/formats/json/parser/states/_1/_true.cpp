// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/log.h"
#include "src/qor/flyers/log/debug.h"
#include "_true.h"
#include "../../nodes/_true.h"

namespace qor { namespace data { namespace parser { namespace json {

    _true::_true(Parser* parser) :
        Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Specific<uint32_t> > > >(
            parser, &m_t, &m_rue, static_cast<uint64_t>(jsonToken::_true)),
            m_t(parser, 0x74),
            m_r(parser, 0x72),
            m_u(parser, 0x75),
            m_e(parser, 0x65),
            m_ue(parser, &m_u, &m_e),
            m_rue(parser, &m_r, &m_ue)
    { }

    _true::~_true() = default;

    void _true::Prepare()
    {
        //log::debug("Looking for a true...");
    }

    void _true::Emit()
    {
        //log::debug("Emitting a true.");
        GetParser()->PushNode(new_ref<_TrueNode>());
    }

    void _true::Fail()
    {
        //log::debug("...Didn't find a true.");
    }

}}}}//qor::data::parser::json
