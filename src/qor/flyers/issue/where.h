// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ISSUE_WHERE
#define QOR_PP_H_ISSUE_WHERE

#include <string>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/module/module.h"

//Where captures the context of where in the code and runtime structure the issue was raised

namespace qor{ 

    class qor_pp_module_interface(QOR_ISSUE) Where
    {
    public:
        Where();
        Where(const char* szFile, const unsigned int uiLine, const char* szFunction, const char* module = nullptr);
        Where( const char* szFile, const unsigned int uiLine, const char* szFunction, const AnyObject& ObjContext, const char* module = nullptr);
        Where(const Where&);
        Where(const ref_of<Where>::type&);
        Where& operator = (const Where&);
        virtual ~Where() noexcept;

        void SetFile(const char* szFile);
        const char* GetFile(void) const;
        void SetLine(const unsigned int uiLine);
        unsigned int GetLine(void) const;
        void SetFunction(const char* pszFunction);
        const char* GetFunction(void) const;
        const AnyObject& GetObjectContext(void) const;
        void SetObjectContext(const AnyObject& ObjContext);
        const std::string GetThreadContext(void) const;
        void SetThreadContext(const std::string threadId);
        const char* GetModuleContext(void) const;
        void SetModuleContext(const char* pModuleContext);
        bool GetInException(void) const;
        void SetInException(bool bInException);
        bool GetInInstance(void) const;

    private:

        unsigned int m_line;
        const char* m_function;
        const char* m_file;
        AnyObject m_objContext;
        std::string m_thread;
        const char* m_moduleContext;        
        bool m_inException;
        bool m_inInstance;

    };

}//qor

#endif//QOR_PP_H_ISSUE_WHERE
