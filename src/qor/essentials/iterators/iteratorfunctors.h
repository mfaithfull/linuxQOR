// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ITERATORS_FUNCTORS
#define QOR_PP_H_ITERATORS_FUNCTORS

#include <stddef.h>

namespace qor{

    // Define "begin" and "end" functors to use with smart iterators
    struct BeginEnd
    {
        template<typename containerT>
        static auto begin(containerT& container)->decltype(container.begin()){return container.begin();}

        template<typename containerT>
        static auto end(containerT& container)->decltype(container.end()){return container.end();}
    };


    struct cBeginEnd
    {
        template<typename containerT>
        static auto begin(const containerT& container)->decltype(container.cbegin()){return container.cbegin();}

        template<typename containerT>
        static auto end(const containerT& container)->decltype(container.cend()){return container.cend();}
    };

    struct rBeginEnd
    {
        template<typename containerT>
        static auto begin(containerT& container)->decltype(container.rbegin()){return container.rbegin();}

        template<typename containerT>
        static auto end(containerT& container)->decltype(container.rend()){return container.rend();}
    };

    struct crBeginEnd
    {
        template<typename containerT>
        static auto begin(const containerT& container)->decltype(container.crbegin()){return container.crbegin();}

        template<typename containerT>
        static auto end(const containerT& container)->decltype(container.crend()){return container.crend();}
    };

    struct AdvanceLinearly
    {
        template<typename iteratorT>
        static void advance(iteratorT& iter,
                            const ptrdiff_t& howManyStepsToAdvanceIter,
                            const iteratorT& beginIter,
                            const iteratorT& endIter,
                            const ptrdiff_t& distanceFromBeginToIter,
                            const ptrdiff_t& distanceFromIterToEnd)
        {
            if(howManyStepsToAdvanceIter > 0)
            {
                if(howManyStepsToAdvanceIter > distanceFromIterToEnd)
                {
                    iter = endIter;
                }
                else
                {
                    std::advance(iter,howManyStepsToAdvanceIter);
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
                    std::advance(iter,howManyStepsToAdvanceIter);
                }
            }
        }

        template<typename iteratorT>
        static ptrdiff_t distance(const ptrdiff_t& distanceFromBeginToIter1,
                                    const ptrdiff_t& distanceFromBeginToIter2,
                                    const ptrdiff_t& distanceFromIterToEnd1,
                                    const ptrdiff_t& distanceFromIterToEnd2)
        {
            return (distanceFromBeginToIter2 - distanceFromBeginToIter1);
        }
    };

    struct AdvanceCircularly
    {
        template<typename iteratorT>
        static void advance(iteratorT& iter,
                            const ptrdiff_t& howManyStepsToAdvanceIter,
                            const iteratorT& beginIter,
                            const iteratorT& endIter,
                            const ptrdiff_t& distanceFromBeginToIter,
                            const ptrdiff_t& distanceFromIterToEnd)
        {
            if(howManyStepsToAdvanceIter > 0)
            {
                if(howManyStepsToAdvanceIter < distanceFromIterToEnd)
                {
                    std::advance(iter,howManyStepsToAdvanceIter);
                }
                else
                {
                    iter = beginIter;
                    std::advance(iter,
                                (howManyStepsToAdvanceIter - distanceFromIterToEnd) %
                                (distanceFromBeginToIter + distanceFromIterToEnd));
                }
            }
            else if(howManyStepsToAdvanceIter < 0)
            {
                if((-howManyStepsToAdvanceIter) < distanceFromBeginToIter)
                {
                    std::advance(iter,howManyStepsToAdvanceIter);
                }
                else
                {
                    iter = beginIter;
                    std::advance(iter,
                                (distanceFromBeginToIter + distanceFromIterToEnd) +
                                (howManyStepsToAdvanceIter - distanceFromIterToEnd) %
                                (distanceFromBeginToIter + distanceFromIterToEnd));
                }
            }
        }

        template<typename iteratorT>
        static ptrdiff_t distance(const ptrdiff_t& distanceFromBeginToIter1, const ptrdiff_t& distanceFromBeginToIter2,
                                    const ptrdiff_t& distanceFromIterToEnd1, const ptrdiff_t& distanceFromIterToEnd2)
        {
            if(distanceFromBeginToIter1 <= distanceFromBeginToIter2)
            {
                return ( distanceFromBeginToIter2 - distanceFromBeginToIter1 );
            }
            else
            {
                return ( distanceFromIterToEnd1 + distanceFromBeginToIter2);
            }
        }
    };

}//qor

#endif//QOR_PP_H_ITERATORS_FUNCTORS
