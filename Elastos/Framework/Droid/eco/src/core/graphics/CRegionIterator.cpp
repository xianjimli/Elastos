
#include "graphics/CRegionIterator.h"
#include "graphics/CRegion.h"
#include "graphics/CRect.h"
#include "graphics/Graphics.h"


CRegionIterator::~CRegionIterator()
{
    NativeDestructor(mNativeIter);
}

ECode CRegionIterator::constructor(
    /* [in] */ IRegion* region)
{
    mNativeIter = NativeConstructor(((CRegion*)region)->Ni());
    return NOERROR;
}

ECode CRegionIterator::Next(
    /* [in] */ IRect* r,
    /* [out] */ Boolean* result)
{
    if (r == NULL) {
        // throw new NullPointerException("The Rect must be provided");
        return E_NULL_POINTER_EXCEPTION;
    }
    *result = NativeNext(mNativeIter, r);
    return NOERROR;
}

CRegionIterator::RgnIterPair* CRegionIterator::NativeConstructor(
    /* [in] */ SkRegion* nativeRegion)
{
    SkASSERT(nativeRegion);
    return new RgnIterPair(*nativeRegion);
}

void CRegionIterator::NativeDestructor(
    /* [in] */ RgnIterPair* nativeIter)
{
    SkASSERT(nativeIter);
    delete nativeIter;
}

Boolean CRegionIterator::NativeNext(
    /* [in] */ RgnIterPair* nativeIter,
    /* [in] */ IRect* r)
{
    // the caller has checked that rectObject is not nul
    SkASSERT(nativeIter);
    SkASSERT(r);

    RgnIterPair* pair = nativeIter;
    if (!pair->fIter.done()) {
        Graphics::SkIRect2IRect(pair->fIter.rect(), r);
        pair->fIter.next();
        return TRUE;
    }
    return FALSE;
}
