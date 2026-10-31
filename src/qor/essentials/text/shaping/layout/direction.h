

#ifndef QOR_PP_H_SRC_QOR_ESSENTIALS_TEXT_SHAPING_LAYOUT_DIRECTION
#define QOR_PP_H_SRC_QOR_ESSENTIALS_TEXT_SHAPING_LAYOUT_DIRECTION

enum class Direction : unsigned int
{
    invalid = 0,
    ltr = 4,
    rtl = 8,
    ttb = 16,
    btt = 32
};

//Primary and secondary direction is possible
//ltr over ttb for English
//btt over rtl for Japanese Katakana perhaps?




#endif//QOR_PP_H_SRC_QOR_ESSENTIALS_TEXT_SHAPING_LAYOUT_DIRECTION
