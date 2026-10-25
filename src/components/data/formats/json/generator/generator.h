// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_SRC_COMPONENTS_DATA_FORMATS_JSON_GENERATOR_GENERATOR
#define QOR_PP_H_SRC_COMPONENTS_DATA_FORMATS_JSON_GENERATOR_GENERATOR

namespace qor{ namespace data{ namespace generator{ namespace json{

    // The states that make up the JSON value grammar. Each state is added as it
    // is implemented;
    class _null;
    class _true;
    class _false;
    class _string;
    class _number;
    class _array;
    class _object;
    class _value;

}}}}// namespace qor::data::generator::json

#endif//QOR_PP_H_SRC_COMPONENTS_DATA_FORMATS_JSON_GENERATOR_GENERATOR
