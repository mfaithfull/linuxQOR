cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fcoroutines -fno-threadsafe-statics -fvisibility=hidden")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()
add_subdirectory(mock)

add_executable(QORTest test_main.cpp test_test.cpp test_compiler.cpp 
mock/test_mock.cpp mock/test_isvirtual.cpp mock/test_args.cpp
mock/test_array.cpp mock/test_cfuncs.cpp mock/test_classargs.cpp
mock/test_transaction.cpp mock/test_retval.cpp mock/test_regression_arg_count.cpp
mock/test_ref_args.cpp mock/test_overload.cpp mock/test_outparam.cpp
mock/test_optional.cpp mock/test_objectreturn.cpp mock/test_nevercall.cpp
mock/test_mi.cpp mock/test_membermock.cpp mock/test_inparam.cpp
mock/test_filter.cpp mock/test_exception_quality.cpp mock/test_except.cpp
mock/test_dynamic_cast.cpp mock/test_dontcare.cpp mock/test_do.cpp
mock/test_cv_funcs.cpp mock/test_constref_params.cpp
mock/test_com_support_stdcall.cpp mock/test_zombie.cpp
assert/bool.cpp assert/complex.cpp assert/double.cpp assert/float.cpp
assert/int.cpp assert/iterators.cpp assert/map.cpp assert/multimap.cpp
assert/optional.cpp assert/pointers.cpp assert/priority_queue.cpp
assert/set.cpp assert/shared_ptr.cpp assert/span.cpp
assert/stack.cpp assert/string.cpp assert/string_view.cpp 
assert/unique_ptr.cpp assert/unordered_map.cpp assert/unordered_multimap.cpp 
assert/unordered_set.cpp assert/user_class.cpp assert/user_subject.cpp 
assert/weak_ptr.cpp datastructures/guid.cpp
framework/host.cpp profiling/test_profiling.cpp
factory/test_factory.cpp reference/test_ref.cpp
objectcontext/test_objectcontext.cpp
observer/test_chainlink.cpp observer/test_observer.cpp
delegate/test_delegate.cpp delegate/test_multicastdelegate.cpp framework/test_thread.cpp
interception/test_functioncontext.cpp interception/test_callinterceptor.cpp
interception/test_functioncontextlock.cpp interception/test_parameters.cpp
injection/test_injection.cpp singleton/test_singleton.cpp singleton/test_threadsingleton.cpp
issue/test_issue.cpp error/test_error.cpp framework/test_signals.cpp
reflection/test_reflection.cpp framework/test_pipeline.cpp
framework/test_workflow.cpp application/test_application.cpp
components/test_optionparser.cpp
system/test_filesystem.cpp framework/test_threadpool.cpp
framework/task/asyncautoresetevent_tests.cpp
framework/task/asyncgenerator_tests.cpp
framework/task/asynclatch_tests.cpp
framework/task/asyncmanualresetevent_tests.cpp
framework/task/asyncmutex_tests.cpp
framework/task/cancellationtoken_tests.cpp
framework/task/counted.cpp
framework/task/generator_tests.cpp
framework/task/multiproducersequencer_tests.cpp
framework/task/recursivegenerator_tests.cpp
framework/task/sequencebarrier_tests.cpp
framework/task/sharedtask_tests.cpp
framework/task/singleconsumerasyncautoresetevent_tests.cpp
framework/task/singleproducersequencer_tests.cpp
framework/task/syncwait_tests.cpp
framework/task/task_tests.cpp
framework/task/whenallready_tests.cpp
framework/task/whenalltests.cpp
components/threadmemory/test_threadheap.cpp
components/threadmemory/test_stackpage.cpp
components/threadmemory/test_fastbucket.cpp
components/threadmemory/test_fastheap.cpp
components/threadmemory/test_smallobjectpage.cpp
components/threadmemory/test_smallobjectbucket.cpp
components/threadmemory/test_smallobjectheap.cpp
components/pipeline/test_stdinsource.cpp
components/pipeline/test_stdoutsink.cpp property/test_property.cpp)

target_compile_definitions(QORTest PUBLIC -DQOR_PP_EXECUTABLE)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
target_link_libraries(QORTest QORCompiler qor_host qor_datastructures qor_objectcontext 
qor_thread qor_interception qor_injection qor_sync qor_issue qor_error qor_signals 
qor_pipeline qor_workflow qor_role qor_application qor_optionparser qor_filesystem 
linq_filesystem linq_thread linq_process uring qor_task qor_threadmemory qor_system qor_stdinsource qor_stdoutsink qor_property)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
target_link_libraries(QORTest QORCompiler qor_host qor_datastructures qor_objectcontext 
qor_thread qor_interception qor_injection qor_sync qor_issue qor_error qor_signals 
qor_pipeline qor_workflow qor_role qor_application qor_optionparser qor_filesystem 
winq_filesystem winq_thread winq_process qor_task qor_threadmemory qor_system qor_stdinsource qor_stdoutsink qor_property)
endif()
