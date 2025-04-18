cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-threadsafe-statics -fvisibility=hidden")
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
system/test_filesystem.cpp framework/test_threadpool.cpp)

target_compile_definitions(QORTest PUBLIC -DQOR_PP_EXECUTABLE)

target_link_libraries(QORTest QORCompiler qor_host qor_datastructures qor_objectcontext 
qor_thread qor_interception qor_injection qor_sync qor_issue qor_error qor_signals 
qor_pipeline qor_workflow qor_role qor_application qor_optionparser qor_filesystem 
linq_filesystem linq_thread linq_process)
