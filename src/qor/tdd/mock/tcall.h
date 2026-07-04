// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_TCALL
#define QOR_PP_H_TESTMOCK_TCALL

namespace qor{ namespace mock{
    
    template <typename Y, typename... Args>
    class TCall : public Call
    {
    protected:

        ReturnValueHandle<Y> retVal;
        std::unique_ptr<ComparableTupleBase<Args...>> args;
        std::function<Y(Args...)> doFunctor;
        std::function<bool(Args...)> matchFunctor;
        std::unique_ptr<ExceptionHolder> eHolder;

    public:
        
        void printArgs(std::ostream& os) const override
        {
            if constexpr (sizeof...(Args) == 0)
                os << "()";
            else if (args)
                args->print(os);
            else
                os << "(...)";
        }

        TCall(RegistrationType expect, base_mock* baseMock, std::pair<int, int> index, int X, const char* func, const char* file) : Call(expect, baseMock, index, X, func, file){}

        ~TCall(){}

        // This function checks that, given this is a call for this function, whether this call struct matches your call input.
        bool matches(const std::tuple<Args...>& tupl)
        {
            return (!args || args->equals(tupl)) && (!matchFunctor || invoke(matchFunctor, tupl));
        }

        // This function handles your call. You have to check it at least applies before you call this.
        Y handle(std::tuple<Args...>& callArgs)
        {
            // If we have too many calls, this is the first to handle.
            ++called;
            if (called > expectation.maximum)
            {
                std::stringstream argstr;
                printTuple(argstr, callArgs);
                MockRepoInstanceHolder<0>::reporter->ExpectationExceeded(*this, *MockRepoInstanceHolder<0>::instance, argstr.str(), funcName);
                //TODO: use qor_reporting to raise an error here
                std::abort(); // There's no way to return a Y from here without knowing how to make one. Only way out is an exception, so if you don't have those...
            }

            // Handle in/out arguments
            if (args)
            {
                args->assignInOut(callArgs);
            }

            // If there's a doFunctor to invoke, invoke it now. A retVal overrides the functor return.
            if (doFunctor)
            {
                if (!retVal.set())
                {
                    return invoke(doFunctor, callArgs);
                }
                invoke(doFunctor, callArgs);
            }

            // If we have an exception to throw, let's throw it.
            if (eHolder)
            {
                eHolder->rethrow();
            }

            // If not, we have to have a return value to give back. Void is folded into this as always being set.
            if (!retVal.set())
            {
                std::stringstream argstr;
                printTuple(argstr, callArgs);
                MockRepoInstanceHolder<0>::reporter->NoResultSetUp(*this, *MockRepoInstanceHolder<0>::instance, argstr.str(), funcName);
            }
            return retVal.value();
        }

        template <typename... CArgs>
        TCall<Y, Args...>& With(CArgs... args)
        {
            this->args.reset(new ComparableTuple<std::tuple<Args...>, CArgs...>(args...));
            return *this;
        }

        TCall<Y, Args...>& After(Call& call)
        {
            previousCalls.push_back(&call);
            return *this;
        }

        template <typename T>
        TCall<Y, Args...>& Do(T function)
        {
            doFunctor = function; return *this;
        }

        template <typename T>
        TCall<Y, Args...>& Match(T function)
        {
            matchFunctor = function; return *this;
        }

        template <typename RY, typename OY = Y, typename = typename std::enable_if<!std::is_same<OY, void>::value, bool>::type>
        Call& Return(RY obj)
        {
            retVal = new ReturnValueWrapperCopy<Y, RY>(obj); return *this;
        }
        
        template <typename Ex>
        Call& Throw(Ex exception)
        {
            eHolder.reset(new ExceptionWrapper<Ex>(exception)); return *this;
        }

        template <typename F>
        Call& ThrowFunc(F functor)
        {
            eHolder.reset(new ExceptionFunctor(functor)); return *this;
        }
    };

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_TCALL
