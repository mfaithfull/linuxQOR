// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// Copyright 2026 Ilya Korolev
// Licensed under the Apache License, Version 2.0
// SPDX-License-Identifier: Apache-2.0

#ifndef QOR_PP_H_CONTRACT_ADAPTERS_SCHEMA
#define QOR_PP_H_CONTRACT_ADAPTERS_SCHEMA

#include "../contract.h"

#include <ostream>
#include <sstream>
#include <string>

namespace qor { namespace contract::adapters {

    class schema_writer 
    {
    public:
        explicit schema_writer(std::ostream& out)
            : out_(out) {}

        template<class Field, class Value>
        void field(const Field& field, const Value&) 
        {
            this->field(field);
        }

        template<class Field>
        void field(const Field& field) 
        {
            if (!first_) {
                out_ << '\n';
            }

            first_ = false;
            out_ << Field::id << ' ' << field.name;
        }

    private:
        std::ostream& out_;
        bool first_ = true;
    };

    template<class T>
    std::string schema_string() 
    {
        std::ostringstream out;
        schema_writer writer(out);

        contract::for_each_field<T>(
            [&](const auto&... fields) 
            {
                (writer.field(fields), ...);
            });

        return out.str();
    }

    template<class Object>
    std::string schema_string(const Object&) 
    {
        return schema_string<Object>();
    }

}}//qor::contract::adapters

#endif//QOR_PP_H_CONTRACT_ADAPTERS_SCHEMA
