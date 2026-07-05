// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_CALLCONTEXT
#define QOR_PP_H_CALLCONTEXT

#include "icallcontext.h"

namespace qor {

    class qor_pp_module_interface(QOR_INTERCEPTION) ParameterBase
	{
	public:

		inline void Clear(void);

		void* m_p{nullptr};
	};

    class CallContext : public ICallContext
    {
    public:

        template< class T >
        class ParameterPass : public ParameterBase
        {
        public:
		
            ParameterPass(T& t) : ParameterBase()
			{
				m_p = &t;//Capture the parameter address on construction
			}

			ParameterPass(const ParameterBase& src) : ParameterBase(src){}

			ParameterPass(const ParameterPass& src) : ParameterBase(src){}

			ParameterPass& operator = (const ParameterPass& src)
			{
				ParameterBase::operator=(src);
				return *this;
			}

			virtual ~ParameterPass() = default;
		
			T& ParamterRef() 	//Access to the parameter reference
			{
				return *(reinterpret_cast<T*>(m_p));
			}
        };

        template< typename T > void qor_pp_forceinline Register(T& t)
        {
            ParameterPass< T > paramt(t);
            OutOfLineRegistration(paramt);
        }

        template< typename T > void qor_pp_forceinline RegisterReturn(T& t)
        {
            ParameterPass< T > ReturnValue( t );
            m_ReturnValue = ReturnValue;
        }

        qor_pp_module_interface(QOR_INTERCEPTION) virtual void CallMade(IFunctionContext*);
        qor_pp_module_interface(QOR_INTERCEPTION) virtual void CallCompleted();
        qor_pp_module_interface(QOR_INTERCEPTION) virtual void OnReturnAssignment();
        qor_pp_module_interface(QOR_INTERCEPTION) virtual void OnReturn();
    
		qor_pp_module_interface(QOR_INTERCEPTION) ParameterBase* Parameters();						//Access to the array of registered parameters
		qor_pp_module_interface(QOR_INTERCEPTION) ParameterBase* ReturnValue();						//Access to the return value
    
		qor_pp_module_interface(QOR_INTERCEPTION) void qor_pp_noinline OutOfLineRegistration(ParameterBase& Param);
		ParameterBase m_aParameters[10];
		ParameterBase m_ReturnValue;							//Return value
		unsigned char m_paramCount{0};

    };
}//qor

#endif//QOR_PP_H_CALLCONTEXT
