// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OPTIONPARSER_NONOPTTRACKER
#define QOR_PP_H_OPTIONPARSER_NONOPTTRACKER

#include "argument_iterator.h"

namespace qor{ namespace app{

    //Keeps track of any non option parameters, juggling them to the end of the list
    //Augments the functionality of the argument iterator
    class NonOptTracker
    {
        public:

        NonOptTracker(arg_iter& it) : m_it(it), m_FirstNonOpt(0), m_LastNonOpt(0) {}

        void Init()
        {
            m_FirstNonOpt = m_LastNonOpt = m_it.Set(1);;
        }

  		// Set the next-arg-index to point at the non-options that we previously skipped, so the caller will digest them.
        void HandleEndOfOptions()
        {
            if (m_FirstNonOpt != m_LastNonOpt)
            {
                m_it.Set(m_FirstNonOpt);
            }    
        }

        void HandleDoubleDash()
        {
            m_it.Next();
            if (m_FirstNonOpt != m_LastNonOpt && m_LastNonOpt != m_it.Index())
            {
                Exchange();
            }
            else if (m_FirstNonOpt == m_LastNonOpt)
            {
                m_FirstNonOpt = m_it.Index();
            }
            m_LastNonOpt = m_it.End();
            m_it.SetEnd();
        }
    
        void Exchange()
        {
            int bottom = m_FirstNonOpt;
            int middle = m_LastNonOpt;
            int top = m_it.Index();
            /* Exchange the shorter segment with the far end of the longer segment.
               That puts the shorter segment into the right place.
               It leaves the longer segment in the right place overall,
               but it consists of two parts that need to be swapped next.  */
            while (top > middle && middle > bottom)
            {
                if (top - middle > middle - bottom)
                {
                    // Bottom segment is the short one.
                    int len = middle - bottom;
                    int i;
                    // Swap it with the top part of the top segment.
                    for (i = 0; i < len; i++)
                    {
                        m_it.SwapTop(bottom, middle, top, i);
                    }
                    // Exclude the moved bottom segment from further swapping.
                    top -= len;
                }
                else
                {
                    // Top segment is the short one.
                    int len = top - middle;
                    int i;
                    // Swap it with the bottom part of the bottom segment.
                    for (i = 0; i < len; i++)
                    {
                        m_it.SwapBottom(bottom, middle, i);
                    }
                    // Exclude the moved top segment from further swapping.
                    bottom += len;
                }
            }
            // Update records for the slots the non-options now occupy.
            m_FirstNonOpt += (m_it.Index() - m_LastNonOpt);
            m_LastNonOpt = m_it.Index();
        }
    
        void Permute()
        {
            // If we have just processed some options following some non-options,
            // exchange them so that the options come first.
            if (m_FirstNonOpt != m_LastNonOpt && m_LastNonOpt != m_it.Index())
            {
                Exchange();
            }
            else if (m_LastNonOpt != m_it.Index())
            {
                m_FirstNonOpt = m_it.Index();
            }
    
            m_it.SkipNonOptions();
            m_LastNonOpt = m_it.Index();
        }
    
        void ParseNextOption()
        {
            // Advance to the next ARGV-element.
            // Give FIRST_NONOPT & LAST_NONOPT rational values if OPTIND has been moved back by the user (who may also have changed the arguments).
            if (m_LastNonOpt > m_it.Index())
            {
                m_LastNonOpt = m_it.Index();
            }
            if (m_FirstNonOpt > m_it.Index())
            {
                m_FirstNonOpt = m_it.Index();
            }        
        }

    private:

        arg_iter& m_it;
		int m_FirstNonOpt;//'first_nonopt' is the index in ARGV of the first non-options that have been skipped
		int m_LastNonOpt;//'last_nonopt' is the index after the last of them.
    };

}}//qor::app

#endif//QOR_PP_H_OPTIONPARSER_NONOPTTRACKER
