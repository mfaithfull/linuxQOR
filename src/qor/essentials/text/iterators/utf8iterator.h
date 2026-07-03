// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_UTF8ITERATOR
#define QOR_PP_H_TEXT_UTF8ITERATOR

#include "src/qor/essentials/iterators/iterators.h"

namespace qor
{
    constexpr uint8_t codepoint_decoded_length[256] = 
    {   /* Basic Latin */
        1, 1, 1, 1, 1, 1, 1, 1, /* 0x00 - 0x07 */
        1, 1, 1, 1, 1, 1, 1, 1, /* 0x08 - 0x0F */
        1, 1, 1, 1, 1, 1, 1, 1, /* 0x10 - 0x17 */
        1, 1, 1, 1, 1, 1, 1, 1, /* 0x18 - 0x1F */
        1, 1, 1, 1, 1, 1, 1, 1, /* 0x20 - 0x27 */
        1, 1, 1, 1, 1, 1, 1, 1, /* 0x28 - 0x2F */
        1, 1, 1, 1, 1, 1, 1, 1, /* 0x30 - 0x37 */
        1, 1, 1, 1, 1, 1, 1, 1, /* 0x38 - 0x3F */
        1, 1, 1, 1, 1, 1, 1, 1, /* 0x40 - 0x47 */
        1, 1, 1, 1, 1, 1, 1, 1, /* 0x48 - 0x4F */
        1, 1, 1, 1, 1, 1, 1, 1, /* 0x50 - 0x57 */
        1, 1, 1, 1, 1, 1, 1, 1, /* 0x58 - 0x5F */
        1, 1, 1, 1, 1, 1, 1, 1, /* 0x60 - 0x67 */
        1, 1, 1, 1, 1, 1, 1, 1, /* 0x68 - 0x6F */
        1, 1, 1, 1, 1, 1, 1, 1, /* 0x70 - 0x77 */
        1, 1, 1, 1, 1, 1, 1, 1, /* 0x78 - 0x7F */

        /* Malformed continuation byte */
        0, 0, 0, 0, 0, 0, 0, 0, /* 0x80 - 0x87 */
        0, 0, 0, 0, 0, 0, 0, 0, /* 0x88 - 0x8F */
        0, 0, 0, 0, 0, 0, 0, 0, /* 0x90 - 0x97 */
        0, 0, 0, 0, 0, 0, 0, 0, /* 0x98 - 0x9F */
        0, 0, 0, 0, 0, 0, 0, 0, /* 0xA0 - 0xA7 */
        0, 0, 0, 0, 0, 0, 0, 0, /* 0xA8 - 0xAF */
        0, 0, 0, 0, 0, 0, 0, 0, /* 0xB0 - 0xB7 */
        0, 0, 0, 0, 0, 0, 0, 0, /* 0xB8 - 0xBF */

        /* Two bytes */
        2, 2, 2, 2, 2, 2, 2, 2, /* 0xC0 - 0xC7 */
        2, 2, 2, 2, 2, 2, 2, 2, /* 0xC8 - 0xCF */
        2, 2, 2, 2, 2, 2, 2, 2, /* 0xD0 - 0xD7 */
        2, 2, 2, 2, 2, 2, 2, 2, /* 0xD8 - 0xDF */

        /* Three bytes */
        3, 3, 3, 3, 3, 3, 3, 3, /* 0xE0 - 0xE7 */
        3, 3, 3, 3, 3, 3, 3, 3, /* 0xE8 - 0xEF */

        /* Four bytes */
        4, 4, 4, 4, 4, 4, 4, 4, /* 0xF0 - 0xF7 */

        /* Five bytes */
        5, 5, 5, 5,             /* 0xF8 - 0xFB */

        /* Six bytes */
        6, 6,                   /* 0xFC - 0xFD */

        /* Invalid */
        7, 7                    /* 0xFE - 0xFF */
    };

    struct AdvanceUTF8ly
    {
        template<typename iteratorT>
        static void advance(iteratorT& iter, const ptrdiff_t& howManyStepsToAdvanceIter, const iteratorT& beginIter, const iteratorT& endIter, const ptrdiff_t& distanceFromBeginToIter, const ptrdiff_t& distanceFromIterToEnd)
        {
            if(howManyStepsToAdvanceIter > 0)
            {
                if(howManyStepsToAdvanceIter > distanceFromIterToEnd)
                {
                    iter = endIter;
                }
                else
                {
                    ptrdiff_t remainingSteps = howManyStepsToAdvanceIter;
                    while(remainingSteps > 0)
                    {
                        typename iteratorT::value_type value = *iter;//should be a char for UTF-8
                        if(value <= 0x7F)
                        {
                            --remainingSteps;
                            std::advance(iter,1);
                        }
                        else
                        {
                            uint8_t decoded_length = codepoint_decoded_length[value];
                            if(decoded_length == 0 || decoded_length > 6)
                            {   //malformed UTF-8 sequence, skip
                                std::advance(iter,1);
                            }
                            else
                            {
                                --remainingSteps;
                                if(decoded_length >= distanceFromIterToEnd)
                                {
                                    iter = endIter;
                                }
                                else
                                {
                                    std::advance(iter,decoded_length);
                                }
                            }
                        }
                    }
                }
            }
            else if(howManyStepsToAdvanceIter < 0)
            {
                if((-howManyStepsToAdvanceIter) > distanceFromBeginToIter)
                {
                    iter = beginIter;
                }
                else
                {
                    ptrdiff_t remainingSteps = -howManyStepsToAdvanceIter;
                    while(remainingSteps > 0)
                    {
                        typename iteratorT::value_type value = *iter--;
                        if(value < 0x80 || value > 0xBF)//ignore continuation bytes
                        {   
                            --remainingSteps;                         
                        }
                    }
                }
            }
        }

        template<typename iteratorT>
        static ptrdiff_t distance(const ptrdiff_t& distanceFromBeginToIter1, const ptrdiff_t& distanceFromBeginToIter2, const ptrdiff_t& /*distanceFromIterToEnd1*/, const ptrdiff_t& /*distanceFromIterToEnd2*/)
        {
            //Pointer distances will always be >= character distances 
            //and they will always reach zero at the same time 
            //so there is no need to change anything here. Any ineffciency
            //will be less than the cost of counting characters
            return (distanceFromBeginToIter2 - distanceFromBeginToIter1);
        }
    };

    template<typename containerT>
    class UTF8Iterator : public Iterator<containerT,AdvanceUTF8ly,BeginEnd>{using Iterator<containerT,AdvanceUTF8ly,BeginEnd>::Iterator;};

    template<typename containerT>
    class UTF8ConstIterator : public Iterator<containerT,AdvanceUTF8ly,cBeginEnd>{using Iterator<containerT,AdvanceUTF8ly,cBeginEnd>::Iterator;};

    template<typename containerT>
    class UTF8ReverseIterator : public Iterator<containerT,AdvanceUTF8ly,rBeginEnd>{using Iterator<containerT,AdvanceUTF8ly,rBeginEnd>::Iterator;};

    template<typename containerT>
    class UTF8ConstReverseIterator : public Iterator<containerT,AdvanceUTF8ly,crBeginEnd>{using Iterator<containerT,AdvanceUTF8ly,crBeginEnd>::Iterator;};

}//qor

#endif//QOR_PP_H_TEXT_UTF8ITERATOR