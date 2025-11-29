#ifndef QOR_PP_H_SDK_USING_FRAMEWORK
#define QOR_PP_H_SDK_USING_FRAMEWORK

#include "using_qor.h"

#include "src/framework/application/application_builder.h"
#include "src/framework/role/ifeature.h"
#include "src/framework/role/role.h"
#include "src/framework/thread/currentthread.h"
#include "src/framework/thread/icurrentthread.h"
#include "src/framework/thread/icurrentprocess.h"
#include "src/framework/thread/thread.h"
#include "src/framework/thread/threadpool.h"
#include "src/framework/task/task.h"
#include "src/framework/task/syncwait.h"
#include "src/framework/workflow/workflow.h"
#include "src/framework/pipeline/podbuffer.h"
#include "src/framework/pipeline/pipeline.h"
#include "src/framework/pipeline/inlinefilter.h"
#include "src/framework/pipeline/plug.h"
#include "src/framework/asyncioservice/asyncioservice.h"

using namespace qor::framework;
using namespace qor::pipeline;
using namespace qor::workflow;

#endif//QOR_PP_H_SDK_USING_FRAMEWORK