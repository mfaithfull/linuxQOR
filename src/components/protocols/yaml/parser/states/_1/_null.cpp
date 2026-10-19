// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/log.h"
#include "src/qor/flyers/log/debug.h"
#include "_null.h"
#include "../../nodes/_null.h"

namespace qor { namespace protocols { namespace yaml {

    _null::_null(Parser* parser) :
        Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Specific<uint32_t> > > >(
            parser, &m_n, &m_ull, static_cast<uint64_t>(yamlToken::_null)),
        m_n(parser, 0x6e),
        m_u(parser, 0x75),
        m_l1(parser, 0x6c),
        m_l2(parser, 0x6c),
        m_ll(parser, &m_l1, &m_l2),
        m_ull(parser, &m_u, &m_ll)
    { }

    _null::~_null() noexcept = default;

    void _null::Emit()
    {
        GetParser()->PushNode(new_ref<_NullNode>());
    }

}}}//qor::protocols::yaml