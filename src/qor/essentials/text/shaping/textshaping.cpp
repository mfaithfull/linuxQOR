// Copyright Querysoft Limited 2008 - 2025

#include "src/configuration/configuration.h"
#include "src/qor/memory/reference/newref.h"
#include "font.h"
#include "buffer.h"
#include "feature.h"

namespace qor{ namespace qb{

    void shape(ref_of<font_t>::type font, ref_of<buffer_t>::type buffer, ref_of< std::vector< feature_t > >::type features)
    {
        shape_full(font, buffer, features, nullptr);
    }

    //Return value: false if all shapers failed, true otherwise
    bool_t shape_full(font_t* font, buffer_t* buffer, ref_of< std::vector< feature_t > >::type features, ref_of< std::vector< std::string > >::type shaper_list)
    {
        if (!buffer->len)
        {
            return true;
        }

        buffer->enter();

        buffer_t* text_buffer = nullptr;
        if (buffer->flags & HB_BUFFER_FLAG_VERIFY)
        {
            text_buffer = hb_buffer_create();
            hb_buffer_append(text_buffer, buffer, 0, -1);
        }

        hb_shape_plan_t* shape_plan = hb_shape_plan_create_cached2(font->face, &buffer->props, features, num_features, font->coords, font->num_coords, shaper_list);

        bool_t res = hb_shape_plan_execute(shape_plan, font, buffer, features, num_features);

        hb_shape_plan_destroy(shape_plan);

        if (text_buffer)
        {
            if(res && buffer->successful && text_buffer->successful && !buffer->verify(text_buffer, font, features, num_features, shaper_list))
            {
                res = false;
            }
            hb_buffer_destroy(text_buffer);
        }

        buffer->leave();

        return res;
    }

}}//qor::qb