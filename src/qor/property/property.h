// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROPERTY
#define QOR_PP_H_PROPERTY

#include "propertyname.h"
#include "propertyvalue.h"
#include "src/qor/reflection/reflection.h"

namespace qor {

	template <std::size_t I, std::size_t N>
    struct PropertyGenerator;

	class qor_pp_module_interface(QOR_PROPERTY) Property : public PropertyBase
	{
	public:

		Property(const PropertyName & name) : PropertyBase(name) {}
		Property(const PropertyName & name, PropertyValue & value) : PropertyBase(name), m_value(value) {}

		virtual void SetValue(const PropertyValue& value)
		{
			m_value = value;
		}

		virtual PropertyValue Value() const
		{
			return m_value;
		}

		template<class T>
		bool CheckValueType() const
		{
			return m_value.CheckValueType<T>();
		}

		template<class T>
		T Get() const
		{
			return m_value.Get<T>();
		}

		virtual PVT GetType() const
		{
			return m_value.GetType();
		}

		PropertyVector GetVector() const
		{
			return m_value.GetVector();
		}

		PropertyMap GetMap() const
		{
			return m_value.GetMap();
		}

		template< class T >
		void Set(const T& t)
		{
			m_value.Set<T>(t);
		}

		template< class T >
		static Property FromStruct(const T& t)
		{
			constexpr std::size_t fieldCount = qor_reflection::detail::fields_count<T>();
			Property result(PropertyName(typeid(T).name()));
			PropertyVector fields;
			PropertyGenerator<0, fieldCount>::Generate(fields, qor_reflection::detail::tie_as_tuple(t), t);
			result.SetValue(PropertyValue().Set(fields));
			return result;
		}

		template< class T >
		T ToStruct()
		{
			T result;
			qor_reflection::for_each_field_with_name(result, [this](std::string_view name, auto& value, std::size_t /*i*/)
				{
					if (this->GetType() == PVT_Vector)
					{
						using VT = std::remove_reference<decltype(value)>::type;
						auto v = this->GetVector();

						auto it = std::find_if(v.cbegin(), v.cend(), [name, value](const Property& prop)->bool
							{
								return strncmp(prop.GetName().c_str(), name.data(), name.length()) == 0;
							}
						);
						if (it != v.end())
						{
							auto propval = it->Value();//TODO: need to recurse here if it->GetType() is a container
							value = propval.template Get<VT>();
						}
					}
					//TODO: need cases for Map, Null, Property and Value with recursion
				}
			);

			return result;
		}
	private:

		PropertyValue m_value;
	};

}//qor

#include "propertygenerator.h"

#endif//QOR_PP_H_PROPERTY
