// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef QOR_PP_H_COMPONENTS_CONSOLE_VIEW
#define QOR_PP_H_COMPONENTS_CONSOLE_VIEW

#include "consolerenderer.h"

//Owns token buffers
//Takes each of the Console View Model items and paints it into the next token buffer

namespace qor { namespace components {

    class qor_pp_module_interface(QOR_CONSOLE) ConsoleView
    {
    public:

        ConsoleView();
        ~ConsoleView();

        unsigned int GetWidth();
        unsigned int GetHeight();
        void SetWidth(unsigned int width);
        void SetHeight(unsigned int height);
        void DrawToken(int x, int y, int token);
        void DrawCharacter(int x, int y, char_t c);
        void DrawWindow(int x, int y, int width, int height);
        void ClearWindow(int x, int y, int width, int height);
        void FillWindow(int x, int y, int width, int height, int token);
        void DrawLabel(int x, int y, string_t label);
        void DrawString(int x, int y, string_t text);
        void DrawInternalHLine(int x, int y, int length);
        void Render();
        TokenBuffer* GetBuffer();

    private:

        int LookupTokenForChar(char_t c);

        static std::map<char_t, int> s_LookupToken;
        unsigned int m_nextBuffer;
        TokenBuffer m_tokenbuffers[2];
        ref_of<ConsoleRenderer>::type m_renderer;
    };

}}//qor::components

#endif//QOR_PP_H_COMPONENTS_CONSOLE_VIEW
