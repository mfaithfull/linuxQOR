// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_RESOURCES_IMAGE
#define QOR_PP_H_FRAMEWORK_RESOURCES_IMAGE

#include "src/platform/compiler/compiler.h"

namespace qor{ namespace framework{ 

#pragma pack(push, 1)
    struct rgb8
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };

    struct rbga
    {
        unsigned char r;
        unsigned char b;
        unsigned char g;
        unsigned char a;
    };
#pragma pack(pop)

    template< typename TPixelFormat = rgb8 >
    struct RGBPixel : public TPixelFormat
    {
        typedef TPixelFormat base;

        unsigned int Red()
        {
            return base::r;
        }

        unsigned int Green()
        {
            return base::g;
        }

        unsigned int Blue()
        {
            return base::b;
        }

        unsigned int Alpha()
        {
            if constexpr (sizeof(base) > 3 * sizeof(base::r))
            {
                return base::a;
            }
            else
            {
                return 0;
            }
        }

        void SetRed(unsigned int red)
        {
            base::r = static_cast<decltype(base::r)>(red);
        }

        void SetGreen(unsigned int green)
        {
            base::g = static_cast<decltype(base::g)>(green);
        }

        void SetBlue(unsigned int blue)
        {
            base::b = static_cast<decltype(base::b)>(blue);            
        }

        void SetAlpha(unsigned int alpha)
        {
            if constexpr (sizeof(base) > 3 * sizeof(base::r))
            {
                base::a = static_cast<decltype(base::a)>(alpha);
            }
            else
            {   //empty case, no alpha             
            }
        }
    };

    class Image
    {
    public:

        template< class TPixel = RGBPixel<>>
		Image()
        {
            m_pixelSize = sizeof(TPixel);            
        }

        template< class TPixel = RGBPixel<>>
		Image(size_t width, size_t height)
        {
            m_pixelSize = sizeof(TPixel);
            m_bytes = new TPixel[width * height];
        }

        void SetSize(size_t width, size_t height);

		virtual ~Image()
        {
            Free();
        }

        byte* At(size_t x, size_t y);
        byte* operator[](size_t index);

        template< class TPixel = RGBPixel<>, typename TPixelFunction >
        void VisitPixels(TPixelFunction&& pixel_function)
        {
            if(m_bytes)
            {
                for(size_t y = 0; y < m_height; ++y)
                {
                    for(size_t x = 0; x < m_width; ++x)
                    {
                        TPixel* pixel = reinterpret_cast<TPixel*>(m_bytes + ((y * m_width * m_pixelSize) + (x * m_pixelSize)));
                        pixel_function(pixel);
                    }
                }
            }
        }

        template< class TPixel = RGBPixel<>, typename TLineFunction >
        void VisitLines(TLineFunction&& line_function)
        {
            if(m_bytes)
            {
                for(size_t y = 0; y < m_height; ++y)
                {
                    TPixel* line = reinterpret_cast<TPixel*>(m_bytes + ((y * m_width * m_pixelSize)));
                    line_function(line);
                }
            }
        }

    protected:

        void Free();

        size_t m_pixelSize{0};
        size_t m_width;
        size_t m_height;
        byte* m_bytes{nullptr};
    };

}}//qor::framwork

#endif//QOR_PP_H_FRAMEWORK_RESOURCES_IMAGE
