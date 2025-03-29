
build - The sources for the QOR build system
src - The sources for the QOR
test - The sources for tests for the QOR

src/configuration - main project header and overall configuration headers
src/framework - structural elements from which to put together an application
src/macros - preprocessor header library
src/platform - platform support modules
src/qor - the ubiqitous modules and libraries

src/framework/host - library that forms part of every top level executable that makes it a QOR executable

src/platform/architecture - architecture support module for x86, ARM, RISC-V, 32-bit, 64-bit, big/little endian, etc.
src/platform/compiler - compiler support module for GCC, MSVC, Clang
src/platform/os - operating system support module for Linux, Windows, Mac

src/qor/assert - fluent assertions header library
src/qor/datastructures - basic POD types, chains etc. Used everywhere.
src/qor/factory - object factories and factory traits
src/qor/injection - dependency injection support
src/qor/instance - object instancers and instancer traits
src/qor/memory - memory allocation
src/qor/mock - mock header library for testing with mocks/fakes
src/qor/module - a static library for inclusion in every QOR loadable module. Also provides Library class for static library tagging
src/qor/profiling - a simple chrono timer profiler for function call tree timing
src/qor/reference - smart references using the instancing, factory and memory support
src/qor/sync - synchronization primitives
src/qor/test - unit testing header library

test - self tests for qor_test
test/assert - tests for qor_assert
test/datastructures - tests for qor_datastructures
test/framework - test for QOR Framework libraries
test/mock - tests for qor_mock