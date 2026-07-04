// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

namespace qor {

	class ValueTagType 
	{
	public:
		ValueTagType() {}
		virtual ~ValueTagType() {}
		virtual ValueTagType* Clone() { return nullptr; }
		virtual size_t Size() const { return 0; }
	};

	template<typename T>
	class Value : public ValueTagType
	{
	public:

		typedef T type;

		Value() {}

		virtual ~Value() {}

		Value(const Value& src)
		{
			m_ = src;
		}

		Value(const T& t) : m_(t) {}

		operator T& ()
		{
			return m_;
		}

		Value& operator = (const T& t)
		{
			Set(t);
			return *this;
		}

		void Set(const T& t)
		{
			m_ = t;
		}

		T Get() const
		{
			return m_;
		}

		virtual size_t Size() const
		{
			return sizeof(T);
		}

		virtual ValueTagType* Clone() 
		{  
			return new Value<T>(m_);
		}

	private:
		T m_;
	};
}//qor