// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DETECTTOOLCHAIN
#define QOR_PP_H_DETECTTOOLCHAIN

#undef qor_pp_compiler                      //Don't allow the compiler to be pre-defined. It is whatever is really in use at the time
#include "detectcompiler.h"	                //Detect which compiler toolchain is being used
#include "../../macros/preprocessor.h"		//Setup the pre-processor definitions
#include "compilerpaths.h"		            //Determine which headers will be included to match the compiler in use

#endif//QOR_PP_H_DETECTTOOLCHAIN
