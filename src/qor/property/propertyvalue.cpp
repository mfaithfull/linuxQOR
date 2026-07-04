// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "property.h"
#include "propertyvalue.h"

namespace qor {

	PropertyBase* PropertyValue::GetProperty()
	{
		return m_pp;
	}

	PropertyBase* PropertyValue::InternalCreateProperty(PropertyBase* pb)
	{
		Property* prop = new Property(*(dynamic_cast<Property*>(pb)));
		return prop;
	}

}