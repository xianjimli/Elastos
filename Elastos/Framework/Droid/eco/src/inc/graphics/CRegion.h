
#ifndef __CREGION_H__
#define __CREGION_H__

#include "_CRegion.h"
#include <skia/core/SkRegion.h>

CarClass(CRegion)
{
public:
    /** Set the region to the empty region
     */
    CARAPI SetEmpty();

    /** Set the region to the specified region.
     */
    CARAPI Set(
        /* [in] */ IRegion* src,
        /* [out] */ Boolean* result);

    /** Set the region to the specified rectangle
     */
    CARAPI SetEx(
        /* [in] */ IRect* r,
        /* [out] */ Boolean* result);

    /** Set the region to the specified rectangle
     */
    CARAPI SetEx2(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom,
        /* [out] */ Boolean* result);

    /**
     * Set the region to the area described by the path and clip.
     * Return true if the resulting region is non-empty. This produces a region
     * that is identical to the pixels that would be drawn by the path
     * (with no antialiasing).
     */
    CARAPI SetPath(
        /* [in] */ IPath* path,
        /* [in] */ IRegion* clip,
        /* [out] */ Boolean* result);

    /**
     * Return true if this region is empty
     */
    CARAPI IsEmpty(
        /* [out] */ Boolean* isEmpty);

    /**
     * Return true if the region contains a single rectangle
     */
    CARAPI IsRect(
        /* [out] */ Boolean* isRect);

    /**
     * Return true if the region contains more than one rectangle
     */
    CARAPI IsComplex(
        /* [out] */ Boolean* isComplex);

    /**
     * Return a new Rect set to the bounds of the region. If the region is
     * empty, the Rect will be set to [0, 0, 0, 0]
     */
    CARAPI GetBounds(
        /* [out] */ IRect** bounds);

    /**
     * Set the Rect to the bounds of the region. If the region is empty, the
     * Rect will be set to [0, 0, 0, 0]
     */
    CARAPI GetBoundsEx(
        /* [in, out] */ IRect* r,
        /* [out] */ Boolean* result);

    /**
     * Return the boundary of the region as a new Path. If the region is empty,
     * the path will also be empty.
     */
    CARAPI GetBoundaryPath(
        /* [out] */ IPath** path);

    /**
     * Set the path to the boundary of the region. If the region is empty, the
     * path will also be empty.
     */
    CARAPI GetBoundaryPathEx(
        /* [in, out] */ IPath* path,
        /* [out] */ Boolean* result);

    /**
     * Return true if the region contains the specified point
     */
    CARAPI Contains(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [out] */ Boolean* result);

    /**
     * Return true if the region is a single rectangle (not complex) and it
     * contains the specified rectangle. Returning false is not a guarantee
     * that the rectangle is not contained by this region, but return true is a
     * guarantee that the rectangle is contained by this region.
     */
    CARAPI QuickContains(
        /* [in] */ IRect* r,
        /* [out] */ Boolean* result);

    /**
     * Return true if the region is a single rectangle (not complex) and it
     * contains the specified rectangle. Returning false is not a guarantee
     * that the rectangle is not contained by this region, but return true is a
     * guarantee that the rectangle is contained by this region.
     */
    CARAPI QuickContainsEx(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom,
        /* [out] */ Boolean* result);

    /**
     * Return true if the region is empty, or if the specified rectangle does
     * not intersect the region. Returning false is not a guarantee that they
     * intersect, but returning true is a guarantee that they do not.
     */
    CARAPI QuickReject(
        /* [in] */ IRect* r,
        /* [out] */ Boolean* result);

    /**
     * Return true if the region is empty, or if the specified rectangle does
     * not intersect the region. Returning false is not a guarantee that they
     * intersect, but returning true is a guarantee that they do not.
     */
    CARAPI QuickRejectEx(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom,
        /* [out] */ Boolean* result);

    /**
     * Return true if the region is empty, or if the specified region does not
     * intersect the region. Returning false is not a guarantee that they
     * intersect, but returning true is a guarantee that they do not.
     */
    CARAPI QuickRejectEx2(
        /* [in] */ IRegion* rgn,
        /* [out] */ Boolean* result);

    /**
     * Translate the region by [dx, dy]. If the region is empty, do nothing.
     */
    CARAPI Translate(
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy);

    /**
     * Set the dst region to the result of translating this region by [dx, dy].
     * If this region is empty, then dst will be set to empty.
     */
    CARAPI TranslateEx(
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy,
        /* [in] */ IRegion* dst);

    /**
     * Scale the region by the given scale amount. This re-constructs new region by
     * scaling the rects that this region consists of. New rectis are computed by scaling
     * coordinates by float, then rounded by roundf() function to integers. This may results
     * in less internal rects if 0 < scale < 1. Zero and Negative scale result in
     * an empty region. If this region is empty, do nothing.
     *
     * @hide
     */
    CARAPI Scale(
        /* [in] */ Float scale);

    /**
     * Set the dst region to the result of scaling this region by the given scale amount.
     * If this region is empty, then dst will be set to empty.
     * @hide
     */
    CARAPI ScaleEx(
        /* [in] */ Float scale,
        /* [in] */ IRegion* dst);

    CARAPI Union(
        /* [in] */ IRect* r,
        /* [out] */ Boolean* result);

    /**
     * Perform the specified Op on this region and the specified rect. Return
     * true if the result of the op is not empty.
     */
    CARAPI Op(
        /* [in] */ IRect* r,
        /* [in] */ RegionOp op,
        /* [out] */ Boolean* result);

    /**
     * Perform the specified Op on this region and the specified rect. Return
     * true if the result of the op is not empty.
     */
    CARAPI OpEx(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom,
        /* [in] */ RegionOp op,
        /* [out] */ Boolean* result);

    /**
     * Perform the specified Op on this region and the specified region. Return
     * true if the result of the op is not empty.
     */
    CARAPI OpEx2(
        /* [in] */ IRegion* region,
        /* [in] */ RegionOp op,
        /* [out] */ Boolean* result);

    /**
     * Set this region to the result of performing the Op on the specified rect
     * and region. Return true if the result is not empty.
     */
    CARAPI OpEx3(
        /* [in] */ IRect* rect,
        /* [in] */ IRegion* region,
        /* [in] */ RegionOp op,
        /* [out] */ Boolean* result);

    /**
     * Set this region to the result of performing the Op on the specified
     * regions. Return true if the result is not empty.
     */
    CARAPI OpEx4(
        /* [in] */ IRegion* region1,
        /* [in] */ IRegion* region2,
        /* [in] */ RegionOp op,
        /* [out] */ Boolean* result);

    CARAPI Equals(
        /* [in] */ IRegion* r,
        /* [out] */ Boolean* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IRegion* region);

    CARAPI constructor(
        /* [in] */ IRect* r);

    CARAPI constructor(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI constructor(
        /* [in] */ Int32 ni);

public:
    /*package*/ CARAPI_(SkRegion*) Ni();

private:
    SkRegion* mNativeRegion;
};

#endif //__CREGION_H__
