
#ifndef QOR_PP_H_SDK_USING_QOR
#define QOR_PP_H_SDK_USING_QOR

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/delegate/delegate.h"
#include "src/qor/observer/observer.h"
#include "src/qor/instance/instance.h"
#include "src/qor/property/property.h"
#include "src/qor/reflection/reflection.h"
#include "src/qor/test/test.h"
#include "src/qor/mock/mocks.h"
#include "src/qor/factory/factory.h"
#include "src/qor/datastructures/guid.h"
#include "src/qor/profiling/profiling.h"
#include "src/qor/assert/assert.h"
#include "src/qor/objectcontext/anyobject.h"
#include "src/qor/memory/memory.h"
#include "src/qor/injection/diwrapper.h"
#include "src/qor/interception/functioncontext.h"
#include "src/qor/interception/callcontext.h"
#include "src/framework/thread/currentthread.h"
#include "src/qor/reference/newref.h"
#include "src/qor/issue/issue.h"
#include "src/qor/error/error.h"
#include "src/qor/error/handler.h"
#include "src/qor/error/defaulterrorhandler.h"
#include "src/qor/log/debug.h"
#include "src/qor/log/informative.h"
#include "src/qor/log/impactful.h"
#include "src/qor/log/important.h"
#include "src/qor/log/imperative.h"
#include "src/qor/log/defaultloghandler.h"
#include "src/qor/sync/sync.h"
#include "src/qor/sync/mutex.h"
#include "src/qor/sync/recursive_mutex.h"
#include "src/qor/sync/nullsection.h"

using namespace qor;
using namespace qor::log;

#endif//QOR_PP_H_SDK_USING_QOR