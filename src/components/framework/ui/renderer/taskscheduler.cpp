/*
 * Copyright (c) 2020 - 2025 the ThorVG project. All rights reserved.

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "src/configuration/configuration.h"
#include "common/array.h"
#include "inlist.h"
#include "taskscheduler.h"

namespace qor{ namespace components{ namespace ui{ namespace renderer{

    struct TaskSchedulerImpl
    {
        TaskSchedulerImpl(TVG_UNUSED uint32_t threadCnt) {}
        void request(Task* task) { task->run(0); }
        uint32_t threadCnt() { return 0; }
    };

    static TaskSchedulerImpl* _inst = nullptr;
    static ThreadID _tid;   //dominant thread id

    void TaskScheduler::init(uint32_t threads)
    {
        if (_inst) return;
        _inst = new TaskSchedulerImpl(threads);
        _tid = tid();
    }

    void TaskScheduler::term()
    {
        delete(_inst);
        _inst = nullptr;
    }

    void TaskScheduler::request(Task* task)
    {
        if (_inst) _inst->request(task);
    }

    uint32_t TaskScheduler::threads()
    {
        return _inst ? _inst->threadCnt() : 0;
    }

    bool TaskScheduler::onthread()
    {
        return _tid != tid();
    }

    ThreadID TaskScheduler::tid()
    {
        return 0;
    }    

}}}}//qor::components::ui::renderer
