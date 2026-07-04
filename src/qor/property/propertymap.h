// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROPERTY_MAP
#define QOR_PP_H_PROPERTY_MAP

#include <unordered_map>
#include "property.h"

namespace qor {

	typedef std::unordered_map< PropertyName, Property > PropertyMap;

}//qor

#endif//QOR_PP_H_PROPERTY_MAP
