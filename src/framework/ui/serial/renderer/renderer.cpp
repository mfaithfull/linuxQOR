// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/reference/newref.h"

#include "renderer.h"
#include "../serialconsole.h"

namespace qor { namespace ui { namespace sercon {

    std::map<int,char_t> Renderer::s_charLookup = 
    {
        {TokenBuffer::Tokens::BLANK,' '},
        {TokenBuffer::Tokens::QUESTION_MARK,'?'},
        {TokenBuffer::Tokens::BOX_TOP_LEFT,'/'},
        {TokenBuffer::Tokens::BOX_TOP,'='},
        {TokenBuffer::Tokens::BOX_TOP_RIGHT,'\\'},
        {TokenBuffer::Tokens::BOX_LEFT,'|'},
        {TokenBuffer::Tokens::BOX_RIGHT,'|'},
        {TokenBuffer::Tokens::BOX_BOTTOM_LEFT,'\\'},
        {TokenBuffer::Tokens::BOX_BOTTOM,'_'},
        {TokenBuffer::Tokens::BOX_BOTTOM_RIGHT,'/'},
        {TokenBuffer::Tokens::DARK_FILL, ' '},
        {TokenBuffer::Tokens::MID_FILL,'.'},
        {TokenBuffer::Tokens::LIGHT_FILL,'#'},
        {TokenBuffer::Tokens::LEFT_T_JOIN,'|'},
        {TokenBuffer::Tokens::RIGHT_T_JOIN,'|'},
        {TokenBuffer::Tokens::COLON,':'},
        {TokenBuffer::Tokens::CHAR_A,'A'},
        {TokenBuffer::Tokens::CHAR_B,'B'},
        {TokenBuffer::Tokens::CHAR_C,'C'},
        {TokenBuffer::Tokens::CHAR_D,'D'},
        {TokenBuffer::Tokens::CHAR_E,'E'},
        {TokenBuffer::Tokens::CHAR_F,'F'},
        {TokenBuffer::Tokens::CHAR_G,'G'},
        {TokenBuffer::Tokens::CHAR_H,'H'},
        {TokenBuffer::Tokens::CHAR_I,'I'},
        {TokenBuffer::Tokens::CHAR_J,'J'},
        {TokenBuffer::Tokens::CHAR_K,'K'},
        {TokenBuffer::Tokens::CHAR_L,'L'},
        {TokenBuffer::Tokens::CHAR_M,'M'},
        {TokenBuffer::Tokens::CHAR_N,'N'},
        {TokenBuffer::Tokens::CHAR_O,'O'},
        {TokenBuffer::Tokens::CHAR_P,'P'},
        {TokenBuffer::Tokens::CHAR_Q,'Q'},
        {TokenBuffer::Tokens::CHAR_R,'R'},
        {TokenBuffer::Tokens::CHAR_S,'S'},
        {TokenBuffer::Tokens::CHAR_T,'T'},
        {TokenBuffer::Tokens::CHAR_U,'U'},
        {TokenBuffer::Tokens::CHAR_V,'V'},
        {TokenBuffer::Tokens::CHAR_W,'W'},
        {TokenBuffer::Tokens::CHAR_X,'X'},
        {TokenBuffer::Tokens::CHAR_Y,'Y'},
        {TokenBuffer::Tokens::CHAR_Z,'Z'},
        {TokenBuffer::Tokens::CHAR_0,'0'},
        {TokenBuffer::Tokens::CHAR_1,'1'},
        {TokenBuffer::Tokens::CHAR_2,'2'},
        {TokenBuffer::Tokens::CHAR_3,'3'},
        {TokenBuffer::Tokens::CHAR_4,'4'},
        {TokenBuffer::Tokens::CHAR_5,'5'},
        {TokenBuffer::Tokens::CHAR_6,'6'},
        {TokenBuffer::Tokens::CHAR_7,'7'},
        {TokenBuffer::Tokens::CHAR_8,'8'},
        {TokenBuffer::Tokens::CHAR_9,'9'},
        {TokenBuffer::Tokens::CHAR_a,'a'},
        {TokenBuffer::Tokens::CHAR_b,'b'},
        {TokenBuffer::Tokens::CHAR_c,'c'},
        {TokenBuffer::Tokens::CHAR_d,'d'},
        {TokenBuffer::Tokens::CHAR_e,'e'},
        {TokenBuffer::Tokens::CHAR_f,'f'},
        {TokenBuffer::Tokens::CHAR_g,'g'},
        {TokenBuffer::Tokens::CHAR_h,'h'},
        {TokenBuffer::Tokens::CHAR_i,'i'},
        {TokenBuffer::Tokens::CHAR_j,'j'},
        {TokenBuffer::Tokens::CHAR_k,'k'},
        {TokenBuffer::Tokens::CHAR_l,'l'},
        {TokenBuffer::Tokens::CHAR_m,'m'},
        {TokenBuffer::Tokens::CHAR_n,'n'},
        {TokenBuffer::Tokens::CHAR_o,'o'},
        {TokenBuffer::Tokens::CHAR_p,'p'},
        {TokenBuffer::Tokens::CHAR_q,'q'},
        {TokenBuffer::Tokens::CHAR_r,'r'},
        {TokenBuffer::Tokens::CHAR_s,'s'},
        {TokenBuffer::Tokens::CHAR_t,'t'},
        {TokenBuffer::Tokens::CHAR_u,'u'},
        {TokenBuffer::Tokens::CHAR_v,'v'},
        {TokenBuffer::Tokens::CHAR_w,'w'},
        {TokenBuffer::Tokens::CHAR_x,'x'},
        {TokenBuffer::Tokens::CHAR_y,'y'},
        {TokenBuffer::Tokens::CHAR_z,'z'},
        {TokenBuffer::Tokens::CHAR_110,110},
        {TokenBuffer::Tokens::CHAR_111,111},
        {TokenBuffer::Tokens::CHAR_112,112},
        {TokenBuffer::Tokens::CHAR_113,113},
        {TokenBuffer::Tokens::CHAR_114,114},
        {TokenBuffer::Tokens::CHAR_115,115},
        {TokenBuffer::Tokens::CHAR_116,116},
        {TokenBuffer::Tokens::CHAR_117,117},
        {TokenBuffer::Tokens::CHAR_118,118},
        {TokenBuffer::Tokens::CHAR_119,119},
        {TokenBuffer::Tokens::CHAR_120,120},
        {TokenBuffer::Tokens::CHAR_121,121},
        {TokenBuffer::Tokens::CHAR_122,122},
        {TokenBuffer::Tokens::CHAR_123,123},
        {TokenBuffer::Tokens::CHAR_124,124},
        {TokenBuffer::Tokens::CHAR_125,125},
        {TokenBuffer::Tokens::CHAR_126,126},
        {TokenBuffer::Tokens::CHAR_127,127},
        {TokenBuffer::Tokens::CHAR_128,128},
        {TokenBuffer::Tokens::CHAR_129,129},
        {TokenBuffer::Tokens::CHAR_130,130},
        {TokenBuffer::Tokens::CHAR_131,131},
        {TokenBuffer::Tokens::CHAR_132,132},
        {TokenBuffer::Tokens::CHAR_133,133},
        {TokenBuffer::Tokens::CHAR_134,134},
        {TokenBuffer::Tokens::CHAR_135,135},
        {TokenBuffer::Tokens::CHAR_136,136},
        {TokenBuffer::Tokens::CHAR_137,137},
        {TokenBuffer::Tokens::CHAR_138,138},
        {TokenBuffer::Tokens::CHAR_139,139},
        {TokenBuffer::Tokens::CHAR_140,140},
        {TokenBuffer::Tokens::CHAR_141,141},
        {TokenBuffer::Tokens::CHAR_142,142},
        {TokenBuffer::Tokens::CHAR_143,143},
        {TokenBuffer::Tokens::CHAR_144,144},
        {TokenBuffer::Tokens::CHAR_145,145},
        {TokenBuffer::Tokens::CHAR_146,146},
        {TokenBuffer::Tokens::CHAR_147,147},
        {TokenBuffer::Tokens::CHAR_148,148},
        {TokenBuffer::Tokens::CHAR_149,149},
        {TokenBuffer::Tokens::CHAR_150,150},
        {TokenBuffer::Tokens::CHAR_151,151},
        {TokenBuffer::Tokens::CHAR_152,152},
        {TokenBuffer::Tokens::CHAR_153,153},
        {TokenBuffer::Tokens::CHAR_154,154},
        {TokenBuffer::Tokens::CHAR_155,155},
        {TokenBuffer::Tokens::CHAR_156,156},
        {TokenBuffer::Tokens::CHAR_157,157},
        {TokenBuffer::Tokens::CHAR_158,158},
        {TokenBuffer::Tokens::CHAR_159,159},
        {TokenBuffer::Tokens::CHAR_160,160},
        {TokenBuffer::Tokens::CHAR_161,161},
        {TokenBuffer::Tokens::CHAR_162,162},
        {TokenBuffer::Tokens::CHAR_163,163},
        {TokenBuffer::Tokens::CHAR_164,164},
        {TokenBuffer::Tokens::CHAR_165,165},
        {TokenBuffer::Tokens::CHAR_166,166},
        {TokenBuffer::Tokens::CHAR_167,167},
        {TokenBuffer::Tokens::CHAR_168,168},
        {TokenBuffer::Tokens::CHAR_169,169},
        {TokenBuffer::Tokens::CHAR_170,170},
        {TokenBuffer::Tokens::CHAR_171,171},
        {TokenBuffer::Tokens::CHAR_172,172},
        {TokenBuffer::Tokens::CHAR_173,173},
        {TokenBuffer::Tokens::CHAR_174,174},
        {TokenBuffer::Tokens::CHAR_175,175},
        {TokenBuffer::Tokens::CHAR_176,176},
        {TokenBuffer::Tokens::CHAR_177,177},
        {TokenBuffer::Tokens::CHAR_178,178},
        {TokenBuffer::Tokens::CHAR_179,179},
        {TokenBuffer::Tokens::CHAR_180,180},
        {TokenBuffer::Tokens::CHAR_181,181},
        {TokenBuffer::Tokens::CHAR_182,182},
        {TokenBuffer::Tokens::CHAR_183,183},
        {TokenBuffer::Tokens::CHAR_184,184},
        {TokenBuffer::Tokens::CHAR_185,185},
        {TokenBuffer::Tokens::CHAR_186,186},
        {TokenBuffer::Tokens::CHAR_187,187},
        {TokenBuffer::Tokens::CHAR_188,188},
        {TokenBuffer::Tokens::CHAR_189,189},
        {TokenBuffer::Tokens::CHAR_190,190},
        {TokenBuffer::Tokens::CHAR_191,191},
        {TokenBuffer::Tokens::CHAR_192,192},
        {TokenBuffer::Tokens::CHAR_193,193},
        {TokenBuffer::Tokens::CHAR_194,194},
        {TokenBuffer::Tokens::CHAR_195,195},
        {TokenBuffer::Tokens::CHAR_196,196},
        {TokenBuffer::Tokens::CHAR_197,197},
        {TokenBuffer::Tokens::CHAR_198,198},
        {TokenBuffer::Tokens::CHAR_199,199},
        {TokenBuffer::Tokens::CHAR_200,200},
        {TokenBuffer::Tokens::CHAR_201,201},
        {TokenBuffer::Tokens::CHAR_202,202},
        {TokenBuffer::Tokens::CHAR_203,203},
        {TokenBuffer::Tokens::CHAR_204,204},
        {TokenBuffer::Tokens::CHAR_205,205},
        {TokenBuffer::Tokens::CHAR_206,206},
        {TokenBuffer::Tokens::CHAR_207,207},
        {TokenBuffer::Tokens::CHAR_208,208},
        {TokenBuffer::Tokens::CHAR_209,209},
        {TokenBuffer::Tokens::CHAR_210,210},
        {TokenBuffer::Tokens::CHAR_211,211},
        {TokenBuffer::Tokens::CHAR_212,212},
        {TokenBuffer::Tokens::CHAR_213,213},
        {TokenBuffer::Tokens::CHAR_214,214},
        {TokenBuffer::Tokens::CHAR_215,215},
        {TokenBuffer::Tokens::CHAR_216,216},
        {TokenBuffer::Tokens::CHAR_217,217},
        {TokenBuffer::Tokens::CHAR_218,218},
        {TokenBuffer::Tokens::CHAR_219,219},
        {TokenBuffer::Tokens::CHAR_220,220},
        {TokenBuffer::Tokens::CHAR_221,221},
        {TokenBuffer::Tokens::CHAR_222,222},
        {TokenBuffer::Tokens::CHAR_223,223},
        {TokenBuffer::Tokens::CHAR_224,224},
        {TokenBuffer::Tokens::CHAR_225,225},
        {TokenBuffer::Tokens::CHAR_226,226},
        {TokenBuffer::Tokens::CHAR_227,227},
        {TokenBuffer::Tokens::CHAR_228,228},
        {TokenBuffer::Tokens::CHAR_229,229},
        {TokenBuffer::Tokens::CHAR_230,230},
        {TokenBuffer::Tokens::CHAR_231,231},
        {TokenBuffer::Tokens::CHAR_232,232},
        {TokenBuffer::Tokens::CHAR_233,233},
        {TokenBuffer::Tokens::CHAR_234,234},
        {TokenBuffer::Tokens::CHAR_235,235},        
        {TokenBuffer::Tokens::CHAR_236,236},
        {TokenBuffer::Tokens::CHAR_237,237},
        {TokenBuffer::Tokens::CHAR_238,238},
        {TokenBuffer::Tokens::CHAR_239,239},
        {TokenBuffer::Tokens::CHAR_240,240},
        {TokenBuffer::Tokens::CHAR_241,241},
        {TokenBuffer::Tokens::CHAR_242,242},
        {TokenBuffer::Tokens::CHAR_243,243},
        {TokenBuffer::Tokens::CHAR_244,244},
        {TokenBuffer::Tokens::CHAR_245,245},
        {TokenBuffer::Tokens::CHAR_246,246},
        {TokenBuffer::Tokens::CHAR_247,247},
        {TokenBuffer::Tokens::CHAR_248,248},
        {TokenBuffer::Tokens::CHAR_249,249},
        {TokenBuffer::Tokens::CHAR_250,250},
        {TokenBuffer::Tokens::CHAR_251,251},
        {TokenBuffer::Tokens::CHAR_252,252},
        {TokenBuffer::Tokens::CHAR_253,253},
        {TokenBuffer::Tokens::CHAR_254,254},
        {TokenBuffer::Tokens::CHAR_255,255},
    };

