// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_CONSOLE_POSITION
#define QOR_PP_H_COMPONENTS_CONSOLE_POSITION

namespace qor { namespace components {

    struct qor_pp_module_interface(QOR_CONSOLE) Position
    {
        int m_column;
        int m_row;
    };

}}//qor::components

#endif//QOR_PP_H_COMPONENTS_CONSOLE_POSITION
