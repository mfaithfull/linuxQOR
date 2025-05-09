// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "../../src/configuration/configuration.h"
#include "../../src/qor/test/test.h"
#include "../../src/qor/assert/assert.h"
#include "../../src/qor/module/moduleregistry.h"
#include "../../src/qor/injection/typeidentity.h"
#include "../../src/qor/objectcontext/anyobject.h"
#include "../../src/framework/thread/currentthread.h"
#include "../../src/framework/thread/thread.h"
#include "../../src/qor/reference/newref.h"

using namespace qor;
using namespace qor::test;
using namespace qor::framework;

struct ThreadTestSuite{};

class Worker : public Thread
{
    public:

    virtual void Run()
    {
        std::cout << " Worker thread executing.";
    }
};

class IntrospectiveWorker : public Thread
{
    public:

    virtual void Run()
    {
        const CurrentThread& worker_thread_in_progress = CurrentThread::GetCurrent();
        auto id = worker_thread_in_progress.GetID();
        worker_thread_in_progress.Sleep(1);
        worker_thread_in_progress.Yield();
        worker_thread_in_progress.Context();
        std::cout << " Worker thread " << id << " has done it's job.";
    }
};

class SelfAwareWorker : public Thread
{
    public:

    virtual void Run()
    {
        ThreadContext& context = CurrentThread::GetCurrent().Context();
        std::cout << " Worker thread has access to its mutable context.";
    }
};

qor_pp_test_suite_case(ThreadTestSuite, canDefaultConstructThread)
{
    Thread defaultThread;
    qor_pp_assert_that(&defaultThread).isNotNull();
}

qor_pp_test_suite_case(ThreadTestSuite, canRunDerivedThreadType)
{
    auto worker = new_ref<Worker>();
    worker->Join();
}

qor_pp_test_suite_case(ThreadTestSuite, canAccessImmutableThreadContextFromWithinThread)
{
    auto worker = new_ref<IntrospectiveWorker>();
    worker->Join();
}

qor_pp_test_suite_case(ThreadTestSuite, canAccessMutableThreadContextFromWithinThread)
{
    auto worker = new_ref<SelfAwareWorker>();
    worker->Join();
}
