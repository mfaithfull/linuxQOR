// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_WINDOWS_GUI_REGION
#define QOR_PP_H_WINDOWS_GUI_REGION

#include "src/platform/os/windows/common/handles/handle.h"
#include "src/platform/os/windows/common/structures.h"
#include "gdiobject.h"
#include "../view/drawing/rect.h"
#include "../view/drawing/point.h"

//All types on this interface must be portable
namespace qor{ namespace platform { namespace win{

    struct RgnDataHeader
    {
        unsigned long   dwSize;
        unsigned long   iType;
        unsigned long   nCount;
        unsigned long   nRgnSize;
        Rect            rcBound;
    };

    struct RgnData
    {
        RgnDataHeader   rdh;
        char            Buffer[1];
    };

    class qor_pp_module_interface(QOR_WINGUI) Region : public GDIObject
    {
    public:
        Region();
        Region(const PrimitiveHandle& h, bool takeOwnerShip);
        virtual ~Region();

        bool operator == (const Region& cmp);
        int Combine(const Region& rgn1, const Region& rgn2, int combineMode);
        unsigned long GetData(unsigned long count, RgnData* rgnData);
        int GetBox(Rect& r);
        bool IsRectIn(const Rect& r);
        bool SetRect(int left, int top, int right, int bottom);
        int Offset(int xOffset, int yOffset);
        bool IsPointIn(int x, int y);

        static Region CreateElliptic(int left, int top, int right, int bottom);
        static Region CreateElliptic(const Rect& r);
        static Region CreatePolygon(const Point* pt, int countPoints, int polyFillMode);
        static Region CreatePolyPolygon(const Point* pt, const int* polyCounts, int count, int polyFillMode);
        static Region CreateRect(int left, int top, int right, int bottom);
        static Region CreateRect(const Rect& r);
        static Region CreateRoundRect(int left, int top, int right, int bottom, int widthEllipse, int heightEllipse);
        static Region Create( const XForm* xform, unsigned long count, const RgnData* rgnData);
    };

}}}//qor::platform::win

#endif//QOR_PP_H_WINDOWS_GUI_REGION