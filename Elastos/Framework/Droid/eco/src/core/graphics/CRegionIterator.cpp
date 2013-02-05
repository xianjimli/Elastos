
#include "graphics/CRegionIterator.h"
#include "graphics/CRegion.h"
#include "graphics/CRect.h"


ECode CRegionIterator::next(
    /* [in] */ IRect * pR,
    /* [out] */ Boolean * pResult)
{
    if (pR == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    *pResult = nativeNext(mNativeIter, pR);

    return NOERROR;
}

ECode CRegionIterator::finalize()
{
    nativeDestructor(mNativeIter);

    return NOERROR;
}

ECode CRegionIterator::constructor(
    /* [in] */ IRegion * pRegion)
{
    mNativeIter = nativeConstructor(((CRegion*)pRegion)->Ni());

    return NOERROR;
}

RgnIterPair* CRegionIterator::nativeConstructor(SkRegion* native_region)
{
    SkASSERT(native_region);    
    return new RgnIterPair(*native_region);
}

void CRegionIterator::nativeDestructor(RgnIterPair* native_iter) 
{
	SkASSERT(native_iter);
	delete native_iter;
}

Boolean CRegionIterator::nativeNext(RgnIterPair* native_iter, IRect* r)
{
	// the caller has checked that rectObject is not nul
	SkASSERT(native_iter);
	SkASSERT(r);

	RgnIterPair* pair = native_iter;
	
	if (!pair->fIter.done()) {
			const SkIRect  Rect = pair->fIter.rect();
			
			((CRect*)r)->mLeft = Rect.fLeft;
			((CRect*)r)->mTop = Rect.fTop;
			((CRect*)r)->mRight = Rect.fRight;
			((CRect*)r)->mBottom = Rect.fBottom;
			
			pair->fIter.next();
			return true;
	}
	
	return false;
}
