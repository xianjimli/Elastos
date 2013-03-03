
#ifndef __CREGIONITERATOR_H__
#define __CREGIONITERATOR_H__

#include "_CRegionIterator.h"
#include <skia/core/SkRegion.h>


CarClass(CRegionIterator)
{
private:
    struct RgnIterPair {
        RgnIterPair(const SkRegion& rgn) : fRgn(rgn)
        {
            // have our iterator reference our copy (fRgn), so we know it will be
            // unchanged for the lifetime of the iterator
            fIter.reset(fRgn);
        }

        SkRegion fRgn;   // a copy of the caller's region
        SkRegion::Iterator fIter;  // an iterator acting upon the copy (fRgn)
    };

public:
    ~CRegionIterator();

    /**
     * Return the next rectangle in the region. If there are no more rectangles
     * this returns false and r is unchanged. If there is at least one more,
     * this returns true and r is set to that rectangle.
     */
    CARAPI Next(
        /* [in] */ IRect* r,
        /* [out] */ Boolean* result);

    CARAPI constructor(
        /* [in] */ IRegion* region);

private:
    static CARAPI_(RgnIterPair*) NativeConstructor(
        /* [in] */ SkRegion* nativeRegion);

    static CARAPI_(void) NativeDestructor(
        /* [in] */ RgnIterPair* nativeIter);

    static CARAPI_(Boolean) NativeNext(
        /* [in] */ RgnIterPair* nativeIter,
        /* [in] */ IRect* r);

private:
    RgnIterPair* mNativeIter;
};

#endif // __CREGIONITERATOR_H__