    std::map<char_t, int> Renderer::s_LookupToken = 
    {
        {'A',TokenBuffer::Tokens::CHAR_A},
        {'B',TokenBuffer::Tokens::CHAR_B},
        {'C',TokenBuffer::Tokens::CHAR_C},
        {'D',TokenBuffer::Tokens::CHAR_D},
        {'E',TokenBuffer::Tokens::CHAR_E},
        {'F',TokenBuffer::Tokens::CHAR_F},
        {'G',TokenBuffer::Tokens::CHAR_G},
        {'H',TokenBuffer::Tokens::CHAR_H},
        {'I',TokenBuffer::Tokens::CHAR_I},
        {'J',TokenBuffer::Tokens::CHAR_J},
        {'K',TokenBuffer::Tokens::CHAR_K},
        {'L',TokenBuffer::Tokens::CHAR_L},
        {'M',TokenBuffer::Tokens::CHAR_M},
        {'N',TokenBuffer::Tokens::CHAR_N},
        {'O',TokenBuffer::Tokens::CHAR_O},
        {'P',TokenBuffer::Tokens::CHAR_P},
        {'Q',TokenBuffer::Tokens::CHAR_Q},
        {'R',TokenBuffer::Tokens::CHAR_R},
        {'S',TokenBuffer::Tokens::CHAR_S},
        {'T',TokenBuffer::Tokens::CHAR_T},
        {'U',TokenBuffer::Tokens::CHAR_U},
        {'V',TokenBuffer::Tokens::CHAR_V},
        {'W',TokenBuffer::Tokens::CHAR_W},
        {'X',TokenBuffer::Tokens::CHAR_X},
        {'Y',TokenBuffer::Tokens::CHAR_Y},
        {'Z',TokenBuffer::Tokens::CHAR_Z},
        {'0',TokenBuffer::Tokens::CHAR_0},
        {'1',TokenBuffer::Tokens::CHAR_1},
        {'2',TokenBuffer::Tokens::CHAR_2},
        {'3',TokenBuffer::Tokens::CHAR_3},
        {'4',TokenBuffer::Tokens::CHAR_4},
        {'5',TokenBuffer::Tokens::CHAR_5},
        {'6',TokenBuffer::Tokens::CHAR_6},
        {'7',TokenBuffer::Tokens::CHAR_7},
        {'8',TokenBuffer::Tokens::CHAR_8},
        {'9',TokenBuffer::Tokens::CHAR_9},
        {'a',TokenBuffer::Tokens::CHAR_a},
        {'b',TokenBuffer::Tokens::CHAR_b},
        {'c',TokenBuffer::Tokens::CHAR_c},
        {'d',TokenBuffer::Tokens::CHAR_d},
        {'e',TokenBuffer::Tokens::CHAR_e},
        {'f',TokenBuffer::Tokens::CHAR_f},
        {'g',TokenBuffer::Tokens::CHAR_g},
        {'h',TokenBuffer::Tokens::CHAR_h},
        {'i',TokenBuffer::Tokens::CHAR_i},
        {'j',TokenBuffer::Tokens::CHAR_j},
        {'k',TokenBuffer::Tokens::CHAR_k},
        {'l',TokenBuffer::Tokens::CHAR_l},
        {'m',TokenBuffer::Tokens::CHAR_m},
        {'n',TokenBuffer::Tokens::CHAR_n},
        {'o',TokenBuffer::Tokens::CHAR_o},
        {'p',TokenBuffer::Tokens::CHAR_p},
        {'q',TokenBuffer::Tokens::CHAR_q},
        {'r',TokenBuffer::Tokens::CHAR_r},
        {'s',TokenBuffer::Tokens::CHAR_s},
        {'t',TokenBuffer::Tokens::CHAR_t},
        {'u',TokenBuffer::Tokens::CHAR_u},
        {'v',TokenBuffer::Tokens::CHAR_v},
        {'w',TokenBuffer::Tokens::CHAR_w},
        {'x',TokenBuffer::Tokens::CHAR_x},
        {'y',TokenBuffer::Tokens::CHAR_y},
        {'z',TokenBuffer::Tokens::CHAR_z},
        {'|',TokenBuffer::Tokens::BOX_RIGHT},
        {'>',TokenBuffer::Tokens::CHAR_175}
    };

