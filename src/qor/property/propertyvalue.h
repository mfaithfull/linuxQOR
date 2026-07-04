// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROPERTY_PROPERTYVALUE
#define QOR_PP_H_PROPERTY_PROPERTYVALUE

#include <vector>
#include <unordered_map>

#include "propertyvaluebase.h"
#include "propertybase.h"

namespace qor {

	class qor_pp_module_interface(QOR_PROPERTY) Property;
	typedef std::unordered_map< PropertyName, Property > PropertyMap;
	typedef std::vector< Property > PropertyVector;

	class qor_pp_module_interface(QOR_PROPERTY) PropertyValue : public PropertyValueBase
	{
	public:

		PropertyValue(){ }

		PropertyValue(const PropertyValue& src) : PropertyValueBase(src)
		{
			*this = src;
		}

		PropertyValue& operator =(const PropertyValue& src)
		{
			if (&src != this)
			{
				return Set(src);
			}
			return *this;
		}

		~PropertyValue()
		{
			delete m_pv;
			m_pv = nullptr;
			delete m_pm;
			m_pm = nullptr;
			delete m_pp;
			m_pp = nullptr;
		}

		PropertyVector GetVector() const
		{
			return *m_pv;
		}

		PropertyMap GetMap() const
		{
			return *m_pm;
		}

		PropertyBase* GetProperty();

		template< class T >
		PropertyValue& Set(const T& t)
		{
			delete m_p;			
			delete m_pv;
			delete m_pm;
			delete m_pp;

			if constexpr (std::is_same < T, PropertyValue >::value)
			{
				pvt = t.GetType();
				switch (pvt)
				{
				case PVT_Null:
					break;
				case PVT_Map:
					m_pm = new PropertyMap(*t.m_pm);
					break;
				case PVT_Property:
					m_pp = InternalCreateProperty(t.m_pp);
					break;
				case PVT_Vector:
					m_pv = new PropertyVector(*t.m_pv);
					break;
				case PVT_Value:
					m_p = t.m_p->Clone();
					break;
				}
				return *this;
			}
			else if constexpr (std::is_same < T, Property >::value)
			{
				pvt = PVT_Property;
				m_pp = t.m_pp->Clone();
				return *this;
			}
			else if constexpr (std::is_same < T, PropertyVector >::value)
			{
				pvt = PVT_Vector;
				m_pv = new PropertyVector(t);
				return *this;
			}
			else if constexpr (std::is_same < T, PropertyMap >::value)
			{
				pvt = PVT_Map;
				m_pm = new PropertyMap(t);
				return *this;
			}
			else if constexpr (std::is_base_of < UnsafeAnyProperty, T >::value)
			{
				pvt = PVT_Value;
				m_p = t.Clone();
				return *this;
			}
			else
			{
				pvt = PVT_Value;
				m_p = new TypedAnyProperty<T>(t);
				return *this;
			}
		}

	private:

		PropertyBase* InternalCreateProperty(PropertyBase* pb);

		PropertyVector* m_pv{nullptr};
		PropertyMap* m_pm{nullptr};
		PropertyBase* m_pp{nullptr};

	};

}//qor

#endif//QOR_PP_H_PROPERTY_PROPERTYVALUE
