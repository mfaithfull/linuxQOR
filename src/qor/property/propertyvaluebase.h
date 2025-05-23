// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef QOR_PP_H_PROPERTY_PROPERTYVALUEBASE
#define QOR_PP_H_PROPERTY_PROPERTYVALUEBASE

#include "src/qor/objectcontext/typedanyproperty.h"

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

		PropertyValueBase() : pvt(PVT_Null), m_p(nullptr) {}

		PropertyValueBase(const PropertyValueBase & src) : m_p(nullptr), pvt(PVT_Null) {}

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

		PVT pvt;
		UnsafeAnyProperty* m_p;
	};

}//qor

#endif//QOR_PP_H_PROPERTY_PROPERTYVALUEBASE
