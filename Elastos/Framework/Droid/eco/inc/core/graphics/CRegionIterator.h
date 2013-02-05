
#ifndef __CREGIONITERATOR_H__
#define __CREGIONITERATOR_H__

#include "_CRegionIterator.h"
#include <skia/core/SkRegion.h>

struct RgnIterPair {
    SkRegion            fRgn;   // a copy of the caller's region
    SkRegion::Iterator  fIter;  // an iterator acting upon the copy (fRgn)
    
    RgnIterPair(const SkRegion& rgn) : fRgn(rgn) {
        // have our iterator reference our copy (fRgn), so we know it will be
        // unchanged for the lifetime of the iterator
        fIter.reset(fRgn);
    }
};

CarClass(CRegionIterator)
{
public:
    CARAPI next(
        /* [in] */ IRect * pR,
        /* [out] */ Boolean * pResult);

    CARAPI finalize();

    CARAPI constructor(
        /* [in] */ IRegion * pRegion);

private:
    static CARAPI_(RgnIterPair*) nativeConstructor(SkRegion* native_region);
    static CARAPI_(void) nativeDestructor(RgnIterPair* native_iter);
    static CARAPI_(Boolean) nativeNext(RgnIterPair* native_iter, IRect* r);

    RgnIterPair* mNativeIter;
};

#endif // __CREGIONITERATOR_H__