    Renderer::Renderer(SerialConsole* display) : m_buffer(nullptr), m_display(display){ }

    Renderer::~Renderer() = default;

    void Renderer::SwapTokenBuffer(TokenBuffer*& old, TokenBuffer* newBuffer)
    {
        old = m_buffer;
        m_buffer = newBuffer;
    }

    char_t Renderer::CharLookup(int token)
    {
        auto it = s_charLookup.find(token);
        if(it == s_charLookup.end())
        {
            return '?';
        }
        else
        {
            return it->second;
        }
    }

    int Renderer::LookupTokenForChar(char_t c)
    {
        int token = 0;
        auto it = s_LookupToken.find(c);
        if(it != s_LookupToken.end())
        {
            token = it->second;
        }
        return token;
    }


    void Renderer::Present()
    {
        if(!m_buffer || !m_display)
        {
            return;
        }        
        for(int line = 0; line < m_buffer->GetHeight(); ++line)
        {
            string_t lineToWrite;
            lineToWrite.reserve(m_buffer->GetWidth());
            for(int column = 0; column < m_buffer->GetWidth(); column++)
            {
                int token = m_buffer->GetToken(line,column);
                char_t c = CharLookup(token);
                lineToWrite.push_back(c);
            }
            m_display->WriteLine(lineToWrite);
        }
    }

