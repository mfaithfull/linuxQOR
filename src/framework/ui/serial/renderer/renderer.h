// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_SERIALCONSOLE_RENDERER
#define QOR_PP_H_FRAMEWORK_UI_SERIALCONSOLE_RENDERER

#include <map>

#include "src/qor/memory/injection/typeidentity.h"
#include "../compositor/tokenbuffer.h"

/*Turn a token buffer into WriteLine calls with strings of chars*/

namespace qor { namespace ui { 

    class qor_pp_module_interface(QOR_SERIALCONSOLE) SerialConsole;

    namespace sercon {

    class qor_pp_module_interface(QOR_SERIALCONSOLE) Renderer
    {
    public:

        Renderer(SerialConsole* display);
        ~Renderer();

        void SwapTokenBuffer(TokenBuffer*& old, TokenBuffer* newBuffer);
        char_t CharLookup(int token);
        int LookupTokenForChar(char_t c);
        void Present();

        void DrawToken(int x, int y, int token);
        void DrawCharacter(int x, int y, char_t c);
        void DrawWindow(int x, int y, int width, int height);
        void ClearWindow(int x, int y, int width, int height);
        void FillWindow(int x, int y, int width, int height, int token);
        void DrawLabel(int x, int y, string_t label);
        void DrawString(int x, int y, string_t text);
        void DrawInternalHLine(int x, int y, int length);

    private:    
        SerialConsole* m_display;
        TokenBuffer* m_buffer;
        static std::map<int,char_t> s_charLookup;
        static std::map<char_t, int> s_LookupToken;        
    };

}}}//qor::ui::sercon

#endif//QOR_PP_H_FRAMEWORK_UI_SERIALCONSOLE_RENDERER
