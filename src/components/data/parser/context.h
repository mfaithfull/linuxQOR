// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PARSER_CONTEXT
#define QOR_PP_H_COMPONENTS_PARSER_CONTEXT

#include "src/platform/compiler/compiler.h"

namespace qor { namespace components { namespace parser {

    //Taps a parser into a pipeline buffer, keeping track where the parser is in the buffer
    class qor_pp_module_interface(QOR_PARSER) Context
    {
    public:

        Context();
        Context(Context&& src);
        Context(byte* data, size_t itemCount);
        bool GetOctet(byte*& data);
        bool PutOctet(byte data);
        bool ConsumeOctet();
        size_t GetPosition();
        void SetData(byte* data, size_t itemCount);
        bool HasUnparsedData();
        bool HasSpace();
        void Diagnostic();
        void Reset();
        
    private:
    
        byte* m_octetStream{ nullptr };
        size_t m_position{ 0 };
        size_t m_size{ 0 };

    };

}}}//qor::components::parser

#endif//QOR_PP_H_COMPONENTS_PARSER_CONTEXT
