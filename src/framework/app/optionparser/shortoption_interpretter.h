// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OPTIONPARSER_SHORTOPTIONINTERPRETTER
#define QOR_PP_H_OPTIONPARSER_SHORTOPTIONINTERPRETTER

#include "argument_iterator.h"

namespace qor{ namespace app{

    enum Ordering
    {
        REQUIRE_ORDER,
        PERMUTE,
        RETURN_IN_ORDER
    };

    //Helper to decode a short option set
    class ShortOptInterpretter
    {

    public:

        ShortOptInterpretter(){}

        void Init(const char* shortOptions)
        {
            m_ShortOptionSet = shortOptions;
        }

        bool DashOrPlus()
        {
            return (m_ShortOptionSet[0] == '-' || m_ShortOptionSet[0] == '+') ? true : false;
        }

        bool Colon()
        {
            return (m_ShortOptionSet[0] == ':') ? true : false;
        }

        bool Dash()
        {
            return (m_ShortOptionSet[0] == '-' ) ? true : false;
        }

        bool Plus()
        {
            return (m_ShortOptionSet[0] == '+' ) ? true : false;
        }

        void Next()
        {
            m_ShortOptionSet++;
        }

        void Inc()
        {
            ++m_ShortOptionSet;
        }

        const char* Find( char c )
        {
            return strchr(m_ShortOptionSet, c);
        }

        char ColonQuestion()
        {
            return m_ShortOptionSet[0] == ':' ? ':' : '?';
        }

   		// Determine how to handle the ordering of options and nonoptions.
        Ordering DetermineOptionOrdering(bool posixlyCorrect)
        {
            Ordering result = PERMUTE;
            // Determine how to handle the ordering of options and nonoptions.
            if (Dash())
            {
                result = RETURN_IN_ORDER;
                Inc();
            }
            else if (Plus())
            {
                result = REQUIRE_ORDER;
                Inc();
            }
            else if (posixlyCorrect || !!getenv("POSIXLY_CORRECT"))
            {
                result = REQUIRE_ORDER;
            }
            return result;
        }

    private:

        const char* m_ShortOptionSet;
    };

}}//qor::app

#endif//QOR_PP_H_OPTIONPARSER_SHORTOPTIONINTERPRETTER
