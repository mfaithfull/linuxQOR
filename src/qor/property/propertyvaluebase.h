// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROPERTY_PROPERTYVALUEBASE
#define QOR_PP_H_PROPERTY_PROPERTYVALUEBASE

#include "src/qor/essentials/objectcontext/typedanyproperty.h"

namespace qor {

	enum qor_pp_module_interface(QOR_PROPERTY) PVT
	{
		PVT_Value = 0,
		PVT_Property,
		PVT_Vector,
		PVT_Map,
		PVT_Null
	};

	class qor_pp_module_interface(QOR_PROPERTY) PropertyValueBase
	{
	public:

		PropertyValueBase(){ }

		PropertyValueBase(const PropertyValueBase & src)
		{
			if(&src != this)
			{
				//TODO: Copy property
			}
		}

		virtual ~PropertyValueBase()
		{
			delete m_p;
			m_p = nullptr;
		}

		PVT GetType() const
		{
			return pvt;
		}

		template<class T>
		T Get() const
		{
			return *(m_p->template GetAs<T>());
		}

		template<class T>
		bool CheckValueType() const
		{
			return pvt == PVT_Value && m_p->GetAs<T>() != nullptr;
		}

		bool IsNull() { return pvt == PVT_Null; }
		bool IsValue() { return pvt == PVT_Value; }
		bool IsProperty() { return pvt == PVT_Property; }
		bool IsVector() { return pvt == PVT_Vector; }
		bool IsMap() { return pvt == PVT_Map; }

	protected:

		PVT pvt{PVT_Null};
		UnsafeAnyProperty* m_p{nullptr};
	};

}//qor

#endif//QOR_PP_H_PROPERTY_PROPERTYVALUEBASE
