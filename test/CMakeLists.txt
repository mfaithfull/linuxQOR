cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_COMPILE_OPTIONS_RTTI)
set(CMAKE_CXX_RTTI ON)
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
mock/test_com_support_stdcall.cpp mock/test_zombie.cpp)
target_link_libraries(QORTest QORCompiler)
