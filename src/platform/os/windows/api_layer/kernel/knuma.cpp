// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"
#include "errorhandler.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "kernel32.h" //kernel32.h must be the first windows header as it's the primary inclusion point for windows.h
#include "../returncheck.h"
#include "../library.h"

namespace qor { namespace win { namespace api {

    BOOL Kernel32::GetLogicalProcessorInformation( ::PSYSTEM_LOGICAL_PROCESSOR_INFORMATION Buffer, PDWORD ReturnLength )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetLogicalProcessorInformation );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, ::PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD >(pFunc, Buffer, ReturnLength);
        return bResult;
    }

    BOOL Kernel32::GetNumaAvailableMemoryNode( UCHAR Node, PULONGLONG AvailableBytes )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetNumaAvailableMemoryNode );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, UCHAR, PULONGLONG >(pFunc, Node, AvailableBytes);
        return bResult;
    }

    BOOL Kernel32::GetNumaHighestNodeNumber( PULONG HighestNodeNumber )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetNumaHighestNodeNumber );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, PULONG >(pFunc, HighestNodeNumber);
        return bResult;
    }

    BOOL Kernel32::GetNumaNodeProcessorMask( UCHAR Node, PULONGLONG ProcessorMask )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetNumaNodeProcessorMask );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, UCHAR, PULONGLONG >(pFunc, Node, ProcessorMask);
        return bResult;
    }

    BOOL Kernel32::GetNumaProcessorNode( UCHAR Processor, PUCHAR NodeNumber )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetNumaProcessorNode );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, UCHAR, PUCHAR >(pFunc, Processor, NodeNumber);
        return bResult;
    }

    BOOL Kernel32::GetNumaProximityNode( ULONG ProximityId, PUCHAR NodeNumber )
    {
        qor_pp_fcontext;
        Win32ErrorHandler _;
        qor_pp_useswinapi(kernel32, GetNumaProximityNode );
        CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< BOOL, ULONG, PUCHAR >(pFunc, ProximityId, NodeNumber);
        return bResult;
    }

}}}//qor::win::api