    void Renderer::DrawToken(int x, int y, int token)
    {
        if(m_buffer)
        {
            m_buffer->SetToken(y, x, token);
        }
    }

    void Renderer::DrawCharacter(int x, int y, char_t c)
    {
        DrawToken(x, y, LookupTokenForChar(c));
    }

    void Renderer::DrawWindow(int x, int y, int width, int height)
    {
        DrawToken(x, y, TokenBuffer::Tokens::BOX_TOP_LEFT);
        DrawToken(x + width, y, TokenBuffer::Tokens::BOX_TOP_RIGHT);
        DrawToken(x, y + height, TokenBuffer::Tokens::BOX_BOTTOM_LEFT);
        DrawToken(x + width, y + height, TokenBuffer::Tokens::BOX_BOTTOM_RIGHT);
        for( int xpos = x + 1; xpos < x + width; ++xpos)
        {
            DrawToken(xpos,y, TokenBuffer::Tokens::BOX_TOP);
            DrawToken(xpos,y+height, TokenBuffer::Tokens::BOX_BOTTOM);
        }
        for( int ypos = y + 1; ypos < y + height; ++ypos)
        {
            DrawToken(x, ypos, TokenBuffer::Tokens::BOX_LEFT);
            DrawToken(x + width, ypos, TokenBuffer::Tokens::BOX_RIGHT);
        }
    }

