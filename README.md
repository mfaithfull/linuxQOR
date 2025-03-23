# linuxQOR
Querysoft Open Runtime development - Linux

A cross platform modular C++ framework

Author:	Matthew Faithfull:	mfaithfull at btopenworld.com

For licensing please see LICENSE
For development status please see STATUS.md
For acknowledgments please see CREDITS.md
For FAQs please see FAQ.md
For an explanation of the source layout please see LAYOUT.md
For latest changes please see CHANGES.md

The QOR is intended to be self aware software with very strong separation of concerns.
For what this means and why you'd want that in a framework see the article KEY_CONCEPTS.md

The outcomes of this approach are intended to be:


The QOR should be testable

The QOR should be easy to extend

The QOR should be flexible. Use what you need and the rest won't bother you with breaking changes.

The QOR will not give you the highest possible performance implementation
however you'll get a reliable working implementation sooner than anything more optimised.


Development process:

The intent is to use test driven development (TDD) to ensure the operation of the QOR. 
In particular given the intent to support multiple compilers and target platforms, extensive testing is essential

Language:

The QOR is a C++ project. If it can be done in C++ or C then that's the default way to do it.
The particular flavour of C++ used will be opinionated and therefore almost always wrong.
A modern C++20 compiler will be needed becuse there are things I've wanted to do for years that can only be done with recent compilers. 
We'll still be using old fashioned stuff like macros because I'm old. 
C++ modules? Maybe but not yet.
Pre compiled headers (PCHs)? Yes, heavily. Both to speed up builds and to prepare for C++ modules in the future.

A note not to confuse C++ modules (A technology we aren't using yet) with QOR modules. 
The QOR module terminonolgy will be used heavily to refer to what are .DLL(s) in Windows, .so(s) in Linux, runtime loadable libraries.
