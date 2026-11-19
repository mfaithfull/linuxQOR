// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_RUNABLE
#define QOR_PP_H_RUNABLE

#include <functional>
#include <iostream>
#include "src/qor/memory/reference/newref.h"

namespace qor{ namespace app {

    //Abstract interface intended to be included and implemented by main loop implementors.
    class IRunable
    {
    public:

        IRunable() = default;
        virtual ~IRunable() = default;

        virtual int Run(void) = 0;
    };

    //Wrapper to turn a lambda or std::function into a runable object
    class RunableFunc : public IRunable
    {
    public:

        RunableFunc( std::function< int(void) >&& func ) : m_func(std::move(func)){}

        virtual ~RunableFunc() = default;

        virtual int Run(void)
        {
            try
            {
                return m_func();
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
            catch(...)
            {
                std::cerr << "Unhandled exception." << std::endl;
            }
            return EXIT_FAILURE;
        }

    private:

        std::function< int(void) > m_func;
    };

    //Get a runable object from a lambda or std::function
    template<typename Tlambda>
    ref_of<IRunable>::type make_runable(Tlambda&& func)
    {
        return new_ref<RunableFunc>(func).template AsRef<IRunable>();
    }

}}//qor::app

#endif//QOR_PP_H_RUNABLE
