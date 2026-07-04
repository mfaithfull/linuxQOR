// Copyright Querysoft Limited 2008 - 2025

#ifndef QOR_PP_H_TEXT_SHAPING_COMMON
#define QOR_PP_H_TEXT_SHAPING_COMMON

#include <stdint.h>

namespace qor{ namespace qb{

    typedef int bool_t;
    typedef uint32_t tag_t;
    typedef int32_t position_t;
    typedef uint32_t codepoint_t;
    typedef uint32_t mask_t;

    typedef union _var_int_t 
    {
        uint32_t u32;
        int32_t i32;
        uint16_t u16[2];
        int16_t i16[2];
        uint8_t u8[4];
        int8_t i8[4];
    }_var_int_t;

    typedef enum 
    {
        HB_DIRECTION_INVALID = 0,
        HB_DIRECTION_LTR = 4,
        HB_DIRECTION_RTL,
        HB_DIRECTION_TTB,
        HB_DIRECTION_BTT
    } direction_t;

/* len=-1 means str is NUL-terminated */
extern direction_t hb_direction_from_string(const char *str, int len);
extern const char* hb_direction_to_string(direction_t direction);

#define HB_DIRECTION_IS_VALID(dir)	    ((((unsigned int) (dir)) & ~3U) == 4)
#define HB_DIRECTION_IS_HORIZONTAL(dir)	((((unsigned int) (dir)) & ~1U) == 4)
#define HB_DIRECTION_IS_VERTICAL(dir)	((((unsigned int) (dir)) & ~1U) == 6)
#define HB_DIRECTION_IS_FORWARD(dir)	((((unsigned int) (dir)) & ~2U) == 4)
#define HB_DIRECTION_IS_BACKWARD(dir)	((((unsigned int) (dir)) & ~2U) == 5)
#define HB_DIRECTION_REVERSE(dir)	    ((direction_t) (((unsigned int) (dir)) ^ 1))


    //A method for destroying user-data callbacks.
    typedef void (*destroy_func_t) (void *user_data);

    typedef struct glyph_extents_t 
    {
        position_t x_bearing;
        position_t y_bearing;
        position_t width;
        position_t height;
    } glyph_extents_t;

    struct language_impl_t 
    {
        const char s[1];
    };

    typedef const struct language_impl_t* language_t;
}}

#endif//QOR_PP_H_TEXT_SHAPING_COMMON