    void Renderer::ClearWindow(int x, int y, int width, int height)
    {
        for( int xpos = x; xpos <= x + width; ++xpos)
        {
            for( int ypos = y; ypos <= y + height; ++ypos)
            {
                DrawToken(xpos, ypos, TokenBuffer::Tokens::BLANK);
            }
        }
    }

    void Renderer::FillWindow(int x, int y, int width, int height, int token)
    {
        for( int xpos = x; xpos <= x + width; ++xpos)
        {
            for( int ypos = y; ypos <= y + height; ++ypos)
            {
                DrawToken(xpos, ypos, token);
            }
        }
    }

    void Renderer::DrawLabel(int x, int y, string_t label)
    {
        int xpos = x;
        int ypos = y;
        auto it = label.cbegin();
        while(it != label.cend())
        {
            DrawCharacter(xpos, ypos, *it);
            xpos++;
            it++;
            if( xpos > m_buffer->GetWidth())
            {
                xpos = x;
                ypos++;
                if(ypos > m_buffer->GetHeight())
                {
                    return;
                }
            }
        }
        DrawToken(xpos, ypos, TokenBuffer::Tokens::COLON);
    }

    void Renderer::DrawString(int x, int y, string_t text)
    {
        int xpos = x;
        int ypos = y;
        auto it = text.cbegin();
        while(it != text.cend())
        {
            DrawCharacter(xpos, ypos, *it);
            xpos++;
            it++;
            if( xpos > m_buffer->GetWidth())
            {
                xpos = x;
                ypos++;
                if(ypos > m_buffer->GetHeight())
                {
                    return;
                }
            }
        }
    }

    void Renderer::DrawInternalHLine(int x, int y, int length)
    {
        DrawToken(x,y,TokenBuffer::Tokens::LEFT_T_JOIN);
        DrawToken(x+length,y,TokenBuffer::Tokens::RIGHT_T_JOIN);
        for(int t = x+1; t < x + length - 1 ; ++t)
        {
            DrawToken(t,y,TokenBuffer::Tokens::BOX_BOTTOM);
        }
    }


}}}//qor::ui::sercon
