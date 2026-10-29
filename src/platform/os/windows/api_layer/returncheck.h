// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_SRC_PLATFORM_OS_WINDOWS_API_LAYER_RETURNCHECK
#define QOR_PP_H_SRC_PLATFORM_OS_WINDOWS_API_LAYER_RETURNCHECK

#include "src/qor/flyers/error/error.h"

namespace qor { namespace win { namespace api {

	//Generic validated return value class
	//TReturn is the type of the return value
	//TCheck is the type of validation check to be performed
	template<class TReturn, class TCheck >
	class CheckReturn
	{
	public:

#if		( qor_pp_parameter_checking == qor_pp_yes )
		typedef CheckReturn< TReturn, TCheck > TType;
#else
		typedef TReturn TType;
#endif

		CheckReturn() : m_bInitialised(false)
		{
			TCheck AutoCheck;
			TCheck::Init(m_Param, &AutoCheck);
		}

		CheckReturn(TReturn param) : m_Param(param), m_bInitialised(true)
		{
			TCheck AutoCheck;
			TCheck::Check(m_Param, &AutoCheck);
		}

		CheckReturn& operator = (TReturn& value)
		{
			m_Param = value;
			m_bInitialised = true;
			TCheck AutoCheck;
			TCheck::Check(m_Param, &AutoCheck);
			return *this;
		}

		operator TReturn()
		{
			if (!m_bInitialised)
			{
				continuable("Uninitialised parameter error");
			}
			return m_Param;
		}

	protected:

		TReturn m_Param;			//Instance of parameter type

	private:

		bool m_bInitialised;
	};

	template< typename TValue >
	class AutoErrorCheck
	{
	public:

		AutoErrorCheck() = default;
		virtual ~AutoErrorCheck()= default;

		virtual bool Test(TValue& /*value*/)
		{
			return false;
		}

		virtual void DefaultInit(TValue& /*value*/){ }

		static void Check(TValue& value, AutoErrorCheck* pInstance)
		{
			if (!pInstance->Test(value))
			{
				continuable("A checked parameter failed a test");
			}
		}

		static void Init(TValue& value, AutoErrorCheck* pInstance)
		{
			pInstance->DefaultInit(value);
		}
	};


	class BoolCheck : public AutoErrorCheck< BOOL >
	{
	public:


		virtual bool Test(BOOL& value)
		{
			return value ? true : false;
		}


		virtual void DefaultInit(BOOL& value)
		{
			value = 0;
		}
	};


	class HandleCheck : public AutoErrorCheck< HANDLE >
	{
	public:


		virtual bool Test(HANDLE& value)
		{
			return (value == (HANDLE)((size_t)(-1))) ? false : true;
		}
	};


	class HandleNullCheck : public AutoErrorCheck< HANDLE >
	{
	public:


		virtual bool Test(HANDLE& value)
		{
			return (value == (HANDLE)((size_t)(-1)) || value == (HANDLE)(size_t)(0)) ? false : true;
		}
	};


	template< typename THandle >
	class CheckNonZeroHandle : public AutoErrorCheck< THandle >
	{
	public:


		virtual bool Test(THandle& value)
		{
			return (value == 0x00000000) ? false : true;
		}
	};


	class SuccessCheck : public AutoErrorCheck< intptr_t >
	{
	public:


		virtual bool Test(intptr_t& value)
		{
			return (value == 0) ? true : false;
		}
	};


	class LongSuccessCheck : public AutoErrorCheck< long >
	{
	public:


		virtual bool Test(intptr_t& value)
		{
			return (value == 0) ? true : false;
		}
	};


	template< typename TNumeric >
	class TSuccessCheck : public AutoErrorCheck< TNumeric >
	{
	public:


		virtual bool Test(TNumeric& value)
		{
			return (value == 0) ? true : false;
		}
	};


	template< typename TNumeric >
	class CheckNonZero : public AutoErrorCheck< TNumeric >
	{
	public:


		virtual bool Test(TNumeric& value)
		{
			return (value == 0) ? false : true;
		}
	};


	template< int iFailure >
	class CheckFailureInt : public AutoErrorCheck< int >
	{
	public:


		virtual bool Test(int& value)
		{
			return (value == iFailure) ? false : true;
		}
	};


	template< typename TFailure, TFailure iFailure >
	class TCheckFailureValue : public AutoErrorCheck< TFailure >
	{
	public:


		virtual void DefaultInit(TFailure& value)
		{
			value = iFailure;
		}


		virtual bool Test(TFailure& value)
		{
			return (value == iFailure) ? false : true;
		}
	};


	template< typename TReturn, TReturn Min, TReturn Max >
	class TRangeCheck : public AutoErrorCheck< TReturn >
	{
	public:


		virtual void DefaultInit(TReturn& value)
		{
			value = Min - 1;
		}


		virtual bool Test(TReturn& value)
		{
			return (value >= Min && value <= Max) ? true : false;
		}
	};


	template< typename TReturn, TReturn Min >
	class TCheckNotLess : public AutoErrorCheck< TReturn >
	{
	public:


		virtual void DefaultInit(TReturn& value)
		{
			value = Min - 1;
		}


		virtual bool Test(TReturn& value)
		{
			return (value < Min) ? false : true;
		}
	};

}}}//qor:win::api

#endif//QOR_PP_H_SRC_PLATFORM_OS_WINDOWS_API_LAYER_RETURNCHECK
