// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_UTF16ITERATOR
#define QOR_PP_H_TEXT_UTF16ITERATOR

#include "src/qor/essentials/iterators/iterators.h"

namespace qor
{
    struct AdvanceUTF16ly
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
                        typename iteratorT::value_type value = *iter;                        
                        std::advance(iter, (value < 0xD800 || value > 0xDFFF) ? 1 : 2 );
                        --remainingSteps;
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
                        if(value < 0xD800 || value > 0xDFFF)//ignore continuation units
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
    class UTF16Iterator : public Iterator<containerT,AdvanceUTF16ly,BeginEnd>{using Iterator<containerT,AdvanceUTF16ly,BeginEnd>::Iterator;};

    template<typename containerT>
    class UTF16ConstIterator : public Iterator<containerT,AdvanceUTF16ly,cBeginEnd>{using Iterator<containerT,AdvanceUTF16ly,cBeginEnd>::Iterator;};

    template<typename containerT>
    class UTF16ReverseIterator : public Iterator<containerT,AdvanceUTF16ly,rBeginEnd>{using Iterator<containerT,AdvanceUTF16ly,rBeginEnd>::Iterator;};

    template<typename containerT>
    class UTF16ConstReverseIterator : public Iterator<containerT,AdvanceUTF16ly,crBeginEnd>{using Iterator<containerT,AdvanceUTF16ly,crBeginEnd>::Iterator;};

}//qor

#endif//QOR_PP_H_TEXT_UTF16ITERATOR