We want to be test driven so we need to start with some tests and for that we need a test framework.
Givem we don't have a test framework then we'll need to start with a test for a test framework we don't have yet, 
so we can start writing the test framework.

Decisions:

The license/copyright header will be included in all files
qor test will be a header library based on YAFFUT 
All QOR preprocessor macros will be prefixed qor_pp_
namespace for QOR will be qor
namespace for QOR Test will be qor::test
There will be a configuration folder to contain the build time configuration settings for the overall project
The global PCH will be invoked by including the configuration/configuration.h header

We'll use old fashioned portable include guards of the form 

#ifndef QOR_PP_H_TEST
#define QOR_PP_H_TEST
//...
#endif//QOR_PP_H_TEST

