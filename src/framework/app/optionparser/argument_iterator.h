// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OPTIONPARSER_ARGITERATOR
#define QOR_PP_H_OPTIONPARSER_ARGITERATOR

#include <stdlib.h>
#include <string.h>

namespace qor{ namespace app{

    //Manages the walk through the elements of argv parameter as passed to main
    class arg_iter
    {
    private:

        int m_Index;
        char** m_argv;
        int m_argc;

    public:

        arg_iter()
        {
            m_Index = 0;
            m_argv = nullptr;
            m_argc = 0;
        }

        int IsEmpty()
        {
            return (m_argc < 1) ? true : false;
        }

        int Init(int argc, char** arguments)
        {
            m_Index = 1;
            m_argc = argc;
            m_argv = arguments;
            return m_Index;
        }

        int Begin()
        {
            m_Index = 1;
            return m_Index;
        }

        int Index()
        {
            return m_Index;
        }

        int Set(int i)
        {
            m_Index = i;
            return m_Index;
        }

        int Next()
        {
            return m_Index++;
        }

        int Inc()
        {
            return ++m_Index;
        }

        char* Arg()
        {
            return m_argv[m_Index];
        }

        char* Arg(int i)
        {
            return m_argv[i];
        }
        
        char* NextArg()
        {
            return m_argv[m_Index++];
        }

        bool NotAtEnd()
        {
            return (m_Index < m_argc) ? true : false;
        }

        bool AtEnd()
        {
            return (m_Index == m_argc) ? true : false;
        }

        int SetEnd()
        {
            m_Index = m_argc;
            return m_Index;
        }

        int End()
        {
            return m_argc;
        }

        bool AtNonOption()
        {
            return (m_argv[m_Index][0] != '-' || m_argv[m_Index][1] == '\0') ? true : false;
        }

        bool AtLongOption()
        {
            return (m_argv[m_Index][1] == '-') ? true : false;
        }

        void SwapTop(int bottom, int middle, int top, int i)
        {
            char* temp = m_argv[bottom + 1];
            m_argv[bottom + i] = m_argv[top - (middle - bottom) + i];
            m_argv[top - (middle - bottom) + i] = temp;
        }

        void SwapBottom(int bottom, int middle, int i)
        {
            char* temp = m_argv[bottom + i];
            m_argv[bottom + i] = m_argv[middle + i];
            m_argv[middle + i] = temp;
        }

        void SkipNonOptions()
        {
            while (m_Index < m_argc && (m_argv[m_Index][0] != '-' || m_argv[m_Index][1] == '\0'))
            {
                m_Index++;
            }    
        }

        bool IsDoubleDash()
        {
            return (m_Index != m_argc && !strcmp(m_argv[m_Index], "--")) ? true : false;
        }

    };

}}//qor::app

#endif//QOR_PP_H_OPTIONPARSER_ARGITERATOR
