// Copyright Querysoft Limited 2008 - 2025

#ifndef QOR_PP_H_TEXT_SHAPING_FONT
#define QOR_PP_H_TEXT_SHAPING_FONT

#include <stdint.h>
#include <utility>
#include <math.h>

#include "objectheader.h"
#include "face.h"
#include "algs.h"
#include "shaper.h"
#include "machinery.h"
#include "buffer.h"
#include "geometry.h"

#define HB_FONT_FUNCS_IMPLEMENT_CALLBACKS \
  HB_FONT_FUNC_IMPLEMENT (get_,font_h_extents) \
  HB_FONT_FUNC_IMPLEMENT (get_,font_v_extents) \
  HB_FONT_FUNC_IMPLEMENT (get_,nominal_glyph) \
  HB_FONT_FUNC_IMPLEMENT (get_,nominal_glyphs) \
  HB_FONT_FUNC_IMPLEMENT (get_,variation_glyph) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_h_advance) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_v_advance) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_h_advances) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_v_advances) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_h_origin) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_v_origin) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_h_origins) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_v_origins) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_h_kerning) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_v_kerning) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_extents) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_contour_point) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_name) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_from_name) \
  HB_FONT_FUNC_IMPLEMENT (,draw_glyph_or_fail) \
  HB_FONT_FUNC_IMPLEMENT (,paint_glyph_or_fail) \

namespace qor{ namespace qb{

    typedef struct font_extents_t 
    {
        position_t ascender;
        position_t descender;
        position_t line_gap;
        /*< private >*/
        position_t reserved9;
        position_t reserved8;
        position_t reserved7;
        position_t reserved6;
        position_t reserved5;
        position_t reserved4;
        position_t reserved3;
        position_t reserved2;
        position_t reserved1;
    } font_extents_t;

    struct font_funcs_t
    {
        object_header_t header;

        struct 
        {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) void *name;
            HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
        } *user_data;

        struct 
        {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) destroy_func_t name;
            HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
        } *destroy;

        // Don't access these directly.  Call font->get_*() instead.
        union get_t 
        {
            struct get_funcs_t {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) font_##get_##name##_func_t name;
            HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
            } f;

