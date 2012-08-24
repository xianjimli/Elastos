
#include "graphics/CPoint.h"
#include "ext/frameworkdef.h"
#include <elastos/AutoPtr.h>

CPoint::CPoint()
    : mX(0)
    , mY(0)
{}

ECode CPoint::Set(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    mX = x;
    mY = y;
    return NOERROR;
}

ECode CPoint::Negate()
{
    mX = -mX;
    mY = -mY;
    return NOERROR;
}

ECode CPoint::Offset(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    mX += dx;
    mY += dy;
    return NOERROR;
}

ECode CPoint::Equals(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mX == x && mY == y;
    return NOERROR;
}

ECode CPoint::EqualsEx(
    /* [in] */ IInterface* _p,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IPoint> p = (IPoint*)_p->Probe(EIID_IPoint);
    if (p != NULL) {
        *result = (mX = ((CPoint*)p.Get())->mX) &&
                  (mY = ((CPoint*)p.Get())->mY);
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode CPoint::constructor()
{
    return NOERROR;
}

ECode CPoint::constructor(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    mX = x;
    mY = y;
    return NOERROR;
}

ECode CPoint::constructor(
    /* [in] */ IPoint* p)
{
    mX = ((CPoint*)p)->mX;
    mY = ((CPoint*)p)->mY;
    return NOERROR;
}
