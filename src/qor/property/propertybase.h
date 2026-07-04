// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROPERTY_PROPERTYBASE
#define QOR_PP_H_PROPERTY_PROPERTYBASE

#include <vector>
#include <unordered_map>

#include "propertyvaluebase.h"
#include "propertyname.h"

namespace qor {

	class qor_pp_module_interface(QOR_PROPERTY) PropertyValue;
	
	class qor_pp_module_interface(QOR_PROPERTY) PropertyBase
	{
	public:
		PropertyBase(const PropertyName & name) : m_name(name) {}

		virtual ~PropertyBase() = default;

		void SetName(const PropertyName & name)
		{
			m_name = name;
		}

		PropertyName GetName() const
		{
			return m_name;
		}

		virtual void SetValue(const PropertyValue & value) = 0;
		virtual PropertyValue Value() const = 0;
		virtual PVT GetType() const = 0;

	protected:

		PropertyName m_name;
	};
}

#endif//QOR_PP_H_PROPERTY_PROPERTYBASE