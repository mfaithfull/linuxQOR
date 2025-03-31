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

In order for qor_test to report the names of types it needs to demangle them which is a compiler specific function.
Therefore we need to delve into compiler detection and abstraction early on.
This in turn mens we need some preprocessor library basics like qor_pp_stringize and qor_pp_cat for which we'll use the Boost preprocessor library as a source
The compiler detection can also be sourced from Boost

The basic methid is to determine which Compiler is in use. 
Set a macro based on that
Use the macro to form a path to the files supporting that compiler and include them
This relies on
#include A_MACRO_THAT_EXPANDS_TO_A_VALID_INCLUDE_PATH_STRING
working which fortunately it does as long as the expansion is correct.

We also need the fundamental configuration.h header which will be included everywhere and from which we'll eventually get the PCH
All translation units will include configuration.h first and configuration.h will define all the global constants for the project, including macros

Everything in now on main. At head development from now on, at least for the foreseable

Working towards a makefile for the tests, much as I loath make and makefiles, it's the shortest path.

Test suite builds and passes 1 self test. First minor milestone reached.

Sketched in the QOR Mock header library based on HippoMocks. It builds cleanly but is still scratch code.

Now I need to abstract out the Compiler, OS and Architecture dependencies into those libraries but first need to wrap it in some tests to make sure it continues to operate as it gets refactored.

Interesting to note that GCC doesn't generate type information for classes with missing function implementations but it does all the compilation if the missing function isn't referenced.

Porting in assertcc a fluent assertions library. This will give us tests with nice assertions and mocking
When all the tests are in place for this. There are two ways we could go.

More header only libraries for delegates, factories and references

Start on the QOR module system so that we can build delf describing, runtime loadable, versioned, modules. Maybe ones that test themselves.

Decided to start on the QOR module system. A few more integrated docs added first to help with what belong where in the project structure.

Added a GUID class as the first item in a library of basic data structures.

qor_datastructures and qor_host are now separate libraries to prevent multiply defined symbols and we have our first element of the Framework.

The QOR Framework is the actual application building blocks, like and application class, startup up, main loops, state machines etc. 
Most of these will be larger scale, smaller number of instances, longer lived, objects like pipelines of GUIs

The Host is part of the Framework. There wil be only one, a singleton, and it will get linked into the main executable.
We need this early on because the Host will own the application wide type registry that will support our dependency injection model.

Had a go at adding smart references. Requires a lot of things to be in place to get it working but Ref<T> is up and testable.

Implemented the self assembling module system for static libraries and executables. Dynamic libraries are next.

Module stitching is now operational for static and dynamic libraries. It requires a small amount of boiler plate in the final executable but this can likely be removed in favout of linking it with a tiny static exebootstrap library

Type Registration may need to be moved to its own module as the host module is the root dynamic dependency and can't contain data types from QOR modules.

The next big step is threads which could reasonably be part of the 'everywhere' libraries or the framework but we'll put them in the framework as the intention is that they be used as stuctural elements by top level applications.
One more everywhere tool to do first which is delegates, the corallary to observers

Last day of March and we've hit the wall with the native GCC on Ubuntu 20.04. Normal service will resume after and upgrade to GCC 13 or a new Arch based system, whichever works.
