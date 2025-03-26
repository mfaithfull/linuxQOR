
build - The sources for the QOR build system
src - The sources for the QOR
test - The sources for tests for the QOR

src/configuration - main project header and overall configuration headers
src/macros - preprocessor header library
src/platform - platform support modules
src/qor - the ubiqitous modules and libraries

src/platform/architecture - architecture support module for x86, ARM, RISC-V, 32-bit, 64-bit, big/little endian, etc.
src/platform/compiler - compiler support module for GCC, MSVC, Clang
src/platform/os - operating system support module for Linux, Windows, Mac

src/qor/assert - fluent assertions header library
src/qor/mock - mock header library for testing with mocks/fakes
src/qor/test - unit testing header library

test - self tests for qor_test
test/assert - tests for qor_assert
test/mock - tests for qor_mock