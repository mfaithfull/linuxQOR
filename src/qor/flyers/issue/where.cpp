// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <sstream>
#include "src/qor/module/module.h"
#include "where.h"

//Implements the where-it-happened class.

namespace qor{

    Where::Where()
    {
        m_line = 0;
        m_function = "Unknown_function";
        m_file = "Unknown_file";
        m_moduleContext = ThisModule().Name();
        std::stringstream ss;
        ss << CurrentThread::GetCurrent().GetID();
        m_thread = ss.str();
        m_inInstance = false;
        m_inException = false;
    }

    Where::Where(const char* szFile, const unsigned int uiLine, const char* szFunction, const char* module)
    {
        m_inInstance = false;
        m_inException = false;
        m_file = szFile;
        m_line = uiLine;
        SetFunction(szFunction);
        m_moduleContext = module == nullptr ? ThisModule().Name() : module;
        std::optional<std::string> maybeName = CurrentThread::GetCurrent().GetName();
        if(maybeName.has_value())
        {
            m_thread = maybeName.value();
        }
        else
        {
            std::stringstream ss;
            ss << CurrentThread::GetCurrent().GetID();
            m_thread = ss.str();
        }
    }

    Where::Where(const char* szFile, const unsigned int uiLine, const char* szFunction, const AnyObject& ObjContext, const char* module) : Where(szFile, uiLine, szFunction, module)
    {
        if(!ObjContext.IsNull()) { SetObjectContext(ObjContext); };       
    }

    Where::Where(const Where& src)
    {
        *this = src;
    }

    Where::Where(const ref_of<Where>::type& src)
    {
        m_line = src->m_line;
        m_function = src->m_function;
        m_file = src->m_file;
        m_thread = src->m_thread;
        m_moduleContext = src->m_moduleContext;
        m_objContext = src->m_objContext;
        m_inInstance = src->m_inInstance;
        m_inException = src->m_inException;
    }

    Where& Where::operator = (const Where& src)
    {
        if (&src != this)
        {
            m_line = src.m_line;
            m_function = src.m_function;
            m_file = src.m_file;
            m_thread = src.m_thread;
            m_moduleContext = src.m_moduleContext;
            m_objContext = src.m_objContext;
            m_inInstance = src.m_inInstance;
            m_inException = src.m_inException;
        }
        return *this;
    }

    bool Where::GetInInstance(void) const
    {
        return m_inInstance;
    }

    unsigned int Where::GetLine(void) const
    {
        return m_line;
    }

    void Where::SetLine(const unsigned int uiLine)
    {
        m_line = uiLine;
    }

    void Where::SetFunction(const char* pszFunction)
    {
        m_function = pszFunction;
    }

    const char* Where::GetFunction(void) const
    {
        return m_function;
    }

    const char* Where::GetFile(void) const
    {
        return m_file;
    }

    void Where::SetFile(const char* szFile)
    {
        m_file = szFile;
    }

    const AnyObject& Where::GetObjectContext(void) const
    {
        return m_inInstance ? m_objContext : AnyObject::EmptyObject();
    }

    void Where::SetObjectContext(const AnyObject& ObjContext)
    {
        m_objContext = ObjContext;
        m_inInstance = true;
    }

    const std::string Where::GetThreadContext(void) const
    {
        return m_thread;
    }

    void Where::SetThreadContext(const std::string threadId)
    {
        m_thread = threadId;
    }

    const char* Where::GetModuleContext(void) const
    {
        return m_moduleContext;
    }

    void Where::SetModuleContext(const char* pModuleContext)
    {
        m_moduleContext = pModuleContext;
    }

    bool Where::GetInException(void) const
    {
        return m_inException;
    }

    void Where::SetInException(bool bInException)
    {
        m_inException = bInException;
    }

}//qor
