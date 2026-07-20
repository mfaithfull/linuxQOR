// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

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
