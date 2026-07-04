// Copyright Querysoft Limited 2008 - 2025

#ifndef QOR_PP_H_TEXT_SHAPING_SHAPER
#define QOR_PP_H_TEXT_SHAPING_SHAPER

namespace qor{ namespace qb{

    template <typename Object>
    struct hb_shaper_object_dataset_t
    {
        void init0 (Object *parent_data)
        {
            this->parent_data = parent_data;
#define HB_SHAPER_IMPLEMENT(shaper) shaper.init0 ();
//#include "hb-shaper-list.hh"
            HB_SHAPER_IMPLEMENT (ot) 
#undef HB_SHAPER_IMPLEMENT
        }

        void fini ()
        {
#define HB_SHAPER_IMPLEMENT(shaper) shaper.fini ();
//#include "hb-shaper-list.hh"
            HB_SHAPER_IMPLEMENT (ot) 
#undef HB_SHAPER_IMPLEMENT
        }

        Object* parent_data; /* MUST be JUST before the lazy loaders. */

#define HB_SHAPER_IMPLEMENT(shaper) \
            hb_shaper_lazy_loader_t<Object, HB_SHAPER_ORDER(shaper), \
                        typename hb_shaper_object_data_type_t<HB_SHAPER_ORDER(shaper), Object>::value \
                        > shaper;
//#include "hb-shaper-list.hh"
            HB_SHAPER_IMPLEMENT (ot) 
#undef HB_SHAPER_IMPLEMENT
    };

}}

#endif//QOR_PP_H_TEXT_SHAPING_SHAPER