// Copyright Querysoft Limited 2008 - 2025

#ifndef QOR_PP_H_TEXT_SHAPING_GEOMETRY
#define QOR_PP_H_TEXT_SHAPING_GEOMETRY

#include "common.h"

namespace qor{ namespace qb{

    template <typename Float = float>
    struct extents_t
    {
        extents_t () {}

        extents_t (const glyph_extents_t &extents)
        {
            double x0 = (double) extents.x_bearing;
            double y0 = (double) extents.y_bearing;
            double x1 = x0 + (double) extents.width;
            double y1 = y0 + (double) extents.height;

            xmin = (Float) hb_min (x0, x1);
            ymin = (Float) hb_min (y0, y1);
            xmax = (Float) hb_max (x0, x1);
            ymax = (Float) hb_max (y0, y1);
        }

        extents_t (Float xmin, Float ymin, Float xmax, Float ymax) :
        xmin (xmin), ymin (ymin), xmax (xmax), ymax (ymax) {}

        bool is_empty () const { return xmin >= xmax || ymin >= ymax; }
        bool is_void () const { return xmin > xmax; }

        void union_ (const extents_t &o)
        {
            if (o.is_empty ()) return;
            if (is_empty ())
            {
                *this = o;
                return;
            }
            xmin = hb_min (xmin, o.xmin);
            ymin = hb_min (ymin, o.ymin);
            xmax = hb_max (xmax, o.xmax);
            ymax = hb_max (ymax, o.ymax);
        }

        void intersect (const extents_t &o)
        {
            if (o.is_empty () || is_empty ())
            {
                *this = extents_t {};
                return;
            }
            xmin = hb_max (xmin, o.xmin);
            ymin = hb_max (ymin, o.ymin);
            xmax = hb_min (xmax, o.xmax);
            ymax = hb_min (ymax, o.ymax);
        }

        void add_point (Float x, Float y)
        {
            if (unlikely (is_void ()))
            {
                xmin = xmax = x;
                ymin = ymax = y;
            }
            else
            {
                xmin = hb_min (xmin, x);
                ymin = hb_min (ymin, y);
                xmax = hb_max (xmax, x);
                ymax = hb_max (ymax, y);
            }
        }

        glyph_extents_t to_glyph_extents (bool xneg = false, bool yneg = false) const
        {
            double x0 = round ((double) xmin);
            double y0 = round ((double) ymin);
            double x1 = round ((double) xmax);
            double y1 = round ((double) ymax);

            if (!std::isfinite (x0) || !std::isfinite (y0) || !std::isfinite (x1) || !std::isfinite (y1))
            return glyph_extents_t {0, 0, 0, 0};

            return glyph_extents_t 
            {
                clamp_to_hb_position (xneg ? x1 : x0),
                clamp_to_hb_position (yneg ? y0 : y1),
                clamp_to_hb_position (xneg ? x0 - x1 : x1 - x0),
                clamp_to_hb_position (yneg ? y1 - y0 : y0 - y1)
            };
        }

        Float xmin = 0;
        Float ymin = 0;
        Float xmax = -1;
        Float ymax = -1;

        static position_t clamp_to_hb_position (double v)
        {
            return (position_t) hb_clamp (v, (double) hb_int_min (position_t), (double) hb_int_max (position_t));
        }
    };

}}

#endif//QOR_PP_H_TEXT_SHAPING_GEOMETRY