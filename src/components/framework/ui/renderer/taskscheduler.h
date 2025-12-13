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

#ifndef QOR_PP_H_COMPONENTS_FRAMEWORK_UI_RENDERER_TASKSCHEDULER
#define QOR_PP_H_COMPONENTS_FRAMEWORK_UI_RENDERER_TASKSCHEDULER

#include "common/common.h"
#include "inlist.h"

namespace qor{ namespace components{ namespace ui{ namespace renderer{

    using ThreadID = uint8_t;

    struct Task
    {
    public:
        INLIST_ITEM(Task);

        virtual ~Task() = default;
        void done() {}

    protected:
        virtual void run(unsigned tid) = 0;

    private:
        friend struct TaskSchedulerImpl;
    };

    struct TaskScheduler
    {
        static uint32_t threads();
        static void init(uint32_t threads);
        static void term();
        static void request(Task* task);
        static bool onthread();  //figure out whether on worker thread or not
        static ThreadID tid();
    };

    
}}}}//qor::components::ui::renderer

#endif//QOR_PP_H_COMPONENTS_FRAMEWORK_UI_RENDERER_TASKSCHEDULER
