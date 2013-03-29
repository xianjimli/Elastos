
#ifndef __CRECT_H__
#define __CRECT_H__

#include "_CRect.h"

CarClass(CRect)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI constructor(
        /* [in] */ IRect* r);

    CARAPI Equals(
        /* [in] */ IRect* r,
        /* [out] */ Boolean* result);

    CARAPI IsEmpty(
        /* [out] */ Boolean* result);

    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI SetEmpty();

    CARAPI Set(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI SetEx(
        /* [in] */ IRect* src);

    CARAPI Offset(
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy);

    CARAPI Inset(
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy);

    /**
     * Returns true if (x,y) is inside the rectangle. The left and top are
     * considered to be inside, while the right and bottom are not. This means
     * that for a x,y to be contained: left <= x < right and top <= y < bottom.
     * An empty rectangle never contains any point.
     *
     * @param x The X coordinate of the point being tested for containment
     * @param y The Y coordinate of the point being tested for containment
     * @return true iff (x,y) are contained by the rectangle, where containment
     *              means left <= x < right and top <= y < bottom
     */
    CARAPI Contains(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [out] */ Boolean* result);

    /**
     * Returns true iff the 4 specified sides of a rectangle are inside or equal
     * to this rectangle. i.e. is this rectangle a superset of the specified
     * rectangle. An empty rectangle never contains another rectangle.
     *
     * @param left The left side of the rectangle being tested for containment
     * @param top The top of the rectangle being tested for containment
     * @param right The right side of the rectangle being tested for containment
     * @param bottom The bottom of the rectangle being tested for containment
     * @return true iff the the 4 specified sides of a rectangle are inside or
     *              equal to this rectangle
     */
    CARAPI ContainsEx(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom,
        /* [out] */ Boolean* result);

    /**
     * Returns true iff the specified rectangle r is inside or equal to this
     * rectangle. An empty rectangle never contains another rectangle.
     *
     * @param r The rectangle being tested for containment.
     * @return true iff the specified rectangle r is inside or equal to this
     *              rectangleIntersects
     */
    CARAPI ContainsEx2(
        /* [in] */ IRect* r,
        /* [out] */ Boolean* result);

    CARAPI Intersect(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom,
        /* [out] */ Boolean* result);

    CARAPI IntersectEx(
        /* [in] */ IRect* r,
        /* [out] */ Boolean* result);

    CARAPI Intersects(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom,
        /* [out] */ Boolean* result);

    CARAPI IntersectsEx(
        /* [in] */ IRect* a,
        /* [in] */ IRect* b,
        /* [out] */ Boolean* result);

    CARAPI Union(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI UnionEx(
        /* [in] */ IRect* r);

    CARAPI Scale(
        /* [in] */ Float scale);

    CARAPI GetLeft(
        /* [out] */ Int32* left);

    CARAPI SetLeft(
        /* [in] */ Int32 left);

    CARAPI GetTop(
        /* [out] */ Int32* top);

    CARAPI SetTop(
        /* [in] */ Int32 top);

    CARAPI GetRight(
        /* [out] */ Int32* right);

    CARAPI SetRight(
        /* [in] */ Int32 right);

    CARAPI GetBottom(
        /* [out] */ Int32* bottom);

    CARAPI SetBottom(
        /* [in] */ Int32 bottom);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI_(Boolean) Equals(
        /* [in] */ IRect* r);

    CARAPI_(Boolean) IsEmpty();

    CARAPI_(Int32) GetWidth();

    CARAPI_(Int32) GetHeight();

public:
    Int32 mLeft;
    Int32 mTop;
    Int32 mRight;
    Int32 mBottom;
};

#endif //__CRECT_H__
