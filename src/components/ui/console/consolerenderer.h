// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_CONSOLE_RENDERER
#define QOR_PP_H_COMPONENTS_CONSOLE_RENDERER

#include <map>

#include "src/qor/memory/injection/typeidentity.h"
#include "tokenbuffer.h"
#include "console.h"

/*Turn a token buffer into WriteLine calls with strings of chars*/

namespace qor { namespace components {

    class qor_pp_module_interface(QOR_CONSOLE) ConsoleRenderer
    {
    public:

        ConsoleRenderer();
        ~ConsoleRenderer();

        void SwapTokenBuffer(TokenBuffer*& old, TokenBuffer* newBuffer);
        char_t CharLookup(int token);
        void Render();

    private:
        static std::map<int,char_t> s_charLookup;
        ref_of<Console>::type m_console;
        TokenBuffer* m_buffer;
    };

}}//qor::components

#endif//QOR_PP_H_COMPONENTS_CONSOLE_RENDERER
