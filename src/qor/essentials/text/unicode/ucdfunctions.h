// Copyright Querysoft Limited 2008 - 2025

#ifndef QOR_PP_H_TEXT_UNICODE_UCDFUNCTIONS
#define QOR_PP_H_TEXT_UNICODE_UCDFUNCTIONS

#include "func.h"

namespace qor{ namespace unicode{

    class UCDFunctions : public IUnicodeFunctions
    {
    private:        

        static const unsigned int SBASE = 0xAC00u;
        static const unsigned int LBASE = 0x1100u;
        static const unsigned int VBASE = 0x1161u;
        static const unsigned int TBASE = 0x11A7u;
        static const unsigned int SCOUNT = 11172u;
        static const unsigned int LCOUNT = 19u;
        static const unsigned int VCOUNT = 21u;
        static const unsigned int TCOUNT = 28u;
        static const unsigned int NCOUNT = (VCOUNT * TCOUNT);

    public:

        virtual combining_class_t combining_class(codepoint_t cp) override;
        virtual unsigned int eastasian_width(codepoint_t cp);
        virtual general_category_t general_category(codepoint_t cp);
        virtual typename codepoint_t mirroring(codepoint_t cp);
        virtual enum class script script(codepoint_t cp);
        virtual bool compose(codepoint_t a, codepoint_t b, codepoint_t* ab);
        virtual bool decompose(codepoint_t ab, codepoint_t* a, codepoint_t* b);
        virtual unsigned int decompose_compatability(codepoint_t u, codepoint_t* decomposed);        

        inline bool compose_hangul(codepoint_t a, codepoint_t b, codepoint_t* ab)
        {
            if (a >= SBASE && a < (SBASE + SCOUNT) && b > TBASE && b < (TBASE + TCOUNT) && !((a - SBASE) % TCOUNT))
            {
                /* LV,T */
                *ab = a + (b - TBASE);
                return true;
            }
            else if (a >= LBASE && a < (LBASE + LCOUNT) && b >= VBASE && b < (VBASE + VCOUNT))
            {
                /* L,V */
                int li = a - LBASE;
                int vi = b - VBASE;
                *ab = SBASE + li * NCOUNT + vi * TCOUNT;
                return true;
            }
            else
            return false;
        }

        inline bool decompose_hangul(codepoint_t ab, codepoint_t* a, codepoint_t* b)
        {
            unsigned si = ab - SBASE;

            if (si >= SCOUNT)
            {
                return false;
            }

            if (si % TCOUNT)
            {
                // LV,T
                *a = SBASE + (si / TCOUNT) * TCOUNT;
                *b = TBASE + (si % TCOUNT);
                return true;
            } 
            else 
            {
                // L,V
                *a = LBASE + (si / NCOUNT);
                *b = VBASE + (si % NCOUNT) / TCOUNT;
                return true;
            }
        }

    };

}}//qor::unicode

#endif//QOR_PP_H_TEXT_UNICODE_UCDFUNCTIONS

