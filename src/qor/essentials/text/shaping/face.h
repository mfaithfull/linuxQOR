// Copyright Querysoft Limited 2008 - 2025

#ifndef QOR_PP_H_TEXT_SHAPING_FACE
#define QOR_PP_H_TEXT_SHAPING_FACE

#include "objectheader.h"
#include "common.h"
#include "blob.h"

namespace qor{ namespace qb{

    typedef blob_t * (*reference_table_func_t)  (face_t *face, tag_t tag, void *user_data);

    struct face_t
    {
        object_header_t header;

        unsigned int index;			                /* Face index in a collection, zero-based. */
        mutable atomic_t<unsigned> upem;	        /* Units-per-EM. */
        mutable atomic_t<unsigned> num_glyphs;      /* Number of glyphs. */

        reference_table_func_t  reference_table_func;
        void* user_data;
        destroy_func_t          destroy;

        hb_get_table_tags_func_t   get_table_tags_func;
        void                      *get_table_tags_user_data;
        destroy_func_t          get_table_tags_destroy;

        hb_shaper_object_dataset_t<face_t> data;/* Various shaper data. */
        hb_ot_face_t table;			/* All the face's tables. */

        /* Cache */
        struct plan_node_t
        {
            hb_shape_plan_t *shape_plan;
            plan_node_t *next;
        };

#ifndef HB_NO_SHAPER
        atomic_t<plan_node_t *> shape_plans;
#endif

        hb_blob_t *reference_table (hb_tag_t tag) const
        {
            hb_blob_t *blob;

            if (!reference_table_func)
            {
                return hb_blob_get_empty ();
            }

            blob = reference_table_func (/*Oh, well.*/const_cast<face_t *> (this), tag, user_data);
            if(!blob)
            {
                return hb_blob_get_empty ();
            }

            return blob;
        }

        unsigned int get_upem () const
        {
            unsigned int ret = upem;
            if (!ret)
            {
                return load_upem ();
            }
            return ret;
        }

        unsigned int get_num_glyphs () const
        {
            unsigned int ret = num_glyphs;
            if (ret == UINT_MAX)
            {
                return load_num_glyphs ();
            }
            return ret;
        }

    private:
        unsigned int load_upem () const;
        unsigned int load_num_glyphs () const;
    };

}}

#endif//QOR_PP_H_TEXT_SHAPING_FEATURE