            void (*array[0
#define HB_FONT_FUNC_IMPLEMENT(get_,name) +1
            HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
            ]) ();
        } get;
    };

    class font_t
    {
    public:

        object_header_t header;
        atomic_t<unsigned> serial;
        atomic_t<unsigned> serial_coords;

        font_t* parent;
        face_t* face;

        int32_t x_scale;
        int32_t y_scale;

        bool is_synthetic;

        float x_embolden;
        float y_embolden;
        bool embolden_in_place;
        int32_t x_strength; // x_embolden, in scaled units.
        int32_t y_strength; // y_embolden, in scaled units.

        float slant;
        float slant_xy;

        float x_multf;
        float y_multf;
        int64_t x_mult;
        int64_t y_mult;

        unsigned int x_ppem;
        unsigned int y_ppem;

        float ptem;

        // Font variation coordinates. 
        unsigned int instance_index;
        bool has_nonzero_coords;
        unsigned int num_coords;
        int* coords;
        float* design_coords;

        font_funcs_t* klass;
        void* user_data;
        destroy_func_t  destroy;

        hb_shaper_object_dataset_t<font_t> data; // Various shaper data.


        // Convert from font-space to user-space
        int64_t dir_mult (direction_t direction)
        { return HB_DIRECTION_IS_VERTICAL(direction) ? y_mult : x_mult; }

        position_t em_scale_x (int16_t v) { return em_mult (v, x_mult); }

        position_t em_scale_y (int16_t v) { return em_mult (v, y_mult); }

        position_t em_scalef_x (float v) { return em_multf (v, x_multf); }

        position_t em_scalef_y (float v) { return em_multf (v, y_multf); }

        float em_fscale_x (int16_t v) { return em_fmult (v, x_multf); }

        float em_fscale_y (int16_t v) { return em_fmult (v, y_multf); }

        float em_fscalef_x (float v) { return em_fmultf (v, x_multf); }

        float em_fscalef_y (float v) { return em_fmultf (v, y_multf); }

        position_t em_scale_dir (int16_t v, direction_t direction)
        { return em_mult (v, dir_mult (direction)); }

        // Convert from parent-font user-space to our user-space
        position_t parent_scale_x_distance (position_t v)
        {
        if (parent && parent->x_scale && parent->x_scale != x_scale)
            return (position_t) (v * (int64_t) this->x_scale / this->parent->x_scale);
        return v;
        }

        position_t parent_scale_y_distance (position_t v)
        {
        if (parent && parent->y_scale && parent->y_scale != y_scale)
            return (position_t) (v * (int64_t) this->y_scale / this->parent->y_scale);
        return v;
        }

        position_t parent_scale_x_position (position_t v)
        { return parent_scale_x_distance (v); }

        position_t parent_scale_y_position (position_t v)
        { return parent_scale_y_distance (v); }

        void parent_scale_distance (position_t *x, position_t *y)
        {
        *x = parent_scale_x_distance (*x);
        *y = parent_scale_y_distance (*y);
        }

        void parent_scale_position (position_t *x, position_t *y)
        {
        *x = parent_scale_x_position (*x);
        *y = parent_scale_y_position (*y);
        }

        void scale_glyph_extents (glyph_extents_t *extents)
        {
            float x1 = em_fscale_x (extents->x_bearing);
            float y1 = em_fscale_y (extents->y_bearing);
            float x2 = em_fscale_x (extents->x_bearing + extents->width);
            float y2 = em_fscale_y (extents->y_bearing + extents->height);

            extents->x_bearing = floorf (x1);
            extents->y_bearing = floorf (y1);
            extents->width = ceilf (x2) - extents->x_bearing;
            extents->height = ceilf (y2) - extents->y_bearing;
        }

        void synthetic_glyph_extents (glyph_extents_t *extents)
        {
            // Slant. 
            if (slant_xy)
            {
                position_t x1 = extents->x_bearing;
                position_t y1 = extents->y_bearing;
                position_t x2 = extents->x_bearing + extents->width;
                position_t y2 = extents->y_bearing + extents->height;

                x1 += floorf (std::min(y1 * slant_xy, y2 * slant_xy));
                x2 += ceilf (std::max(y1 * slant_xy, y2 * slant_xy));

                extents->x_bearing = x1;
                extents->width = x2 - extents->x_bearing;
            }

            // Embolden. 
            if (x_strength || y_strength)
            {
                // Y
                int y_shift = y_strength;
                if (y_scale < 0) y_shift = -y_shift;
                extents->y_bearing += y_shift;
                extents->height -= y_shift;

                // X
                int x_shift = x_strength;
                if (x_scale < 0) x_shift = -x_shift;
                if (embolden_in_place)
            extents->x_bearing -= x_shift / 2;
                extents->width += x_shift;
            }
        }


        // Public getters 

        bool has_func (unsigned int i);
        bool has_func_set (unsigned int i);

        // has_* ... 
        #define HB_FONT_FUNC_IMPLEMENT(get_,name) \
        bool \
        has_##name##_func () \
        { \
        font_funcs_t *funcs = this->klass; \
        unsigned int i = offsetof (font_funcs_t::get_t::get_funcs_t, name) / sizeof (funcs->get.array[0]); \
        return has_func (i); \
        } \
        bool \
        has_##name##_func_set () \
        { \
        font_funcs_t *funcs = this->klass; \
        unsigned int i = offsetof (font_funcs_t::get_t::get_funcs_t, name) / sizeof (funcs->get.array[0]); \
        return has_func_set (i); \
        }
        //HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
        #undef HB_FONT_FUNC_IMPLEMENT

        bool_t get_font_h_extents (font_extents_t *extents, bool synthetic = true)
        {
            hb_memset (extents, 0, sizeof (*extents));
            bool ret = klass->get.f.font_h_extents (this, user_data, extents, !klass->user_data ? nullptr : klass->user_data->font_h_extents);

            if (synthetic && ret)
            {
                // Embolden
                int y_shift = y_scale < 0 ? -y_strength : y_strength;
                extents->ascender += y_shift;
            }

            return ret;
        }

        bool_t get_font_v_extents (font_extents_t *extents, bool synthetic = true)
        {
            hb_memset (extents, 0, sizeof (*extents));
            bool ret = klass->get.f.font_v_extents (this, user_data, extents, !klass->user_data ? nullptr : klass->user_data->font_v_extents);

            if (synthetic && ret)
            {
                // Embolden 
                int x_shift = x_scale < 0 ? -x_strength : x_strength;
                if (embolden_in_place)
                {
                    extents->ascender += x_shift / 2;
                    extents->descender -= x_shift - x_shift / 2;
                }
                else
                    extents->ascender += x_shift;
            }

            return ret;
        }

        bool has_glyph (codepoint_t unicode)
        {
            codepoint_t glyph;
            return get_nominal_glyph (unicode, &glyph);
        }

        bool_t get_nominal_glyph (codepoint_t unicode, codepoint_t *glyph, codepoint_t not_found = 0)
        {
            *glyph = not_found;
            return klass->get.f.nominal_glyph (this, user_data, unicode, glyph, !klass->user_data ? nullptr : klass->user_data->nominal_glyph);
        }

        unsigned int get_nominal_glyphs (unsigned int count, const codepoint_t *first_unicode, unsigned int unicode_stride, codepoint_t *first_glyph, unsigned int glyph_stride)
        {
            return klass->get.f.nominal_glyphs (this, user_data, count, first_unicode, unicode_stride, first_glyph, glyph_stride, !klass->user_data ? nullptr : klass->user_data->nominal_glyphs);
        }

        bool_t get_variation_glyph (codepoint_t unicode, codepoint_t variation_selector, codepoint_t *glyph, codepoint_t not_found = 0)
        {
            *glyph = not_found;
            return klass->get.f.variation_glyph (this, user_data, unicode, variation_selector, glyph, !klass->user_data ? nullptr : klass->user_data->variation_glyph);
        }

        position_t get_glyph_h_advance (codepoint_t glyph, bool synthetic = true)
        {
            position_t advance = klass->get.f.glyph_h_advance (this, user_data, glyph, !klass->user_data ? nullptr : klass->user_data->glyph_h_advance);

            if (synthetic && x_strength && !embolden_in_place)
            {
                // Embolden
                position_t strength = x_scale >= 0 ? x_strength : -x_strength;
                advance += advance ? strength : 0;
            }

            return advance;
        }

        position_t get_glyph_v_advance (codepoint_t glyph, bool synthetic = true)
        {
            position_t advance = klass->get.f.glyph_v_advance (this, user_data, glyph, !klass->user_data ? nullptr : klass->user_data->glyph_v_advance);

            if (synthetic && y_strength && !embolden_in_place)
            {
                // Embolden
                position_t strength = y_scale >= 0 ? y_strength : -y_strength;
                advance += advance ? strength : 0;
            }

            return advance;
        }

        void get_glyph_h_advances (unsigned int count, const codepoint_t *first_glyph, unsigned int glyph_stride, position_t *first_advance, unsigned int advance_stride, bool synthetic = true)
        {
            klass->get.f.glyph_h_advances (this, user_data, count, first_glyph, glyph_stride, first_advance, advance_stride, !klass->user_data ? nullptr : klass->user_data->glyph_h_advances);

            if (synthetic && x_strength && !embolden_in_place)
            {
                // Embolden
                position_t strength = x_scale >= 0 ? x_strength : -x_strength;
                for (unsigned int i = 0; i < count; i++)
                {
                    *first_advance += *first_advance ? strength : 0;
                    first_advance = &StructAtOffsetUnaligned<position_t> (first_advance, advance_stride);
                }
            }
        }

        void get_glyph_v_advances (unsigned int count, const codepoint_t *first_glyph, unsigned int glyph_stride, position_t *first_advance, unsigned int advance_stride, bool synthetic = true)
        {
            klass->get.f.glyph_v_advances (this, user_data, count, first_glyph, glyph_stride, first_advance, advance_stride, !klass->user_data ? nullptr : klass->user_data->glyph_v_advances);

            if (synthetic && y_strength && !embolden_in_place)
            {
                // Embolden 
                position_t strength = y_scale >= 0 ? y_strength : -y_strength;
                for (unsigned int i = 0; i < count; i++)
                {
                    *first_advance += *first_advance ? strength : 0;
                    first_advance = &StructAtOffsetUnaligned<position_t> (first_advance, advance_stride);
                }
            }
        }

        bool_t get_glyph_h_origin (codepoint_t glyph, position_t *x, position_t *y, bool synthetic = true)
        {
            *x = *y = 0;
            bool ret = klass->get.f.glyph_h_origin (this, user_data, glyph, x, y, !klass->user_data ? nullptr : klass->user_data->glyph_h_origin);

            if (synthetic && ret)
            {
                // Slant is ignored as it does not affect glyph origin

                // Embolden
                if (!embolden_in_place)
                {
                    *x += x_scale < 0 ? -x_strength : x_strength;
                    *y += y_scale < 0 ? -y_strength : y_strength;
                }
            }

            return ret;
        }

        bool_t get_glyph_v_origin (codepoint_t glyph, position_t *x, position_t *y, bool synthetic = true)
        {
            *x = *y = 0;
            bool ret = klass->get.f.glyph_v_origin (this, user_data, glyph, x, y, !klass->user_data ? nullptr : klass->user_data->glyph_v_origin);

            if (synthetic && ret)
            {
                // Slant is ignored as it does not affect glyph origin

                // Embolden 
                if (!embolden_in_place)
                {
                    *x += x_scale < 0 ? -x_strength : x_strength;
                    *y += y_scale < 0 ? -y_strength : y_strength;
                }
            }

            return ret;
        }

        bool_t get_glyph_h_origins (unsigned int count, const codepoint_t* first_glyph, unsigned int glyph_stride, position_t* first_x, unsigned int x_stride, position_t* first_y, unsigned int y_stride, bool synthetic = true)
        {
            bool ret = klass->get.f.glyph_h_origins (this, user_data, count, first_glyph, glyph_stride, first_x, x_stride, first_y, y_stride, !klass->user_data ? nullptr : klass->user_data->glyph_h_origins);

            if (synthetic && ret)
            {
                position_t x_shift = x_scale < 0 ? -x_strength : x_strength;
                position_t y_shift = y_scale < 0 ? -y_strength : y_strength;
                for (unsigned i = 0; i < count; i++)
                {
                    // Slant is ignored as it does not affect glyph origin

                    // Embolden 
                    if (!embolden_in_place)
                    {
                        *first_x += x_shift;
                        *first_y += y_shift;
                    }
                    first_x = &StructAtOffsetUnaligned<position_t> (first_x, x_stride);
                    first_y = &StructAtOffsetUnaligned<position_t> (first_y, y_stride);
                }
            }
            return ret;
        }

        bool_t get_glyph_v_origins (unsigned int count, const codepoint_t *first_glyph, unsigned int glyph_stride, position_t* first_x, unsigned int x_stride, position_t* first_y, unsigned int y_stride, bool synthetic = true)
        {
            bool ret = klass->get.f.glyph_v_origins (this, user_data, count, first_glyph, glyph_stride, first_x, x_stride, first_y, y_stride, klass->user_data ? nullptr : klass->user_data->glyph_v_origins);

            if (synthetic && is_synthetic && ret)
            {
                position_t x_shift = x_scale < 0 ? -x_strength : x_strength;
                position_t y_shift = y_scale < 0 ? -y_strength : y_strength;
                for (unsigned i = 0; i < count; i++)
                {
                    // Slant is ignored as it does not affect glyph origin

                    // Embolden 
                    if (!embolden_in_place)
                    {
                        *first_x += x_shift;
                        *first_y += y_shift;
                    }
                    first_x = &StructAtOffsetUnaligned<position_t> (first_x, x_stride);
                    first_y = &StructAtOffsetUnaligned<position_t> (first_y, y_stride);
                }
            }

            return ret;
        }

        position_t get_glyph_h_kerning (codepoint_t left_glyph, codepoint_t right_glyph)
        {
#ifdef HB_DISABLE_DEPRECATED
            return 0;
#else
            return klass->get.f.glyph_h_kerning (this, user_data, left_glyph, right_glyph, !klass->user_data ? nullptr : klass->user_data->glyph_h_kerning);
#endif
        }

        position_t get_glyph_v_kerning (codepoint_t top_glyph, codepoint_t bottom_glyph)
        {
#ifdef HB_DISABLE_DEPRECATED
            return 0;
#else
            return klass->get.f.glyph_v_kerning (this, user_data, top_glyph, bottom_glyph, !klass->user_data ? nullptr : klass->user_data->glyph_v_kerning);
#endif
        }

        bool_t get_glyph_extents (codepoint_t glyph, glyph_extents_t *extents, bool synthetic = true)
        {
            hb_memset (extents, 0, sizeof (*extents));

            //This is rather messy, but necessary. 

            if (!synthetic)
            {
                return klass->get.f.glyph_extents (this, user_data, glyph, extents, !klass->user_data ? nullptr : klass->user_data->glyph_extents);
            }
            if (!is_synthetic && klass->get.f.glyph_extents (this, user_data, glyph, extents, !klass->user_data ? nullptr : klass->user_data->glyph_extents))
                return true;

            // Try getting extents from paint(), then draw(), *then* get_extents()
            // and apply synthetic settings in the last case.

#ifndef HB_NO_PAINT
            hb_paint_extents_context_t paint_extents;
            if (paint_glyph_or_fail (glyph, hb_paint_extents_get_funcs (), &paint_extents, 0, 0))
            {
                *extents = paint_extents.get_extents ().to_glyph_extents ();
                return true;
            }
#endif

#ifndef HB_NO_DRAW
            extents_t<> draw_extents;
            if (draw_glyph_or_fail (glyph, hb_draw_extents_get_funcs (), &draw_extents))
            {
                *extents = draw_extents.to_glyph_extents ();
                return true;
            }
#endif

            bool ret = klass->get.f.glyph_extents (this, user_data, glyph, extents, !klass->user_data ? nullptr : klass->user_data->glyph_extents);
            if (ret)
            {
                synthetic_glyph_extents (extents);
            }

            return ret;
        }

        bool_t get_glyph_contour_point(codepoint_t glyph, unsigned int point_index, position_t *x, position_t *y, bool synthetic = true)
        {
            *x = *y = 0;
            bool ret = klass->get.f.glyph_contour_point (this, user_data, glyph, point_index, x, y, !klass->user_data ? nullptr : klass->user_data->glyph_contour_point);

            if (synthetic && ret)
            {
                // Slant
                if (slant_xy)
                *x += roundf (*y * slant_xy);

                // Embolden
                if (!embolden_in_place)
                {
                    int x_shift = x_scale < 0 ? -x_strength : x_strength;
                    *x += x_shift;
                }
            }

            return ret;
        }

        bool_t get_glyph_name (codepoint_t glyph, char *name, unsigned int size)
        {
            if (size) *name = '\0';
            return klass->get.f.glyph_name (this, user_data, glyph, name, size, !klass->user_data ? nullptr : klass->user_data->glyph_name);
        }

        bool_t get_glyph_from_name (const char *name, int len, // -1 means nul-terminated
                        codepoint_t *glyph)
        {
            *glyph = 0;
            if (len == -1) len = strlen (name);
            return klass->get.f.glyph_from_name (this, user_data, name, len, glyph, !klass->user_data ? nullptr : klass->user_data->glyph_from_name);
        }

        bool draw_glyph_or_fail (codepoint_t glyph, hb_draw_funcs_t *draw_funcs, void *draw_data, bool synthetic = true)
        {
#ifndef HB_NO_DRAW
#ifndef HB_NO_OUTLINE
        bool embolden = x_strength || y_strength;
        bool slanted = slant_xy;
        synthetic = synthetic && (embolden || slanted);
#else
        synthetic = false;
#endif

        if (!synthetic)
        {
            return klass->get.f.draw_glyph_or_fail (this, user_data,
                                glyph,
                                draw_funcs, draw_data,
                                !klass->user_data ? nullptr : klass->user_data->draw_glyph_or_fail);
        }

#ifndef HB_NO_OUTLINE

        hb_outline_t outline;
        if (!klass->get.f.draw_glyph_or_fail (this, user_data, glyph, hb_outline_recording_pen_get_funcs (), &outline, !klass->user_data ? nullptr : klass->user_data->draw_glyph_or_fail))
            return false;

        // Slant before embolden; produces nicer results.

        if (slanted)
        {
            position_t xo = 0, yo = 0;
            get_glyph_h_origin (glyph, &xo, &yo, false);
            outline.translate (-xo, -yo);
            outline.slant (slant_xy);
            outline.translate (xo, yo);
        }

        if (embolden)
        {
            float x_shift = embolden_in_place ? 0 : (float) x_strength / 2;
            float y_shift = (float) y_strength / 2;
            if (x_scale < 0) x_shift = -x_shift;
            if (y_scale < 0) y_shift = -y_shift;
            outline.embolden (x_strength, y_strength, x_shift, y_shift);
        }

        outline.replay (draw_funcs, draw_data);

        return true;
#endif
#endif
        return false;
        }

        bool paint_glyph_or_fail (codepoint_t glyph, hb_paint_funcs_t *paint_funcs, void *paint_data, unsigned int palette, hb_color_t foreground, bool synthetic = true)
        {
#ifndef HB_NO_PAINT
            // Slant
            if (synthetic && slant_xy)
                hb_paint_push_transform (paint_funcs, paint_data, 1.f, 0.f, slant_xy, 1.f, 0.f, 0.f);

            bool ret = klass->get.f.paint_glyph_or_fail (this, user_data, glyph, paint_funcs, paint_data, palette, foreground, !klass->user_data ? nullptr : klass->user_data->paint_glyph_or_fail);

            if (synthetic && slant_xy)
                hb_paint_pop_transform (paint_funcs, paint_data);

            return ret;
#endif
            return false;
        }

        // A bit higher-level, and with fallback 

        void paint_glyph (codepoint_t glyph, hb_paint_funcs_t* paint_funcs, void* paint_data, unsigned int palette, hb_color_t foreground);

        void get_h_extents_with_fallback (font_extents_t *extents)
        {
            if (!get_font_h_extents (extents))
            {
                extents->ascender = y_scale * .8;
                extents->descender = extents->ascender - y_scale;
                extents->line_gap = 0;
            }
        }
        void get_v_extents_with_fallback (font_extents_t *extents)
        {
            if (!get_font_v_extents (extents))
            {
                extents->ascender = x_scale / 2;
                extents->descender = extents->ascender - x_scale;
                extents->line_gap = 0;
            }
        }

        void get_extents_for_direction (direction_t direction, font_extents_t *extents)
        {
            if (HB_DIRECTION_IS_HORIZONTAL (direction))
                get_h_extents_with_fallback (extents);
            else
                get_v_extents_with_fallback (extents);
        }

        void get_glyph_advance_for_direction (codepoint_t glyph, direction_t direction, position_t *x, position_t *y)
        {
            *x = *y = 0;
            if (HB_DIRECTION_IS_HORIZONTAL (direction))
                *x = get_glyph_h_advance (glyph);
            else
                *y = get_glyph_v_advance (glyph);
        }

        void get_glyph_advances_for_direction (direction_t direction, unsigned int count, const codepoint_t *first_glyph, unsigned glyph_stride, position_t *first_advance, unsigned advance_stride)
        {
            if (HB_DIRECTION_IS_HORIZONTAL (direction))
                get_glyph_h_advances (count, first_glyph, glyph_stride, first_advance, advance_stride);
            else
                get_glyph_v_advances (count, first_glyph, glyph_stride, first_advance, advance_stride);
        }

        void apply_offset (position_t *x, position_t *y, position_t dx, position_t dy, signed mult)
        {
            assert (mult == -1 || mult == +1);

            *x += dx * mult;
            *y += dy * mult;
        }

        void add_offset (position_t *x, position_t *y, position_t dx, position_t dy)
        {
            *x += dx;
            *y += dy;
        }

        void subtract_offset (position_t *x, position_t *y, position_t dx, position_t dy)
        {
            *x -= dx;
            *y -= dy;
        }

        void guess_v_origin_minus_h_origin (codepoint_t glyph, position_t *x, position_t *y)
        {
            *x = get_glyph_h_advance (glyph) / 2;

            font_extents_t extents;
            get_h_extents_with_fallback (&extents);
            *y = extents.ascender;
        }

        void apply_glyph_h_origins_with_fallback (buffer_t *buf, int mult)
        {
            bool has_ascender = false;
            position_t ascender = 0;

            struct { position_t x, y; } origins[32];

            unsigned int offset = 0;
            unsigned int count = buf->len;
            while (offset < count)
            {
                unsigned n = std::min(count - offset, ARRAY_LENGTH (origins));
                if (!get_glyph_h_origins (n,
                        &buf->info[offset].codepoint, sizeof (glyph_info_t),
                        &origins[0].x, sizeof (origins[0]),
                        &origins[0].y, sizeof (origins[0])))
                {
                    if (get_glyph_v_origins (n,
                                &buf->info[offset].codepoint, sizeof (glyph_info_t),
                                &origins[0].x, sizeof (origins[0]),
                                &origins[0].y, sizeof (origins[0])))
                    {
                        if (!has_ascender)
                        {
                        font_extents_t extents;
                        get_h_extents_with_fallback (&extents);
                        ascender = extents.ascender;
                        has_ascender = true;
                        }

                        // We got the v_origins, adjust them to h_origins.
                        for (unsigned j = 0; j < n; j++)
                        {
                        codepoint_t glyph = buf->info[offset + j].codepoint;
                        origins[j].x -= get_glyph_h_advance (glyph) / 2;
                        origins[j].y -= ascender;
                        }
                    }
                    else
                    {
                        mult = 0; // Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below
                    }
                }

                assert (mult == -1 || mult == +1 || mult == 0);
                if (mult == +1)
                for (unsigned j = 0; j < n; j++)
                {
                    glyph_position_t *pos = &buf->pos[offset + j];
                    add_offset (&pos->x_offset, &pos->y_offset,
                            origins[j].x, origins[j].y);
                }
                else if (mult == -1)
                for (unsigned j = 0; j < n; j++)
                {
                    glyph_position_t *pos = &buf->pos[offset + j];
                    subtract_offset (&pos->x_offset, &pos->y_offset,
                            origins[j].x, origins[j].y);
                }
                // else if (mult == 0) --> Do nothing 

                offset += n;
            }
        }

        void apply_glyph_v_origins_with_fallback (buffer_t *buf, int mult)
        {
            bool has_ascender = false;
            position_t ascender = 0;

            struct { position_t x, y; } origins[32];

            unsigned int offset = 0;
            unsigned int count = buf->len;
            while (offset < count)
            {
                unsigned n = hb_min (count - offset, ARRAY_LENGTH (origins));
                if (!get_glyph_v_origins (n, &buf->info[offset].codepoint, sizeof (glyph_info_t), &origins[0].x, sizeof (origins[0]), &origins[0].y, sizeof (origins[0])))
                {
                    if (get_glyph_h_origins (n, &buf->info[offset].codepoint, sizeof (glyph_info_t), &origins[0].x, sizeof (origins[0]), &origins[0].y, sizeof (origins[0])))
                    {
                        if (!has_ascender)
                        {
                            font_extents_t extents;
                            get_h_extents_with_fallback (&extents);
                            ascender = extents.ascender;
                            has_ascender = true;
                        }

                        // We got the h_origins, adjust them to v_origins. 
                        for (unsigned j = 0; j < n; j++)
                        {
                            codepoint_t glyph = buf->info[offset + j].codepoint;
                            origins[j].x += get_glyph_h_advance (glyph) / 2;
                            origins[j].y += ascender;
                        }
                    }
                    else
                    {
                        mult = 0; // Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below 
                    }
                }

                assert (mult == -1 || mult == +1 || mult == 0);
                if (mult == +1)
                    for (unsigned j = 0; j < n; j++)
                    {
                        glyph_position_t *pos = &buf->pos[offset + j];
                        add_offset (&pos->x_offset, &pos->y_offset,
                                origins[j].x, origins[j].y);
                    }
                else if (mult == -1)
                    for (unsigned j = 0; j < n; j++)
                    {
                        glyph_position_t *pos = &buf->pos[offset + j];
                        subtract_offset (&pos->x_offset, &pos->y_offset,
                                origins[j].x, origins[j].y);
                    }
                //else if (mult == 0) --> Do nothing 

                offset += n;
            }
        }

        void get_glyph_h_origin_with_fallback (codepoint_t glyph, position_t *x, position_t *y)
        {
        if (!get_glyph_h_origin (glyph, x, y) &&
            get_glyph_v_origin (glyph, x, y))
        {
            position_t dx, dy;
            guess_v_origin_minus_h_origin (glyph, &dx, &dy);
            subtract_offset (x, y, dx, dy);
        }
        }

        void get_glyph_v_origin_with_fallback (codepoint_t glyph, position_t *x, position_t *y)
        {
            if (!get_glyph_v_origin (glyph, x, y) && get_glyph_h_origin (glyph, x, y))
            {
                position_t dx, dy;
                guess_v_origin_minus_h_origin (glyph, &dx, &dy);
                add_offset (x, y, dx, dy);
            }
        }

        void get_glyph_origin_for_direction (codepoint_t glyph, direction_t direction, position_t *x, position_t *y)
        {
            if (HB_DIRECTION_IS_HORIZONTAL (direction))
                get_glyph_h_origin_with_fallback (glyph, x, y);
            else
                get_glyph_v_origin_with_fallback (glyph, x, y);
        }

        void add_glyph_h_origins (buffer_t *buf)
        {
            apply_glyph_h_origins_with_fallback (buf, +1);
        }

        void add_glyph_v_origins (buffer_t *buf)
        {
            apply_glyph_v_origins_with_fallback (buf, +1);
        }

        void add_glyph_origin_for_direction (codepoint_t glyph, direction_t direction, position_t *x, position_t *y)
        {
            position_t origin_x, origin_y;
            get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
            add_offset (x, y, origin_x, origin_y);
        }

        void subtract_glyph_h_origins (buffer_t *buf)
        {
            apply_glyph_h_origins_with_fallback (buf, -1);
        }
        
        void subtract_glyph_v_origins (buffer_t *buf)
        {
            apply_glyph_v_origins_with_fallback (buf, -1);
        }
        
        void subtract_glyph_origin_for_direction (codepoint_t glyph, direction_t direction, position_t *x, position_t *y)
        {
            position_t origin_x, origin_y;
            get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
            subtract_offset (x, y, origin_x, origin_y);
        }

        void get_glyph_kerning_for_direction (codepoint_t first_glyph, codepoint_t second_glyph, direction_t direction, position_t *x, position_t *y)
        {
            if (HB_DIRECTION_IS_HORIZONTAL (direction)) 
            {
                *y = 0;
                *x = get_glyph_h_kerning (first_glyph, second_glyph);
            } 
            else 
            {
                *x = 0;
                *y = get_glyph_v_kerning (first_glyph, second_glyph);
            }
        }

        bool_t get_glyph_extents_for_origin (codepoint_t glyph, direction_t direction, glyph_extents_t *extents)
        {
            bool_t ret = get_glyph_extents (glyph, extents);

            if (ret)
                subtract_glyph_origin_for_direction (glyph, direction, &extents->x_bearing, &extents->y_bearing);

            return ret;
        }

        bool_t get_glyph_contour_point_for_origin (codepoint_t glyph, unsigned int point_index, direction_t direction, position_t *x, position_t *y)
        {
            bool_t ret = get_glyph_contour_point (glyph, point_index, x, y);

            if (ret)
                subtract_glyph_origin_for_direction (glyph, direction, x, y);

            return ret;
        }

        // Generates gidDDD if glyph has no name.
        void
        glyph_to_string (codepoint_t glyph, char *s, unsigned int size)
        {
            if (get_glyph_name (glyph, s, size)) return;

            if (size && snprintf (s, size, "gid%" PRIu32, glyph) < 0)
                *s = '\0';
        }

        // Parses gidDDD and uniUUUU strings automatically.
        bool_t glyph_from_string (const char *s, int len, // -1 means nul-terminated 
                    codepoint_t* glyph)
        {
            if (get_glyph_from_name (s, len, glyph)) return true;

            if (len == -1) len = strlen (s);

            // Straight glyph index. 
            if (hb_codepoint_parse (s, len, 10, glyph))
            {
                return true;
            }

            if (len > 3)
            {
                // gidDDD syntax for glyph indices.
                if (0 == strncmp (s, "gid", 3) && hb_codepoint_parse (s + 3, len - 3, 10, glyph))
                {
                    return true;
                }

                // uniUUUU and other Unicode character indices.
                codepoint_t unichar;
                if (0 == strncmp (s, "uni", 3) && hb_codepoint_parse (s + 3, len - 3, 16, &unichar) && get_nominal_glyph (unichar, glyph))
                {
                    return true;
                }
            }

            return false;
        }

        void changed ()
        {
            float upem = face->get_upem ();

            x_multf = x_scale / upem;
            y_multf = y_scale / upem;
            bool x_neg = x_scale < 0;
            x_mult = (x_neg ? -((int64_t) -x_scale << 16) : ((int64_t) x_scale << 16)) / upem;
            bool y_neg = y_scale < 0;
            y_mult = (y_neg ? -((int64_t) -y_scale << 16) : ((int64_t) y_scale << 16)) / upem;

            is_synthetic =  x_embolden || y_embolden || slant;

            x_strength = roundf (abs (x_scale) * x_embolden);
            y_strength = roundf (abs (y_scale) * y_embolden);

            slant_xy = y_scale ? slant * x_scale / y_scale : 0.f;

            data.fini ();

            serial++;
        }

        position_t em_mult (int16_t v, int64_t mult)
        { return (position_t) ((v * mult + 32768) >> 16); }

        position_t em_multf (float v, float mult)
        { return (position_t) roundf (em_fmultf (v, mult)); }

        float em_fmultf (float v, float mult)
        { return v * mult; }

        float em_fmult (int16_t v, float mult)
        { return (float) v * mult; }

    };

}}

#endif//QOR_PP_H_TEXT_SHAPING_FONT