// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ERROR_CLIBRESULT
#define QOR_PP_H_ERROR_CLIBRESULT

#include <string>
#include <cstring>
#include <map>
#include "error.h"

namespace qor{ 
    
    class qor_pp_module_interface(QOR_ERROR) CLibResult
    {
    public:

        CLibResult() : m_result(errno)
        {   
            Check();             
        }

        CLibResult(int i) : m_result(i)
        {
            Check();           
        }

        CLibResult(const CLibResult& src) : m_result(src.m_result)
        {
            Check();                
        }

        CLibResult& operator = (const CLibResult src)
        {                
            m_result = src.m_result;
            Check();
            return *this;
        }

        CLibResult& operator = (const int s)
        {                
            m_result = s;
            Check();
            return *this;
        }

        operator int()
        {
            Check();
            return m_result;
        }

        bool IsOK()
        {
            return m_result == 0 ? true : false;
        }

        bool IsError()
        {
            return !IsOK();
        }

        std::string AsString()
        {
            return GetErrorMessage();
        }

        bool static IndicatesFailure(int result)
        {
            if(result !=0 )
            {
                return true;
            }
            return false;
        }

    private:

        void Check()
        {
            if(IsError())
            {
                continuable(GetErrorMessage());
            }
        }

#if(qor_pp_compiler == qor_pp_compiler_msvc)
#pragma warning(push)
#pragma warning(disable: 4996)
#endif
        std::string GetErrorMessage() 
        {
            auto it = error_map.find(m_result);
            if (it != error_map.end()) 
            {
                return std::string(it->second);
            }
            char* msg = strerror(m_result);
            if (msg) 
            {
                return std::string(msg);
            }
            return "Unknown error";
        }
#if(qor_pp_compiler == qor_pp_compiler_msvc)        
#pragma warning(pop)
#endif
        int m_result;
        static std::map<int, const char*> error_map;
    };
}//qor

#endif//QOR_PP_H_ERROR_CLIBRESULT
